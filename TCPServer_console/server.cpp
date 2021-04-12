#include "server.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

Server::Server()
{}

Server::~Server()
{}

void Server::startServer()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    tcpServer = new QTcpServer(this);

    int port = 45383;
    if (tcpServer->listen(QHostAddress(QHostAddress::LocalHost), port))
    {
       /* qDebug() << "Listening from port " << port*/;
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        qDebug() << "The server is running on\n\nIP: " << ipAddress << "\nport: " << tcpServer->serverPort() << "\n\n";
    }
    else
    {
        qDebug() << "Error listening";
        return;
    }
    qDebug() << "HostAdress:" << QHostAddress::LocalHost;
}

void Server::incomConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

            connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
            connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

    qDebug() << socketDescriptor << "Client connected";

    socket->write("You are connected to server");
    qDebug() << "Information have sent to client - YES";
}

void Server::sockReady()
{}

void Server::sockDisc()
{
    qDebug() << "Disconnected";
    socket->deleteLater();
}
