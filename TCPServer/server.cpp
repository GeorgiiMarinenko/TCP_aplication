#include "server.h"

Server::Server()
{}

Server::~Server()
{}

void Server::startServer()
{
    int port = 443;
    if (this->listen(QHostAddress::Any, port))
        qDebug() << "Listening from port " << port;
    else
        qDebug() << "Error listening";
    qDebug() << "HostAdress:" << QHostAddress::Any;
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
