/*******************************************************************************************************************************//**
 *
 * @file		PR_USART.c
 * @brief		Descripcion del modulo
 * @date		Jan 5, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <stddef.h>
#include "PR_USART.h"
#include <PR_SYSCON.h>
#include "DR_USART.h"
#include <DR_NVIC.h>
#include <DR_SWM.h>
#include <DR_SYSCON.h>


/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
volatile usart_context_t usart_ctx[kUSART_SEL_AMOUNT];

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
static void dummy_callback(usart_sel_en d);

static void (*p_usart_rx_callback[])(usart_sel_en) = //!< Callbacks registrados a la recepcion de un dato por USART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static void (*p_usart_tx_callback[])(usart_sel_en) = //!< Callbacks registrados a la finalizacion de transmision de un dato por USART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static uint16_t usart_calculate_brgval(uint32_t usart_clock, uint32_t baudrate, uint8_t oversampling);
static void usart_handle_irq(uint8_t inst);
static void usart_set_baudRate(uint8_t inst, uint32_t baudrate_Bps, uint32_t srcClock_Hz);

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
/**
 * @brief Calculo del valor para el registro de Baudrate.
 * @param[in] uart_clock Clock asociado con la UART.
 * @param[in] baudrate Baudrate deseado a calcular.
 * @param[in] oversampling Oversampling deseado para la UART.
 * @return Valor a poner en el registro BRG.
 */
static uint16_t usart_calculate_brgval(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling)
{
	return ((uart_clock) / ((oversampling + 1) * baudrate)) - 1;
}

static void usart_handle_irq(uint8_t inst)
{
	uint32_t dummy_data;

	if(USART_get_irq_status_RXRDY(inst) && USART_get_flag_RXRDY(inst)) {
		p_usart_rx_callback[inst](inst);

		// Limpio flag de interrupcion leyendo el registro correspondiente
		dummy_data = USART_get_data(inst);
		(void) dummy_data;
	}

	if(USART_get_irq_status_TXRDY(inst) && USART_get_flag_TXRDY(inst)) {
		// Deshabilito interrupciones
		USART_disable_irq_TXRDY(inst);

		// Es probable que en este callback se inicie otra transmision, en cuyo caso se volveran a habilitar
		p_usart_tx_callback[inst](inst);
	}
}

static void usart_set_baudRate(uint8_t inst, uint32_t baudrate_Bps, uint32_t srcClock_Hz)
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

	USART_set_OSRVAL(inst, best_osrval);
	USART_set_BRGVAL(inst, best_brgval);

	return;
}

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federcio Speroni
 	\date Jan 5, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Inicializar contexto USART (buffers, indices y flagTx)
 * @param[in] inst Instancia de USART inicializar
 * @pre
 */
void usart_init_ctx(usart_sel_en  inst)
{
	if (inst > kUSART_SEL_4)
		return;

	usart_ctx[inst].instance = inst;

	usart_ctx[inst].txInput = 0;
	usart_ctx[inst].txOutput = 0;

	usart_ctx[inst].rxInput = 0;
	usart_ctx[inst].rxOutput = 0;

	usart_ctx[inst].txFlag = 0;
}

/**
 * @brief Inicializar USART con los parámetros deseados
 * @param[in] inst Instancia de USART inicializar
 * @param[in] config Configuración deseada de la instancia
 * @pre Haber inicializado la fuente de clock a utilizar correctamente. Ver @ref SYSCON
 */
