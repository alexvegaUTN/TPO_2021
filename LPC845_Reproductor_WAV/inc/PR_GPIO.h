/*******************************************************************************************************************************//**
 *
 * @file		PR_GPIO.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_GPIO_H_
#define PR_GPIO_H_

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
/** Macro para obtener número de puerto a partir de un puerto/pin */
#define		GPIO_PORTPIN_TO_PORT(x)		((gpio_port_en)(x / 32))

/** Macro para obtener número de pin a partir de un puerto/pin */
#define		GPIO_PORTPIN_TO_PIN(x)		(x % 32)

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/** Enumeraciones de puertos disponibles */
typedef enum
{
	kGPIO_PORT_0 = 0, /**< Puerto 0 */
	kGPIO_PORT_1 /**< Puerto 1 */
}gpio_port_en;

/** Enumeraciones de puerto/pin disponibles */
typedef enum
{
	kGPIO_PORTPIN_0_0 = 0, /**< Puerto 0, pin 0 */
	kGPIO_PORTPIN_0_1, /**< Puerto 0, pin 1 */
	kGPIO_PORTPIN_0_2, /**< Puerto 0, pin 2 */
	kGPIO_PORTPIN_0_3, /**< Puerto 0, pin 3 */
	kGPIO_PORTPIN_0_4, /**< Puerto 0, pin 4 */
	kGPIO_PORTPIN_0_5, /**< Puerto 0, pin 5 */
	kGPIO_PORTPIN_0_6, /**< Puerto 0, pin 6 */
	kGPIO_PORTPIN_0_7, /**< Puerto 0, pin 7 */
	kGPIO_PORTPIN_0_8, /**< Puerto 0, pin 8 */
	kGPIO_PORTPIN_0_9, /**< Puerto 0, pin 9 */
	kGPIO_PORTPIN_0_10, /**< Puerto 0, pin 10 */
	kGPIO_PORTPIN_0_11, /**< Puerto 0, pin 11 */
	kGPIO_PORTPIN_0_12, /**< Puerto 0, pin 12 */
	kGPIO_PORTPIN_0_13, /**< Puerto 0, pin 13 */
	kGPIO_PORTPIN_0_14, /**< Puerto 0, pin 14 */
	kGPIO_PORTPIN_0_15, /**< Puerto 0, pin 15 */
	kGPIO_PORTPIN_0_16, /**< Puerto 0, pin 16 */
	kGPIO_PORTPIN_0_17, /**< Puerto 0, pin 17 */
	kGPIO_PORTPIN_0_18, /**< Puerto 0, pin 18 */
	kGPIO_PORTPIN_0_19, /**< Puerto 0, pin 19 */
	kGPIO_PORTPIN_0_20, /**< Puerto 0, pin 20 */
	kGPIO_PORTPIN_0_21, /**< Puerto 0, pin 21 */
	kGPIO_PORTPIN_0_22, /**< Puerto 0, pin 22 */
	kGPIO_PORTPIN_0_23, /**< Puerto 0, pin 23 */
	kGPIO_PORTPIN_0_24, /**< Puerto 0, pin 24 */
	kGPIO_PORTPIN_0_25, /**< Puerto 0, pin 25 */
	kGPIO_PORTPIN_0_26, /**< Puerto 0, pin 26 */
	kGPIO_PORTPIN_0_27, /**< Puerto 0, pin 27 */
	kGPIO_PORTPIN_0_28, /**< Puerto 0, pin 28 */
	kGPIO_PORTPIN_0_29, /**< Puerto 0, pin 29 */
	kGPIO_PORTPIN_0_30, /**< Puerto 0, pin 30 */
	kGPIO_PORTPIN_0_31, /**< Puerto 0, pin 31 */
	kGPIO_PORTPIN_1_0, /**< Puerto 1, pin 0 */
	kGPIO_PORTPIN_1_1, /**< Puerto 1, pin 1 */
	kGPIO_PORTPIN_1_2, /**< Puerto 1, pin 2 */
	kGPIO_PORTPIN_1_3, /**< Puerto 1, pin 3 */
	kGPIO_PORTPIN_1_4, /**< Puerto 1, pin 4 */
	kGPIO_PORTPIN_1_5, /**< Puerto 1, pin 5 */
	kGPIO_PORTPIN_1_6, /**< Puerto 1, pin 6 */
	kGPIO_PORTPIN_1_7, /**< Puerto 1, pin 7 */
	kGPIO_PORTPIN_1_8, /**< Puerto 1, pin 8 */
	kGPIO_PORTPIN_1_9, /**< Puerto 1, pin 9 */
	kGPIO_PORTPIN_NOT_USED /**< Puerto/pin no utilizado */
}gpio_portpin_en;

