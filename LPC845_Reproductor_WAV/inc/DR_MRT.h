/*******************************************************************************************************************************//**
 *
 * @file		DR_MRT.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_MRT_H_
#define DR_MRT_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define		MRT_BASE		0x40004000

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Enumeraciones
 **********************************************************************************************************************************/
typedef enum
{
	MRT_CHANNEL_0 = 0,
	MRT_CHANNEL_1,
	MRT_CHANNEL_2,
	MRT_CHANNEL_3
}MRT_channel_sel_en;

typedef enum
{
	MRT_MODE_REPEAT = 0,
	MRT_MODE_ONE_SHOT,
	MRT_MODE_ONE_SHOT_BUS_STALL
}MRT_mode_en;

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Estructuras
 **********************************************************************************************************************************/
typedef struct
{
	uint32_t LVALUE : 31;
	uint32_t LOAD : 1;
}MRT_INTVAL_reg_t;

typedef struct
{
	uint32_t VALUE : 31;
	uint32_t : 1;
}MRT_TIMER_reg_t;

typedef struct
{
	uint32_t INTEN : 1;
	uint32_t MODE : 2;
	uint32_t : 29;
}MRT_CTRL_reg_t;

typedef struct
{
	uint32_t INTFLAG : 1;
	uint32_t RUN : 1;
	uint32_t : 30;
}MRT_STAT_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t CHAN : 4;
	uint32_t : 24;
}MRT_IDLE_CH_reg_t;

typedef struct
{
	uint32_t CFLAG0 : 1;
	uint32_t CFLAG1 : 1;
	uint32_t CFLAG2 : 1;
	uint32_t CFLAG3 : 1;
	uint32_t : 28;
}MRT_IRQ_FLAG_reg_t;

typedef struct
{
	MRT_INTVAL_reg_t INTVAL;
	const MRT_TIMER_reg_t TIMER;
	MRT_CTRL_reg_t CTRL;
	MRT_STAT_reg_t STAT;
}MRT_CHN_reg_t;

typedef struct
{
	__RW MRT_CHN_reg_t CHN[4];
	const uint32_t RESERVED;
	__R MRT_IDLE_CH_reg_t IDLE_CH;
	__RW MRT_IRQ_FLAG_reg_t IRQ_FLAG;
}MRT_per_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void MRT_set_interval(MRT_channel_sel_en channel, uint32_t interval);
void MRT_set_interval_and_stop_timer(MRT_channel_sel_en channel, uint32_t interval);
uint32_t MRT_get_current_value(MRT_channel_sel_en channel);
void MRT_enable_irq(MRT_channel_sel_en channel);
void MRT_disable_irq(MRT_channel_sel_en channel);
void MRT_config_mode(MRT_channel_sel_en channel, MRT_mode_en mode);
uint8_t MRT_get_idle_channel(void);
uint8_t MRT_get_irq_flag(MRT_channel_sel_en channel);
void MRT_clear_irq_flag(MRT_channel_sel_en channel);

#endif /* DR_MRT_H_ */
