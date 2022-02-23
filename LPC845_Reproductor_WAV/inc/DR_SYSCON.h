/*******************************************************************************************************************************//**
 *
 * @file		DR_SYSCON.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_SYSCON_H_
#define DR_SYSCON_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include "DR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/* SYSCON - Peripheral instance base addresses */
/** Peripheral SYSCON base address */
#define SYSCON_BASE                         (0x40048000u)

#define	SYSCON_PER_CLK_SOURCE_FRO			0
#define	SYSCON_PER_CLK_SOURCE_MAIN			1
#define	SYSCON_PER_CLK_SOURCE_FRG0			2
#define	SYSCON_PER_CLK_SOURCE_FRG1			3
#define	SYSCON_PER_CLK_SOURCE_FRO_DIV		4
#define	SYSCON_PER_CLK_SOURCE_NONE			7

/*! @brief FRO clock setting API address in ROM. */
#define CLOCK_FRO_SETTING_API_ROM_ADDRESS (0x0F0026F5U)

/*! @brief FAIM base address*/
#define CLOCK_FAIM_BASE (0x50010000U)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
/*! @name FROOSCCTRL - FRO oscillator control */
/*! @{ */
#define SYSCON_FROOSCCTRL_FRO_DIRECT_MASK        (0x20000U)
#define SYSCON_FROOSCCTRL_FRO_DIRECT_SHIFT       (17U)
/*! FRO_DIRECT - fro direct clock select
 *  0b0..fro clock is divider by 2 or 16,depend on FAIM slow boot value
 *  0b1..fro clock is direct from FRO oscillator
 */
#define SYSCON_FROOSCCTRL_FRO_DIRECT(x)          (((uint32_t)(((uint32_t)(x)) << SYSCON_FROOSCCTRL_FRO_DIRECT_SHIFT)) & SYSCON_FROOSCCTRL_FRO_DIRECT_MASK)
/*! @} */

/*! @name FRODIRECTCLKUEN - FRO direct clock source update enable register */
/*! @{ */
#define SYSCON_FRODIRECTCLKUEN_ENA_MASK          (0x1U)
#define SYSCON_FRODIRECTCLKUEN_ENA_SHIFT         (0U)
/*! ENA - Enable fro clock source update
 *  0b0..no change
 *  0b1..update clock source
 */
#define SYSCON_FRODIRECTCLKUEN_ENA(x)            (((uint32_t)(((uint32_t)(x)) << SYSCON_FRODIRECTCLKUEN_ENA_SHIFT)) & SYSCON_FRODIRECTCLKUEN_ENA_MASK)
/*! @} */

/*! @name MAINCLKSEL - Main clock source select register */
/*! @{ */
#define SYSCON_MAINCLKSEL_SEL_MASK               (0x3U)
#define SYSCON_MAINCLKSEL_SEL_SHIFT              (0U)
/*! SEL - System PLL clock source
 *  0b00..FRO
 *  0b01..External clock
 *  0b10..Watchdog oscillator
 *  0b11..FRO_DIV
 */
#define SYSCON_MAINCLKSEL_SEL(x)                 (((uint32_t)(((uint32_t)(x)) << SYSCON_MAINCLKSEL_SEL_SHIFT)) & SYSCON_MAINCLKSEL_SEL_MASK)
/*! @} */

/*! @name MAINCLKUEN - Main clock source update enable register */
/*! @{ */
#define SYSCON_MAINCLKUEN_ENA_MASK               (0x1U)
#define SYSCON_MAINCLKUEN_ENA_SHIFT              (0U)
/*! ENA - Enable main clock source update
 *  0b0..no change
 *  0b1..update clock source
 */
#define SYSCON_MAINCLKUEN_ENA(x)                 (((uint32_t)(((uint32_t)(x)) << SYSCON_MAINCLKUEN_ENA_SHIFT)) & SYSCON_MAINCLKUEN_ENA_MASK)
/*! @} */

/*! @name MAINCLKPLLSEL - Main clock source select register */
/*! @{ */
#define SYSCON_MAINCLKPLLSEL_SEL_MASK            (0x3U)
#define SYSCON_MAINCLKPLLSEL_SEL_SHIFT           (0U)
/*! SEL - System PLL clock source
 *  0b00..main_clk_pre_pll
 *  0b01..sys pll
 *  0b10..none
 *  0b11..none
 */
#define SYSCON_MAINCLKPLLSEL_SEL(x)              (((uint32_t)(((uint32_t)(x)) << SYSCON_MAINCLKPLLSEL_SEL_SHIFT)) & SYSCON_MAINCLKPLLSEL_SEL_MASK)
/*! @} */

/*! @name MAINCLKPLLUEN - Main clock source update enable register */
/*! @{ */
#define SYSCON_MAINCLKPLLUEN_ENA_MASK            (0x1U)
#define SYSCON_MAINCLKPLLUEN_ENA_SHIFT           (0U)
/*! ENA - Enable main clock source update
 *  0b0..no change
 *  0b1..update clock source
 */
#define SYSCON_MAINCLKPLLUEN_ENA(x)              (((uint32_t)(((uint32_t)(x)) << SYSCON_MAINCLKPLLUEN_ENA_SHIFT)) & SYSCON_MAINCLKPLLUEN_ENA_MASK)
/*! @} */

