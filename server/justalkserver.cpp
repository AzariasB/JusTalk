#include "justalkserver.h"

JustTalkServer::JustTalkServer(QObject *parent) :
    QMainWindow(),
    server_(new QTcpServer(parent))
{
    setupUi(this);
    connect(server_,SIGNAL(newConnection()),this,SLOT(incomingConnection()));
    connect(server_,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
    connect(quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(userListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenu(QPoint)));
    addActions();

}

void JustTalkServer::kickUser()
{
    QListWidgetItem* selected = userListWidget->selectedItems()[0];
    QString pseudo = selected->text();
    QTcpSocket *userSocket = users_.key(pseudo);
    QString reason = QInputDialog::getText(this,"Reason","Kick reason");
    QString message = QString("/kicked:"+reason + "\n");
    userSocket->write(QString(message).toUtf8());
    userSocket->disconnectFromHost();
    users_.remove(userSocket);
}

void JustTalkServer::customContextMenu(QPoint p)
{
    QPoint globalPos = userListWidget->mapToGlobal(p);
    QMenu ctxMenu;
    ctxMenu.addAction("Kick",this,SLOT(kickUser()));
    ctxMenu.exec(globalPos);
}

void JustTalkServer::handleError(QAbstractSocket::SocketError er)
{
    QString title = "Connection error";
    QString message = "An error occured";
    switch (er) {
        case QAbstractSocket::ConnectionRefusedError:
            title = "Connection refused";
            message = "The connection to the server was refused.";
            break;
    case QAbstractSocket::RemoteHostClosedError:
            title = "Remote host closed";
            message = "The remote host was closed";
            break;
    case QAbstractSocket::HostNotFoundError:
            title = "Host not found";
            message ="The host you trying to access does not exist or is unreachable";
            break;
    case QAbstractSocket::SocketTimeoutError:
            title = "Timeout";
            message = "The request is taking too long.";
        default:
            break;
    }
    QMessageBox::warning(this,title,message);
}

void JustTalkServer::addActions()
{
    //User presentation : '/me:pseudo'
    actions_.addAction("^/me:(.*)$","readUserPresentation");

    //User wispering to another : '@pseudo:message'
    actions_.addAction("^@([^ ]+) (.*)","readUserWisper");

    //Normal message : 'message'
    actions_.addAction("^(.*)$","readUserMessage");
}

bool JustTalkServer::listen(QHostAddress::SpecialAddress host, int port)
{
    return server_->listen(host,port);
}

void JustTalkServer::incomingConnection()
{
    //Get the connection
    QTcpSocket *client = server_->nextPendingConnection();

    //Insert into the set
    clients_.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    //Connect client's slots to detect when ready to read, when disconnect.
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client,SIGNAL(disconnected()),this,SLOT(refreshUserList()));
}

void JustTalkServer::refreshUserList()
{
    userListWidget->clear();
    //Update user list
    foreach (QString user, users_.values()) {
        new QListWidgetItem(user,userListWidget);
    }
}

void JustTalkServer::readUserPresentation(QRegExp reg, QString)
{
    //User pseudo is first group of the regex
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
    //Destination pseudo is the first group
    QString receiver = reg.cap(1);
    QString sender = users_[currentClient_];

    //Message is second group
    QString wisperMesssage = reg.cap(2);


    for(auto it = users_.begin(); it != users_.end();++it){
        //Send the message to the receiver and the sender
        if(it.value() == receiver || it.key() == currentClient_){
            QString res = QString(sender + ":" + wisperMesssage + "\n");
            it.key()->write(res.toUtf8());
        }
    }
}

void JustTalkServer::readUserMessage(QRegExp, QString message)
{
    QString user = users_[currentClient_];
    qDebug() << "User:" << user;
    qDebug() << "Message : " << message;
    //Write the message to each connected client
    foreach(QTcpSocket *otherClient, clients_)
        otherClient->write(QString(user + ":" + message + "\n").toUtf8());
}

void JustTalkServer::readyRead()
{
    //Read message from client
    currentClient_ = (QTcpSocket*)sender();
    while(currentClient_->canReadLine())
    {
        QString line = QString::fromUtf8(currentClient_->readLine()).trimmed();
        //Trigger the action
        qDebug() << "Read line:" << line;
        if(actions_.triggerActions(line,this))
            qDebug() << "Action found";
        else
            qWarning() << "Got bad message from client:" << currentClient_->peerAddress().toString() << line;

    }
}

void JustTalkServer::disconnected()
{
    //Get the sender of the signal
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    //Remove from the set
    clients_.remove(client);

    QString user = users_[client];
    //and from the map
    users_.remove(client);

    //Refresh the client's user list
    sendUserList();
    foreach(QTcpSocket *client, clients_)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void JustTalkServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users_.values())
        userList << user;

    //For each client, send the list of user joined with a ','
    QString message = QString("/users:" + userList.join(",") + "\n");
    foreach(QTcpSocket *client, clients_)
        client->write(message.toUtf8());
}
