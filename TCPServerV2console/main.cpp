#include <QtGui>
#include "tcpserver.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MyServer     server(45383);

    server.show();

    return app.exec();
}
