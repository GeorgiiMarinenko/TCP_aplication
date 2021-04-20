#include "mainwindow.h"
#include "listenthread.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TcpSocket = new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::StrInverting(QByteArray array)
{
    int size;
    QString result;

    size = array.size();
    for(int i=0; i<size; i++) result[size-i]=array[i];
    result += " Marinenko G.V.";
    qDebug() << result;
    return (result);
}

void MainWindow::on_pushButton_clicked() //.Connect to server
{
    TcpServer = new QTcpServer(this);
    QString                 statusString;
    QString                 localTime;
    QList<QHostAddress>     ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            IpAddressStr = ipAddressesList.at(i).toString();
            IpAddress = ipAddressesList.at(i);
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (IpAddressStr.isEmpty())
    {
        IpAddress = QHostAddress(QHostAddress::LocalHost);
        IpAddressStr = QHostAddress(QHostAddress::LocalHost).toString();
    }
    port = ui->lineEdit->text();
    qDebug() << port.toInt();
    TcpServer->setMaxPendingConnections(4);
    if (!TcpServer->listen(IpAddress, port.toInt()))
    {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + TcpServer->errorString()
                             );
        TcpServer->close();
        return;
    }
    else
    {
        statusString = "host address: " + IpAddress.toString()
                        + "\nport: " + port;
        ui->textBrowser->append("<font color='blue'>" + statusString + "</font>");
        localTime = "Server started at: " + QTime::currentTime().toString("HH:mm:ss")
                    + "\n" + statusString + "\n";
        RecordLogs(localTime, "", "","", 0);
//                ui->textBrowser->setTextColor();
//                ui->textBrowser->textBackgroundColor().green();
        qDebug() << "host address: " << IpAddress;
    }
    connect(TcpServer, SIGNAL(newConnection()),
                this,         SLOT(slotNewConnection()));
}

void MainWindow::reduceConnections(int pandingConnections)
{
    pandingConnections--;
}

void MainWindow::slotNewConnection()
{
    QTcpSocket* pClientSocket = TcpServer->nextPendingConnection();
    QString localTime;
    QString criticalLog;
    QString clients;

    TcpSocket = pClientSocket;
//    socketsList.push_back(*pClientSocket);
//    connect(pClientSocket, SIGNAL(readyRead()),
//            this,          SLOT(slotReadClient()));
    connect(pClientSocket, SIGNAL(disconnected()),
            this, SLOT(reduceConnections(int *)));
    if (MainWindow::pandingConnections < 5)
    {
        ListenThread* thread = new ListenThread(pClientSocket, 0, 0);
        connect(thread, SIGNAL(sendingCompleted(QString, QByteArray)),
                this, SLOT(Recording(QString, QByteArray)));
        thread->start();
        localTime = QTime::currentTime().toString("HH:mm:ss");
    }
    else
    {
        QMessageBox::critical(0,
                              "Max number of users",
                              "Server is overloaded.\nNo more users - max:5"
                             );
        localTime = QTime::currentTime().toString("HH:mm:ss");
        criticalLog = "\n\n*" + localTime + " Server is overloaded!\n";
        RecordLogs(criticalLog, " ", "","", 0);
        return;
    }
    MainWindow::pandingConnections++;
    clients = QString::number(MainWindow::pandingConnections);
    ui->textBrowser->append("\n" + localTime + " - Client "
                            + clients + " is connected!");
}

//void MainWindow::SocketPopBack(QTcpSocket pClientSocket)
//{
//    socketsList.removeOne(pClientSocket);
//}

