/*******************************************************************************************************************************//**
 *
 * @file		DR_SWM.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_SWM_H_
#define DR_SWM_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_Tipos.h"
#include "DR_SYSCON.h"
#include "DR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/** Peripheral SWM0 base address */
#define 	SWM_BASE                   (0x4000C000u)

#define		PINENABLE_XTALIN_ON			0
#define		PINENABLE_XTALIN_OFF		1
#define		PINENABLE_XTALOUT_ON		0
#define		PINENABLE_XTALOUT_OFF		1

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef enum
{
	SWM_ENABLE = 0,
	SWM_DISABLE
}SWM_enable_en;

typedef struct
{
	uint32_t U0_TXD_O : 8;
	uint32_t U0_RXD_I : 8;
	uint32_t U0_RTS_O : 8;
	uint32_t U0_CTS_I : 8;
}SWM_PINASSIGN0_reg_t;

typedef struct
{
	uint32_t U0_SCLK_IO : 8;
	uint32_t U1_TXD_O : 8;
	uint32_t U1_RXD_I : 8;
	uint32_t U1_RTS_O : 8;
}SWM_PINASSIGN1_reg_t;

typedef struct
{
	uint32_t U1_CTS_I : 8;
	uint32_t U1_SCLK_IO : 8;
	uint32_t U2_TXD_O : 8;
	uint32_t U2_RXD_I : 8;
}SWM_PINASSIGN2_reg_t;

typedef struct
{
	uint32_t U2_RTS_O : 8;
	uint32_t U2_CTS_I : 8;
	uint32_t U2_SCLK_IO : 8;
	uint32_t SPI0_SCK_IO : 8;
}SWM_PINASSIGN3_reg_t;

typedef struct
{
	uint32_t SPI0_MOSI_IO : 8;
	uint32_t SPI0_MISO_IO : 8;
	uint32_t SPI0_SSEL0_IO : 8;
	uint32_t SPI0_SSEL1_IO : 8;
}SWM_PINASSIGN4_reg_t;

typedef struct
{
	uint32_t SPI0_SSEL2_IO : 8;
	uint32_t SPI0_SSEL3_IO : 8;
	uint32_t SPI1_SCK_IO : 8;
	uint32_t SPI1_MOSI_IO : 8;
}SWM_PINASSIGN5_reg_t;

typedef struct
{
	uint32_t SPI1_MISO_IO : 8;
	uint32_t SPI1_SSEL0_IO : 8;
	uint32_t SPI1_SSEL1_IO : 8;
	uint32_t SCT0_GPIO_IN_A_I : 8;
}SWM_PINASSIGN6_reg_t;

typedef struct
{
	uint32_t SCT0_GPIO_IN_B_I : 8;
	uint32_t SCT0_GPIO_IN_C_I : 8;
	uint32_t SCT0_GPIO_IN_D_I : 8;
	uint32_t SCT_OUT0_O : 8;
}SWM_PINASSIGN7_reg_t;

typedef struct
{
	uint32_t SCT_OUT1_O : 8;
	uint32_t SCT_OUT2_O : 8;
	uint32_t SCT_OUT3_O : 8;
	uint32_t SCT_OUT4_O : 8;
}SWM_PINASSIGN8_reg_t;

typedef struct
{
	uint32_t SCT_OUT5_O : 8;
	uint32_t SCT_OUT6_O : 8;
	uint32_t I2C1_SDA_IO : 8;
	uint32_t I2C1_SCL_IO : 8;
}SWM_PINASSIGN9_reg_t;

typedef struct
{
	uint32_t I2C2_SDA_IO : 8;
	uint32_t I2C2_SCL_IO : 8;
	uint32_t I2C3_SDA_IO : 8;
	uint32_t I2C3_SCL_IO : 8;
}SWM_PINASSIGN10_reg_t;

typedef struct
{
	uint32_t COMP0_OUT_O : 8;
	uint32_t CLKOUT_O : 8;
	uint32_t GPIO_INT_BMAT_O : 8;
	uint32_t UART3_TXD : 8;
}SWM_PINASSIGN11_reg_t;

typedef struct
{
	uint32_t UART3_RXD : 8;
	uint32_t UART3_SCLK : 8;
	uint32_t UART4_TXD : 8;
	uint32_t UART4_RXD : 8;
}SWM_PINASSIGN12_reg_t;

typedef struct
{
	uint32_t UART4_SCLK : 8;
	uint32_t T0_MAT0 : 8;
	uint32_t T0_MAT1 : 8;
	uint32_t T0_MAT2 : 8;
}SWM_PINASSIGN13_reg_t;

typedef struct
{
	uint32_t T0_MAT3 : 8;
	uint32_t T0_CAP0 : 8;
	uint32_t T0_CAP1 : 8;
	uint32_t T0_CAP2 : 8;
}SWM_PINASSIGN14_reg_t;

typedef struct
{
	uint32_t ACMP_I1 : 1;
	uint32_t ACMP_I2 : 1;
	uint32_t ACMP_I3 : 1;
	uint32_t ACMP_I4 : 1;
	uint32_t ACMP_I5 : 1;
	uint32_t SWCLK : 1;
	uint32_t SWDIO : 1;
	uint32_t XTALIN : 1;
	uint32_t XTALOUT : 1;
	uint32_t RESETN : 1;
	uint32_t CLKIN : 1;
	uint32_t VDDCMP : 1;
	uint32_t I2C0_SDA : 1;
	uint32_t I2C0_SCL : 1;
	uint32_t ADC_0 : 1;
	uint32_t ADC_1 : 1;
	uint32_t ADC_2 : 1;
	uint32_t ADC_3 : 1;
	uint32_t ADC_4 : 1;
	uint32_t ADC_5 : 1;
	uint32_t ADC_6 : 1;
	uint32_t ADC_7 : 1;
	uint32_t ADC_8 : 1;
	uint32_t ADC_9 : 1;
	uint32_t ADC_10 : 1;
	uint32_t ADC_11 : 1;
	uint32_t DACOUT0 : 1;
	uint32_t DACOUT1 : 1;
	uint32_t CAPT_X0 : 1;
	uint32_t CAPT_X1 : 1;
	uint32_t CAPT_X2 : 1;
	uint32_t CAPT_X3 : 1;
}SWM_PINENABLE0_reg_t;

