/*
===============================================================================
 Name        : LPC845_Reproductor_WAV.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <cr_section_macros.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// TODO: insert other include files here
#include "PR_USART.h"
#include "PR_SYSCON.h"
#include "PR_SPI.h"
#include "PR_MMC_SD.h"
#include "PR_SYSTICK.h"
#include "PR_Leds.h"
#include "PR_Timer.h"
#include "AP_Serial.h"
#include "AP_MdE.h"


/* --- SYSTICK -------------------------------------- */
#define		TICK_PERIOD_MS		(5)
#define		TICK_PERIOD_US		(TICK_PERIOD_MS * 1000)

#define 	BLINK_TIME_MS		(500)

/* --- USART ---------------------------------------- */
/** Baudrate de USART a utilizar */
#define		USART0_BAUDRATE		9600
#define		USART1_BAUDRATE		19200 //115200
/** Puerto/pin en donde recibir datos por USART */
#define		UART_RX_PORTPIN		GPIO_PORTPIN_0_24
/** Puerto/pin en donde transmitir datos por USART */
#define		UART_TX_PORTPIN		GPIO_PORTPIN_0_25

/** Instancia de SPI a utilizar */
#define		SPI_INSTANCE		0
/*  Puerto/pin de periferico SPI
 *
 *	v1: MOSI y MISO a través de pines sin conexión, cableados con wire wrap al CN4.
 * 	MISO_0 -- CN4_6  -- PIN_18  -- PIO0_09
 * 	MOSI_0 -- CN4_4  -- PIN_19  -- PIO0_08
 * 	SCK_0  -- CN10_2 -- DIG_IN0 -- PIO0_04
 * 	SSEL_0 -- CN10_3 -- DIG_IN1 -- PIO0_26
 *
 *  v2: MOSI y MISO a través de RX1 y TX1 de la USART1, tomados desde CN4. Esta es la que está configurada, no hace falta cablear.
 *  MISO_0 -- CN4_8  -- TX1     -- PIO0_16
 *  MOSI_0 -- CN4_1  -- RX1     -- PIO0_17
 *  SCK_0  -- CN10_2 -- DIG_IN0 -- PIO0_04
 *  SSEL_0 -- CN10_3 -- DIG_IN1 -- PIO0_26
 *
 *  v3: LPC845-BRK montado en Protoboard.
 *  MISO_0 -- PIN_22 -- PIO0_09
 *  MOSI_0 -- PIN_21 -- PIO0_08
 *  SCK_0  -- PIN_35 -- PIO0_04
 *  SSEL_0 -- PIN_12 -- PIO0_26
 *
 *  En ambos casos, SCLK y SSEL son tomados desde CN10.
 *
 *  Alimentación SD
 *  Verificar tensión de alimentación del módulo: puede admitir +5V y/o +3.3V.
 *  IMORTANTE: NO debe alimentarse la SD directamente con +5v.
 *  En caso de alimentar con +5V, confirmar que el módulo admite la tensión y verificar la conexión.
 *  Si el módulo no admite +5V, alimentar con +3.3V.
 *
 *	+5V -- CN10_1
 *	GND -- CN10_4
 *
 */

typedef enum {
	SPI_MOSI_PORTPIN = GPIO_PORTPIN_0_8,		//GPIO_PORTPIN_0_17,
	SPI_MISO_PORTPIN = GPIO_PORTPIN_0_9,  		//GPIO_PORTPIN_0_16,
	SPI_SCK_PORTPIN = GPIO_PORTPIN_0_4,
	SPI_SSEL_PORTPIN = GPIO_PORTPIN_0_26
}spi_port_pin_en;

typedef enum {
	USART_DEBUG_INST = 0,
	USART_INTERFACE_INST = 1,
}usart_instances_en;

