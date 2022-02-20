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


#define		LONG_MAX_RESP			(16)


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

typedef enum _estados_UART0 {
	BYTE_START = 0,
	TIPO,
	AUDIO_DATA,
	COMANDO,
	BYTE_STOP
} estados_UART0_e;

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


/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO - Estructuras
 **********************************************************************************************************************************/

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

/****************IMPORTANT**********************************************************************************
 * ----> The length of strings RIFF, WAVE, fmt and data is 5 because needs null in the final position */

typedef struct
{
	char cmd_play[LONG_MAX_CMD];
	char cmd_pause[LONG_MAX_CMD];
	char cmd_stop[LONG_MAX_CMD];
}commands_rx_t;

typedef struct
{
	char resp_play[LONG_MAX_RESP];
	char resp_pause[LONG_MAX_RESP];
	char resp_stop[LONG_MAX_RESP];
}response_cmd_tx_t;

typedef struct
{
	char resp_RIFF[LONG_MAX_RESP];
	char resp_fmt_subchunk[LONG_MAX_RESP];
	char resp_data_subchunk[LONG_MAX_RESP];
	char resp_DATA[LONG_MAX_RESP];
}response_packet_t;


typedef union{
	uint8_t array[sizeof(uint32_t)];
	uint32_t dword;
}buffer_uint32_union_t;

typedef union{
	uint8_t array[sizeof(uint16_t)];
	uint16_t word;
}buffer_uint16_union_t;


/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
volatile send_file_packet_t file_packet;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
static volatile wav_hdr_t WAV_FILE;


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


