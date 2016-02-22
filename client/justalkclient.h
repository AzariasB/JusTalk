
#ifndef JUSTALK_CLIENT
#define JUSTALK_CLIENT

#define DEBUG 1

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

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

        void talkClicked();

        void readyRead();

        // When the socket tells us it's connected.
        void connected();

        //Handle any socket errors
        void handlError(QAbstractSocket::SocketError er);

        void roomContextMenu(QPoint p);

    private:

        // This is the socket that will let us communitate with the server.
        QTcpSocket *socket_;

        QString pseudo_;
};

#endif