/*! @name SYSAHBCLKDIV - System clock divider register */
/*! @{ */
#define SYSCON_SYSAHBCLKDIV_DIV_MASK             (0xFFU)
#define SYSCON_SYSAHBCLKDIV_DIV_SHIFT            (0U)
#define SYSCON_SYSAHBCLKDIV_DIV(x)               (((uint32_t)(((uint32_t)(x)) << SYSCON_SYSAHBCLKDIV_DIV_SHIFT)) & SYSCON_SYSAHBCLKDIV_DIV_MASK)
/*! @} */

/* clock mux register definition */
#define CLK_MUX_GET_REG(x)                   ((volatile uint32_t *)(((uint32_t)(SYSCON)) + (((uint32_t)(x) >> 8U) & 0xFFU)))
#define CLK_MUX_GET_MUX(x)                   (((uint32_t)(x)) & 0xFFU)
#define CLK_MAIN_CLK_MUX_DEFINE(preMux, mux) ((preMux) << 8U | (mux))
#define CLK_MAIN_CLK_MUX_GET_PRE_MUX(x)      (((uint32_t)(x) >> 8U) & 0xFFU)
#define CLK_MAIN_CLK_MUX_GET_MUX(x)          ((uint32_t)(x)&0xFFU)


/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/*! @brief fro oscillator output frequency value definition */
typedef enum _clock_fro_osc_freq
{
    kCLOCK_FroOscOut18M = 18000U, /*!< FRO oscillator output 18M */
    kCLOCK_FroOscOut24M = 24000U, /*!< FRO oscillator output 24M */
    kCLOCK_FroOscOut30M = 30000U, /*!< FRO oscillator output 30M */
} clock_fro_osc_freq_t;

/*! @brief fro output frequency source definition */
typedef enum _clock_fro_src
{
    kCLOCK_FroSrcLpwrBootValue = 0,//U, /*!< fro source from the fro oscillator divided by low power boot value */
    kCLOCK_FroSrcFroOsc = 1 //U << SYSCON_FROOSCCTRL_FRO_DIRECT_SHIFT, /*!< fre source from the fro oscillator directly  */
} clock_fro_src_t;

/*! @brief Main clock source definition */
typedef enum _clock_main_clk_src
{
    kCLOCK_MainClkSrcFro    = CLK_MAIN_CLK_MUX_DEFINE(0U, 0U), /*!< main clock source from FRO */
    kCLOCK_MainClkSrcExtClk = CLK_MAIN_CLK_MUX_DEFINE(1U, 0U), /*!< main clock source from Ext clock */
    kCLOCK_MainClkSrcWdtOsc = CLK_MAIN_CLK_MUX_DEFINE(2U, 0U), /*!< main clock source from watchdog oscillator */
    kCLOCK_MainClkSrcFroDiv = CLK_MAIN_CLK_MUX_DEFINE(3U, 0U), /*!< main clock source from FRO Div */
    kCLOCK_MainClkSrcSysPll = CLK_MAIN_CLK_MUX_DEFINE(0U, 1U), /*!< main clock source from system pll */
} clock_main_clk_src_t;

typedef enum
{
	SYSCON_BYPASS_DISABLED = 0,
	SYSCON_BYPASS_ENABLED
}SYSCON_bypass_sel_en;

typedef enum
{
	SYSCON_FREQRANGE_MINUS_20MHZ = 0,
	SYSCON_FREQRANGE_PLUS_20MHZ
}SYSCON_freqrange_sel_en;

typedef enum
{
	SYSCON_WATCHDOG_CLKANA_0KHZ = 0,
	SYSCON_WATCHDOG_CLKANA_600KHZ,
	SYSCON_WATCHDOG_CLKANA_1050KHZ,
	SYSCON_WATCHDOG_CLKANA_1400KHZ,
	SYSCON_WATCHDOG_CLKANA_1750KHZ,
	SYSCON_WATCHDOG_CLKANA_2100KHZ,
	SYSCON_WATCHDOG_CLKANA_2400KHZ,
	SYSCON_WATCHDOG_CLKANA_3000KHZ,
	SYSCON_WATCHDOG_CLKANA_3250KHZ,
	SYSCON_WATCHDOG_CLKANA_3500KHZ,
	SYSCON_WATCHDOG_CLKANA_3750KHZ,
	SYSCON_WATCHDOG_CLKANA_4000KHZ,
	SYSCON_WATCHDOG_CLKANA_4200KHZ,
	SYSCON_WATCHDOG_CLKANA_4400KHZ,
	SYSCON_WATCHDOG_CLKANA_4600KHZ
}SYSCON_watchdog_clkana_sel_en;

typedef enum
{
	SYSCON_PLL_SOURCE_SEL_FRO = 0,
	SYSCON_PLL_SOURCE_SEL_EXT_CLK,
	SYSCON_PLL_SOURCE_SEL_WATCHDOG,
	SYSCON_PLL_SOURCE_SEL_FRO_DIV
}SYSCON_pll_source_sel_en;

typedef enum
{
	SYSCON_MAIN_CLOCK_SEL_FRO = 0,
	SYSCON_MAIN_CLOCK_SEL_EXT_CLK,
	SYSCON_MAIN_CLOCK_SEL_WATCHDOG,
	SYSCON_MAIN_CLOCK_SEL_FRO_DIV,
	SYSCON_MAIN_CLOCK_SEL_PLL
}SYSCON_main_clock_sel_en;

