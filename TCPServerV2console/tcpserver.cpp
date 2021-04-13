#include "tcpserver.h"
#include <QString>

MyServer::MyServer(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                    , m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + m_ptcpServer->errorString()
                             );
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);
}

MyServer::~MyServer()
{
    m_ptcpServer->close();
}

/*virtual*/ void MyServer::slotNewConnection()
{
    QString TextBoxMessage;
    char Message[42] = "Hello, World!!! I am echo server!\r\n";
    TextBoxMessage = "Sent: Hello, World!!! I am echo server!\r\n";

    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    mTcpSocket = pClientSocket;

    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));

    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient()));

    qDebug() << "Client is connected!";
    qDebug() << pClientSocket->ShareAddress;
    m_ptxt->append("Client is connected!");
//    sendToClient(pClientSocket, "Server Response: Connected!");

    mTcpSocket->write(Message);
    m_ptxt->append(TextBoxMessage);
}

void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);

    while(mTcpSocket->bytesAvailable() > 0)
    {
        QByteArray array = mTcpSocket->readAll();

        mTcpSocket->write(array);
    }

//    while (true) {
//        if (!m_nNextBlockSize)
//        {
//            if (pClientSocket->bytesAvailable() < sizeof(quint16))
//            {
//                qDebug() << "if 1";
//                break;
//            }
//            in >> m_nNextBlockSize;
//        }

//        if (pClientSocket->bytesAvailable() < m_nNextBlockSize)
//        {
//            qDebug() << "if 2";
//            break;
//        }
//        QTime   time;
//        QString str;
//        in >> time >> str;

//        QString strMessage =
//            time.toString() + " " + "Client has sended - " + str;
//        qDebug() << "Point 1";
//        qDebug() << strMessage;
//        m_ptxt->append(strMessage);

//        m_nNextBlockSize = 0;

//        sendToClient(pClientSocket,
//                     "Server Response: Received \"" + str + "\""
//                    );
//    }
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


