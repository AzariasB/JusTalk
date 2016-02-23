#ifndef JUSTALK_SERVER
#define JUSTALK_SERVER

#include <QMainWindow>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QListWidgetItem>

#include "ui_server.h"

class JustTalkServer : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT

    public:
        JustTalkServer(QObject *parent=0);
        bool listen(QHostAddress::SpecialAddress host, int port);

        virtual ~JustTalkServer()
        {
            server_->close();
            delete server_;
        }

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();
        void incomingConnection();
        void refreshUserList();

    private:        
        QTcpServer *server_;

        QSet<QTcpSocket*> clients_;
        QMap<QTcpSocket*,QString> users_;
};

#endif
