/*******************************************************************************************************************************//**
 *
 * @file		DR_SYSTICK.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_SYSTICK_H_
#define DR_SYSTICK_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define	SYSTICK_BASE	(0xE000E000)		//!< Base del periferico SYSTICK

/* SysTick Reload Register Definitions */
#define SYSTICK_RELOAD_VALUE_POS             0U                                         /*!< SysTick Reload Value Position */
#define SYSTICK_RELOAD_VALUE_MSK            (0xFFFFFFUL << SYSTICK_RELOAD_VALUE_POS)    /*!< SysTick Reload Value Mask */

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef enum
{
	SYSTICK_CLOCK_SOURCE_MAIN_CLOCK_2 = 0,
	SYSTICK_CLOCK_SOURCE_MAIN_CLOCK
}SYSTICK_clock_source_en;

typedef struct
{
	uint32_t RESERVED;
}SYSTICK_RESERVED_reg_t;

typedef struct
{
	uint32_t ENABLE : 1;
	uint32_t TICKINT : 1;
	uint32_t CLKSOURCE : 1;
	uint32_t : 13;
	uint32_t COUNTFLAG : 1;
	uint32_t : 15;
}SYSTICK_CSR_reg_t;

typedef struct
{
	uint32_t RELOAD : 23;
	uint32_t : 9;
}SYSTICK_RVR_reg_t;

typedef struct
{
	uint32_t CURRENT : 23;
	uint32_t : 9;
}SYSTICK_CVR_reg_t;

typedef struct
{
	uint32_t TENMS : 23;
	uint32_t : 7;
	uint32_t SKEW : 1;
	uint32_t NOREF : 1;
}SYSTICK_CALIB_reg_t;

typedef struct
{
	const SYSTICK_RESERVED_reg_t RESERVED[4];
	__RW SYSTICK_CSR_reg_t CSR;
	__RW SYSTICK_RVR_reg_t RVR;
	__RW SYSTICK_CVR_reg_t CVR;
	__RW SYSTICK_CALIB_reg_t CALIB;
}SYSTICK_reg_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void SYSTICK_enable_count(void);
void SYSTICK_disable_count(void);
void SYSTICK_enable_interrupt(void);
void SYSTICK_disable_interrupt(void);
void SYSTICK_select_clock_source(SYSTICK_clock_source_en clock_source);
uint8_t SYSTICK_get_count_flag(void);
uint32_t SYSTICK_set_reload(uint32_t reload);
void SYSTICK_set_clear_current_value(void);
uint32_t SYSTICK_get_current_value(void);



#endif /* DR_SYSTICK_H_ */
