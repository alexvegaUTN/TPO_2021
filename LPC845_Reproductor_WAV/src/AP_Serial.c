/*******************************************************************************************************************************//**
 *
 * @file		AP_Serial.c
 * @brief		Descripcion del modulo
 * @date		Jan 09, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "AP_Serial.h"
#include "PR_USART.h"

#include "PR_MMC_SD.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define		START_BYTE				('$')
#define		STOP_BYTE				('#')

#define		LONG_MAX_CMD			(32)
#define 	FILE_MAX_PACKET_SIZE	(128)

#define size_riff 		4
#define size_wave		4
#define size_fmt		4
#define size_data_sc	4
#define _RIFF		(dato_rx == 'R' || dato_rx == 'I' || dato_rx == 'F')
#define _WAVE		(dato_rx == 'W' || dato_rx == 'A' || dato_rx == 'V' || dato_rx == 'E')
#define _fmt		(dato_rx == 'f' || dato_rx == 'm' || dato_rx == 't' || dato_rx == ' ')
#define _data		(dato_rx == 'd' || dato_rx == 'a' || dato_rx == 't' || dato_rx == 'a')


/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO - Enumeraciones
 **********************************************************************************************************************************/
typedef enum
{
	false = 0,
	true
}flag_state_e;

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO - Estructuras
 **********************************************************************************************************************************/
typedef struct
{
	char * cmd_play;
	char * cmd_pause;
	char * cmd_stop;
}commands_rx_t;

typedef struct
{
	char * resp_play;
	char * resp_pause;
	char * resp_stop;
}response_tx_t;

typedef union{
	uint8_t array[sizeof(uint32_t)];
	uint32_t dword;
}buffer_uint32_union_t;

typedef union{
	uint8_t array[sizeof(uint16_t)];
	uint16_t word;
}buffer_uint16_union_t;

//*****ACA DEFINIR STRUCT FMT Y RIFF PARA PODER GUARDAR LOS DATOS QUE ME LLEGAN, EN LA MAQUINA ESTADOS,
//*****TAMBIEN SUBCHUNK2DSIZE, ETC........*******

typedef struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
	char	        RIFF[5];        // RIFF Header Magic header   uint8_t
    uint32_t        ChunkSize;      // RIFF Chunk Size
    char 	        WAVE[5];        // WAVE Header
    /* "fmt" sub-chunk */
    char	        fmt[5];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    char	        Subchunk2ID[5]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr_t;

typedef struct {
    uint16_t 	dataLenght;								// Longitud del paquete de Data
    uint16_t 	blockNumber;							// Número de bloque de datos de Firmware.
    uint8_t 	endPacket;								// Indica si es el bloque final de envío del FW: 0x00 = Continúan mas paquetes, 0x01 = Último paquete
    uint8_t 	lenDataPacket;							// Largo de datos del paquete. Máximo 128 bytes.
    uint8_t 	dataPacket[FILE_MAX_PACKET_SIZE];       // Paquete de datos del archivo WAV.
}send_file_packet_t;

/****************IMPORTANT**********************************************************************************
 * ----> The length of strings RIFF,WAVE,fmt and SubCHunk2ID is 5 because needs null in the final of himself */


/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
wav_hdr_t WAV_FILE;
send_file_packet_t file_packet;

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federico Speroni
 	\date Nov 21, 2021
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/


