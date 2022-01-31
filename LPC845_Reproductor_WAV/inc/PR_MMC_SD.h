/*******************************************************************************************************************************//**
 *
 * @file		PR_MMC_SD.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef PR_MMC_SD_H_
#define PR_MMC_SD_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/
#include <stdint.h>
#include "DR_Tipos.h"
#include "DR_GPIO.h"
#include "PR_SPI.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
/* SPI Clock frequency and Baudrate */
#define 	SPI0_CLOCK_FREQUENCY 	(15000000UL)
#define 	SPI0_BAUD_RATE			(200000UL)
/* SPI select pin */
#define SPI_SEL 0x00100000
/* The SPI data is 8 bit long, the SD/MMC use 48 bits, 6 bytes */
#define SD_CMD_SIZE 6
/* The max MMC flash size is 256MB */
#define SD_DATA_SIZE 512 /* 16-bit in size, 512 bytes */

#define MAX_TIMEOUT 0xFF

//#define 	SPI_TX_BUFFER_SIZE			(512)
//#define 	SPI_RX_BUFFER_SIZE			(512)

#define	ENABLED							1
#define	DISABLED						0
#define	ENABLED_N						0
#define	DISABLED_N						1
#define ASSERT_N						0
#define DEASSERT_N						1

#define IDLE_STATE_TIMEOUT 				1
#define OP_COND_TIMEOUT 				2
#define SET_BLOCKLEN_TIMEOUT 			3
#define WRITE_BLOCK_TIMEOUT 			4
#define WRITE_BLOCK_FAIL 				5
#define READ_BLOCK_TIMEOUT 				6
#define READ_BLOCK_DATA_TOKEN_MISSING 	7
#define DATA_TOKEN_TIMEOUT 				8
#define SELECT_CARD_TIMEOUT 			9
#define SET_RELATIVE_ADDR_TIMEOUT 		10

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/
/* Card type flags (CardType) */
typedef enum {
	CT_NOT_DEFINED = 0,
	CT_MMC,
	CT_SDv1,
	CT_SDv2,
	CT_SDv2_SC,
	CT_SDv2_HC_XC
}card_type_en;


/*
#pragma pack(push, 1)
typedef struct
{
	uint8_t in_idle_state : 1;
	uint8_t erase_reset : 1;
	uint8_t illegal_cmd : 1;
	uint8_t cmd_crc_error : 1;
	uint8_t erase_seq_error : 1;
	uint8_t address_error : 1;
	uint8_t parameter_error : 1;
	uint8_t zero : 1;
}R1_response_str_t;

typedef union
{
	uint8_t byte;
	R1_response_str_t bits;
}R1_response_union_t;

typedef struct
{
	uint8_t card_locked : 1;
	uint8_t wp_erase_skip_or_locked_unlock_cmd_failed : 1;
	uint8_t error : 1;
	uint8_t cc_error : 1;
	uint8_t card_ecc_failed : 1;
	uint8_t wp_violation : 1;
	uint8_t erase_param : 1;
	uint8_t out_of_range_or_csd_overwr : 1;

	R1_response_union_t r1;
}R2_response_str_t;

typedef union
{
	R2_response_str_t str;
	uint16_t word;
	uint8_t array[sizeof(R2_response_str_t)];
}R2_response_union_t;
#pragma pack(pop)
*/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;
/*extern volatile spi_master_mode_tx_data_t spi_tx_buff[];
extern volatile uint16_t spi_txInput;
extern volatile uint16_t spi_txOutput;

extern volatile uint8_t spi_rx_buff[];
extern volatile uint32_t spi_rxInput;
extern volatile uint32_t spi_rxOutput;

extern volatile uint8_t spi_rx_complete_flag;
extern volatile uint8_t spi_tx_flag;*/

extern volatile uint8_t sd_not_init;
extern volatile uint8_t sd_write_flag;
extern volatile uint8_t sd_read_flag;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void mmc_delay_discount(void);

int mmc_start_init(uint8_t spi_inst);
card_type_en sd_init(uint8_t spi_inst);
card_type_en sd_init_v2(uint8_t spi_inst);

int mmc_response(uint8_t response);
int mmc_read_block(uint16_t block_number);
int mmc_write_block(uint8_t spi_inst, uint16_t block_number);
int mmc_wait_for_write_finish(void);

void sd_send_command(volatile spi_context_t ctx[], uint8_t command, uint32_t argument);
void sd_send_command_v2(volatile spi_context_t ctx[], uint8_t command, uint32_t argument);
void sd_send_dummy_bytes(volatile spi_context_t ctx[], uint32_t size, spi_ssel_enable_en ssel0_en, spi_rx_ignore_en rx_ignore);
void sd_send_dummy_bytes_v2(volatile spi_context_t ctx[], uint32_t size, spi_ssel_enable_en ssel0_en, spi_eot_en eot, spi_rx_ignore_en rx_ignore);

uint16_t sd_write_single_block(uint8_t spi_inst, uint32_t block_number, uint8_t data[], uint32_t size);
uint16_t sd_read_single_block(uint8_t spi_inst, uint32_t block_number, uint8_t data_buffer[]);

/* --- OLD ------------------ */
uint8_t mmc_load_dummy_OLD(volatile spi_context_t ctx[], uint16_t size);
void mmc_load_command_OLD(volatile spi_context_t ctx[], uint8_t command, uint32_t argument);

void mmc_send_dummy_bytes_OLD(volatile spi_context_t ctx[], uint32_t size, spi_rx_ignore_en rx_ignore);
void mmc_send_command_OLD(volatile spi_context_t ctx[], uint8_t command, uint32_t argument);


uint8_t sd_crc7 (const uint8_t *p_data, const uint32_t len);
uint16_t sd_swap_uint16(uint16_t value);
uint32_t sd_swap_uint32(uint32_t value);


void timer_sd_init_handler(uint32_t value);
void timer_sd_cmd41_handler(uint32_t value);


#endif /* PR_MMC_SD_H_ */
