#include "Wave.h"
#include <QFile>
#include <QDataStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>

using namespace std;


void MainWindow::infoWav(QString paths)
{
    wav_hdr wavHeader;
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
            //** aqui tomo los datos de la musica **/
            ui->textEdit_Datos_Recibidos->append("Los bytesTead: " + QString::number( bytesRead) + " bytes del buffer: " + buffer[1] );
        }
        filelength = getFileSize(wavFile);
        QString aux(buffer);
//        QByteArray aux;
//        aux.QByteArray::fromRawData(buffer,BUFFER_SIZE);



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

}

int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

void MainWindow::Enviar(QString Mensaje)
{
     QByteArray MSJ;
     MSJ.append(Mensaje);
     port->write(MSJ);
     MSJ.clear();
}
