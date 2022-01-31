/*******************************************************************************************************************************//**
 *
 * @file		PR_MMC_SD.c
 * @brief		Descripcion del modulo
 * @date		Jan 8, 2022
 * @author		Federico Speroni
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_MMC_SD.h"
#include "DR_SPI.h"
#include "DR_SYSCON.h"
#include "PR_Timer.h"

#include <string.h>
#include <stdio.h>

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
/* Definitions for MMC/SDC command */

/************************************* Basic command set **************************************************************************/
/* Reset the SD card to idle state */
#define CMD0	(0)										/* GO_IDLE_STATE */
/* Sends host capacity support, activates card init. process (MMC mode, do not use for SD cards) */
#define CMD1	(1)										/* SEND_OP_COND (MMC) */
/* Sends interface condition, checks if SD can operate at supplied voltage */
#define CMD8	(8)										/* SEND_IF_COND */
/* Card sends the CSD */
#define CMD9	(9)										/* SEND_CSD */
/* Card sends the CID */
#define CMD10	(10)									/* SEND_CID */
/* Stop a multiple block (stream) read/write operation */
#define CMD12	(12)									/* STOP_TRANSMISSION */
/* Get the addressed card’s status register */
#define CMD13 	(13)									/* SEND_STATUS */
/* Read the OCR register (SPI mode only) */
#define CMD58	(58)									/* READ_OCR */
/* Turn CRC option on or off */
#define CMD59 	(59)									/* CRC_ON_OFF */

/********************************** Block read commands ***************************************************************************/
/* Set the block length (only in SDSC, fixed 512B in SDHC/SDXC) */
#define CMD16	(16)									/* SET_BLOCKLEN */
/* Read a single block of size selected with CMD16*/
#define CMD17	(17)									/* READ_SINGLE_BLOCK */
/* Read multiple blocks until a CMD12 */
#define CMD18	(18)									/* READ_MULTIPLE_BLOCK */

/********************************** Block write commands **************************************************************************/
/* Set block count (MMC mode, do not use for SD cards) */
#define CMD23	(23)									/* SET_BLOCK_COUNT (MMC) */
/* Write a block of the size selected with CMD16 */
#define CMD24	(24)									/* WRITE_BLOCK */
/* Multiple block write until a CMD12 */
#define CMD25	(25)									/* WRITE_MULTIPLE_BLOCK */
/* Program the programmable bits of the CSD */
#define CMD27	(27)									/* PROGRAM_CSD */

/********************************** Write protection ******************************************************************************/
/* Set the write protection bit of the addressed group */
#define CMD28 	(28)									/* SET_WRITE_PROT */
/* Clear the write protection bit of the addressed group */
#define CMD29 	(29)									/* CLR_WRITE_PROT */
/* Ask the card for the status of the write protection bits */
#define CMD30 	(30)									/* SEND_WRITE_PROT */

/********************************** Erase commands *********************************************************************************/
/* Set the address of the first write block to be erased */
#define CMD32 	(32)									/* ERASE_WR_BLK_START_ADDR */
/* Set the address of the last write block to be erased */
#define CMD33 	(33)									/* ERASE_WR_BLK_AND_ADDR */
/* Erase the selected write blocks */
#define CMD38 	(38)									/* ERASE */

/********************************** Application specific commands ******************************************************************/
/* Flag that the next command is application-specific */
#define CMD55	(55)									/* APP_CMD */
/* General purpose I/O Data Block for application-specific commands */
#define CMD56 	(56)									/* GEN_CMD */

/* Get the SD card’s status */
#define ACMD13	(13)									/* SD_STATUS (SDC) */
/* Get the number of written write blocks (Minus errors ) */
#define ACMD22 	(22)									/* SEND_NUM_WR_BLOCKS */
/* Set the number of write blocks to be pre-erased before writing */
#define	ACMD23	(23)									/* SET_WR_BLK_ERASE_COUNT (SDC) */
/* Sends host capacity support, activates card init. process (SD mode) */
#define	ACMD41	(41)									/* SEND_OP_COND (SDC) */
/* Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 	(42)									/* SET_CLR_CARD_DETECT */
/* Reads the SD configuration register (SCR) */
#define ACMD51 	(51)									/* SEND_SCR */

/* SD R1 Responses */
#define SD_READY            (0x00)
#define SD_IN_IDLE_STATE    (0x01)
#define SD_ERASE_RESET	    (0x02)
#define SD_ILLEGAL_CMD    	(0x04)
#define SD_COM_CRC_ERROR    (0x08)
#define SD_ERASE_SEQ_ERROR  (0x10)
#define SD_ADDRESS_ERROR    (0x20)
#define SD_PARAMETER_ERROR  (0x40)

/* SD Write Data Token Responses */
#define SD_DATA_ACCEPTED			(0xE5)
#define SD_DATA_REJECTED_CRC_ERROR	(0xEB)
#define SD_DATA_REJECTED_WR_ERROR	(0xED)

#define SD_BUSY		    	(0x00)

/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT			0x01	/* Drive not initialized */
#define STA_NODISK			0x02	/* No medium in the drive */
#define STA_PROTECT			0x04	/* Write protected */

/* Read / Write Data Tokens */
#define SD_START_BLOCK_TOKEN_SINLE_READ   	(0xFE)
#define SD_START_BLOCK_TOKEN_SINGLE_WRITE 	(0xFE)
#define SD_START_BLOCK_TOKEN_MULTI_READ  	(0xFE)
#define SD_START_BLOCK_TOKEN_MULTI_WRITE  	(0xFC)
#define SD_STOP_TRAN_TOKEN 					(0xFD)

#define SD_GENERIC_CRC16 					(0xFFFF)

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO - Enumeraciones
 **********************************************************************************************************************************/
typedef enum sd_initialization_state
{
  SD_POWER_CYCLE = 0,
  SD_SEND_CMD0,
  SD_SEND_CMD8,
  SD_SEND_CMD58,
  SD_SEND_CMD55,
  SD_SEND_ACMD41,
  SD_SEND_CMD16,
  SD_SEND_CMD1,
  SD_SUCCESS,
  SD_ERROR
}sd_init_phase_en;

typedef enum
{
	SD_CMD24_SEND = 0,
	SD_CMD24_RESP,
	SD_SEND_PACKET,
	SD_PACKET_RESP,
	SD_WRITING_BUSY
}sd_single_write_states_e;

typedef enum
{
	SD_CMD17_SEND = 0,
	SD_CMD17_RESP,

 }sd_single_read_states_e;

 typedef enum
 {
 	SD_CMD18_SEND = 0,
 	SD_CMD18_RESP,
	SD_READ_MULTI_PACKETS,

  }sd_multi_read_states_e;

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO - Estructuras
 **********************************************************************************************************************************/
/* Status of Disk Functions */
typedef uint8_t	DSTATUS;

/* resguardo alineacion asignada por el compilador y seteo alineacion de estructuras sin padding,
 * de modo tal que el tamaño de sd_command_struct_t coincida exactamente con MMC_CMD_SIZE */
#pragma pack(push, 1)

typedef struct {
	uint8_t cmdIndex;
	uint32_t argument;
	uint8_t crc;
}sd_command_struct_t;

typedef union {
	sd_command_struct_t cmd;
	uint8_t cmdArray[SD_CMD_SIZE];
}sd_command_union_t;

/* --- SD Registers ------------------------------------------------- */

typedef struct
{
	uint32_t reserved_1        : 15;
	uint32_t range_2v7_2v8     : 1;
	uint32_t range_2v8_2v9     : 1;
	uint32_t range_2v9_3v0     : 1;
	uint32_t range_3v0_3v1     : 1;
	uint32_t range_3v1_3v2     : 1;
	uint32_t range_3v2_3v3     : 1;
	uint32_t range_3v3_3v4     : 1;
	uint32_t range_3v4_3v5 	   : 1;
	uint32_t range_3v5_3v6     : 1;
	uint32_t s18a 		       : 1;
	uint32_t reserved_2        : 2;
	uint32_t co2t 		       : 1;
	uint32_t reserved_3    	   : 1;
	uint32_t UHSII_card_status : 1;
	uint32_t ccs 			   : 1;
	uint32_t card_pwrup_status : 1;
}OCR_reg_t;

