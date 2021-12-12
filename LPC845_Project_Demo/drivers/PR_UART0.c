/*******************************************************************************************************************************//**
 *
 * @file		PR_UART0.c
 * @brief		Descripcion del modulo
 * @date		Nov 21, 2020
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_UART0.h"
#include "DR_UART0.h"
#include "board.h"
#include "string.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define		START_BYTE			('$')
#define		STOP_BYTE			('#')

#define		LONG_MAX_CMD		(32)

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
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

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
void UART0_Analizar_Comando(const char comando[]);

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
 	\date Nov 21, 2020
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

int32_t UART0_Pop_RX(void)
{
	int32_t retVal;

	if(UART0_rxOutput == UART0_rxInput) {		// Indices iguales -> Buffer vacio
		retVal = -1;
	}
	else {
		retVal = UART0_bufferRx[UART0_rxOutput];
		UART0_rxOutput++;

		UART0_rxOutput %= RX_BUFFER_SIZE;
	}

	return retVal;

}


/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federico Speroni
 	\date Nov 21, 2020
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

void UART0_Push_TX(uint8_t datoTx)
{
	if(UART0_TX_flag){
		UART0_bufferTx[UART0_txInput] = datoTx ;
		UART0_txInput++;

		UART0_txInput %= TX_BUFFER_SIZE;
	}
	else{
		UART0_write_TX_reg(datoTx);
		//USART0->INTENSET |= USART_STAT_TXRDY_MASK;
		UART0_TX_flag = 1;
	}

	return;
}

/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federico Speroni
 	\date Nov 21, 2020
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void UART0_Send_Data(const void * dataTx, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
		UART0_Push_TX(*((uint8_t *)(dataTx + i)));

	return;
}



/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federico Speroni
 	\date Nov 21, 2021
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void UART0_Analizar_Trama()
{
	typedef enum _estados_UART0 {
		BYTE_START = 0,
		DATA,
		BYTE_STOP
	} estados_UART0_e;

	static estados_UART0_e estado = BYTE_START;
	static char comando[LONG_MAX_CMD];
	static uint32_t i = 0;

	int32_t dato_rx = UART0_Pop_RX();

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
				UART0_Analizar_Comando(comando);
			}
			break;

		default:
			break;
	}
}


void UART0_Analizar_Comando(const char comando[])
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
		LED_ROJO_OFF;
		LED_AZUL_OFF;
		LED_VERDE_ON;

		UART0_Send_Data(responses.resp_play, strlen(responses.resp_play));
	}
	else if (!strcmp(comando, commands.cmd_pause)) {
		LED_VERDE_OFF;
		LED_ROJO_OFF;
		LED_AZUL_ON;

		UART0_Send_Data(responses.resp_pause, strlen(responses.resp_pause));
	}
	else if (!strcmp(comando, commands.cmd_stop)) {
		LED_AZUL_OFF;
		LED_VERDE_OFF;
		LED_ROJO_ON;

		UART0_Send_Data(responses.resp_stop, strlen(responses.resp_stop));
	}
	else {

	}

}


