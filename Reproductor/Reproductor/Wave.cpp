//#include "Wave.h"
#include "Wave2.h"
#include <QFile>
#include <QDataStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

void MainWindow::infoWav(QString paths)
{
    uint32_t fwLenght;
    uint32_t wavLenght;
    uint16_t blockQty;
    wav_hdr wavHeader;
    QString AudioData;
    FILE * p_file;
    SendFilePacket_t filePacket;

    QByteArray path = paths.toLocal8Bit();
    QByteArray Paquete;
    QByteArray MSJ;

    char fileName[100];
    memcpy(fileName,path,sizeof (fileName));

    //const char fileName[] = "C:/Users/alexx/Documents/GitHub/TPO_2021/Reproductor/Reproductor/test.wav"; //C:\Users\alexx\Documents\GitHub\TPO_2021\Reproductor\Reproductor\test.wav

    /* Verifico existencia del archivo y que pueda ser abierto*/
    p_file = fileManager_GetFWFile(fileName);

    if (p_file) {
        /* Calculo longitud del Fw y la cantidad de bloques en que debe ser dividido */
        fwLenght = fileManager_GetFWFileSize(&p_file, fileName);
        wavLenght = sizeof (wavHeader);
        fwLenght = fwLenght - wavLenght ;
        blockQty = fileManager_GetFWBlockQuantity(fwLenght);
        printf(" longitud del Fw: %d y la cantidad de bloques en que debe ser dividido: %d\n",fwLenght,blockQty);

        /* Capturo el header cabecera del archivo wave*/
        GetWavHeader(&p_file, fileName,&wavHeader);

        /* Verifico los datos y preparo los datos a enviar por el puerto */
        if( wavHeader.RIFF[0] == 'R' && wavHeader.RIFF[1] == 'I' && wavHeader.RIFF[2] == 'F' && wavHeader.RIFF[3] == 'F' )
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
          }

        for (uint32_t i = 0; i < blockQty; i++) {

            /* --- Esta función va leyendo paquetes de archivo y los guarda en la estructura flePacket --- */
              fileManager_SendFWDataInput(&filePacket, &p_file, fileName, fwLenght, blockQty, i, wavLenght);
               printf("OK");
            /* --- Acá debería tomar la estructura filePacket hacer el envío de la data --- */
               AudioData = (int)i;
               for (uint32_t j = 0; j <= FILE_MAX_PACKET_SIZE; j++) {
                   //if(j==0) Paquete.append(filePacket.dataPacket[j]);
                   Paquete.append(filePacket.dataPacket[j]);
               }
               MSJ.append("$" + AudioData + filePacket.blockNumber + filePacket.endPacket + filePacket.lenDataPacket + "#" );//Paquete +
               Enviar(MSJ);
               MSJ.clear();
        }
    }
}

FILE *MainWindow::fileManager_GetFWFile(const char *fileName)
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

uint32_t MainWindow::fileManager_GetFWFileSize(FILE **p_file, const char fileName[])
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

uint32_t MainWindow::fileManager_GetFWBlockQuantity(uint32_t fwLen)
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

uint16_t MainWindow::fileManager_SendFWDataInput(SendFilePacket_t *p_SendFWData, FILE **p_file, const char fileName[], uint32_t fwLenght, uint32_t blockQty, uint32_t blockNumber, uint32_t wavLenght)
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


void MainWindow::Enviar(QString Mensaje)
{
     QByteArray MSJ;
     MSJ.append(Mensaje);
     port->write(MSJ);
     MSJ.clear();
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
