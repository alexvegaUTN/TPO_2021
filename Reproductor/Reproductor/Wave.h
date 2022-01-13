#ifndef WAVE_H
#define WAVE_H

#include <QString>
#include "stdio.h"
#include <QFile>
#include <QDataStream>

typedef		unsigned int		uint32_t;
typedef		short unsigned int	uint16_t;
typedef		unsigned char		uint8_t ;
typedef		int					int32_t;
typedef		short int			int16_t;
typedef		signed char			int8_t;

typedef struct WAV_RIFF {
    /* chunk "riff" */
    char ChunkID[4];   /* "RIFF" */
    /* sub-chunk-size */
    uint32_t ChunkSize; /* 36 + Subchunk2Size */
    /* sub-chunk-data */
    char Format[4];    /* "WAVE" */
} RIFF_t;

typedef struct WAV_FMT {
    /* sub-chunk "fmt" */
    char Subchunk1ID[4];    /* "fmt " */
    /* sub-chunk-size */
    uint32_t Subchunk1Size; /* 16 for PCM */
    /* sub-chunk-data */
    uint16_t AudioFormat;   /* PCM = 1*/
    uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */
    uint32_t SampleRate;    /* 8000, 44100, etc. */
    uint32_t ByteRate;      /* = SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
    uint16_t BitsPerSample; /* 8bits, 16bits, etc. */
} FMT_t;

typedef struct WAV_data {
    /* sub-chunk "data" */
    char Subchunk2ID[4];   /* "data" */
    /* sub-chunk-size */
    uint32_t Subchunk2Size; /* data size */
    /* sub-chunk-data */
//    Data_block_t block;
} Data_t;

//typedef struct WAV_data_block {
//} Data_block_t;

typedef struct WAV_fotmat {
   RIFF_t riff;
   FMT_t fmt;
   Data_t data;
} Wave;


typedef union WAV {
    uint8_t Global_Data;

    struct WAVHeader {
      char ChunkID[4];          /*  4 bytes   */
      int32_t ChunkSize;        /*  4 bytes   */
      char Format[4];           /*  4 bytes  */

      char Subchunk1ID[4];      /*  4 bytes   */
      int32_t Subchunk1Size;    /*  4 bytes  */
      int16_t AudioFormat;      /*  2 bytes   */
      int16_t NumChannels;      /*  2 bytes   */
      int32_t SampleRate;       /*  4 bytes   */
      int32_t ByteRate;         /*  4 bytes   */
      int16_t BlockAlign;       /*  2 bytes   */
      int16_t BitsPerSample;    /*  2 bytes   */

      char Subchunk2ID[4];      /*  4 bytes   */
      int32_t Subchunk2Size;    /*  4 bytes   */
    } WAVHeader;

}Wave_t;

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    char         RIFF[4];        // RIFF Header Magic header   uint8_t
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

//----------------- funciones --------------------------------------------
//void infoWav( QString paths );
int getFileSize(FILE* inFile);

#endif // WAVE_H
