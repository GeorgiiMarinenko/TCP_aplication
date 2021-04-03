#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>

class Server: public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void startServer();
    void incomConnection(qintptr socketDescriptor); //Метод по умолчания для сервера TCP - описание дейсвтий в случае присоединения нового клиента
    void sockReady();
    void sockDisc(); //Отключение клиента от сокета (удаления объекта)

private:
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
};

#endif // SERVER_H
