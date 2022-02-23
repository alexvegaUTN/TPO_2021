/*******************************************************************************************************************************//**
 *
 * @file		DR_SPI.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 7, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_SPI_H_
#define DR_SPI_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define	SPI0_BASE		0x40058000
#define	SPI1_BASE		0x4005C000

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Enumeraciones
 **********************************************************************************************************************************/
typedef enum
{
	SPI_STATUS_FLAG_RXRDY = 0,
	SPI_STATUS_FLAG_TXRDY,
	SPI_STATUS_FLAG_RXOV,
	SPI_STATUS_FLAG_TXUR,
	SPI_STATUS_FLAG_SSA,
	SPI_STATUS_FLAG_SSD,
	SPI_STATUS_FLAG_STALLED,
	SPI_STATUS_FLAG_ENDTRANSFER,
	SPI_STATUS_FLAG_MSTIDLE,
}SPI_status_flag_en;

typedef enum
{
	SPI_IRQ_RXRDY = 0,
	SPI_IRQ_TXRDY,
	SPI_IRQ_RXOV,
	SPI_IRQ_TXUR,
	SPI_IRQ_SSA,
	SPI_IRQ_SSD
}SPI_irq_sel_en;

typedef enum
{
	SPI_DATA_LENGTH_1_BIT = 0,
	SPI_DATA_LENGTH_2_BIT,
	SPI_DATA_LENGTH_3_BIT,
	SPI_DATA_LENGTH_4_BIT,
	SPI_DATA_LENGTH_5_BIT,
	SPI_DATA_LENGTH_6_BIT,
	SPI_DATA_LENGTH_7_BIT,
	SPI_DATA_LENGTH_8_BIT,
	SPI_DATA_LENGTH_9_BIT,
	SPI_DATA_LENGTH_10_BIT,
	SPI_DATA_LENGTH_11_BIT,
	SPI_DATA_LENGTH_12_BIT,
	SPI_DATA_LENGTH_13_BIT,
	SPI_DATA_LENGTH_14_BIT,
	SPI_DATA_LENGTH_15_BIT,
	SPI_DATA_LENGTH_16_BIT
}SPI_data_length_en;



/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Estructuras
 **********************************************************************************************************************************/
typedef struct
{
	uint32_t ENABLE : 1;
	uint32_t : 1;
	uint32_t MASTER : 1;
	uint32_t LSBF : 1;
	uint32_t CPHA : 1;
	uint32_t CPOL : 1;
	uint32_t : 1;
	uint32_t LOOP : 1;
	uint32_t SPOL0 : 1;
	uint32_t SPOL1 : 1;
	uint32_t SPOL2 : 1;
	uint32_t SPOL3 : 1;
	uint32_t : 20;
}SPI_CFG_reg_t;

typedef struct
{
	uint32_t PRE_DELAY : 4;
	uint32_t POST_DELAY : 4;
	uint32_t FRAME_DELAY : 4;
	uint32_t TRANSFER_DELAY : 4;
	uint32_t : 16;
}SPI_DLY_reg_t;

typedef struct
{
	const uint32_t RXRDY : 1;
	const uint32_t TXRDY : 1;
	uint32_t RXOV : 1;
	uint32_t TXUR : 1;
	uint32_t SSA : 1;
	uint32_t SSD : 1;
	const uint32_t STALLED : 1;
	uint32_t ENDTRANSFER : 1;
	const uint32_t MSTIDLE : 1;
	uint32_t : 21;
}SPI_STAT_reg_t;

typedef struct
{
	uint32_t RXRDYEN : 1;
	uint32_t TXRDYEN : 1;
	uint32_t RXOVEN : 1;
	uint32_t TXUREN : 1;
	uint32_t SSAEN : 1;
	uint32_t SSDEN : 1;
	uint32_t : 26;
}SPI_INTENSET_reg_t;

typedef struct
{
	uint32_t RXRDYEN : 1;
	uint32_t TXRDYEN : 1;
	uint32_t RXOVEN : 1;
	uint32_t TXUREN : 1;
	uint32_t SSAEN : 1;
	uint32_t SSDEN : 1;
	uint32_t : 26;
}SPI_INTENCLR_reg_t;

typedef struct
{
	uint32_t RXDAT : 16;
	uint32_t RXSSEL0_N : 1;
	uint32_t RXSSEL1_N : 1;
	uint32_t RXSSEL2_N : 1;
	uint32_t RXSSEL3_N : 1;
	uint32_t SOT : 1;
	uint32_t : 11;
}SPI_RXDAT_reg_t;

typedef struct
{
	uint32_t TXDAT : 16;
	uint32_t TXSSEL0_N : 1;
	uint32_t TXSSEL1_N : 1;
	uint32_t TXSSEL2_N : 1;
	uint32_t TXSSEL3_N : 1;
	uint32_t EOT : 1;
	uint32_t EOf : 1; // El caracter en minuscula es a proposito
	uint32_t RXIGNORE : 1;
	uint32_t : 1;
	uint32_t LEN : 4;
	uint32_t : 4;
}SPI_TXDATCTL_reg_t;

typedef struct
{
	uint32_t DATA : 16;
	uint32_t : 16;
}SPI_TXDAT_reg_t;

