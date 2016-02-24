#include "justalkserver.h"

#include <QRegExp>

JustTalkServer::JustTalkServer(QObject *parent) :
    QMainWindow(),
    server_(new QTcpServer(parent))
{
    setupUi(this);
    connect(server_,SIGNAL(newConnection()),this,SLOT(incomingConnection()));
    connect(quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    addActions();

}

void JustTalkServer::addActions()
{
    actions_.addAction("^/me:(.*)$","readUserPresentation");
    actions_.addAction("^@([^ ]+) (.*)","readUserWisper");
    actions_.addAction("^(.*)$","readUserMessage");
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

void JustTalkServer::readUserPresentation(QRegExp reg, QString str)
{
    QString user = reg.cap(1);
    if(!users_.contains(currentClient_)){
        users_[currentClient_] = user;
        refreshUserList();

        foreach(QTcpSocket *client, clients_)
            client->write(QString("Server:" + user + " has joined.\n").toUtf8());
        sendUserList();
    }
}

void JustTalkServer::readUserWisper(QRegExp reg, QString)
{
    QString userPseudo = reg.cap(1);
    QString wisperMesssage = reg.cap(2);
    for(auto it = users_.begin(); it != users_.end();++it){
        if(it.value() == userPseudo || it.key() == currentClient_){
            QString res = QString(userPseudo + ":" + wisperMesssage + "\n");
            qDebug() << res;
            it.key()->write(res.toUtf8());
        }
    }
}

void JustTalkServer::readUserMessage(QRegExp reg, QString message)
{
    QString user = users_[currentClient_];
    qDebug() << "User:" << user;
    qDebug() << "Message : " << message;

    foreach(QTcpSocket *otherClient, clients_)
        otherClient->write(QString(user + ":" + message + "\n").toUtf8());
}

void JustTalkServer::readyRead()
{
    currentClient_ = (QTcpSocket*)sender();
    while(currentClient_->canReadLine())
    {
        QString line = QString::fromUtf8(currentClient_->readLine()).trimmed();
        qDebug() << "Read line:" << line;
        if(actions_.triggerActions(line,this))
            qDebug() << "Action found";
        else
            qWarning() << "Got bad message from client:" << currentClient_->peerAddress().toString() << line;

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
