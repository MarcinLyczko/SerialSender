#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //tu będzie kod który zamknie usta niedowiarkom
    QString stringi;
    stringi="dupa";
    stringi+=" dupcia";
    ui->label->setText(stringi);
}
