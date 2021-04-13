#ifndef TCPSERVER_H
#define TCPSERVER_H

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
#include <QDataStream>

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class MyServer : public QWidget {
Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTcpSocket* mTcpSocket;

    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;


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