typedef union
{
	uint32_t dword;
	OCR_reg_t bits;
}OCR_union_t;

/* --- SD Responses ------------------------------------------------- */

/* R1 is 1 byte only and it is used for most commands */

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
	uint8_t one : 1;
	uint8_t status : 3;
	uint8_t zero : 1;
	uint8_t reserved : 3;
}Data_resp_token_str_t;

typedef union
{
	uint8_t byte;
	Data_resp_token_str_t bits;

}Data_resp_token_union_t;

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

typedef struct
{
	R1_response_union_t r1;
	OCR_union_t ocr;
}R3_response_str_t;

typedef union
{
	R3_response_str_t str;
	uint8_t array[sizeof(R1_response_union_t) + sizeof(uint32_t)];
}R3_response_union_t;

typedef struct
{
	uint32_t echo 			  : 8;
	uint32_t voltage_accepted : 4;
	uint32_t reserved 		  : 16;
	uint32_t cmd_version	  : 4;
}If_cond_reg_t;

typedef union
{
	uint32_t dword;
	If_cond_reg_t bits;
}If_cond_union_t;

typedef struct
{
	R1_response_union_t r1;
	If_cond_union_t interf_cond;
}R7_response_str_t;

typedef union
{
	R7_response_str_t str;
	uint8_t array[sizeof(R1_response_union_t) + sizeof(If_cond_union_t)];
}R7_response_union_t;

typedef union {
	uint16_t word;
	uint8_t array[2];
}sd_crc16_t;

typedef struct
{
	uint8_t data_token;
	uint8_t data_block[SD_DATA_SIZE];
	uint16_t crc;
}sd_data_packet_t;

/* reestablezco la alineacion original asignada por el compilador */
#pragma pack(pop)

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
/*volatile spi_master_mode_tx_data_t spi_tx_buff[SPI_TX_BUFFER_SIZE + 1];

volatile uint16_t spi_txInput = 0;
volatile uint16_t spi_txOutput = 0;

volatile uint8_t spi_rx_buff[100];
volatile uint32_t spi_rxInput = 0;
volatile uint32_t spi_rxOutput = 0;

volatile uint8_t spi_rx_complete_flag = 0;
volatile uint8_t spi_tx_flag = 0;*/

//static char comando[6];
//static char buffer[512];
//static uint32_t i = 0;

/* SD not initialized flag TRUE */
volatile uint8_t sd_not_init = 1;
/* SD test write flag */
volatile uint8_t sd_write_flag = 1;
volatile uint8_t sd_read_flag = 1;

volatile uint8_t MMCWRData[SD_DATA_SIZE];
volatile uint8_t MMCRDData[SD_DATA_SIZE];
volatile uint8_t MMCCmd[SD_CMD_SIZE];
volatile uint16_t delay_mmc = 0;

volatile uint8_t MMCStatus = 0;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
static volatile sd_init_phase_en estado_sd = SD_POWER_CYCLE;

static volatile uint8_t sd_power_cycle_flag = 0;
static volatile uint8_t sd_cmd0_flag = 0;
static volatile uint8_t sd_cmd8_flag = 0;
static volatile uint8_t sd_cmd55_flag = 0;
static volatile uint8_t sd_cmd58_flag = 0;
static volatile uint8_t sd_acmd41_flag = 0;

static volatile uint8_t sd_acmd41_retry = 0;
static volatile uint8_t sd_acmd41_timeout = 0;

static volatile uint8_t sd_cmd16_flag = 0;
static volatile uint8_t sd_cmd1_flag = 0;

static volatile uint8_t sd_cmd58_pass1 = 0;
static volatile uint8_t sd_cmd58_pass2 = 0;

static volatile R1_response_union_t r1 = {
		.byte = 0
};

static volatile R3_response_union_t r3 = {
		.array = { 0x00, 0x00, 0x00, 0x00, 0x00 }
};

static volatile R7_response_union_t r7 = {
		.array = { 0x00, 0x00, 0x00, 0x00, 0x00 }
};

static volatile uint16_t dato_rx;

static volatile card_type_en card_type = CT_NOT_DEFINED;

static volatile DSTATUS stat = STA_NOINIT;	/* Disk status */


/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

void sd_write_tx_buffer(volatile spi_master_mode_tx_data_t * spi_buff, uint16_t data, spi_ssel_enable_en ssel, spi_eot_en eot,
						spi_eof_en eof, spi_rx_ignore_en rx_ignore, spi_data_length_en data_lenght);
void sd_write_dummy_bytes(spi_master_mode_tx_data_t dummy_buffer[], uint32_t size, spi_eot_en eot, spi_rx_ignore_en rx_ignore);

//void sd_send_dummy_bytes(volatile spi_context_t ctx[], uint32_t size, spi_ssel_enable_en ssel0_en, spi_rx_ignore_en rx_ignore);
//void sd_send_dummy_bytes_v2(volatile spi_context_t ctx[], uint32_t size, spi_ssel_enable_en ssel0_en, spi_eot_en eot, spi_rx_ignore_en rx_ignore);
//void sd_send_command(volatile spi_context_t ctx[], uint8_t command, uint32_t argument);
//void sd_send_command_v2(volatile spi_context_t ctx[], uint8_t command, uint32_t argument);

uint8_t sd_receive_r1(volatile R1_response_union_t *r1);
uint8_t sd_receive_r2(volatile R2_response_union_t *r2);
uint8_t sd_receive_r3(volatile R3_response_union_t *r3);
uint8_t sd_receive_r7(volatile R7_response_union_t *r7);

void sd_power_cycle(volatile spi_context_t * spi_ctx);
uint8_t sd_go_idle_state(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1);
uint8_t sd_send_interface_condition(volatile spi_context_t * spi_ctx, volatile R7_response_union_t * r7);
uint8_t sd_read_ocr(volatile spi_context_t * spi_ctx, volatile R3_response_union_t * r3);
uint8_t sd_send_app_cmd(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1);
uint8_t sd_send_op_cond(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1);
uint8_t sd_set_blocklen(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1);
uint8_t sd_send_op_cond_mmc(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1);


 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/
/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

/*!
 * brief Send a command packet to MMC.
 * param command: command byte.
 * param argument: argument.
 */


 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Federcio Speroni
 	\date Jan 8, 2022
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/

/*
 * Discounts global delay counter for MMC.
 * Called by Systick Interrupt Handler, returns void.
 */

/* ---------------------------------------------------------------------------------------------------------------------------------------- */

void sd_write_tx_buffer(volatile spi_master_mode_tx_data_t * spi_buff,
						 uint16_t data,
						 spi_ssel_enable_en ssel,
						 spi_eot_en eot,
						 spi_eof_en eof,
						 spi_rx_ignore_en rx_ignore,
						 spi_data_length_en data_lenght)
{
	spi_buff->data = data;

	switch (ssel) {
		case kSPI_SSEL0_ENABLED: 	{ spi_buff->ssel0_n = 0;  spi_buff->ssel1_n = 1;  spi_buff->ssel2_n = 1;  spi_buff->ssel3_n = 1;  break; }
		case kSPI_SSEL1_ENABLED: 	{ spi_buff->ssel0_n = 1;  spi_buff->ssel1_n = 0;  spi_buff->ssel2_n = 1;  spi_buff->ssel3_n = 1;  break; }
		case kSPI_SSEL2_ENABLED: 	{ spi_buff->ssel0_n = 1;  spi_buff->ssel1_n = 1;  spi_buff->ssel2_n = 0;  spi_buff->ssel3_n = 1;  break; }
		case kSPI_SSEL3_ENABLED: 	{ spi_buff->ssel0_n = 1;  spi_buff->ssel1_n = 1;  spi_buff->ssel2_n = 1;  spi_buff->ssel3_n = 0;  break; }
		case kSPI_SSEL0_3_DISABLED:	{ spi_buff->ssel0_n = 1;  spi_buff->ssel1_n = 1;  spi_buff->ssel2_n = 1;  spi_buff->ssel3_n = 1;  break; }
		default:  break;
	}

	spi_buff->eot = eot;
	spi_buff->eof = eof;
	spi_buff->rxignore = rx_ignore;
	spi_buff->data_length = data_lenght;
}

