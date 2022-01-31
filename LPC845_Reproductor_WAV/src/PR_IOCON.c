/*******************************************************************************************************************************//**
 *
 * @file		PR_IOCON.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_IOCON.h"
#include "DR_IOCON.h"
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
 * @brief Configuracion de un pin
 *
 * Nótese que las configuraciones de modo analógico no están en la estructura de configuración
 * @ref hal_iocon_config_t. Esto es debido a que dichas configuraciones se realizarán en el periférico
 * correspondiente, sea el @ref ADC o @ref DAC.
 *
 * @param[in] portpin Puerto/pin a configurar
 * @param[in] config Puntero a estructura de configuracion del pin
 * @pre Configuración de divisores de clock de bancos de filtro de glitches en caso de ser necesario.
 */
void iocon_config_io(gpio_portpin_en portpin, const iocon_config_t *config)
{
	uint8_t port = GPIO_PORTPIN_TO_PORT(portpin);
	uint8_t pin = GPIO_PORTPIN_TO_PIN(portpin);

	IOCON_init();

	if(portpin == kGPIO_PORTPIN_0_11)
	{
		// Este pin tiene I2C
		IOCON->PIO0_11.I2CMODE = config->i2c_mode;
	}
	else if(portpin == kGPIO_PORTPIN_0_10)
	{
		// Este pin tiene I2C
		IOCON->PIO0_10.I2CMODE = config->i2c_mode;
	}
	else
	{
		// Los pines que no tienen I2C, tienen MODE
		IOCON_PIN_TABLE[port][pin]->MODE = config->pull_mode;
	}

	IOCON_PIN_TABLE[port][pin]->HYS = config->hysteresis;
	IOCON_PIN_TABLE[port][pin]->INV = config->invert_input;
	IOCON_PIN_TABLE[port][pin]->OD = config->open_drain;
	IOCON_PIN_TABLE[port][pin]->S_MODE = config->sample_mode;
	IOCON_PIN_TABLE[port][pin]->CLK_DIV = config->clk_sel;

	IOCON_deinit();
}
