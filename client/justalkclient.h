
#ifndef JUSTALK_CLIENT
#define JUSTALK_CLIENT

#define DEBUG 1

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>


#include "../actionlist.h"
#include "ui_justalkclient.h"

/*
 * This is the MainWindow class that we have told to inherit from
 * our Designer MainWindow (ui::MainWindow)
 */
class JusTalkClient : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:

        // Every QWidget needs a constructor, and they should allow
        // the user to pass a parent QWidget (or not).
        JusTalkClient(QWidget *parent=0);

    public slots:
        // Connect to the server
        void login(const QString &hostname = "localhost");

        //The 'talk' button is clicked, or the user pressed enter
        void talkClicked();

        //A message was received
        void readyRead();

        // When the socket tells us it's connected.
        void connected();

        //Handle any socket errors
        void handlError(QAbstractSocket::SocketError er);

        /**
         * @brief roomContextMenu show the contextMenu in the userList
         * @param p the position of the right-click
         *
         * Get the selected user and display a menu
         * where the user right clicked
         */
        void roomContextMenu(QPoint p);

        /**
         * @brief readUserList
         * @param reg the regex used to detect tha is was a userList
         *
         * Called when the server send the userlist to the client
         */
        void readUserList(QRegExp reg,QString);

        /**
         * @brief readUserMessage
         * @param reg the regex used to detect that the server send a message
         *
         * Called when the server send a message (from another user or from itself)
         */
        void readUserMessage(QRegExp reg, QString);

        /**
         * @brief readKicked read the kicked message
         * @param reg the regex to know what was the reason of the kick
         */
        void readKicked(QRegExp reg,QString);

        /**
         * @brief readWhisper
         * @param reg the regex to know who's the sender, who's the receiver
         */
        void readWhisper(QRegExp reg,QString);

    private slots:
        /**
         * This function is called whenever a user right click on a user
         * (on the user list) and select 'wisper'.
         * It will add the '@PSEUDO' in the user's chat bar
         * @brief wisperTo wisper to a single user
         */
        void wisperTo();

        /**
         * This function is called when the user click on a user
         * (on the user list) and select 'add to the blacklist'
         *
         * @brief addToBlackList add a user to the blacklist
         */
        void addToBlackList();

        /**
         * @brief removeFromBlacklist remove a user from the blacklist
         *
         * Called whenever a user is already in the blaclist and the user
         * right click on the user list and select 'remove from black list'
         *
         */
        void removeFromBlacklist();

        /**
         * @brief createActions create all the key-value actions
         * to processes every received message
         */
        void createActions();

        /**
         * @brief updateUserDisplay update the list of users
         */
        void updateUserDisplay();

        /**
         * @brief answerWhisper whenever a client type '/r' in his line edit
         */
        void answerWhisper(QString text);

    private:
        ActionList actions_;

        // This is the socket that will let us communitate with the server.
        QTcpSocket *socket_;

        QString pseudo_;

        QString lastWhisper_;

        QStringList blackListed_;
};

#endif
