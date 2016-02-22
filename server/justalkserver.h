#ifndef JUSTALK_SERVER
#define JUSTALK_SERVER

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

class JustTalkServer : public QTcpServer
{
    Q_OBJECT

    public:
        JustTalkServer(QObject *parent=0);

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();

    protected:
        void incomingConnection(int socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
};

#endif
