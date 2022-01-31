/*******************************************************************************************************************************//**
 *
 * @file		PR_SYSCON.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_SYSCON_H_
#define PR_SYSCON_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/

#include "DR_SYSCON.h"
#include "PR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/** Selección de fuente de clock para el clock principal */
typedef enum
{
	kSYSCON_SYSTEM_CLOCK_SEL_FRO = 0, /**< Free Running Oscillator */
	kSYSCON_SYSTEM_CLOCK_SEL_EXT, /**< Clcok externo */
	kSYSCON_SYSTEM_CLOCK_SEL_WATCHDOG, /**< Watchdog Oscillator */
	kSYSCON_SYSTEM_CLOCK_SEL_FRO_DIV, /**< Free Running Oscillator dividido 2 */
	kSYSCON_SYSTEM_CLOCK_SEL_PLL /**< Phase Locked Loop */
}syscon_system_clock_sel_en;

/** Selección de fuente de clock para la salida CLKOUT */
typedef enum
{
	kSYSCON_CLKOUT_SOURCE_SEL_FRO = 0, /**< Free Running Oscillator */
	kSYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK, /**< Clock principal */
	kSYSCON_CLKOUT_SOURCE_SEL_SYS_PLL, /**< Phase Locked Loop */
	kSYSCON_CLKOUT_SOURCE_SEL_EXT_CLOCK, /**< Clock externo */
	kSYSCON_CLKOUT_SOURCE_SEL_WATCHDOG_OSC /**< Watchdog Oscillator */
}syscon_clkout_source_sel_en;

/** Generadores fraccionales de clock */
typedef enum
{
	kSYSCON_FRG0 = 0, 	/**< Fractional Generator 0 */
	kSYSCON_FRG1 = 1 	/**< Fractional Generator 1 */
}syscon_frg_en;

/** Selección de fuente de clock para los generadores fraccionales de clock */
typedef enum
{
	kSYSCON_FRG_CLOCK_SEL_FRO = 0, /**< Free Running Oscillator */
	kSYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, /**< Clock principal */
	kSYSCON_FRG_CLOCK_SEL_SYS_PLL, /**< Phase Locked Loop */
	kSYSCON_FRG_CLOCK_SEL_NONE /**< Ninguno */
}syscon_frg_clock_sel_en;

/** Selección de frecuencia base para el watchdog oscillator */
typedef enum
{
	kSYSCON_WATCHDOG_CLKANA_0KHZ = 0, /**< 0MHz */
	kSYSCON_WATCHDOG_CLKANA_600KHZ, /**< 0.6MHz */
	kSYSCON_WATCHDOG_CLKANA_1050KHZ, /**< 1.05MHz */
	kSYSCON_WATCHDOG_CLKANA_1400KHZ, /**< 1.4MHz */
	kSYSCON_WATCHDOG_CLKANA_1750KHZ, /**< 1.75MHz */
	kSYSCON_WATCHDOG_CLKANA_2100KHZ, /**< 2.1MHz */
	kSYSCON_WATCHDOG_CLKANA_2400KHZ, /**< 2.4MHz */
	kSYSCON_WATCHDOG_CLKANA_3000KHZ, /**< 3MHz */
	kSYSCON_WATCHDOG_CLKANA_3250KHZ, /**< 3.25MHz */
	kSYSCON_WATCHDOG_CLKANA_3500KHZ, /**< 3.5MHz */
	kSYSCON_WATCHDOG_CLKANA_3750KHZ, /**< 3.75MHz */
	kSYSCON_WATCHDOG_CLKANA_4000KHZ, /**< 4MHz */
	kSYSCON_WATCHDOG_CLKANA_4200KHZ, /**< 4.2MHz */
	kSYSCON_WATCHDOG_CLKANA_4400KHZ, /**< 4.4MHz */
	kSYSCON_WATCHDOG_CLKANA_4600KHZ /**< 4.6MHz */
}syscon_watchdog_clkana_sel_en;

