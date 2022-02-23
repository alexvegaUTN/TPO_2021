/*******************************************************************************************************************************//**
 *
 * @file		DR_SYSCON.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <assert.h>
#include "DR_SYSCON.h"

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
volatile SYSCON_per_t * const SYSCON = (SYSCON_per_t *) SYSCON_BASE; //!< Periferico SYSCON

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
static void SYSCON_update_clk_src(volatile uint32_t *base, uint32_t mask);

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
/*
 *! @brief	.
 * @param	base: , mask: .
 * @return	Nothing
 */
static void SYSCON_update_clk_src(volatile uint32_t *base, uint32_t mask)
{
    assert(base);

    *base &= ~mask;
    *base |= mask;
    while ((*base & mask) == 0U)
    {
    }
}

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

/**
 * @brief Configuracion del registro de control del PLL
 * @param[in] m Valor del divisor del feedback
 * @param[in] p Valor del post divisor
 */
void SYSCON_set_pll_control(uint8_t m, uint8_t p)
{
	SYSCON->SYSPLLCTRL.MSEL = m;
	SYSCON->SYSPLLCTRL.PSEL = p;
}

/**
 * @brief Obtener estado de lock del PLL
 * @return Valor del estado de lock del PLL
 */
uint8_t SYSCON_get_pll_lock_status(void)
{
	return SYSCON->SYSPLLSTAT.LOCK;
}

/**
 * @brief Configurar el registro de control del sistema del oscilador
 * @param[in] bypass Seleccion de bypass
 * @param[in] freqrange Rango de frecuencia a utilizar
 */
void SYSCON_set_oscillator_control(SYSCON_bypass_sel_en bypass, SYSCON_freqrange_sel_en freqrange)
{
	SYSCON->SYSOSCCTRL.BYPASS = bypass;
	SYSCON->SYSOSCCTRL.FREQRANGE = freqrange;
}

/**
 * @brief Configuracion del registro de control del oscilador del watchdog
 * @param[in] divsel Seleccion de divsel
 * @param[in] clkana_sel Seleccion de frecuencia base
 */
void SYSCON_set_watchdog_oscillator_control(uint8_t divsel, SYSCON_watchdog_clkana_sel_en clkana_sel)
{
	SYSCON->WDTOSCCTRL.DIVSEL = divsel;
	SYSCON->WDTOSCCTRL.FREQSEL = clkana_sel;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador directo (sin division)
 */
void SYSCON_set_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 1;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador dividido (con division dependiente de FAIM)
 */
void SYSCON_clear_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;
}

/**
 * @brief Configuracion de la fuente de clock para el PLL
 * @param[in] pll_source Fuente de entrada para el PLL
 */
void SYSCON_set_pll_clk_source(SYSCON_pll_source_sel_en pll_source)
{
	SYSCON->SYSPLLCLKSEL.SEL = pll_source;

	SYSCON->SYSPLLCLKUEN.ENA = 0;
	SYSCON->SYSPLLCLKUEN.ENA = 1;
}

/*
 * @brief Seleccion de fuente de clock para el clock principal
 * @param[in] clock_selection Seleccion de clock deseada
 */
void SYSCON_set_system_clock_source(SYSCON_main_clock_sel_en clock_selection)
{
	if(clock_selection != SYSCON_MAIN_CLOCK_SEL_PLL)
	{
		SYSCON->MAINCLKSEL.SEL = clock_selection;
		SYSCON->MAINCLKUEN.ENA = 0;
		SYSCON->MAINCLKUEN.ENA = 1;

		SYSCON->MAINCLKPLLSEL.SEL = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 1;
	}
	else
	{
		SYSCON->MAINCLKPLLSEL.SEL = 1;
		SYSCON->MAINCLKPLLUEN.ENA = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 1;
	}
}

/*
 * @brief Seleccion del divisor del system clock
 * @param[in] divider division deseada (cero desactiva el system clock)
 */
void SYSCON_set_system_clock_divider(uint8_t divider)
{
	SYSCON->SYSAHBCLKDIV.DIV = divider;
}

/**
 * @brief Seleccion de la fuente de clock para el periferico de control de touch capacitivo
 * @param[in] source_sel Seleccion de fuente de clock deseada
 */
void SYSCON_set_capacitive_clock_source(SYSCON_capacitive_clock_sel_en source_sel)
{
	SYSCON->CAPTCLKSEL.SEL = source_sel;
}

/**
 * @brief Seleccion de clock y divisor para el periferico ADC
 * @param[in] clock_sel Fuente de clock deseada para el periferico
 * @param[in] div Divisor deseado
 */
