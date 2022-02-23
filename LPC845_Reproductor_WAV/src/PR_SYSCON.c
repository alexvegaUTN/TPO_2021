/*******************************************************************************************************************************//**
 *
 * @file		PR_SYSCON.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_SYSCON.h"
#include <DR_SYSCON.h>
#include <DR_IOCON.h>
#include <DR_SWM.h>

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
/** Número de puerto del XTALIN */
#define		XTALIN_PORT			0

/** Número de pin del XTALIN */
#define		XTALIN_PIN			8

/** Número de puerto del XTALOUT */
#define		XTALOUT_PORT		0

/** Número de pin del XTALOUT */
#define		XTALOUT_PIN			9

/** Frecuencia del FRO base */
#define		FRO_DIRECT_FREQ		24e6

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
static uint8_t current_main_div = 1; 						//!< Divisor actual del clock principal
static uint32_t current_fro_freq = FRO_DIRECT_FREQ / 2;		//!< Frecuencia actual del FRO
static uint32_t current_fro_div_freq = FRO_DIRECT_FREQ / 4; //!< Frecuencia actual del FRO DIV
static uint32_t current_crystal_freq = 0; 					//!< Frecuencia del cristal configurada
static uint32_t current_frg_freq[2] = { 0, 0 }; 			//!< Frecuencia de los FRG
static uint32_t current_pll_freq = 0; 						//!< Frecuencia del PLL
static uint32_t current_ext_freq = 0; 						//!< Frecuencia de la fuente de clock externa
static uint32_t current_watchdog_freq = 0; 					//!< Frecuencia del watchod oscillator
static uint32_t *current_main_freq = &current_fro_freq; 	//!< Frecuencia actual del main clock

//!< Frecuencias bases posibles del watchdog oscillator
static const uint32_t base_watchdog_freq[] = { 0, 0.6e6, 1.05e6, 1.4e6, 1.75e6, 2.1e6, 2.4e6, 2.7e6,
											   3e6, 3.25e6, 3.5e6, 3.75e6, 4e6, 4.2e6, 4.4e6, 4.6e6 };

/* External clock rate.
 * Either external clk in rate or system oscillator frequency.
 */
volatile uint32_t g_Ext_Clk_Freq = 0U;
/** watch dog oscillator rate in Hz.*/
volatile uint32_t g_Wdt_Osc_Freq = 0U;

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
 	\date Jan 4, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

void syscon_boot_clock_run(void)
{
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_FROOUT);	/*!< Ensure FRO is on  */
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_FRO);
	SYSCON_set_fro_osc_freq(kCLOCK_FroOscOut30M);			/*!< Set up FRO freq */
	SYSCON_set_fro_out_clk_src(kCLOCK_FroSrcFroOsc);       	/*!< Set FRO clock source */

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_SYSOSC);    /*!< Ensure Main osc is on */
	SYSCON_ext_clock_source_set(SYSCON_EXT_CLOCK_SOURCE_SEL_SYS_OSC);	/*!<select external clock source to sys_osc */

	SYSCON_set_system_clock_source(SYSCON_MAIN_CLOCK_SEL_FRO);
	//SYSCON_set_main_clk_src(kCLOCK_MainClkSrcFro);			/*!< select fro for main clock */

	SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART0, SYSCON_PERIPHERAL_CLOCK_SEL_FRO);		/*!< select FRO for UART0 */
	SYSCON_set_core_sys_clk_div(1U);
}

/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t syscon_system_clock_get(void)
{
	return (*current_main_freq) / current_main_div;
}

/**
 * @brief Configuración de fuente de clock para el clock principal
 * @param[in] clock_source Selección deseada
 */
void syscon_system_clock_set_source(syscon_system_clock_sel_en clock_source)
{
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_SYSOSC);    /*!< Ensure Main osc is on */
	SYSCON_set_system_clock_source(clock_source);

	switch(clock_source)
	{
	case kSYSCON_SYSTEM_CLOCK_SEL_FRO: { current_main_freq = &current_fro_freq; break; }
	case kSYSCON_SYSTEM_CLOCK_SEL_FRO_DIV: { current_main_freq = &current_fro_div_freq; break; }
	case kSYSCON_SYSTEM_CLOCK_SEL_EXT:
	{
		if(SYSCON_ext_clock_source_get() == SYSCON_EXT_CLOCK_SOURCE_SEL_CLK_IN)
			current_main_freq = &current_ext_freq;
		else
			current_main_freq = &current_crystal_freq;

		break;
	}
	case kSYSCON_SYSTEM_CLOCK_SEL_WATCHDOG: { current_main_freq = &current_watchdog_freq; break; }
	case kSYSCON_SYSTEM_CLOCK_SEL_PLL: { current_main_freq = &current_pll_freq; break; }
	}
}

