#include "justalkclient.h"


JusTalkClient::JusTalkClient(QWidget *parent) :
    QMainWindow(parent)
{

    setupUi(this);

    socket_ = new QTcpSocket(this);

    login();

    if(!pseudo_.isEmpty()){
        createActions();

        roomTextEdit->setReadOnly(true);

        userListWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

        connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket_, SIGNAL(connected()), this, SLOT(connected()));
        connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handlError(QAbstractSocket::SocketError)));
        connect(userListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(roomContextMenu(QPoint)));
        connect(talkButton,SIGNAL(clicked()), this, SLOT(talkClicked()));
        connect(sayLineEdit,SIGNAL(returnPressed()),this,SLOT(talkClicked()));
        connect(sayLineEdit,SIGNAL(textEdited(QString)),this,SLOT(answerWhisper(QString)));
    }
}

void JusTalkClient::roomContextMenu(QPoint p)
{
    if(userListWidget->selectedItems().size() == 1){
        QPoint globalPos = userListWidget->mapToGlobal(p);

        QMenu ctxMenu;
        ctxMenu.addAction("Wisper",this,SLOT(wisperTo()));
        foreach (QListWidgetItem *itm, userListWidget->selectedItems()) {
            if(blackListed_.contains(itm->text())){
                ctxMenu.addAction("Remove from blacklist",this,SLOT(removeFromBlacklist()));
            }else{
                ctxMenu.addAction("Add to blacklist",this,SLOT(addToBlackList()));
            }
        }
        ctxMenu.exec(globalPos);
    }
}

void JusTalkClient::answerWhisper(QString text)
{
    if(text == "/r " &&  !lastWhisper_.isEmpty() ){
        sayLineEdit->setText("@"+lastWhisper_+" ");
        sayLineEdit->setFocus();
    }
}

void JusTalkClient::addToBlackList()
{
    foreach (QListWidgetItem *item, userListWidget->selectedItems()) {
        if(item->text() != pseudo_ && !blackListed_.contains(item->text()))
            blackListed_ << item->text();
    }
    updateUserDisplay();
}

void JusTalkClient::removeFromBlacklist()
{
    foreach (QListWidgetItem *item, userListWidget->selectedItems()) {
        if(blackListed_.contains(item->text()))
            blackListed_.removeAll(item->text());
    }
    updateUserDisplay();
}

void JusTalkClient::updateUserDisplay()
{
    for(int i = 0 ; i< userListWidget->count();i++) {
        QListWidgetItem *item = userListWidget->item(i);
        if(blackListed_.contains(item->text()))
            item->setIcon(QIcon(":/icons/blocked.png"));
        else
            item->setIcon(QIcon());
    }
}

void JusTalkClient::wisperTo()
{
    //Edit the lineEdit
    foreach (QListWidgetItem *item, userListWidget->selectedItems()) {
        sayLineEdit->setText("@" + item->text() + " ");
        sayLineEdit->setFocus();
    }
}


void JusTalkClient::handlError(QAbstractSocket::SocketError er)
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
    //Read last messages
    readyRead();
}

void JusTalkClient::createActions()
{
    //The userlist : /users:a,b,cd...
    actions_.addAction("^/users:(.*)$","readUserList");

    //When kicked from the server: kicked:reason
    actions_.addAction("^/kicked:(.*)$","readKicked");

    //Receiving a whisper : whisper:from>to:message
    actions_.addAction("^/whisper:([^>]+)>([^:]+):(.*)","readWhisper");

    //A message : 'someone:message'
    actions_.addAction("^([^:]+):(.*)$","readUserMessage");
}

void JusTalkClient::readWhisper(QRegExp reg, QString)
{
    QString from = reg.cap(1);
    QString to   = reg.cap(2);
    QString msg  = reg.cap(3);

    QString title;
    if(from == pseudo_){
        lastWhisper_ = to;
        title = "[me->" + to + "]";
    }else if(to == pseudo_){
        if(!blackListed_.contains(from)){
            lastWhisper_ = from;
            title = "[" + from +"->you]";
        }
    }else{
        lastWhisper_ = "";
        title = QString("[%1->%2]").arg(from).arg(to);
    }

    if(!title.isEmpty() && !msg.isEmpty())
        writeMessage(title,msg);
}

void JusTalkClient::readKicked(QRegExp reg, QString)
{
    QString reason = reg.cap(1);
    writeMessage("You have been kicked, reason",reason);
    userListWidget->clear();
}

void JusTalkClient::login()
{
    QString hostname;
    int port;
    #if DEBUG
        pseudo_ = QString::number(rand());
        hostname = "localhost";
        port = 4200;
    #else
        InfoDialog connectionInfos(this);
        QMap<QString,QString> infos = connectionInfos.getInfos();
        connectionInfos.close();
        if(infos.isEmpty())
            return;
        pseudo_ = infos["pseudo"];
        hostname = infos["address"];
        port = infos["port"].toInt();
    #endif
    socket_->connectToHost(hostname, port);
}

// This gets called when the user clicks the sayButton (next to where
// they type text to send to the chat room):
void JusTalkClient::talkClicked()
{
    // What did they want to say (minus white space around the string):
    QString message = sayLineEdit->text().trimmed();

    // Only send the text to the chat server if it's not empty:
    if(!message.isEmpty())
    {
        socket_->write(QString(message + "\n").toUtf8());
    }

    // Clear out the input box so they can type something else:
    sayLineEdit->clear();

    // Put the focus back into the input box so they can type again:
    sayLineEdit->setFocus();
}

// This function gets called whenever the chat server has sent us some text:
void JusTalkClient::readyRead()
{
    //For each line received
    while(socket_->canReadLine())
    {
        //Get the line
        QString line = QString::fromUtf8(socket_->readLine()).trimmed();
#if DEBUG
        qDebug() << "Received " << line;
#endif
        //Trigger the action if the regex matches
        actions_.triggerActions(line,this);
    }
}


void JusTalkClient::readUserList(QRegExp reg, QString)
{
    //User list is the first group of the regex, separated by a ','.
    QStringList users = reg.cap(1).split(",");
    userListWidget->clear();
    foreach (QString user, users) {
        if(user != pseudo_)
             new QListWidgetItem(user,userListWidget);

    }
}

void JusTalkClient::writeMessage(QString author, QString message)
{
    QString style = "margin:0;";
    if(author == "Server"){
        style += "background-color:cyan;";
    }
    QString res = QString("<span style='%3' ><b>%1 : </b>%2</span>")
            .arg(author)
            .arg(message)
            .arg(style);

    roomTextEdit->append(res);
}


void JusTalkClient::readUserMessage(QRegExp reg, QString)
{
    QString user = reg.cap(1);

    if(!blackListed_.contains(user)){
        QString message = reg.cap(2);
        user = user.replace(pseudo_,"me");
        writeMessage(user,message);
    }
}

// This function gets called when our socket has successfully connected to the chat
// server.
void JusTalkClient::connected()
{
    // Send our username to the chat server.
    if(socket_->isOpen() ){
        socket_->write(QString("/me:" + pseudo_ + "\n").toUtf8());
        sayLineEdit->setFocus();
    }
}