void Serial_Analizar_Trama_v2(const uint8_t usart_instance)
{
	typedef enum _estados_UART0 {
		BYTE_START = 0,
		TIPO,
		AUDIO_DATA,
		COMANDO,
		BYTE_STOP
	} estados_UART0_e;

	typedef enum _packet_types {
		__notDefined = 0,
		__RIFF = 'R',
		__fmtSubchunk = 'f',
		__dataSubchunk = 'd',
		__DATA = 'D',
		__command = 'C'
	}packet_types_t;

	typedef enum _estados_riff{
		__ChunkID = 0,
		__ChunkSize,
		__Format
	}estados_riff_t;

	typedef enum _estados_fmt {
		__Subchunk1ID = 0,
		__Subchunk1Size,
		__AudioFormat,
		__NumOfChan,
		__SamplesPerSec,
		__bytesPerSec,
		__blockAlign,
		__bitsPerSample
	} estados_fmt_t;

	typedef enum _estados_data {
		__Subchunk2ID = 0,
		__Subchunk2Size,
	} estados_data_t;

	typedef enum _estados_file_packet {
		__blockNumber = 0,
		__endPacket,
		__lenDataPacket,
		__Packet,
	}estados_file_packet_t;

	static const uint8_t dbg_inst = 0;

	static estados_UART0_e estado = BYTE_START;
	static packet_types_t packet_type = __notDefined;
	static estados_riff_t estado_riff = __ChunkID;
	static estados_fmt_t estado_fmt = __Subchunk1ID ;
	static estados_data_t estado_data = __Subchunk2ID;
	static estados_file_packet_t estado_file_packet = __blockNumber;

	static char comando[LONG_MAX_CMD];
	static uint32_t i = 0;

	static buffer_uint32_union_t aux_uint32;
	static buffer_uint16_union_t aux_uint16;

	static uint8_t dbg_buffer[136];
	static uint8_t idx = 0;

	int32_t dato_rx = usart_pop_RX(&usart_ctx[usart_instance]);

	if (dato_rx < 0)
		return;

	dato_rx = (uint8_t)dato_rx;

	switch (estado) {
		case BYTE_START:
			if (dato_rx == START_BYTE) {
				dbg_buffer[idx++] = (uint8_t)dato_rx;

				static uint8_t a = 0;

				if (a == 0) {
					__asm volatile ("nop");		// RIFF
					a++;
				}
				if (a == 1) {
					__asm volatile ("nop");		// fmt
					a++;
				}
				if (a == 2) {
					__asm volatile ("nop");		// dataSubChunk2
					a++;
				}
				if (a == 3) {
					__asm volatile ("nop");		// DATA
					a = 0;
				}


				estado = TIPO;
			}
			break;

		case TIPO:
			if (dato_rx == 'R' || dato_rx == 'f' || dato_rx == 'd' || dato_rx == 'D' || dato_rx == 'C') {

				switch (dato_rx) {

					case __RIFF:
						packet_type = __RIFF;
						dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __fmtSubchunk:
						packet_type = __fmtSubchunk;
						dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __dataSubchunk:
						packet_type = __dataSubchunk;
						dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __DATA:
						packet_type = __DATA;
						dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __command:
						dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = COMANDO;
						break;

					default:
						estado = BYTE_START;
						break;
				}
			}

			break;

		case AUDIO_DATA:

			switch (packet_type) {
				case __RIFF:

					/* ******************************* RIFF ************************************** */

					switch (estado_riff) {
						case __ChunkID:
							if (_RIFF && (i < size_riff)) {
								WAV_FILE.RIFF[i++] = (uint8_t)dato_rx;

								if (i == size_riff) {
									WAV_FILE.RIFF[i] = '\0';
									memcpy(&dbg_buffer[idx], WAV_FILE.RIFF, size_riff);
									idx += size_riff;
									i = 0;

									estado_riff = __ChunkSize;
								}
							}

							break;

						case __ChunkSize:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.ChunkSize) {
								WAV_FILE.ChunkSize = aux_uint32.dword;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.ChunkSize, sizeof WAV_FILE.ChunkSize);
								idx += sizeof WAV_FILE.ChunkSize;
								i = 0;

								estado_riff = __Format;
							}

							break;

						case __Format:
							if (_WAVE && (i < size_wave)) {
								WAV_FILE.WAVE[i++] = (uint8_t)dato_rx;

								if (i == size_wave) {
									WAV_FILE.WAVE[i] = '\0';
									memcpy(&dbg_buffer[idx], WAV_FILE.WAVE, size_wave);
									idx += size_wave;
									i = 0;

									estado_riff = __ChunkID;
									estado = BYTE_STOP;
								}
							}

							break;

						default:
							estado_riff = __ChunkID;
							break;
					}

					break;

				case __fmtSubchunk:
					/* ***************************** fmt ********************************** */
					switch (estado_fmt) {

						case __Subchunk1ID:
							if (_fmt && (i < size_fmt)) { 							//size_fmt is 4, "fmt ";
								WAV_FILE.fmt[i++] = (uint8_t)dato_rx;

								if (i == size_fmt) {
									WAV_FILE.fmt[i] = '\0';
									memcpy(&dbg_buffer[idx], WAV_FILE.fmt, size_fmt);
									idx += size_fmt;
									i = 0;

									estado_fmt = __Subchunk1Size;
								}
							}
							break;

						case __Subchunk1Size:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.Subchunk1Size) {
								WAV_FILE.Subchunk1Size = aux_uint32.dword;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.Subchunk1Size, sizeof WAV_FILE.Subchunk1Size);
								idx += sizeof WAV_FILE.Subchunk1Size;
								i = 0;

								estado_fmt = __AudioFormat;
							}

							break;

						case __AudioFormat:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.AudioFormat) {
								WAV_FILE.AudioFormat = aux_uint16.word;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.AudioFormat, sizeof WAV_FILE.AudioFormat);
								idx += sizeof WAV_FILE.AudioFormat;
								i = 0;

								estado_fmt = __NumOfChan;
							}

							break;

						case __NumOfChan:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.NumOfChan) {
								WAV_FILE.NumOfChan = aux_uint16.word;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.NumOfChan, sizeof WAV_FILE.NumOfChan);
								idx += sizeof WAV_FILE.NumOfChan;
								i = 0;

								estado_fmt = __SamplesPerSec;
							}

							break;

						case __SamplesPerSec:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.SamplesPerSec) {
								WAV_FILE.SamplesPerSec = aux_uint32.dword;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.SamplesPerSec, sizeof WAV_FILE.SamplesPerSec);
								idx += sizeof WAV_FILE.SamplesPerSec;
								i = 0;

								estado_fmt = __bytesPerSec;
							}

							break;

						case __bytesPerSec:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.bytesPerSec) {
								WAV_FILE.bytesPerSec = aux_uint32.dword;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.bytesPerSec, sizeof WAV_FILE.bytesPerSec);
								idx += sizeof WAV_FILE.bytesPerSec;
								i = 0;

								estado_fmt = __blockAlign;
							}

							break;

						case __blockAlign:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.blockAlign) {
								WAV_FILE.blockAlign = aux_uint16.word;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.blockAlign, sizeof WAV_FILE.blockAlign);
								idx += sizeof WAV_FILE.blockAlign;
								i = 0;

								estado_fmt = __bitsPerSample;
							}

							break;

						case __bitsPerSample:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.bitsPerSample) {
								WAV_FILE.bitsPerSample = aux_uint16.word;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.bitsPerSample, sizeof WAV_FILE.bitsPerSample);
								idx += sizeof WAV_FILE.bitsPerSample;
								i = 0;

								estado_fmt = __Subchunk1ID;
								estado = BYTE_STOP;
							}

							break;

						default:
							estado_fmt = __Subchunk1ID;
							break;
					}

					break;

				/* ******************************* DATA ********************************** */

				case __dataSubchunk:
					/* ***** data_subchunk2 ***** */
					switch (estado_data) {

						case __Subchunk2ID:
							if (_data && (i < size_data_sc))
							{
								WAV_FILE.Subchunk2ID[i++] = (uint8_t)dato_rx;

								if (i == size_data_sc) {
									WAV_FILE.Subchunk2ID[i] = '\0';
									memcpy(&dbg_buffer[idx], WAV_FILE.Subchunk2ID, size_data_sc);
									idx += size_data_sc;
									i = 0;

									estado_data = __Subchunk2Size;
								}
							}

							break;

						case __Subchunk2Size:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.Subchunk2Size) {
								WAV_FILE.Subchunk2Size = aux_uint32.dword;
								memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.Subchunk2Size, sizeof WAV_FILE.Subchunk2Size);
								idx += sizeof WAV_FILE.Subchunk2Size;
								i = 0;

								estado = BYTE_STOP;
								estado_data = __Subchunk2ID;
								break;
							}

							break;

						default:
							estado_data = __Subchunk2ID;
							break;
					}

					break;

				case __DATA:
					/* ***** data_file_packet ***** */
					switch (estado_file_packet) {

					case __blockNumber:
						aux_uint16.array[i++] = (uint8_t)dato_rx;

						if (i == sizeof file_packet.blockNumber) {
							file_packet.blockNumber = aux_uint16.word;
							memcpy(&dbg_buffer[idx], (char *)&file_packet.blockNumber, sizeof file_packet.blockNumber);
							idx += sizeof file_packet.blockNumber;
							i = 0;

							estado_file_packet= __endPacket;
						}

						break;

					case __endPacket:
						dbg_buffer[idx++] = file_packet.endPacket = (uint8_t)dato_rx;
						estado_file_packet = __lenDataPacket;
						break;

					case __lenDataPacket:
						dbg_buffer[idx++] = file_packet.lenDataPacket = (uint8_t)dato_rx;
						estado_file_packet = __Packet;

						break;

					case __Packet:
						file_packet.dataPacket[i++] = (uint8_t)dato_rx;

						if (i == file_packet.lenDataPacket) {
							memcpy(&dbg_buffer[idx], file_packet.dataPacket, file_packet.lenDataPacket);
							idx += file_packet.lenDataPacket;
							i = 0;
							estado_file_packet = __blockNumber;

							estado = BYTE_STOP;
						}
						break;

					default:
						estado_file_packet = __blockNumber;
						break;
					}

					break;

				default:
					packet_type = __notDefined;
					break;
			}

			break;

		case COMANDO:
			if (dato_rx != STOP_BYTE) {
				comando[i++] = dato_rx;

			}
			else {
				comando[i] = '\0';
				estado = BYTE_STOP;
			}

			break;

		case BYTE_STOP:
			if (dato_rx == STOP_BYTE) {
				dbg_buffer[idx++] = (uint8_t)dato_rx;

				usart_send_data(&usart_ctx[dbg_inst], dbg_buffer, idx);

				i = 0;
				idx = 0;
				estado = BYTE_START;
				packet_type = __notDefined;
				estado_riff = __ChunkID;
				estado_fmt = __Subchunk1ID;
				estado_data = __Subchunk2ID;
				estado_file_packet = __blockNumber;

				Serial_Analizar_Comando(usart_instance, comando);
			}

			break;

		default:
			estado = BYTE_START;
			break;
	}
}