/*
 * @brief Fijar el divisor del clock principal
 * @param[in] div Divisor deseado. Cero inhabilita el clock principal
 */
void syscon_system_clock_set_divider(uint8_t div)
{
	current_main_div = div;
	SYSCON_set_system_clock_divider(div);
}

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t syscon_fro_clock_get(void)
{
	return current_fro_freq;
}

/**
 * @brief Configurar el ext clock a partir de un cristal externo
 * @param[in] crystal_freq Frecuencia del cristal externo utilizado
 */
void syscon_external_crystal_config(uint32_t crystal_freq)
{
	uint8_t counter;

	// Remocion de pull ups en los pines XTAL
	IOCON_init();
	IOCON_config_pull_mode(XTALIN_PORT, XTALIN_PIN, IOCON_PULL_NONE);
	IOCON_config_pull_mode(XTALOUT_PORT, XTALOUT_PIN, IOCON_PULL_NONE);

	// Habilitacion de los pines XTAL
	SWM_init();
	SWM_enable_XTALIN(SWM_ENABLE);
	SWM_enable_XTALOUT(SWM_ENABLE);
	SWM_deinit();

	if(crystal_freq < 20e6)
	{
		SYSCON_set_oscillator_control(SYSCON_BYPASS_DISABLED, SYSCON_FREQRANGE_MINUS_20MHZ);
	}
	else
	{
		SYSCON_set_oscillator_control(SYSCON_BYPASS_DISABLED, SYSCON_FREQRANGE_PLUS_20MHZ);
	}

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_SYSOSC);

	counter = crystal_freq / 100; // Delay de aprox 1mseg a lo guaso
	while(counter) counter--; // Estabilizacion del cristal

	SYSCON_ext_clock_source_set(SYSCON_EXT_CLOCK_SOURCE_SEL_CLK_IN);
	current_crystal_freq = crystal_freq;
}

/**
 * @brief Configurar el ext clock a partir de una fuente de clock externa
 * @param[in] external_clock_freq Frecuencia de la fuente de clock externa en Hz
 */
void syscon_external_clock_config(uint32_t external_clock_freq)
{
	SYSCON_ext_clock_source_set(SYSCON_EXT_CLOCK_SOURCE_SEL_CLK_IN);
	current_ext_freq = external_clock_freq;
}

/**
 * @brief Configurar el clock FRO
 *
 * @note Esta función habilita el FRO
 *
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 */
void syscon_fro_clock_config_v1(uint8_t direct)
{
	// Encendido del FRO
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_FRO);
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_FROOUT);

	if(direct)
	{
		SYSCON_set_fro_direct();
		current_fro_freq = (uint32_t) (FRO_DIRECT_FREQ);
		current_fro_div_freq = (uint32_t) ((FRO_DIRECT_FREQ) / 2);
	}
	else
	{
		SYSCON_clear_fro_direct();
		current_fro_freq = (uint32_t) ((FRO_DIRECT_FREQ) / 2);
		current_fro_div_freq = (uint32_t) ((FRO_DIRECT_FREQ) / 4);
	}
}

/**
 * @brief Configurar el clock FRO
 *
 * @note Esta función habilita el FRO
 *
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 */
void syscon_fro_clock_config(clock_fro_src_t direct, clock_fro_osc_freq_t fro_osc_freq)
{
	// Encendido del FRO
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_FRO);
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_FROOUT);

	SYSCON_set_fro_osc_freq(fro_osc_freq);			/*!< Set up FRO freq */

	if(direct)
	{
		SYSCON_set_fro_direct();
		current_fro_freq = (uint32_t) syscon_get_fro_freq();
		current_fro_div_freq = (uint32_t) (syscon_get_fro_freq() / 2);
	}
	else
	{
		SYSCON_clear_fro_direct();
		current_fro_freq = (uint32_t) (syscon_get_fro_freq() / 2);
		current_fro_div_freq = (uint32_t) (syscon_get_fro_freq() / 4);
	}
}

/**
 * @brief Inhabilitar el FRO
 */
void syscon_fro_clock_disable(void)
{
	SYSCON_power_down_peripheral(SYSCON_POWER_SEL_FRO);
	SYSCON_power_down_peripheral(SYSCON_POWER_SEL_FROOUT);
}

/**
 * @brief Configurar el pin de clock out (salida de clock hacia afuera)
 * @param[in] portpin Número de puerto/pin por donde sacar el clock out
 * @param[in] clock_source Fuente deseada para la salida clock out
 * @param[in] divider Divisor deseado para la salida clock out
 */
