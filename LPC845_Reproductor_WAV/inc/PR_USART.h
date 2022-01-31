/*******************************************************************************************************************************//**
 *
 * @file		PR_USART.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 5, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_USART_H_
#define PR_USART_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "PR_SYSCON.h"
#include "PR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define TX_BUFFER_SIZE				(256)
#define RX_BUFFER_SIZE				(256)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Enumeraciones
 **********************************************************************************************************************************/
/** Seleccion de instancia de USART */
typedef enum
{
	kUSART_SEL_0, /**< Instancia 0 */
	kUSART_SEL_1, /**< Instancia 1 */
	kUSART_SEL_2, /**< Instancia 2 */
	kUSART_SEL_3, /**< Instancia 3 */
	kUSART_SEL_4, /**< Instancia 4 */
	kUSART_SEL_AMOUNT /**< Cantidad de instancias */
}usart_sel_en;

/** Cantidad de bits en cada dato */
typedef enum
{
	kUSART_DATALEN_7BIT = 0, /**< Largo de dato de 7 bits */
	kUSART_DATALEN_8BIT, /**< Largo de dato de 8 bits */
	kUSART_DATALEN_9BIT /**< Largo de dato de 9 bits */
}usart_datalen_en;

/** Paridad en la comunicación */
typedef enum
{
	kUSART_PARITY_NO_PARITY = 0, /**< Comunicación sin paridad */
	kUSART_PARITY_EVEN = 2, /**< Comunicación con paridad par */
	kUSART_PARITY_ODD /**< Comunicación con paridad impar */
}usart_parity_en;

/** Cantidad de bits de stop en la comunicación */
typedef enum
{
	kUSART_STOPLEN_1BIT = 0, /**< Un bit de stop */
	kUSART_STOPLEN_2BIT /**< Dos bits de stop */
}usart_stop_en;

/** Oversampling en el muestreo de los bits */
typedef enum
{
	kUSART_OVERSAMPLING_X5 = 4, /**< Oversampling x5 */
	kUSART_OVERSAMPLING_X6, 	/**< Oversampling x6 */
	kUSART_OVERSAMPLING_X7, 	/**< Oversampling x7 */
	kUSART_OVERSAMPLING_X8, 	/**< Oversampling x8 */
	kUSART_OVERSAMPLING_X9, 	/**< Oversampling x9 */
	kUSART_OVERSAMPLING_X10, 	/**< Oversampling x10 */
	kUSART_OVERSAMPLING_X11, 	/**< Oversampling x11 */
	kUSART_OVERSAMPLING_X12, 	/**< Oversampling x12 */
	kUSART_OVERSAMPLING_X13, 	/**< Oversampling x13 */
	kUSART_OVERSAMPLING_X14, 	/**< Oversampling x14 */
	kUSART_OVERSAMPLING_X15, 	/**< Oversampling x15 */
	kUSART_OVERSAMPLING_X16, 	/**< Oversampling x16 */
}usart_oversampling_en;

/** Resultado de un intento de inicio de transmisión */
typedef enum
{
	kUSART_TX_RESULT_OK = 0, /**< Inicio de transmisión ok */
	kUSART_TX_RESULT_NOT_READY /**< Inicio de transmisión no exitoso, se estaba transmitiendo */
}usart_tx_result_e;

/** Resultado de un intento de lectura de recepción */
typedef enum
{
	kUSART_RX_RESULT_OK = 0, /**< Resultado de recepción ok */
	kUSART_RX_RESULT_NOT_READY /**< Resultado de recepción no exitoso, no se había recibido ningún dato */
}usart_rx_result_e;

typedef enum
{
	kUSART_IRQ_RXRDY = 0,
	kUSART_IRQ_TXRDY,
	kUSART_IRQ_TXIDLE,
	kUSART_IRQ_DELTACTS,
	kUSART_IRQ_TXDIS,
	kUSART_IRQ_OVERRUN,
	kUSART_IRQ_DELTARXBRK,
	kUSART_IRQ_START,
	kUSART_IRQ_FRAMERR,
	kUSART_IRQ_PARITYERR,
	kUSART_IRQ_RXNOISE,
	kUSART_IRQ_ABERR
}usart_irq_src_en;

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Tipo de dato para los callback en recepción de dato completa.
 * @note Estos callbacks son ejecutados desde un contexto de interrupción, por lo que el usuario deberá tener
 * todas las consideraciones necesarias al respecto.
 */
typedef void (*usart_rx_callback)(usart_sel_en);

/**
 * @brief Tipo de dato para los callback en transmisión de dato completa
 * @note Estos callbacks son ejecutados desde un contexto de interrupción, por lo que el usuario deberá tener
 * todas las consideraciones necesarias al respecto.
 */
typedef void (*usart_tx_callback)(usart_sel_en);

