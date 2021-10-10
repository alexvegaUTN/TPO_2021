/*******************************************************************************************************************************//**
 *
 * @file		DR_GPIO.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		30 sep. 2021
 * @author		ALEXIS VEGA
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_GPIO_H_
#define DR_GPIO_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

#define 	BAJO		0
#define 	ALTO		1

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

typedef signed char 	int8_t;
typedef unsigned char 	uint8_t;
typedef short 			int16_t;
typedef unsigned short 	uint16_t;
typedef int 			int32_t;
typedef unsigned int 	uint32_t;

#define  __I    volatile const  /*Defines 'read only' permission */
#define  __O    volatile             /*Defines 'write only' permission */
#define  __IO   volatile             /*Defines 'read / write' permission */

typedef struct {
  __IO uint8_t B[2][32];
       uint8_t RESERVED_0[4032];
  __IO uint32_t W[2][32];
       uint8_t RESERVED_1[3840];
  __IO uint32_t DIR[2];
       uint8_t RESERVED_2[120];
  __IO uint32_t MASK[2];
       uint8_t RESERVED_3[120];
  __IO uint32_t PIN[2];
       uint8_t RESERVED_4[120];
  __IO uint32_t MPIN[2];
       uint8_t RESERVED_5[120];
  __IO uint32_t SET[2];
       uint8_t RESERVED_6[120];
  __O  uint32_t CLR[2];
       uint8_t RESERVED_7[120];
  __O  uint32_t NOT[2];
       uint8_t RESERVED_8[120];
  __O  uint32_t DIRSET[2];
       uint8_t RESERVED_9[120];
  __O  uint32_t DIRCLR[2];
       uint8_t RESERVED_10[120];
  __O  uint32_t DIRNOT[2];
} GPIO_Type;

#define	 GPIO  (  (GPIO_Type* )  0xA0000000UL)

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

int8_t GPIO_SetPin_Version1 ( uint8_t port , uint8_t bit , uint8_t estado );
int8_t GPIO_SetPin_Version2 ( uint8_t port , uint8_t bit , uint8_t estado );
int8_t GPIO_SetPin_Version3 ( uint8_t port , uint8_t bit , uint8_t estado );
int8_t GPIO_SetPin_Version4 ( uint8_t port , uint8_t bit , uint8_t estado );
int8_t GPIO_SetTogglePin ( uint8_t port , uint8_t bit );
int8_t GPIO_SetDir_Version1 ( uint8_t port , uint8_t bit , uint8_t dir );
int8_t GPIO_SetDir_Version2 ( uint8_t port , uint8_t bit , uint8_t dir );
int8_t GPIO_SetToggleDir ( uint8_t port , uint8_t bit );
int8_t GPIO_GetPin_Version1 ( uint8_t port , uint8_t bit , uint8_t actividad );
int8_t GPIO_GetPin_Version2 ( uint8_t port , uint8_t bit , uint8_t actividad );
int8_t GPIO_GetPin_Version3 ( uint8_t port , uint8_t bit , uint8_t actividad );

#endif /* DR_GPIO_H_ */