void syscon_clkout_config(gpio_portpin_en portpin, syscon_clkout_source_sel_en clock_source, uint8_t divider)
{
	SYSCON_set_clkout_config(clock_source, divider);

	SWM_init();
	SWM_assign_CLKOUT(portpin);
	SWM_deinit();
}

/**
 * @brief Configurar el divisor fraccional
 *
 * El divisor siempre se debe fijar en 256 para estos MCU.
 *
 * @param[in] inst Instancia de FRG a configurar
 * @param[in] clock_source Fuente de clock de entrada para el FRG
 * @param[in] mul Multiplicador deseado
 */
void syscon_frg_config(uint8_t inst, syscon_frg_clock_sel_en clock_source, uint32_t mul)
{
	uint32_t aux_freq;

	SYSCON_set_frg_config(inst, clock_source, mul, 0xFF);

	switch(clock_source)
	{
	case kSYSCON_FRG_CLOCK_SEL_FRO: { aux_freq = current_fro_freq; break; }
	case kSYSCON_FRG_CLOCK_SEL_MAIN_CLOCK: { aux_freq = *current_main_freq; break; }
	case kSYSCON_FRG_CLOCK_SEL_NONE: { aux_freq = 0; break; }
	case kSYSCON_FRG_CLOCK_SEL_SYS_PLL: { aux_freq = current_pll_freq; break; }
	}

	current_frg_freq[inst] = (uint32_t) (aux_freq / (1 + ((float) mul / 256)));
}

/**
 * @brief Configuración del watchdog oscillator
 * @param[in] clkana_sel Selección de frecuencia base del oscilador
 * @param[in] div Divisor. El valor efectivo de división es: 2 (1 + div)
 *
 * @note Recordar que la presición de este oscilador es de más/menos 40%
 */
void syscon_watchdog_oscillator_config(syscon_watchdog_clkana_sel_en clkana_sel, uint8_t div)
{
	SYSCON_set_watchdog_oscillator_control(div, clkana_sel);

	current_watchdog_freq = base_watchdog_freq[clkana_sel] / (2 * (1 + div));
}

/**
 * @brief Obtener la frecuencia de clock en Hz configurada para cierto periférico
 * @param[in] peripheral Periférico deseado
 * @return Frecuencia en Hz del clock del periférico
 */
uint32_t syscon_peripheral_clock_get(syscon_peripheral_sel_en peripheral)
{
	uint32_t ret;

	switch(SYSCON->PERCLKSEL[peripheral].SEL)
	{
	case kSYSCON_PERIPHERAL_CLOCK_SEL_FRG0: { ret = syscon_get_FRG0_clk_freq(); /*current_frg_freq[0];*/ break; }
	case kSYSCON_PERIPHERAL_CLOCK_SEL_FRG1: { ret = syscon_get_FRG1_clk_freq(); /*current_frg_freq[1];*/ break; }
	case kSYSCON_PERIPHERAL_CLOCK_SEL_FRO: { ret = syscon_get_fro_freq(); /*current_fro_freq;*/ break; }
	case kSYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV: { ret = syscon_get_fro_freq() / 2; /* current_fro_freq / 2;*/ break; }
	case kSYSCON_PERIPHERAL_CLOCK_SEL_MAIN: { ret = syscon_get_main_clk_freq(); /*current_main_freq;*/ break; }
	case kSYSCON_PERIPHERAL_CLOCK_SEL_NONE: { ret = 0; break; }
	}

	return ret;
}

/**
 * @brief Configurar divisor para el clock de glitches del IOCON
 * @param[in] sel Selección de divisor
 * @param[in] div Valor de división deseado
 */
void syscon_iocon_glitch_divider_set(syscon_iocon_glitch_sel_en sel, uint32_t div)
{
	SYSCON_set_iocon_glitch_divider(sel, div);
}

/**
 * @brief Configurar el PLL
 * @param[in] clock_source Fuente de clock de referencia para el PLL
 * @param[in] freq Frecuencia deseada de salida del PLL
 */
void syscon_pll_clock_config(syscon_pll_source_sel_en clock_source, uint32_t freq)
{
	#warning Ojo, falta hacer la implementación de esta función
	current_pll_freq = 0; // Tiene que dejar el valor correcto de la frecuencia del PLL, en Hz
}

/**
 * @brief Obtener frecuencia actual configurada del PLL
 * @return Frecuencia actual del PLL en Hz
 */
uint32_t syscon_pll_clock_get(void)
{
	return current_pll_freq;
}

/* ---------------- */

/*! brief  Return Frequency of UART0
 *  return Frequency of UART0
 */