/** Estructura de contexto de una instancia de UART */
typedef struct
{
	volatile usart_sel_en instance;					/**< Instancia USARTn */

	volatile uint8_t bufferTx[TX_BUFFER_SIZE];		/**< Buffer de Transmisión */
	volatile uint32_t txInput;						/**< Indice de entrada del buffer de Transmsión */
	volatile uint32_t txOutput;						/**< Indice de salida del buffer de Transmsión */

	volatile uint8_t bufferRx[RX_BUFFER_SIZE];		/**< Buffer de Recepción */
	volatile uint32_t rxInput;						/**< Indice de entrada del buffer de Recepción */
	volatile uint32_t rxOutput;						/**< Indice de salida del buffer de Recepción */

	volatile uint8_t txFlag;						/**< Flag de Transmisión */

}usart_context_t;

/** Estructura de configuración de una instancia de UART */
typedef struct
{
	usart_datalen_en data_length; 					/**< Largo en bits de cada dato en la comunicación */
	usart_parity_en parity; 						/**< Paridad de la comunicación */
	usart_stop_en stop_bits; 						/**< Cantidad de bits de stop de la comunicación */
	usart_oversampling_en oversampling; 			/**< Oversampling de la instancia */
	syscon_peripheral_clock_sel_en clock_selection; /**< Fuente de clock para la instancia */
	uint32_t baudrate; 								/**< Baudrate deseado para la comunicación */
	gpio_portpin_en tx_portpin; 					/**< Puerto/pin donde configurar las transmisiones */
	gpio_portpin_en rx_portpin; 					/**< Puerto/pin donde configurar las recepciones */

	usart_tx_callback tx_ready_callback; 			/**< Callback a ejecutar en una transmisión exitosa (Cuando se terminó de enviar un dato) */
	usart_rx_callback rx_ready_callback; 			/**< Callback a ejecutar en una recepción exitosa (Cuando se terminó de recibir un dato) */

}usart_config_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile usart_context_t usart_ctx[];

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Inicializar contexto USART (buffers, indices y flagTx)
 * @param[in] inst Instancia de USART inicializar
 * @pre
 */
void usart_init_ctx(usart_sel_en  inst);

/**
 * @brief Inicializar USART con los parámetros deseados
 * @param[in] inst Instancia de USART inicializar
 * @param[in] config Configuración deseada de la instancia
 * @pre Haber inicializado la fuente de clock a utilizar correctamente. Ver @ref SYSCON
 */
void usart_init(usart_sel_en inst, const usart_config_t * config);

/**
 * @brief Transmitir un dato mediante una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
usart_tx_result_e usart_tx_data(usart_sel_en inst, uint32_t data);

/**
 * @brief Recibir un dato de una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepción
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
usart_rx_result_e usart_rx_data(usart_sel_en inst, uint32_t *data);

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejectutar cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void usart_rx_register_callback(usart_sel_en inst, usart_rx_callback new_callback);

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejecutar cada vez que se termina de enviar un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void usart_tx_register_callback(usart_sel_en inst, usart_tx_callback new_callback);

/**
 * @brief Inicializar USART con los parámetros deseados
 * @param[in] inst Instancia de USART inicializar
 * @param[in] config Configuración deseada de la instancia
 * @pre Haber inicializado la fuente de clock a utilizar correctamente. Ver @ref SYSCON
 */
void usart_init(usart_sel_en inst, const usart_config_t * config);

/**
 * @brief Transmitir un dato mediante una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
usart_tx_result_e usart_tx_data(usart_sel_en inst, uint32_t data);
/**
 * @brief Recibir un dato de una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepción
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */

usart_rx_result_e usart_rx_data(usart_sel_en inst, uint32_t *data);
/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejectutar cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void usart_rx_register_callback(usart_sel_en inst, usart_rx_callback new_callback);
/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejecutar cada vez que se termina de enviar un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void usart_tx_register_callback(usart_sel_en inst, usart_tx_callback new_callback);

/**
 * @brief Llamado a funcion dummy para irq iniciales
 */

/**
 * @brief Interrupcion de UART0
 */
void UART0_IRQHandler(void);
/**
 * @brief Interrupcion de UART1
 */
void UART1_IRQHandler(void);
/**
 * @brief Interrupcion de UART2
 */
void UART2_IRQHandler(void);
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
void UART3_irq(void);
/**
 * @brief Interrupcion de UART4
 */
void UART4_irq(void);

int32_t usart_pop_RX(volatile usart_context_t * context);
void usart_push_TX(volatile usart_context_t * context, const uint8_t datoTx);

int32_t USART_pop_TX(volatile usart_context_t * context);
void USART_push_RX(volatile usart_context_t * context, uint8_t datoRx);

void usart_send_data(volatile usart_context_t * context, const void * dataTx, uint32_t size);

void usart_enable_irq(usart_sel_en inst, usart_irq_src_en irq_src);
void usart_disable_irq(usart_sel_en inst, usart_irq_src_en irq_src);

#endif /* PR_USART_H_ */
