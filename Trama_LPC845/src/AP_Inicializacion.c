/**
*	\file       AP_Inicializacion.c
*	\brief      Funciones de inicializacion de todo el proyecto
*	\details    Descripcion detallada del archivo
*	\author     AlexisVega
*	\date       15-09-2021 16:00:53
*/

/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/
#include "AP_Inicializacion.h"
#include "AP_MDE.h"

/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/

/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/
static void Inicializar_Trama( void ) ;

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void Inicializar_Trama( void )
*	\brief   Inicializacion de las diferentes maquinas de estado
*	\details Amplie la descripcion
*	\author  AlexisVega
*	\date    15-09-2021 16:00:53
*   \param   void
*	\return  void
*/
static void Inicializar_Trama( void )
{
    //!< Coloque aqui su codigo
	RX_Valido = 0 ;
	OK = 0 ;

    return ;
}

/************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void Inicializar( void )
*	\brief   Inicializacion de las diferentes maquinas de estado
*	\details Amplie la descripcion
*	\author  AlexisVega
*	\date    15-09-2021 16:00:53
*   \param   void
*	\return  void
*/
void Inicializar( void )
{
    //Coloque aqui su codigo

	Inicializar_Trama( );

	
    return ;
}

