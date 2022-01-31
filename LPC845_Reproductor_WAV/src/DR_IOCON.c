/*******************************************************************************************************************************//**
 *
 * @file		DR_IOCON.c
 * @brief		Descripcion del modulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_IOCON.h"

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
volatile IOCON_per_t * const IOCON = (IOCON_per_t *) IOCON_BASE; 	//!< Periferico IOCON
volatile IOCON_PIO_reg_t dummy_reg; 								//!< Registro dummy para los pines no disponibles en el encapsulado

volatile IOCON_PIO_reg_t * const IOCON_PIN_TABLE[2][32] = 			//!< Tabla de registros de configuracion
{
	{ // Port 0
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_0, (IOCON_PIO_reg_t * ) &IOCON->PIO0_1,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_2, (IOCON_PIO_reg_t * ) &IOCON->PIO0_3,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_4, (IOCON_PIO_reg_t * ) &IOCON->PIO0_5,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_6, (IOCON_PIO_reg_t * ) &IOCON->PIO0_7,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_8, (IOCON_PIO_reg_t * ) &IOCON->PIO0_9,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_10, (IOCON_PIO_reg_t * ) &IOCON->PIO0_11,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_12, (IOCON_PIO_reg_t * ) &IOCON->PIO0_13,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_14, (IOCON_PIO_reg_t * ) &IOCON->PIO0_15,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_16, (IOCON_PIO_reg_t * ) &IOCON->PIO0_17,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_18, (IOCON_PIO_reg_t * ) &IOCON->PIO0_19,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_20, (IOCON_PIO_reg_t * ) &IOCON->PIO0_21,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_22, (IOCON_PIO_reg_t * ) &IOCON->PIO0_23,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_24, (IOCON_PIO_reg_t * ) &IOCON->PIO0_25,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_26, (IOCON_PIO_reg_t * ) &IOCON->PIO0_27,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_28, (IOCON_PIO_reg_t * ) &IOCON->PIO0_29,
		(IOCON_PIO_reg_t * ) &IOCON->PIO0_30, (IOCON_PIO_reg_t * ) &IOCON->PIO0_31
	},

	{ // Port 1
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_0, (IOCON_PIO_reg_t * ) &IOCON->PIO1_1,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_2, (IOCON_PIO_reg_t * ) &IOCON->PIO1_3,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_4, (IOCON_PIO_reg_t * ) &IOCON->PIO1_5,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_6, (IOCON_PIO_reg_t * ) &IOCON->PIO1_7,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_8, (IOCON_PIO_reg_t * ) &IOCON->PIO1_9,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_10, (IOCON_PIO_reg_t * ) &IOCON->PIO1_11,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_12, (IOCON_PIO_reg_t * ) &IOCON->PIO1_13,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_14, (IOCON_PIO_reg_t * ) &IOCON->PIO1_15,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_16, (IOCON_PIO_reg_t * ) &IOCON->PIO1_17,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_18, (IOCON_PIO_reg_t * ) &IOCON->PIO1_19,
		(IOCON_PIO_reg_t * ) &IOCON->PIO1_20, (IOCON_PIO_reg_t * ) &IOCON->PIO1_21,
		&dummy_reg, &dummy_reg,
		&dummy_reg, &dummy_reg, &dummy_reg, &dummy_reg,
		&dummy_reg, &dummy_reg, &dummy_reg, &dummy_reg
	},
};

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

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

/**
 * @brief Inicializacion del modulo IOCON
 *
 * Unicamente habilita el clock del modulo
 */
void IOCON_init(void)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_IOCON);
}

/**
 * @brief Inhabilitacion del modulo IOCON
 *
 * Unicamente inhabilita el clock del modulo
 */
void IOCON_deinit(void)
{
	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_IOCON);
}

/**
 * @brief Configurar modo de funcionamiento (pull up, pull down, etc) en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 * @param[in] pull_mode Modo de funcionamiento
 */
void IOCON_config_pull_mode(uint8_t port, uint8_t pin, IOCON_pull_mode_en pull_mode)
{
	IOCON_PIN_TABLE[port][pin]->MODE = pull_mode;
}

/**
 * @brief Habilitar histeresis en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
void IOCON_enable_hysteresis(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->HYS = 1;
}

/**
 * @brief Inhabilitar histeresis en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
void IOCON_disable_hysteresis(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->HYS = 0;
}

/**
 * @brief Habilitar inversion en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
void IOCON_enable_invert(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->INV = 1;
}

/**
 * @brief Inhabilitar inversion en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
void IOCON_disable_invert(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->INV = 0;
}

/**
 * @brief Habilitar modo open drain en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
void IOCON_enable_open_drain(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->OD = 1;
}

/**
 * @brief Inhabilitar modo open drain en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
void IOCON_disable_open_drain(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->OD = 0;
}

/**
 * @brief Configurar modo de sampleo en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 * @param[in] sample_mode Modo de sampleo
 */
