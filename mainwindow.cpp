#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkdata.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->server = new NetworkServer(this);
    this->server->listen(QHostAddress::Any,16667);
    this->socket = new NetworkSocket(new QTcpSocket(),this);

    QString host = "192.168.43.6";
    QString localhost="127.0.0.1";
    Q_UNUSED(localhost);
    this->socket->hello(host,16667);
    this->socket->base()->waitForConnected();
    qDebug()<<"Success";
    connect(this->server,&NetworkServer::receive,this,&MainWindow::receieveData);
    connect(this->socket,&NetworkSocket::receive,this,&MainWindow::receieveDataFromServer);
    connect(this->ui->clientSendButton,&QPushButton::clicked,this,&MainWindow::onClientSendButtonClicked);
    connect(this->ui->serverSendButton,&QPushButton::clicked,this,&MainWindow::onServerSendButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receieveData(QTcpSocket* client, NetworkData data)
{
    qDebug()<<"Server get a data: "<<client<<" "<<data.encode();
    lastOne=client;
    this->ui->serverGetEdit->setText(data.data1);
}

void MainWindow::receieveDataFromserver(NetworkData data)
{
    qDebug()<<"Client get a data: "<<data.encode();
    this->ui->clientGetEdit->setText(data.data1);
}

void MainWindow::onClientSendButtonClicked()
{
    this->socket->send(NetworkData(OPCODE::START_GAME_OP,this->ui->clientSendEdit->text(),"send by client"));
}

void MainWindow::onserverSendButtonClicked()
{
    this->server->send(lastOne,NetworkData(OPCODE::START_GAME_OP,this->ui->serverSendEdit->text(),"send by server"));
}