void SYSCON_set_adc_clock(SYSCON_adc_clock_sel_en source_sel, uint8_t div)
{
	SYSCON->ADCCLKSEL.SEL = source_sel;
	SYSCON->ADCCLKDIV.DIV = div;
}

/**
 * @brief Seleccion de clock y divisor para el periferico SCT
 * @param[in] clock_sel Fuente de clock deseada para el periferico
 * @param[in] div Divisor deseado
 */
void SYSCON_set_sct_clock(SYSCON_sct_clock_sel_en source_sel, uint8_t div)
{
	SYSCON->SCTCLKSEL.SEL = source_sel;
	SYSCON->SCTCLKDIV.DIV = div;
}

/**
 * @brief Seleccion de fuente para el clock externo
 * @param[in] source_selection Seleccion deseada
 */
void SYSCON_ext_clock_source_set(SYSCON_ext_clock_source_sel_en source_selection)
{
	SYSCON->EXTCLKSEL.SEL = source_selection;
}

/**
 * @brief Obtención de fuente de clock externo configurada
 * @return Configuración actual
 */
SYSCON_ext_clock_source_sel_en SYSCON_ext_clock_source_get(void)
{
	return SYSCON->EXTCLKSEL.SEL;
}

/**
 * @brief Habilitacion del clock de un periferico
 * @param[in] peripheral Periferico en el cual habilitar el clock
 */
void SYSCON_enable_clock(SYSCON_enable_clock_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL1) |= (1 << peripheral);
	}
}

/**
 * @brief Inhabilitacion del clock de un periferico
 * @param[in] peripheral Periferico en el cual habilitar el clock
 */
void SYSCON_disable_clock(SYSCON_enable_clock_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL1) |= (1 << peripheral);
	}
}

/**
 * @brief Generar el reset en el periferico seleccionado
 * @param[in] peripheral Periferico a generar el reset
 */
void SYSCON_assert_reset(SYSCON_reset_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->PRESETCTRL0) &= ~(1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->PRESETCTRL1) &= ~(1 << peripheral);
	}
}

/**
 * @brief Liberar el reset en el periferico seleccionado
 * @param[in] peripheral Periferico a liberar el reset
 */
void SYSCON_clear_reset(SYSCON_reset_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->PRESETCTRL0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->PRESETCTRL1) |= (1 << peripheral);
	}
}

/**
 * @brief Seleccion de fuente de clock para los distintos perifericos
 * @param[in] peripheral Periferico cuya fuente seleccionar
 * @param[in] clock Fuente de clock para el periferico seleccionada
 */
void SYSCON_set_peripheral_clock_source(SYSCON_peripheral_sel_en peripheral, SYSCON_peripheral_clock_sel_en clock)
{
	SYSCON->PERCLKSEL[peripheral].SEL = clock;
}

/**
 * @brief Configuracion del FRG
 * @param[in] frg_selection Cual de los FRG configurar, cero o uno
 * @param[in] clock_source Fuente de clock del FRG
 * @param[in] mul Multiplicador del FRG 0 ~ 255
 * @param[in] div Divisor del FRG 0~255
 */
void SYSCON_set_frg_config(uint8_t frg_selection, SYSCON_frg_clock_sel_en clock_source, uint8_t mul, uint8_t div)
{
	if(frg_selection == 0)
	{
		SYSCON->FRG0CLKSEL.SEL = clock_source;
		SYSCON->FRG0DIV.DIV = div;
		SYSCON->FRG0MUL.MULT = mul;
	}
	else if(frg_selection == 1)
	{
		SYSCON->FRG1CLKSEL.SEL = clock_source;
		SYSCON->FRG1DIV.DIV = div;
		SYSCON->FRG1MUL.MULT = mul;
	}
}

/**
 * @brief Seleccion de fuente para el CLOCK OUT
 * @param[in] clock_source Fuente deseada
 * @param[in] divider Divisor del CLOCK OUT
 */
void SYSCON_set_clkout_config(SYSCON_clkout_source_sel_en clock_source, uint8_t divider)
{
	SYSCON->CLKOUTSEL.SEL = clock_source;
	SYSCON->CLKOUTDIV.DIV = divider;
}

/**
 * @brief Leer el contenido de PIOPORCAPn
 * @param[in] inst Instancia a leer (0 o 1)
 * @return Valor del registro leido
 */
uint32_t SYSCON_get_por_pio_status_register(uint8_t inst)
{
	return SYSCON->PIOPORCAP[inst].PIOSTAT;
}

/**
 * @brief Fijar el divisor para el filtro de glitches del IOCON
 * @param[in] inst Instancia a escribir
 * @param[in] div Divisor deseado
 */
