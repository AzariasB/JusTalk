#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client();
    ~Client();

public slots:
    void readMessage();
    void handleError(QAbstractSocket::SocketError);
    void requestConnection(QString hostName,qint16 adress);

private:
    //Ui::MainWindow *ui;
    QTcpSocket *socket_;

    qint16 blockSize_;

};

#endif // CLIENT_H
