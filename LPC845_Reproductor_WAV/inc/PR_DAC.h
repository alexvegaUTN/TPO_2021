/*******************************************************************************************************************************//**
 *
 * @file		PR_DAC.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_DAC_H_
#define PR_DAC_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/** Enumeraciones de instancias disponibles de *DAC* */
typedef enum
{
	kDAC_SEL_0 = 0, 	/**< Instancia 0 */
	kDAC_SEL_1, 		/**< Instancia 1 */
	kDAC_SEL_AMOUNT 	/**< Cantidad de instancias */
}dac_sel_en;

/** Tiempos de establecimiento disponibles para el *DAC* */
typedef enum
{
	kDAC_SETTLING_TIME_1US_MAX = 0, /**< Tiempo de establecimiento de 1 microsegundo máximo */
	kDAC_SETTLING_TIME_2_5US_MAX /**< Tiempo de establecimiento de 2.5 microsegundos máximo */
}dac_settling_time_en;

/** Estructura de configuración del *DAC* */
typedef struct
{
	uint8_t count_enable : 1; /**< Habilitación del contador (para modo DMA) */
	uint8_t double_buffering : 1; /**< Doble buffer (para modo DMA) */
	uint8_t dma_enable : 1; /**< Habilitacion de funcionamiento con el DMA */
	uint8_t dma_request : 1; /**< Pedido de DMA */
}dac_ctrl_config_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
/**
 * @brief Inicialización del DAC
 *
 * Esta función inicializa el @ref IOCON de la forma necesaria para que el pin quede configurado correctamente.
 *
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversión del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void dac_init(dac_sel_en dac, dac_settling_time_en settling_time, uint32_t initial_value);

/**
 * @brief Actualización del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 * @pre Haber inicializado el periférico
 */
void dac_update_value(dac_sel_en dac, uint16_t new_value);

/**
 * @brief Configuración del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuración deseada
 * @pre Haber inicializado el periférico
 */
void dac_config_ctrl(dac_sel_en dac, dac_ctrl_config_t * config);


#endif /* PR_DAC_H_ */