/* Serial Debug */
typedef enum {
	USART0_RX_PORTPIN = GPIO_PORTPIN_0_17,//GPIO_PORTPIN_0_24,
	USART0_TX_PORTPIN = GPIO_PORTPIN_0_16,//GPIO_PORTPIN_0_25,
}usart0_port_pin_en;

/* Serial Interface */
typedef enum {
	USART1_RX_PORTPIN = GPIO_PORTPIN_0_24,//GPIO_PORTPIN_0_17,
	USART1_TX_PORTPIN = GPIO_PORTPIN_0_25,//GPIO_PORTPIN_0_16,
}usart1_port_pin_en;

// INICIALIZAR_SD
// ANALIZAR_SERIAL_RX
// ESCRIBIR_SD
// RESPONDER_SERIAL_TX
// LEER_SD



static void tick_callback(void);
static void usart0_rx_callback(void);
static void usart0_tx_callback(void);
static void usart1_rx_callback(void);
static void usart1_tx_callback(void);
static void spi_rx_callback(void);
static void spi_tx_callback(void);
//static void dummy_callback(void);

/** Estructura de configuación de la USART - DEBUG*/
static const usart_config_t usart0_config =
{
	.data_length = kUSART_DATALEN_8BIT,
	.parity = kUSART_PARITY_NO_PARITY,
	.stop_bits = kUSART_STOPLEN_1BIT,
	.oversampling = kUSART_OVERSAMPLING_X16,
	.clock_selection = kSYSCON_PERIPHERAL_CLOCK_SEL_FRO,
	.baudrate = USART0_BAUDRATE,
	.tx_portpin = USART0_TX_PORTPIN,
	.rx_portpin = USART0_RX_PORTPIN,
	.tx_ready_callback = usart0_tx_callback,
	.rx_ready_callback = usart0_rx_callback,

	//.ctx.instance = UART_NUMBER
};

/** Estructura de configuación de la USART - INTERFACE*/
static const usart_config_t usart1_config =
{
	.data_length = kUSART_DATALEN_8BIT,
	.parity = kUSART_PARITY_NO_PARITY,
	.stop_bits = kUSART_STOPLEN_1BIT,
	.oversampling = kUSART_OVERSAMPLING_X16,
	.clock_selection = kSYSCON_PERIPHERAL_CLOCK_SEL_FRO,
	.baudrate = USART1_BAUDRATE,
	.tx_portpin = USART1_TX_PORTPIN,
	.rx_portpin = USART1_RX_PORTPIN,
	.tx_ready_callback = usart1_tx_callback,
	.rx_ready_callback = usart1_rx_callback,

	//.ctx.instance = UART_NUMBER
};

/** Estructura de configuación del SPI */
static const spi_master_mode_config_t spi_master_config =
{
	.clock_source = kSYSCON_PERIPHERAL_CLOCK_SEL_MAIN,
	.pre_delay = 4,
	.post_delay = 4,
	.frame_delay = 4,
	.transfer_delay = 4,
	.sck_portpin = SPI_SCK_PORTPIN,
	.mosi_portpin = SPI_MOSI_PORTPIN,
	.miso_portpin = SPI_MISO_PORTPIN,
	.ssel_portpin[0] = SPI_SSEL_PORTPIN,
	.ssel_portpin[1] = kGPIO_PORTPIN_NOT_USED,
	.ssel_portpin[2] = kGPIO_PORTPIN_NOT_USED,
	.ssel_portpin[3] = kGPIO_PORTPIN_NOT_USED,
	.ssel_polarity[0] = kSPI_SSEL_POLARITY_LOW,
	.ssel_polarity[1] = kSPI_SSEL_POLARITY_LOW,
	.ssel_polarity[2] = kSPI_SSEL_POLARITY_LOW,
	.ssel_polarity[3] = kSPI_SSEL_POLARITY_LOW,
	.tx_free_callback = spi_tx_callback,
	.rx_ready_callback = spi_rx_callback
};