void MainWindow::slotReadClient()
{
    QString         TextBoxMessage;
    QString         strMessageLogs;
    QString         strMessageClient;
    QString         logs;
    char            Message[42] = "Hello, Client! I'm Georgii's server\r\n";

    QElapsedTimer   timer;
    timer.start(); //TIMER ON

    QTcpSocket*     pClientSocket = (QTcpSocket*)sender();
    QDataStream     in(pClientSocket);

    QString         strTime;
    QString         localTime;

    array = TcpSocket->readAll();
    TcpSocket->write(array);

    TcpSocket->write(Message);
    localTime = QTime::currentTime().toString("HH:mm:ss");
    TextBoxMessage = localTime +
                     " - Hello, Client! I'm Georgii's server\r\n";
    ui->textBrowser_2->append(TextBoxMessage);

    strTime = QString::number(timer.nsecsElapsed()); //TIMER OFF
    qDebug() << "The operation took " << strTime << " nanoseconds";
    strMessageLogs = "Message was sent for: " +  strTime +
                     " nanosec\nCurrent time: " + localTime;
    logs = "Message was sent for: " + strTime + " nanoseconds";
    strMessageClient = localTime + " - " + array + "\n";
    RecordLogs(TextBoxMessage,strMessageClient, StrInverting(array),logs, 1);
    qDebug() << strMessageLogs;
    ui->textBrowser->append(strMessageLogs);
    ui->textBrowser_3->append(strMessageClient);
}

void MainWindow::showInfo(QByteArray logs, QString localTime)
{
    QString serverInfo;
    QString clientInfo;
    QString logStr;

    logStr = logs;
    serverInfo = localTime + " " + StrInverting(logs);
    clientInfo = localTime + " " + logStr;

    ui->textBrowser_3->append(clientInfo);
    ui->textBrowser_2->append(serverInfo);
}

void MainWindow::Recording(QString LocalTime, QByteArray logs)
{
    QString     logFilePath;
//    QByteArray  logsAsByte;

    showInfo(logs, LocalTime);
    logFilePath = ui->lineEdit_2->text();
    if (logFilePath.isEmpty())
    {
        QMessageBox::critical(0,
                              "Empty log-file path!",
                              "Put the correct file path!"
                             );
        TcpServer->close();
        this->close();
        return;
    }
    QFile fileOut(logFilePath);
    if(fileOut.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream( &fileOut );
        stream << "\n\rCurrent time of connection: " + LocalTime;
        stream << "\nClient " + QString::number(MainWindow::pandingConnections) + " : " + logs;
        stream << "\nServer: " + StrInverting(logs);
    }
    fileOut.close();
}

void MainWindow::RecordLogs(QString ServerResponce, QString ClientRsponce,
                            QString InvertStr, QString logs, int flag)
{
    QString logFilePath;
    logFilePath = ui->lineEdit_2->text();
    if (logFilePath.isEmpty())
    {
        QMessageBox::critical(0,
                              "Empty log-file path!",
                              "Put the correct file path!"
                             );
        TcpServer->close();
        this->close();
        return;
    }
    QFile fileOut(logFilePath);
    if(fileOut.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream( &fileOut );
        if (flag == 0)
        {
            stream << ServerResponce;
        }
        else if (flag == 1)
        {
            stream << "\nServer: " + ServerResponce;
            stream << "Client: " + ClientRsponce;
            stream << "Invert string: " + InvertStr << Qt::endl;
            stream << logs + "\n";
        }
    }
    fileOut.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString localTime;
    if ((TcpServer->isListening()) && (!TcpServer->listen(IpAddress, port.toInt())))
    {
        TcpServer->close();
        TcpServer->deleteLater();
        ui->textBrowser->append(" ");
        ui->textBrowser->append("<font color='green'><b>Server successfully closed</b></font>");
        localTime = "\n\nServer successfully closed at: "
                    + QTime::currentTime().toString("HH:mm:ss")
                    + "\n_______________________________________________________\n";;
        RecordLogs(localTime, "", "","", 0);
    }
    else
    {
        ui->textBrowser->append(" ");
        ui->textBrowser->append("<font color='red'><b>\nServer is already closed</b></font>");
     }
//    ui->MainWindow::centralwidget->close();
}

//void MainWindow::on_pushButton_3_clicked()
//{
//    StrInverting(array);
//}