/** Enumeraciones de direcciones posibles para un puerto/pin */
typedef enum
{
	kGPIO_DIR_INPUT = 0, /**< Puerto/pin como entrada */
	kGPIO_DIR_OUTPUT /**< Puerto/pin como salida */
}gpio_dir_en;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Inicializar un puerto
 * @param[in] port Puerto a inicializar
 */
void gpio_init(gpio_port_en port);

/**
 * @brief Fijar dirección de una GPIO
 * @param[in] portpin Número de puerto/pin a configurar
 * @param[in] dir Dirección deseada
 * @param[in] initial_state Estado inicial (aplica únicamente para salidas)
 *
 * @note Es importante recordar que el @ref IOCON controla aspectos de hardware del puerto/pin, por ejemplo la
 * inversión en la lógica del mismo, y que esta función no configura ninguno de esos aspectos. En caso de ser
 * necesario configurar dichas características, ver @ref hal_iocon_config_io.
 *
 * @see gpio_portpin_en
 * @see gpio_dir_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_dir(gpio_portpin_en portpin, gpio_dir_en dir, uint8_t initial_state);

/**
 * @brief Fijar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_pin(gpio_portpin_en portpin);
void gpio_set_pin_active_low(gpio_portpin_en portpin);

/**
 * @brief Fijar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_port(gpio_port_en port, uint32_t bits_to_set);

/**
 * @brief Fijar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_masked_set_port(gpio_port_en port, uint32_t bits_to_set);

/**
 * @brief Limpiar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_pin(gpio_portpin_en portpin);
void gpio_clear_pin_active_low(gpio_portpin_en portpin);

/**
 * @brief Limpiar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_port(gpio_port_en port, uint32_t bits_to_clear);

/**
 * @brief Limpiar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_masked_clear_port(gpio_port_en port, uint32_t bits_to_clear);

/**
 * @brief Invertir estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 */
void gpio_toggle_pin(gpio_portpin_en portpin);

/**
 * @brief Invertir estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_toggle_port(gpio_port_en port, uint32_t bits_to_toggle);

/**
 * @brief Invertir estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_masked_toggle_port(gpio_port_en port, uint32_t bits_to_toggle);

/**
 * @brief Leer el estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 * @return Estado actual del puerto/pin omitiendo máscara
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint8_t gpio_read_pin(gpio_portpin_en portpin);

/**
 * @brief Leer estado de un puerto (sin importar máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto sin enmascarar
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint32_t gpio_read_port(gpio_port_en port);

/**
 * @brief Leer estado de un puerto (teniendo en cuenta máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto contemplando la máscara asociada
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint32_t gpio_masked_read_port(gpio_port_en port);

/**
 * @brief Fijar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a fijar enmascaramiento
 * @param[in] mask Máscara de bits a fijar
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_mask_bits(gpio_port_en port, uint32_t mask);

/**
 * @brief Limpiar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a limpiar enmascaramiento
 * @param[in] mask Máscara de bits a limpiar
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_mask_bits(gpio_port_en port, uint32_t mask);

/**
 * @brief Invertir enmascaramiento de pines en un puerto
 * @param[in] port Puerto a invertir enmascaramiento
 * @param[in] mask Máscara de bits a invertir
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_toggle_mask_bits(gpio_port_en port, uint32_t mask);

#endif /* PR_GPIO_H_ */
