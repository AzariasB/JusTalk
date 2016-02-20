#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpServer>
#include <QNetworkConfigurationManager>
#include <QTcpSocket>
#include <QDebug>

#include "../connection.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent =0);
    ~Server();

public slots:
    void newConnection();

private:


};

#endif // CLIENT_H
