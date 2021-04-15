#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QString>
#include <iostream>

#include <QObject>
#include <QWidget>

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>


#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

#include <QTime>
#include <QElapsedTimer>
#include <QDataStream>

#include <QtNetwork>
#include <QtCore>

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class MyServer : public QWidget
{
Q_OBJECT
private:
    QTcpServer* pTcpServer;
    QTcpSocket* TcpSocket;

    QTextEdit*  ptxt;
    quint16     nNextBlockSize;


private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
    MyServer(int nPort, QWidget* pwgt = 0);
    ~MyServer();

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
};

#endif // TCPSERVER_H