void sd_write_dummy_bytes(spi_master_mode_tx_data_t dummy_buffer[], uint32_t size, spi_eot_en eot, spi_rx_ignore_en rx_ignore)
{
	for (size_t i = 0; i < size; ++i)
			sd_write_tx_buffer(&dummy_buffer[i], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, eot, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);
}

void sd_send_dummy_bytes(volatile spi_context_t ctx[], uint32_t size, spi_ssel_enable_en ssel0_en, spi_rx_ignore_en rx_ignore)
{
	spi_master_mode_tx_data_t dummy_array[size];

	for (size_t i = 0; i < size; ++i) {
		if (i < size - 1)
			sd_write_tx_buffer(&dummy_array[i], SPI_DUMMY_BYTE, ssel0_en, kSPI_NOT_EOT, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);
		else
			sd_write_tx_buffer(&dummy_array[i], SPI_DUMMY_BYTE, ssel0_en, kSPI_EOT, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);

	}

	spi_master_mode_send_data(ctx, dummy_array, size);
}

void sd_send_dummy_bytes_v2(volatile spi_context_t ctx[], uint32_t size, spi_ssel_enable_en ssel0_en, spi_eot_en eot, spi_rx_ignore_en rx_ignore)
{
	spi_master_mode_tx_data_t dummy_array[size];

	for (size_t i = 0; i < size; ++i) {
		if (i < size - 1)
			sd_write_tx_buffer(&dummy_array[i], SPI_DUMMY_BYTE, ssel0_en, kSPI_NOT_EOT, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);
		else
			sd_write_tx_buffer(&dummy_array[i], SPI_DUMMY_BYTE, ssel0_en, eot, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);

	}

	spi_master_mode_send_data(ctx, dummy_array, size);
}