typedef enum
{
	SYSCON_CAPACITIVE_CLOCK_SEL_FRO = 0,
	SYSCON_CAPACITIVE_CLOCK_SEL_MAIN_CLOCK,
	SYSCON_CAPACITIVE_CLOCK_SEL_SYS_PLL,
	SYSCON_CAPACITIVE_CLOCK_SEL_FRO_DIV,
	SYSCON_CAPACITIVE_CLOCK_SEL_WATCHDOG_OSC
}SYSCON_capacitive_clock_sel_en;

typedef enum
{
	SYSCON_ADC_CLOCK_SEL_FRO = 0,
	SYSCON_ADC_CLOCK_SEL_SYS_PLL
}SYSCON_adc_clock_sel_en;

typedef enum
{
	SYSCON_SCT_CLOCK_SEL_FRO = 0,
	SYSCON_SCT_CLOCK_SEL_MAIN_CLOCK,
	SYSCON_SCT_CLOCK_SEL_SYS_PLL
}SYSCON_sct_clock_sel_en;

typedef enum
{
	SYSCON_EXT_CLOCK_SOURCE_SEL_SYS_OSC = 0,
	SYSCON_EXT_CLOCK_SOURCE_SEL_CLK_IN
}SYSCON_ext_clock_source_sel_en;

typedef enum
{
	SYSCON_ENABLE_CLOCK_SEL_ROM = 1,
	SYSCON_ENABLE_CLOCK_SEL_RAM,
	SYSCON_ENABLE_CLOCK_SEL_FLASH = 4,
	SYSCON_ENABLE_CLOCK_SEL_IIC0,
	SYSCON_ENABLE_CLOCK_SEL_GPIO0,
	SYSCON_ENABLE_CLOCK_SEL_SWM,
	SYSCON_ENABLE_CLOCK_SEL_SCT,
	SYSCON_ENABLE_CLOCK_SEL_WKT,
	SYSCON_ENABLE_CLOCK_SEL_MRT,
	SYSCON_ENABLE_CLOCK_SEL_SPI0,
	SYSCON_ENABLE_CLOCK_SEL_SPI1,
	SYSCON_ENABLE_CLOCK_SEL_CRC,
	SYSCON_ENABLE_CLOCK_SEL_UART0,
	SYSCON_ENABLE_CLOCK_SEL_UART1,
	SYSCON_ENABLE_CLOCK_SEL_UART2,
	SYSCON_ENABLE_CLOCK_SEL_WWDT,
	SYSCON_ENABLE_CLOCK_SEL_IOCON,
	SYSCON_ENABLE_CLOCK_SEL_ACMP,
	SYSCON_ENABLE_CLOCK_SEL_GPIO1,
	SYSCON_ENABLE_CLOCK_SEL_IIC1,
	SYSCON_ENABLE_CLOCK_SEL_IIC2,
	SYSCON_ENABLE_CLOCK_SEL_IIC3,
	SYSCON_ENABLE_CLOCK_SEL_ADC,
	SYSCON_ENABLE_CLOCK_SEL_CTIMER,
	SYSCON_ENABLE_CLOCK_SEL_MTB,
	SYSCON_ENABLE_CLOCK_SEL_DAC0,
	SYSCON_ENABLE_CLOCK_SEL_GPIO_INT,
	SYSCON_ENABLE_CLOCK_SEL_DMA,
	SYSCON_ENABLE_CLOCK_SEL_UART3,
	SYSCON_ENABLE_CLOCK_SEL_UART4,
	SYSCON_ENABLE_CLOCK_SEL_CAPT,
	SYSCON_ENABLE_CLOCK_SEL_DAC1
}SYSCON_enable_clock_sel_en;

typedef enum
{
	SYSCON_RESET_SEL_FLASH = 4,
	SYSCON_RESET_SEL_IIC0,
	SYSCON_RESET_SEL_GPIO0,
	SYSCON_RESET_SEL_SWM,
	SYSCON_RESET_SEL_SCT,
	SYSCON_RESET_SEL_WKT,
	SYSCON_RESET_SEL_MRT,
	SYSCON_RESET_SEL_SPI0,
	SYSCON_RESET_SEL_SPI1,
	SYSCON_RESET_SEL_CRC,
	SYSCON_RESET_SEL_UART0,
	SYSCON_RESET_SEL_UART1,
	SYSCON_RESET_SEL_UART2,
	SYSCON_RESET_SEL_IOCON = 18,
	SYSCON_RESET_SEL_ACMP,
	SYSCON_RESET_SEL_GPIO1,
	SYSCON_RESET_SEL_IIC1,
	SYSCON_RESET_SEL_IIC2,
	SYSCON_RESET_SEL_IIC3,
	SYSCON_RESET_SEL_ADC,
	SYSCON_RESET_SEL_CTIMER,
	SYSCON_RESET_SEL_DAC0 = 27,
	SYSCON_RESET_SEL_GPIOINT,
	SYSCON_RESET_SEL_DMA,
	SYSCON_RESET_SEL_UART3,
	SYSCON_RESET_SEL_UART4,
	SYSCON_RESET_SEL_CAPT,
	SYSCON_RESET_SEL_DAC1,
	SYSCON_RESET_SEL_FRG0 = 35,
	SYSCON_RESET_SEL_FRG1
}SYSCON_reset_sel_en;