void Serial_Analizar_Trama_v2(const uint8_t usart_instance, serial_data_descriptor_t * serial_data_desc)
{
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

	//static uint8_t dbg_buffer[136];
	//static uint8_t idx = 0;

	int32_t dato_rx = usart_pop_RX(&usart_ctx[usart_instance]);

	if (dato_rx < 0)
		return 0;

	switch (estado) {
		case BYTE_START:
			if (dato_rx == START_BYTE) {
				//dbg_buffer[idx++] = (uint8_t)dato_rx;

				estado = TIPO;
			}
			break;

		case TIPO:
			if (dato_rx == 'R'  || dato_rx == 'f'  || dato_rx == 'd'	|| dato_rx == 'D' 	|| dato_rx == 'C') {

				switch (dato_rx) {

					case __RIFF:
						serial_data_desc->pckt_type = packet_type = __RIFF;
						//dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __fmtSubchunk:
						serial_data_desc->pckt_type = packet_type = __fmtSubchunk;
						//dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __dataSubchunk:
						serial_data_desc->pckt_type = packet_type = __dataSubchunk;
						//dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __DATA_BLK:
						serial_data_desc->pckt_type = packet_type = __DATA_BLK;
						//dbg_buffer[idx++] = (uint8_t)dato_rx;
						estado = AUDIO_DATA;
						break;

					case __COMMAND:
						serial_data_desc->pckt_type = packet_type = __COMMAND;
						//dbg_buffer[idx++] = (uint8_t)dato_rx;
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
									//memcpy(&dbg_buffer[idx], WAV_FILE.RIFF, size_riff);
									//idx += size_riff;
									i = 0;

									estado_riff = __ChunkSize;
								}
							}

							break;

						case __ChunkSize:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.ChunkSize) {
								WAV_FILE.ChunkSize = aux_uint32.dword;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.ChunkSize, sizeof WAV_FILE.ChunkSize);
								//idx += sizeof WAV_FILE.ChunkSize;
								i = 0;

								estado_riff = __Format;
							}

							break;

						case __Format:
							if (_WAVE && (i < size_wave)) {
								WAV_FILE.WAVE[i++] = (uint8_t)dato_rx;

								if (i == size_wave) {
									WAV_FILE.WAVE[i] = '\0';
									//memcpy(&dbg_buffer[idx], WAV_FILE.WAVE, size_wave);
									//idx += size_wave;
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
									//memcpy(&dbg_buffer[idx], WAV_FILE.fmt, size_fmt);
									//idx += size_fmt;
									i = 0;

									estado_fmt = __Subchunk1Size;
								}
							}
							break;

						case __Subchunk1Size:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.Subchunk1Size) {
								WAV_FILE.Subchunk1Size = aux_uint32.dword;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.Subchunk1Size, sizeof WAV_FILE.Subchunk1Size);
								//idx += sizeof WAV_FILE.Subchunk1Size;
								i = 0;

								estado_fmt = __AudioFormat;
							}

							break;

						case __AudioFormat:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.AudioFormat) {
								WAV_FILE.AudioFormat = aux_uint16.word;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.AudioFormat, sizeof WAV_FILE.AudioFormat);
								//idx += sizeof WAV_FILE.AudioFormat;
								i = 0;

								estado_fmt = __NumOfChan;
							}

							break;

						case __NumOfChan:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.NumOfChan) {
								WAV_FILE.NumOfChan = aux_uint16.word;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.NumOfChan, sizeof WAV_FILE.NumOfChan);
								//idx += sizeof WAV_FILE.NumOfChan;
								i = 0;

								estado_fmt = __SamplesPerSec;
							}

							break;

						case __SamplesPerSec:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.SamplesPerSec) {
								WAV_FILE.SamplesPerSec = aux_uint32.dword;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.SamplesPerSec, sizeof WAV_FILE.SamplesPerSec);
								//idx += sizeof WAV_FILE.SamplesPerSec;
								i = 0;

								estado_fmt = __bytesPerSec;
							}

							break;

						case __bytesPerSec:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.bytesPerSec) {
								WAV_FILE.bytesPerSec = aux_uint32.dword;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.bytesPerSec, sizeof WAV_FILE.bytesPerSec);
								//idx += sizeof WAV_FILE.bytesPerSec;
								i = 0;

								estado_fmt = __blockAlign;
							}

							break;

						case __blockAlign:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.blockAlign) {
								WAV_FILE.blockAlign = aux_uint16.word;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.blockAlign, sizeof WAV_FILE.blockAlign);
								//idx += sizeof WAV_FILE.blockAlign;
								i = 0;

								estado_fmt = __bitsPerSample;
							}

							break;

						case __bitsPerSample:
							aux_uint16.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.bitsPerSample) {
								WAV_FILE.bitsPerSample = aux_uint16.word;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.bitsPerSample, sizeof WAV_FILE.bitsPerSample);
								//idx += sizeof WAV_FILE.bitsPerSample;
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
									//memcpy(&dbg_buffer[idx], WAV_FILE.Subchunk2ID, size_data_sc);
									//idx += size_data_sc;
									i = 0;

									estado_data = __Subchunk2Size;
								}
							}

							break;

						case __Subchunk2Size:
							aux_uint32.array[i++] = (uint8_t)dato_rx;

							if (i == sizeof WAV_FILE.Subchunk2Size) {
								WAV_FILE.Subchunk2Size = aux_uint32.dword;
								//memcpy(&dbg_buffer[idx], (char *)&WAV_FILE.Subchunk2Size, sizeof WAV_FILE.Subchunk2Size);
								//idx += sizeof WAV_FILE.Subchunk2Size;
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

				case __DATA_BLK:
					/* ***** data_file_packet ***** */
					switch (estado_file_packet) {

					case __blockNumber:
						aux_uint16.array[i++] = (uint8_t)dato_rx;

						if (i == sizeof file_packet.blockNumber) {
							file_packet.blockNumber = aux_uint16.word;
							//memcpy(&dbg_buffer[idx], (char *)&file_packet.blockNumber, sizeof file_packet.blockNumber);
							//idx += sizeof file_packet.blockNumber;
							i = 0;

							estado_file_packet= __endPacket;
						}

						break;

					case __endPacket:
						//dbg_buffer[idx++] = file_packet.endPacket = (uint8_t)dato_rx;
						file_packet.endPacket = (uint8_t)dato_rx;

						estado_file_packet = __lenDataPacket;
						break;

					case __lenDataPacket:
						aux_uint16.array[i++] = (uint8_t)dato_rx;

						if (i == sizeof file_packet.lenDataPacket) {
							//dbg_buffer[idx++] = file_packet.lenDataPacket = (uint8_t)dato_rx;
							file_packet.lenDataPacket = aux_uint16.word;
							i = 0;

							estado_file_packet = __Packet;
						}

						break;

					case __Packet:
						file_packet.dataPacket[i++] = (uint8_t)dato_rx;

						i %= FILE_MAX_PACKET_SIZE;

						if (i == 0) {
							estado_file_packet = __blockNumber;
							estado = BYTE_STOP;
						}

/*						if (i == file_packet.lenDataPacket) {
							//memcpy(&dbg_buffer[idx], file_packet.dataPacket, file_packet.lenDataPacket);
							//idx += file_packet.lenDataPacket;

							//Serial_Load_SD_Buffer(sd_wr_buffer, file_packet.dataPacket, file_packet.lenDataPacket);
							//memcpy((uint8_t *)sd_wr_buffer, file_packet.dataPacket, file_packet.lenDataPacket);

							i = 0;
							estado_file_packet = __blockNumber;
							estado = BYTE_STOP;
						}*/
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
				serial_data_desc->comando[i] = comando[i] = (char)dato_rx;
				i++;
				break;							// Se coloca este break para salir mientras recibe el comando
			}
			else {
				serial_data_desc->comando[i] = comando[i] = '\0';
				i = 0;
				estado = BYTE_STOP;				// No se coloca break al final de case COMANDO para permitir llegar al estado BYTE_STOP
			}

		case BYTE_STOP:
			if (dato_rx == STOP_BYTE) {
				//dbg_buffer[idx++] = (uint8_t)dato_rx;
				//usart_send_data(&usart_ctx[dbg_inst], dbg_buffer, idx);

				serial_data_desc->end_pckt = 1;

/*				if (packet_type == __RIFF) {
					__asm volatile ("nop");
				}*/

				//Serial_Send_Confirmation(usart_instance, packet_type);

				/*if (packet_type != __DATA_BLK) {
					Serial_Send_Confirmation(usart_instance, packet_type);
				} else {
					//Serial_Load_SD_Buffer(sd_wr_buffer, file_packet.dataPacket, file_packet.lenDataPacket);
					memcpy((uint8_t *)sd_wr_buffer, file_packet.dataPacket, file_packet.lenDataPacket);
					//if (Serial_Load_SD_Buffer(sd_wr_buffer, file_packet.dataPacket, file_packet.lenDataPacket)) {
						sd_write_flag_set();
						Serial_Send_Confirmation(usart_instance, packet_type);
					//}
				}*/

				/*if (packet_type == __COMMAND)
					Serial_Analizar_Comando(usart_instance, comando);*/

				i = 0;
				//idx = 0;
				estado = BYTE_START;
				packet_type = __notDefined;
				estado_riff = __ChunkID;
				estado_fmt = __Subchunk1ID;
				estado_data = __Subchunk2ID;
				estado_file_packet = __blockNumber;
			}

			break;

		default:
			estado = BYTE_START;
			break;
	}
}