static const spi_master_mode_tx_config_t spi_tx_config =
{
	.clock_mode = kSPI_CLOCK_MODE_0,
	.baudrate_bps = 150000,
	.clock_div = 23,
};

card_type_en sd_type = CT_NOT_DEFINED;
//uint8_t test_data[SD_DATA_SIZE];


// TODO: insert other definitions and declarations here
int detect_endianness(void)
{
   uint32_t val  = 0x01;
   uint8_t * buff = (uint8_t *)&val;
   int endian;

   if (buff[0] == 0) {
	    char message[] = "Big Endian...\n\r";
	    usart_send_data(&usart_ctx[USART_DEBUG_INST], message, strlen(message));
	    endian = 1;
   } else {
	    char message[] = "Little Endian...\n\r";
	    usart_send_data(&usart_ctx[USART_DEBUG_INST], message, strlen(message));
	    endian = 0;
   }

   return endian;
}

int generate_test_data(uint8_t data[], uint32_t size)
{
	const uint8_t max_val = 0xFF;
	const uint32_t div = RAND_MAX / (max_val + 1);
	uint32_t val = 0;

	srand((unsigned int)systick_get_current_value());

	for (uint32_t i = 0; i < size; ++i) {
		do {
			val = rand() / div;
		} while (val > max_val);

		data[i] = (uint8_t)val ;
	}

	return 0;
}

int generate_test_data_v2(uint8_t data[], uint32_t size)
{
	const uint8_t max_val = 0xFF;
	uint32_t end = RAND_MAX / (max_val + 1);
	uint32_t val = 0;

	end *= (max_val + 1);

	srand((unsigned int)systick_get_current_value());

	for (uint32_t i = 0; i < size; ++i) {
		while((val = rand()) >= end);
		data[i] = (uint8_t)val % max_val;
	}

	return 0;
}


int main(void) {
    // TODO: insert code here
	// Configuro como Main Clock el FRO @ 30MHz
	syscon_fro_clock_config(kCLOCK_FroSrcFroOsc , kCLOCK_FroOscOut30M);
	syscon_system_clock_set_source(kSYSCON_SYSTEM_CLOCK_SEL_FRO);

	// Configuro el clock fraccional para poder tener buena presicion para un baudrate de 115200bps
	syscon_frg_config(0, kSYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, 47);

	gpio_init(kGPIO_PORT_0);
	gpio_init(kGPIO_PORT_1);

	leds_init();

	systick_init(TICK_PERIOD_US, tick_callback);
	Timer_Start(TIMER_LED_ID, BLINK_TIME_MS, MSEG, timer_led_handler);

	// Inicialización de USART
	//usart_init(USART_DEBUG_INST, &usart0_config);
	usart_init(USART_INTERFACE_INST, &usart1_config);

	// Inicialización de SPI
	spi_master_mode_init(SPI_INSTANCE, &spi_master_config);
	spi_master_mode_tx_config(SPI_INSTANCE, &spi_tx_config);

    //char message[] = "Testing Serial...\n\r";
    //usart_send_data(&usart_ctx[USART_INSTANCE], message, sizeof message - 1);

    //generate_test_data(test_data, SD_DATA_SIZE);


    // INICIALIZAR_SD
    // ANALIZAR_SERIAL_RX
    // ESCRIBIR_SD
    // RESPONDER_SERIAL_TX
    // LEER_SD

    // Enter an infinite loop, just incrementing a counter
    while(1) {
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");

        MdE_State_Machine(USART_INTERFACE_INST, SPI_INSTANCE);

/*        if (sd_not_init)
        	sd_type = sd_init_v2(SPI_INSTANCE);
        	//sd_type = sd_init(SPI_INSTANCE);
        else {
        	if (sd_write_flag) {
        		if (sd_write_single_block(SPI_INSTANCE, sd_wr_address, (uint8_t *)sd_wr_buffer, SD_DATA_SIZE) == SD_WRITE_BLOCK_FINISHED) {
					sd_wr_address += SD_DATA_SIZE;

					sd_write_flag_clear();
				}
			}
        	if (sd_read_flag) {
        		sd_read_single_block(SPI_INSTANCE, 0x00, test_data);
			}

        	//Serial_Analizar_Trama_v2(USART_INTERFACE_INST);
        }

        Serial_Analizar_Trama_v2(USART_INTERFACE_INST); */

        Timer_Event();
    }
    return 0 ;
}


