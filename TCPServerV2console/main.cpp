#include <QtGui>
#include "tcpserver.h"
#include <QApplication>
#include <iostream>

int main(int argc, char** argv)
{
    int port;
        std::cout << "Введите номер порта: ";
        std::cin >> port;
    QApplication app(argc, argv);
    MyServer     server(port);

    server.show();

    return app.exec();
}
