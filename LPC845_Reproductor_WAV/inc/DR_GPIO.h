/*******************************************************************************************************************************//**
 *
 * @file		DR_GPIO.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_GPIO_H_
#define DR_GPIO_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/** Peripheral GPIO base address */
#define GPIO_BASE                                (0x0A0000000u)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef enum
{
	GPIO_DIR_INPUT = 0,
	GPIO_DIR_OUTPUT
}GPIO_dir_en;

typedef enum
{
	GPIO_PORT_0 = 0,
	GPIO_PORT_1
}GPIO_port_en;

typedef enum
{
	GPIO_PORTPIN_0_0 = 0,
	GPIO_PORTPIN_0_1,
	GPIO_PORTPIN_0_2,
	GPIO_PORTPIN_0_3,
	GPIO_PORTPIN_0_4,
	GPIO_PORTPIN_0_5,
	GPIO_PORTPIN_0_6,
	GPIO_PORTPIN_0_7,
	GPIO_PORTPIN_0_8,
	GPIO_PORTPIN_0_9,
	GPIO_PORTPIN_0_10,
	GPIO_PORTPIN_0_11,
	GPIO_PORTPIN_0_12,
	GPIO_PORTPIN_0_13,
	GPIO_PORTPIN_0_14,
	GPIO_PORTPIN_0_15,
	GPIO_PORTPIN_0_16,
	GPIO_PORTPIN_0_17,
	GPIO_PORTPIN_0_18,
	GPIO_PORTPIN_0_19,
	GPIO_PORTPIN_0_20,
	GPIO_PORTPIN_0_21,
	GPIO_PORTPIN_0_22,
	GPIO_PORTPIN_0_23,
	GPIO_PORTPIN_0_24,
	GPIO_PORTPIN_0_25,
	GPIO_PORTPIN_0_26,
	GPIO_PORTPIN_0_27,
	GPIO_PORTPIN_0_28,
	GPIO_PORTPIN_0_29,
	GPIO_PORTPIN_0_30,
	GPIO_PORTPIN_0_31,
	GPIO_PORTPIN_1_0,
	GPIO_PORTPIN_1_1,
	GPIO_PORTPIN_1_2,
	GPIO_PORTPIN_1_3,
	GPIO_PORTPIN_1_4,
	GPIO_PORTPIN_1_5,
	GPIO_PORTPIN_1_6,
	GPIO_PORTPIN_1_7,
	GPIO_PORTPIN_1_8,
	GPIO_PORTPIN_1_9,
	GPIO_PORTPIN_1_10,
	GPIO_PORTPIN_1_11,
	GPIO_PORTPIN_1_12,
	GPIO_PORTPIN_1_13,
	GPIO_PORTPIN_1_14,
	GPIO_PORTPIN_1_15,
	GPIO_PORTPIN_1_16,
	GPIO_PORTPIN_1_17,
	GPIO_PORTPIN_1_18,
	GPIO_PORTPIN_1_19,
	GPIO_PORTPIN_1_20,
	GPIO_PORTPIN_1_21
}GPIO_portpin_en;

typedef struct
{
	uint8_t PBYTE : 1;
	uint8_t : 7;
}GPIO_B_reg_t;

typedef struct
{
	uint32_t PWORD;
}GPIO_W_reg_t;

typedef struct
{
	uint32_t DIRP;
}GPIO_DIR_reg_t;

typedef struct
{
	uint32_t MASKP;
}GPIO_MASK_reg_t;

typedef struct
{
	uint32_t PORT;
}GPIO_PIN_reg_t;

typedef struct
{
	uint32_t MPORTP;
}GPIO_MPIN_reg_t;

typedef struct
{
	uint32_t SETP;
}GPIO_SET_reg_t;

typedef struct
{
	uint32_t CLRP;
}GPIO_CLR_reg_t;

typedef struct
{
	uint32_t NOTP;
}GPIO_NOT_reg_t;

typedef struct
{
	uint32_t DIRSETP;
}GPIO_DIRSET_reg_t;

typedef struct
{
	uint32_t DIRCLRP;
}GPIO_DIRCLR_reg_t;

typedef struct
{
	uint32_t DIRNOTP;
}GPIO_DIRNOT_reg_t;

typedef struct
{
	__RW GPIO_B_reg_t B[54];
	const uint8_t RESERVED_1[0xFCA];
	__RW GPIO_W_reg_t W[54];
	const uint8_t RESERVED_2[0xF28];
	__RW GPIO_DIR_reg_t DIR[2];
	const uint8_t RESERVED_3[0x78];
	__RW GPIO_MASK_reg_t MASK[2];
	const uint8_t RESERVED_4[0x78];
	__RW GPIO_PIN_reg_t PIN[2];
	const uint8_t RESERVED_5[0x78];
	__RW GPIO_MPIN_reg_t MPIN[2];
	const uint8_t RESERVED_6[0x75];
	__RW GPIO_SET_reg_t SET[2];
	const uint8_t RESERVED_7[0x78];
	__RW GPIO_CLR_reg_t CLR[2];
	const uint8_t RESERVED_8[0x78];
	__RW GPIO_NOT_reg_t NOT[2];
	const uint8_t RESERVED_9[0x78];
	__RW GPIO_DIRSET_reg_t DIRSET[2];
	const uint8_t RESERVED_10[0x78];
	__RW GPIO_DIRCLR_reg_t DIRCLR[2];
	const uint8_t RESERVED_11[0x78];
	__RW GPIO_DIRNOT_reg_t DIRNOT[2];
}GPIO_per_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
uint8_t GPIO_read_port_byte(GPIO_portpin_en portpin);
void GPIO_write_port_byte(GPIO_portpin_en portpin, uint8_t value);
uint8_t GPIO_read_port_word(GPIO_portpin_en portpin);
void GPIO_write_port_word(GPIO_portpin_en portpin, uint8_t value);
uint32_t GPIO_read_dir(GPIO_port_en port);
void GPIO_write_dir(GPIO_port_en port, uint32_t value);
uint32_t GPIO_read_mask(GPIO_port_en port);
void GPIO_write_mask(GPIO_port_en port, uint32_t value);
uint32_t GPIO_read_portpin(GPIO_port_en port);
void GPIO_write_portpin(GPIO_port_en port, uint32_t value);
uint32_t GPIO_read_masked_portpin(GPIO_port_en port);
void GPIO_write_masked_portpin(GPIO_port_en port, uint32_t value);
void GPIO_write_set(GPIO_port_en port, uint32_t value);
void GPIO_write_clear(GPIO_port_en port, uint32_t value);
void GPIO_write_toggle(GPIO_port_en port, uint32_t value);
void GPIO_write_dir_set(GPIO_port_en port, uint32_t value);
void GPIO_write_dir_clear(GPIO_port_en port, uint32_t value);
void GPIO_write_dir_toggle(GPIO_port_en port, uint32_t value);

#endif /* DR_GPIO_H_ */
