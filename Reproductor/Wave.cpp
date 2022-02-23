//#include "Wave.h"
#include "Wave2.h"
#include <QFile>
#include <QDataStream>
#include <QThread>
#include <QVector>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

void MainWindow::infoWav(QString paths)
{
    uint32_t fileLenght;
    uint32_t wavHdrLenght;
    uint16_t blockQty;
    wav_hdr wavHeader;
    QString AudioData;
    uint8_t audio_data;
    FILE * p_file;
    SendFilePacket_t filePacket;

    QByteArray path = paths.toLocal8Bit();

    QVector<uint8_t>package;
    QVector<uint8_t>message;

    char fileName[100];
    memcpy(fileName,path,sizeof (fileName));
    //const char fileName[] = "C:/Users/alexx/Documents/GitHub/TPO_2021/Reproductor/Reproductor/test.wav"; //C:\Users\alexx\Documents\GitHub\TPO_2021\Reproductor\Reproductor\test.wav

    /* Verifico existencia del archivo y que pueda ser abierto*/
    p_file = fileManager_GetWavFile(fileName);

    /* Calculo longitud del Fw y la cantidad de bloques en que debe ser dividido */
    fileLenght = fileManager_GetWavFileSize(&p_file, fileName);
    wavHdrLenght = sizeof (wavHeader);
    fileLenght = fileLenght - wavHdrLenght ;
    blockQty = fileManager_GetWavBlockQuantity(fileLenght);
    printf(" longitud del Fw: %d y la cantidad de bloques en que debe ser dividido: %d\n",fileLenght,blockQty);

    if (p_file) {
        //static unsigned int estado_infoWAV = __RRIF;
        /* Capturo el header cabecera del archivo wave*/
        GetWavHeader(&p_file, fileName,&wavHeader);

        switch ( estado_infoWAV )
        {
            case __RRIF:

                    /* Verifico los datos y preparo los datos a enviar por el puerto */
                    if( wavHeader.RIFF[0] == 'R' && wavHeader.RIFF[1] == 'I' && wavHeader.RIFF[2] == 'F' && wavHeader.RIFF[3] == 'F' )
                    {
                        buffer_u32_union_t aux_u32_chunkSize;
                        aux_u32_chunkSize.dword = wavHeader.ChunkSize;
                        audio_data = wavHeader.RIFF[0];

                        message.append('$');
                        message.append(audio_data);
                        for (size_t i = 0; i < sizeof wavHeader.RIFF; ++i)
                            message.append(wavHeader.RIFF[i]);
                        for (size_t i = 0; i < sizeof aux_u32_chunkSize.dword; ++i)
                            message.append(aux_u32_chunkSize.array[i]);
                        for (size_t i = 0; i < sizeof wavHeader.WAVE; ++i)
                            message.append(wavHeader.WAVE[i]);
                        message.append('#');

                        Enviar_v3(reinterpret_cast<const char *>(message.data()), message.length());
                        message.clear();
                    }
                    estado_infoWAV = __FMT;
                    break;

            case __FMT:

                    if( wavHeader.fmt[0] == 'f' && wavHeader.fmt[1] == 'm' && wavHeader.fmt[2] == 't' )
                    {
                        buffer_u32_union_t aux_u32_subChunk1_size;
                        buffer_u16_union_t aux_u16_audioFormat;
                        buffer_u16_union_t aux_u16_numOfChan;
                        buffer_u32_union_t aux_u32_samplesPerSec;
                        buffer_u32_union_t aux_u32_bytesPerSec;
                        buffer_u16_union_t aux_u16_blockAlign;
                        buffer_u16_union_t aux_u16_bitsPerSmple;

                        aux_u32_subChunk1_size.dword = wavHeader.Subchunk1Size;
                        aux_u16_audioFormat.word = wavHeader.AudioFormat;
                        aux_u16_numOfChan.word = wavHeader.NumOfChan;
                        aux_u32_samplesPerSec.dword = wavHeader.SamplesPerSec;
                        aux_u32_bytesPerSec.dword = wavHeader.bytesPerSec;
                        aux_u16_blockAlign.word = wavHeader.blockAlign;
                        aux_u16_bitsPerSmple.word = wavHeader.bitsPerSample;

                        audio_data = wavHeader.fmt[0];

                        message.append('$');
                        message.append(audio_data);
                        for (size_t i = 0; i < sizeof wavHeader.fmt; ++i) {
                            message.append(wavHeader.fmt[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u32_subChunk1_size.dword; ++i) {
                            message.append(aux_u32_subChunk1_size.array[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u16_audioFormat.word; ++i) {
                            message.append(aux_u16_audioFormat.array[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u16_numOfChan.word; ++i) {
                            message.append(aux_u16_numOfChan.array[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u32_samplesPerSec.dword; ++i) {
                            message.append(aux_u32_samplesPerSec.array[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u32_bytesPerSec.dword; ++i) {
                            message.append(aux_u32_bytesPerSec.array[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u16_blockAlign.word; ++i) {
                            message.append(aux_u16_blockAlign.array[i]);
                        }
                        for (size_t i = 0; i < sizeof aux_u16_bitsPerSmple.word; ++i) {
                            message.append(aux_u16_bitsPerSmple.array[i]);
                        }
                        message.append('#');

                        Enviar_v3(reinterpret_cast<const char *>(message.data()), message.length());
                        message.clear();
                    }
                    estado_infoWAV = __DATA;
                    break;
            case __DATA:

                    if( wavHeader.Subchunk2ID[0] == 'd' && wavHeader.Subchunk2ID[1] == 'a' && wavHeader.Subchunk2ID[2] == 't' && wavHeader.Subchunk2ID[3] == 'a' )
                    {
                      buffer_u32_union_t aux_u32_subChunk2_size;
                      aux_u32_subChunk2_size.dword = wavHeader.Subchunk2Size;

                      AudioData = wavHeader.Subchunk2ID[0];
                      audio_data = wavHeader.Subchunk2ID[0];

                      message.append('$');
                      message.append(audio_data);
                      for (size_t i = 0; i < sizeof wavHeader.Subchunk2ID; ++i) {
                          message.append(wavHeader.Subchunk2ID[i]);
                      }
                      for (size_t i = 0; i < sizeof aux_u32_subChunk2_size.dword; ++i) {
                          message.append(aux_u32_subChunk2_size.array[i]);
                      }
                      message.append('#');

                      Enviar_v3(reinterpret_cast<const char *>(message.data()), message.length());
                      message.clear();
                    }
                    estado_infoWAV = __BLOQUE;
                    break;
            case __BLOQUE:
                        //static uint32_t block_idx = 0;
//                        if(block_idx < blockQty){             //for ( static uint32_t i = 0; i < blockQty; i++) {
                        /* --- Esta función va leyendo paquetes de archivo y los guarda en la estructura flePacket --- */
                        fileManager_SendWavDataInput(&filePacket, &p_file, fileName, fileLenght, blockQty, block_idx, wavHdrLenght);
                        printf("OK");

                        /* --- Acá debería tomar la estructura filePacket hacer el envío de la data --- */
                        audio_data = wavHeader.Subchunk2ID[0] - 0x20;

                        for (uint16_t j = 0; j < FILE_MAX_PACKET_SIZE; j++) {
                            package.append(filePacket.dataPacket[j]);
                            //printf(" paquete[%d]: %x ",j,filePacket.dataPacket[j]);
                        }

                        buffer_u16_union_t aux_u16_blockNumber;
                        aux_u16_blockNumber.word = (uint16_t)block_idx;   //filePacket.blockNumber;

                        buffer_u16_union_t aux_u16_lenDataPacket;
                        aux_u16_lenDataPacket.word = FILE_MAX_PACKET_SIZE;   //filePacket.blockNumber;

                        message.append('$');
                        message.append(audio_data);
                        for (size_t k = 0; k < sizeof aux_u16_blockNumber.word; ++k) {
                            message.append(aux_u16_blockNumber.array[k]);
                        }

                        message.append(filePacket.endPacket);
                        for (size_t k = 0; k < sizeof aux_u16_lenDataPacket.word; ++k) {
                            message.append(aux_u16_lenDataPacket.array[k]);
                        }

                        message.append(package);
                        message.append('#');

                        Enviar_v3(reinterpret_cast<const char *>(message.data()), message.length());

                        //printf(" paquete[%d]",block_idx);

                        message.clear();
                        package.clear();

                        estado_infoWAV = __BLOQUE;
                        block_idx++;

                        if( block_idx == blockQty )
                        {
                            block_idx = 0;
                            estado_infoWAV = __ESPERA;
                        }
//                    }


                    break;

            case __ESPERA:

                    break;

            default:
                        estado_infoWAV = __RRIF;
                    break;
        }

    }
}

FILE *MainWindow::fileManager_GetWavFile(const char *fileName)
{
    FILE * p_file = NULL;

    /* Verifico que el nombre de archivo no sea nulo */
    if (fileName != NULL) {
        /* Abro el archivo */
        p_file = fopen(fileName,"rb");

        /* --- Verifico que el archivo se pueda abrir correctamente --- */
            if (!p_file) {
                printf("No se pudo ABRIR el archivo, ERROR AL ABRIR\n");
            }
    }

    return (p_file);
}

uint32_t MainWindow::fileManager_GetWavFileSize(FILE **p_file, const char fileName[])
{
    uint32_t fwSize = 0;

    /* Verifico que el nombre de archivo sea válido */
    if (fileName != NULL){
        /* Abro archivo */
        *p_file = fopen(fileName, "rb");
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

uint32_t MainWindow::fileManager_GetWavBlockQuantity(uint32_t fwLen)
{
    uint32_t blockQty;

        blockQty = (uint32_t)(fwLen / FILE_MAX_PACKET_SIZE);

        if (fwLen % FILE_MAX_PACKET_SIZE) {
            blockQty++;
        }

        return (blockQty);
}

void MainWindow::GetWavHeader(FILE **p_file, const char fileName[], wav_hdr *p_wavHeader)
{
    if ((p_file != NULL) && (fileName != NULL)) {
        *p_file = fopen(fileName, "rb");

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

uint16_t MainWindow::fileManager_SendWavDataInput(SendFilePacket_t *p_SendWavData, FILE **p_file, const char fileName[], uint32_t fileLenght, uint32_t blockQty, uint32_t blockNumber, uint32_t headerLength)
{
    uint16_t lastBlock = blockQty - 1;
    int i;

    if ((p_file != NULL) && (fileName != NULL)) {
        *p_file = fopen(fileName, "rb");

        /* --- Verifico que el archivo se pueda abrir correctamente --- */
        if (*p_file) {
            p_SendWavData->blockNumber = blockNumber;
             printf(" blockNumber: %u\n",p_SendWavData->blockNumber);

            if (blockNumber < lastBlock) {
                p_SendWavData->endPacket = PACKETS_CONTINUE;
                p_SendWavData->lenDataPacket = FILE_MAX_PACKET_SIZE;
                printf(" Musica endPacket: %u\n",p_SendWavData->endPacket);
                printf(" Musica lenDataPacket: %u\n",p_SendWavData->lenDataPacket);
            }
            else {
                if (blockNumber == lastBlock) {
                    p_SendWavData->endPacket = LAST_PACKET;
                    if (fileLenght % FILE_MAX_PACKET_SIZE) {
                        p_SendWavData->lenDataPacket = fileLenght % FILE_MAX_PACKET_SIZE;
                        printf(" Musica endPacket: %u\n",p_SendWavData->endPacket);
                        printf(" Musica lenDataPacket: %u\n",p_SendWavData->lenDataPacket);
                    }
                    else {
                        p_SendWavData->lenDataPacket = FILE_MAX_PACKET_SIZE;
                        printf(" Musica endPacket: %u\n",p_SendWavData->endPacket);
                        printf(" Musica lenDataPacket: %u\n",p_SendWavData->lenDataPacket);
                    }
                }
            }
            if (*p_file) {
                fseek(*p_file, (long)( headerLength + (blockNumber * FILE_MAX_PACKET_SIZE) ), SEEK_SET);
                fread(p_SendWavData->dataPacket,sizeof *p_SendWavData->dataPacket, 1, *p_file);//fread(p_SendFWData->dataPacket,sizeof *p_SendFWData->dataPacket, p_SendFWData->lenDataPacket, *p_file);
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

    return (p_SendWavData->blockNumber);
}


void MainWindow::Enviar(QString Mensaje)
{
     QByteArray MSJ;
     MSJ.append(Mensaje);
     port->write(MSJ);
     MSJ.clear();
}

void MainWindow::Enviar_v2(QByteArray &msj)
{
    port->write(msj);
}

void MainWindow::Enviar_v3(const char * msj, qint64 length)
{
    port->write(msj, length);
}

/*
 * wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;

    QByteArray path = paths.toLocal8Bit();
    const char* filePath= path.data();
    FILE* wavFile = fopen(filePath, "r");

    QString AudioData;
    QByteArray MSJ;

    if (wavFile == nullptr)
    {
        QMessageBox::critical(this, "Error en la APERTURA DEL ARCHIVO ", paths);
    }
    //Read the header
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    ui->textEdit_Datos_Recibidos->setText("Header Read " + QString::number( bytesRead) + "bytes. (estructura)");
    if (bytesRead > 0)
    {
        //Read the data
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?

        int BUFFER_SIZE = wavHeader.Subchunk2Size;
        char * buffer = new char[BUFFER_SIZE];
        while ((bytesRead = fread(buffer, sizeof (buffer[0]), BUFFER_SIZE , wavFile)) > 0)
        {

            ui->textEdit_Datos_Recibidos->append("Los bytesTead: " + QString::number( bytesRead) + " bytes del buffer: " + buffer[1] );
        }
        filelength = getFileSize(wavFile);
        QString aux(buffer);

        ui->textEdit_Datos_Recibidos->append("File is: " + QString::number( filelength) + " bytes.");
        ui->textEdit_Datos_Recibidos->append("RIFF header: " + QString::number(0) + wavHeader.RIFF[0] + wavHeader.RIFF[1] + wavHeader.RIFF[2] + wavHeader.RIFF[3] );
        ui->textEdit_Datos_Recibidos->append("Data size: " + QString::number( wavHeader.ChunkSize) );
        ui->textEdit_Datos_Recibidos->append("WAVE header: " + QString::number(0) + wavHeader.WAVE[0] + wavHeader.WAVE[1] + wavHeader.WAVE[2] + wavHeader.WAVE[3] );

        ui->textEdit_Datos_Recibidos->append("FMT: " + QString::number(0)+ wavHeader.fmt[0] + wavHeader.fmt[1] + wavHeader.fmt[2] + wavHeader.fmt[3] );
        ui->textEdit_Datos_Recibidos->append("Subchunk1Size: " + QString::number( wavHeader.Subchunk1Size) );
        ui->textEdit_Datos_Recibidos->append("Audio Format: " + QString::number( wavHeader.AudioFormat) );

        ui->textEdit_Datos_Recibidos->append("Number of channels: " + QString::number( wavHeader.NumOfChan ) );
        ui->textEdit_Datos_Recibidos->append("Sampling Rate: " + QString::number( wavHeader.SamplesPerSec ) );
        ui->textEdit_Datos_Recibidos->append("Number of bytes per second: " + QString::number( wavHeader.bytesPerSec ) );

        ui->textEdit_Datos_Recibidos->append("Block align: " + QString::number( wavHeader.blockAlign ) );
        ui->textEdit_Datos_Recibidos->append("Number of bits used: " + QString::number( wavHeader.bitsPerSample ) );
        ui->textEdit_Datos_Recibidos->append("Data string: " + QString::number(0) + wavHeader.Subchunk2ID[0] + wavHeader.Subchunk2ID[1] + wavHeader.Subchunk2ID[2] + wavHeader.Subchunk2ID[3] );
        ui->textEdit_Datos_Recibidos->append("Data length: " + QString::number( wavHeader.Subchunk2Size ) );
        ui->textEdit_Datos_Recibidos->append("Data Musica: " + aux );

        if( wavHeader.RIFF[0] == 'R' && wavHeader.RIFF[1] == 'I' && wavHeader.RIFF[2] == 'F' && wavHeader.RIFF[3] == 'F' )//&& wavHeader.RIFF[1] == 'I' && wavHeader.RIFF[2] == 'F' && wavHeader.RIFF[3] == 'F'
        {
            AudioData = wavHeader.RIFF[0]  ;
            MSJ.append("$" + AudioData + wavHeader.RIFF[0] + wavHeader.RIFF[1] + wavHeader.RIFF[2] + wavHeader.RIFF[3] + QString::number( wavHeader.ChunkSize) + wavHeader.WAVE[0] + wavHeader.WAVE[1] + wavHeader.WAVE[2] + wavHeader.WAVE[3] + "#" );
            Enviar(MSJ);

        }
        if( wavHeader.fmt[0] == 'f' && wavHeader.fmt[1] == 'm' && wavHeader.fmt[2] == 't' )
        {
            AudioData = wavHeader.fmt[0]  ;
            MSJ.append("$" + AudioData + wavHeader.fmt[0] + wavHeader.fmt[1] + wavHeader.fmt[2] + wavHeader.fmt[3] + QString::number( wavHeader.ChunkSize) + wavHeader.Subchunk1Size + wavHeader.AudioFormat + wavHeader.NumOfChan + wavHeader.SamplesPerSec + wavHeader.bytesPerSec + wavHeader.blockAlign + wavHeader.bitsPerSample  + "#" ); //+ wavHeader.Subchunk1Size + wavHeader.AudioFormat + wavHeader.NumOfChan + wavHeader.SamplesPerSec + wavHeader.bytesPerSec + wavHeader.blockAlign + wavHeader.bitsPerSample
            Enviar(MSJ);
        }
        if( wavHeader.Subchunk2ID[0] == 'd' && wavHeader.Subchunk2ID[1] == 'a' && wavHeader.Subchunk2ID[2] == 't' && wavHeader.Subchunk2ID[3] == 'a' )
          {
            AudioData = wavHeader.Subchunk2ID[0];
            MSJ.append("$" + AudioData + wavHeader.Subchunk2ID[0] + wavHeader.Subchunk2ID[1] + wavHeader.Subchunk2ID[2] + wavHeader.Subchunk2ID[3] + QString::number( wavHeader.Subchunk2Size) + "#" );
            Enviar(MSJ);
            // envio los datos primera (prueba!)
            MSJ.clear();
            MSJ.append("$" + AudioData + buffer[0] + "#" );
          }


        delete [] buffer;
        buffer = nullptr;

    }
    fclose(wavFile);




int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

 *
*/
