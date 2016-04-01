#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort;

    QTimer *timer = new QTimer(this);
    timer->setInterval(200);

    connect(timer, SIGNAL(timeout()),this, SLOT(update()));
    timer->start();

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

MainWindow::~MainWindow()
{
    serial->close();
    delete serial;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString stringi;

    stringi=ui->comboBox->currentText();

    if(serial->isOpen()==false)
    {
        //próba otwarcia
        serial->setPortName(stringi);
        serial->setBaudRate(QSerialPort::Baud115200);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        ui->tablica->appendPlainText("Otwieram "+serial->portName());
        if(serial->open(QIODevice::ReadWrite)){
            ui->tablica->appendPlainText("Port "+serial->portName()+" otwarty");
        }
        else{
            ui->tablica->appendPlainText("Error!"+serial->errorString());
        }
    }
    else
    {
        //port otwarty to zamkniemy
        ui->tablica->appendPlainText("Port "+serial->portName()+" zamknięty");
        serial->close();
    }


}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tablica->clear();
}

void MainWindow::update()
{
    QTime zegar = QTime::currentTime();
    QString a;

    if(serial->isOpen()){
        ui->label2->setText("Port otwarty");
        a=zegar.toString("hh:mm:ss")+" "+QString::number(ui->suwak->value());
        a+="\r";
        serial->write(a.toStdString().c_str());
        //serial->waitForBytesWritten(-1);
    }
    else{
        ui->label2->setText("Port zamkniety");
    }


    ui->label->setText(zegar.toString("hh:mm:ss")+" "+QString::number(ui->suwak->value()));

}
