/*******************************************************************************************************************************//**
 *
 * @file		DR_SPI.c
 * @brief		Descripcion del modulo
 * @date		Jan 7, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "DR_SPI.h"

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
volatile SPI_per_t * const SPI[] = { //!< Perifericos SPI
		(SPI_per_t *) SPI0_BASE,
		(SPI_per_t *) SPI1_BASE
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
 	\date Jan 7, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/**
 * @brief Habilitar modulo
 * @param[in] inst Instancia a habilitar
 */
void SPI_enable(uint8_t inst)
{
	SPI[inst]->CFG.ENABLE = 1;
}

/**
 * @brief Inhabilitar modulo
 * @param[in] inst Instancia a inhabilitar
 */
void SPI_disable(uint8_t inst)
{
	SPI[inst]->CFG.ENABLE = 0;
}

/**
 * @brief Configurar como modo master
 * @param[in] inst Instancia a configurar
 */
void SPI_set_master_mode(uint8_t inst)
{
	SPI[inst]->CFG.MASTER = 1;
}

/**
 * @brief Configurar como modo slave
 * @param[in] inst Instancia a configurar
 */
void SPI_set_slave_mode(uint8_t inst)
{
	SPI[inst]->CFG.MASTER = 0;
}

/**
 * @brief Configurar orden de datos MSB primero
 * @param[in] inst Instancia a configurar
 */
void SPI_set_data_order_msb_first(uint8_t inst)
{
	SPI[inst]->CFG.LSBF = 0;
}

/**
 * @brief Configurar orden de datos LSB primero
 * @param[in] inst Instancia a configurar
 */
void SPI_set_data_order_lsb_first(uint8_t inst)
{
	SPI[inst]->CFG.LSBF = 1;
}

/**
 * @brief Configurar fase del clock (modo change)
 * @param[in] inst Instancia a configurar
 */
void SPI_set_cpha_change(uint8_t inst)
{
	SPI[inst]->CFG.CPHA = 0;
}

/**
 * @brief Configurar fase del clock (modo capture)
 * @param[in] inst Instancia a configurar
 */
void SPI_set_cpha_capture(uint8_t inst)
{
	SPI[inst]->CFG.CPHA = 1;
}

/**
 * @brief Configurar polaridad del clock (polaridad baja)
 * @param[in] inst Instancia a configurar
 */
void SPI_set_cpol_low(uint8_t inst)
{
	SPI[inst]->CFG.CPOL = 0;
}

/**
 * @brief Configurar polaridad del clock (polaridad alta)
 * @param[in] inst Instancia a configurar
 */
void SPI_set_cpol_high(uint8_t inst)
{
	SPI[inst]->CFG.CPOL = 1;
}

void SPI_set_opMode_0(uint8_t inst)
{
	SPI_set_cpha_change(inst);
	SPI_set_cpol_low(inst);
}

void SPI_set_opMode_1(uint8_t inst)
{
	SPI_set_cpha_capture(inst);
	SPI_set_cpol_low(inst);
}

void SPI_set_opMode_2(uint8_t inst)
{
	SPI_set_cpha_change(inst);
	SPI_set_cpol_high(inst);
}

void SPI_set_opMode_3(uint8_t inst)
{
	SPI_set_cpha_capture(inst);
	SPI_set_cpol_high(inst);
}

/**
 * @brief Habilitar modo loopback
 * @param[in] inst Instancia a configurar
 */
void SPI_enable_loopback_mode(uint8_t inst)
{
	SPI[inst]->CFG.LOOP = 1;
}

/**
 * @brief Inhabilitar modo loopback
 * @param[in] inst Instancia a configurar
 */
void SPI_disable_loopback_mode(uint8_t inst)
{
	SPI[inst]->CFG.LOOP = 0;
}

/**
 * @brief Fijar polaridad de slave select como activa baja
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal de SSEL a configurar
 */
void SPI_set_ssel_active_low(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->CFG.SPOL0 = 0; break; }
	case 1: { SPI[inst]->CFG.SPOL1 = 0; break; }
	case 2: { SPI[inst]->CFG.SPOL2 = 0; break; }
	case 3: { SPI[inst]->CFG.SPOL3 = 0; break; }
	}
}

/**
 * @brief Fijar polaridad de slave select como activa alta
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal de SSEL a configurar
 */
void SPI_set_ssel_active_high(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->CFG.SPOL0 = 1; break; }
	case 1: { SPI[inst]->CFG.SPOL1 = 1; break; }
	case 2: { SPI[inst]->CFG.SPOL2 = 1; break; }
	case 3: { SPI[inst]->CFG.SPOL3 = 1; break; }
	}
}

