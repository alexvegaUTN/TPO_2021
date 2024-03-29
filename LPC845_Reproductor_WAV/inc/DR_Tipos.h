/*******************************************************************************************************************************//**
 *
 * @file		Tipos.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Nov 13, 2020
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_TIPOS_H_
#define DR_TIPOS_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/


/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define     __R             volatile const     // !< Modificador para solo lectura
#define 	__W     		volatile 	       // !<  Modificador para solo escritura
#define 	__RW            volatile

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/
#define 	NULL 			((void *)0)			// NULL pointer

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
typedef	unsigned int        uint32_t;
typedef	short unsigned int	uint16_t;
typedef	unsigned char      	uint8_t ;
typedef	int                 int32_t;
typedef	short int           int16_t;
typedef	signed char          int8_t;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/


#endif /* DR_TIPOS_H_ */
