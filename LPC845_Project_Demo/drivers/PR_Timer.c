/*******************************************************************************************************************************//**
 *
 * @file		PR_Timer.c
 * @brief		Descripcion del modulo
 * @date		Oct 8, 2021
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_Timer.h"
#include "DR_Timer.h"

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
 	\author Federico Speroni
 	\date Oct 8, 2021
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void Timer_Event(void)
{
	for (uint32_t i = 0; i < MAX_TIMERS ; ++i) {
		if(timers[i].expired == YES) {
			timers[i].handler(i);
			timers[i].expired = NO;
		}
	}

	return;
}

int8_t Timer_Start(uint8_t id, uint32_t tiempo, uint8_t base, timer_handler handler)
{
	int8_t retVal = 0;

	switch (base) {
		case MSEG:
			timers[id].msecTicks = tiempo * MILISEGUNDOS;
			break;

		case SEG:
			timers[id].msecTicks = tiempo * SEGUNDOS;
			break;

		case MIN:
			timers[id].msecTicks = tiempo * MINUTOS;
			break;

		default:
			retVal = -1;
			break;
	}

	timers[id].handler = handler;
	timers[id].expired = NO;
	timers[id].active = RUN;

	return retVal;
}

int8_t Timer_Set(uint8_t id, uint32_t tiempo)
{
	int8_t retVal = 0;

	switch (timers[id].base) {
		case MSEG:
			timers[id].msecTicks = tiempo * MILISEGUNDOS;
			break;

		case SEG:
			timers[id].msecTicks = tiempo * SEGUNDOS;
			break;

		case MIN:
			timers[id].msecTicks = tiempo * MINUTOS;
			break;

		default:
			retVal = -1;
			break;
	}

	return retVal;
}

uint32_t Timer_Get(uint8_t id)
{
	uint32_t retVal = 0;

	switch (timers[id].base) {
		case MSEG:
			retVal = timers[id].msecTicks / MILISEGUNDOS;
			break;

		case SEG:
			retVal = timers[id].msecTicks / SEGUNDOS;
			break;

		case MIN:
			retVal = timers[id].msecTicks / MINUTOS;
			break;

		default: break;
	}

	return retVal;
}

void Timer_Standby(uint8_t id, uint8_t accion)
{
	timers[id].active = accion;
}

void Timer_Stop(uint8_t id)
{
	timers[id].active = STOP;
	timers[id].expired = NO;
	timers[id].msecTicks = 0;
	timers[id].base = 0;
	timers[id].handler = NULL;

	return;
}

void Timer_Close(void)
{
	for (uint8_t i = 0; i < MAX_TIMERS; ++i) {
			Timer_Stop(i);
		}

}
