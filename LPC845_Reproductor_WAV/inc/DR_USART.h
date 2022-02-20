/*******************************************************************************************************************************//**
 *
 * @file		DR_USART.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 5, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DR_USART_H_
#define DR_USART_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>
#include "DR_Tipos.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/** Peripheral USART0 base address */
#define USART0_BASE                             (0x40064000u)
/** Peripheral USART1 base address */
#define USART1_BASE                             (0x40068000u)
/** Peripheral USART2 base address */
#define USART2_BASE                             (0x4006C000u)
/** Peripheral USART3 base address */
#define USART3_BASE                             (0x40070000u)
/** Peripheral USART4 base address */
#define USART4_BASE                             (0x40074000u)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Enumeraciones
 **********************************************************************************************************************************/
/** Seleccion de instancia de USART */
typedef enum
{
	USART_SEL_0, /**< Instancia 0 */
	USART_SEL_1, /**< Instancia 1 */
	USART_SEL_2, /**< Instancia 2 */
	USART_SEL_3, /**< Instancia 3 */
	USART_SEL_4, /**< Instancia 4 */
	USART_SEL_AMOUNT /**< Cantidad de instancias */
}USART_sel_en;

typedef enum
{
	USART_DATALEN_7BIT = 0,
	USART_DATALEN_8BIT,
	USART_DATALEN_9BIT
}USART_datalen_en;

typedef enum
{
	USART_PARITY_NO_PARITY = 0,
	USART_PARITY_EVEN = 2,
	USART_PARITY_ODD = 3
}USART_parity_en;

typedef enum
{
	USART_STOPLEN_1BIT = 0,
	USART_STOPLEN_2BIT
}USART_stoplen_en;

typedef enum
{
	USART_SYNC_MODE_ASYNCHRONOUS = 0,
	USART_SYNC_MODE_SYNCHRONOUS
}USART_sync_mode_en;

typedef enum
{
	USART_POLARITY_FALLING_EDGE = 0,
	USART_POLARITY_RISING_EDGE
}USART_polarity_en;

typedef enum
{
	USART_MASTER_MODE_SLAVE = 0,
	USART_MASTER_MODE_MASTER
}USART_master_mode_en;

typedef enum
{
	USART_OUTPUT_ENABLE_POL_LOW = 0,
	USART_OUTPUT_ENABLE_POL_HIGH
}USART_output_enable_pol_en;

typedef enum
{
	USART_IRQ_RXRDY = 0,
	USART_IRQ_TXRDY,
	USART_IRQ_TXIDLE,
	USART_IRQ_DELTAACTS,
	USART_IRQ_TXDIS,
	USART_IRQ_OVERRUN,
	USART_IRQ_DELTARXBRK,
	USART_IRQ_START,
	USART_IRQ_FRAMERR,
	USART_IRQ_PARITYERR,
	USART_IRQ_RXNOISE,
	USART_IRQ_ABERR
}USART_irq_src_en;

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES - Estructuras
 **********************************************************************************************************************************/
typedef struct
{
	uint32_t ENABLE : 1;
	uint32_t : 1;
	uint32_t DATALEN : 2;
	uint32_t PARITYSEL : 2;
	uint32_t STOPLEN : 1;
	uint32_t : 2;
	uint32_t CTSEN : 1;
	uint32_t : 1;
	uint32_t SYNCEN : 1;
	uint32_t CLKPOL : 1;
	uint32_t : 1;
	uint32_t SYNCMST : 1;
	uint32_t LOOP :1;
	uint32_t : 2;
	uint32_t OETA : 1;
	uint32_t AUTOADDR : 1;
	uint32_t OESEL : 1;
	uint32_t OEPOL : 1;
	uint32_t RXPOL : 1;
	uint32_t TXPOL : 1;
	uint32_t : 8;
}USART_CFG_reg_t;

typedef struct
{
	uint32_t : 1;
	uint32_t TXBRKEN : 1;
	uint32_t ADDRDET : 1;
	uint32_t : 3;
	uint32_t TXDIS : 1;
	uint32_t : 1;
	uint32_t CC : 1;
	uint32_t CLRCONRX : 1;
	uint32_t : 6;
	uint32_t AUTOBAUD : 1;
	uint32_t : 15;
}USART_CTL_reg_t;

typedef struct
{
	uint32_t RXRDY : 1;
	uint32_t RXIDLE : 1;
	uint32_t TXRDY : 1;
	uint32_t TXIDLE : 1;
	uint32_t CTS : 1;
	uint32_t DELTACTS : 1;
	uint32_t TXDISSTAT : 1;
	uint32_t : 1;
	uint32_t OVERRUNINT : 1;
	uint32_t : 1;
	uint32_t RXBRK : 1;
	uint32_t DELTARXBRK : 1;
	uint32_t START : 1;
	uint32_t FRAMERRINT : 1;
	uint32_t PARITYERRINT : 1;
	uint32_t RXNOISEINT : 1;
	uint32_t ABERR : 1;
	uint32_t : 14;
}USART_STAT_reg_t;

