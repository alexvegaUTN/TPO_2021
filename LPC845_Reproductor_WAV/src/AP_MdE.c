/*******************************************************************************************************************************//**
 *
 * @file		AP_MdE.c
 * @brief		Descripcion del modulo
 * @date		Feb 19, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <string.h>
#include "AP_MdE.h"
#include "AP_Serial.h"
#include "PR_MMC_SD.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/
typedef enum _main_states{
	__SD_INIT = 0,
	__SERIAL_RX,
	__SD_WRITE,
	__SERIAL_TX,
	__SD_READ
}main_states_e;


/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
static main_states_e main_state = __SD_INIT;
static packet_types_t packetType = __notDefined;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

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
 	\author Federcio Speroni
 	\date Feb 19, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

void MdE_State_Machine(const uint8_t usart_inst, const uint8_t spi_inst)
{
	static serial_data_descriptor_t serial_data_desc = {
			.pckt_type = __notDefined,
			.comando = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			.end_pckt = 0,
	};

	static commands_rx_en cmd_rx = __undefined_CMD;

	//card_type_en sd_type = CT_NOT_DEFINED;

    switch (main_state) {
		case __SD_INIT:
			if (sd_not_init)
				sd_init_v2(spi_inst);
			else
				main_state = __SERIAL_RX;

			break;

		case __SERIAL_RX:
			Serial_Analizar_Trama_v2(usart_inst, &serial_data_desc);
			if ((serial_data_desc.pckt_type == __DATA_BLK) && (serial_data_desc.end_pckt == 1)) {
				//Serial_Load_SD_Buffer(sd_wr_buffer, (uint8_t *)file_packet.dataPacket, file_packet.lenDataPacket);
				memcpy((uint8_t *)sd_wr_buffer, (uint8_t *)file_packet.dataPacket, file_packet.lenDataPacket);
				serial_data_desc.end_pckt = 0;
				main_state = __SD_WRITE ;
			}

			if ((serial_data_desc.pckt_type != __DATA_BLK) && (serial_data_desc.end_pckt == 1)) {

				serial_data_desc.end_pckt = 0;
				main_state = __SERIAL_TX;
			}

			break;

		case __SERIAL_TX:
			if ((serial_data_desc.pckt_type == __RIFF) 			||
				(serial_data_desc.pckt_type == __fmtSubchunk) 	||
				(serial_data_desc.pckt_type == __dataSubchunk) 	||
				(serial_data_desc.pckt_type == __DATA_BLK)) {

				Serial_Send_Confirmation(usart_inst, serial_data_desc.pckt_type);

				//main_state = __SERIAL_RX;
			}

			if (serial_data_desc.pckt_type == __COMMAND) {
				cmd_rx = Serial_Analizar_Comando(usart_inst, serial_data_desc.comando);
				if (cmd_rx == __PLAY) {
					main_state = __SD_READ;
					cmd_rx = __undefined_CMD;

					break;
				}
			}

			main_state = __SERIAL_RX;

			break;

		case __SD_WRITE:
    		if (sd_write_single_block(spi_inst, sd_wr_address, (uint8_t *)sd_wr_buffer, SD_DATA_SIZE) == SD_WRITE_BLOCK_FINISHED) {
				sd_wr_address += SD_DATA_SIZE;
				main_state = __SERIAL_TX;
			}

			break;

		case __SD_READ:
    		if (sd_read_single_block(spi_inst, sd_rd_address, (uint8_t *)sd_wr_buffer) == SD_READ_OK) {

				//main_state = __SERIAL_TX;
			}

			break;

		default:
			break;
	}
}
