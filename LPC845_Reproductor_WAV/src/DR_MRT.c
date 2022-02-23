/*******************************************************************************************************************************//**
 *
 * @file		DR_MRT.c
 * @brief		Descripcion del modulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_MRT.h"

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
volatile MRT_per_t * const MRT = (MRT_per_t *) MRT_BASE; 	//!< Periferico MRT

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
 * @brief Fijar intervalo de un canal del MRT sin detener el conteo actual
 * @param[in] channel Canal a configurar
 * @param[in] interval Intervalo a cargar
 */
void MRT_set_interval(MRT_channel_sel_en channel, uint32_t interval)
{
	*((uint32_t *) &MRT->CHN[channel].INTVAL) = (interval & 0x80000000);
}

/**
 * @brief Fijar intervalo de un canal del MRT deteniendo el conteo actual inmediatamente
 * @param[in] channel Canal a configurar
 * @param[in] interval Intervalo a cargar
 */
void MRT_set_interval_and_stop_timer(MRT_channel_sel_en channel, uint32_t interval)
{
	*((uint32_t *) &MRT->CHN[channel].INTVAL) = (interval | 0x80000000);
}

/**
 * @brief Obtener el valor de la cuenta actual de un canal del MRT
 * @param[in] channel Canal a consultar
 * @return Cuenta actual
 */
uint32_t MRT_get_current_value(MRT_channel_sel_en channel)
{
	return MRT->CHN[channel].TIMER.VALUE;
}

/*
 * @brief Habilitar interrupcion en un canal del MRT
 * @param[in] channel Canal a configurar
 */
void MRT_enable_irq(MRT_channel_sel_en channel)
{
	MRT->CHN[channel].CTRL.INTEN = 1;
}

/*
 * @brief Inhabilitar interrupcion en un canal del MRT
 * @param[in] channel Canal a configurar
 */
void MRT_disable_irq(MRT_channel_sel_en channel)
{
	MRT->CHN[channel].CTRL.INTEN = 0;
}

/**
 * @brief Configurar modo de funcionamiento de un canal del MRT
 * @param[in] channel Canal a configurar
 * @param[in] mode Modo deseado
 */
void MRT_config_mode(MRT_channel_sel_en channel, MRT_mode_en mode)
{
	MRT->CHN[channel].CTRL.MODE = mode;
}

/**
 * @brief Obtener el canal que este en estado IDLE
 * @return Menor canal de los que esten en estado IDLE
 */
uint8_t MRT_get_idle_channel(void)
{
	return MRT->IDLE_CH.CHAN;
}

/**
 * @brief Obtener flag de interrupcion de un canal
 * @param[in] channel Canal a consultar
 * @return Flag actual de interrupcion del canal consultado
 */
uint8_t MRT_get_irq_flag(MRT_channel_sel_en channel)
{
	switch(channel)
	{
	case MRT_CHANNEL_0: { return MRT->IRQ_FLAG.CFLAG0; break; }
	case MRT_CHANNEL_1: { return MRT->IRQ_FLAG.CFLAG1; break; }
	case MRT_CHANNEL_2: { return MRT->IRQ_FLAG.CFLAG2; break; }
	case MRT_CHANNEL_3: { return MRT->IRQ_FLAG.CFLAG3; break; }
	}

	return 0;
}

/**
 * @brief Limpiar flag de interrupcion de un canal
 * @param[in] channel Canal a consultar
 */
void MRT_clear_irq_flag(MRT_channel_sel_en channel)
{
	switch(channel)
	{
	case MRT_CHANNEL_0: { MRT->IRQ_FLAG.CFLAG0 = 1; break; }
	case MRT_CHANNEL_1: { MRT->IRQ_FLAG.CFLAG1 = 1; break; }
	case MRT_CHANNEL_2: { MRT->IRQ_FLAG.CFLAG2 = 1; break; }
	case MRT_CHANNEL_3: { MRT->IRQ_FLAG.CFLAG3 = 1; break; }
	}

}
