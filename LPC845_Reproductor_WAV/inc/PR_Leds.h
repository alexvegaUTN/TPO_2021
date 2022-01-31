/*******************************************************************************************************************************//**
 *
 * @file		DR_Leds.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Nov 26, 2021
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_LEDS_H_
#define PR_LEDS_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include "DR_GPIO.h"
#include "PR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/


#define     LED_VERDE_PORT_PIN 	kGPIO_PORTPIN_1_0 //PIO1_00
#define     LED_AZUL_PORT_PIN  	kGPIO_PORTPIN_1_1 //PIO1_01
#define     LED_ROJO_PORT_PIN 	kGPIO_PORTPIN_1_2 //PIO1_02

#define     LED_1_PORT_PIN    	kGPIO_PORTPIN_0_1 //PIO1_02

/*! @name PIO1_0 (number 11), LED_VERDE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_VERDE_PORT 1U                  /*!<@brief PORT device index: 1 */
#define BOARD_INITPINS_LED_VERDE_PIN 0U                   /*!<@brief PORT pin number */
                                                          /* @} */
/*! @name PIO1_1 (number 14), LED_AZUL
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_AZUL_PORT 1U                  /*!<@brief PORT device index: 1 */
#define BOARD_INITPINS_LED_AZUL_PIN 1U                   /*!<@brief PORT pin number */
                                                         /* @} */
/*! @name PIO1_2 (number 16), LED_ROJO
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_ROJO_PORT 1U                  /*!<@brief PORT device index: 1 */
#define BOARD_INITPINS_LED_ROJO_PIN 2U                   /*!<@brief PORT pin number */
                                                         /* @} */
/*! @name PIO0_1 (number 24), LED_1
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_1_PORT 0U                  /*!<@brief PORT device index: 0 */
#define BOARD_INITPINS_LED_1_PIN 1U                   /*!<@brief PORT pin number */
                                                      /* @} */

#define LED_VERDE_ON		gpio_set_pin_active_low(LED_VERDE_PORT_PIN)
#define LED_VERDE_OFF		gpio_clear_pin_active_low(LED_VERDE_PORT_PIN)
#define LED_VERDE_TOGGLE	gpio_toggle_pin(LED_VERDE_PORT_PIN)

#define LED_AZUL_ON			gpio_set_pin_active_low(LED_AZUL_PORT_PIN)
#define LED_AZUL_OFF		gpio_clear_pin_active_low(LED_AZUL_PORT_PIN)
#define LED_AZUL_TOGGLE		gpio_toggle_pin(LED_AZUL_PORT_PIN)

#define LED_ROJO_ON			gpio_set_pin_active_low(LED_ROJO_PORT_PIN)
#define LED_ROJO_OFF		gpio_clear_pin_active_low(LED_ROJO_PORT_PIN)
#define LED_ROJO_TOGGLE		gpio_toggle_pin(LED_ROJO_PORT_PIN)

#define LED1_ON				gpio_set_pin(LED_1_PORT_PIN)
#define LED1_OFF			gpio_clear_pin(LED_1_PORT_PIN)
#define LED1_TOGGLE			gpio_toggle_pin(LED_1_PORT_PIN)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef enum {
	Stick_Led_ON = 0,
	Stick_Led_OFF = 1
}stick_led_logic_e;

typedef enum {
	Board_Led_OFF = 0,
	Board_Led_ON = 1
}board_led_logic_e;

typedef enum {
	OFF,
	ON
} On_Off_t;

typedef enum {
	INPUT,
	OUTPUT
} Direccion_t;

typedef enum {
	LED_ON,
	LED_OFF
} Leds_On_Off_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void leds_init(void);

void led_verde_on(void);
void led_verde_off(void);
void led_verde_toggle(void);

void led_azul_on(void);
void led_azul_off(void);
void led_azul_toggle(void);

void led_rojo_on(void);
void led_rojo_off(void);
void led_rojo_toggle(void);

#endif /* PR_LEDS_H_ */
