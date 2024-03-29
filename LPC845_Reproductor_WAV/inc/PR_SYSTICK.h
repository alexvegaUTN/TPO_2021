/*******************************************************************************************************************************//**
 *
 * @file		PR_SYSTICK.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 10, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_SYSTICK_H_
#define PR_SYSTICK_H_

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

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/** Tipo de dato para callbacks del *SYSTICK* */
typedef void (*p_systick_callback_t)(void);

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile uint32_t systick_period_ms;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

/**
 * @brief Inicializacion del SYSTICK
 * @param[in] tick_us Tiempo en microsegundos deseado para el tick
 * @param[in] callback Funcion a llamar en cada tick
 */
void systick_init(uint32_t tick_us, void (*callback)(void));

/**
 * @brief Actualizar callback del SYSTICK
 * @param[in] callback Nuevo callback a ejecutar en cada tick
 *
 * Si se pasa como parámetro *NULL*, se inhabilitarán las interrupciones
 */
void systick_update_callback(p_systick_callback_t callback);

/**
 * @brief Inhabilitar interrupciones de *SYSTICK*
 */
void systick_inhibit_set(void);

/**
 * @brief Habilitar interrupciones de *SYSTICK*
 */
void systick_inhibit_clear(void);


void systick_set_period_ms(uint32_t tick_us);
uint32_t systick_get_period_ms(void);
uint32_t systick_get_current_value(void);

#endif /* PR_SYSTICK_H_ */
