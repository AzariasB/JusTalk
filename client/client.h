#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include "../connection.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = 0);
    ~Client();


private:
    Connection *serverConnection_;

};

#endif // CLIENT_H
