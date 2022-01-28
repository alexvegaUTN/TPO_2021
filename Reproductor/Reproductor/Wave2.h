#ifndef WAVE2_H
#define WAVE2_H

#include <stdio.h>
#include <stdint.h>

#define FILE_MAX_PACKET_SIZE			128

/* --- Indica si es el bloque final de envío del FW --- */

typedef enum {
    PACKETS_CONTINUE = 0x00,
    LAST_PACKET = 0x01
}SendFwEndPacket;

/* Send Data -------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma pack(push,1)
typedef struct {
    uint16_t dataLenght;								// Longitud del paquete de Data
    uint16_t blockNumber;								// Número de bloque de datos de Firmware.
    uint8_t endPacket;									// Indica si es el bloque final de envío del FW: 0x00 = Continúan mas paquetes, 0x01 = Último paquete
    uint8_t lenDataPacket;								// Largo de datos del paquete. Máximo 128 bytes.
    uint8_t dataPacket[FILE_MAX_PACKET_SIZE];           // Paquete de datos del FW.
}SendFilePacket_t;

typedef struct {

}Data_;

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

typedef union{
    uint8_t array[sizeof(uint32_t)];
    uint32_t dword;
}buffer_u32_union_t;

typedef union{
    uint8_t array[sizeof(uint16_t)];
    uint16_t word;
}buffer_u16_union_t;

#pragma pack(pop)

FILE * fileManager_GetFWFile(const char * fileName);
uint32_t fileManager_GetFWFileSize(FILE ** p_file, const char fileName[]); //determina el tamanio del archivo
uint32_t fileManager_GetFWBlockQuantity(uint32_t fwLen);//determina la cantidad de bloques para el archivo
void GetWavHeader(FILE ** p_file, const char fileName[] ,wav_hdr *p_wavHeader);
uint16_t SendFWDataInput(SendFilePacket_t * p_SendFWData, FILE **p_file, const char fileName[], uint32_t fileLenght, uint32_t blockQty, uint32_t blockNumber, uint32_t headerLength);
uint16_t fileManager_SwapBytes16(uint16_t value);


#endif // WAVE2_H