void Serial_Send_Confirmation(const uint8_t usart_inst, const packet_types_t packet_type)
{
	static const response_packet_t responses = {
					.resp_RIFF = "$RIFF_OK#",
					.resp_fmt_subchunk = "$fmt_OK#",
					.resp_data_subchunk = "$data_OK#",
					.resp_DATA = "$Blk_OK#",
	};

	switch (packet_type) {
		case __RIFF:
			usart_send_data(&usart_ctx[usart_inst], responses.resp_RIFF, strlen(responses.resp_RIFF));
			break;

		case __fmtSubchunk:
			usart_send_data(&usart_ctx[usart_inst], responses.resp_fmt_subchunk, strlen(responses.resp_fmt_subchunk));
			break;

		case __dataSubchunk:
			usart_send_data(&usart_ctx[usart_inst], responses.resp_data_subchunk, strlen(responses.resp_data_subchunk));
			break;

		case __DATA_BLK:
			usart_send_data(&usart_ctx[usart_inst], responses.resp_DATA, strlen(responses.resp_DATA));
			break;

		default:
			break;
	}
}

commands_rx_en Serial_Analizar_Comando(const uint8_t usart_inst, const char comando[])
{
	static const commands_rx_t commands = {
			.cmd_play = "PLAY",
			.cmd_pause = "PAUSA",
			.cmd_stop = "STOP"
	};

	static const response_cmd_tx_t responses = {
			.resp_play = "$PLAY_OK#",
			.resp_pause = "$PAUSA_OK#",
			.resp_stop = "$STOP_OK#"
		};

	commands_rx_en ret_command = __undefined_CMD;

	if (!strcmp(comando, commands.cmd_play)) {
		ret_command = __PLAY;
		usart_send_data(&usart_ctx[usart_inst], responses.resp_play, strlen(responses.resp_play));
	}
	else if (!strcmp(comando, commands.cmd_pause)) {
		ret_command = __PAUSE;
		usart_send_data(&usart_ctx[usart_inst], responses.resp_pause, strlen(responses.resp_pause));
	}
	else if (!strcmp(comando, commands.cmd_stop)) {
		ret_command = __STOP;
		usart_send_data(&usart_ctx[usart_inst], responses.resp_stop, strlen(responses.resp_stop));
	}
	else {

	}

	return ret_command;
}


int8_t Serial_Load_SD_Buffer(volatile uint8_t sd_wr_buffer[], const uint8_t data_packet[], const size_t data_packet_length)
{
	const uint16_t qty_sub_pckts = SD_DATA_SIZE / data_packet_length;
	const uint16_t last_packet_size = data_packet_length % SD_DATA_SIZE;

	static uint16_t idx = 0;
	static uint32_t i = 0;

	int8_t result = 1;

	if (data_packet_length <= SD_DATA_SIZE ) {
		if (i < qty_sub_pckts) {
			memcpy((uint8_t *)&sd_wr_buffer[idx], data_packet, data_packet_length);
			idx += data_packet_length;
			i++;
		}

		if (i == qty_sub_pckts) {
			idx = 0;
			i = 0;
		}
	}
	else {
		result = 0;
	}

	return result;
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

