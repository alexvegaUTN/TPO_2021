/*******************************************************************************************************************************//**
 *
 * @file		PR_SPI.c
 * @brief		Descripcion del modulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <stddef.h>
#include "PR_SPI.h"
#include "PR_IOCON.h"
#include "DR_SPI.h"
#include "DR_SWM.h"
#include "DR_SYSCON.h"
#include "DR_NVIC.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/
/*! @name DIV - SPI clock Divider */
/*! @{ */
#define SPI_DIV_DIVVAL_MASK                      (0xFFFFU)
#define SPI_DIV_DIVVAL_SHIFT                     (0U)
/*! DIVVAL - Rate divider value. Specifies how the Flexcomm clock (FCLK) is divided to produce the
 *    SPI clock rate in master mode. DIVVAL is -1 encoded such that the value 0 results in FCLK/1,
 *    the value 1 results in FCLK/2, up to the maximum possible divide value of 0xFFFF, which results
 *    in FCLK/65536.
 */
#define SPI_DIV_DIVVAL(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_DIV_DIVVAL_SHIFT)) & SPI_DIV_DIVVAL_MASK)
/*! @} */

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
volatile spi_context_t spi_ctx[kSPI_AMOUNT];

volatile spi_master_mode_tx_data_t spi_tx_buff[SPI_TX_BUFFER_SIZE + 1];
volatile uint16_t spi_txInput = 0;
volatile uint16_t spi_txOutput = 0;

volatile uint16_t spi_rx_buff[100];
volatile uint32_t spi_rxInput = 0;
volatile uint32_t spi_rxOutput = 0;

volatile uint8_t spi_rx_complete_flag = 0;
volatile uint8_t spi_tx_flag = 0;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
volatile uint8_t spi_configured_instance = 0xFF;

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
/**
 * @brief Escribe la instancia configurada del SPI en variable interna
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
static void spi_set_configured_instance(uint8_t inst);

static void dummy_irq(void);
static void spi_irq_handler(uint8_t inst);

static void (*p_spi_rx_callback[])(void) = //!< Callbacks registrados a la recepcion de un dato por SPI
{
	dummy_irq,
	dummy_irq
};

static void (*p_spi_tx_callback[])(void) = //!< Callbacks registrados a la liberacion del buffer de transmision de SPI
{
	dummy_irq,
	dummy_irq
};

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
static void spi_set_configured_instance(uint8_t inst)
{
	spi_configured_instance = inst;
}

/*
 * @brief Funcion dummy para inicialiar punteros de interrupcion
 */
static void dummy_irq(void)
{
	return;
}

/**
 * @brief Manejador generico de interrupciones de SPI
 * @param[in] inst Instancia que genero la interrupcion
 */
static void spi_irq_handler(uint8_t inst)
{
	if(SPI_get_irq_flag_status(inst, SPI_IRQ_RXRDY) && SPI_get_status_flag(inst, SPI_STATUS_FLAG_RXRDY))
	{
		p_spi_rx_callback[inst]();

		// Lectura dummy para evitar loops infinitos si no importaba leer la informacion
		(void) SPI_read_rx_data(inst);
	}

	if(SPI_get_irq_flag_status(inst, SPI_IRQ_TXRDY) && SPI_get_status_flag(inst, SPI_STATUS_FLAG_TXRDY))
	{
		// Inhabilitacion de interrupcion de TX por si no se envia mas informacion
		spi_disable_irq(inst, kSPI_IRQ_TXRDY);

		// Si se envia nueva informacion, en este llamado se activa de nuevo la interrupcion
		p_spi_tx_callback[inst]();
	}
}

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federcio Speroni
 	\date Jan 8, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Inicializar contexto SPI (buffers, indices y flagTx)
 * @param[in] inst Instancia de SPI inicializar
 * @pre
 */
void spi_init_ctx(spi_sel_en  inst)
{
	if (inst > kSPI_1)
		return;

	spi_ctx[inst].instance = inst;

	spi_ctx[inst].txInput = 0;
	spi_ctx[inst].txOutput = 0;

	spi_ctx[inst].rxInput = 0;
	spi_ctx[inst].rxOutput = 0;

	spi_ctx[inst].txFlag = 0;
}

