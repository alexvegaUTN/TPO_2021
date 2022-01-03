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
    if (wavFile == nullptr)
    {
        QMessageBox::critical(this, "Error en la APERTURA DEL ARCHIVO ", paths);
    }
    //Read the header
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    ui->textEdit_Datos_Recibidos->setText("Header Read " + QString::number( bytesRead) + "bytes.");
    if (bytesRead > 0)
    {
        //Read the data
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
        static const uint16_t BUFFER_SIZE = 4096;
        char* buffer = new char[BUFFER_SIZE];
        while ((bytesRead = fread(buffer, sizeof buffer, BUFFER_SIZE , wavFile)) > 0)
        {
            /** aqui tomo los datos de la musica **/
//            cout << "Read " << bytesRead << " bytes." << endl;
//            ui->textEdit_Datos_Recibidos->setText("Read " + QString::number( bytesRead) + "bytes.");
        }
        filelength = getFileSize(wavFile);
        QString aux(buffer);

        ui->textEdit_Datos_Recibidos->append("File is: " + QString::number( filelength) + " bytes.");
        ui->textEdit_Datos_Recibidos->append("RIFF header: " + QString::number(wavHeader.RIFF[0] + wavHeader.RIFF[1] + wavHeader.RIFF[2] + wavHeader.RIFF[3] ));
        ui->textEdit_Datos_Recibidos->append("Data size: " + QString::number( wavHeader.ChunkSize) );
        ui->textEdit_Datos_Recibidos->append("WAVE header: " + QString::number( wavHeader.WAVE[0] + wavHeader.WAVE[1] + wavHeader.WAVE[2] + wavHeader.WAVE[3] ));


        ui->textEdit_Datos_Recibidos->append("FMTr: " + QString::number( wavHeader.fmt[0]) + wavHeader.fmt[1] + wavHeader.fmt[2] + wavHeader.fmt[3] );
        ui->textEdit_Datos_Recibidos->append("Subchunk1Size: " + QString::number( wavHeader.Subchunk1Size) );
        ui->textEdit_Datos_Recibidos->append("Audio Format: " + QString::number( wavHeader.AudioFormat) );

        ui->textEdit_Datos_Recibidos->append("Number of channels: " + QString::number( wavHeader.NumOfChan ) );
        ui->textEdit_Datos_Recibidos->append("Sampling Rate: " + QString::number( wavHeader.SamplesPerSec ) );
        ui->textEdit_Datos_Recibidos->append("Number of bytes per second: " + QString::number( wavHeader.bytesPerSec ) );

        ui->textEdit_Datos_Recibidos->append("Block align: " + QString::number( wavHeader.blockAlign ) );
        ui->textEdit_Datos_Recibidos->append("Number of bits used: " + QString::number( wavHeader.bitsPerSample ) );
        ui->textEdit_Datos_Recibidos->append("Data string: " + QString::number( wavHeader.Subchunk2ID[0] + wavHeader.Subchunk2ID[1] + wavHeader.Subchunk2ID[2] + wavHeader.Subchunk2ID[3] ) );
        ui->textEdit_Datos_Recibidos->append("Data length: " + QString::number( wavHeader.Subchunk2Size ) );
        ui->textEdit_Datos_Recibidos->append("Data Musica: " + aux );

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
