/*******************************************************************************************************************************//**
 *
 * @file		DR_DAC.c
 * @brief		Descripcion del modulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_DAC.h"

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
volatile DAC_per_t * const DAC[] = { //!< Perifericos DAC
		(DAC_per_t *) DAC0_BASE,
		(DAC_per_t *) DAC1_BASE
};

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
 	\date Jan 8, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac Instancia a actualizar
 * @param[in] new_value Nuevo valor a poner en el DAC
 */
void DAC_write(DAC_sel_en dac, uint16_t new_value)
{
	DAC[dac]->CR.VALUE = new_value;
}

/**
 * @brief Configuracion del settling time del DAC
 * @param[in] dac Instancia a configurar
 * @param[in] settling_time Configuracion deseada
 */
void DAC_config_settling_time(DAC_sel_en dac, DAC_settling_time_en settling_time)
{
	DAC[dac]->CR.BIAS = settling_time;
}

/**
 * @brief Habilitar interrupcion de DMA cuando el timer tiemoutea
 * @param[in] dac Instancia a configurar
 */
void DAC_enable_DMA_request(DAC_sel_en dac)
{
	DAC[dac]->CTRL.INT_DMA_REQ = 1;
}

/**
 * @brief Inhabilitar interrupcion de DMA cuando el timer tiemoutea
 * @param[in] dac Instancia a configurar
 */
void DAC_disable_DMA_request(DAC_sel_en dac)
{
	DAC[dac]->CTRL.INT_DMA_REQ = 0;
}

/**
 * @brief Habilitar double buffering
 * @param[in] dac Instancia a configurar
 */
void DAC_enable_double_buffer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DBLBUF_ENA = 1;
}

/**
 * @brief Inhabilitar double buffering
 * @param[in] dac Instancia a configurar
 */
void DAC_disable_double_buffer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DBLBUF_ENA = 0;
}

/**
 * @brief Habilitar operacion del timer
 * @param[in] dac Instancia a configurar
 */
void DAC_enable_timer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.CNT_ENA = 1;
}

/**
 * @brief Inhabilitar operacion del timer
 * @param[in] dac Instancia a configurar
 */
void DAC_disable_timer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.CNT_ENA = 0;
}

/**
 * @brief Habilitar DMA request asociada al DAC
 * @param[in] dac Instancia a configurar
 */
void DAC_enable_DMA(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DMA_ENA = 1;
}

/**
 * @brief Inhabilitar DMA request asociada al DAC
 * @param[in] dac Instancia a configurar
 */
void DAC_disable_DMA(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DMA_ENA = 0;
}

/**
 * @brief Escribir valor a recargar para el timer de DMA
 * @param[in] dac Instancia a configurar
 * @param[in] value Valor deseado
 */
void DAC_write_reaload_value(DAC_sel_en dac, uint16_t value)
{
	DAC[dac]->CNTVAL.VALUE = value;
}
