#ifndef JUSTALK_SERVER
#define JUSTALK_SERVER

#include <QMainWindow>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegExp>

#include "../actionlist.h"
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

    public slots:
        /**
         * @brief readUserPresentation a user's presentation
         * @param reg the regex used to detect
         *
         * Called when a user send a 'presentation' message
         * This will add the new user to the userList,
         * and send the new user list to all the connected users
         */
        void readUserPresentation(QRegExp reg,QString);

        /**
         * @brief readUserWisper read a user's whisper
         * @param reg detected !
         *
         * Called when a user wants to wisper to another user.
         * This will send the message only to the user and the concerned person
         */
        void readUserWisper(QRegExp reg, QString);

        /**
         * @brief readUserMessage read a user's message
         * @param reg message detected !
         *
         * Called when a user wants to send a message to all the users
         * This will send the message to all the connected users
         */
        void readUserMessage(QRegExp, QString);

    private slots:
        //A message was received
        void readyRead();

        //A user disconnected
        void disconnected();

        //Send user list to all users
        void sendUserList();

        /**
         * @brief incomingConnection when a user is trying to connect
         *
         * This will add the user to the connecte users's list
         */
        void incomingConnection();


        /**
         * @brief roomContextMenu show the contextMenu in the userList
         * @param p the position of the right-click
         *
         * Get the selected user and display a menu
         * where the user right clicked
         */
        void customContextMenu(QPoint p);

        /**
         * @brief kickUser kick the selected user
         */
        void kickUser();

        //Update the user list UI
        void refreshUserList();

        //Handle a socket/server error
        void handleError(QAbstractSocket::SocketError er);

    private:
        //Add the actions to call a function depending on the message
        void addActions();

        ActionList actions_;

        //The server
        QTcpServer *server_;

        //The last client that sent data
        QTcpSocket *currentClient_;

        //All the clients !
        QSet<QTcpSocket*> clients_;

        //Clients <=> Pseudo
        QMap<QTcpSocket*,QString> users_;
};

#endif