typedef enum
{
	SYSCON_PERIPHERAL_SEL_UART0 = 0,
	SYSCON_PERIPHERAL_SEL_UART1,
	SYSCON_PERIPHERAL_SEL_UART2,
	SYSCON_PERIPHERAL_SEL_UART3,
	SYSCON_PERIPHERAL_SEL_UART4,
	SYSCON_PERIPHERAL_SEL_I2C0,
	SYSCON_PERIPHERAL_SEL_I2C1,
	SYSCON_PERIPHERAL_SEL_I2C2,
	SYSCON_PERIPHERAL_SEL_I2C3,
	SYSCON_PERIPHERAL_SEL_SPI0,
	SYSCON_PERIPHERAL_SEL_SPI1
}SYSCON_peripheral_sel_en;

typedef enum
{
	SYSCON_PERIPHERAL_CLOCK_SEL_FRO = 0,
	SYSCON_PERIPHERAL_CLOCK_SEL_MAIN,
	SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	SYSCON_PERIPHERAL_CLOCK_SEL_FRG1,
	SYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV,
	SYSCON_PERIPHERAL_CLOCK_SEL_NONE = 7
}SYSCON_peripheral_clock_sel_en;

typedef enum
{
	SYSCON_FRG_CLOCK_SEL_FRO = 0,
	SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK,
	SYSCON_FRG_CLOCK_SEL_SYS_PLL,
	SYSCON_FRG_CLOCK_SEL_NONE
}SYSCON_frg_clock_sel_en;

typedef enum
{
	SYSCON_CLKOUT_SOURCE_SEL_FRO = 0,
	SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK,
	SYSCON_CLKOUT_SOURCE_SEL_SYS_PLL,
	SYSCON_CLKOUT_SOURCE_SEL_EXT_CLOCK,
	SYSCON_CLKOUT_SOURCE_SEL_WATCHDOG_OSC
}SYSCON_clkout_source_sel_en;

typedef enum
{
	SYSCON_BOD_LEVEL_1 = 1,
	SYSCON_BOD_LEVEL_2,
	SYSCON_BOD_LEVEL_3
}SYSCON_bod_level_en;

typedef enum
{
	SYSCON_BOD_DISABLE = 0,
	SYSCON_BOD_ENABLE
}SYSCON_bod_enale_en;

typedef enum
{
	SYSCON_NMI_DISABLE = 0,
	SYSCON_NMI_ENABLE
}SYSCON_nmi_enable_en;

typedef enum
{
	SYSCON_WAKEUP_ENABLE_SEL_PINT0 = 0,
	SYSCON_WAKEUP_ENABLE_SEL_PINT1,
	SYSCON_WAKEUP_ENABLE_SEL_PINT2,
	SYSCON_WAKEUP_ENABLE_SEL_PINT3,
	SYSCON_WAKEUP_ENABLE_SEL_PINT4,
	SYSCON_WAKEUP_ENABLE_SEL_PINT5,
	SYSCON_WAKEUP_ENABLE_SEL_PINT6,
	SYSCON_WAKEUP_ENABLE_SEL_PINT7,
	SYSCON_WAKEUP_ENABLE_SEL_SPI0 = 32,
	SYSCON_WAKEUP_ENABLE_SEL_SPI1,
	SYSCON_WAKEUP_ENABLE_SEL_USART0 = 35,
	SYSCON_WAKEUP_ENABLE_SEL_USART1,
	SYSCON_WAKEUP_ENABLE_SEL_USART2,
	SYSCON_WAKEUP_ENABLE_SEL_IIC1 = 39,
	SYSCON_WAKEUP_ENABLE_SEL_IIC0,
	SYSCON_WAKEUP_ENABLE_SEL_CAPTOUCH = 43,
	SYSCON_WAKEUP_ENABLE_SEL_WWDT,
	SYSCON_WAKEUP_ENABLE_SEL_BOD,
	SYSCON_WAKEUP_ENABLE_SEL_WKT = 47,
	SYSCON_WAKEUP_ENABLE_SEL_IIC2 = 53,
	SYSCON_WAKEUP_ENABLE_SEL_IIC3,
	SYSCON_WAKEUP_ENABLE_SEL_USART3 = 62,
	SYSCON_WAKEUP_ENABLE_SEL_USART4
}SYSCON_enable_wakeup_sel_en;

typedef enum
{
	SYSCON_DEEP_SLEEP_POWERED = 0,
	SYSCON_DEEP_SLEEP_POWERED_DOWN
}SYSCON_deep_sleep_power_en;

