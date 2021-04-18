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

    QTcpSocket*     socket;
    QByteArray      Data;
    QByteArray      array;
    QTcpServer*     TcpServer;
    QTcpSocket*     TcpSocket;

    QString         IpAddressStr;
    QHostAddress    IpAddress;
    QString         port;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void RecordLogs(QString ServerResponce, QString ClientRsponce,
                    QString InvertStr, QString logs, int flag);

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
