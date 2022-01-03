#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>


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
    void infoWav(const QString paths);  //-----------

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