void usart_init(usart_sel_en inst, const usart_config_t * config)
{
	uint32_t aux;

	// Configuro instancia USARTn en la estructura de contexto
	usart_init_ctx(inst);

	SWM_init();

	if(config->rx_portpin != kGPIO_PORTPIN_NOT_USED) {
		SWM_assign_uart_RXD(inst, GPIO_PORTPIN_TO_PORT(config->rx_portpin), GPIO_PORTPIN_TO_PIN(config->rx_portpin));
	}

	if(config->tx_portpin != kGPIO_PORTPIN_NOT_USED) {
		SWM_assign_uart_TXD(inst, GPIO_PORTPIN_TO_PORT(config->tx_portpin), GPIO_PORTPIN_TO_PIN(config->tx_portpin));
	}

	SWM_deinit();

	switch(inst)
	{
	case kUSART_SEL_0:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART0, config->clock_selection);

		aux = usart_calculate_brgval(syscon_peripheral_clock_get(kSYSCON_PERIPHERAL_SEL_UART0), config->baudrate, config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART0);
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART0);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART0);

		break;
	case kUSART_SEL_1:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART1, config->clock_selection);

		aux = usart_calculate_brgval(syscon_peripheral_clock_get(kSYSCON_PERIPHERAL_SEL_UART1),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART1);
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART1);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART1);

		break;
	case kUSART_SEL_2:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART2, config->clock_selection);

		aux = usart_calculate_brgval(syscon_peripheral_clock_get(kSYSCON_PERIPHERAL_SEL_UART2),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART2);
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART2);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART2);

		break;
	case kUSART_SEL_3:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART3, config->clock_selection);

		aux = usart_calculate_brgval(syscon_peripheral_clock_get(kSYSCON_PERIPHERAL_SEL_UART3),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART3);
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART3);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT6_UART3);

		break;
	case kUSART_SEL_4:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART4, config->clock_selection);

		aux = usart_calculate_brgval(syscon_peripheral_clock_get(kSYSCON_PERIPHERAL_SEL_UART4),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART4);
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART4);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT7_UART4);

		break;
	default: { return; break; }
	}

	USART_set_OSRVAL(inst, config->oversampling);
	USART_config_data_length(inst, config->data_length);
	USART_config_parity(inst, config->parity);
	USART_config_stop_bits(inst, config->stop_bits);
	USART_set_BRGVAL(inst, aux);

	// Las interrupciones de TX se habilitaran cuando se envie algun byte

	if(config->rx_ready_callback != NULL)
	{
		USART_enable_irq_RXRDY(inst);
		usart_rx_register_callback(inst, config->rx_ready_callback);
	}

	if(config->tx_ready_callback != NULL)
	{
		usart_tx_register_callback(inst, config->tx_ready_callback);
	}

	USART_enable(inst);
}

/**
 * @brief Transmitir un dato mediante una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 * @pre Haber inicializado la instancia mediante @ref usart_init
 */
usart_tx_result_e usart_tx_data(usart_sel_en inst, uint32_t data)
{
	// Chequeo si se puede enviar data
	if(USART_get_flag_TXRDY(inst)) {
		// Escribo data
		USART_write_data(inst, data);

		if(p_usart_tx_callback[inst] != dummy_callback)
		{
			// Habilito interrupciones de TXRDY unicamente si se paso un callback
			USART_enable_irq_TXRDY(inst);
		}
	}
	else
		return kUSART_TX_RESULT_NOT_READY;

	return kUSART_TX_RESULT_OK;
}

/**
 * @brief Recibir un dato de una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepción
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
usart_rx_result_e usart_rx_data(usart_sel_en inst, uint32_t *data)
{
	if(USART_get_flag_RXRDY(inst))
		*data = USART_get_data(inst);
	else
		return kUSART_RX_RESULT_NOT_READY;

	return kUSART_RX_RESULT_OK;
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

int32_t usart_pop_RX(volatile usart_context_t * context)
{
	int32_t retVal;

	if(context->rxOutput == context->rxInput) {		// Indices iguales -> Buffer vacio
		retVal = -1;
	}
	else {
		retVal = context->bufferRx[context->rxOutput];
		context->rxOutput++;

		context->rxOutput %= RX_BUFFER_SIZE;
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

void usart_push_TX(volatile usart_context_t * context, const uint8_t datoTx)
{
	if(context->txFlag){
		context->bufferTx[context->txInput] = datoTx ;
		context->txInput++;

		context->txInput %= TX_BUFFER_SIZE;
	}
	else{
		usart_tx_data(context->instance, datoTx);
		context->txFlag = 1;
	}

	return;
}

int32_t USART_pop_TX(volatile usart_context_t * context)
{
	int32_t retVal;

	if(context->txInput == context->txOutput) {		// Indices iguales -> Buffer vacio
		retVal = -1;
	}
	else {
		retVal = context->bufferTx[context->txOutput];
		context->txOutput++;

		context->txOutput %= TX_BUFFER_SIZE;
	}

	return retVal;
}

void USART_push_RX(volatile usart_context_t * context, uint8_t datoRx)
{
	context->bufferRx[context->rxInput] = datoRx;
	context->rxInput++;

	context->rxInput %= RX_BUFFER_SIZE;

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
void usart_send_data(volatile usart_context_t * context, const void * dataTx, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
		usart_push_TX(context, *((uint8_t *)(dataTx + i)));

	return;
}

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejectutar cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void usart_rx_register_callback(usart_sel_en inst, usart_rx_callback new_callback)
{
	if(new_callback == NULL)
	{
		USART_disable_irq_RXRDY(inst);
		p_usart_rx_callback[inst] = dummy_callback;
	}
	else
	{
		USART_enable_irq_RXRDY(inst);
		p_usart_rx_callback[inst] = new_callback;
	}
}

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejecutar cada vez que se termina de enviar un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void usart_tx_register_callback(usart_sel_en inst, usart_tx_callback new_callback)
{
	if(new_callback == NULL)
	{
		p_usart_tx_callback[inst] = dummy_callback;
	}
	else
	{
		// Las interrupciones de TX se habilitaran (en caso de ser necesario) en el envio de un dato
		p_usart_tx_callback[inst] = new_callback;
	}
}

/**
 * @brief Llamado a funcion dummy para irq iniciales
 */
