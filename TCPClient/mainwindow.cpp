#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
            connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
            connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString ipAddress;
    QString ListeningState;
    ListeningState = (socket->ListeningState);
    qDebug() << "First: " << ListeningState;
    socket->connectToHost("127.0.0.1", 45383);//если удаленный то сменить адрес
    ListeningState = (socket->ListeningState);
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "The server is running on\n\nIP: " << ipAddress << " " << ListeningState;

}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}

void MainWindow::sockReady()
{
        socket->waitForReadyRead(5000);
        Data = socket->readAll();
        qDebug() << Data.constData();
}
