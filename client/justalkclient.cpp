#include "justalkclient.h"

#include <QRegExp>

JusTalkClient::JusTalkClient(QWidget *parent) :
    QMainWindow(parent)
{

    setupUi(this);

    socket_ = new QTcpSocket(this);

    login();
    createActions();

    roomTextEdit->setReadOnly(true);

    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket_, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handlError(QAbstractSocket::SocketError)));
    connect(userListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(roomContextMenu(QPoint)));
    connect(talkButton,SIGNAL(clicked()), this, SLOT(talkClicked()));
    connect(sayLineEdit,SIGNAL(returnPressed()),this,SLOT(talkClicked()));
}

void JusTalkClient::roomContextMenu(QPoint p)
{
    QPoint globalPos = userListWidget->mapToGlobal(p);

    QMenu ctxMenu;
    ctxMenu.addAction("Wisper",this,SLOT(wisperTo()));

    ctxMenu.exec(globalPos);
}

void JusTalkClient::wisperTo()
{
    foreach (QListWidgetItem *item, userListWidget->selectedItems()) {
        sayLineEdit->setText("@" + item->text() + " ");
        sayLineEdit->setFocus();
    }
}


void JusTalkClient::handlError(QAbstractSocket::SocketError er)
{
    qDebug() << er;
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

void JusTalkClient::createActions()
{
    actions_.addAction("^/users:(.*)$","readUserList");
    actions_.addAction("^([^:]+):(.*)$","readUserMessage");
}

void JusTalkClient::login(const QString &hostname)
{

    socket_->connectToHost(hostname, 4200);
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
    while(socket_->canReadLine())
    {
        QString line = QString::fromUtf8(socket_->readLine()).trimmed();
        actions_.triggerActions(line,this);
    }
}


void JusTalkClient::readUserList(QRegExp reg, QString userList)
{
    qDebug() << userList;
    QStringList users = reg.cap(1).split(",");
    userListWidget->clear();
    foreach (QString user, users) {
        new QListWidgetItem(user,userListWidget);
    }
}

void JusTalkClient::readUserMessage(QRegExp reg, QString str)
{
    QString user = reg.cap(1);
    QString message = reg.cap(2);
    user = user.replace(pseudo_,"me");
    roomTextEdit->append("<b>" + user + "</b>: " + message);
}

// This function gets called when our socket has successfully connected to the chat
// server.
void JusTalkClient::connected()
{
#if DEBUG
    pseudo_ = QString::number(rand());
#else
    pseudo_ = QInputDialog::getText(this,"Pseudo","Choose your pseudo");
#endif
    // Send our username to the chat server.
    socket_->write(QString("/me:" + pseudo_ + "\n").toUtf8());
    sayLineEdit->setFocus();
}