typedef enum
{
	SYSCON_WAKEUP_POWER_SEL_FROOUT = 0,
	SYSCON_WAKEUP_POWER_SEL_FRO,
	SYSCON_WAKEUP_POWER_SEL_FLASH,
	SYSCON_WAKEUP_POWER_SEL_BOD,
	SYSCON_WAKEUP_POWER_SEL_ADC,
	SYSCON_WAKEUP_POWER_SEL_SYSOSC,
	SYSCON_WAKEUP_POWER_SEL_WDTOSC,
	SYSCON_WAKEUP_POWER_SEL_SYSPLL,
	SYSCON_WAKEUP_POWER_SEL_VREF2 = 10,
	SYSCON_WAKEUP_POWER_SEL_DAC0 = 13,
	SYSCON_WAKEUP_POWER_SEL_DAC1,
	SYSCON_WAKEUP_POWER_SEL_ACMP
}SYSCON_wakeup_power_sel_en;

typedef enum
{
	SYSCON_POWER_SEL_FROOUT = 0,
	SYSCON_POWER_SEL_FRO,
	SYSCON_POWER_SEL_FLASH,
	SYSCON_POWER_SEL_BOD,
	SYSCON_POWER_SEL_ADC,
	SYSCON_POWER_SEL_SYSOSC,
	SYSCON_POWER_SEL_WDTOSC,
	SYSCON_POWER_SEL_SYSPLL,
	SYSCON_POWER_SEL_DAC0 = 13,
	SYSCON_POWER_SEL_DAC1,
	SYSCON_POWER_SEL_ACMP
}SYSCON_power_sel_en;


typedef struct
{
	uint32_t RESERVED;
}SYSCON_RESERVED_reg_t;

typedef struct
{
	uint32_t MAP : 2;
	uint32_t : 30;
}SYSCON_SYSMEMREMAP_reg_t;

typedef struct
{
	uint32_t MSEL : 4;
	uint32_t PSEL : 2;
	uint32_t : 26;
}SYSCON_SYSPLLCTRL_reg_t;

typedef struct
{
	uint32_t LOCK : 1;
	uint32_t : 31;
}SYSCON_SYSPLLSTAT_reg_t;

typedef struct
{
	uint32_t BYPASS : 1;
	uint32_t FREQRANGE : 1;
	uint32_t : 30;
}SYSCON_SYSOSCCTRL_reg_t;

typedef struct
{
	uint32_t DIVSEL : 5;
	uint32_t FREQSEL : 4;
	uint32_t : 23;
}SYSCON_WDTOSCCTRL_reg_t;

typedef struct
{
	uint32_t CORESEL : 2;
	uint32_t : 15;
	uint32_t FRO_DIRECT : 1;
	uint32_t : 14;
}SYSCON_FROOSCCTRL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_FRODIRECTCLKUEN_reg_t;

typedef struct
{
	uint32_t POR : 1;
	uint32_t EXTRST : 1;
	uint32_t WDT : 1;
	uint32_t BOD : 1;
	uint32_t SYSRST : 1;
	uint32_t : 27;
}SYSCON_SYSRSTSTAT_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_SYSPLLCLKSEL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_SYSPLLCLKUEN_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_MAINCLKPLLSEL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_MAINCLKPLLUEN_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_MAINCLKSEL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_MAINCLKUEN_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_SYSAHBCLKDIV_reg_t;

typedef struct
{
	uint32_t SEL : 3;
	uint32_t : 29;
}SYSCON_CAPTCLKSEL_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_ADCCLKSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_ADCCLKDIV_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_SCTCLKSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_SCTCLKDIV_reg_t;

typedef struct
{
	uint32_t SEL : 1;
	uint32_t : 31;
}SYSCON_EXTCLKSEL_reg_t;

typedef struct
{
	uint32_t SYS : 1;
	uint32_t ROM : 1;
	uint32_t RAM0_1 : 1;
	uint32_t : 1;
	uint32_t FLASH : 1;
	uint32_t I2C0 : 1;
	uint32_t GPIO0 : 1;
	uint32_t SWM : 1;
	uint32_t SCT : 1;
	uint32_t WKT : 1;
	uint32_t MRT : 1;
	uint32_t SPI0 : 1;
	uint32_t SPI1 : 1;
	uint32_t CRC : 1;
	uint32_t UART0 : 1;
	uint32_t UART1 : 1;
	uint32_t UART2 : 1;
	uint32_t WWDT : 1;
	uint32_t IOCON : 1;
	uint32_t ACMP : 1;
	uint32_t GPIO1 : 1;
	uint32_t I2C1 : 1;
	uint32_t I2C2 : 1;
	uint32_t I2C3 : 1;
	uint32_t ADC : 1;
	uint32_t CTIMER0 : 1;
	uint32_t MTB : 1;
	uint32_t DAC0 : 1;
	uint32_t GPIO_INT : 1;
	uint32_t DMA : 1;
	uint32_t UART3 : 1;
	uint32_t UART4 : 1;
}SYSCON_SYSAHBCLKCTRL0_reg_t;

