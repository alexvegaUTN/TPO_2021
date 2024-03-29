/*******************************************************************************************************************************//**
 *
 * @file		PR_Leds.c
 * @brief		Descripcion del modulo
 * @date		Nov 26, 2021
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <PR_Leds.h>
#include "PR_Timer.h"

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
 	\date Nov 26, 2021
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

void leds_init(void)
{
	gpio_set_dir(LED_VERDE_PORT_PIN, kGPIO_DIR_OUTPUT, Stick_Led_OFF);
	gpio_set_dir(LED_AZUL_PORT_PIN, kGPIO_DIR_OUTPUT, Stick_Led_OFF);
	gpio_set_dir(LED_ROJO_PORT_PIN, kGPIO_DIR_OUTPUT, Stick_Led_OFF);

	gpio_set_dir(LED_1_PORT_PIN, kGPIO_DIR_OUTPUT, Board_Led_OFF);

	//GPIO_SetDir_v1 ( DIG_OUT3, OUTPUT );
	//GPIO_SetPin_v1 ( DIG_OUT3 , OFF );
}


void led_verde_on(void)
{
	gpio_set_pin_active_low(LED_VERDE_PORT_PIN);
}

void led_verde_off(void)
{
	gpio_clear_pin_active_low(LED_VERDE_PORT_PIN);
}

void led_verde_toggle(void)
{
	gpio_toggle_pin(LED_VERDE_PORT_PIN);
}

void led_azul_on(void)
{
	gpio_set_pin_active_low(LED_AZUL_PORT_PIN);
}

void led_azul_off(void)
{
	gpio_clear_pin_active_low(LED_AZUL_PORT_PIN);
}

void led_azul_toggle(void)
{
	gpio_toggle_pin(LED_AZUL_PORT_PIN);
}

void led_rojo_on(void)
{
	gpio_set_pin_active_low(LED_ROJO_PORT_PIN);
}

void led_rojo_off(void)
{
	gpio_clear_pin_active_low(LED_ROJO_PORT_PIN);
}

void led_rojo_toggle(void)
{
	gpio_toggle_pin(LED_ROJO_PORT_PIN);
}



