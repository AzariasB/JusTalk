#include "justalkserver.h"

#include <QRegExp>

JustTalkServer::JustTalkServer(QObject *parent) :
    QMainWindow(),
    server_(new QTcpServer(parent))
{
    setupUi(this);
    connect(server_,SIGNAL(newConnection()),this,SLOT(incomingConnection()));
    connect(quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

bool JustTalkServer::listen(QHostAddress::SpecialAddress host, int port)
{
    return server_->listen(host,port);
}

void JustTalkServer::incomingConnection()
{
    QTcpSocket *client = server_->nextPendingConnection();
    clients_.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client,SIGNAL(disconnected()),this,SLOT(refreshUserList()));
}

void JustTalkServer::refreshUserList()
{
    userListWidget->clear();
    foreach (QString user, users_.values()) {
        new QListWidgetItem(user,userListWidget);
    }
}

void JustTalkServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            users_[client] = user;
            refreshUserList();

            foreach(QTcpSocket *client, clients_)
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
            sendUserList();
        }
        else if(users_.contains(client))
        {
            QString message = line;
            QRegExp wisper("^@([^ ]+) (.*)");
            if(wisper.indexIn(message) == 0){
                QString userPseudo = wisper.cap(1);
                QString wisperMesssage = wisper.cap(2);
                for(auto it = users_.begin(); it != users_.end();++it){
                    if(it.value() == userPseudo){
                        QString res = QString(userPseudo + ":" + wisperMesssage + "\n");
                        qDebug() << res;
                        it.key()->write(res.toUtf8());
                    }
                }
            }else{
                QString user = users_[client];
                qDebug() << "User:" << user;
                qDebug() << "Message:" << message;

                foreach(QTcpSocket *otherClient, clients_)
                    otherClient->write(QString(user + ":" + message + "\n").toUtf8());
            }
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void JustTalkServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients_.remove(client);

    QString user = users_[client];
    users_.remove(client);

    sendUserList();
    foreach(QTcpSocket *client, clients_)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void JustTalkServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users_.values())
        userList << user;

    foreach(QTcpSocket *client, clients_)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}
