/*******************************************************************************************************************************//**
 *
 * @file		DR_SYSTICK.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_SYSTICK.h"

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
volatile SYSTICK_reg_t * const SYSTICK = (SYSTICK_reg_t *) SYSTICK_BASE; 	//!< Periferico SYSTICK

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

/*
 * @brief Habilitar el conteo
 */
void SYSTICK_enable_count(void)
{
	SYSTICK->CSR.ENABLE = 1;
}

/*
 * @brief Inhabilitar el conteo
 */
void SYSTICK_disable_count(void)
{
	SYSTICK->CSR.ENABLE = 0;
}

/*
 * @brief Habilitar interrupcion de tick
 */
void SYSTICK_enable_interrupt(void)
{
	SYSTICK->CSR.TICKINT = 1;
}

/*
 * @brief Inhabilitar interrupcion de tick
 */
void SYSTICK_disable_interrupt(void)
{
	SYSTICK->CSR.TICKINT = 0;
}

/**
 * @brief Seleccion de fuente de clock
 * @param[in] clock_source Fuente deseada
 */
void SYSTICK_select_clock_source(SYSTICK_clock_source_en clock_source)
{
	SYSTICK->CSR.CLKSOURCE = clock_source;
}

/**
 * @brief Obtener flag de conteo terminado
 * @return Si el SYSTICK habia terminado la cuenta antes de leer el registro, devuelve 1
 */
uint8_t SYSTICK_get_count_flag(void)
{
	return SYSTICK->CSR.COUNTFLAG;
}

/**
 * @brief Fijar el valor de reload
 */
uint32_t SYSTICK_set_reload(uint32_t reload)
{
	uint32_t val = 1;

	if ((reload - 1) <= SYSTICK_RELOAD_VALUE_MSK) {
		*((uint32_t *)(&SYSTICK->RVR)) = 0;
		SYSTICK->RVR.RELOAD = reload - 1;
		val = 0;
	}

	return val;
}

/**
 * @brief Limpiar el conteo actual
 */
void SYSTICK_set_clear_current_value(void)
{
	SYSTICK->CVR.CURRENT = 1;
}

/**
 * @brief Devuelve el valor actual del contador
 */
uint32_t SYSTICK_get_current_value(void)
{
	return SYSTICK->CVR.CURRENT;
}
