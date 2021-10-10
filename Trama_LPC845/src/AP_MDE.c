/**
*	\file       AP_MDE.c
*	\brief      Maquina/s de Estado
*	\details    Descripcion detallada del archivo
*	\author     AlexisVega
*	\date       15-09-2021 16:00:53
*/
/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/
#include "AP_MDE.h"
#include "AP_FuncionesMDE.h"

/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/
#define		RESET		0	
//Maquina: Trama
#define		RECIBIR		1	// < Maquina: Trama >
#define		VERIFICAR_TRAMA		2	// < Maquina: Trama >
#define		ACCION		3	// < Maquina: Trama >

/************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 ************************************************************************************************************/
#define     m_Apagar_LED()


/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/
static int Trama ( int );

/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/
int RX_Valido ;		//
int OK ;		//

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

/**
*	\fn      static int Trama ( int Estado )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  AlexisVega
*	\date    15-09-2021 16:00:53
*   \param   [in] Estado: caso dentro de la maquina (estado actual)
*	\return  int : estado siguiente
*/

static int Trama ( int  Estado )
{
    switch ( Estado )
    {

        case RESET :
            m_Apagar_LED();

            Estado = RECIBIR;

            break;

        case RECIBIR :
            if ( s_RX_Uart() )
            {

                Estado = VERIFICAR_TRAMA;
            }

            break;

        case VERIFICAR_TRAMA :
            if ( RX_Valido == 1 )
            {
                f_Comando_Led();
                OK = 1;

                Estado = ACCION;
            }

            if ( RX_Valido == -1 )
            {

                Estado = RECIBIR;
            }

            break;

        case ACCION :
            if ( OK == 1 )
            {

                Estado = RECIBIR;
            }

            break;

        default:
            Estado = RESET ;
            break;

    }
    return Estado ;
}

/************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void MaquinaDeEstados ( void )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  AlexisVega
*	\date    15-09-2021 16:00:53
*   \param   void
*	\return  void
*/
void MaquinaDeEstados ( void )
{
    static int estados_Trama = RESET;

    // Coloque su codigo aqui

    estados_Trama = Trama( estados_Trama );


    return ;
}

