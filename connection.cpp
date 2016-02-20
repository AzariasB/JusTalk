

#include "connection.h"

Connection::Connection(QObject *parent):
    QTcpSocket(parent)
{
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
}

bool Connection::sendMessage(const QString &message)
{
    return sendMessage(TEXT,message);
}

bool Connection::sendMessage(MESSAGE_TYPE type, const QString &msg)
{

    QByteArray content = static_cast<qint8>(type) + msg.toUtf8();
    QByteArray wrap = content.size() + content;
    return write(wrap) == wrap.size() && flush();
}

const QString &Connection::receiveMessage()
{
    return readBuffer();
}

void Connection::handleError(QAbstractSocket::SocketError er)
{
    qDebug() << er;
}

void Connection::requestConnection(QString hostName, qint16 port)
{
    abort();
    qDebug() << "trying to connect";
    connectToHost(hostName,port);
}

QString Connection::readBuffer()
{
    if(bytesAvailable() == 0)
        return "";


    QDataStream in(readAll());

    int size;
    in >> size;
    if(size ==0)
        return "";

    int type;
    in >> type;

    //Use later
    MESSAGE_TYPE t = static_cast<MESSAGE_TYPE>(type);

    QString message;
    in >> message;
    return message;

}

Connection::~Connection()
{

}