/**
 * @brief Configurar ciclos de clock entre la activacion de SSEL y la transmision de datos
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
void SPI_set_pre_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.PRE_DELAY = delay;
}

/**
 * @brief Configurar ciclos de clock entre la finalizacion de transmision y desactivacion de SSEL
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
void SPI_set_post_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.POST_DELAY = delay;
}

/**
 * @brief Configurar ciclos de clock entre transmisiones sin desactivar SSEL
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
void SPI_set_frame_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.FRAME_DELAY = delay;
}

/**
 * @brief Configurar ciclos de clock entre desactivacion/activacion de SSEL
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
void SPI_set_transfer_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.TRANSFER_DELAY = delay;
}

/**
 * @brief Leer un flag de status
 * @param[in] inst Instancia a consultar
 * @param[in] flag Flag a consultar
 * @return Estado del flag actual
 */
uint8_t SPI_get_status_flag(uint8_t inst, SPI_status_flag_en flag)
{
	switch(flag)
	{
	case SPI_STATUS_FLAG_RXRDY: 		{ return SPI[inst]->STAT.RXRDY;	 		break; }
	case SPI_STATUS_FLAG_TXRDY: 		{ return SPI[inst]->STAT.TXRDY; 		break; }
	case SPI_STATUS_FLAG_RXOV: 			{ return SPI[inst]->STAT.RXOV; 			break; }
	case SPI_STATUS_FLAG_TXUR: 			{ return SPI[inst]->STAT.TXUR; 			break; }
	case SPI_STATUS_FLAG_SSA: 			{ return SPI[inst]->STAT.SSA; 			break; }
	case SPI_STATUS_FLAG_SSD: 			{ return SPI[inst]->STAT.SSD; 			break; }
	case SPI_STATUS_FLAG_STALLED: 		{ return SPI[inst]->STAT.STALLED; 		break; }
	case SPI_STATUS_FLAG_ENDTRANSFER: 	{ return SPI[inst]->STAT.ENDTRANSFER; 	break; }
	case SPI_STATUS_FLAG_MSTIDLE: 		{ return SPI[inst]->STAT.MSTIDLE; 		break; }
	}

	return 0;
}

/**
 * @brief Limpiar un flag de status
 * @param[in] inst Instancia a limpiar
 * @param[in] flag Flag a limpiar
 */
uint8_t SPI_clear_status_flag(uint8_t inst, SPI_status_flag_en flag)
{
	switch(flag)
	{
	case SPI_STATUS_FLAG_RXOV: 			{ SPI[inst]->STAT.RXOV = 1; 		break; }
	case SPI_STATUS_FLAG_TXUR: 			{ SPI[inst]->STAT.TXUR = 1; 		break; }
	case SPI_STATUS_FLAG_SSA: 			{ SPI[inst]->STAT.SSA = 1; 			break; }
	case SPI_STATUS_FLAG_SSD: 			{ SPI[inst]->STAT.SSD = 1; 			break; }
	case SPI_STATUS_FLAG_ENDTRANSFER: 	{ SPI[inst]->STAT.ENDTRANSFER = 1; 	break; }
	case SPI_STATUS_FLAG_MSTIDLE: 		break;
	case SPI_STATUS_FLAG_RXRDY: 		break;
	case SPI_STATUS_FLAG_STALLED: 		break;
	case SPI_STATUS_FLAG_TXRDY: 		break;
	}

	return 0;
}

/**
 * @brief Habilitar interrupcion
 * @param[in] inst Instancia a configurar
 * @param[in] irq Interrupcion a habilitar
 */
void SPI_enable_irq_RXRDY(uint8_t inst)
{
	SPI[inst]->INTENSET.RXRDYEN = 1;
}

void SPI_enable_irq_TXRDY(uint8_t inst)
{
	SPI[inst]->INTENSET.TXRDYEN = 1;
}

void SPI_enable_irq_RXOV(uint8_t inst)
{
	SPI[inst]->INTENSET.RXOVEN = 1;
}

void SPI_enable_irq_TXUR(uint8_t inst)
{
	SPI[inst]->INTENSET.TXUREN = 1;
}

void SPI_enable_irq_SSA(uint8_t inst)
{
	SPI[inst]->INTENSET.SSAEN = 1;
}

void SPI_enable_irq_SSD(uint8_t inst)
{
	SPI[inst]->INTENSET.SSDEN = 1;
}

/**
 * @brief Inhabilitar interrupcion
 * @param[in] inst Instancia a configurar
 * @param[in] irq Interrupcion a inhabilitar
 */
void SPI_disable_irq_RXRDY(uint8_t inst)
{
	SPI[inst]->INTENCLR.RXRDYEN = 1;
}

void SPI_disable_irq_TXRDY(uint8_t inst)
{
	SPI[inst]->INTENCLR.TXRDYEN = 1;
}

void SPI_disable_irq_RXOV(uint8_t inst)
{
	SPI[inst]->INTENCLR.RXOVEN = 1;
}

void SPI_disable_irq_TXUR(uint8_t inst)
{
	SPI[inst]->INTENCLR.TXUREN = 1;
}

void SPI_disable_irq_SSA(uint8_t inst)
{
	SPI[inst]->INTENCLR.SSAEN = 1;
}

void SPI_disable_irq_SSD(uint8_t inst)
{
	SPI[inst]->INTENCLR.SSDEN = 1;
}