/**
 * @brief Inicializar SPI en modo master
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
void spi_master_mode_init(spi_sel_en inst, const spi_master_mode_config_t *config)
{
	const iocon_config_t spi_mosi_pin_config = {
			.pull_mode = kIOCON_PULL_UP, /**< Resistor interno */
			.hysteresis = kIOCON_HYST_DISABLE, /**< Histéresis. Cualquier valor distinto de cero la activa */
			.invert_input = kIOCON_INPUT_NOT_INVERTED, /**< Inversión de lógica de lectura. Cualquier valor distinto de cero la activa */
			.open_drain = kIOCON_OP_DRAIN_DISABLED, /**< Modo *Open Drain*. Cualquier valor distinto de cero lo activa */
			.sample_mode = kIOCON_SAMPLE_MODE_BYPASS, /**< Cantidad de muestras del filtro de glitches */
			.clk_sel = kIOCON_CLK_DIV_0, /**< Selección de clock para el filtro de glitches */
			.i2c_mode = kIOCON_IIC_MODE_GPIO, /**< Selección de modo I2C */
	};

	const iocon_config_t spi_miso_pin_config = {
			.pull_mode = kIOCON_PULL_UP, /**< Resistor interno */
			.hysteresis = kIOCON_HYST_DISABLE, /**< Histéresis. Cualquier valor distinto de cero la activa */
			.invert_input = kIOCON_INPUT_NOT_INVERTED, /**< Inversión de lógica de lectura. Cualquier valor distinto de cero la activa */
			.open_drain = kIOCON_OP_DRAIN_DISABLED, /**< Modo *Open Drain*. Cualquier valor distinto de cero lo activa */
			.sample_mode = kIOCON_SAMPLE_MODE_BYPASS, /**< Cantidad de muestras del filtro de glitches */
			.clk_sel = kIOCON_CLK_DIV_0, /**< Selección de clock para el filtro de glitches */
			.i2c_mode = kIOCON_IIC_MODE_GPIO, /**< Selección de modo I2C */
	};

	const iocon_config_t spi_ssel0_pin_config = {
			.pull_mode = kIOCON_PULL_UP, /**< Resistor interno */
			.hysteresis = kIOCON_HYST_DISABLE, /**< Histéresis. Cualquier valor distinto de cero la activa */
			.invert_input = kIOCON_INPUT_NOT_INVERTED, /**< Inversión de lógica de lectura. Cualquier valor distinto de cero la activa */
			.open_drain = kIOCON_OP_DRAIN_DISABLED, /**< Modo *Open Drain*. Cualquier valor distinto de cero lo activa */
			.sample_mode = kIOCON_SAMPLE_MODE_BYPASS, /**< Cantidad de muestras del filtro de glitches */
			.clk_sel = kIOCON_CLK_DIV_0, /**< Selección de clock para el filtro de glitches */
			.i2c_mode = kIOCON_IIC_MODE_GPIO, /**< Selección de modo I2C */
	};

	const iocon_config_t spi_clk_pin_config = {
			.pull_mode = kIOCON_PULL_UP, /**< Resistor interno */
			.hysteresis = kIOCON_HYST_DISABLE, /**< Histéresis. Cualquier valor distinto de cero la activa */
			.invert_input = kIOCON_INPUT_NOT_INVERTED, /**< Inversión de lógica de lectura. Cualquier valor distinto de cero la activa */
			.open_drain = kIOCON_OP_DRAIN_DISABLED, /**< Modo *Open Drain*. Cualquier valor distinto de cero lo activa */
			.sample_mode = kIOCON_SAMPLE_MODE_BYPASS, /**< Cantidad de muestras del filtro de glitches */
			.clk_sel = kIOCON_CLK_DIV_0, /**< Selección de clock para el filtro de glitches */
			.i2c_mode = kIOCON_IIC_MODE_GPIO, /**< Selección de modo I2C */
	};

	// Inicializo contexto del SPI (buffers de Tx/Rx e indices In/Out) y reservo la instancia SPI configurada
	spi_init_ctx(inst);
	spi_set_configured_instance(inst);

	SWM_init();

	if(config->miso_portpin != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_MISO(inst, GPIO_PORTPIN_TO_PORT(config->miso_portpin), GPIO_PORTPIN_TO_PIN(config->miso_portpin));

	if(config->mosi_portpin != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_MOSI(inst, GPIO_PORTPIN_TO_PORT(config->mosi_portpin), GPIO_PORTPIN_TO_PIN(config->mosi_portpin));

	if(config->sck_portpin != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_SCK(inst, GPIO_PORTPIN_TO_PORT(config->sck_portpin), GPIO_PORTPIN_TO_PIN(config->sck_portpin));

	if(config->ssel_portpin[0] != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_SSEL0(inst, GPIO_PORTPIN_TO_PORT(config->ssel_portpin[0]), GPIO_PORTPIN_TO_PIN(config->ssel_portpin[0]));

	if(config->ssel_portpin[1] != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_SSEL1(inst, GPIO_PORTPIN_TO_PORT(config->ssel_portpin[1]), GPIO_PORTPIN_TO_PIN(config->ssel_portpin[1]));

	if(config->ssel_portpin[2] != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_SSEL2(inst, GPIO_PORTPIN_TO_PORT(config->ssel_portpin[2]), GPIO_PORTPIN_TO_PIN(config->ssel_portpin[2]));

	if(config->ssel_portpin[3] != kGPIO_PORTPIN_NOT_USED)
		SWM_assign_spi_SSEL3(inst, GPIO_PORTPIN_TO_PORT(config->ssel_portpin[3]), GPIO_PORTPIN_TO_PIN(config->ssel_portpin[3]));

	SWM_deinit();

	for(uint16_t i = 0; i < 4; i++)
	{
		if(config->ssel_polarity[i] == kSPI_SSEL_POLARITY_LOW)
			SPI_set_ssel_active_low(inst, i);
		else
			SPI_set_ssel_active_high(inst, i);
	}

	/* --- IOCON -------------------------------------------------- */
	iocon_config_io(config->mosi_portpin, &spi_mosi_pin_config);
	iocon_config_io(config->miso_portpin, &spi_miso_pin_config);
	iocon_config_io(config->sck_portpin, &spi_ssel0_pin_config);
	iocon_config_io(config->sck_portpin, &spi_clk_pin_config);

	switch(inst)
	{
	case kSPI_0:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_SPI0, config->clock_source);
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_SPI0);
		SYSCON_clear_reset(SYSCON_RESET_SEL_SPI0);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_SPI0);

		break;

	case kSPI_1:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_SPI1, config->clock_source);
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_SPI1);
		SYSCON_clear_reset(SYSCON_RESET_SEL_SPI1);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_SPI1);

		break;

	default:
		break;
	}

	SPI_enable(inst);

	SPI_set_master_mode(inst);
	SPI_set_opMode_0(inst);
	SPI_set_data_order_msb_first(inst);
	SPI_select_slave(inst, 0xFF);

	SPI_set_pre_delay(inst, config->pre_delay);
	SPI_set_post_delay(inst, config->post_delay);
	SPI_set_frame_delay(inst, config->frame_delay);
	SPI_set_transfer_delay(inst, config->transfer_delay);

	if(config->rx_ready_callback != NULL) {
		spi_enable_irq(inst, kSPI_IRQ_RXRDY);
		p_spi_rx_callback[inst] = config->rx_ready_callback;
	}

	if(config->tx_free_callback != NULL) {
		// Las interrupciones se habilitaran en el comienzo de una transmision
		p_spi_tx_callback[inst] = config->tx_free_callback;
	}
}

