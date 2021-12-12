/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    board.h
 * @brief   Board initialization header file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_gpio.h"
#include "pin_mux.h"

#define LED_VERDE_ON		GPIO_PinWrite(BOARD_INITPINS_LED_VERDE_GPIO, BOARD_INITPINS_LED_VERDE_PORT, BOARD_INITPINS_LED_VERDE_PIN, LOGIC_LED_ON)
#define LED_VERDE_OFF		GPIO_PinWrite(BOARD_INITPINS_LED_VERDE_GPIO, BOARD_INITPINS_LED_VERDE_PORT, BOARD_INITPINS_LED_VERDE_PIN, LOGIC_LED_OFF)
#define LED_VERDE_TOGGLE	GPIO_PortToggle(BOARD_INITPINS_LED_VERDE_GPIO, BOARD_INITPINS_LED_VERDE_PORT, BOARD_INITPINS_LED_VERDE_PIN_MASK)

#define LED_AZUL_ON			GPIO_PinWrite(BOARD_INITPINS_LED_AZUL_GPIO, BOARD_INITPINS_LED_AZUL_PORT, BOARD_INITPINS_LED_AZUL_PIN, LOGIC_LED_ON)
#define LED_AZUL_OFF		GPIO_PinWrite(BOARD_INITPINS_LED_AZUL_GPIO, BOARD_INITPINS_LED_AZUL_PORT, BOARD_INITPINS_LED_AZUL_PIN, LOGIC_LED_OFF)
#define LED_AZUL_TOGGLE		GPIO_PortToggle(BOARD_INITPINS_LED_AZUL_GPIO, BOARD_INITPINS_LED_AZUL_PORT, BOARD_INITPINS_LED_AZUL_PIN_MASK)

#define LED_ROJO_ON			GPIO_PinWrite(BOARD_INITPINS_LED_ROJO_GPIO, BOARD_INITPINS_LED_ROJO_PORT, BOARD_INITPINS_LED_ROJO_PIN, LOGIC_LED_ON)
#define LED_ROJO_OFF		GPIO_PinWrite(BOARD_INITPINS_LED_ROJO_GPIO, BOARD_INITPINS_LED_ROJO_PORT, BOARD_INITPINS_LED_ROJO_PIN, LOGIC_LED_OFF)
#define LED_ROJO_TOGGLE		GPIO_PortToggle(BOARD_INITPINS_LED_ROJO_GPIO, BOARD_INITPINS_LED_ROJO_PORT, BOARD_INITPINS_LED_ROJO_PIN_MASK)


/**
 * @brief	The board name 
 */
#define BOARD_NAME "board"

/**
 * @brief	The flash size
 */
#define BOARD_FLASH_SIZE (0x10000U)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief 	Initialize board specific settings.
 */
void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @brief Board led mapping */
#define LOGIC_LED_ON  0U
#define LOGIC_LED_OFF 1U

void LED_Verde_ON(void);
void LED_Verde_OFF(void);
void LED_Verde_Toggle(void);
void LED_Azul_ON(void);
void LED_Azul_OFF(void);
void LED_Azul_Toggle(void);
void LED_Rojo_ON(void);
void LED_Rojo_OFF(void);
void LED_rojo_Toggle(void);

/*! @brief State led */
#define STATE_LED_ON  1U
#define STATE_LED_OFF 0U


#endif /* _BOARD_H_ */


