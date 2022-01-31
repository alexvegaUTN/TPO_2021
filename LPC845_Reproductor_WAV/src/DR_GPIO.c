/*******************************************************************************************************************************//**
 *
 * @file		DR_GPIO.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_GPIO.h"

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
volatile GPIO_per_t * const GPIO = (GPIO_per_t *) GPIO_BASE; 		//!< Periferico GPIO

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
 * @brief Leer estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a consultar
 * @return Estado del pin absoluto
 */
uint8_t GPIO_read_port_byte(GPIO_portpin_en portpin)
{
	return GPIO->B[portpin].PBYTE;
}

/**
 * @brief Escribir estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a escribir
 * @param[in] value Valor a escribir
 */
void GPIO_write_port_byte(GPIO_portpin_en portpin, uint8_t value)
{
	GPIO->B[portpin].PBYTE = value;
}

/**
 * @brief Leer estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a consultar
 * @return Estado del pin absoluto
 */
uint8_t GPIO_read_port_word(GPIO_portpin_en portpin)
{
	return GPIO->W[portpin].PWORD;
}

/**
 * @brief Escribir estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a escribir
 * @param[in] value Valor a escribir
 */
void GPIO_write_port_word(GPIO_portpin_en portpin, uint8_t value)
{
	GPIO->W[portpin].PWORD = value;
}

/**
 * @brief Leer registro de direccion
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
uint32_t GPIO_read_dir(GPIO_port_en port)
{
	return GPIO->DIR[port].DIRP;
}

/**
 * @brief Escribir registro de direccion
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_dir(GPIO_port_en port, uint32_t value)
{
	GPIO->DIR[port].DIRP = value;
}

/**
 * @brief Leer registro de mascara
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
uint32_t GPIO_read_mask(GPIO_port_en port)
{
	return GPIO->MASK[port].MASKP;
}

/**
 * @brief Escribir registro de mascara
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_mask(GPIO_port_en port, uint32_t value)
{
	GPIO->MASK[port].MASKP = value;
}

/**
 * @brief Leer registro de puerto/pin
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
uint32_t GPIO_read_portpin(GPIO_port_en port)
{
	return GPIO->PIN[port].PORT;
}

/**
 * @brief Escribir registro de puerto/pin
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_portpin(GPIO_port_en port, uint32_t value)
{
	GPIO->PIN[port].PORT = value;
}

/**
 * @brief Leer registro de puerto/pin enmascarado
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
uint32_t GPIO_read_masked_portpin(GPIO_port_en port)
{
	return GPIO->MPIN[port].MPORTP;
}

/**
 * @brief Escribir registro de puerto/pin enmascarado
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_masked_portpin(GPIO_port_en port, uint32_t value)
{
	GPIO->MPIN[port].MPORTP = value;
}

/**
 * @brief Escribir registro de set
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_set(GPIO_port_en port, uint32_t value)
{
	GPIO->SET[port].SETP = value;
}

/**
 * @brief Escribir registro de clear
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_clear(GPIO_port_en port, uint32_t value)
{
	GPIO->CLR[port].CLRP = value;
}

/**
 * @brief Escribir registro de toggle
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_toggle(GPIO_port_en port, uint32_t value)
{
	GPIO->NOT[port].NOTP = value;
}

/**
 * @brief Escribir registro de direction set
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_dir_set(GPIO_port_en port, uint32_t value)
{
	GPIO->DIRSET[port].DIRSETP = value;
}

/**
 * @brief Escribir registro de direction clear
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_dir_clear(GPIO_port_en port, uint32_t value)
{
	GPIO->DIRCLR[port].DIRCLRP = value;
}

/**
 * @brief Escribir registro de direction toggle
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
void GPIO_write_dir_toggle(GPIO_port_en port, uint32_t value)
{
	GPIO->DIRNOT[port].DIRNOTP = value;
}