#if 0
void Serial_Analizar_Trama(const uint8_t usart_inst)
{
	typedef enum _estados_UART0 {
		BYTE_START = 0,
		TIPO,
		AUDIO_DATA,
		COMANDO,
		BYTE_STOP
	} estados_UART0_e;

	typedef enum _estados_fmt {
		__Subchunk1Size = 0,
		__AudioFormat,
		__NumOfChan,
		__SamplesPerSec,
		__bytesPerSec,
		__blockAlign,
		__bitsPerSample
	} estados_fmt;

	typedef enum _estados_data {
		__blockNumber = 0,
		__endPacket,
		__lenDataPacket,
		__Packet,
	} estados_data_t;

	const uint8_t dbg_inst = 0;

	static estados_UART0_e estado = BYTE_START;
	static estados_fmt estado_fmt = __Subchunk1Size;
	static estados_data_t estado_data = __blockNumber;
	static char comando[LONG_MAX_CMD];
	static uint32_t i,j,k,z,x;
	static uint8_t idx_aux = 0;
	static uint8_t flag_riff, flag_wave, flag_fmt, flag_data_sc, flag_data_sc2, flag_data_sc3, flag_sound_data, flag_data_sd, flag_end_data, packet_number;
	static uint32_t* sound_data;

	static buffer_uint32_union_t aux_uint32;
	static buffer_uint16_union_t aux_uint16;

	int32_t dato_rx = usart_pop_RX(&usart_ctx[usart_inst]);

	if (dato_rx < 0)
		return;

	switch (estado) {
		case BYTE_START:
			if (dato_rx == START_BYTE) {

				//usart_send_data(&usart_ctx[dbg_inst], (uint8_t *)&dato_rx, sizeof (uint8_t));

				static uint8_t a = 0;

				if (a == 0)
					__asm volatile ("nop");
				if (a == 1)
					__asm volatile ("nop");
				if (a == 2)
					__asm volatile ("nop");
				if (a == 3)
					__asm volatile ("nop");

				a++;

				estado = TIPO;
			}

			break;

		case TIPO:
			if (dato_rx != STOP_BYTE)
			{
				//usart_send_data(&usart_ctx[dbg_inst], (uint8_t *)&dato_rx, sizeof (uint8_t));

				static uint8_t d = 0;

				if (dato_rx == 'R' || dato_rx == 'f' || dato_rx == 'd') {

					if (dato_rx == 'R') {
						__asm volatile ("nop");
					}

					if (dato_rx == 'f') {
						__asm volatile ("nop");
					}

					if (dato_rx == 'd') {
						if (d == 0)
							__asm volatile ("nop");
						if (d == 1)
							__asm volatile ("nop");

						d++;
					}

					estado = AUDIO_DATA;
				}

				if (dato_rx == 'C') {
					//usart_send_data(&usart_ctx[dbg_inst], (uint8_t *)&dato_rx, sizeof (uint8_t));

					estado = COMANDO;
				}
			}
			break;

		case AUDIO_DATA:
			/*******************RIFF*******************/
			if (_RIFF && (i < size_riff))
			{
				WAV_FILE.RIFF[i] = dato_rx;
				i++;
				estado = AUDIO_DATA;
				if (i == size_riff)
				{
					WAV_FILE.RIFF[i] = '\0';

					//usart_send_data(&usart_ctx[dbg_inst], WAV_FILE.RIFF, sizeof WAV_FILE.RIFF / sizeof WAV_FILE.RIFF[0]);

					flag_riff = true;
				}
				break;
			}

			if (!strcmp(WAV_FILE.RIFF,"RIFF") && flag_riff) //compare, if its "RIFF" in WAVFILE.. continue...
			{
				aux_uint32.array[idx_aux++] = dato_rx;

				if (idx_aux == sizeof(uint32_t)) {
					WAV_FILE.ChunkSize = aux_uint32.dword;
					//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.ChunkSize, sizeof WAV_FILE.ChunkSize);

					estado = AUDIO_DATA;
					idx_aux = 0;
					flag_riff = false;
				}

				break;
			}
			else if(strcmp(WAV_FILE.RIFF,"RIFF"))
			{
				estado = BYTE_START;
				break;
			}

			/*******************WAVE*******************/

			if (_WAVE && !flag_riff && j < size_wave)
			{
				WAV_FILE.WAVE[j] = dato_rx;
				j++;
				estado = AUDIO_DATA;

				if (j == size_wave)
				{
					WAV_FILE.WAVE[j] = '\0';
					//usart_send_data(&usart_ctx[dbg_inst], WAV_FILE.WAVE, sizeof WAV_FILE.WAVE / sizeof WAV_FILE.WAVE[0]);

					flag_wave = true;
					estado = BYTE_STOP;
				}
				break;
			}

			if (!strcmp(WAV_FILE.WAVE,"WAVE") && flag_wave)			// simplificar
				flag_wave = true;
			else
			{
				flag_wave = false;
				estado = BYTE_START;
				break;
			}

			/*******************fmt*******************/

			if (_fmt && flag_wave && k < size_fmt) //size_fmt is 4, "fmt ";
			{
				WAV_FILE.fmt[k] = dato_rx;
				k++;
				estado = AUDIO_DATA;

				if (k == size_fmt)
				{
					WAV_FILE.fmt[k] = '\0';
					//usart_send_data(&usart_ctx[dbg_inst], WAV_FILE.fmt, sizeof WAV_FILE.fmt / sizeof WAV_FILE.fmt[0]);

					flag_fmt = true;
				}
				break;
			}

			//if (!strcmp(WAV_FILE.fmt,"fmt "))
				//flag_fmt = true;
			//else
			if (strcmp(WAV_FILE.fmt,"fmt ")) {
				flag_fmt = false;
				estado = BYTE_START;
				break;
			}

			if (flag_fmt)
			{
				switch (estado_fmt)
				{
					case __Subchunk1Size:

						aux_uint32.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint32_t)) {
							WAV_FILE.Subchunk1Size = aux_uint32.dword;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.Subchunk1Size, sizeof WAV_FILE.Subchunk1Size);

							idx_aux = 0;
							estado_fmt = __AudioFormat;
						}

					break;

					case __AudioFormat:

						aux_uint16.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint16_t)) {
							WAV_FILE.AudioFormat = aux_uint16.word;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.AudioFormat, sizeof WAV_FILE.AudioFormat);

							idx_aux = 0;
							estado_fmt = __NumOfChan;
						}

					break;

					case __NumOfChan:

						aux_uint16.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint16_t)) {
							WAV_FILE.NumOfChan = aux_uint16.word;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.NumOfChan, sizeof WAV_FILE.NumOfChan);

							idx_aux = 0;
							estado_fmt = __SamplesPerSec;
						}

					break;

					case __SamplesPerSec:

						aux_uint32.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint32_t)) {
							WAV_FILE.SamplesPerSec = aux_uint32.dword;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.SamplesPerSec, sizeof WAV_FILE.SamplesPerSec);

							idx_aux = 0;
							estado_fmt = __bytesPerSec;
						}

					break;

					case __bytesPerSec:

						aux_uint32.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint32_t)) {
							WAV_FILE.bytesPerSec = aux_uint32.dword;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.bytesPerSec, sizeof WAV_FILE.bytesPerSec);

							idx_aux = 0;
							estado_fmt = __blockAlign;
						}

					break;

					case __blockAlign:

						aux_uint16.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint16_t)) {
							WAV_FILE.blockAlign = aux_uint16.word;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.blockAlign, sizeof WAV_FILE.blockAlign);

							idx_aux = 0;
							estado_fmt = __bitsPerSample;
						}

					break;

					case __bitsPerSample:

						aux_uint16.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint16_t)) {
							WAV_FILE.bitsPerSample = aux_uint16.word;
							//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.bitsPerSample, sizeof WAV_FILE.bitsPerSample);

							idx_aux = 0;
							estado_fmt++;
						}

					break;
				}

				if (estado_fmt <= __bitsPerSample) {
					estado = AUDIO_DATA;
					break;
				}
				else {
					flag_fmt = false;
					estado_fmt = __Subchunk1Size;
					flag_data_sc = true;
					estado = BYTE_STOP;
					break;
				}
			}

			/*******************DATA*******************/
			/******************data_sc*****************/

			if (_data && flag_data_sc)
			{
				if (_data && z < size_data_sc)
				{
					WAV_FILE.Subchunk2ID[z++] = dato_rx;

					if (z == size_data_sc) {
						WAV_FILE.Subchunk2ID[z] = '\0';
						//usart_send_data(&usart_ctx[dbg_inst], WAV_FILE.Subchunk2ID, sizeof WAV_FILE.Subchunk2ID / sizeof WAV_FILE.Subchunk2ID[0]);

						flag_data_sc2 = true;
					}
					estado = AUDIO_DATA;
					break;
				}

				if (!strcmp(WAV_FILE.Subchunk2ID,"data") && flag_data_sc2)
					flag_data_sc2 = true;
				else
				{
					flag_data_sc2 = false;
					estado = BYTE_START;
					break;
				}

/*				if (packet_number == -1)
				{
					packet_number = dato_rx;
					if (packet_number == 0)
					{
						flag_data_sc2 = true;
						estado = AUDIO_DATA;
						break;
					}
					else //protection
					{
						estado = BYTE_START;
						break;
					}
				}

				if (flag_data_sc2)
				{
					if (_data && z < size_data_sc)
					{
						WAV_FILE.Subchunk2ID[z] = dato_rx;
						z++;
						if (z == size_data_sc)
						{
							WAV_FILE.Subchunk2ID[z] = '\0';
							flag_data_sc3 = true;
						}
						estado = AUDIO_DATA;
						break;
					}

					if (flag_data_sc3)
					{
						WAV_FILE.Subchunk2Size = dato_rx;
						sound_data = sizeof(WAV_FILE.Subchunk2Size);
						flag_data_sc = false;
						flag_data_sc2 = false;
						flag_data_sc3 = false;
						flag_sound_data = true;
						packet_number = -1;
						estado = AUDIO_DATA;
						break;
					}
				}*/
			}

			if (flag_data_sc2 == true) {

				aux_uint32.array[idx_aux++] = dato_rx;

				if (idx_aux == sizeof(uint32_t)) {
					WAV_FILE.Subchunk2Size = aux_uint32.dword;
					//usart_send_data(&usart_ctx[dbg_inst], &WAV_FILE.Subchunk2Size, sizeof WAV_FILE.Subchunk2Size);

					flag_data_sc = false;
					flag_data_sc2 = false;
					flag_data_sc3 = false;
					flag_sound_data = true;
					idx_aux = 0;

					estado = BYTE_STOP;
					break;
				}
			}
			/****************SOUND DATA******************/

			if (flag_sound_data)
			{
				// filePacket.blockNumber + filePacket.endPacket + filePacket.lenDataPacket + Paquete

			    //uint16_t 	dataLenght;								// Longitud del paquete de Data
			    //uint16_t 	blockNumber;							// Número de bloque de datos de Firmware.
			    //uint8_t 	endPacket;								// Indica si es el bloque final de envío del FW: 0x00 = Continúan mas paquetes, 0x01 = Último paquete
			    //uint8_t 	lenDataPacket;							// Largo de datos del paquete. Máximo 128 bytes.
			    //uint8_t 	dataPacket[FILE_MAX_PACKET_SIZE];       // Paquete de datos del archivo WAV.

				switch (estado_data) {
					case __blockNumber:
						aux_uint16.array[idx_aux++] = dato_rx;

						if (idx_aux == sizeof(uint16_t)) {
							file_packet.blockNumber = aux_uint16.word;
							idx_aux = 0;

							estado_data = __endPacket;
						}

						estado_data = __blockNumber;

						break;

					case __endPacket:
						file_packet.endPacket = dato_rx;
						estado_data = __lenDataPacket;

						break;

					case __lenDataPacket:
						file_packet.lenDataPacket = dato_rx;
						estado_data = __Packet;

						break;

					case __Packet:
						file_packet.dataPacket[idx_aux++] = dato_rx;

						if (idx_aux == FILE_MAX_PACKET_SIZE) {
							idx_aux = 0;
							estado_data = __blockNumber;

							estado = BYTE_STOP;
						}

						break;

					default:
						break;
				}


/*				if (packet_number == -1)
				{
					packet_number = dato_rx;
					if (packet_number == 1)
					{
						flag_data_sd = true;
						estado = AUDIO_DATA;
						break;
					}
					else //protection
					{
						estado = BYTE_START;
						break;
					}
				}*/

/*				if (flag_data_sd)
				{
					if (packet_number < WAV_FILE.Subchunk2Size/128) //this division is the num of packets
					{
						if (x < 128/sizeof(dato_rx)) //if x < 32, luego 32x4 = 128 -> bytes cargados
						{
							sound_data[x + (packet_number-1)*(128/sizeof(dato_rx))] = dato_rx;
							x++;
							estado = AUDIO_DATA;
							break;
						}
						else 				//cuando pasen los 128 bytes
						{
							x = 0;
							packet_number = dato_rx; // en la sig pasada recibo el sig packet_number
							estado = AUDIO_DATA;
							break;
						}
					}
					else
					{
						flag_data_sd = false;
						flag_sound_data = false;
						flag_end_data = true;
						estado = BYTE_STOP;
						break;
					}
				}*/
			}

		break;

		/******************COMANDO**********************/

		case COMANDO:
			if (dato_rx != STOP_BYTE) {
				comando[i] = dato_rx;
				i++;
				estado = COMANDO;
				break;
			}
			else {
				comando[i] = '\0';
				estado = BYTE_STOP;
			}
		break;

		case BYTE_STOP:
			if (dato_rx == STOP_BYTE) {
				i = 0;

				static uint8_t b = 0;

				if (b == 0)
					__asm volatile ("nop");
				if (b == 1)
					__asm volatile ("nop");
				if (b == 2)
					__asm volatile ("nop");
				if (b == 3)
					__asm volatile ("nop");

				b++;

				estado = BYTE_START;

				Serial_Analizar_Comando(usart_inst, comando);
			}
		break;

		default:

		break;
	}
}
#endif