typedef struct
{
	uint32_t CAPT : 1;
	uint32_t DAC1 : 1;
	uint32_t : 30;
}SYSCON_SYSAHBCLKCTRL1_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t FLASH_RST_N : 1;
	uint32_t I2C0_RST_N : 1;
	uint32_t GPIO0_RST_N : 1;
	uint32_t SWM_RST_N : 1;
	uint32_t SCT_RST_N : 1;
	uint32_t WKT_RST_N : 1;
	uint32_t MRT_RST_N : 1;
	uint32_t SPI0_RST_N : 1;
	uint32_t SPI1_RST_N : 1;
	uint32_t CRC_RST_N : 1;
	uint32_t UART0_RST_N : 1;
	uint32_t UART1_RST_N : 1;
	uint32_t UART2_RST_N : 1;
	uint32_t : 1;
	uint32_t IOCON_RST_N : 1;
	uint32_t ACMP_RST_N : 1;
	uint32_t GPIO1_RST_N : 1;
	uint32_t I2C1_RST_N : 1;
	uint32_t I2C2_RST_N : 1;
	uint32_t I2C3_RST_N : 1;
	uint32_t ADC_RST_N : 1;
	uint32_t CTIMER0_RST_N : 1;
	uint32_t : 1;
	uint32_t DAC0_RST_N : 1;
	uint32_t GPIOINT_RST_N : 1;
	uint32_t DMA_RST_N : 1;
	uint32_t UART3_RST_N : 1;
	uint32_t UART4_RST_N : 1;
}SYSCON_PRESETCTRL0_reg_t;

typedef struct
{
	uint32_t CAPT_RST_N : 1;
	uint32_t DAC1_RST_N : 1;
	uint32_t : 1;
	uint32_t FRG0_RST_N : 1;
	uint32_t FRG1_RST_N : 1;
	uint32_t : 27;
}SYSCON_PRESETCTRL1_reg_t;

typedef struct
{
	uint32_t SEL : 3;
	uint32_t : 29;
}SYSCON_PERCLKSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_FRGDIV_reg_t;

typedef struct
{
	uint32_t MULT : 8;
	uint32_t : 24;
}SYSCON_FRGMULT_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_FRGCLKSEL_reg_t;

typedef struct
{
	uint32_t SEL : 3;
	uint32_t : 29;
}SYSCON_CLKOUTSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_CLKOUTDIV_reg_t;

typedef struct
{
	uint32_t START : 1;
	uint32_t STOP : 1;
	uint32_t : 30;
}SYSCON_EXTTRACECMD_reg_t;

typedef struct
{
	uint32_t PIOSTAT;
}SYSCON_PIOPORCAP_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_IOCONCLKDIV_reg_t;

typedef struct
{
	uint32_t BODRSTLEV : 2;
	uint32_t BODINTVAL : 2;
	uint32_t BODRSTENA : 1;
	uint32_t : 27;
}SYSCON_BODCTRL_reg_t;

typedef struct
{
	uint32_t CAL : 26;
	uint32_t : 6;
}SYSCON_SYSTCKCAL_reg_t;

typedef struct
{
	uint32_t LATENCY : 8;
	uint32_t : 24;
}SYSCON_IRQLATENCY_reg_t;

typedef struct
{
	uint32_t IRQN : 5;
	uint32_t : 26;
	uint32_t NMIEN : 1;
}SYSCON_NMISRC_reg_t;

typedef struct
{
	uint32_t INTPIN : 6;
	uint32_t : 26;
}SYSCON_PINTSEL_reg_t;

typedef struct
{
	uint32_t PINT0 : 1;
	uint32_t PINT1 : 1;
	uint32_t PINT2 : 1;
	uint32_t PINT3 : 1;
	uint32_t PINT4 : 1;
	uint32_t PINT5 : 1;
	uint32_t PINT6 : 1;
	uint32_t PINT7 : 1;
	uint32_t : 24;
}SYSCON_STARTERP0_reg_t;

typedef struct
{
	uint32_t SPI0 : 1;
	uint32_t SPI1 : 1;
	uint32_t : 1;
	uint32_t USART0 : 1;
	uint32_t USART1 : 1;
	uint32_t USART2 : 1;
	uint32_t : 1;
	uint32_t I2C1 : 1;
	uint32_t I2C0 : 1;
	uint32_t : 2;
	uint32_t CAP_TOUCH : 1;
	uint32_t WWDT : 1;
	uint32_t BOD : 1;
	uint32_t : 1;
	uint32_t WKT : 1;
	uint32_t : 5;
	uint32_t I2C2 : 1;
	uint32_t I2C3 : 1;
	uint32_t : 7;
	uint32_t USART3 : 1;
	uint32_t USART4 : 1;
}SYSCON_STARTERP1_reg_t;

typedef struct
{
	uint32_t : 3;
	uint32_t BOD_PD : 1;
	uint32_t : 2;
	uint32_t WDTOSC_PD : 1;
	uint32_t : 25;
}SYSCON_PDSLEEPCFG_reg_t;

typedef struct
{
	uint32_t FRO_OUT_PD : 1;
	uint32_t FRO_PD : 1;
	uint32_t FLASH_PD : 1;
	uint32_t BOD_PD : 1;
	uint32_t ADC_PD : 1;
	uint32_t SYSOSC_PD : 1;
	uint32_t WDTOSC_PD : 1;
	uint32_t SYSPLL_PD : 1;
	uint32_t : 2;
	uint32_t VREF2_PD : 1;
	uint32_t : 2;
	uint32_t DAC0 : 1;
	uint32_t DAC1 : 1;
	uint32_t ACMP : 1;
	uint32_t : 16;
}SYSCON_PDAWAKECFG_reg_t;

