/*******************************************************************************************************************************//**
 *
 * @file		DR_UART0.c
 * @brief		Descripcion del modulo
 * @date		Nov 20, 2020
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_UART0.h"
//#include "DR_PLL.h"
//#include "DR_PINSEL.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
//#define		ISER0			(*(uint32_t *)(0xE000E100))
/*
#define		DIR_UART0		( ( __RW uint32_t  * ) 0x4000C000UL )

#define		UART0RBR		DIR_UART0[0]	// Registro de Recepción RBR
#define		UART0THR		DIR_UART0[0]	// Registro de Transmisión THR
#define		UART0DLL		DIR_UART0[0]	// Parte baja del divisor de la UART0:
#define		UART0IER		DIR_UART0[1]	// Registro de Habilitación de interrupciones de la UART0:
#define		UART0DLM		DIR_UART0[1]	// Parte Alta del divisor de la UART0:
#define		UART0IIR		DIR_UART0[2]	// Registro de Identificación de interrupciones de la UART0:
#define		UART0LCR		DIR_UART0[3]	// Line CONTROL Register de la UART0:
#define		UART0LSR		DIR_UART0[5]	// Line STATUS Register de la UART0:

#define 	PCUART0_PCON_BIT			(3)
#define 	PCLK_UART0_PCLKSEL0_BIT		(6)
#define 	UART0LCR_PARITY_BIT			(3)
#define 	UART0LCR_STOP_BIT			(2)

#define 	RLS_INTERRUPT				0x03 //0b011
#define 	RDA_INTERRUPT				0x02 //0b010
#define 	CTI_INTERRUPT				0x06 //0b110
#define 	THRE_INTERRUPT				0x01 //0b001
*/

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/
//#define		UART0DLAB_OFF	(UART0LCR & 0xEF)
//#define		UART0DLAB_ON	(UART0LCR | 0x80)

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
volatile uint8_t UART0_bufferTx[TX_BUFFER_SIZE];
volatile uint32_t UART0_txInput = 0;
volatile uint32_t UART0_txOutput = 0;

volatile uint8_t UART0_bufferRx[RX_BUFFER_SIZE];
volatile uint32_t UART0_rxInput = 0;
volatile uint32_t UART0_rxOutput = 0;

volatile uint8_t UART0_TX_flag = 0;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
void SetBaudRate(uint32_t baudrate_Bps, uint32_t srcClock_Hz);

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/



/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federico Speroni
 	\date Nov 20, 2020
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void UART0_Init(uint32_t baudRate, usart0_data_len_t dataLen, usart0_parity_mode_t parity, usart0_stop_bit_count_t stopCount)
{
	// 1 - Habilito CLOCK del periferico USART0
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_UART0(ENABLED);

	// 2 - Clear USART0 Peripheral Resets
	SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_UART0_RST_N(ENABLED);

	// 3 - Switch matrix SWM - Route RXD and TXD pins
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM(ENABLED);				// Enable SWM clock
	SWM0->PINASSIGN.PINASSIGN0 |= SWM_PINASSIGN0_U0_RXD_I(0x18);			// P0.24 -> RXD
	SWM0->PINASSIGN.PINASSIGN0 |= SWM_PINASSIGN0_U0_TXD_O(0x19);			// P0.25 -> TXD
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM(DISABLED);				// Disable SWM clock

	// 4 - USART0 Configuration
	USART0->CFG = USART_CFG_ENABLE(0b0) 		| 		// Initially Disable USART
				  USART_CFG_DATALEN(dataLen) 	| 		// DATALEN 8bits
				  USART_CFG_PARITYSEL(parity) 	| 		// NO Parity
				  USART_CFG_STOPLEN(stopCount) 	| 		// 1bit STOP
				  USART_CFG_CTSEN(0b0) 			| 		// CTS disabled -> No flow control
				  USART_CFG_SYNCEN(0b0) 		| 		// Asynchronous mode
				  USART_CFG_LOOP(0b0) 			| 		// Normal mode
				  USART_CFG_OETA(0b0)			|
				  USART_CFG_AUTOADDR(0b0)		|
				  USART_CFG_OESEL(0b0)			|
				  USART_CFG_RXPOL(0b0)			|
				  USART_CFG_TXPOL(0b0);

	USART0->CTL = (uint32_t) 0;

	// 5 - Baudrate Configuration for 9600bps
		//	En nuestro caso FCLK = 15MHz
		//	OSR = 16
		//	Baudrate = 9600

		//	BGRVAL = (FCLK / (OSRVAL + 1) * BaudRate) - 1
		//	BGRVAL = (15e06 / (15 + 1) * 9600) - 1
		//	BGRVAL = 96

		//	Ahora verificamos si esta dentro del error esperado (%2)
		//	baud rate = FCLK/((OSRVAL+1) x (BRGVAL+1)).
		//	BaudRateReal = 15e06 / (15+1) * (96+1)
		//	BaudRateReal = 9664

		//	Error% = 100 * (9664-9600)/9600
		//	Error% = 0,67%

	SetBaudRate(USART0_BAUD_RATE, USART0_CLOCK_FREQUENCY);

	// 6 - Enable USART0 Interrupts
	NVIC->ISER[0] |= (uint32_t)(1 << USART0_IRQn);			// Global Interrupts
	USART0->INTENSET = USART_INTENSET_RXRDYEN(ENABLED);		// RX Ready Interrupt

	// 7 - Enable USART0 RX / TX
	USART0->CFG |= USART_CFG_ENABLE(0b1);
	USART0->CTL &= ~USART_CTL_TXDIS_MASK;

	return;
}