void SYSCON_set_iocon_glitch_divider(uint8_t inst, uint8_t div)
{
	switch(inst)
	{
	case 0: { SYSCON->IOCONCLKDIV0.DIV = div; break; }
	case 1: { SYSCON->IOCONCLKDIV1.DIV = div; break; }
	case 2: { SYSCON->IOCONCLKDIV2.DIV = div; break; }
	case 3: { SYSCON->IOCONCLKDIV3.DIV = div; break; }
	case 4: { SYSCON->IOCONCLKDIV4.DIV = div; break; }
	case 5: { SYSCON->IOCONCLKDIV5.DIV = div; break; }
	case 6: { SYSCON->IOCONCLKDIV6.DIV = div; break; }
	}
}

/**
 * @brief Configurar el registro de control del brown-out detector
 * @param[in] reset_level Nivel deseado para el reset
 * @param[in] bod_level Nivel deseado para el BOD
 * @param[in] reset_enable Habilitacion deseada para el reset
 */
void SYSCON_set_bod_control(SYSCON_bod_level_en reset_level, SYSCON_bod_level_en bod_level, SYSCON_bod_enale_en reset_enable)
{
	SYSCON->BODCTRL.BODRSTLEV = reset_level;
	SYSCON->BODCTRL.BODINTVAL = bod_level;
	SYSCON->BODCTRL.BODRSTENA = reset_enable;
}

/**
 * @brief Obtener el valor de calibracion del SYSTICK
 * @return Valor de calibracion del SYSTICK
 */
uint32_t SYSCON_get_systick_calib(void)
{
	return SYSCON->SYSTCKCAL.CAL;
}

/**
 * @brief Obtener el valor de latencia de interrupciones del MCU
 * @return Valor de latencia de interrupcion
 */
uint8_t SYSCON_get_irq_latency(void)
{
	return SYSCON->IRQLATENCY.LATENCY;
}

/**
 * @brief Fijar que numero de interrupcion actuara como NMI
 * @param[in] irq Numero de interrupcion deseado
 * @param[in] enable Habilitacion/inhabilitacion de la NMI
 */
void SYSCON_set_nmi_source(uint8_t irq, SYSCON_nmi_enable_en enable)
{
	SYSCON->NMISRC.IRQN = irq;
	SYSCON->NMISRC.NMIEN = enable;
}

/**
 * @brief Configurar pin a utilizar como fuente de PININT
 * @param[in] channel Canal de PININT a configurar
 * @param[in] portpin Puerto/pin a utilizar
 */
void SYSCON_set_pinint_pin(uint8_t channel, GPIO_portpin_en portpin)
{
	SYSCON->PINTSEL[channel].INTPIN = portpin;
}

/**
 * @brief Habilitar alguna de las interrupciones del periferico seleccionado como fuente de wakeup
 * @param[in] peripheral Periferico deseado
 */
void SYSCON_enable_wakeup_source(SYSCON_enable_wakeup_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->STARTERP0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->STARTERP1) |= (1 << peripheral);
	}
}

/**
 * @brief Inhabilitar alguna de las interrupciones del periferico seleccionado como fuente de wakeup
 * @param[in] peripheral Periferico deseado
 */
void SYSCON_disable_wakeup_source(SYSCON_enable_wakeup_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->STARTERP0) &= ~(1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->STARTERP1) &= ~(1 << peripheral);
	}
}

/**
 * @brief Habilitar o inhabilitacion de la alimentacion del BOD en deep sleep
 * @param[in] power Habilitacion o inhabilitacion de la alimentacion
 */
void SYSCON_deep_sleep_power_bod(SYSCON_deep_sleep_power_en power)
{
	SYSCON->PDSLEEPCFG.BOD_PD = power;
}

/**
 * @brief Habilitar o inhabilitacion de la alimentacion del WDTOSC en deep sleep
 * @param[in] power Habilitacion o inhabilitacion de la alimentacion
 */
void SYSCON_deep_sleep_power_wdtosc(SYSCON_deep_sleep_power_en power)
{
	SYSCON->PDSLEEPCFG.WDTOSC_PD = power;
}

/**
 * @brief Fijar que un periferico comience encendido al haber un wakeup
 * @param[in] peripheral Periferico que comenzara encendido al haber un wakeup
 */
void SYSCON_set_powered_on_wakeup(SYSCON_wakeup_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDAWAKECFG) &= ~(1 << peripheral);
}

/**
 * @brief Fijar que un periferico comience apagado al haber un wakeup
 * @param[in] peripheral Periferico que comenzara apagado al haber un wakeup
 */