typedef struct
{
	uint32_t FROOUT_PD : 1;
	uint32_t FRO_PD : 1;
	uint32_t FLASH_PD : 1;
	uint32_t BOD_PD : 1;
	uint32_t ADC_PD : 1;
	uint32_t SYSOSC_PD : 1;
	uint32_t WDTOSC_PD : 1;
	uint32_t SYSPLL_PD : 1;
	uint32_t : 5;
	uint32_t DAC0 : 1;
	uint32_t DAC1 : 1;
	uint32_t ACMP : 1;
	uint32_t : 16;
}SYSCON_PDRUNCFG_reg_t;

typedef struct
{
	uint32_t DEVICE_ID;
}SYSCON_DEVICE_ID_reg_t;

typedef struct
{
	__RW SYSCON_SYSMEMREMAP_reg_t SYSMEMREMAP;
	__R SYSCON_RESERVED_reg_t RESERVED_1;
	__RW SYSCON_SYSPLLCTRL_reg_t SYSPLLCTRL;
	__R SYSCON_SYSPLLSTAT_reg_t SYSPLLSTAT;
	__R SYSCON_RESERVED_reg_t RESERVED_2;
	__R SYSCON_RESERVED_reg_t RESERVED_3;
	__R SYSCON_RESERVED_reg_t RESERVED_4;
	__R SYSCON_RESERVED_reg_t RESERVED_5;
	__RW SYSCON_SYSOSCCTRL_reg_t SYSOSCCTRL;
	__RW SYSCON_WDTOSCCTRL_reg_t WDTOSCCTRL;
	__RW SYSCON_FROOSCCTRL_reg_t FROOSCCTRL;
	__R SYSCON_RESERVED_reg_t RESERVED_6;
	__RW SYSCON_FRODIRECTCLKUEN_reg_t FRODIRECTCLKUEN;
	__R SYSCON_RESERVED_reg_t RESERVED_7;
	__RW SYSCON_SYSRSTSTAT_reg_t SYSRSTSTAT;
	__R SYSCON_RESERVED_reg_t RESERVED_8;
	__RW SYSCON_SYSPLLCLKSEL_reg_t SYSPLLCLKSEL;
	__RW SYSCON_SYSPLLCLKUEN_reg_t SYSPLLCLKUEN;
	__RW SYSCON_MAINCLKPLLSEL_reg_t MAINCLKPLLSEL;
	__RW SYSCON_MAINCLKPLLUEN_reg_t MAINCLKPLLUEN;
	__RW SYSCON_MAINCLKSEL_reg_t MAINCLKSEL;
	__RW SYSCON_MAINCLKUEN_reg_t MAINCLKUEN;
	__RW SYSCON_SYSAHBCLKDIV_reg_t SYSAHBCLKDIV;
	__R SYSCON_RESERVED_reg_t RESERVED_9;
	__RW SYSCON_CAPTCLKSEL_reg_t CAPTCLKSEL;
	__RW SYSCON_ADCCLKSEL_reg_t ADCCLKSEL;
	__RW SYSCON_ADCCLKDIV_reg_t ADCCLKDIV;
	__RW SYSCON_SCTCLKSEL_reg_t SCTCLKSEL;
	__RW SYSCON_SCTCLKDIV_reg_t SCTCLKDIV;
	__RW SYSCON_EXTCLKSEL_reg_t EXTCLKSEL;
	__R SYSCON_RESERVED_reg_t RESERVED_10;
	__R SYSCON_RESERVED_reg_t RESERVED_11;
	__RW SYSCON_SYSAHBCLKCTRL0_reg_t SYSAHBCLKCTRL0;
	__RW SYSCON_SYSAHBCLKCTRL1_reg_t SYSAHBCLKCTRL1;
	__RW SYSCON_PRESETCTRL0_reg_t PRESETCTRL0;
	__RW SYSCON_PRESETCTRL1_reg_t PRESETCTRL1;
	__RW SYSCON_PERCLKSEL_reg_t PERCLKSEL[11];
	__R SYSCON_RESERVED_reg_t RESERVED_12;
	__R SYSCON_RESERVED_reg_t RESERVED_13;
	__R SYSCON_RESERVED_reg_t RESERVED_14;
	__R SYSCON_RESERVED_reg_t RESERVED_15;
	__R SYSCON_RESERVED_reg_t RESERVED_16;
	__RW SYSCON_FRGDIV_reg_t FRG0DIV;
	__RW SYSCON_FRGMULT_reg_t FRG0MUL;
	__RW SYSCON_FRGCLKSEL_reg_t FRG0CLKSEL;
	__R SYSCON_RESERVED_reg_t RESERVED_17;
	__RW SYSCON_FRGDIV_reg_t FRG1DIV;
	__RW SYSCON_FRGMULT_reg_t FRG1MUL;
	__RW SYSCON_FRGCLKSEL_reg_t FRG1CLKSEL;
	__R SYSCON_RESERVED_reg_t RESERVED_18;
	__RW SYSCON_CLKOUTSEL_reg_t CLKOUTSEL;
	__RW SYSCON_CLKOUTDIV_reg_t CLKOUTDIV;
	__R SYSCON_RESERVED_reg_t RESERVED_19;
	__RW SYSCON_EXTTRACECMD_reg_t EXTTRACECMD;
	__R SYSCON_PIOPORCAP_reg_t PIOPORCAP[2];
	__R SYSCON_RESERVED_reg_t RESERVED_20[0x2C / 4];
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV6;
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV5;
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV4;
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV3;
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV2;
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV1;
	__RW SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV0;
	__RW SYSCON_BODCTRL_reg_t BODCTRL;
	__RW SYSCON_SYSTCKCAL_reg_t SYSTCKCAL;
	__R SYSCON_RESERVED_reg_t RESERVED_21[0x18 / 4];
	__RW SYSCON_IRQLATENCY_reg_t IRQLATENCY;
	__RW SYSCON_NMISRC_reg_t NMISRC;
	__RW SYSCON_PINTSEL_reg_t PINTSEL[8];
	__R SYSCON_RESERVED_reg_t RESERVED_22[0x6C / 4];
	__RW SYSCON_STARTERP0_reg_t STARTERP0;
	__R SYSCON_RESERVED_reg_t RESERVED_23[0x0C / 4];
	__RW SYSCON_STARTERP1_reg_t STARTERP1;
	__R SYSCON_RESERVED_reg_t RESERVED_24[0x18 / 4];
	__RW SYSCON_PDSLEEPCFG_reg_t PDSLEEPCFG;
	__RW SYSCON_PDAWAKECFG_reg_t PDAWAKECFG;
	__RW SYSCON_PDRUNCFG_reg_t PDRUNCFG;
	__R SYSCON_RESERVED_reg_t RESERVED_25[0x1BC / 4];
	__R SYSCON_DEVICE_ID_reg_t DEVICE_ID;
}SYSCON_per_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

