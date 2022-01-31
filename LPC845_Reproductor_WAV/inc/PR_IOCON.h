/*******************************************************************************************************************************//**
 *
 * @file		PR_IOCON.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 4, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_IOCON_H_
#define PR_IOCON_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_IOCON.h"
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
/** Selección de resistor interno en el pin */
typedef enum
{
	kIOCON_PULL_NONE = 0, 	/**< Ningun resistor interno */
	kIOCON_PULL_DOWN, 		/**< Resistor interno conectado a Vss */
	kIOCON_PULL_UP, 		/**< Resistor interno conectado a Vdd */
	kIOCON_PULL_REPEATER 	/**< Modo repetidor */
}iocon_pull_mode_en;

/** Selección de Histeresis del pin */
typedef enum
{
	kIOCON_HYST_DISABLE = 0, 	/**< Histeresis deshabilitada */
	kIOCON_HYST_ENABLE, 		/**< Histeresis habilitada */
}iocon_hysteresis_en;

/** Selección de nivle de actividad entrada  */
typedef enum
{
	kIOCON_INPUT_NOT_INVERTED = 0, 		/**< Entrada Activo ALTO */
	kIOCON_INPUT_INVERTED, 				/**< Entrada Activo BAJO */
}iocon_invert_input_en;

/** Selección de salida */
typedef enum
{
	kIOCON_OP_DRAIN_DISABLED= 0, 		/**< Salida Push Pull */
	kIOCON_OP_DRAIN_ENABLED, 			/**< Salida Open Drain  */
}iocon_open_drain_en;

/** Modo de muestreo del filtro de glitches */
typedef enum
{
	kIOCON_SAMPLE_MODE_BYPASS = 0, 	/**< Sin filtro de glitches */
	kIOCON_SAMPLE_MODE_1_CLOCK, 	/**< Por lo menos un ciclo de clock para filtrar el glitch */
	kIOCON_SAMPLE_MODE_2_CLOCK, 	/**< Por lo menos dos ciclos de clock para filtrar el glitch */
	kIOCON_SAMPLE_MODE_3_CLOCK 		/**< Por lo menos tres ciclos de clock para filtrar el glitch */
}iocon_sample_mode_en;

/** Selección de banco de división de clock para el filtro de glitches */
typedef enum
{
	kIOCON_CLK_DIV_0 = 0, 	/**< Banco 0 */
	kIOCON_CLK_DIV_1, 		/**< Banco 1 */
	kIOCON_CLK_DIV_2, 		/**< Banco 2 */
	kIOCON_CLK_DIV_3, 		/**< Banco 3 */
	kIOCON_CLK_DIV_4, 		/**< Banco 4 */
	kIOCON_CLK_DIV_5, 		/**< Banco 5 */
	kIOCON_CLK_DIV_6 		/**< Banco 6 */
}iocon_clk_sel_en;

/** Modo de funcionamiento I2C */
typedef enum
{
	kIOCON_IIC_MODE_STANDARD = 0, 	/**< Modo IIC standard */
	kIOCON_IIC_MODE_GPIO, 			/**< Modo GPIO */
	kIOCON_IIC_MODE_FAST_MODE 		/**< Modo IIC Fast-Mode */
}iocon_i2c_mode_en;

/** Estructura de configuración de un pin mediante el *IOCON* */
typedef struct
{
	iocon_pull_mode_en pull_mode; 		/**< Resistor interno */
	iocon_hysteresis_en hysteresis; 	/**< Histéresis. Cualquier valor distinto de cero la activa */
	iocon_invert_input_en invert_input; /**< Inversión de lógica de lectura. Cualquier valor distinto de cero la activa */
	iocon_open_drain_en open_drain; 	/**< Modo *Open Drain*. Cualquier valor distinto de cero lo activa */
	iocon_sample_mode_en sample_mode; 	/**< Cantidad de muestras del filtro de glitches */
	iocon_clk_sel_en clk_sel; 			/**< Selección de clock para el filtro de glitches */
	iocon_i2c_mode_en i2c_mode; 		/**< Selección de modo I2C */
}iocon_config_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
extern volatile IOCON_per_t * const IOCON; 	//!< Periferico IOCON
extern volatile IOCON_PIO_reg_t dummy_reg; 								//!< Registro dummy para los pines no disponibles en el encapsulado

extern volatile IOCON_PIO_reg_t * const IOCON_PIN_TABLE[2][32]; 			//!< Tabla de registros de configuracion

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Configuracion de un pin
 *
 * Nótese que las configuraciones de modo analógico no están en la estructura de configuración
 * @ref hal_iocon_config_t. Esto es debido a que dichas configuraciones se realizarán en el periférico
 * correspondiente, sea el @ref ADC o @ref DAC.
 *
 * @param[in] portpin Puerto/pin a configurar
 * @param[in] config Puntero a estructura de configuracion del pin
 * @pre Configuración de divisores de clock de bancos de filtro de glitches en caso de ser necesario.
 */
void iocon_config_io(gpio_portpin_en portpin, const iocon_config_t *config);

#endif /* PR_IOCON_H_ */
