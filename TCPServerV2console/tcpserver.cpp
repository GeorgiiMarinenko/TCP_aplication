#include "tcpserver.h"
#include <QString>

MyServer::MyServer(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                    , m_nNextBlockSize(0)
{
    QString adress;
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::AnyIPv6, nPort)) {
        qDebug() << "Server Error. Unable to start the server:" << m_ptcpServer->errorString();
        m_ptcpServer->close();
        return;
    }
    else
    {
        adress = QHostAddress(QHostAddress::AnyIPv6).toString();
        qDebug() << "Connection completed";
        qDebug() << adress;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

//    m_ptxt = new QTextEdit;
//    m_ptxt->setReadOnly(true);

//    //Layout setup
//    QVBoxLayout* pvbxLayout = new QVBoxLayout;
//    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
//    pvbxLayout->addWidget(m_ptxt);
//    setLayout(pvbxLayout);
}

/*virtual*/ void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );
    qDebug() << "Client is connected!";
    qDebug() << pClientSocket->ShareAddress;
    sendToClient(pClientSocket, "Server Response: Connected!");
}

void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        QString strMessage =
            time.toString() + " " + "Client has sended - " + str;
       qDebug() << strMessage;

        m_nNextBlockSize = 0;

        sendToClient(pClientSocket,
                     "Server Response: Received \"" + str + "\""
                    );
    }
}

void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}


