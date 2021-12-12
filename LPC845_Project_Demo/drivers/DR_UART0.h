/*******************************************************************************************************************************//**
 *
 * @file		DR_UART0.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Nov 20, 2020
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_UART0_H_
#define DR_UART0_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"
#include "LPC845.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define 	TX_BUFFER_SIZE				(100)
#define 	RX_BUFFER_SIZE				(100)

#define		ENABLED							1
#define  	DISABLED						0

#define 	USART0_CLOCK_FREQUENCY 		(15000000UL)
#define 	USART0_BAUD_RATE			(9600UL)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/*! @brief USART data size. */
typedef enum _usart0__data_len
{
    USART0_7_Bits_Data = 0x0U, /*!< Seven bit mode */
    USART0_8_Bits_Data = 0x1U, /*!< Eight bit mode */
	USART0_9_Bits_Data = 0x2U, /*!< nine bit mode */
} usart0_data_len_t;

/*! @brief USART parity mode. */
typedef enum _usart0_parity_mode
{
    USART0_ParityDisabled = 0x0U, /*!< Parity disabled */
    USART0_ParityEven     = 0x2U, /*!< Parity enabled, type even, bit setting: PARITYSEL = 10 */
    USART0_ParityOdd      = 0x3U, /*!< Parity enabled, type odd,  bit setting: PARITYSEL = 11 */
} usart0_parity_mode_t;

/*! @brief USART stop bit count. */
typedef enum _usart0_stop_bit_count
{
    USART0_OneStopBit = 0U, /*!< One stop bit */
    USART0_TwoStopBit = 1U, /*!< Two stop bits */
} usart0_stop_bit_count_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
extern volatile uint8_t UART0_bufferTx[];
extern volatile uint32_t UART0_txInput;
extern volatile uint32_t UART0_txOutput;

extern volatile uint8_t UART0_bufferRx[];
extern volatile uint32_t UART0_rxInput;
extern volatile uint32_t UART0_rxOutput;

extern volatile uint8_t UART0_TX_flag;


/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void UART0_Init(uint32_t baudRate, usart0_data_len_t dataLen, usart0_parity_mode_t parity, usart0_stop_bit_count_t stopCount);

int32_t UART0_Pop_TX(void);
void UART0_Push_RX(uint8_t datoRx);

void UART0_write_TX_reg(uint8_t datoTx);


#endif /* DR_UART0_H_ */