void SYSCON_set_pll_control(uint8_t m, uint8_t p);
uint8_t SYSCON_get_pll_lock_status(void);
void SYSCON_set_oscillator_control(SYSCON_bypass_sel_en bypass, SYSCON_freqrange_sel_en freqrange);
void SYSCON_set_watchdog_oscillator_control(uint8_t divsel, SYSCON_watchdog_clkana_sel_en clkana_sel);
void SYSCON_set_fro_direct(void);
void SYSCON_clear_fro_direct(void);
void SYSCON_set_pll_clk_source(SYSCON_pll_source_sel_en pll_source);
void SYSCON_set_system_clock_source(SYSCON_main_clock_sel_en clock_selection);
void SYSCON_set_system_clock_divider(uint8_t divider);
void SYSCON_set_adc_clock(SYSCON_adc_clock_sel_en source_sel, uint8_t div);
void SYSCON_set_sct_clock(SYSCON_sct_clock_sel_en source_sel, uint8_t div);
void SYSCON_ext_clock_source_set(SYSCON_ext_clock_source_sel_en source_selection);
SYSCON_ext_clock_source_sel_en SYSCON_ext_clock_source_get(void);
void SYSCON_enable_clock(SYSCON_enable_clock_sel_en peripheral);
void SYSCON_disable_clock(SYSCON_enable_clock_sel_en peripheral);
void SYSCON_assert_reset(SYSCON_reset_sel_en peripheral);
void SYSCON_clear_reset(SYSCON_reset_sel_en peripheral);
void SYSCON_set_peripheral_clock_source(SYSCON_peripheral_sel_en peripheral, SYSCON_peripheral_clock_sel_en clock);
void SYSCON_set_frg_config(uint8_t frg_selection, SYSCON_frg_clock_sel_en clock_source, uint8_t mul, uint8_t div);
void SYSCON_set_clkout_config(SYSCON_clkout_source_sel_en clock_source, uint8_t divider);
uint32_t SYSCON_get_por_pio_status_register(uint8_t inst);
void SYSCON_set_iocon_glitch_divider(uint8_t inst, uint8_t div);
void SYSCON_set_bod_control(SYSCON_bod_level_en reset_level, SYSCON_bod_level_en bod_level, SYSCON_bod_enale_en reset_enable);
uint32_t SYSCON_get_systick_calib(void);
uint8_t SYSCON_get_irq_latency(void);
void SYSCON_set_nmi_source(uint8_t irq, SYSCON_nmi_enable_en enable);
void SYSCON_set_pinint_pin(uint8_t channel, GPIO_portpin_en portpin);
void SYSCON_enable_wakeup_source(SYSCON_enable_wakeup_sel_en peripheral);
void SYSCON_disable_wakeup_source(SYSCON_enable_wakeup_sel_en peripheral);
void SYSCON_deep_sleep_power_bod(SYSCON_deep_sleep_power_en power);
void SYSCON_deep_sleep_power_wdtosc(SYSCON_deep_sleep_power_en power);
void SYSCON_set_powered_on_wakeup(SYSCON_wakeup_power_sel_en peripheral);
void SYSCON_clear_powered_on_wakeup(SYSCON_wakeup_power_sel_en peripheral);
void SYSCON_power_up_peripheral(SYSCON_power_sel_en peripheral);
void SYSCON_power_down_peripheral(SYSCON_power_sel_en peripheral);
uint32_t SYSCON_get_device_id(void);

void SYSCON_set_fro_osc_freq(clock_fro_osc_freq_t freq);
void SYSCON_set_fro_out_clk_src(clock_fro_src_t src);
void SYSCON_set_main_clk_src(clock_main_clk_src_t src);
void SYSCON_set_core_sys_clk_div(uint32_t value);

#endif /* DR_SYSCON_H_ */
