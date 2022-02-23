/*******************************************************************************************************************************//**
 *
 * @file		PR_SPI.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_SPI_H_
#define PR_SPI_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>
#include "PR_SYSCON.h"
#include "PR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define		SPI_DUMMY_BYTE		(0xFF)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
#define SPI_TX_BUFFER_SIZE				(512)
#define SPI_RX_BUFFER_SIZE				(512)

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Enumeraciones
 **********************************************************************************************************************************/
typedef enum
{
	kSPI_0 = 0,
	kSPI_1,
	kSPI_AMOUNT
}spi_sel_en;

typedef enum {
	kSPI_SSEL0_ENABLED = 0,
	kSPI_SSEL1_ENABLED,
	kSPI_SSEL2_ENABLED,
	kSPI_SSEL3_ENABLED,
	kSPI_SSEL0_3_DISABLED
}spi_ssel_enable_en;

typedef enum {
	kSPI_NOT_EOT = 0,
	kSPI_EOT
}spi_eot_en;

typedef enum {
	kSPI_NOT_EOF = 0,
	kSPI_EOF
}spi_eof_en;

typedef enum {
	kSPI_RX_NOT_IGNORE = 0,
	kSPI_RX_IGNORE
}spi_rx_ignore_en;

typedef enum
{
	kSPI_DATA_LENGTH_1_BIT = 0,
	kSPI_DATA_LENGTH_2_BIT,
	kSPI_DATA_LENGTH_3_BIT,
	kSPI_DATA_LENGTH_4_BIT,
	kSPI_DATA_LENGTH_5_BIT,
	kSPI_DATA_LENGTH_6_BIT,
	kSPI_DATA_LENGTH_7_BIT,
	kSPI_DATA_LENGTH_8_BIT,
	kSPI_DATA_LENGTH_9_BIT,
	kSPI_DATA_LENGTH_10_BIT,
	kSPI_DATA_LENGTH_11_BIT,
	kSPI_DATA_LENGTH_12_BIT,
	kSPI_DATA_LENGTH_13_BIT,
	kSPI_DATA_LENGTH_14_BIT,
	kSPI_DATA_LENGTH_15_BIT,
	kSPI_DATA_LENGTH_16_BIT
}spi_data_length_en;

typedef enum
{
	kSPI_CLOCK_MODE_0 = 0,
	kSPI_CLOCK_MODE_1,
	kSPI_CLOCK_MODE_2,
	kSPI_CLOCK_MODE_3
}spi_clock_mode_en;

typedef enum
{
	kSPI_SSEL_POLARITY_LOW = 0,
	kSPI_SSEL_POLARITY_HIGH
}spi_ssel_polarity_en;

typedef enum
{
	kSPI_SSEL_SELECTION_0 = 0,
	kSPI_SSEL_SELECTION_1,
	kSPI_SSEL_SELECTION_2,
	kSPI_SSEL_SELECTION_3,
	kSPI_SSEL_SELECTION_OTHER
}spi_ssel_sel_en;

typedef enum
{
	kSPI_IRQ_RXRDY = 0,
	kSPI_IRQ_TXRDY,
	kSPI_IRQ_RXOV,
	kSPI_IRQ_TXUR,
	kSPI_IRQ_SSA,
	kSPI_IRQ_SSD
}spi_irq_sel_en;

/** Resultado de un intento de inicio de transmisión */
typedef enum
{
	kSPI_TX_RESULT_OK = 0, /**< Inicio de transmisión ok */
	kSPI_TX_RESULT_NOT_READY /**< Inicio de transmisión no exitoso, se estaba transmitiendo */
}spi_tx_result_e;

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Estructuras
 **********************************************************************************************************************************/
typedef struct
{
	syscon_peripheral_clock_sel_en clock_source;
	uint8_t pre_delay;
	uint8_t post_delay;
	uint8_t frame_delay;
	uint8_t transfer_delay;
	gpio_portpin_en sck_portpin;
	gpio_portpin_en miso_portpin;
	gpio_portpin_en mosi_portpin;
	gpio_portpin_en ssel_portpin[4];
	spi_ssel_polarity_en ssel_polarity[4];
	void (*tx_free_callback)(void);
	void (*rx_ready_callback)(void);
}spi_master_mode_config_t;

typedef struct
{
	spi_clock_mode_en clock_mode;
	uint16_t clock_div;
	uint32_t baudrate_bps;
}spi_master_mode_tx_config_t;

typedef struct
{
	uint32_t data : 16;
	uint32_t ssel0_n : 1;
	uint32_t ssel1_n : 1;
	uint32_t ssel2_n : 1;
	uint32_t ssel3_n : 1;
	uint32_t eot : 1;
	uint32_t eof : 1;
	uint32_t rxignore : 1;
	uint32_t : 1;
	uint32_t data_length : 4;
	uint32_t : 4;
}spi_master_mode_tx_data_t;

