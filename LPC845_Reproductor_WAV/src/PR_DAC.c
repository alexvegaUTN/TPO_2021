/*******************************************************************************************************************************//**
 *
 * @file		PR_DAC.c
 * @brief		Descripcion del modulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_DAC.h"
#include "DR_DAC.h"
#include "DR_SYSCON.h"
#include "DR_SWM.h"
#include "DR_IOCON.h"

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
 	\date Jan 8, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Inicialización del DAC
 * Esta función inicializa el @ref IOCON de la forma necesaria para que el pin quede configurado correctamente.
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversión del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void dac_init(dac_sel_en dac, dac_settling_time_en settling_time, uint32_t initial_value)
{
	SWM_init();
	IOCON_init();

	if(dac == kDAC_SEL_0) {
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_DAC0);
		SYSCON_power_up_peripheral(SYSCON_POWER_SEL_DAC0);
		SWM_enable_DAC(dac, SWM_ENABLE);
		IOCON_enable_dac0();
	}
	else {
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_DAC1);
		SYSCON_power_up_peripheral(SYSCON_POWER_SEL_DAC1);
		SWM_enable_DAC(dac, SWM_ENABLE);
		IOCON_enable_dac1();
	}

	IOCON_deinit();
	SWM_deinit();

	DAC_config_settling_time(dac, settling_time);
	DAC_write(dac, initial_value);
}

/**
 * @brief Actualización del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 * @pre Haber inicializado el periférico
 */
void dac_update_value(dac_sel_en dac, uint16_t new_value)
{
	DAC_write(dac, new_value);
}

/**
 * @brief Configuración del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuración deseada
 * @pre Haber inicializado el periférico
 */
void dac_config_ctrl(dac_sel_en dac, dac_ctrl_config_t * config)
{
	if(config->count_enable)
		DAC_enable_timer(dac);
	else
		DAC_disable_timer(dac);

	if(config->double_buffering)
		DAC_enable_double_buffer(dac);
	else
		DAC_disable_double_buffer(dac);

	if(config->dma_enable)
		DAC_enable_DMA(dac);
	else
		DAC_disable_DMA(dac);

	if(config->dma_request)
		DAC_enable_DMA_request(dac);
	else
		DAC_disable_DMA_request(dac);
}
