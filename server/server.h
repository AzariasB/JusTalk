#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpServer>
#include <QNetworkConfigurationManager>
#include <QTcpSocket>
#include <QDebug>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent =0);
    ~Server();

public slots:
    void newConnection();

private:
  //  Ui::MainWindow *ui;
    QTcpServer *server_;


};

#endif // CLIENT_H