/** Trama de respuesta automatica para responder por serie */
//static char trama[] = "Trama de respuesta automatica\n";

/** Contador de caracter de trama */
//static uint32_t trama_counter = 0;

static void tick_callback(void)
{
	//static uint32_t tick_counter = 0;
	//static uint32_t nrf_counter = 0;

/*	++tick_counter;
	tick_counter %= BLINK_TIME_MS / TICK_PERIOD_MS;

	if(tick_counter == 0) {
		led_verde_toggle();
	}*/

	Timer_Discount();

}

/**
 * @brief Callback a ejecutar en recepcion de dato por USART
 */
static void usart0_rx_callback(void)
{
	uint32_t data;

	usart_rx_data(USART_DEBUG_INST, &data);
	USART_push_RX(&usart_ctx[USART_DEBUG_INST], (uint8_t)data);

}

/**
 * @brief Callback a ejecutar en finalizacion de transmision de dato por USART
 */
static void usart0_tx_callback(void)
{
	int32_t dato;

	dato = USART_pop_TX(&usart_ctx[USART_DEBUG_INST]);

	if (dato >= 0) {
		usart_tx_data(USART_DEBUG_INST, dato);
	} else {
		usart_disable_irq(USART_DEBUG_INST, kUSART_IRQ_TXRDY);
		usart_ctx[USART_DEBUG_INST].txFlag = 0;
	}

}

/**
 * @brief Callback a ejecutar en recepcion de dato por USART
 */
static void usart1_rx_callback(void)
{
	uint32_t data;

	usart_rx_data(USART_INTERFACE_INST, &data);
	USART_push_RX(&usart_ctx[USART_INTERFACE_INST], (uint8_t)data);

}

/**
 * @brief Callback a ejecutar en finalizacion de transmision de dato por USART
 */
static void usart1_tx_callback(void)
{
	int32_t dato;

	dato = USART_pop_TX(&usart_ctx[USART_INTERFACE_INST]);

	if (dato >= 0) {
		usart_tx_data(USART_INTERFACE_INST, dato);
	} else {
		usart_disable_irq(USART_INTERFACE_INST, kUSART_IRQ_TXRDY);
		usart_ctx[USART_INTERFACE_INST].txFlag = 0;
	}
}

static void spi_tx_callback(void)
{
	spi_master_mode_tx_data_t dato;

	dato = SPI_pop_TX(&spi_ctx[SPI_INSTANCE]);

	if ( dato.data != 0xFFFF ) {
		spi_master_mode_tx_data(SPI_INSTANCE, &dato);
	} else {
		spi_disable_irq(SPI_INSTANCE, kUSART_IRQ_TXRDY);
		spi_ctx[SPI_INSTANCE].txFlag = 0;
	}
}

static void spi_rx_callback(void)
{
	uint16_t data;

	data = spi_master_mode_rx_data(SPI_INSTANCE);		// PUSH_RX
	SPI_push_RX(&spi_ctx[SPI_INSTANCE], (int16_t)data);

}

void timer_led_handler(uint32_t value)
{
	led_verde_toggle();
	Timer_Start(TIMER_LED_ID, BLINK_TIME_MS, MSEG, timer_led_handler);
}


/*static void dummy_callback(void)
{
	return;
}*/


/* USART0_IRQn interrupt handler */
/*
void USART0_IRQHandler(void) {  // void USART0_USART_IRQHANDLER(void) {

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


}
*/
