/*******************************************************************************************************************************//**
 *
 * @file		PR_GPIO.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_GPIO.h"
#include "DR_GPIO.h"
#include "DR_SYSCON.h"

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

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federcio Speroni
 	\date Jan 4, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Inicializar un puerto
 * @param[in] port Puerto a inicializar
 */
void gpio_init(gpio_port_en port)
{
	if(port == kGPIO_PORT_0)
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO0);
	}
	else
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO1);
	}
}

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
void gpio_set_dir(gpio_portpin_en portpin, gpio_dir_en dir, uint8_t initial_state)
{
	if(dir == kGPIO_DIR_INPUT)
	{
		GPIO_write_dir_clear(GPIO_PORTPIN_TO_PORT(portpin), 1 << GPIO_PORTPIN_TO_PIN(portpin));
	}
	else
	{
		GPIO_write_dir_set(GPIO_PORTPIN_TO_PORT(portpin), 1 << GPIO_PORTPIN_TO_PIN(portpin));

		if(initial_state)
		{
			GPIO_write_set(GPIO_PORTPIN_TO_PORT(portpin), 1 << GPIO_PORTPIN_TO_PIN(portpin));
		}
	}
}

/**
 * @brief Fijar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_pin(gpio_portpin_en portpin)
{
	GPIO_write_port_byte(portpin, 1);
}

/**
 * @brief Fijar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_pin_active_low(gpio_portpin_en portpin)
{
	GPIO_write_port_byte(portpin, 0);
}

/**
 * @brief Fijar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_port(gpio_port_en port, uint32_t bits_to_set)
{
	GPIO_write_portpin(port, GPIO_read_portpin(port) | bits_to_set);
}

/**
 * @brief Fijar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_masked_set_port(gpio_port_en port, uint32_t bits_to_set)
{
	GPIO_write_masked_portpin(port, GPIO_read_portpin(port) | bits_to_set);
}

/**
 * @brief Limpiar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_pin(gpio_portpin_en portpin)
{
	GPIO_write_port_byte(portpin, 0);
}

/**
 * @brief Limpiar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_pin_active_low(gpio_portpin_en portpin)
{
	GPIO_write_port_byte(portpin, 1);
}

/**
 * @brief Limpiar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_port(gpio_port_en port, uint32_t bits_to_clear)
{
	GPIO_write_portpin(port, GPIO_read_portpin(port) & (~bits_to_clear));
}

/**
 * @brief Limpiar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_masked_clear_port(gpio_port_en port, uint32_t bits_to_clear)
{
	GPIO_write_masked_portpin(port, GPIO_read_portpin(port) & (~bits_to_clear));
}

/**
 * @brief Invertir estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see gpio_portpin_en
 */
void gpio_toggle_pin(gpio_portpin_en portpin)
{
	GPIO_write_toggle(GPIO_PORTPIN_TO_PORT(portpin), 1 << GPIO_PORTPIN_TO_PIN(portpin));
}

/**
 * @brief Invertir estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_toggle_port(gpio_port_en port, uint32_t bits_to_toggle)
{
	GPIO_write_portpin(port, GPIO_read_portpin(port) ^ bits_to_toggle);
}

/**
 * @brief Invertir estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_masked_toggle_port(gpio_port_en port, uint32_t bits_to_toggle)
{
	GPIO_write_masked_portpin(port, GPIO_read_portpin(port) ^ bits_to_toggle);
}

/**
 * @brief Leer el estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 * @return Estado actual del puerto/pin omitiendo máscara
 *
 * @see gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint8_t gpio_read_pin(gpio_portpin_en portpin)
{
	return GPIO_read_port_byte(portpin);
}

/**
 * @brief Leer estado de un puerto (sin importar máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto sin enmascarar
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint32_t gpio_read_port(gpio_port_en port)
{
	return GPIO_read_portpin(port);
}

/**
 * @brief Leer estado de un puerto (teniendo en cuenta máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto contemplando la máscara asociada
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint32_t gpio_masked_read_port(gpio_port_en port)
{
	return GPIO_read_masked_portpin(port);
}

/**
 * @brief Fijar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a fijar enmascaramiento
 * @param[in] mask Máscara de bits a fijar
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_set_mask_bits(gpio_port_en port, uint32_t mask)
{
	GPIO_write_mask(port, GPIO_read_mask(port) | mask);
}

/**
 * @brief Limpiar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a limpiar enmascaramiento
 * @param[in] mask Máscara de bits a limpiar
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_clear_mask_bits(gpio_port_en port, uint32_t mask)
{
	GPIO_write_mask(port, GPIO_read_mask(port) & (~mask));
}

/**
 * @brief Invertir enmascaramiento de pines en un puerto
 * @param[in] port Puerto a invertir enmascaramiento
 * @param[in] mask Máscara de bits a invertir
 *
 * @see gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void gpio_toggle_mask_bits(gpio_port_en port, uint32_t mask)
{
	GPIO_write_mask(port, GPIO_read_mask(port) ^ mask);
}

