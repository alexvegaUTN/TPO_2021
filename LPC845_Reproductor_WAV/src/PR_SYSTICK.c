/*******************************************************************************************************************************//**
 *
 * @file		PR_SYSTICK.c
 * @brief		Descripcion del modulo
 * @date		Jan 10, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <stddef.h>
#include "PR_SYSTICK.h"
#include "PR_SYSCON.h"
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
volatile uint32_t systick_period_ms = 0;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
static void dummy_irq(void);
static void (*systick_callback)(void) = dummy_irq; 	//!< Callback a llamar en la interrupcion

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
 	\date Jan 10, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Inicializacion del SYSTICK
 * @param[in] tick_us Tiempo en microsegundos deseado para el tick
 * @param[in] callback Funcion a llamar en cada tick
 */
void systick_init(uint32_t tick_us, void (*callback)(void))
{
	double aux_f;
	uint32_t aux;

	// En base a los us deseados calculo el valor de STRELOAD
	aux_f = ((double) syscon_system_clock_get()) * ((double) tick_us);
	aux = aux_f / 1e6;

	aux--;

	if(aux >= (1 << 24)) {
		// En este caso habria overflow. Lo fijo al maximo.
		aux = (1 << 24) - 1;
	}

	systick_set_period_ms(tick_us);

	SYSTICK_set_reload(aux);
	SYSTICK_select_clock_source(SYSTICK_CLOCK_SOURCE_MAIN_CLOCK);
	SYSTICK_set_clear_current_value();
	SYSTICK_enable_count();

	systick_update_callback(callback);
}

/**
 * @brief Actualizar callback del SYSTICK
 * @param[in] callback Nuevo callback a ejecutar en cada tick
 *
 * Si se pasa como parámetro *NULL*, se inhabilitarán las interrupciones
 */
void systick_update_callback(void (*callback)(void))
{
	if(callback != NULL)
	{
		systick_callback = callback;
		SYSTICK_enable_interrupt();
	}
	else
	{
		systick_callback = dummy_irq;
		SYSTICK_disable_interrupt();
	}
}

/**
 * @brief Inhabilitar interrupciones de *SYSTICK*
 */
void systick_inhibit_set(void)
{
	SYSTICK_disable_interrupt();
}

/**
 * @brief Habilitar interrupciones de *SYSTICK*
 */
void systick_inhibit_clear(void)
{
	SYSTICK_enable_interrupt();
}

/**
 * @brief Interrupcion de SYSTICK
 */
void SysTick_Handler(void)
{
	systick_callback();
}

/**
 * @brief Dummy function para inicializar los punteros a los callbacks
 */
static void dummy_irq(void)
{
	return;
}

void systick_set_period_ms(uint32_t tick_us)
{
	systick_period_ms = tick_us / 1000;
}

uint32_t systick_get_period_ms(void)
{
	return systick_period_ms;
}

uint32_t systick_get_current_value(void)
{
	return SYSTICK_get_current_value();
}
