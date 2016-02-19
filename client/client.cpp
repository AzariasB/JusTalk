#include "client.h"
#include "ui_client.h"

Client::Client() :
    QObject()
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    socket_ = new QTcpSocket(this);

    connect(socket_,SIGNAL(disconnected()),this,SLOT(readMessage()));
    connect(socket_,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
    requestConnection("localhost",1234);
}

void Client::readMessage(){
    qDebug() << "message received";
    qDebug() << QString(socket_->readAll());

}

void Client::handleError(QAbstractSocket::SocketError er){
    QApplication::quit();
}

void Client::requestConnection(QString hostName, qint16 adress){
    socket_->abort();
    blockSize_ = 0;
    qDebug() << "trying to connect";
    socket_->connectToHost("127.0.0.1",1234);
}

Client::~Client()
{
    delete socket_;
    //delete ui;
}