typedef struct
{
	uint32_t CAPT_X4 : 1;
	uint32_t CAPT_X5 : 1;
	uint32_t CAPT_X6 : 1;
	uint32_t CAPT_X7 : 1;
	uint32_t CAPT_X8 : 1;
	uint32_t CAPT_YL : 1;
	uint32_t CAPT_YH : 1;
	uint32_t : 26;
}SWM_PINENABLE1_reg_t;

typedef struct
{
	__RW SWM_PINASSIGN0_reg_t PINASSIGN0;
	__RW SWM_PINASSIGN1_reg_t PINASSIGN1;
	__RW SWM_PINASSIGN2_reg_t PINASSIGN2;
	__RW SWM_PINASSIGN3_reg_t PINASSIGN3;
	__RW SWM_PINASSIGN4_reg_t PINASSIGN4;
	__RW SWM_PINASSIGN5_reg_t PINASSIGN5;
	__RW SWM_PINASSIGN6_reg_t PINASSIGN6;
	__RW SWM_PINASSIGN7_reg_t PINASSIGN7;
	__RW SWM_PINASSIGN8_reg_t PINASSIGN8;
	__RW SWM_PINASSIGN9_reg_t PINASSIGN9;
	__RW SWM_PINASSIGN10_reg_t PINASSIGN10;
	__RW SWM_PINASSIGN11_reg_t PINASSIGN11;
	__RW SWM_PINASSIGN12_reg_t PINASSIGN12;
	__RW SWM_PINASSIGN13_reg_t PINASSIGN13;
	__RW SWM_PINASSIGN14_reg_t PINASSIGN14;
	__R uint32_t RESERVED[(0x188 - 4) / 4];
	__RW SWM_PINENABLE0_reg_t PINENABLE0;
	__RW SWM_PINENABLE1_reg_t PINENABLE1;
}SWM_per_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile SYSCON_per_t * const SYSCON; //!< Periferico SYSCON

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void SWM_init(void);
void SWM_deinit(void);
void SWM_assign_uart_TXD(uint8_t uart, uint8_t port, uint8_t pin);
void SWM_assign_uart_RXD(uint8_t uart, uint8_t port, uint8_t pin);
void SWM_assign_uart_RTS(uint8_t uart, uint8_t port, uint8_t pin);
void SWM_assign_uart_CTS(uint8_t uart, uint8_t port, uint8_t pin);
void SWM_assign_uart_SCLK(uint8_t uart, uint8_t port, uint8_t pin);
void SWM_assign_spi_SCK(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_spi_MOSI(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_spi_MISO(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_spi_SSEL0(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_spi_SSEL1(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_spi_SSEL2(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_spi_SSEL3(uint8_t spi, uint8_t port, uint8_t pin);
void SWM_assign_sct_IN_A(uint8_t port, uint8_t pin);
void SWM_assign_sct_IN_B(uint8_t port, uint8_t pin);
void SWM_assign_sct_IN_C(uint8_t port, uint8_t pin);
void SWM_assign_sct_IN_D(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT0(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT1(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT2(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT3(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT4(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT5(uint8_t port, uint8_t pin);
void SWM_assign_sct_OUT6(uint8_t port, uint8_t pin);
void SWM_assign_i2c_SDA(uint8_t i2c, uint8_t port, uint8_t pin);
void SWM_assign_i2c_SCL(uint8_t i2c, uint8_t port, uint8_t pin);
void SWM_assign_COMP0_OUT(uint8_t port, uint8_t pin);
void SWM_assign_CLKOUT(GPIO_portpin_en portpin);
void SWM_assign_INT_BMAT(uint8_t port, uint8_t pin);
void SWM_assign_T0_MAT(uint8_t mat, uint8_t port, uint8_t pin);
void SWM_assign_T0_CAP(uint8_t cap, uint8_t port, uint8_t pin);
void SWM_enable_ACMP(uint8_t acmp, SWM_enable_en en_dis);
void SWM_enable_SWCLK(SWM_enable_en en_dis);
void SWM_enable_SWDIO(SWM_enable_en en_dis);
void SWM_enable_XTALIN(SWM_enable_en en_dis);
void SWM_enable_XTALOUT(SWM_enable_en en_dis);
void SWM_enable_RESETN(SWM_enable_en en_dis);
void SWM_enable_CLKIN(SWM_enable_en en_dis);
void SWM_enable_VDDCMP(SWM_enable_en en_dis);
void SWM_enable_ADC(uint8_t adc, SWM_enable_en en_dis);
void SWM_enable_DAC(uint8_t dac, SWM_enable_en en_dis);
void SWM_enable_CAPTX(uint8_t captx, SWM_enable_en en_dis);
void SWM_enable_CAPYL(SWM_enable_en en_dis);
void SWM_enable_CAPYH(SWM_enable_en en_dis);

#endif /* DR_SWM_H_ */
