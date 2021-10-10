/*******************************************************************************************************************************//**
 *
 * @file		DR_NVIC.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		6 oct. 2021
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_NVIC_H_
#define DR_NVIC_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

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

#define NVIC_BASE	( ( __IO uint32_t*) 0xE000E000UL)

#define	ISER0		(*( __IO uint32_t*) 0xE000E100UL)
#define	ICER0		(*( __IO uint32_t*) 0xE000E180UL)
#define	ISPR0		(*( __IO uint32_t*) 0xE000E200UL)
#define	ICPR0		(*( __IO uint32_t*) 0xE000E280UL)
#define	ICPR0		(*( __IO uint32_t*) 0xE000E280UL)
#define	IPR0		(*( __IO uint32_t*) 0xE000E400UL)
#define	IPR1		(*( __IO uint32_t*) 0xE000E404UL)
#define	IPR2		(*( __IO uint32_t*) 0xE000E408UL)
#define	IPR3		(*( __IO uint32_t*) 0xE000E40CUL)
#define	IPR4		(*( __IO uint32_t*) 0xE000E410UL)
#define	IPR5		(*( __IO uint32_t*) 0xE000E414UL)
#define	IPR6		(*( __IO uint32_t*) 0xE000E418UL)
#define	IPR7		(*( __IO uint32_t*) 0xE000E41CUL)

#define ISE_SPI0	(0)

//
//typedef struct {
//
//	__IO uint32_t ISER0[255];
//		 uint8_t RESERVED_1[123];
//	__IO uint32_t ICER0[];
//		 uint8_t RESERVED_2[];
//	__IO uint32_t ISPR0[];
//	 	 uint8_t RESERVED_3[];
//	__IO uint32_t ICPR0[123];
//		 uint8_t RESERVED_4[];
//	__IO uint32_t IPR[8];
//}NVIC_Type;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

#endif /* DR_NVIC_H_ */