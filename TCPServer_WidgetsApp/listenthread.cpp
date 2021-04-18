#include "listenthread.h"
#include "mainwindow.h"

ListenThread::ListenThread(QTcpSocket* pClientSocket, int descriptor, QObject *parent):QThread(parent), socketDescriptor(descriptor)
{
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient()));

    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));
}


ListenThread::~ListenThread()
{
    delete socket;
}

void ListenThread::slotReadClient()
{
    QByteArray      array;
    QElapsedTimer   timer;
    QString         logs;
    timer.start(); //TIMER ON

    QTcpSocket*     pClientSocket = (QTcpSocket*)sender();
    QDataStream     in(pClientSocket);

    QString         strTime;
    QString         localTime;
    QByteArray         serverResponce;

    array = pClientSocket->readAll();
    serverResponce = StrInverting(array);
    pClientSocket->write(serverResponce);
    logs = array;
    localTime = QTime::currentTime().toString("HH:mm:ss");
    strTime = QString::number(timer.nsecsElapsed()); //TIMER OFF
    emit sendingCompleted(localTime, array);
}

QByteArray ListenThread::StrInverting(QByteArray array)
{
    int size;
    QByteArray result;

    size = array.size();
    for(int i=0; i<size; i++) result[size-i]=array[i];
    result += " Marinenko G.V.";
    return (result);
}
