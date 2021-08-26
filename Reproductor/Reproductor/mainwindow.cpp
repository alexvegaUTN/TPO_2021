#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QFileDialog>
#include <QSerialPortInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //------------ puertos--------------------------
    port = NULL;
    enumerarPuertos();
    actualizarEstadoConexion();

    //----------- puntero de reproduccion ---------
    reproductor = new QMediaPlayer;

    //----------- barra de progreso ---------------
    connect(reproductor,&QMediaPlayer::positionChanged, [&]( qint64 pos ) {
        ui->Progreso_Musica->setValue( pos );}
    );


    connect(reproductor,&QMediaPlayer::durationChanged, [&]( qint64 duracion) {
                ui->Progreso_Musica->setMaximum(duracion);
            });

}

MainWindow::~MainWindow()
{
    delete ui;
    if (port != NULL) {
        port->close();
        delete port;
    }
}

void MainWindow::enumerarPuertos()
{
    ui->comboBox_Puertos->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); i++)
    {
        ui->comboBox_Puertos->addItem(ports.at(i).portName(), ports.at(i).portName());
    }
}
void MainWindow::actualizarEstadoConexion()
{
    if (conectado()) {
        ui->Boton_Conectar->setStyleSheet("font-weight: bold; color: black; background-color: lightgreen;");
        ui->Boton_Conectar->setText("CONECTADO");
        ui->Boton_Conectar->setText("Desconectar");
    } else {
        ui->Boton_Conectar->setStyleSheet("font-weight: normal; color: white; background-color: red;");
        ui->Boton_Conectar->setText("Desconectado");
        ui->Boton_Conectar->setText("Conectar");
    }
}

bool MainWindow::conectado()
{
    if (port != NULL) {
        return (port->isOpen());
    } else
        return false;
}

void MainWindow::on_Boton_Play_clicked()
{

//    reproductor->setMedia(QUrl("qrc:/Musicas/Roses.mp3"));
    reproductor->setVolume(10);
    reproductor->play();
}


void MainWindow::on_Boton_Pause_clicked()
{
    reproductor->pause();
}


void MainWindow::on_Boton_Stop_clicked()
{
    reproductor->stop();
}


void MainWindow::on_Boton_Mute_clicked()
{
    if(ui->Boton_Mute->text() == "Mute" )
    {
     reproductor->setMuted(true);
     ui->Boton_Mute->setText("unMute");
    }
    else
    {
     reproductor->setMuted(false);
     ui->Boton_Mute->setText("Mute");
    }

}


void MainWindow::on_Barra_Volumen_valueChanged(int value)
{
    reproductor->setVolume(value);
}


void MainWindow::on_Boton_Conectar_clicked()
{
    //Si no hay conexión activa, creamos una nueva
    if (!port) {
        //Obtenemos el nombre del puerto del comboBox
        QString portName;
        int i = ui->comboBox_Puertos->currentIndex();
        portName = ui->comboBox_Puertos->itemData(i).toString();
        if (portName.isEmpty())
        {
            QMessageBox::critical(this, QString::fromLatin1("Error de conexión"), QString::fromLatin1("Seleccione un puerto válido"));
            return;
        }
        //Creamos y configuramos el puerto
        port = new QSerialPort(portName);
        port->setBaudRate(QSerialPort::Baud9600);
        port->setFlowControl(QSerialPort::NoFlowControl);
        port->setParity(QSerialPort::EvenParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        //Abrimos el puerto en modo lectura-escritura
        if (port->open(QIODevice::ReadWrite) == true)
        {
            //Conectamos las señales que nos interesen

        }
        else {
            // Si hubo un error en la apertura...
            QMessageBox::critical(this, "Error", "No se puede abrir el puerto "+port->portName());
            delete port;
            port = NULL;
        }
    }
    else {
        //Si había una conexión activa, la cerramos
        port->close();
        delete port;
        port = NULL;
    }

    actualizarEstadoConexion();
}


void MainWindow::on_Boton_Abrir_clicked()
{
  QString filename = QFileDialog::getOpenFileName(this,"Abrir");
  if( filename.isEmpty() )
  {
      return;
  }
  else
  {
      reproductor->setMedia(QUrl::fromLocalFile(filename));
      reproductor->setVolume(ui->Barra_Volumen->value());

  }
}

