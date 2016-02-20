
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    explicit Connection(QObject *parent =0);
    ~Connection();

    enum MESSAGE_TYPE{
        GREETING,
        TEXT,
        CLOSING
    };

    bool sendMessage(const QString &message);

    bool sendMessage(MESSAGE_TYPE type, const QString &msg);

    const QString &receiveMessage();

    void requestConnection(QString hostName,qint16 port);

public slots:
    void handleError(QAbstractSocket::SocketError er);

private:
    QString readBuffer();



};

#endif // CONNECTION_H
