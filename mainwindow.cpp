#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDateTime>
#include <QMessagebox>

//Konstruktor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort;

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);

    //połaczenie sygnału timera z oknem update
    connect(timer, SIGNAL(timeout()),this, SLOT(update()));
    timer->start();

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    QSerialPortInfo porty;
    QList<QSerialPortInfo> lista;
    lista=porty.availablePorts();
    ui->tablica->appendPlainText("Aviable Ports:");


    QList<QSerialPortInfo>::iterator i;
    for (i=lista.begin() ; i<lista.end();i++)
       {
          ui->tablica->appendPlainText(i->portName()+" "+i->description());
          ui->comboBox->addItem(i->portName());
       }
}

//destruktor
MainWindow::~MainWindow()
{
    serial->close();
    delete serial;
    delete ui;
}

//Klawisz START
void MainWindow::on_pushButton_clicked()
{

    QString stringi;

    stringi=ui->comboBox->currentText();



    if(serial->isOpen()==false)
    {
        //próba otwarcia
        serial->setPortName(stringi);

        ui->tablica->appendPlainText("Otwieram "+serial->portName());
        if(serial->open(QIODevice::ReadWrite)){
            ui->tablica->appendPlainText("Port "+serial->portName()+" otwarty");
            serial->setBaudRate(QSerialPort::Baud115200);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::HardwareControl);
            //serial->setFlowControl(QSerialPort::NoFlowControl);
        }
        else{
            ui->tablica->appendPlainText("Error!"+serial->errorString());
            QMessageBox::critical(0, "ERROR", serial->errorString());
        }
    }
    else
    {
        //port otwarty to zamkniemy
        ui->tablica->appendPlainText("Port "+serial->portName()+" zamknięty");
        serial->close();
    }


}

//Funkcja czyści okno
void MainWindow::on_pushButton_2_clicked()
{
    ui->tablica->clear();
}

void MainWindow::readData()
{
    //Sprawdzeie czy nie ma danych do odbioru
    QByteArray daneRX;

    ui->tablica->appendPlainText("przyszło...");

    while(serial->bytesAvailable()>0)
    {
        daneRX+=serial->readAll();
        ui->tablica->appendPlainText(QString(daneRX));
    }
}

//Funkcja wysyła dane na zdarzeniu update
void MainWindow::update()
{
    QTime zegar = QTime::currentTime();
    QString a;
    QByteArray daneRX;

    //wysyłanie danych
    if(serial->isOpen()){
        ui->label2->setText("Port otwarty");
        a=zegar.toString("hh:mm:ss")+" "+QString::number(ui->suwak->value());
        a+="\r";
        if(ui->checkBox->isChecked()) serial->write(a.toStdString().c_str());


    }
    else{
        ui->label2->setText("Port zamkniety");
    }
    ui->label->setText(zegar.toString("hh:mm:ss")+" "+QString::number(ui->suwak->value()));

}