/** Estructura de contexto de una instancia de SPI */
typedef struct
{
	volatile spi_sel_en instance;											/**< Instancia SPIn */

	volatile spi_master_mode_tx_data_t  bufferTx[SPI_TX_BUFFER_SIZE];		/**< Buffer de Transmisión */
	volatile uint32_t txInput;												/**< Indice de entrada del buffer de Transmsión */
	volatile uint32_t txOutput;												/**< Indice de salida del buffer de Transmsión */

	volatile uint16_t bufferRx[SPI_RX_BUFFER_SIZE];							/**< Buffer de Recepción */
	volatile uint32_t rxInput;												/**< Indice de entrada del buffer de Recepción */
	volatile uint32_t rxOutput;												/**< Indice de salida del buffer de Recepción */

	volatile uint8_t txFlag;												/**< Flag de Transmisión */

}spi_context_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile spi_context_t spi_ctx[];

extern volatile spi_master_mode_tx_data_t spi_tx_buff[];
extern volatile uint16_t spi_txInput;
extern volatile uint16_t spi_txOutput;

extern volatile uint16_t spi_rx_buff[];
extern volatile uint32_t spi_rxInput;
extern volatile uint32_t spi_rxOutput;

extern volatile uint8_t spi_rx_complete_flag;
extern volatile uint8_t spi_tx_flag;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Inicializar contexto SPI (buffers, indices y flagTx)
 * @param[in] inst Instancia de SPI inicializar
 * @pre
 */
void spi_init_ctx(spi_sel_en  inst);
/**
 * @brief Inicializar SPI en modo master
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
void spi_master_mode_init(spi_sel_en inst, const spi_master_mode_config_t *config);

/**
 * @brief Lee la instancia configurada del SPI
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
uint8_t spi_get_configured_instance();

/**
 * @brief Leer el dato recibido
 * @param[in] inst Instancia a consultar
 * @return Dato recibido
 */
uint16_t spi_master_mode_rx_data(spi_sel_en inst);

/**
 * @brief Configurar la transmision
 * @param[in] inst Instancia a configurar
 * @param[in] config Configuracion para la transmision deseada
 */
void spi_master_mode_tx_config(spi_sel_en inst, const spi_master_mode_tx_config_t *config);

//int32_t spi_pop_RX();
//void spi_push_TX(uint8_t inst, const spi_master_mode_tx_data_t datoTx);

//spi_master_mode_tx_data_t SPI_pop_TX();
//void SPI_push_RX(const spi_master_mode_tx_data_t  datoRx);

//void spi_master_mode_send_data(volatile spi_context_t * context, const spi_master_mode_tx_data_t * data, uint32_t size);

/**
 * @brief Transmitir dato
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir, con controles asociados
 */
spi_tx_result_e spi_master_mode_tx_data(spi_sel_en inst, const volatile spi_master_mode_tx_data_t *data);

/**
 * @brief Actualizar callback en TXRDY
 * @param[in] inst Instancia a configurar
 * @param[in] new_callback Nuevo callback a ejecutar en TXRDY
 */
void spi_master_mode_tx_register_callback(spi_sel_en inst, void (*new_callback)(void));

/**
 * @brief Actualizar callback en RXRDY
 * @param[in] inst Instancia a configurar
 * @param[in] new_callback Nuevo callback a ejecutar en RXRDY
 */
void spi_master_mode_rx_register_callback(spi_sel_en inst, void (*new_callback)(void));

/**
 * @brief Calcula y setea el divisor requerido para obtener el baud rate deseado
 * @param[in] inst Instancia a configurar
 * @param[in] baudrate_bps Baudrate en bits por segundo
 * @param[in] Valor en Hz del clock source del periferico SPI
 */
uint32_t spi_set_baud_rate(spi_sel_en inst, uint32_t baudrate_bps, uint32_t srcClock_Hz);

uint8_t spi_enable_irq(uint8_t inst, spi_irq_sel_en irq);
uint8_t spi_disable_irq(uint8_t inst, spi_irq_sel_en irq);

void spi_select_slave(uint8_t inst, uint8_t channel);
void spi_deselect_slave(uint8_t inst, uint8_t channel);

int16_t spi_pop_RX(volatile spi_context_t * context);
void spi_push_TX(volatile spi_context_t * context, const spi_master_mode_tx_data_t datoTx);
spi_master_mode_tx_data_t SPI_pop_TX(volatile spi_context_t * context);
void SPI_push_RX(volatile spi_context_t * context, const int16_t  datoRx);

void spi_master_mode_send_data(volatile spi_context_t * context, const spi_master_mode_tx_data_t * data, uint32_t size);



#endif /* PR_SPI_H_ */