/** Selección de periférico para controlar fuente de clock */
typedef enum
{
	kSYSCON_PERIPHERAL_SEL_UART0 = 0, /**< UART0 */
	kSYSCON_PERIPHERAL_SEL_UART1, /**< UART1 */
	kSYSCON_PERIPHERAL_SEL_UART2, /**< UART2 */
	kSYSCON_PERIPHERAL_SEL_UART3, /**< UART3 */
	kSYSCON_PERIPHERAL_SEL_UART4, /**< UART4 */
	kSYSCON_PERIPHERAL_SEL_IIC0, /**< IIC0 */
	kSYSCON_PERIPHERAL_SEL_IIC1, /**< IIC1 */
	kSYSCON_PERIPHERAL_SEL_IIC2, /**< IIC2 */
	kSYSCON_PERIPHERAL_SEL_IIC3, /**< IIC3 */
	kSYSCON_PERIPHERAL_SEL_SPI0, /**< SPI0 */
	kSYSCON_PERIPHERAL_SEL_SPI1 /**< SPI1 */
}syscon_peripheral_sel_en;

/** Selección de fuente de clock para los periféricos */
typedef enum
{
	kSYSCON_PERIPHERAL_CLOCK_SEL_FRO = 0, /**< Free Running Oscillator */
	kSYSCON_PERIPHERAL_CLOCK_SEL_MAIN, /**< Clock principal */
	kSYSCON_PERIPHERAL_CLOCK_SEL_FRG0, /**< Generador fraccional 0 */
	kSYSCON_PERIPHERAL_CLOCK_SEL_FRG1, /**< Generador fraccional 1 */
	kSYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV, /**< Free Running Oscillator dividido por 2 */
	kSYSCON_PERIPHERAL_CLOCK_SEL_NONE = 7 /**< Ninguna */
}syscon_peripheral_clock_sel_en;

/** Selección de banco de división para filtros de glitches */
typedef enum
{
	kSYSCON_IOCON_GLITCH_SEL_0 = 0, /**< Banco 0 */
	kSYSCON_IOCON_GLITCH_SEL_1, /**< Banco 1 */
	kSYSCON_IOCON_GLITCH_SEL_2, /**< Banco 2 */
	kSYSCON_IOCON_GLITCH_SEL_3, /**< Banco 3 */
	kSYSCON_IOCON_GLITCH_SEL_4, /**< Banco 4 */
	kSYSCON_IOCON_GLITCH_SEL_5, /**< Banco 5 */
	kSYSCON_IOCON_GLITCH_SEL_6, /**< Banco 6 */
	kSYSCON_IOCON_GLITCH_SEL_7 /**< Banco 7 */
}syscon_iocon_glitch_sel_en;

/** Fuente de clock para el PLL */
typedef enum
{
	kSYSCON_PLL_SOURCE_SEL_FRO = 0, /**< Free Running Oscillator */
	kSYSCON_PLL_SOURCE_SEL_EXT_CLK, /**< Clock externo */
	kSYSCON_PLL_SOURCE_SEL_WATCHDOG, /**< Watchdog Oscillator */
	kSYSCON_PLL_SOURCE_SEL_FRO_DIV /**< Free Running Oscillator dividido por 2 */
}syscon_pll_source_sel_en;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile SYSCON_per_t * const SYSCON; 	//!< Periferico SYSCON

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t syscon_system_clock_get(void);

/**
 * @brief Configuración de fuente de clock para el clock principal
 * @param[in] clock_source Selección deseada
 */
void syscon_system_clock_set_source(syscon_system_clock_sel_en clock_source);

/**
 * @brief Fijar el divisor del clock principal
 * @param[in] div Divisor deseado. Cero inhabilita el clock principal
 */
void syscon_system_clock_set_divider(uint8_t div);

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t syscon_fro_clock_get(void);

