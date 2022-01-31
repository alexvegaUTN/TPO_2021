#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <Wave2.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QMediaPlayer;
class QSerialPort;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool conectado();
    void enumerarPuertos();
    void actualizarEstadoConexion();
    void ProcesarDatosRecibidos();
    void infoWav(const QString paths);
    //-----------
    FILE * fileManager_GetWavFile(const char * fileName);
    uint32_t fileManager_GetWavFileSize(FILE ** p_file, const char fileName[]);
    uint32_t fileManager_GetWavBlockQuantity(uint32_t fwLen);
    void GetWavHeader(FILE ** p_file, const char fileName[] ,wav_hdr *p_wavHeader);
    uint16_t fileManager_SendWavDataInput(SendFilePacket_t * p_SendWavData, FILE **p_file, const char fileName[], uint32_t fwLenght, uint32_t blockQty, uint32_t blockNumber, uint32_t wavLenght);
    //-----------
    void Enviar(QString);
    void Enviar_v2(QByteArray &msj);
    void Enviar_v3(const char * msj, qint64 length);

    enum
    {
        ESPERO_MENSAJE,
        RECIBO_MENSAJE_MSB,
        RECIBO_MENSAJE_LSB,
        FIN_DE_TRAMA,
    };

private slots:

    void on_Boton_Play_clicked();
    void on_Boton_Pause_clicked();
    void on_Boton_Stop_clicked();
    void on_Boton_Mute_clicked();
    void on_Barra_Volumen_valueChanged(int value);
    void on_Boton_Conectar_clicked();
    void on_Boton_Abrir_clicked();

    void on_Boton_Actualizar_Puertos_clicked();
    void onDatosRecibidos();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *reproductor;
    QSerialPort  *port;
    QByteArray  datosRecibidos;


};
#endif // MAINWINDOW_H
