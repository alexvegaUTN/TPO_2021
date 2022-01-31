/*******************************************************************************************************************************//**
 *
 * @file		DR_NVIC.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_NVIC_H_
#define DR_NVIC_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define	NVIC_BASE	(0xE000E000u)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
// Enumeraciones
typedef enum
{
	NVIC_IRQ_SEL_SPI0 = 0,
	NVIC_IRQ_SEL_SPI1,
	NVIC_IRQ_SEL_DAC0,
	NVIC_IRQ_SEL_UART0,
	NVIC_IRQ_SEL_UART1,
	NVIC_IRQ_SEL_UART2,
	NVIC_IRQ_SEL_IIC1 = 7,
	NVIC_IRQ_SEL_IIC0,
	NVIC_IRQ_SEL_SCT,
	NVIC_IRQ_SEL_MRT,
	NVIC_IRQ_SEL_CMP_CAPT,
	NVIC_IRQ_SEL_WDT,
	NVIC_IRQ_SEL_BOD,
	NVIC_IRQ_SEL_FLASH,
	NVIC_IRQ_SEL_WKT,
	NVIC_IRQ_SEL_ADC_SEQA,
	NVIC_IRQ_SEL_ADC_SEQB,
	NVIC_IRQ_SEL_ADC_THCMP,
	NVIC_IRQ_SEL_ADC_OVR,
	NVIC_IRQ_SEL_DMA,
	NVIC_IRQ_SEL_IIC2,
	NVIC_IRQ_SEL_IIC3,
	NVIC_IRQ_SEL_CTIMER,
	NVIC_IRQ_SEL_PININT0,
	NVIC_IRQ_SEL_PININT1,
	NVIC_IRQ_SEL_PININT2,
	NVIC_IRQ_SEL_PININT3,
	NVIC_IRQ_SEL_PININT4,
	NVIC_IRQ_SEL_PININT5_DAC1,
	NVIC_IRQ_SEL_PININT6_UART3,
	NVIC_IRQ_SEL_PININT7_UART4
}NVIC_irq_sel_en;

typedef enum
{
	NVIC_IRQ_PRIORITY_HIGHEST = 0,
	NVIC_IRQ_PRIORITY_HIGH,
	NVIC_IRQ_PRIORITY_LOW,
	NVIC_IRQ_PRIORITY_LOWEST
}NVIC_irq_priority_en;


typedef struct
{
	uint32_t ISE_SPI0 : 1;
	uint32_t ISE_SPI1 : 1;
	uint32_t ISE_DAC0 : 1;
	uint32_t ISE_UART0 : 1;
	uint32_t ISE_UART1 : 1;
	uint32_t ISE_UART2 : 1;
	uint32_t : 1;
	uint32_t ISE_I2C1 : 1;
	uint32_t ISE_I2C0 : 1;
	uint32_t ISE_SCT : 1;
	uint32_t ISE_MRT : 1;
	uint32_t ISE_CMP : 1;
	uint32_t ISE_WDT : 1;
	uint32_t ISE_BOD : 1;
	uint32_t ISE_FLASH : 1;
	uint32_t ISE_WKT : 1;
	uint32_t ISE_ADC_SEQA : 1;
	uint32_t ISE_ADC_SEQB : 1;
	uint32_t ISE_ADC_THCMP : 1;
	uint32_t ISE_ADC_OVR : 1;
	uint32_t ISE_SDMA : 1;
	uint32_t ISE_I2C2 : 1;
	uint32_t ISE_I2C3 : 1;
	uint32_t ISE_CT32B0 : 1;
	uint32_t ISE_PININT0 : 1;
	uint32_t ISE_PININT1 : 1;
	uint32_t ISE_PININT2 : 1;
	uint32_t ISE_PININT3 : 1;
	uint32_t ISE_PININT4 : 1;
	uint32_t ISE_PININT5 : 1;
	uint32_t ISE_PININT6 : 1;
	uint32_t ISE_PININT7 : 1;
}NVIC_ISER0_reg_t;

typedef struct
{
	uint32_t ICE_SPI0 : 1;
	uint32_t ICE_SPI1 : 1;
	uint32_t ICE_DAC0 : 1;
	uint32_t ICE_UART0 : 1;
	uint32_t ICE_UART1 : 1;
	uint32_t ICE_UART2 : 1;
	uint32_t : 1;
	uint32_t ICE_I2C1 : 1;
	uint32_t ICE_I2C0 : 1;
	uint32_t ICE_SCT : 1;
	uint32_t ICE_MRT : 1;
	uint32_t ICE_CMP : 1;
	uint32_t ICE_WDT : 1;
	uint32_t ICE_BOD : 1;
	uint32_t ICE_FLASH : 1;
	uint32_t ICE_WKT : 1;
	uint32_t ICE_ADC_SEQA : 1;
	uint32_t ICE_ADC_SEQB : 1;
	uint32_t ICE_ADC_THCMP : 1;
	uint32_t ICE_ADC_OVR : 1;
	uint32_t ICE_SDMA : 1;
	uint32_t ICE_I2C2 : 1;
	uint32_t ICE_I2C3 : 1;
	uint32_t ICE_CT32b0 : 1;
	uint32_t ICE_PININT0 : 1;
	uint32_t ICE_PININT1 : 1;
	uint32_t ICE_PININT2 : 1;
	uint32_t ICE_PININT3 : 1;
	uint32_t ICE_PININT4 : 1;
	uint32_t ICE_PININT5 : 1;
	uint32_t ICE_PININT6 : 1;
	uint32_t ICE_PININT7 : 1;
}NVIC_ICER0_reg_t;

typedef struct
{
	uint32_t ISP_SPI0 : 1;
	uint32_t ISP_SPI1 : 1;
	uint32_t ISP_DAC0 : 1;
	uint32_t ISP_UART0 : 1;
	uint32_t ISP_UART1 : 1;
	uint32_t ISP_UART2 : 1;
	uint32_t : 1;
	uint32_t ISP_I2C1 : 1;
	uint32_t ISP_I2C0 : 1;
	uint32_t ISP_SCT : 1;
	uint32_t ISP_MRT : 1;
	uint32_t ISP_CMP : 1;
	uint32_t ISP_WDT : 1;
	uint32_t ISP_BOD : 1;
	uint32_t ISP_FLASH : 1;
	uint32_t ISP_WKT : 1;
	uint32_t ISP_ADC_SEQA : 1;
	uint32_t ISP_ADC_SEQB : 1;
	uint32_t ISP_ADC_THCMP : 1;
	uint32_t ISP_ADC_OVR : 1;
	uint32_t ISP_SDMA : 1;
	uint32_t ISP_I2C2 : 1;
	uint32_t ISP_I2C3 : 1;
	uint32_t ISP_CT32b0 : 1;
	uint32_t ISP_PININT0 : 1;
	uint32_t ISP_PININT1 : 1;
	uint32_t ISP_PININT2 : 1;
	uint32_t ISP_PININT3 : 1;
	uint32_t ISP_PININT4 : 1;
	uint32_t ISP_PININT5 : 1;
	uint32_t ISP_PININT6 : 1;
	uint32_t ISP_PININT7 : 1;
}NVIC_ISPR0_reg_t;

typedef struct
{
	uint32_t ICP_SPI0 : 1;
	uint32_t ICP_SPI1 : 1;
	uint32_t ICP_DAC0 : 1;
	uint32_t ICP_UART0 : 1;
	uint32_t ICP_UART1 : 1;
	uint32_t ICP_UART2 : 1;
	uint32_t : 1;
	uint32_t ICP_I2C1 : 1;
	uint32_t ICP_I2C0 : 1;
	uint32_t ICP_SCT : 1;
	uint32_t ICP_MRT : 1;
	uint32_t ICP_CMP : 1;
	uint32_t ICP_WDT : 1;
	uint32_t ICP_BOD : 1;
	uint32_t ICP_FLASH : 1;
	uint32_t ICP_WKT : 1;
	uint32_t ICP_ADC_SEQA : 1;
	uint32_t ICP_ADC_SEQB : 1;
	uint32_t ICP_ADC_THCMP : 1;
	uint32_t ICP_ADC_OVR : 1;
	uint32_t ICP_SDMA : 1;
	uint32_t ICP_I2C2 : 1;
	uint32_t ICP_I2C3 : 1;
	uint32_t ICP_CT32b0 : 1;
	uint32_t ICP_PININT0 : 1;
	uint32_t ICP_PININT1 : 1;
	uint32_t ICP_PININT2 : 1;
	uint32_t ICP_PININT3 : 1;
	uint32_t ICP_PININT4 : 1;
	uint32_t ICP_PININT5 : 1;
	uint32_t ICP_PININT6 : 1;
	uint32_t ICP_PININT7 : 1;
}NVIC_ICPR0_reg_t;

typedef struct
{
	uint32_t IAB_SPI0 : 1;
	uint32_t IAB_SPI1 : 1;
	uint32_t IAB_DAC0 : 1;
	uint32_t IAB_UART0 : 1;
	uint32_t IAB_UART1 : 1;
	uint32_t IAB_UART2 : 1;
	uint32_t : 1;
	uint32_t IAB_I2C1 : 1;
	uint32_t IAB_I2C0 : 1;
	uint32_t IAB_SCT : 1;
	uint32_t IAB_MRT : 1;
	uint32_t IAB_CMP : 1;
	uint32_t IAB_WDT : 1;
	uint32_t IAB_BOD : 1;
	uint32_t IAB_FLASH : 1;
	uint32_t IAB_WKT : 1;
	uint32_t IAB_ADC_SEQA : 1;
	uint32_t IAB_ADC_SEQB : 1;
	uint32_t IAB_ADC_THCMP : 1;
	uint32_t IAB_ADC_OVR : 1;
	uint32_t IAB_SDMA : 1;
	uint32_t IAB_I2C2 : 1;
	uint32_t IAB_I2C3 : 1;
	uint32_t IAB_CT32b0 : 1;
	uint32_t IAB_PININT0 : 1;
	uint32_t IAB_PININT1 : 1;
	uint32_t IAB_PININT2 : 1;
	uint32_t IAB_PININT3 : 1;
	uint32_t IAB_PININT4 : 1;
	uint32_t IAB_PININT5 : 1;
	uint32_t IAB_PININT6 : 1;
	uint32_t IAB_PININT7 : 1;
}NVIC_IABR0_reg_t;

typedef struct
{
	uint32_t : 6;
	uint32_t IP_SPI0 : 2;
	uint32_t : 6;
	uint32_t IP_SPI1 : 2;
	uint32_t : 6;
	uint32_t IP_DAC0 : 2;
	uint32_t : 6;
	uint32_t IP_UART0 : 2;
}NVIC_IPR0_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_UART1 : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_UART2 : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t RESERVED_4 : 8;
	uint32_t IP_I2C1 : 2;
}NVIC_IPR1_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_I2C0 : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_SCT : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t IP_MRT : 2;
	uint32_t RESERVED_4 : 6;
	uint32_t IP_CMP : 2;
}NVIC_IPR2_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_WDT : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_BOD : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t IP_FLASH : 2;
	uint32_t RESERVED_4 : 6;
	uint32_t IP_WKT : 2;
}NVIC_IPR3_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_ADC_SEQA : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_ADC_SEQB : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t IP_ADC_THCMP : 2;
	uint32_t RESERVED_4 : 6;
	uint32_t ID_ADC_OVR : 2;
}NVIC_IPR4_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_DMA : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_I2C2 : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t IP_I2C3 : 2;
	uint32_t RESERVED_4 : 6;
	uint32_t IP_CT32B0 : 2;
}NVIC_IPR5_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_PININT0 : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_PININT1 : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t IP_PININT2 : 2;
	uint32_t RESERVED_4 : 6;
	uint32_t IP_PININT3 : 2;
}NVIC_IPR6_reg_t;

typedef struct
{
	uint32_t RESERVED_1 : 6;
	uint32_t IP_PININT4 : 2;
	uint32_t RESERVED_2 : 6;
	uint32_t IP_PININT5 : 2;
	uint32_t RESERVED_3 : 6;
	uint32_t IP_PININT6 : 2;
	uint32_t RESERVED_4 : 6;
	uint32_t IP_PININT7 : 2;
}NVIC_IPR7_reg_t;

typedef struct
{
	const uint8_t RESERVED_1[0x100];
	__RW NVIC_ISER0_reg_t ISER0;
	const uint8_t RESERVED_2[0x7C];
	__RW NVIC_ICER0_reg_t ICER0;
	const uint8_t RESERVED_3[0x7C];
	__RW NVIC_ISPR0_reg_t ISPR0;
	const uint8_t RESERVED_4[0x7C];
	__RW NVIC_ICPR0_reg_t ICPR0;
	const uint8_t RESERVED_5[0x7C];
	const NVIC_IABR0_reg_t IABR0;
	const uint8_t RESERVED_6[0xFC];
	__RW NVIC_IPR0_reg_t IPR0;
	__RW NVIC_IPR1_reg_t IPR1;
	__RW NVIC_IPR2_reg_t IPR2;
	__RW NVIC_IPR3_reg_t IPR3;
	__RW NVIC_IPR4_reg_t IPR4;
	__RW NVIC_IPR5_reg_t IPR5;
	__RW NVIC_IPR6_reg_t IPR6;
	__RW NVIC_IPR7_reg_t IPR7;
}NVIC_per_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void NVIC_enable_interrupt(NVIC_irq_sel_en irq);
void NVIC_disable_interrupt(NVIC_irq_sel_en irq);
void NVIC_set_pending_interrupt(NVIC_irq_sel_en irq);
void NVIC_clear_pending_interrupt(NVIC_irq_sel_en irq);
uint8_t NVIC_get_active_interrupt(NVIC_irq_sel_en irq);
void NVIC_set_irq_priority(NVIC_irq_sel_en irq, NVIC_irq_priority_en priority);

#endif /* DR_NVIC_H_ */