uint32_t syscon_get_usart_clk_freq(uint8_t inst)
{
    uint32_t freq = 0U;

    switch (SYSCON->PERCLKSEL[inst].SEL)
    {
        case 0U:
            freq = syscon_get_fro_freq();

            break;
        case 1U:
            freq = syscon_get_main_clk_freq();
            break;
        case 2U:
            freq = syscon_get_FRG0_clk_freq();
            break;
        case 3U:
            freq = syscon_get_FRG1_clk_freq();
            break;
        case 4U:
            freq = syscon_get_fro_freq() >> 1U;
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! brief  Return Frequency of FRO.
 *  return Frequency of FRO.
 */
uint32_t syscon_get_fro_freq(void)
{
    uint32_t froOscSel  = SYSCON->FROOSCCTRL.CORESEL;
    uint32_t froOscFreq = 0U;

    if (froOscSel == 0U)
    {
        froOscFreq = 18000000U;
    }
    else if (froOscSel == 1U)
    {
        froOscFreq = 24000000U;
    }
    else
    {
        froOscFreq = 30000000U;
    }

    if (SYSCON->FROOSCCTRL.FRO_DIRECT == 0U) {
        /* need to check the FAIM low power boot value */
        froOscFreq /= ((*((volatile uint32_t *)(CLOCK_FAIM_BASE)) & 0x2U) != 0UL) ? 16U : 2U;
    }

    return froOscFreq;
}

/*! brief  Return Frequency of Main Clock.
 *  return Frequency of Main Clock.
 */
uint32_t syscon_get_main_clk_freq(void)
{
    uint32_t freq = 0U;

    if (SYSCON->MAINCLKPLLSEL.SEL == 1)
    {
        return syscon_get_system_PLL_freq();
    }

    switch (SYSCON->MAINCLKSEL.SEL)
    {
        case 0U:
            freq = syscon_get_fro_freq();
            break;

        case 1U:
            freq = syscon_get_ext_clk_rreq();
            break;

        case 2U:
            freq = syscon_get_wdt_osc_freq();
            break;

        case 3U:
            freq = syscon_get_fro_freq() >> 1U;
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! @brief  Return Frequency of System PLL
 *  @return Frequency of PLL
 */
uint32_t syscon_get_system_PLL_freq(void)
{
    return syscon_get_system_PLL_input_clock_rate() * (SYSCON->SYSPLLCTRL.MSEL + 1U);
}

/*! brief  Return System PLL input clock rate
 *  return System PLL input clock rate
 */
uint32_t syscon_get_system_PLL_input_clock_rate(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->SYSPLLCLKSEL.SEL)
    {
        /* source from fro div clock */
        case 0x03U:
            freq = syscon_get_fro_freq() >> 1U;
            break;
        /* source from the fro clock */
        case 0x00U:
            freq = syscon_get_fro_freq();
            break;
        /* source from external clock in */
        case 0x01U:
            freq = syscon_get_ext_clk_rreq();
            break;
        /* source from watchdog oscillator */
        case 0x02U:
            freq = syscon_get_wdt_osc_freq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! brief  Return Frequency of FRG0 Clock.
 *  return Frequency of FRG0 Clock.
 */
uint32_t syscon_get_FRG0_clk_freq(void)
{
    return (uint32_t)((uint64_t)(syscon_get_FRG_input_clk_freq(kSYSCON_FRG0)) / ((uint64_t)SYSCON->FRG0MUL.MULT + 256ULL));
}

/*! brief  Return Frequency of FRG1 Clock.
 *  return Frequency of FRG1 Clock.
 */
uint32_t syscon_get_FRG1_clk_freq(void)
{
	return (uint32_t)((uint64_t)(syscon_get_FRG_input_clk_freq(kSYSCON_FRG1)) / ((uint64_t)SYSCON->FRG1MUL.MULT + 256ULL));
}


uint32_t syscon_get_FRG_input_clk_freq(syscon_frg_en frg)
{
    uint32_t sel = 0;

    if (frg == kSYSCON_FRG0)
    	sel = SYSCON->FRG0CLKSEL.SEL;

    if (frg == kSYSCON_FRG1)
    	sel = SYSCON->FRG1CLKSEL.SEL;

    if (sel == 0U)
    {
        return syscon_get_fro_freq();
    }
    else if (sel == 1U)
    {
        return syscon_get_main_clk_freq();
    }
    else
    {
        return syscon_get_system_PLL_freq();
    }
}

/*! @brief  Get watch dog OSC frequency
 * @retval watch dog OSC frequency value.
 */
uint32_t syscon_get_wdt_osc_freq(void)
{
    return g_Wdt_Osc_Freq;
}

/*! @brief  Get external clock frequency
 * @retval external clock frequency value.
 */
uint32_t syscon_get_ext_clk_rreq(void)
{
    return g_Ext_Clk_Freq;
}
