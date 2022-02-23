/*******************************************************************************************************************************//**
 *
 * @file		AP_Serial.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 09, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef AP_SERIAL_H_
#define AP_SERIAL_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define 	FILE_MAX_PACKET_SIZE	(512)
#define		LONG_MAX_CMD			(16)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Enumeraciones
 **********************************************************************************************************************************/
typedef enum _packet_types {
	__notDefined 	=  0,
	__RIFF 			= 'R',
	__fmtSubchunk 	= 'f',
	__dataSubchunk 	= 'd',
	__DATA_BLK 		= 'D',
	__COMMAND 		= 'C'
}packet_types_t;

typedef enum _commands_rx{
	__undefined_CMD = 0,
	__PLAY,
	__PAUSE,
	__STOP
}commands_rx_en;

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Estructuras
 **********************************************************************************************************************************/

typedef struct serial_data_descriptor{
	packet_types_t pckt_type;
	char comando[LONG_MAX_CMD];
	uint8_t end_pckt;
}serial_data_descriptor_t;

typedef struct send_file_packet {
    uint16_t 	dataLenght;								// Longitud del paquete de Data
    uint16_t 	blockNumber;							// Número de bloque de datos de Firmware.
    uint8_t 	endPacket;								// Indica si es el bloque final de envío del FW: 0x00 = Continúan mas paquetes, 0x01 = Último paquete
    uint16_t 	lenDataPacket;							// Largo de datos del paquete. Máximo 512 bytes.
    uint8_t 	dataPacket[FILE_MAX_PACKET_SIZE];       // Paquete de datos del archivo WAV.
}send_file_packet_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile send_file_packet_t file_packet;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void Serial_Analizar_Trama(const uint8_t usart_inst);
void Serial_Analizar_Trama_v2(const uint8_t usart_instance, serial_data_descriptor_t * serial_data_desc);
void Serial_Send_Confirmation(const uint8_t usart_inst, const packet_types_t packet_type);
commands_rx_en Serial_Analizar_Comando(const uint8_t usart_inst, const char comando[]);
int8_t Serial_Load_SD_Buffer(volatile uint8_t sd_wr_buffer[], const uint8_t data_packet[], const size_t data_packet_length);

#endif /* AP_SERIAL_H_ */
