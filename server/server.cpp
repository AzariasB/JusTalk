#include "server.h"
#include "ui_client.h"

Server::Server(QObject *parent) :
    QObject(parent)
    //ui(new Ui::MainWindow)
{
    server_ = new QTcpServer(this);
    connect(server_,SIGNAL(newConnection()),this,SLOT(newConnection()));

    //Every adress, on port 1234
    if(!server_->listen(QHostAddress::Any,1234))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started";
    }
}



void Server::newConnection()
{
    QTcpSocket *socketClient = server_->nextPendingConnection();
    qDebug() << "New connection !";
    socketClient->write("hello wolrd");
    socketClient->flush();
    socketClient->waitForBytesWritten(3000);
    socketClient->close();
}

Server::~Server()
{
    delete server_;
   // delete ui;
}
