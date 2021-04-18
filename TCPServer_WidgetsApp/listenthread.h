#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QThread>
#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMainWindow>

#include <QDebug>

class ListenThread : public QThread
{
    Q_OBJECT
public:
    ListenThread(QTcpSocket* pClientSocket, int descriptor, QObject* parent = 0);
    ~ListenThread();

private:
    int socketDescriptor;
    QTcpSocket* socket;

public slots:
    void slotReadClient();
    QByteArray StrInverting(QByteArray array);

signals:
    void sendingCompleted(QString localTime, QByteArray logs);

};

#endif // LISTENTHREAD_H