typedef struct
{
	uint32_t RXRDYEN : 1;
	uint32_t : 1;
	uint32_t TXRDYEN : 1;
	uint32_t TXIDLEEN : 1;
	uint32_t : 1;
	uint32_t DELTACTSEN : 1;
	uint32_t TXDISEN : 1;
	uint32_t : 1;
	uint32_t OVERRUNEN : 1;
	uint32_t : 2;
	uint32_t DELTARXBRKEN : 1;
	uint32_t STARTEN : 1;
	uint32_t FRAMERREN : 1;
	uint32_t PARITYERREN : 1;
	uint32_t RXNOISEEN : 1;
	uint32_t ABERREN : 1;
	uint32_t : 14;
}USART_INTENSET_reg_t;

typedef struct
{
	uint32_t RXRDYCLR : 1;
	uint32_t : 1;
	uint32_t TXRDYCLR : 1;
	uint32_t TXIDLECLR : 1;
	uint32_t : 1;
	uint32_t DELTACTSCLR : 1;
	uint32_t TXDISCLR : 1;
	uint32_t : 1;
	uint32_t OVERRUNCLR : 1;
	uint32_t : 2;
	uint32_t DELTARXBRKCLR : 1;
	uint32_t STARTCLR : 1;
	uint32_t FRAMERRCLR : 1;
	uint32_t PARITYERRCLR : 1;
	uint32_t RXNOISECLR : 1;
	uint32_t ABERRCLR : 1;
	uint32_t : 14;
}USART_INTENCLR_reg_t;

typedef struct
{
	uint32_t RXDAT : 9;
	uint32_t : 23;
}USART_RXDAT_reg_t;

typedef struct
{
	uint32_t RXDAT : 9;
	uint32_t : 3;
	uint32_t FRAMERR : 1;
	uint32_t PARITYERR : 1;
	uint32_t RXNOISE : 1;
	uint32_t : 17;
}USART_RXDATSTAT_reg_t;


typedef struct
{
	uint32_t TXDAT : 9;
	uint32_t : 23;
}USART_TXDAT_reg_t;

typedef struct
{
	uint32_t BRGVAL : 16;
	uint32_t : 16;
}USART_BRG_reg_t;

typedef struct
{
	uint32_t RXRDY : 1;
	uint32_t : 1;
	uint32_t TXRDY : 1;
	uint32_t TXIDLE : 1;
	uint32_t : 1;
	uint32_t DELTACTS : 1;
	uint32_t TXDISINT : 1;
	uint32_t : 1;
	uint32_t OVERRUNINT : 1;
	uint32_t : 2;
	uint32_t DELTARXBRK : 1;
	uint32_t START : 1;
	uint32_t FRAMERRINT : 1;
	uint32_t PARITYERRINT : 1;
	uint32_t RXNOISEINT : 1;
	uint32_t ABERR : 1;
	uint32_t : 15;
}USART_INTSTAT_reg_t;

typedef struct
{
	uint32_t OSRVAL : 4;
	uint32_t : 28;
}USART_OSR_reg_t;

typedef struct
{
	uint32_t ADDRESS : 8;
	uint32_t : 24;
}USART_ADDR_reg_t;

typedef struct
{
	__RW USART_CFG_reg_t CFG;
	__RW USART_CTL_reg_t CTL;
	__RW USART_STAT_reg_t STAT;
	__RW USART_INTENSET_reg_t INTENSET;
	__RW USART_INTENCLR_reg_t INTENCLR;
	__R USART_RXDAT_reg_t RXDAT;
	__R USART_RXDATSTAT_reg_t RXDATSTAT;
	__RW USART_TXDAT_reg_t TXDAT;
	__RW USART_BRG_reg_t BRG;
	__R USART_INTSTAT_reg_t INTSTAT;
	__RW USART_OSR_reg_t OSR;
	__RW USART_ADDR_reg_t ADDR;
}USART_per_t;



