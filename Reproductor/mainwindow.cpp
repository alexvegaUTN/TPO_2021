#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "Wave.h"
#include <QUrl>
#include <QFileDialog>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFile>
#include <string>
#include <QDataStream>
#include "Wave2.h"

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
    connect(reproductor,&QMediaPlayer::positionChanged, [&]( qint64 pos ){
        ui->Progreso_Musica->setValue( pos );}
    );


    connect(reproductor,&QMediaPlayer::durationChanged, [&]( qint64 duracion){
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
 //       ui->Boton_Conectar->setText("Desconectar");
    } else {
        ui->Boton_Conectar->setStyleSheet("font-weight: normal; color: white; background-color: red;");
        ui->Boton_Conectar->setText("Desconectado");
        datosRecibidos.clear();
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

    QString Comando;
    QByteArray MSJ;

    if( conectado() )
    {
        Comando = "$CPLAY#";
    }
    else
    {
        QMessageBox::critical(this, "Error PLAY", "No se pudo detener la Musica, Conecte el dispositivo! ");
    }

    MSJ.append(Comando);
    port->write(MSJ);

}


void MainWindow::on_Boton_Pause_clicked()
{
    reproductor->pause();
    QString Comando;
    QByteArray MSJ;

    if( conectado() )
    {
        Comando = "$CPAUSA#";
        MSJ.append(Comando);
        port->write(MSJ);
    }
    else
    {
        QMessageBox::critical(this, "Error PAUSE ", "No se pudo detener la Musica, Conecte el dispocitivo! ");
    }
}


void MainWindow::on_Boton_Stop_clicked()
{
    reproductor->stop(); // lo reproduce ne la compu
    QString Comando;
    QByteArray MSJ;

    if( conectado() )
    {
        Comando = "$CSTOP#";
    }
    else
    {
        QMessageBox::critical(this, "Error STOP", "No se pudo detener la Musica, Conecte el dispocitivo! ");
    }

    MSJ.append(Comando);
    port->write(MSJ);

}


void MainWindow::on_Boton_Mute_clicked()
{
    QString Comando;
    QByteArray MSJ;
    if(ui->Boton_Mute->text() == "Mute" )
    {
     reproductor->setMuted(true);
     ui->Boton_Mute->setText("unMute");
     Comando = "$CUNMUTE#";
    }
    else
    {
     reproductor->setMuted(false);
     ui->Boton_Mute->setText("Mute");
     Comando = "$CMUTE#";
    }
    Comando = "$Block_OK#";/// quitar luego
    MSJ.append(Comando);
    port->write(MSJ);
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
        port->setBaudRate(QSerialPort::Baud19200);
        port->setFlowControl(QSerialPort::NoFlowControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);

        //Abrimos el puerto en modo lectura-escritura
        if (port->open(QIODevice::ReadWrite) == true)
        {
            //Conectamos las señales que nos interesen
            connect( port, SIGNAL(readyRead()),this, SLOT(onDatosRecibidos()));
            ui->textEdit_Datos_Recibidos->setText("Abrimos el puerto en modo lectura-escritura");

            QString Comando;
            QByteArray MSJ;
            Comando = "$CONECTADO#";
            MSJ.append(Comando);
            port->write(MSJ);

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
  Path_fileName = QFileDialog::getOpenFileName(this,"Abrir");

  if( Path_fileName.isEmpty() )
  {
      return;
  }
  else
  {
//    ui->textEdit_Datos_Recibidos->setText(Path_fileName);

      /* Inicializo la variable de estados para transmision de paquetes */
      estado_infoWAV = __RRIF;
      block_idx = 0;

      infoWav(Path_fileName); //<----------- AQUI le pasaria la dircecion del arrchivo
  }
}


void MainWindow::on_Boton_Actualizar_Puertos_clicked()
{
    enumerarPuertos();
}


void MainWindow::onDatosRecibidos()
{

    ui->textEdit_Datos_Recibidos->setText("DatosRecibidos");
    //QByteArray bytesRX;

    QVector<char>bytesRX;

    //static int idx = 0;
    //static QVector<char>bytesRX(32);
    //QByteArray bytesRX;
    //static char buffer_rx[50];
    int cant = port->bytesAvailable();
    bytesRX.resize(cant);

    //amount = port->read(&buffer_rx[idx], cant);
    //bytesRX = port->readAll();
    port->read(bytesRX.data(), bytesRX.size());

    //bytesRX = port->readAll();
    //port->read(bytesRX.data(), cant);
    datosRecibidos.append(bytesRX);
    //idx += cant;

    if (bytesRX.last() == '#') {
        datosRecibidos.append('\0');

        ProcesarDatosRecibidos();
        //infoWav(Path_fileName);
        //datosRecibidos.clear();
    }
}

void MainWindow::ProcesarDatosRecibidos()
{
    // $RIFF_OK#----$fmt_OK#-------$data_OK#-------$Block_OK#-----$SDWR_OK#-----
     ui->textEdit_Datos_Recibidos->setText("ProcesarDatosRecibidos");

     if (datosRecibidos.last() == '\0') {

/*         switch (estado_infoWAV) {
         case __RRIF:
             //estado_infoWAV = __FMT;
             break;
         case __FMT:
             infoWav(Path_fileName);
             datosRecibidos.clear();
             //estado_infoWAV = __DATA;
             break;

         case __DATA:
             infoWav(Path_fileName);
             datosRecibidos.clear();
             //estado_infoWAV = __BLOQUE;
             break;

         case __BLOQUE:
             infoWav(Path_fileName);
             datosRecibidos.clear();
             break;

         default:
             datosRecibidos.clear();
             break;
         }*/


         if (!strcmp("$CONECTADO#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos + "VEGAAAA\n" );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos);
            ui->textEdit_Datos_Recibidos->append("VEGAAAA\n");

            datosRecibidos.clear();
         }

         else if (!strcmp("$PLAY_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            datosRecibidos.clear();
         }

         else if (!strcmp("$PAUSA_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos);
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            datosRecibidos.clear();
         }

         else if (!strcmp("$STOP_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos);
             ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            datosRecibidos.clear();
         }

         else if (!strcmp("$MUTE_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos);
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
               QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            //infoWav(Path_fileName);
            datosRecibidos.clear();
         }

         else if (!strcmp("$UNMUTE_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos);
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            //infoWav(Path_fileName);
            datosRecibidos.clear();
         }

         else if (!strcmp("$RIFF_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            infoWav(Path_fileName);
            datosRecibidos.clear();
         }

         else if (!strcmp("$fmt_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            infoWav(Path_fileName);
            datosRecibidos.clear();
         }

         else if (!strcmp("$data_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            infoWav(Path_fileName);
            datosRecibidos.clear();
         }

         else if (!strcmp("$Blk_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

            infoWav(Path_fileName);
            datosRecibidos.clear();
         }

         else if (!strcmp("$SDWR_OK#", reinterpret_cast<const char *>(datosRecibidos.data())) )
         {
            //ui->textEdit_Datos_Recibidos->append("RX:" + datosRecibidos );
            ui->textEdit_Datos_Recibidos->append("RX:");

            QString str_datosRecibidos =
                QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

            ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );
         }
         else {

         }
     }

     ui->textEdit_Datos_Recibidos->append("RX:");

     QString str_datosRecibidos =
         QString::fromLocal8Bit(reinterpret_cast<const char *>(datosRecibidos.data()), datosRecibidos.length());

     ui->textEdit_Datos_Recibidos->append(str_datosRecibidos );

}

