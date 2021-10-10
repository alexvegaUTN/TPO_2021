/*******************************************************************************************************************************//**
 *
 * @file		DR_SPI.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		16 sep. 2021
 * @author		ALEXIS VEGA
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

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/

typedef signed char 	int8_t;
typedef unsigned char 	uint8_t;
typedef short 			int16_t;
typedef unsigned short 	uint16_t;
typedef int 			int32_t;
typedef unsigned int 	uint32_t;

#define  __I    volatile const 		 /*Defines 'read only' permission */
#define  __O    volatile             /*Defines 'write only' permission */
#define  __IO   volatile             /*Defines 'read / write' permission */


#define		SPI_DIR_BASE0			(( __IO uint32_t  *) 0x40058000UL)
#define		SPI_DIR_BASE1			(( __IO uint32_t  *) 0x4005C000UL)

#define		SYSAHBCLKCTRL0			(*( __IO uint32_t  *) 0x40048080UL)
#define		PRESETCTRL0				(*( __IO uint32_t  *) 0x40048088UL)

#define		SYS_CLK_CTRL_SPI0		(11)
#define		SYS_CLK_CTRL_SPI1		(12)

#define 	SPI0_RST_N				(11)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

typedef struct {
	uint32_t ENABLE  	:1;
	uint32_t RESERVED1  	:1;
	uint32_t MASTER  	:1;
	uint32_t LSBF  		:1;
	uint32_t CPHA 		:1;
	uint32_t CPOL 		:1;
	uint32_t RESERVADO2 	:1;
	uint32_t LOOP 		:1;
	uint32_t SPOL0 		:1;
	uint32_t SPOL1 		:1;
	uint32_t SPOL2 		:1;
	uint32_t SPOL3 		:1;
	uint32_t RESERVADO3 	:20;
}SPICFG_reg_t;

typedef struct {
	uint32_t PRE_DELAY 			:4;
	uint32_t POST_DELAY 		:4;
	uint32_t FRAME_DELAY 		:4;
	uint32_t TRANSFER_DELAY 	:4;
	uint32_t RESERVADO1			:16;
}SPIDLY_reg_t;

typedef struct {
	uint32_t RXRDY 				:1;
	uint32_t TXRDY 				:1;
	uint32_t RXOV 				:1;
	uint32_t TXUR 				:1;
	uint32_t SSA				:1;
	uint32_t SSD				:1;
	uint32_t STALLED			:1;
	uint32_t ENDTRANSFER		:1;
	uint32_t MSTIDLE			:1;
	uint32_t RESERVADO			:23;
}SPISTAT_reg_t;

typedef struct {
	uint32_t RXRDYEN 			:1;
	uint32_t TXRDYEN 			:1;
	uint32_t RXOVEN				:1;
	uint32_t TXUREN				:1;
	uint32_t SSAEN				:1;
	uint32_t SSDEN				:1;
	uint32_t RESERVADO			:26;
}SPIINTENSET_reg_t;

typedef struct {
	uint32_t RXRDYEN 			:1;
	uint32_t TXRDYEN 			:1;
	uint32_t RXOVEN				:1;
	uint32_t TXUREN				:1;
	uint32_t SSAEN				:1;
	uint32_t SSDEN				:1;
	uint32_t RESERVADO			:26;
}SPIINTENCLR_reg_t;

typedef struct {
	uint32_t RXDAT 				:16;
	uint32_t RXSSEL0_N 			:1;
	uint32_t RXSSEL1_N			:1;
	uint32_t RXSSEL2_N			:1;
	uint32_t RXSSEL3_N			:1;
	uint32_t SOT					:1;
	uint32_t RESERVADO			:11;
}SPIRXDAT_reg_t;

typedef struct {
	uint32_t TXDAT 				:16;
	uint32_t TXSSEL0_N 			:1;
	uint32_t TXSSEL1_N			:1;
	uint32_t TXSSEL2_N			:1;
	uint32_t TXSSEL3_N			:1;
	uint32_t EOT					:1;
	uint32_t EOF					:1;
	uint32_t RXIGNORE			:1;
	uint32_t RESERVED			:1;
	uint32_t LEN					:4;
	uint32_t RESERVADO			:4;
}SPITXDATCTL_reg_t;

typedef struct {
	uint32_t DATA 				:16;
	uint32_t RESERVADO			:16;
}SPITXDAT_reg_t;

typedef struct {
	uint32_t RESERVADO			:16;
	uint32_t TXSSEL0_N			:1;
	uint32_t TXSSEL1_N			:1;
	uint32_t TXSSEL2_N			:1;
	uint32_t TXSSEL3_N			:1;
	uint32_t EOT					:1;
	uint32_t EOF					:1;
	uint32_t RXIGNORE			:1;
	uint32_t RESERVADO1			:1;
	uint32_t LEN					:4;
	uint32_t RESERVADO2			:4;
}SPITXCTL_reg_t;

typedef struct {
	uint32_t DIVVAL 				:16;
	uint32_t RESERVADO			:16;
}SPIDIV_reg_t;

typedef struct {
	uint32_t RXRDY 		:1;
	uint32_t TXRDY 		:1;
	uint32_t RXOV 		:1;
	uint32_t TXUR 		:1;
	uint32_t SSA			:1;
	uint32_t SSD			:1;
	uint32_t RESERVADO	:26;
}SPIINTSTAT_reg_t;

/** SPI - Register Layout Typedef */
typedef struct {
  __IO SPICFG_reg_t CFG;                               /**< SPI Configuration register, offset: 0x0 */
  __IO SPIDLY_reg_t DLY;                               /**< SPI Delay register, offset: 0x4 */
  __IO SPISTAT_reg_t STAT;                              /**< SPI Status. Some status flags can be cleared by writing a 1 to that bit position, offset: 0x8 */
  __IO SPIINTENSET_reg_t INTENSET;                          /**< SPI Interrupt Enable read and Set. A complete value may be read from this register. Writing a 1 to any implemented bit position causes that bit to be set., offset: 0xC */
  __O  SPIINTENCLR_reg_t INTENCLR;                          /**< SPI Interrupt Enable Clear. Writing a 1 to any implemented bit position causes the corresponding bit in INTENSET to be cleared., offset: 0x10 */
  __I  SPIRXDAT_reg_t RXDAT;                             /**< SPI Receive Data, offset: 0x14 */
  __IO SPITXDATCTL_reg_t TXDATCTL;                          /**< SPI Transmit Data with Control, offset: 0x18 */
  __IO SPITXDAT_reg_t TXDAT;                             /**< SPI Transmit Data., offset: 0x1C */
  __IO SPITXCTL_reg_t TXCTL;                             /**< SPI Transmit Control, offset: 0x20 */
  __IO SPIDIV_reg_t DIV;                               /**< SPI clock Divider, offset: 0x24 */
  __I  SPIINTSTAT_reg_t INTSTAT;                           /**< SPI Interrupt Status, offset: 0x28 */
} SPI_per_t;

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/
// extern tipo nombreVariable;

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/

void SPI_Init(void);


#endif /* DR_SPI_H_ */
