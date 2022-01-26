#include "Wave2.h"

FILE * fileManager_GetFWFile(const char * fileName)
{
    FILE * p_file = NULL;

    /* Verifico que el nombre de archivo no sea nulo */
    if (fileName != NULL) {
        /* Abro el archivo */
        p_file = fopen(fileName,"r");

        /* --- Verifico que el archivo se pueda abrir correctamente --- */
            if (!p_file) {
                printf("No se pudo ABRIR el archivo, ERROR AL ABRIR\n");
            }
    }

    return (p_file);
}

uint32_t fileManager_GetFWFileSize(FILE ** p_file, const char fileName[])
{
    uint32_t fwSize = 0;

    /* Verifico que el nombre de archivo sea válido */
    if (fileName != NULL){
        /* Abro archivo */
        *p_file = fopen(fileName, "r");
        printf("EL path Name: %s\n",fileName);

        /* Determino el tamaño del archivo */
        if (*p_file != NULL) {
            fseek(*p_file, 0L, SEEK_END);
            fwSize = (uint32_t)ftell(*p_file);
            rewind(*p_file);
            fclose(*p_file);
        }
    }
    return (fwSize);
}

uint32_t fileManager_GetFWBlockQuantity(uint32_t fwLen)
{
    uint32_t blockQty;

        blockQty = (uint32_t)(fwLen / FILE_MAX_PACKET_SIZE);

        if (fwLen % FILE_MAX_PACKET_SIZE) {
            blockQty++;
        }

        return (blockQty);
}

uint16_t fileManager_SendFWDataInput(SendFilePacket_t * p_SendFWData, FILE **p_file, const char fileName[], uint32_t fwLenght, uint32_t blockQty, uint32_t blockNumber, uint32_t wavLenght)
{
       uint16_t lastBlock = blockQty - 1;
       int i;

       if ((p_file != NULL) && (fileName != NULL)) {
           *p_file = fopen(fileName, "r");

           /* --- Verifico que el archivo se pueda abrir correctamente --- */
           if (*p_file) {
               p_SendFWData->blockNumber = blockNumber;
                printf(" blockNumber: %u\n",p_SendFWData->blockNumber);

               if (blockNumber < lastBlock) {
                   p_SendFWData->endPacket = PACKETS_CONTINUE;
                   p_SendFWData->lenDataPacket = FILE_MAX_PACKET_SIZE;
                   printf(" Musica endPacket: %u\n",p_SendFWData->endPacket);
                   printf(" Musica lenDataPacket: %u\n",p_SendFWData->lenDataPacket);
               }
               else {
                   if (blockNumber == lastBlock) {
                       p_SendFWData->endPacket = LAST_PACKET;
                       if (fwLenght % FILE_MAX_PACKET_SIZE) {
                           p_SendFWData->lenDataPacket = fwLenght % FILE_MAX_PACKET_SIZE;
                           printf(" Musica endPacket: %u\n",p_SendFWData->endPacket);
                           printf(" Musica lenDataPacket: %u\n",p_SendFWData->lenDataPacket);
                       }
                       else {
                           p_SendFWData->lenDataPacket = FILE_MAX_PACKET_SIZE;
                           printf(" Musica endPacket: %u\n",p_SendFWData->endPacket);
                           printf(" Musica lenDataPacket: %u\n",p_SendFWData->lenDataPacket);
                       }
                   }
               }
               if (*p_file) {
                   fseek(*p_file, (long)( wavLenght + (blockNumber * FILE_MAX_PACKET_SIZE) ), SEEK_SET);
                   fread(p_SendFWData->dataPacket,sizeof *p_SendFWData->dataPacket, 1, *p_file);//fread(p_SendFWData->dataPacket,sizeof *p_SendFWData->dataPacket, p_SendFWData->lenDataPacket, *p_file);
                   for ( i =0; i <= FILE_MAX_PACKET_SIZE ; i++) {
                       //printf(" Musica dataPacket: %u\n",p_SendFWData->dataPacket[i]);
                   }

                   rewind(*p_file);
                   fclose(*p_file);
               }
           }
           else
           {
               printf(" fileManager_SendFWDataInput NO pudo abrir el archivo :\n");
           }
       }

       return (p_SendFWData->blockNumber);
}

uint16_t fileManager_SwapBytes16(uint16_t value)
{
    value = ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
    return(value);
}

void fileManager_GetWavHeader(FILE **p_file, const char fileName[], wav_hdr *p_wavHeader)
{
    if ((p_file != NULL) && (fileName != NULL)) {
        *p_file = fopen(fileName, "r");

        /* --- Verifico que el archivo se pueda abrir correctamente --- */
        if (*p_file) {
            fread(p_wavHeader, sizeof *p_wavHeader, 1, *p_file);

            printf("RIFF header: %c%c%c%c\n",p_wavHeader->RIFF[0],p_wavHeader->RIFF[1],p_wavHeader->RIFF[2],p_wavHeader->RIFF[3]);
            printf("Data size: %u\n",p_wavHeader->ChunkSize);
            printf("WAVE header: %c%c%c%c\n",p_wavHeader->WAVE[0],p_wavHeader->WAVE[1],p_wavHeader->WAVE[2],p_wavHeader->WAVE[3]);
            printf("FMT header: %c%c%c%c\n",p_wavHeader->fmt[0],p_wavHeader->fmt[1],p_wavHeader->fmt[2],p_wavHeader->fmt[3]);

        }
    }

}
