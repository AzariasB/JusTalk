#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
    //ui(new Ui::MainWindow)
{

    //Every adress, on port 1234
    if(!listen(QHostAddress::Any,1234))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started";
        connect(this,SIGNAL(newConnection()),this,SLOT(newConnection()));
    }

}



void Server::newConnection()
{
    QTcpSocket *socketClient = nextPendingConnection();
    Connection *client = new Connection(socketClient);
    qDebug() << "New connection !";
}

Server::~Server()
{

   // delete ui;
}