void sd_send_command(volatile spi_context_t ctx[], uint8_t command, uint32_t argument)
{
	sd_command_union_t sd;
	spi_master_mode_tx_data_t tx_cmd[SD_CMD_SIZE];

	/**
	 *  Load command packet
	 *  i = 0: Start + Command index
	 *  i = 1: Argument[31..24]
	 *  i = 2: Argument[23..16]
	 *  i = 3: Argument[15..8]
	 *  i = 4: Argument[7..0]
	 *  i = 5: (CRC7(cmd[0..4]) << 1) | 0x01
	 */

	/* Armo el comando */
	sd.cmd.cmdIndex = 0x40 | (command & 0x3F);
	sd.cmd.argument = sd_swap_uint32(argument);
	sd.cmd.crc = ((sd_crc7(sd.cmdArray, SD_CMD_SIZE - 1)) << 1) | 0x01;

	for (size_t i = 0; i < SD_CMD_SIZE; ++i) {
		if ( i < SD_CMD_SIZE - 1)
			sd_write_tx_buffer(&tx_cmd[i], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT,
							   kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
		else
			sd_write_tx_buffer(&tx_cmd[i], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_EOT,
							   kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	}

	spi_master_mode_send_data(ctx, tx_cmd, SD_CMD_SIZE);

}

void sd_send_command_v2(volatile spi_context_t ctx[], uint8_t command, uint32_t argument)
{
	sd_command_union_t sd;
	const uint8_t dummy_size = 2;
	spi_master_mode_tx_data_t tx_data[SD_CMD_SIZE + dummy_size];
	size_t i = 0, j = 0;

	/**
	 *  Load command packet
	 *  i = 0: Start | Command index
	 *  i = 1: Argument[31:24]
	 *  i = 2: Argument[23:16]
	 *  i = 3: Argument[15:8]
	 *  i = 4: Argument[7:0]
	 *  i = 5: (CRC7(cmd[0..4]) << 1) | 0x01
	 *
	 */

	/* Armo el comando */
	sd.cmd.cmdIndex = 0x40 | (command & 0x3F);
	sd.cmd.argument = sd_swap_uint32(argument);
	sd.cmd.crc = ((sd_crc7(sd.cmdArray, SD_CMD_SIZE - 1)) << 1) | 0x01;

	/* Dummy Byte before command */
	sd_write_tx_buffer(&tx_data[j], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	for (i = 0, j = 1; i < SD_CMD_SIZE; ++i, ++j)
		sd_write_tx_buffer(&tx_data[j], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	/* Dummy Byte after command */
	sd_write_tx_buffer(&tx_data[j], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	/*for (size_t i = SD_CMD_SIZE; i < SD_CMD_SIZE + dummy_size; ++i) {
		if (i < SD_CMD_SIZE + dummy_size - 1)
			sd_write_tx_buffer(&tx_data[i], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_NOT_EOF, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);
		else
			sd_write_tx_buffer(&tx_data[i], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	}*/

	spi_master_mode_send_data(ctx, tx_data, SD_CMD_SIZE + dummy_size);

}

/*******************************************************************************
 Read R1 from SD card
*******************************************************************************/
uint8_t sd_receive_r1(volatile R1_response_union_t *r1)
{
	uint8_t ret = 0;
	uint8_t resp = (uint8_t)spi_pop_RX(spi_ctx);

	if (resp < 0x80) {
		r1->byte = resp;
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Read R2 from SD card
*******************************************************************************/
uint8_t sd_receive_r2(volatile R2_response_union_t *r2)
{
	uint8_t ret = 0;
	static uint8_t i = 0;

	if ((i == 0) && sd_receive_r1(&r2->str.r1))
		if ((r2->array[i] < 0x80) /*(r2->array[i] == 0x00) || (r2->array[i] == 0x01) || (r2->array[i] == 0x05)*/)
			i++;

	if ((i > 0) && (i < sizeof(R2_response_union_t)))
		r2->array[i++] = (uint8_t)spi_pop_RX(spi_ctx);

	if (i == sizeof(R2_response_union_t)) {
		ret = 1;
		i = 0;
	}

	return ret;
}

/*******************************************************************************
 Read R3 from SD card
*******************************************************************************/
uint8_t sd_receive_r3(volatile R3_response_union_t *r3)
{
	uint8_t ret = 0;
	static uint8_t i = 0;

	if ((i == 0) && sd_receive_r1(&r3->str.r1))
		if ((r3->array[i] == 0x00) || (r3->array[i] == 0x01) || (r3->array[i] == 0x05))
			i++;

	if ((i > 0) && (i < sizeof(R3_response_union_t)))
		r3->array[i++] = (uint8_t)spi_pop_RX(spi_ctx);

	if (i == sizeof(R3_response_union_t)) {
		r3->str.ocr.dword = sd_swap_uint32(r3->str.ocr.dword);
		i = 0;
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Read R7 from SD card
*******************************************************************************/
uint8_t sd_receive_r7(volatile R7_response_union_t *r7)
{
	uint8_t ret = 0;
	static uint8_t i = 0;

	if ((i == 0) && sd_receive_r1(&r7->str.r1))
		if ((r7->array[i] == 0x01) || (r7->array[i] == 0x05))
			i++;

	if ((i > 0) && (i < sizeof(R7_response_union_t)))
		r7->array[i++] = (uint8_t)spi_pop_RX(spi_ctx);

	if (i == sizeof(R7_response_union_t)) {
		r7->str.interf_cond.dword = sd_swap_uint32(r7->str.interf_cond.dword);
		i = 0;
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Read Data Response Token from SD card
*******************************************************************************/
uint8_t sd_receive_data_resp_token(volatile Data_resp_token_union_t *data_resp_token)
{
	uint8_t ret = 0;
	uint8_t resp = (uint8_t)spi_pop_RX(spi_ctx);

	if (resp == SD_DATA_ACCEPTED) {
		data_resp_token->byte = resp;
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Check Busy Flag Response from SD card
*******************************************************************************/
uint8_t sd_check_busy_state(void)
{
	uint8_t resp = (uint8_t)spi_pop_RX(spi_ctx);

	return (resp == SD_BUSY) ? 1 : 0;
}

/*******************************************************************************
 Run power up sequence
*******************************************************************************/
void sd_power_cycle(volatile spi_context_t * spi_ctx)
{
	static volatile uint8_t sd_power_cycle_flag = 0;

	if (sd_power_cycle_flag == 0) {
		/* Initialise the SD card by sending 80 clks on with SSEL0 deselected HIGH*/
		sd_send_dummy_bytes(spi_ctx, 10, kSPI_SSEL0_3_DISABLED, kSPI_RX_IGNORE);		/* 80 dummy clocks */

		sd_power_cycle_flag = 1;
	}
}

/*******************************************************************************
 Send Idle State Command (CMD0)
*******************************************************************************/
uint8_t sd_go_idle_state(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_idle_state_flag = 0;

	if (sd_idle_state_flag == 0) {
		/* GO_IDLE_STATE: to set the card in SPI mode, send CMD0 command. The argument is 0x00000000 for GO_IDLE_STATE command. */
		/* CRC value must be correct only for CMD0 and CMD8, which ar sent as native commands,
		 * but we always send dthe correct CRC, regardless of the command type. */
		sd_send_command(spi_ctx, CMD0, 0x00000000);
		//sd_send_command_v2(spi_ctx, CMD0, 0x00000000);

		/* Send some dummy clocks to receive CMD0 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_idle_state_flag = 1;
	}

	if (sd_receive_r1(r1)) {
		ret = 1;
		sd_idle_state_flag = 0;
	}

	return ret;
}

/*******************************************************************************
 Send Interface Conditions (CMD8)
*******************************************************************************/
uint8_t sd_send_interface_condition(volatile spi_context_t * spi_ctx, volatile R7_response_union_t * r7)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_send_if_cond_flag = 0;

	if (sd_send_if_cond_flag == 0) {
		/* SEND_IF_COND: Send Interface Conditions. The argument is 0x000001AA for the SEND IF COND command. */
		/* CRC value must be correct only for CMD0 and CMD8, which ar sent as native commands,
		 * but we always send dthe correct CRC, regardless of the command type. */
		sd_send_command(spi_ctx, CMD8, 0x000001AA);

		/* Send some dummy clocks to receive CMD8 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_send_if_cond_flag = 1;
	}

	if (sd_receive_r7(r7)) {
		ret = 1;
		sd_send_if_cond_flag = 0;
	}

	return ret;
}

/*******************************************************************************
 Reads OCR from SD Card (CMD58)
*******************************************************************************/
uint8_t sd_read_ocr(volatile spi_context_t * spi_ctx, volatile R3_response_union_t * r3)
{
	//static volatile uint8_t sd_read_ocr_flag = 0;

	if (sd_cmd58_flag == 0) {
		/* READ_OCR. Argument is 0x00000000 for the READ_OCR command. */
		sd_send_command(spi_ctx, CMD58, 0x00000000);

		/* Send some dummy clocks to receive CMD58 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd58_flag = 1;
	}

	sd_receive_r3(r3);

	return 1;
}

/*******************************************************************************
 Send Application Command (CMD55)
*******************************************************************************/
uint8_t sd_send_app_cmd(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	//static volatile uint8_t sd_send_app_cmd_flag = 0;

	if (sd_cmd55_flag == 0) {
		/* APP_CMD
		 * The argument is 0x00000000 for the APP_CMD command. */
		sd_send_command(spi_ctx, CMD55, 0x00000000);

		/* Send some dummy clocks to receive CMD55 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd55_flag = 1;
	}

	if (sd_receive_r1(r1))
		ret = 1;

	return ret;
}

/*******************************************************************************
 Send Operating Condition (ACMD41)
********************************************************************************/
uint8_t sd_send_op_cond(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	//static volatile uint8_t sd_send_op_cond_flag = 0;

	if (sd_acmd41_flag == 0) {
		/* SEND_OP_COND
		 * SDv2: The argument is 0x40000000 for the SD_SEND_OP_COND command. HOST/HIGH CAPACITY SUPPORT: HCS[bit 30] = 1
		 * SDv1: The argument is 0x00000000 for the SD_SEND_OP_COND command. HOST/HIGH CAPACITY SUPPORT: HCS[bit 30] = 0 */

		if (card_type == CT_SDv2)
			sd_send_command(spi_ctx, ACMD41, 0x40000000);
		else if (card_type == CT_SDv1)
			sd_send_command(spi_ctx, ACMD41, 0x00000000);

		/* Send some dummy clocks to receive ACMD41 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		/* Set 1000ms timeout for retrying CMD55-ACMD41 commands */
		if (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)
			Timer_Start(TIMER_SD_INIT_ID, 1000, MSEG, timer_sd_init_handler);

		/* Set 10ms timer for spacing retrying CMD55-ACMD41 commands */
		if (Timer_Activity_Get(TIMER_SD_CMD41_ID) == kTIMER_STOP)
			Timer_Start(TIMER_SD_CMD41_ID, 15, MSEG, timer_sd_cmd41_handler);

		sd_acmd41_flag = 1;
	}

	if (sd_receive_r1(r1))
		ret = 1;

	return ret;
}

/*******************************************************************************
 Set Block Length (CMD16)
********************************************************************************/
uint8_t sd_set_blocklen(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_set_blocklen_flag = 0;

	if (sd_set_blocklen_flag == 0) {
		/* SET_BLOCKLEN: The argument is 0x00000200 to set a block size of 512 bytes. */
		sd_send_command(spi_ctx, CMD16, 0x0000200);

		/* Send some dummy clocks to receive CMD16 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_set_blocklen_flag = 1;
	}

	if (sd_receive_r1(r1))
		ret = 1;

	return ret;
}

/*******************************************************************************
 Send Status (CMD13)
********************************************************************************/
uint8_t sd_send_status(volatile spi_context_t * spi_ctx, volatile R2_response_union_t * r2)
{
	static volatile uint8_t sd_send_status_flag = 0;

	if (sd_send_status_flag == 0) {
		/* SEND_STATUS: Asks the card to send its status register. The argument is 0x00000000. */
		sd_send_command(spi_ctx, CMD13, 0x0000000);

		/* Send some dummy clocks to receive CMD16 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_send_status_flag = 1;
	}

	sd_receive_r2(r2);

	return 1;
}

/*******************************************************************************
 Send Operating Condition (valid for MMC) (CMD1)
********************************************************************************/
uint8_t sd_send_op_cond_mmc(volatile spi_context_t * spi_ctx, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	//static volatile uint8_t sd_send_op_cond_mmc_flag = 0;

	if (sd_cmd1_flag == 0) {
		/* SEND_OP_COND (MMC)
		 * The argument is 0x00000000 for the SEND_OP_COND command. */
		sd_send_command(spi_ctx, CMD1, 0x00000000);

		/* Send some dummy clocks to receive CMD1 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd1_flag = 1;
	}

	if (sd_receive_r1(r1)) {
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Read Block (CMD17)
********************************************************************************/
uint8_t sd_read_block(volatile spi_context_t * spi_ctx, uint32_t block_addr, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_cmd17_flag = 0;

	if (sd_cmd17_flag == 0) {
		/* READ_SINGLE_BLOCK: The argument is the address of the block to read. */
		sd_send_command(spi_ctx, CMD17, block_addr);

		/* Send some dummy clocks to receive CMD1 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd17_flag = 1;
	}

	if (sd_receive_r1(r1)) {
		sd_cmd17_flag = 0;
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Read Multiple Block (CMD18)
********************************************************************************/
uint8_t sd_read_multi_block(volatile spi_context_t * spi_ctx, uint32_t block_addr, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_cmd18_flag = 0;

	if (sd_cmd18_flag == 0) {
		/* READ_SINGLE_BLOCK: The argument is the address of the block to read. */
		sd_send_command(spi_ctx, CMD18, block_addr);

		/* Send some dummy clocks to receive CMD1 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd18_flag = 1;
	}

	if (sd_receive_r1(r1)) {
		sd_cmd18_flag = 0;
		ret = 1;
	}

	return ret;
}


/*******************************************************************************
 Write Block (CMD24)
********************************************************************************/
uint8_t sd_write_block(volatile spi_context_t * spi_ctx, uint32_t block_addr, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_cmd24_flag = 0;

	if (sd_cmd24_flag == 0) {
		/* WRITE_BLOCK: The argument is the address of the block to write. */
		sd_send_command(spi_ctx, CMD24, block_addr);

		/* Send some dummy clocks to receive CMD24 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd24_flag = 1;
	}

	if (sd_receive_r1(r1)) {
		sd_cmd24_flag = 0;
		ret = 1;
	}

	return ret;
}

/*******************************************************************************
 Write Multiple Block (CMD25)
********************************************************************************/
uint8_t sd_write_multi_block(volatile spi_context_t * spi_ctx, uint32_t block_addr, volatile R1_response_union_t * r1)
{
	uint8_t ret = 0;
	static volatile uint8_t sd_cmd25_flag = 0;

	if (sd_cmd25_flag == 0) {
		/* WRITE_MULTIPLE_BLOCK: The argument is the start address of the block to write. */
		sd_send_command(spi_ctx, CMD25, block_addr);

		/* Send some dummy clocks to receive CMD24 response */
		sd_send_dummy_bytes(spi_ctx, 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		sd_cmd25_flag = 1;
	}

	if (sd_receive_r1(r1)) {
		sd_cmd25_flag = 0;
		ret = 1;
	}

	return ret;
}

/**
	\fn  sd_init
	\brief Inicializa memoria SD v1.xx/v2.00+ (SDSC, SDHC o SDXC) o MMC v3.0
 	\param [in] uint8_t inst. Instancia de comunicacion SPI
 	\param [out] card_type_en tipo de memoria.
	\return Tipo de memoria inicalizado
*/

/*
 * Secuencia de Inicializacion Basica
 *
 * 1 - CMD0 arg: 0x00000000, CRC: 0x95 (if response: 0x01 continue, else error)
 * 2 - CMD8 arg: 0x000001AA, CRC: 0x87 (response: 0x01)
 * 3 - CMD55 arg: 0x0000000000, CRC: any (CMD55 being the prefix to every ACMD)
 * 4 - ACMD41 arg: 0x40000000, CRC: any (if response: 0x00, SD init; if it's 0x1 goto 3).
 * 5 - CMD58 arg: 0x00000000, CRC: any (if response 0x01, -> SDHC success, if response 0x00 SDSC, goto 6)
 * 6 - CMD16 arg 0x00000200, CRC: any success
 * */
card_type_en sd_init_v2(uint8_t spi_inst)
{
	switch (estado_sd) {
		case SD_POWER_CYCLE:
			/* Power Cycle: 80 dummy clocks */
			sd_power_cycle(&spi_ctx[spi_inst]);

			estado_sd = SD_SEND_CMD0;

			break;

		case SD_SEND_CMD0:
			/* GO_IDLE_STATE: resets SD card */
			sd_go_idle_state(&spi_ctx[spi_inst], &r1);

			if (r1.byte == 0x01)
				estado_sd = SD_SEND_CMD8;

			break;

		case SD_SEND_CMD8:
			/* SEND_IF_COND: sends interface conditions */
			sd_send_interface_condition(&spi_ctx[spi_inst], &r7);

			if ((r7.str.r1.byte == 0x01) && r7.str.interf_cond.dword == 0x000001AA) {
				/* If we arrive to this point -> SD v2.00 or later */
				card_type = CT_SDv2;

				estado_sd = SD_SEND_CMD58;
			}

			/* If returns illegal command or CRC error -> SD v1.x */
			if ((r7.str.r1.byte == 0x05) || (r7.str.r1.byte == 0x09)) {
				card_type = CT_SDv1;

				estado_sd = SD_SEND_CMD58;
			}

			break;


		case SD_SEND_CMD58:
			/* READ_OCR: Reads the OCR register, bit CCS is OCR[30] */
			sd_read_ocr(&spi_ctx[spi_inst], &r3);

			// r3.array = { 0x01, 0x00, 0xFF, 0x80, 0x00 }   paso 1
			// r3.array = { 0x00, 0x80, 0xFF, 0x80, 0x00 }   paso 2

			if (sd_cmd58_pass1 == 0) {
				/* if r7.array[1] == 0x00 -> pwrup_status = 0, CCS = 0 -> pwrup not finished, SD v2.00, SDSC
				 * if r7.array[1] == 0x40 -> pwrup_status = 0, CCS = 1 -> pwrup not finished, SD v2.00, SDHC o SDXC */
				if ((r3.str.r1.byte == 0x01) && ((r3.str.ocr.dword == 0x00FF8000) || (r3.str.ocr.dword == 0x40FF8000))) {
					/* If we arrive to this point -> SD operates between 2v7 and 3v6 */
					sd_cmd58_pass1 = 1;

					estado_sd = SD_SEND_CMD55;
				}
			} else if ((sd_cmd58_pass1 == 1) &&(sd_cmd58_pass2 == 0) && (card_type == CT_SDv2 )) {
				if ((r3.str.r1.byte == 0x00) && (r3.str.ocr.dword == 0x80FF8000)) {
					sd_cmd58_pass2 = 1;
					/* If we arrive to this point: CCS = 0 -> SD v2.00 or Later, SDSC */
					card_type = CT_SDv2_SC;

					estado_sd = SD_SEND_CMD16;
				}
				else if ((r3.str.r1.byte == 0x00) && (r3.str.ocr.dword == 0xC0FF8000)) {
					sd_cmd58_pass2 = 1;
					/* If we arrive to this point: CCS = 1 -> SD v2.00 or Later, SDHC or SDXC */
					card_type = CT_SDv2_HC_XC;

					estado_sd = SD_SUCCESS;
				}
			}

			break;

		case SD_SEND_CMD55:
			/* APP_CMD: tells the card that next command is an application specific command */
			sd_send_app_cmd(&spi_ctx[spi_inst], &r1);

			if (r1.byte == 0x01)
				estado_sd = SD_SEND_ACMD41;

			break;

		case SD_SEND_ACMD41:
			/* SD_SEND_OP_COND: sends host capacity support and activates the card initialization process */
			sd_send_op_cond(&spi_ctx[spi_inst], &r1);

			/* Set 1000ms timeout for retrying CMD55-ACMD41 commands */
//			if (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)
//				Timer_Start(TIMER_SD_INIT_ID, 1000, MSEG, timer_sd_init_handler);

			/* Set 10ms timer for spacing retrying CMD55-ACMD41 commands */
//			if (Timer_Activity_Get(TIMER_SD_CMD41_ID) == kTIMER_STOP)
//				Timer_Start(TIMER_SD_CMD41_ID, 10, MSEG, timer_sd_cmd41_handler);

			if ((r1.byte == 0x05) || (r1.byte == 0x09) || (sd_acmd41_timeout == 1)) {
				if (card_type == CT_SDv2) {
					/* unknown Card */
					card_type = CT_NOT_DEFINED;

					estado_sd = SD_ERROR;
				}
				else if (card_type == CT_SDv1) {
					/* Card is MMC */
					card_type = CT_MMC;

					estado_sd = SD_SEND_CMD1;
				}
			}

			if ((r1.byte == 0x01) && (sd_acmd41_retry == 1) && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				/* SD not Initialiazed -> Retry */
				sd_acmd41_flag = 0;
				sd_cmd55_flag = 0;
				sd_acmd41_retry = 0;

				estado_sd = SD_SEND_CMD55;
			}

			if ((r1.byte == 0x00) && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				Timer_Stop(TIMER_SD_INIT_ID);
				Timer_Stop(TIMER_SD_CMD41_ID);

				/* SD Initialaized OK!! -> Proceed */
				sd_cmd58_flag = 0;

				estado_sd = SD_SEND_CMD58;
			}

			break;

		case SD_SEND_CMD16:
			/* SET_BLOCKLEN: if card is SDSC, this command sets the length of the data block.
			 * If card is SDHC or SDXC, block length is fixed to 512 bytes */
			sd_set_blocklen(&spi_ctx[spi_inst], &r1);

			if (r1.byte == 0x00) {
				estado_sd = SD_SUCCESS;
			}

			break;

		case SD_SEND_CMD1:
			/* SEND_OP_COND: sends host capacity supprt and activates card initialization process (valid for MMC cards) */
			sd_send_op_cond_mmc(&spi_ctx[spi_inst], &r1);

			/* Set 1000ms timer for retrying CMD1 command */
			if (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)
				Timer_Start(TIMER_SD_INIT_ID, 1000, MSEG, timer_sd_init_handler);

			if ((r1.byte == 0x05) || (r1.byte == 0x09) || (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)) {
				/* Unknown Card */
				card_type = CT_NOT_DEFINED;

				estado_sd = SD_ERROR;
			}

			if ((r1.byte == 0x01) && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				/* MMC not Initialiazed -> Retry */
				sd_cmd1_flag = 0;;

				estado_sd = SD_SEND_CMD1;
			}

			if (r1.byte == 0x00 && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				/* MMC Initialaized OK  */
				Timer_Stop(TIMER_SD_INIT_ID);

				estado_sd = SD_SEND_CMD16;
			}

			break;

		case SD_SUCCESS:
			/* SD not initialized flag FALSE */
			sd_not_init = 0;

			break;

		case SD_ERROR:
			card_type = CT_NOT_DEFINED;

			break;

		default:
			break;
	}

	return card_type;
}

/************************** MMC/SD Init *********************************/

/**
	\fn  sd_init
	\brief Inicializa memoria SD v1.xx/v2.00+ (SDSC, SDHC o SDXC) o MMC v3.0
 	\param [in] uint8_t inst. Instancia de comunicacion SPI
 	\param [out] card_type_en tipo de memoria.
	\return Tipo de memoria inicalizado
*/

/*
 * Secuencia de Inicializacion basica
 *
 * CMD0 arg: 0x0, CRC: 0x95 (response: 0x01)
 * CMD8 arg: 0x000001AA, CRC: 0x87 (response: 0x01)
 * CMD55 arg: 0x0, CRC: any (CMD55 being the prefix to every ACMD)
 * ACMD41 , arg: 0x40000000, CRC: any
 * if response: 0x0, you're OK; if it's 0x1, goto 3.
 * */

card_type_en sd_init(uint8_t spi_inst)
{
	static volatile uint8_t r7_i = 0;
	static volatile uint8_t r3_i = 0;

	//if (dato_rx == 0xFFFF)
		//return;

	switch (estado_sd) {
		case SD_POWER_CYCLE:
			if (sd_power_cycle_flag == 0) {
				/* Delay >= 1ms */
				/* Initialise the SD card by sending 80 clks on with SSEL0 deselected HIGH*/
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 10, kSPI_SSEL0_3_DISABLED, kSPI_RX_IGNORE);		/* 80 dummy clocks */
				sd_power_cycle_flag = 1;
				
				estado_sd = SD_SEND_CMD0;
			}

			break;
			
		case SD_SEND_CMD0:
			if (sd_cmd0_flag == 0) {
				/* GO_IDLE_STATE: to set the card in SPI mode, send CMD0 command. The argument is 0x00000000 for GO_IDLE_STATE command. */
				/* CRC value must be correct only for CMD0 and CMD8, which ar sent as native commands,
				 * but we always send dthe correct CRC, regardless of the command type. */
				sd_send_command(&spi_ctx[spi_inst], CMD0, 0x00000000);

				/* Send some dummy clocks to receive CMD0 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				sd_cmd0_flag = 1;
			}

			r1.byte = (uint8_t)spi_pop_RX(spi_ctx);

			if (r1.byte == 0x01)
				estado_sd = SD_SEND_CMD8;

			break;
			
		case SD_SEND_CMD8:

			if (sd_cmd8_flag == 0) {
				/* SEND_IF_COND: Send Interface Conditions. The argument is 0x000001AA for the SEND IF COND command. */
				/* CRC value must be correct only for CMD0 and CMD8, which ar sent as native commands,
				 * but we always send dthe correct CRC, regardless of the command type. */
				sd_send_command(&spi_ctx[spi_inst], CMD8, 0x000001AA);

				/* Send some dummy clocks to receive CMD8 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				sd_cmd8_flag = 1;
			}

			if (r7_i == 0) {
				r7.array[r7_i] = (uint8_t)spi_pop_RX(spi_ctx);

				if (r7.array[r7_i] == 0x01)
					r7_i++;

				/* If returns illegal command - > SD v1.x */
				if ((r7.array[r7_i] == 0x05) || (r7.array[r7_i] == 0x09)) {
					card_type = CT_SDv1;
					estado_sd = SD_SEND_CMD58;
				}
			}

			if ((r7_i == 1) | (r7_i == 2)) {
				r7.array[r7_i] = (uint8_t)spi_pop_RX(spi_ctx);
				if (r7.array[r7_i] == 0x00)
					r7_i++;
			}

			if (r7_i == 3) {
				r7.array[r7_i] = (uint8_t)spi_pop_RX(spi_ctx);
				if (r7.array[r7_i] == 0x01)
					r7_i++;
			}

			if (r7_i == 4) {
				r7.array[r7_i] = (uint8_t)spi_pop_RX(spi_ctx);
				if (r7.array[r7_i] == 0x0AA) {

					r7.str.interf_cond.dword = sd_swap_uint32(r7.str.interf_cond.dword);

					/* If we arrive to this point -> SD v2.00 or later */
					card_type = CT_SDv2;
					estado_sd = SD_SEND_CMD58;	//SD_SEND_CMD55;
				}
			}

			break;
			
		case SD_SEND_CMD58:
			if (sd_cmd58_flag == 0) {
				/* READ_OCR. Argument is 0x00000000 for the READ_OCR command. */
				sd_send_command(&spi_ctx[spi_inst], CMD58, 0x00000000);

				/* Send some dummy clocks to receive CMD58 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				sd_cmd58_flag = 1;
			}

			// r3.array = { 0x01, 0x00, 0xFF, 0x80, 0x00 }   paso 1
			// r3.array = { 0x00, 0x80, 0xFF, 0x80, 0x00 }   paso 2

			if (sd_cmd58_pass1 == 0) {

				if (r3_i == 0) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0x01)
						r3_i++;
				}

				if (r3_i == 1) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					/* if r7.array[1] == 0x00 -> pwrup_status = 0, CCS = 0 -> pwrup not finished, SD v2.00, SDSC
					 * if r7.array[1] == 0x40 -> pwrup_status = 0, CCS = 1 -> pwrup not finished, SD v2.00, SDHC o SDXC */
					if ((r3.array[r3_i] == 0x00) | (r3.array[r3_i] == 0x40))
						r3_i++;
				}

				if (r3_i == 2) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0xFF)
						r3_i++;
				}

				if (r3_i == 3) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0x80)
						r3_i++;
				}

				if (r3_i == 4) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0x00) {
						sd_cmd58_pass1 = 1;
						r3_i = 0;

						r3.str.ocr.dword = sd_swap_uint32(r3.str.ocr.dword);

						estado_sd = SD_SEND_CMD55;
					}
				}
			}
			else if((sd_cmd58_pass2 == 0) && (card_type == CT_SDv2 )) {
				if (r3_i == 0) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0x00)
						r3_i++;
				}

				if (r3_i == 1) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					/* if r7.array[1] == 0x00 -> pwrup_status = 1, CCS = 0 -> pwrup finished, SD v2.00, SDSC
					 * if r7.array[1] == 0x40 -> pwrup_status = 1, CCS = 1 -> pwrup finished, SD v2.00, SDHC o SDXC */
					if ((r3.array[r3_i] == 0x80) || (r3.array[r3_i] == 0xC0))
						r3_i++;
				}

				if (r3_i == 2) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0xFF)
						r3_i++;
				}

				if (r3_i == 3) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0x80)
						r3_i++;
				}

				if (r3_i == 4) {
					r3.array[r3_i] = (uint8_t)spi_pop_RX(spi_ctx);
					if (r3.array[r3_i] == 0x00) {
						sd_cmd58_pass2 = 1;

						r3.str.ocr.dword = sd_swap_uint32(r3.str.ocr.dword);

						if (r3.str.ocr.dword == 0x80FF8000) {
							/* If we arrive to this point: CCS = 0 -> SD v2.00 or Later, SDSC */
							card_type = CT_SDv2_SC;

							estado_sd = SD_SEND_CMD16;
						}
						else if (r3.str.ocr.dword == 0xC0FF8000){
							/* If we arrive to this point: CCS = 1 -> SD v2.00 or Later, SDDHC or SDXC */
							card_type = CT_SDv2_HC_XC;

							estado_sd = SD_SUCCESS;
						}
					}
				}
			}

			break;

		case SD_SEND_CMD55:
			if (sd_cmd55_flag == 0) {
				/* APP_CMD
				 * The argument is 0x00000000 for the APP_CMD command. */
				sd_send_command(&spi_ctx[spi_inst], CMD55, 0x00000000);

				/* Send some dummy clocks to receive CMD55 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				sd_cmd55_flag = 1;
			}
			
			r1.byte = (uint8_t)spi_pop_RX(spi_ctx);

			if (r1.byte == 0x01)
				estado_sd = SD_SEND_ACMD41;

			break;

		case SD_SEND_ACMD41:
			if (sd_acmd41_flag == 0) {
				/* APP_CMD
				 * SDv2: The argument is 0x40000000 for the SD_SEND_OP_COND command. HOST/HIGH CAPACITY SUPPORT: HCS[bit 30] = 1
				 * SDv1: The argument is 0x00000000 for the SD_SEND_OP_COND command. HOST/HIGH CAPACITY SUPPORT: HCS[bit 30] = 0 */

				if (card_type == CT_SDv2)
					sd_send_command(&spi_ctx[spi_inst], ACMD41, 0x40000000);
				else if (card_type == CT_SDv1)
					sd_send_command(&spi_ctx[spi_inst], ACMD41, 0x00000000);

				/* Send some dummy clocks to receive ACMD41 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				/* Set 1000ms timer for retrying CMD55-ACMD41 commands */
				if (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)
					Timer_Start(TIMER_SD_INIT_ID, 1000, MSEG, timer_sd_init_handler);

				sd_acmd41_flag = 1;
			}

			r1.byte = (uint8_t)spi_pop_RX(spi_ctx);

			if ((r1.byte == 0x05) || (r1.byte == 0x09) || (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)) {
				if (card_type == CT_SDv2) {
					/* unknown Card */
					card_type = CT_NOT_DEFINED;

					estado_sd = SD_ERROR;
				}
				else if (card_type == CT_SDv1) {
					/* Card is MMC */
					card_type = CT_MMC;

					estado_sd = SD_SEND_CMD1;
				}
			}

			if ((r1.byte == 0x01) && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				sd_acmd41_flag = 0;
				sd_cmd55_flag = 0;
				/* SD not Initialiazed -> Retry */

				estado_sd = SD_SEND_CMD55;
			}

			if (r1.byte == 0x00 && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				Timer_Stop(TIMER_SD_INIT_ID);

				/* SD Initialaized OK!! -> Proceed */
				sd_cmd58_flag = 0;

				estado_sd = SD_SEND_CMD58;
			}

			break;

		case SD_SEND_CMD16:
			if (sd_cmd16_flag == 0) {
				/* SET_BLOCKLEN: The argument is 0x00000200 to set a block size of 512 bytes. */
				sd_send_command(&spi_ctx[spi_inst], CMD16, 0x0000200);

				/* Send some dummy clocks to receive CMD16 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				sd_cmd16_flag = 1;
			}

			r1.byte = (uint8_t)spi_pop_RX(spi_ctx);

			if (r1.byte == 0x00) {

				estado_sd = SD_SUCCESS;
			}

			break;

		case SD_SEND_CMD1:
			if (sd_cmd1_flag == 0) {
				/* APP_CMD
				 * The argument is 0x00000000 for the APP_CMD command. */
				sd_send_command(&spi_ctx[spi_inst], CMD1, 0x00000000);

				/* Send some dummy clocks to receive CMD1 response */
				sd_send_dummy_bytes(&spi_ctx[spi_inst], 8, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

				/* Set 1000ms timer for retrying CMD1 command */
				if (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)
					Timer_Start(TIMER_SD_INIT_ID, 1000, MSEG, timer_sd_init_handler);

				sd_cmd1_flag = 1;
			}

			r1.byte = (uint8_t)spi_pop_RX(spi_ctx);

			if ((r1.byte == 0x05) || (r1.byte == 0x09) || (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_STOP)) {
				/* Unknown Card */
				card_type = CT_NOT_DEFINED;

				estado_sd = SD_ERROR;
			}

			if ((r1.byte == 0x01) && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				sd_cmd1_flag = 0;;
				/* MMC not Initialiazed -> Retry */

				estado_sd = SD_SEND_CMD1;
			}

			if (r1.byte == 0x00 && (Timer_Activity_Get(TIMER_SD_INIT_ID) == kTIMER_RUN)) {
				Timer_Stop(TIMER_SD_INIT_ID);
				/* MMC Initialaized OK  */

				estado_sd = SD_SEND_CMD16;
			}

			break;

		case SD_SUCCESS:
			/* SD not initialized flag FALSE */
			sd_not_init = 0;

			break;

		case SD_ERROR:

			break;

		default:
			break;
	}

	return card_type;
}

void sd_send_data_packet(volatile spi_context_t ctx[], const uint8_t data[], const uint32_t size)
{
	const uint8_t data_token = SD_START_BLOCK_TOKEN_SINGLE_WRITE;
	const sd_crc16_t crc = { .word = SD_GENERIC_CRC16 };
	const uint32_t packet_size = sizeof data_token + size + sizeof crc;
	uint32_t i, j = 0;

	spi_master_mode_tx_data_t data_buffer[packet_size];

	sd_write_tx_buffer(&data_buffer[j++], data_token,
					   kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	for (i = 0; i < size; ++i)
		sd_write_tx_buffer(&data_buffer[j++], data[i], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);


	sd_write_tx_buffer(&data_buffer[j++], crc.array[0], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	sd_write_tx_buffer(&data_buffer[j++], crc.array[1], kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	spi_master_mode_send_data(ctx, data_buffer, packet_size);

}

/************************** MMC Write Block ***************************/
/* write a block of data based on the length that has been set
 * in the SET_BLOCKLEN command.
 * Send the WRITE_SINGLE_BLOCK command out first, check the
 * R1 response, then send the data start token(bit 0 to 0) followed by
 * the block of data. The test program sets the block length to 512
 * bytes. When the data write finishes, the response should come back
 * as 0xX5 bit 3 to 0 as 0101B, then another non-zero value indicating
 * that MMC card is in idle state again.
 *
 ***********************************************************************/

uint16_t sd_write_single_block(uint8_t spi_inst, uint32_t block_address, uint8_t data[], uint32_t size)
{
	static R1_response_union_t r1 = { .byte = 0x80, };
	static Data_resp_token_union_t data_resp_token = { .byte = 0x00 };
	static sd_single_write_states_e wr_states = SD_CMD24_SEND;

	if (wr_states == SD_CMD24_SEND) {
		/* Send CMD24(WRITE_SINGLE_BLOCK) to write data to SD card */
		if (sd_write_block(&spi_ctx[spi_inst], block_address, &r1))
			wr_states = SD_CMD24_RESP;
	}

	/* Read CMD24 response: if response OK, send packet */
	if (wr_states == SD_CMD24_RESP)
		if (r1.byte == SD_READY)
			wr_states = SD_SEND_PACKET;

	if (wr_states == SD_SEND_PACKET) {
		sd_send_data_packet(&spi_ctx[spi_inst], data, size);
		sd_send_dummy_bytes(&spi_ctx[spi_inst], 20, kSPI_SSEL0_ENABLED, kSPI_RX_NOT_IGNORE);

		wr_states = SD_PACKET_RESP;
	}

	/* Read Data Response Token response */
	if (wr_states == SD_PACKET_RESP)
		if (sd_receive_data_resp_token(&data_resp_token))
			wr_states = SD_WRITING_BUSY;

	/* Check if SD is busy -> is writing the data */
	if (wr_states == SD_WRITING_BUSY) {
		if (sd_check_busy_state() == 0) {
			sd_write_flag = 0;
			wr_states = SD_CMD24_SEND;
		}
	}

	return 1;
}


/************************** SD Read Block ****************************/
/*
 * Reads a 512 Byte block from the MMC
 * Send READ_SINGLE_BLOCK command first, wait for response come back
 * 0x00 followed by 0xFE. The call SPI_Receive() to read the data
 * block back followed by the checksum.
 *
 **********************************************************************/

uint16_t sd_read_single_block(uint8_t spi_inst, uint32_t block_address, uint8_t data_buffer[])
{
	static R1_response_union_t r1 = { .byte = 0x80, };
	static sd_single_read_states_e rd_states = SD_CMD17_SEND;

	/* Send CMD17(READ_SINGLE_BLOCK) to read data to SD card */
	if (rd_states == SD_CMD17_SEND)
		if (sd_read_block(&spi_ctx[spi_inst], block_address, &r1))
			rd_states = SD_CMD17_RESP;

	/* Read CMD17 response */
	if (rd_states == SD_CMD17_RESP) {
		if (r1.byte == SD_READY) {
			/* Send dummy bytes to receive the data block */
			sd_send_dummy_bytes_v2(&spi_ctx[spi_inst], 520, kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_RX_IGNORE);

			rd_states = SD_CMD17_SEND;
			sd_read_flag = 0;
		}
	}

	return 0;
}

uint16_t sd_read_multiple_block(uint8_t spi_inst, uint32_t block_address, uint8_t block_count, uint8_t data_buffer[])
{
	static R1_response_union_t r1 = { .byte = 0x80, };
	static sd_multi_read_states_e rd_states = SD_CMD18_SEND;

	/* Send CMD17(READ_SINGLE_BLOCK) to read data to SD card */
	if (rd_states == SD_CMD18_SEND)
		if (sd_read_multi_block(&spi_ctx[spi_inst], block_address, &r1))
			rd_states = SD_CMD18_RESP;

	if (rd_states == SD_CMD18_RESP)
		if (r1.byte == SD_READY)
			rd_states = SD_READ_MULTI_PACKETS;

	if (rd_states == SD_READ_MULTI_PACKETS) {

	}

	return 1;
}

/***************** MMC get response *******************/
/*
 * Repeatedly reads the MMC until we get the
 * response we want or timeout
 */

/***************** MMC wait for write finish *******************/
/*
 * Repeatedly reads the MMC until we get a non-zero value (after
 * a zero value) indicating the write has finished and card is no
 * longer busy.
 *
 */


/* -- Swapping ---------------------------------------------------------- */

uint16_t sd_swap_uint16(uint16_t value)
{
    value = ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
    return(value);
}

uint32_t sd_swap_uint32(uint32_t value)
{
    value = ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
    return(value);
}

/*
* Computation of CRC-7 polynomial (x^7 + x^3 + 1)
* Used in SD/MMC commands and responses.
*/

uint8_t sd_crc7 (const uint8_t *p_data, const uint32_t len)
{
	uint8_t crc = 0;
	uint8_t data;

	for (uint32_t i = 0; i < len; i++, p_data++) {
		data = *p_data;
		for (uint8_t i_bit = 0; i_bit < 8; i_bit++) {
			crc <<= 1;

			if ((data ^ crc) & 0x80)
				crc ^= 0x09;

			data <<= 1;
		}

		crc &= 0x7F;
	}

	return crc;
}


/*--- Timer handlers ------------------------------------------------------------ */

void timer_sd_init_handler(uint32_t value)
{
	if (sd_acmd41_timeout == 0) {
		sd_acmd41_timeout = 1;
	}

}

void timer_sd_cmd41_handler(uint32_t value)
{
	if (sd_acmd41_retry == 0) {
		sd_acmd41_retry = 1;
		Timer_Stop(TIMER_SD_CMD41_ID);
	}
}












/* ---- OLD FUNCTIONS ---------------------------------------------------------------------------------------------------------------------------- */

uint8_t mmc_load_dummy_OLD(volatile spi_context_t ctx[], uint16_t size)
{
	if (size > SPI_TX_BUFFER_SIZE)
		return 1;

	for (uint16_t i = 0; i < size; ++i)
		sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
		//mmc_write_tx_buffer(&ctx->bufferTx[ctx->txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	//mmc_write_tx_buffer(&spi_tx_buff[ctx->txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	spi_txInput %= SPI_TX_BUFFER_SIZE;
	//ctx->txInput %= SPI_TX_BUFFER_SIZE;

	return 0;
}

void mmc_load_command_OLD(volatile spi_context_t ctx[], uint8_t command, uint32_t argument)
{
	//spi_ctx[spi_ctx.instance].bufferTx[spi_ctx.txInput++]

	sd_command_union_t sd;

	sd.cmd.cmdIndex = command;
	sd.cmd.argument = argument;

	if (command == CMD0)
		sd.cmd.crc = 0x95;				/* Valid CRC for CMD0(0x00) */
	else if (command == CMD8)
		sd.cmd.crc = 0x87;				/* Valid CRC for CMD8(0x01AA) */
	else
		sd.cmd.crc = SPI_DUMMY_BYTE;	/* For other commands,checksum is no longer required in SPI mode. We always send 0xFF */

	/**
	 *  Load command packet
	 *  i = 0: Start + Command index
	 *  i = 1: Argument[31..24]
	 *  i = 2: Argument[23..16]
	 *  i = 3: Argument[15..8]
	 *  i = 4: Argument[7..0]
	 *  i = 5: CRC
	 */

	for (int i = 0; i < SD_CMD_SIZE; ++i)
		sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
		//mmc_write_tx_buffer(&ctx->bufferTx[ctx->txInput++], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	//mmc_write_tx_buffer(&ctx->bufferTx[ctx->txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	spi_txInput %= SPI_TX_BUFFER_SIZE;
	//ctx->txInput %= SPI_TX_BUFFER_SIZE;
}

void mmc_send_dummy_bytes_OLD(volatile spi_context_t ctx[], uint32_t size, spi_rx_ignore_en rx_ignore)
{
	//spi_master_mode_tx_data_t tx_dummy_byte[10 + 1];

	for (int i = 0; i < size; ++i)
		if (i < size - 1)
			sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);
		else
			sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, rx_ignore, kSPI_DATA_LENGTH_8_BIT);

	//mmc_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);

	//spi_master_mode_send_data(ctx, tx_dummy_byte, size + 1);

	spi_txInput %= SPI_TX_BUFFER_SIZE;
	spi_master_mode_tx_data(ctx->instance, &spi_tx_buff[spi_txOutput++]);

	//spi_tx_flag = 1;
	//spi_push_TX(0, spi_tx_buff[spi_txOutput++]);
}

void mmc_send_command_OLD(volatile spi_context_t ctx[], uint8_t command, uint32_t argument)
{
	sd_command_union_t sd;
	//spi_master_mode_tx_data_t tx_cmd[MMC_CMD_SIZE];

	/* Armo el comando */
	sd.cmd.cmdIndex = command;
	sd.cmd.argument = argument;

	if (command == CMD0)
		sd.cmd.crc = 0x95;				/* Valid CRC for CMD0(0x00) */
	else if (command == CMD8)
		sd.cmd.crc = 0x87;				/* Valid CRC for CMD8(0x01AA) */
	else
		sd.cmd.crc = SPI_DUMMY_BYTE;	/* For other commands,checksum is no longer required in SPI mode. We always send 0xFF */

	/**
	 *  Load command packet
	 *  i = 0: Start + Command index
	 *  i = 1: Argument[31..24]
	 *  i = 2: Argument[23..16]
	 *  i = 3: Argument[15..8]
	 *  i = 4: Argument[7..0]
	 *  i = 5: CRC
	 */

	for (size_t i = 0; i < SD_CMD_SIZE; ++i) {
		if ( i < (SD_CMD_SIZE - 1))
			sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_NOT_EOT, kSPI_NOT_EOF, kSPI_RX_IGNORE, kSPI_DATA_LENGTH_8_BIT);
		else
			sd_write_tx_buffer(&spi_tx_buff[spi_txInput++], sd.cmdArray[i], kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	}

	//mmc_write_tx_buffer(&spi_tx_buff[spi_txInput++], SPI_DUMMY_BYTE, kSPI_SSEL0_ENABLED, kSPI_EOT, kSPI_NOT_EOF, kSPI_RX_NOT_IGNORE, kSPI_DATA_LENGTH_8_BIT);
	//spi_master_mode_send_data(ctx, tx_cmd, MMC_CMD_SIZE);
	spi_txInput %= SPI_TX_BUFFER_SIZE;

	//spi_master_mode_tx_data(ctx->instance, spi_tx_buff);
	//spi_tx_flag = 1;
	//spi_push_TX(0, spi_tx_buff[spi_txOutput++]);


}