static void dummy_callback(usart_sel_en d)
{
	return;
}

/**
 * @brief Interrupcion de UART0
 */
void UART0_IRQHandler(void)
{
	usart_handle_irq(kUSART_SEL_0);
}

/**
 * @brief Interrupcion de UART1
 */
void UART1_IRQHandler(void)
{
	usart_handle_irq(kUSART_SEL_1);
}

/**
 * @brief Interrupcion de UART2
 */
void UART2_IRQHandler(void)
{
	usart_handle_irq(kUSART_SEL_2);
}

/*
 * NOTA IMPORTANTE:
 *
 * Las interrupciones de las USART 3 y 4 se encuentran en el archivo HAL_PININT.c
 * ya que comparten las interrupciones con interrupciones de GPIO.
 *
 * Lo que hago es en ese archivo, llamar a funciones de este, para que quede mas ordenado.
 *
 * En estas dos ultimas, voy a tener que revisar si la interrupcion fue a causa de la UART
 * o fue llamada porque tambien estaba configurada alguna PININT.
 */

/**
 * @brief Interrupcion de UART3
 */
void UART3_irq(void)
{
	usart_handle_irq(kUSART_SEL_3);
}

/**
 * @brief Interrupcion de UART4
 */
void UART4_irq(void)
{
	usart_handle_irq(kUSART_SEL_4);
}

void usart_enable_irq(usart_sel_en inst, usart_irq_src_en irq_src)
{
	switch (irq_src) {
		case kUSART_IRQ_RXRDY:		USART_enable_irq_RXRDY(inst);		break;
		case kUSART_IRQ_TXRDY:		USART_enable_irq_TXRDY(inst);		break;
		case kUSART_IRQ_TXIDLE: 	USART_enable_irq_TXIDLE(inst);		break;
		case kUSART_IRQ_DELTACTS:	USART_enable_irq_DELTACTS(inst);	break;
		case kUSART_IRQ_TXDIS:		USART_enable_irq_TXDISEN(inst);		break;
		case kUSART_IRQ_OVERRUN:    USART_enable_irq_OVERRUN(inst);		break;
		case kUSART_IRQ_DELTARXBRK:	USART_enable_irq_DELTARXBRK(inst);	break;
		case kUSART_IRQ_START:		USART_enable_irq_START(inst);		break;
		case kUSART_IRQ_FRAMERR:	USART_enable_irq_FRAMERR(inst);		break;
		case kUSART_IRQ_PARITYERR:	USART_enable_irq_PARITYERR(inst);	break;
		case kUSART_IRQ_RXNOISE:	USART_enable_irq_RXNOISE(inst);		break;
		case kUSART_IRQ_ABERR:		USART_enable_irq_ABERR(inst);		break;

		default:	break;
	}
}

void usart_disable_irq(usart_sel_en inst, usart_irq_src_en irq_src)
{
	switch (irq_src) {
		case kUSART_IRQ_RXRDY:		USART_disable_irq_RXRDY(inst);		break;
		case kUSART_IRQ_TXRDY:		USART_disable_irq_TXRDY(inst);		break;
		case kUSART_IRQ_TXIDLE: 	USART_disable_irq_TXIDLE(inst);		break;
		case kUSART_IRQ_DELTACTS:	USART_disable_irq_DELTACTS(inst);	break;
		case kUSART_IRQ_TXDIS:		USART_disable_irq_TXDISEN(inst);	break;
		case kUSART_IRQ_OVERRUN:    USART_disable_irq_OVERRUN(inst);	break;
		case kUSART_IRQ_DELTARXBRK:	USART_disable_irq_DELTARXBRK(inst);	break;
		case kUSART_IRQ_START:		USART_disable_irq_START(inst);		break;
		case kUSART_IRQ_FRAMERR:	USART_disable_irq_FRAMERR(inst);	break;
		case kUSART_IRQ_PARITYERR:	USART_disable_irq_PARITYERR(inst);	break;
		case kUSART_IRQ_RXNOISE:	USART_disable_irq_RXNOISE(inst);	break;
		case kUSART_IRQ_ABERR:		USART_disable_irq_ABERR(inst);		break;

		default:	break;
	}

}