void SetBaudRate(uint32_t baudrate_Bps, uint32_t srcClock_Hz)
{
	uint32_t best_diff = (uint32_t)-1;
	uint32_t best_osrval = 0xf;
	uint32_t best_brgval = (uint32_t)-1;

	uint32_t diff = 0U;
	uint32_t brgval = 0U;
	uint32_t osrval = 0U;
	uint32_t baudrate = 0U;

	for (osrval = best_osrval; osrval >= 8U; --osrval) {
		brgval = (srcClock_Hz / ((osrval + 1) * baudrate_Bps)) - 1;
		//brgval = (((srcClock_Hz * 10U) / ((osrval + 1U) * baudrate_Bps)) - 5U) / 10U;

		if (brgval > 0xFFFFU) {
			continue;
		}

		baudrate = srcClock_Hz / ((osrval + 1) * (brgval + 1));
		diff = baudrate_Bps < baudrate ? baudrate - baudrate_Bps : baudrate_Bps - baudrate;
		if (diff < best_diff) {
			best_diff   = diff;
		    best_osrval = osrval;
		    best_brgval = brgval;
		}
	}

	USART0->OSR = best_osrval;
	USART0->BRG = best_brgval;

	return;
}


int32_t UART0_Pop_TX(void)
{
	int32_t retVal;

	if(UART0_txOutput == UART0_txInput) {		// Indices iguales -> Buffer vacio
		retVal = -1;
	}
	else {
		retVal = UART0_bufferTx[UART0_txOutput];
		UART0_txOutput++;

		UART0_txOutput %= TX_BUFFER_SIZE;
	}

	return retVal;
}

void UART0_Push_RX(uint8_t datoRx)
{
	UART0_bufferRx[UART0_rxInput] = datoRx;
	UART0_rxInput++;

	UART0_rxInput %= RX_BUFFER_SIZE;

	return;
}

void UART0_write_TX_reg(uint8_t datoTx)
{
	USART0->TXDAT = datoTx;
	USART0->INTENSET |= USART_STAT_TXRDY_MASK;		// Habilito inetrrupcion TXRDY

	return;
}

/* USART0_IRQn interrupt handler */
void USART0_IRQHandler(void) {  // void USART0_USART_IRQHANDLER(void) {
  /*  Place your code here */
	uint32_t usart0_Status = USART0->INTSTAT;
	int32_t dato;

	if (usart0_Status & USART_STAT_RXRDY_MASK) {

		dato = (int32_t)USART0->RXDAT;
		UART0_Push_RX((uint8_t)dato);
	}

	if (usart0_Status & USART_STAT_TXRDY_MASK) {
		dato = UART0_Pop_TX();

		if (dato >= 0) {
			USART0->TXDAT = (uint8_t)dato;
		}
		else {
			USART0->INTENCLR |= USART_STAT_TXRDY_MASK;
			UART0_TX_flag = 0;
		}
	}

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