uint8_t spi_get_configured_instance()
{
	return spi_configured_instance;
}

/*!
 * brief Sets the baud rate for SPI transfer. This is only used in master.
 *
 * param baudrate_Bps baud rate needed in Hz.
 * param srcClock_Hz SPI source clock frequency in Hz.
 */
uint32_t spi_set_baud_rate(spi_sel_en inst, uint32_t baudrate_bps, uint32_t srcClock_Hz)
{
    uint32_t div;

    /* Calculate baudrate */
    div = (srcClock_Hz / baudrate_bps) - 1U;

    if (div > 0xFFFFU)
        return 0;

    SPI_set_clock_div(inst, div);

    return div;
}

/**
 * @brief Leer el dato recibido
 * @param[in] inst Instancia a consultar
 * @return Dato recibido
 */
uint16_t spi_master_mode_rx_data(spi_sel_en inst)
{
	return SPI_read_rx_data(inst);
}

/**
 * @brief Configurar la transmision
 * @param[in] inst Instancia a configurar
 * @param[in] config Configuracion para la transmision deseada
 */
void spi_master_mode_tx_config(spi_sel_en inst, const spi_master_mode_tx_config_t *config)
{
	switch(config->clock_mode)
	{
	case kSPI_CLOCK_MODE_0: { SPI_set_cpha_change(inst); SPI_set_cpol_low(inst); break; }
	case kSPI_CLOCK_MODE_1: { SPI_set_cpha_capture(inst); SPI_set_cpol_low(inst); break; }
	case kSPI_CLOCK_MODE_2: { SPI_set_cpha_change(inst); SPI_set_cpol_high(inst); break; }
	case kSPI_CLOCK_MODE_3: { SPI_set_cpha_capture(inst); SPI_set_cpol_high(inst); break; }
	}

	spi_set_baud_rate(inst, config->baudrate_bps, syscon_peripheral_clock_get(kSYSCON_PERIPHERAL_SEL_SPI0));
}

