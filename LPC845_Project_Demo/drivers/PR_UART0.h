/*******************************************************************************************************************************//**
 *
 * @file		PR_UART0.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Nov 21, 2020
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_UART0_H_
#define PR_UART0_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
int32_t UART0_Pop_RX(void);
void UART0_Push_TX(uint8_t datoTx);
void UART0_Send_Data(const void * dataTx, uint32_t size);

void UART0_Analizar_Trama(void);

#endif /* PR_UART0_H_ */