void IOCON_config_sample_mode(uint8_t port, uint8_t pin, IOCON_sample_mode_en sample_mode)
{
	IOCON_PIN_TABLE[port][pin]->S_MODE = sample_mode;
}

/**
 * @brief Configurar fuente utilizada para el sampleo en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 * @param[in] clock_source Fuente de clock deseada
 */
void IOCON_config_clock_source(uint8_t port, uint8_t pin, IOCON_clk_sel_en clock_source)
{
	IOCON_PIN_TABLE[port][pin]->CLK_DIV = clock_source;
}

/**
 * @brief Inhabilitar pull-up/pull-down en entrada analógica del periférico ADC.
 * @param[in] channel Canal de *ADC*
 */
void IOCON_disable_pullup_adc(uint8_t channel)
{
	switch(channel)
	{
	case 0: { IOCON_PIN_TABLE[0][7]->MODE = IOCON_PULL_NONE; break; }
	case 1: { IOCON_PIN_TABLE[0][6]->MODE = IOCON_PULL_NONE; break; }
	case 2: { IOCON_PIN_TABLE[0][14]->MODE = IOCON_PULL_NONE; break; }
	case 3: { IOCON_PIN_TABLE[0][23]->MODE = IOCON_PULL_NONE; break; }
	case 4: { IOCON_PIN_TABLE[0][22]->MODE = IOCON_PULL_NONE; break; }
	case 5: { IOCON_PIN_TABLE[0][21]->MODE = IOCON_PULL_NONE; break; }
	case 6: { IOCON_PIN_TABLE[0][20]->MODE = IOCON_PULL_NONE; break; }
	case 7: { IOCON_PIN_TABLE[0][19]->MODE = IOCON_PULL_NONE; break; }
	case 8: { IOCON_PIN_TABLE[0][18]->MODE = IOCON_PULL_NONE; break; }
	case 9: { IOCON_PIN_TABLE[0][17]->MODE = IOCON_PULL_NONE; break; }
	case 10: { IOCON_PIN_TABLE[0][13]->MODE = IOCON_PULL_NONE; break; }
	case 11: { IOCON_PIN_TABLE[0][4]->MODE = IOCON_PULL_NONE; break; }
	}
}

/**
 * @brief Inhabilitar pull-up/pull-down en entrada analógica del comparador analógico.
 * @param[in] acmp_input Entrada del comparador analógico.
 */
void IOCON_disable_pullup_acmp_input(uint8_t acmp_input)
{
	switch(acmp_input)
	{
	case 1: { IOCON_PIN_TABLE[0][0]->MODE = IOCON_PULL_NONE; break; }
	case 2: { IOCON_PIN_TABLE[0][1]->MODE = IOCON_PULL_NONE; break; }
	case 3: { IOCON_PIN_TABLE[0][14]->MODE = IOCON_PULL_NONE; break; }
	case 4: { IOCON_PIN_TABLE[0][23]->MODE = IOCON_PULL_NONE; break; }
	case 5: { IOCON_PIN_TABLE[0][30]->MODE = IOCON_PULL_NONE; break; }
	}
}

/**
 * @brief Inhabilitar pull-up/pull-down en entrada analógica de tensión de referencia externa
 * de la voltage ladder del comparador analógico.
 */
void IOCON_disable_pullup_acmp_ladder_external_ref_VDDCMP(void)
{
	IOCON_PIN_TABLE[0][6]->MODE = IOCON_PULL_NONE;
}

/**
 * @brief Habilitar DAC0 en PIO0_17
 */
void IOCON_enable_dac0(void)
{
	IOCON_PIN_TABLE[0][17]->MODE = IOCON_PULL_NONE;
	IOCON_PIN_TABLE[0][17]->DACMODE = 1;
}

/**
 * @brief Habilitar DAC1 en PIO0_29
 */
void IOCON_enable_dac1(void)
{
	IOCON_PIN_TABLE[0][29]->MODE = IOCON_PULL_NONE;
	IOCON_PIN_TABLE[0][29]->DACMODE = 1;
}

/**
 * @brief Inhabilitar DAC0 en PIO0_17
 */
void IOCON_disable_dac0(void)
{
	IOCON_PIN_TABLE[0][17]->DACMODE = 0;
}

/**
 * @brief Inhabilitar DAC1 en PIO0_29
 */
void IOCON_disable_dac1(void)
{
	IOCON_PIN_TABLE[0][29]->DACMODE = 0;
}

/**
 * @brief Habilitar I2C0_SCL en PIO0_10
 * @param[in] iic_mode Modo de IIC
 */
void IOCON_select_i2c0_scl(IOCON_iic_mode_en iic_mode)
{
	IOCON_PIN_TABLE[0][10]->I2CMODE = iic_mode;
}

/**
 * @brief Habilitar I2C SDA en PIO0_11
 * @param[in] iic_mode Modo de IIC
 */
void IOCON_select_i2c0_sda(IOCON_iic_mode_en iic_mode)
{
	IOCON_PIN_TABLE[0][11]->I2CMODE = iic_mode;
}