void SYSCON_clear_powered_on_wakeup(SYSCON_wakeup_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDAWAKECFG) |= (1 << peripheral);
}

/**
 * @brief Encender el periferico seleccionado
 * @param[in] peripheral Periferico a encender
 */
void SYSCON_power_up_peripheral(SYSCON_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDRUNCFG) &= ~(1 << peripheral);
}

/**
 * @brief Apagar el periferico seleccionado
 * @param[in] peripheral Periferico a encender
 */
void SYSCON_power_down_peripheral(SYSCON_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDRUNCFG) |= (1 << peripheral);
}

/**
 * @brief Obtener el Device ID
 * @return Device ID
 */
uint32_t SYSCON_get_device_id(void)
{
	return SYSCON->DEVICE_ID.DEVICE_ID;
}

/*! @brief Set FRO oscillator output frequency.
 *  Initialize the FRO clock to given frequency (18, 24 or 30 MHz).
 * @param freq Please refer to clock_fro_osc_freq_t definition, frequency must be one of 18000, 24000 or 30000 KHz.
 *
 */
void SYSCON_set_fro_osc_freq(clock_fro_osc_freq_t freq)
{
    (*((void (*)(uint32_t freq))(CLOCK_FRO_SETTING_API_ROM_ADDRESS)))(freq);
}


/*! brief Set FRO clock source
 * param src, please reference _clock_fro_src definition.
 *
 */
void SYSCON_set_fro_out_clk_src(clock_fro_src_t src)
{
	//*((uint32_t*) &SYSCON->PDRUNCFG)

    if (src != SYSCON->FROOSCCTRL.FRO_DIRECT)//((uint32_t)src != (*((uint32_t*) &SYSCON->FROOSCCTRL) & SYSCON_FROOSCCTRL_FRO_DIRECT_MASK))
    {
    	SYSCON->FROOSCCTRL.FRO_DIRECT = src;
        //SYSCON->FROOSCCTRL = (SYSCON->FROOSCCTRL & (~SYSCON_FROOSCCTRL_FRO_DIRECT_MASK)) | (uint32_t)src;
        /* Update clock source */
    	SYSCON_update_clk_src((volatile uint32_t *)(&(SYSCON->FRODIRECTCLKUEN)), SYSCON_FRODIRECTCLKUEN_ENA_MASK);
    }
}

/*! brief  Set main clock reference source.
 * param src, reference clock_main_clk_src_t to set the main clock source.
 */
void SYSCON_set_main_clk_src(clock_main_clk_src_t src)
{
    uint32_t mainMux = CLK_MAIN_CLK_MUX_GET_MUX(src);
    uint32_t mainPreMux = CLK_MAIN_CLK_MUX_GET_PRE_MUX(src);


    if ((SYSCON->MAINCLKSEL.SEL != mainPreMux) && (mainMux == 0U))
    {
        *((uint32_t *)&SYSCON->MAINCLKSEL) = (*((uint32_t *)&SYSCON->MAINCLKSEL) & (~SYSCON_MAINCLKSEL_SEL_MASK)) | SYSCON_MAINCLKSEL_SEL(mainPreMux);
        SYSCON_update_clk_src((volatile uint32_t *)(&(SYSCON->MAINCLKUEN)), SYSCON_MAINCLKUEN_ENA_MASK);
    }

    if ((*((uint32_t *)&SYSCON->MAINCLKSEL) & (~SYSCON_MAINCLKSEL_SEL_MASK)) != mainMux)
    {
    	*((uint32_t *)&SYSCON->MAINCLKPLLSEL) = (*((uint32_t *)&SYSCON->MAINCLKPLLSEL) & (~SYSCON_MAINCLKPLLSEL_SEL_MASK)) | SYSCON_MAINCLKPLLSEL_SEL(mainMux);
        SYSCON_update_clk_src((volatile uint32_t *)(&(SYSCON->MAINCLKPLLUEN)), SYSCON_MAINCLKPLLUEN_ENA_MASK);
    }
}

/*
 *! @brief   Setup Core clock dividers.
 * Be careful about the core divider value, due to core/system frequency should be lower than 30MHZ.
 * @param   value: Value to be divided
 * @return  Nothing
 */
void SYSCON_set_core_sys_clk_div(uint32_t value)
{
    assert(value != 0U);

    SYSCON->SYSAHBCLKDIV.DIV = value;
    //(*((uint32_t *) &SYSCON->SYSAHBCLKDIV)) = ((*((uint32_t *) &SYSCON->SYSAHBCLKDIV)) & (~SYSCON_SYSAHBCLKDIV_DIV_MASK)) | SYSCON_SYSAHBCLKDIV_DIV(value);
}