/**
 * @brief Transmitir dato
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir, con controles asociados
 */
spi_tx_result_e spi_master_mode_tx_data(spi_sel_en inst, const volatile spi_master_mode_tx_data_t *data)
{
	SPI_set_data_and_control(inst, (SPI_TXDATCTL_reg_t *) data);
	spi_enable_irq(inst, kSPI_IRQ_TXRDY);

	return kSPI_TX_RESULT_OK;
}

/**
 * @brief spi_pop_RX()
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir, con controles asociados
 */
int16_t spi_pop_RX(volatile spi_context_t * context)
{
	int16_t rxValue;

	if(context->rxOutput == context->rxInput) {		// Indices iguales -> Buffer vacio
		rxValue = -1;
	}
	else {
		rxValue = context->bufferRx[context->rxOutput++]; //spi_rx_buff[spi_rxOutput++];
		context->rxOutput %= SPI_RX_BUFFER_SIZE;
	}

	return rxValue;
}

void spi_push_TX(volatile spi_context_t * context, const spi_master_mode_tx_data_t datoTx)
{
	if(context->txFlag){
		context->bufferTx[context->txInput++] = datoTx ;
		context->txInput %= SPI_TX_BUFFER_SIZE;
	}
	else{
		spi_master_mode_tx_data(context->instance, &datoTx);
		context->txFlag = 1;
	}

	return;
}

spi_master_mode_tx_data_t SPI_pop_TX(volatile spi_context_t * context)
{
	spi_master_mode_tx_data_t txValue;

	if(context->txInput == context->txOutput) {		// Indices iguales -> Buffer vacio
		txValue.data = 0xFFFF;
	}
	else {
		txValue = context->bufferTx[context->txOutput++]; 	//spi_tx_buff[spi_txOutput++];

		context->txOutput %= SPI_TX_BUFFER_SIZE;
	}

	return txValue;
}

