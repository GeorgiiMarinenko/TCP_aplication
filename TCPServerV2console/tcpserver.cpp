#include "tcpserver.h"

MyServer::MyServer(int nPort, QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                    , nNextBlockSize(0)
{
    pTcpServer = new QTcpServer(this);

    QString ipAddress;
    QHostAddress IpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            IpAddress = ipAddressesList.at(i);
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        IpAddress = QHostAddress(QHostAddress::LocalHost);
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    if (!pTcpServer->listen(IpAddress, nPort))
    {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + pTcpServer->errorString()
                             );
        pTcpServer->close();
        return;
    }
    else
    {
        qDebug() << "host address: " << IpAddress;
    }
    connect(pTcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

    ptxt = new QTextEdit;
    ptxt->setReadOnly(true);
    ptxt->setFixedSize(600,300);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->setMargin(10);
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(ptxt);
    setLayout(pvbxLayout);
}

MyServer::~MyServer()
{
    pTcpServer->close();
}

/*virtual*/ void MyServer::slotNewConnection()
{
    QString TextBoxMessage;
    char Message[42] = "Hello, World!!! I am an echo server!\r\n";
    TextBoxMessage = "Sent: Hello, World!!! I am an echo server!\r\n";

    QTcpSocket* pClientSocket = pTcpServer->nextPendingConnection();
    TcpSocket = pClientSocket;

    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));

    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient()));

    qDebug() << "Client is connected!";
    qDebug() << pClientSocket->ShareAddress;
    ptxt->append("Client is connected!");

    TcpSocket->write(Message);
    ptxt->append(TextBoxMessage);

    qDebug() << TcpSocket->readAll();
//    sendToClient(pClientSocket, "Server Response: Connected!");

}

void MyServer::slotReadClient()
{
    QElapsedTimer timer;
    timer.start(); //TIMER ON


    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);

    QByteArray array;
    QString   strTime;
    QString   localTime;


//    in.setVersion(QDataStream::Qt_4_2);
    while(TcpSocket->bytesAvailable() > 0)
    {
        array = TcpSocket->readAll();
        TcpSocket->write(array);
    }
    localTime = QTime::currentTime().toString("HH:mm:ss");
    strTime = QString::number(timer.nsecsElapsed()); //TIMER OFF
    qDebug() << "The operation took" << strTime << "nanoseconds";
    QString strMessage =
                       "Was sent for: " +  strTime + " nanosec\nCurrent time: " + localTime + "\nClient has sended - " + array;
    qDebug() << strMessage;
    ptxt->append(strMessage);
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


