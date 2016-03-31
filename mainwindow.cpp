#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);

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
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString stringi;
    stringi=ui->comboBox->currentText();

    serial.setPortName(stringi);

    ui->tablica->appendPlainText("Otwieram "+serial.portName());

    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    if(serial.open(QIODevice::ReadWrite)){
        ui->tablica->appendPlainText("Port otwarty");
        serial.write("***************************ok*\n\r");
        serial.waitForBytesWritten(-1);
        serial.close();
    }
    else{
        ui->tablica->appendPlainText("Error!"+serial.errorString());
    }


}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tablica->clear();
}

void MainWindow::update()
{
    ui->tablica->appendPlainText("tik");
}