void SPI_push_RX(volatile spi_context_t * context, const int16_t  datoRx)
{
	context->bufferRx[context->rxInput++] = datoRx;		//spi_rx_buff[spi_rxInput++] = datoRx;
	context->rxInput %= SPI_RX_BUFFER_SIZE;

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

void spi_master_mode_send_data(volatile spi_context_t * context, const spi_master_mode_tx_data_t * data, uint32_t size)
{
	for (uint32_t i = 0; i < size; ++i)
		spi_push_TX(context, data[i]);

	return;
}

/**
 * @brief Actualizar callback en TXRDY
 * @param[in] inst Instancia a configurar
 * @param[in] new_callback Nuevo callback a ejecutar en TXRDY
 */
void spi_master_mode_tx_register_callback(spi_sel_en inst, void (*new_callback)(void))
{
	if(new_callback == NULL)
		p_spi_tx_callback[inst] = dummy_irq;
	else
		p_spi_tx_callback[inst] = new_callback;
}

/**
 * @brief Actualizar callback en RXRDY
 * @param[in] inst Instancia a configurar
 * @param[in] new_callback Nuevo callback a ejecutar en RXRDY
 */
void spi_master_mode_rx_register_callback(spi_sel_en inst, void (*new_callback)(void))
{
	if(new_callback == NULL)
	{
		spi_disable_irq(inst, kSPI_IRQ_RXRDY);
		p_spi_rx_callback[inst] = dummy_irq;
	}
	else
	{
		spi_enable_irq(inst, kSPI_IRQ_RXRDY);
		p_spi_rx_callback[inst] = new_callback;
	}
}

/**
 * @brief Manejador de interrupcion de SPI0
 */
void SPI0_IRQHandler(void)
{
	spi_irq_handler(kSPI_0);
}

/**
 * @brief Manejador de interrupcion de SPI1
 */
void SPI1_IRQHandler(void)
{
	spi_irq_handler(kSPI_1);
}

/**
 * @brief Habilitar interrupcion
 * @param[in] inst Instancia a configurar
 * @param[in] irq Interrupcion a habilitar
 */
uint8_t spi_enable_irq(uint8_t inst, spi_irq_sel_en irq)
{
	switch(irq)
	{
	case kSPI_IRQ_RXRDY: { SPI_enable_irq_RXRDY(inst); 	break; }
	case kSPI_IRQ_TXRDY: { SPI_enable_irq_TXRDY(inst); 	break; }
	case kSPI_IRQ_RXOV:  { SPI_enable_irq_RXOV(inst); 	break; }
	case kSPI_IRQ_TXUR:  { SPI_enable_irq_TXUR(inst); 	break; }
	case kSPI_IRQ_SSA: 	 { SPI_enable_irq_SSA(inst); 	break; }
	case kSPI_IRQ_SSD: 	 { SPI_enable_irq_SSD(inst); 	break; }
	}

	return 0;
}

/**
 * @brief Inhabilitar interrupcion
 * @param[in] inst Instancia a configurar
 * @param[in] irq Interrupcion a inhabilitar
 */
uint8_t spi_disable_irq(uint8_t inst, spi_irq_sel_en irq)
{
	switch(irq)
	{
	case kSPI_IRQ_RXRDY: { SPI_disable_irq_RXRDY(inst); break; }
	case kSPI_IRQ_TXRDY: { SPI_disable_irq_TXRDY(inst); break; }
	case kSPI_IRQ_RXOV:  { SPI_disable_irq_RXOV(inst); 	break; }
	case kSPI_IRQ_TXUR:  { SPI_disable_irq_TXUR(inst); 	break; }
	case kSPI_IRQ_SSA: 	 { SPI_disable_irq_SSA(inst); 	break; }
	case kSPI_IRQ_SSD: 	 { SPI_disable_irq_SSD(inst); 	break; }
	}

	return 0;
}


void spi_select_slave(uint8_t inst, uint8_t channel)
{
	SPI_select_slave(inst, channel);

}
void spi_deselect_slave(uint8_t inst, uint8_t channel)
{
	SPI_deselect_slave(inst, channel);
}


/*

void spi_polling_rx (uint8_t inst) {

	int16_t temporal;
	uint8_t rx_flag = SPI_get_status_flag(inst, SPI_STATUS_FLAG_RXRDY);

	if(rx_flag) {
		temporal = (int16_t)SPI_read_rx_data(uint8_t inst);
		//UART0_PushRx((uint8_t)Temporal);
	}
}

void spi_polling_tx (uint8_t inst) {

	int32_t Temporal;
	uint8_t tx_flag = SPI_get_status_flag(inst, SPI_STATUS_FLAG_TXRDY);

	if(tx_flag) {

		Temporal = UART0_PopTx();
		if(Temporal >= 0) {
			//USART0->TXDAT = (uint8_t)Temporal;
		}
	}
}*/