typedef struct
{
	uint32_t : 16;
	uint32_t TXSSEL0_N : 1;
	uint32_t TXSSEL1_N : 1;
	uint32_t TXSSEL2_N : 1;
	uint32_t TXSSEL3_N : 1;
	uint32_t EOT : 1;
	uint32_t EOf : 1; // El caracter en minuscula es a proposito
	uint32_t RXIGNORE : 1;
	uint32_t : 1;
	uint32_t LEN : 4;
	uint32_t : 4;
}SPI_TXCTL_reg_t;

typedef struct
{
	uint32_t DIVVAL : 16;
	uint32_t : 16;
}SPI_DIV_reg_t;

typedef struct
{
	uint32_t RXRDY : 1;
	uint32_t TXRDY : 1;
	uint32_t RXOV : 1;
	uint32_t TXUR : 1;
	uint32_t SSA : 1;
	uint32_t SSD : 1;
	uint32_t : 26;
}SPI_INTSTAT_reg_t;

typedef struct
{
	__RW SPI_CFG_reg_t CFG;
	__RW SPI_DLY_reg_t DLY;
	__RW SPI_STAT_reg_t STAT;
	__RW SPI_INTENSET_reg_t INTENSET;
	__RW SPI_INTENCLR_reg_t INTENCLR;
	__R SPI_RXDAT_reg_t RXDAT;
	__RW SPI_TXDATCTL_reg_t TXDATCTL;
	__RW SPI_TXDAT_reg_t TXDAT;
	__RW SPI_TXCTL_reg_t TXCTL;
	__RW 	SPI_DIV_reg_t DIV;
	__R SPI_INTSTAT_reg_t INTSTAT;
}SPI_per_t;


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void SPI_enable(uint8_t inst);
void SPI_disable(uint8_t inst);
void SPI_set_master_mode(uint8_t inst);
void SPI_set_slave_mode(uint8_t inst);
void SPI_set_data_order_msb_first(uint8_t inst);
void SPI_set_data_order_lsb_first(uint8_t inst);
void SPI_set_cpha_change(uint8_t inst);
void SPI_set_cpha_capture(uint8_t inst);
void SPI_set_cpol_low(uint8_t inst);
void SPI_set_cpol_high(uint8_t inst);

void SPI_set_opMode_0(uint8_t inst);
void SPI_set_opMode_1(uint8_t inst);
void SPI_set_opMode_2(uint8_t inst);
void SPI_set_opMode_3(uint8_t inst);

void SPI_enable_loopback_mode(uint8_t inst);
void SPI_disable_loopback_mode(uint8_t inst);
void SPI_set_ssel_active_low(uint8_t inst, uint8_t channel);
void SPI_set_ssel_active_high(uint8_t inst, uint8_t channel);
void SPI_set_pre_delay(uint8_t inst, uint8_t delay);
void SPI_set_post_delay(uint8_t inst, uint8_t delay);
void SPI_set_frame_delay(uint8_t inst, uint8_t delay);
void SPI_set_transfer_delay(uint8_t inst, uint8_t delay);
uint8_t SPI_get_status_flag(uint8_t inst, SPI_status_flag_en flag);
uint8_t SPI_clear_status_flag(uint8_t inst, SPI_status_flag_en flag);
void SPI_enable_irq_RXRDY(uint8_t inst);
void SPI_enable_irq_TXRDY(uint8_t inst);
void SPI_enable_irq_RXOV(uint8_t inst);
void SPI_enable_irq_TXUR(uint8_t inst);
void SPI_enable_irq_SSA(uint8_t inst);
void SPI_enable_irq_SSD(uint8_t inst);
void SPI_disable_irq_RXRDY(uint8_t inst);
void SPI_disable_irq_TXRDY(uint8_t inst);
void SPI_disable_irq_RXOV(uint8_t inst);
void SPI_disable_irq_TXUR(uint8_t inst);
void SPI_disable_irq_SSA(uint8_t inst);
void SPI_disable_irq_SSD(uint8_t inst);
uint16_t SPI_read_rx_data(uint8_t inst);
uint8_t SPI_get_active_ssl(uint8_t inst);
uint8_t SPI_get_sot_flag(uint8_t inst);
void SPI_write_txdata(uint8_t inst, uint16_t data);
void SPI_select_slave(uint8_t inst, uint8_t channel);
void SPI_deselect_slave(uint8_t inst, uint8_t channel);
void SPI_set_end_of_transmission(uint8_t inst);
void SPI_clear_end_of_transmission(uint8_t inst);
void SPI_set_end_of_frame(uint8_t inst);
void SPI_clear_end_of_frame(uint8_t inst);
void SPI_set_rx_ignore(uint8_t inst);
void SPI_clear_rx_ignore(uint8_t inst);
void SPI_set_data_length(uint8_t inst, SPI_data_length_en data_length);
void SPI_set_data_and_control(uint8_t inst, SPI_TXDATCTL_reg_t *data_and_control);
void SPI_set_clock_div(uint8_t inst, uint16_t div);
uint8_t SPI_get_irq_flag_status(uint8_t inst, SPI_irq_sel_en irq);

#endif /* DR_SPI_H_ */
