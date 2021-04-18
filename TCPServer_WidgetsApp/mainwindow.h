#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include <QTcpSocket>
#include <QTcpServer>
#include <QNetworkInterface>

#include <QElapsedTimer>
#include <QTime>

#include <QFile>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString StrInverting(QByteArray array);

    QByteArray      Data;
    QByteArray      array;
    QTcpServer*     TcpServer;
    QTcpSocket*     TcpSocket;
//    int             pandingConnections = 0;

    QString         IpAddressStr;
    QHostAddress    IpAddress;
    QString         port;
    QVector<QTcpSocket> socketsList;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void RecordLogs(QString ServerResponce, QString ClientRsponce,
                    QString InvertStr, QString logs, int flag);
    void Recording(QString LocalTime, QByteArray logs);
    void showInfo(QByteArray logs, QString localTime);
//    void reduceConnections(int pandingConnections);
//    void SocketPopBack(QTcpSocket pClientSocket);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

//Log-file path:
// /Users/georgy/Desktop/MAI/Practice MAI/TCP_aplication/TCPServerV2console/log.txt