/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void USART_enable(uint8_t inst);
void USART_disable(uint8_t inst);
void USART_config_data_length(uint8_t inst, USART_datalen_en datalen);
void USART_config_parity(uint8_t inst, USART_parity_en parity);
void USART_config_stop_bits(uint8_t inst, USART_stoplen_en stop_bits);
void USART_enable_CTS(uint8_t inst);
void USART_disable_CTS(uint8_t inst);
void USART_config_sync_mode(uint8_t inst, USART_sync_mode_en sync_mode);
void USART_config_clock_polarity(uint8_t inst, USART_polarity_en polarity);
void USART_config_master_mode(uint8_t inst, USART_master_mode_en master_mode);
void USART_enable_loopback(uint8_t inst);
void USART_disable_loopback(uint8_t inst);
void USART_enable_OETA(uint8_t inst);
void USART_disable_OETA(uint8_t inst);
void USART_enable_auto_address(uint8_t inst);
void USART_disable_auto_address(uint8_t inst);
void USART_enable_OESEL(uint8_t inst);
void USART_disable_OESEL(uint8_t inst);
void USART_config_OEPOL(uint8_t inst, USART_output_enable_pol_en polarity);
void USART_enable_rx_invert(uint8_t inst);
void USART_disable_rx_invert(uint8_t inst);
void USART_enable_tx_invert(uint8_t inst);
void USART_disable_tx_invert(uint8_t inst);
void USART_assert_break(uint8_t inst);
void USART_clear_break(uint8_t inst);
void USART_enable_address_detect(uint8_t inst);
void USART_disable_address_detect(uint8_t inst);
void USART_enable_tx(uint8_t inst);
void USART_disable_tx(uint8_t inst);
void USART_enable_continuous_clock(uint8_t inst);
void USART_disable_continuous_clock(uint8_t inst);
void USART_enable_autoclear_continuous_clock(uint8_t inst);
void USART_disable_autoclear_continuous_clock(uint8_t inst);
void USART_enable_autobaud(uint8_t inst);
void USART_disable_autobaud(uint8_t inst);
uint8_t USART_get_flag_RXRDY(uint8_t inst);
uint8_t USART_get_flag_RXIDLE(uint8_t inst);
uint8_t USART_get_flag_TXRDY(uint8_t inst);
uint8_t USART_get_flag_TXIDLE(uint8_t inst);
uint8_t USART_get_flag_CTS(uint8_t inst);
uint8_t USART_get_flag_DELTACTS(uint8_t inst);
uint8_t USART_get_flag_TXDISSTAT(uint8_t inst);
uint8_t USART_get_flag_OVERRUNINT(uint8_t inst);
uint8_t USART_get_flag_RXBRK(uint8_t inst);
uint8_t USART_get_flag_DELTARXBRK(uint8_t inst);
uint8_t USART_get_flag_START(uint8_t inst);
uint8_t USART_get_flag_FRAMERRINT(uint8_t inst);
uint8_t USART_get_flag_PARITYERRINT(uint8_t inst);
uint8_t USART_get_flag_RXNOISEINT(uint8_t inst);
uint8_t USART_get_flag_ABERR(uint8_t inst);
void USART_enable_irq_RXRDY(uint8_t inst);
void USART_enable_irq_TXRDY(uint8_t inst);
void USART_enable_irq_TXIDLE(uint8_t inst);
void USART_enable_irq_DELTACTS(uint8_t inst);
void USART_enable_irq_TXDISEN(uint8_t inst);
void USART_enable_irq_OVERRUN(uint8_t inst);
void USART_enable_irq_DELTARXBRK(uint8_t inst);
void USART_enable_irq_START(uint8_t inst);
void USART_enable_irq_FRAMERR(uint8_t inst);
void USART_enable_irq_PARITYERR(uint8_t inst);
void USART_enable_irq_RXNOISE(uint8_t inst);
void USART_enable_irq_ABERR(uint8_t inst);
void USART_disable_irq_RXRDY(uint8_t inst);
void USART_disable_irq_TXRDY(uint8_t inst);
void USART_disable_irq_TXIDLE(uint8_t inst);
void USART_disable_irq_DELTACTS(uint8_t inst);
void USART_disable_irq_TXDISEN(uint8_t inst);
void USART_disable_irq_OVERRUN(uint8_t inst);
void USART_disable_irq_DELTARXBRK(uint8_t inst);
void USART_disable_irq_START(uint8_t inst);
void USART_disable_irq_FRAMERR(uint8_t inst);
void USART_disable_irq_PARITYERR(uint8_t inst);
void USART_disable_irq_RXNOISE(uint8_t inst);
void USART_disable_irq_ABERR(uint8_t inst);
uint32_t USART_get_data(uint8_t inst);
uint32_t USART_get_data_and_status(uint8_t inst, uint8_t * frame, uint8_t * parity, uint8_t * noise);
void USART_write_data(uint8_t inst, uint32_t data);
void USART_set_BRGVAL(uint8_t inst, uint32_t brg);
uint8_t USART_get_irq_status_RXRDY(uint8_t inst);
uint8_t USART_get_irq_status_TXRDY(uint8_t inst);
uint8_t USART_get_irq_status_TXIDLE(uint8_t inst);
uint8_t USART_get_irq_status_DELTACTS(uint8_t inst);
uint8_t USART_get_irq_status_TXDIS(uint8_t inst);
uint8_t USART_get_irq_status_OVERRUN(uint8_t inst);
uint8_t USART_get_irq_status_DELTARXBRK(uint8_t inst);
uint8_t USART_get_irq_status_START(uint8_t inst);
uint8_t USART_get_irq_status_FRAMERR(uint8_t inst);
uint8_t USART_get_irq_status_PARITYERR(uint8_t inst);
uint8_t USART_get_irq_status_RXNOISE(uint8_t inst);
uint8_t USART_get_irq_status_ABERR(uint8_t inst);
void USART_set_OSRVAL(uint8_t inst, uint32_t osr);
void USART_set_address(uint8_t inst, uint32_t addr);

void USART_write_TX_reg(uint8_t inst, uint8_t datoTx);

#endif /* DR_USART_H_ */
