/*******************************************************************************************************************************//**
 *
 * @file		DR_IOCON.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_IOCON_H_
#define DR_IOCON_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"
#include "DR_SYSCON.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/* IOCON - Peripheral instance base addresses */
/** Peripheral IOCON base address */
#define IOCON_BASE                               (0x40044000u)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef enum
{
	IOCON_PULL_NONE = 0,
	IOCON_PULL_DOWN,
	IOCON_PULL_UP,
	IOCON_PULL_REPEATER
}IOCON_pull_mode_en;

typedef enum
{
	IOCON_SAMPLE_MODE_BYPASS = 0,
	IOCON_SAMPLE_MODE_1_CLOCK,
	IOCON_SAMPLE_MODE_2_CLOCK,
	IOCON_SAMPLE_MODE_3_CLOCK
}IOCON_sample_mode_en;

typedef enum
{
	IOCON_CLK_DIV_0 = 0,
	IOCON_CLK_DIV_1,
	IOCON_CLK_DIV_2,
	IOCON_CLK_DIV_3,
	IOCON_CLK_DIV_4,
	IOCON_CLK_DIV_5,
	IOCON_CLK_DIV_6
}IOCON_clk_sel_en;

typedef enum
{
	IOCON_IIC_MODE_STANDARD = 0,
	IOCON_IIC_MODE_GPIO,
	IOCON_IIC_MODE_FAST_MODE
}IOCON_iic_mode_en;

/*
 * Nota sobre la siguiente estructura:
 *
 * No todos los pines admiten configuracion de ADC o I2C.
 * Esto sera tomado en cuenta a la hora de configurar cada pin.
 *
 */
typedef struct
{
	uint32_t : 3;
	uint32_t MODE : 2;
	uint32_t HYS : 1;
	uint32_t INV : 1;
	uint32_t I2CMODE : 2;
	uint32_t : 1;
	uint32_t OD : 1;
	uint32_t S_MODE : 2;
	uint32_t CLK_DIV : 3;
	uint32_t DACMODE : 1;
	uint32_t : 15;
}IOCON_PIO_reg_t;

typedef struct
{
	__RW IOCON_PIO_reg_t PIO0_17;
	__RW IOCON_PIO_reg_t PIO0_13;
	__RW IOCON_PIO_reg_t PIO0_12;
	__RW IOCON_PIO_reg_t PIO0_5;
	__RW IOCON_PIO_reg_t PIO0_4;
	__RW IOCON_PIO_reg_t PIO0_3;
	__RW IOCON_PIO_reg_t PIO0_2;
	__RW IOCON_PIO_reg_t PIO0_11;
	__RW IOCON_PIO_reg_t PIO0_10;
	__RW IOCON_PIO_reg_t PIO0_16;
	__RW IOCON_PIO_reg_t PIO0_15;
	__RW IOCON_PIO_reg_t PIO0_1;
	const uint32_t _RESERVED_1;
	__RW IOCON_PIO_reg_t PIO0_9;
	__RW IOCON_PIO_reg_t PIO0_8;
	__RW IOCON_PIO_reg_t PIO0_7;
	__RW IOCON_PIO_reg_t PIO0_6;
	__RW IOCON_PIO_reg_t PIO0_0;
	__RW IOCON_PIO_reg_t PIO0_14;
	const uint32_t _RESERVED_2;
	__RW IOCON_PIO_reg_t PIO0_28;
	__RW IOCON_PIO_reg_t PIO0_27;
	__RW IOCON_PIO_reg_t PIO0_26;
	__RW IOCON_PIO_reg_t PIO0_25;
	__RW IOCON_PIO_reg_t PIO0_24;
	__RW IOCON_PIO_reg_t PIO0_23;
	__RW IOCON_PIO_reg_t PIO0_22;
	__RW IOCON_PIO_reg_t PIO0_21;
	__RW IOCON_PIO_reg_t PIO0_20;
	__RW IOCON_PIO_reg_t PIO0_19;
	__RW IOCON_PIO_reg_t PIO0_18;
	__RW IOCON_PIO_reg_t PIO1_8;
	__RW IOCON_PIO_reg_t PIO1_9;
	__RW IOCON_PIO_reg_t PIO1_12;
	__RW IOCON_PIO_reg_t PIO1_13;
	__RW IOCON_PIO_reg_t PIO0_31;
	__RW IOCON_PIO_reg_t PIO1_0;
	__RW IOCON_PIO_reg_t PIO1_1;
	__RW IOCON_PIO_reg_t PIO1_2;
	__RW IOCON_PIO_reg_t PIO1_14;
	__RW IOCON_PIO_reg_t PIO1_15;
	__RW IOCON_PIO_reg_t PIO1_3;
	__RW IOCON_PIO_reg_t PIO1_4;
	__RW IOCON_PIO_reg_t PIO1_5;
	__RW IOCON_PIO_reg_t PIO1_16;
	__RW IOCON_PIO_reg_t PIO1_17;
	__RW IOCON_PIO_reg_t PIO1_6;
	__RW IOCON_PIO_reg_t PIO1_18;
	__RW IOCON_PIO_reg_t PIO1_19;
	__RW IOCON_PIO_reg_t PIO1_7;
	__RW IOCON_PIO_reg_t PIO0_29;
	__RW IOCON_PIO_reg_t PIO0_30;
	__RW IOCON_PIO_reg_t PIO1_20;
	__RW IOCON_PIO_reg_t PIO1_21;
	__RW IOCON_PIO_reg_t PIO1_11;
	__RW IOCON_PIO_reg_t PIO1_10;
}IOCON_per_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void IOCON_init(void);
void IOCON_deinit(void);
void IOCON_config_pull_mode(uint8_t port, uint8_t pin, IOCON_pull_mode_en pull_mode);
void IOCON_enable_hysteresis(uint8_t port, uint8_t pin);
void IOCON_disable_hysteresis(uint8_t port, uint8_t pin);
void IOCON_enable_invert(uint8_t port, uint8_t pin);
void IOCON_disable_invert(uint8_t port, uint8_t pin);
void IOCON_enable_open_drain(uint8_t port, uint8_t pin);
void IOCON_disable_open_drain(uint8_t port, uint8_t pin);
void IOCON_config_sample_mode(uint8_t port, uint8_t pin, IOCON_sample_mode_en sample_mode);
void IOCON_config_clock_source(uint8_t port, uint8_t pin, IOCON_clk_sel_en clock_source);
void IOCON_disable_pullup_adc(uint8_t channel);
void IOCON_disable_pullup_acmp_input(uint8_t acmp_input);
void IOCON_disable_pullup_acmp_ladder_external_ref_VDDCMP(void);
void IOCON_enable_dac0(void);
void IOCON_enable_dac1(void);
void IOCON_disable_dac0(void);
void IOCON_disable_dac1(void);
void IOCON_select_i2c0_scl(IOCON_iic_mode_en iic_mode);
void IOCON_select_i2c0_sda(IOCON_iic_mode_en iic_mode);

#endif /* DR_IOCON_H_ */