/**
 * @brief Configurar el ext clock a partir de un cristal externo
 * @param[in] crystal_freq Frecuencia del cristal externo utilizado
 */
void syscon_external_crystal_config(uint32_t crystal_freq);

/**
 * @brief Configurar el ext clock a partir de una fuente de clock externa
 * @param[in] external_clock_freq Frecuencia de la fuente de clock externa en Hz
 */
void syscon_external_clock_config(uint32_t external_clock_freq);

/**
 * @brief Configurar el clock FRO
 *
 * @note Esta función habilita el FRO
 *
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 */
void syscon_fro_clock_config(clock_fro_src_t direct, clock_fro_osc_freq_t fro_osc_freq);
void syscon_fro_clock_config_v1(uint8_t direct);
//void syscon_fro_clock_config(uint8_t direct);

/**
 * @brief Inhabilitar el FRO
 */
void syscon_fro_clock_disable(void);

/**
 * @brief Configurar el pin de clock out (salida de clock hacia afuera)
 * @param[in] portpin Número de puerto/pin por donde sacar el clock out
 * @param[in] clock_source Fuente deseada para la salida clock out
 * @param[in] divider Divisor deseado para la salida clock out
 */
void syscon_clkout_config(gpio_portpin_en portpin, syscon_clkout_source_sel_en clock_source, uint8_t divider);

/**
 * @brief Configurar el divisor fraccional
 *
 * El divisor siempre se debe fijar en 256 para estos MCU.
 *
 * @param[in] inst Instancia de FRG a configurar
 * @param[in] clock_source Fuente de clock de entrada para el FRG
 * @param[in] mul Multiplicador deseado
 */
void syscon_frg_config(uint8_t inst, syscon_frg_clock_sel_en clock_source, uint32_t mul);

/**
 * @brief Configuración del watchdog oscillator
 * @param[in] clkana_sel Selección de frecuencia base del oscilador
 * @param[in] div Divisor. El valor efectivo de división es: 2 (1 + div)
 *
 * @note Recordar que la precisión de este oscilador es de más/menos 40%
 */
void syscon_watchdog_oscillator_config(syscon_watchdog_clkana_sel_en clkana_sel, uint8_t div);

/**
 * @brief Obtener la frecuencia de clock en Hz configurada para cierto periférico
 * @param[in] peripheral Periférico deseado
 * @return Frecuencia en Hz del clock del periférico
 */
uint32_t syscon_peripheral_clock_get(syscon_peripheral_sel_en peripheral);

/**
 * @brief Configurar divisor para el clock de glitches del IOCON
 * @param[in] sel Selección de divisor
 * @param[in] div Valor de división deseado
 */
void syscon_iocon_glitch_divider_set(syscon_iocon_glitch_sel_en sel, uint32_t div);

/**
 * @brief Configurar el PLL
 * @param[in] clock_source Fuente de clock de referencia para el PLL
 * @param[in] freq Frecuencia deseada de salida del PLL
 */
void syscon_pll_clock_config(syscon_pll_source_sel_en clock_source, uint32_t freq);

/**
 * @brief Obtener frecuencia actual configurada del PLL
 * @return Frecuencia actual del PLL en Hz
 */
uint32_t syscon_pll_clock_get(void);

uint32_t syscon_get_usart_clk_freq(uint8_t inst);
uint32_t syscon_get_fro_freq(void);
uint32_t syscon_get_main_clk_freq(void);
uint32_t syscon_get_system_PLL_freq(void);
uint32_t syscon_get_system_PLL_input_clock_rate(void);
uint32_t syscon_get_FRG_input_clk_freq(syscon_frg_en frg);
uint32_t syscon_get_FRG0_clk_freq(void);
uint32_t syscon_get_FRG1_clk_freq(void);
uint32_t syscon_get_wdt_osc_freq(void);
uint32_t syscon_get_ext_clk_rreq(void);


#endif /* PR_SYSCON_H_ */