/**
 * @brief Leer resultado de la recepcion
 * @param[in] inst Instancia a consultar
 */
uint16_t SPI_read_rx_data(uint8_t inst)
{
	return SPI[inst]->RXDAT.RXDAT;
}

/**
 * @brief Obtener slave select activo
 * @param[in] inst Instancia a consultar
 */
uint8_t SPI_get_active_ssl(uint8_t inst)
{
	uint8_t ret = 0xFF;

	if(SPI[inst]->RXDAT.RXSSEL0_N == 0)
		ret = 0;

	else if(SPI[inst]->RXDAT.RXSSEL1_N == 0)
		ret = 1;

	else if(SPI[inst]->RXDAT.RXSSEL2_N == 0)
		ret = 2;

	else if(SPI[inst]->RXDAT.RXSSEL3_N == 0)
		ret = 3;

	return ret;
}

/**
 * @brief Obtener estado del flag de start of transfer
 * @param[in] inst Instancia a consultar
 */
uint8_t SPI_get_sot_flag(uint8_t inst)
{
	return SPI[inst]->RXDAT.SOT;
}

/**
 * @brief Escribir registro de datos de transmision
 * @param[in] inst Instancia a escribir
 * @param[in] data Dato a escribir
 */
void SPI_write_txdata(uint8_t inst, uint16_t data)
{
	SPI[inst]->TXDAT.DATA = data;
}

/**
 * @brief Habilitar slave select para la proxima transmision
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal a utilizar en la proxima transmision
 */
void SPI_select_slave(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->TXCTL.TXSSEL0_N = 0; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 1: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 0; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 2: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 0; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 3: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 0; break; }
	default:{ SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	}
}

/**
 * @brief Deshabilitar slave select para la proxima transmision
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal a utilizar en la proxima transmision
 */
void SPI_deselect_slave(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 1: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 2: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 3: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	default:{ SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	}
}

/**
 * @brief Indicar fin de transmision en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
void SPI_set_end_of_transmission(uint8_t inst)
{
	SPI[inst]->TXCTL.EOT = 1;
}

/**
 * @brief Limpiar fin de transmision en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
void SPI_clear_end_of_transmission(uint8_t inst)
{
	SPI[inst]->TXCTL.EOT = 0;
}

/**
 * @brief Indicar fin de trama en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
void SPI_set_end_of_frame(uint8_t inst)
{
	SPI[inst]->TXCTL.EOf = 1;
}

/**
 * @brief Limpiar fin de trama en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
void SPI_clear_end_of_frame(uint8_t inst)
{
	SPI[inst]->TXCTL.EOf = 0;
}

/**
 * @brief Ignorar recepcion en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
void SPI_set_rx_ignore(uint8_t inst)
{
	SPI[inst]->TXCTL.RXIGNORE = 1;
}

/**
 * @brief No ignorar recepcino en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
void SPI_clear_rx_ignore(uint8_t inst)
{
	SPI[inst]->TXCTL.RXIGNORE = 0;
}

/**
 * @brief Configurar largo de bits de palabra
 * @param[in] inst Instancia a configurar
 * @param[in] data_length Largo de palabra deseado
 */
void SPI_set_data_length(uint8_t inst, SPI_data_length_en data_length)
{
	SPI[inst]->TXCTL.LEN = data_length;
}

/**
 * @brief Escribir data a transmitir y control al mismo tiempo (en una unica escritura)
 * @param[in] inst Instancia a utilizar
 * @param[in] data_and_control Dato a transmitir y control
 */
void SPI_set_data_and_control(uint8_t inst, SPI_TXDATCTL_reg_t *data_and_control)
{
	SPI[inst]->TXDATCTL = *data_and_control;
}

/**
 * @brief Configurar divisor de clock
 * @param[in] inst Instancia a configurar
 * @param[in] div Divisor deseado (el valor efectivo es este valor +1)
 */
void SPI_set_clock_div(uint8_t inst, uint16_t div)
{
	SPI[inst]->DIV.DIVVAL = div;
}

/**
 * @brief Leer flag de interrupcion actual
 * @param[in] inst Instancia a consultar
 * @param[in] irq Flag de interrupcion a consultar
 */
uint8_t SPI_get_irq_flag_status(uint8_t inst, SPI_irq_sel_en irq)
{
	switch(irq)
	{
	case SPI_IRQ_RXRDY: { return SPI[inst]->INTSTAT.RXRDY; 	break; }
	case SPI_IRQ_TXRDY: { return SPI[inst]->INTSTAT.TXRDY; 	break; }
	case SPI_IRQ_RXOV: 	{ return SPI[inst]->INTSTAT.RXOV; 	break; }
	case SPI_IRQ_TXUR: 	{ return SPI[inst]->INTSTAT.TXUR; 	break; }
	case SPI_IRQ_SSA: 	{ return SPI[inst]->INTSTAT.SSA; 	break; }
	case SPI_IRQ_SSD: 	{ return SPI[inst]->INTSTAT.SSD; 	break; }
	}

	return 0;
}