void Serial_Analizar_Comando(const uint8_t usart_inst, const char comando[])
{
	static const commands_rx_t commands = {
			.cmd_play = "PLAY",
			.cmd_pause = "PAUSA",
			.cmd_stop = "STOP"
	};

	static const response_tx_t responses = {
				.resp_play = "#PLAY_OK$",
				.resp_pause = "#PAUSA_OK$",
				.resp_stop = "#STOP_OK$"
		};

	if (!strcmp(comando, commands.cmd_play)) {
		//LED_ROJO_OFF;
		//LED_AZUL_OFF;
		//LED_VERDE_ON;

		usart_send_data(&usart_ctx[usart_inst], responses.resp_play, strlen(responses.resp_play));
	}
	else if (!strcmp(comando, commands.cmd_pause)) {
		//LED_VERDE_OFF;
		//LED_ROJO_OFF;
		//LED_AZUL_ON;

		usart_send_data(&usart_ctx[usart_inst], responses.resp_pause, strlen(responses.resp_pause));
	}
	else if (!strcmp(comando, commands.cmd_stop)) {
		//LED_AZUL_OFF;
		//LED_VERDE_OFF;
		//LED_ROJO_ON;

		usart_send_data(&usart_ctx[usart_inst], responses.resp_stop, strlen(responses.resp_stop));
	}
	else {

	}
}


/* ------------------------------------------------------------------------------- */
/*
void Serial_Analizar_Trama(const uint8_t usart_inst)
{
	typedef enum _estados_UART0 {
		BYTE_START = 0,
		DATA,
		BYTE_STOP
	} estados_UART0_e;

	static estados_UART0_e estado = BYTE_START;
	static char comando[LONG_MAX_CMD];
	static uint32_t i = 0;

	int32_t dato_rx = usart_pop_RX(&usart_ctx[usart_inst]);

	if (dato_rx < 0)
		return;

	switch (estado) {
		case BYTE_START:
			if (dato_rx == START_BYTE) {
				estado = DATA;
			}

			break;

		case DATA:
			if (dato_rx != STOP_BYTE) {
				comando[i++] = dato_rx;
				estado = DATA;
				break;
			}
			else {
				comando[i] = '\0';
				estado = BYTE_STOP;
			}

		case BYTE_STOP:
			if (dato_rx == STOP_BYTE) {
				i = 0;
				estado = BYTE_START;

				Serial_Analizar_Comando(usart_inst, comando);
			}
			break;

		default:
			break;
	}
}
*/

