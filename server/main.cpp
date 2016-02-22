#include "justalkserver.h"
#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    JustTalkServer *server = new JustTalkServer();
    bool success = server->listen(QHostAddress::Any, 4200);
    if(!success)
    {
        qFatal("Could not listen on port 4200.");
    }

    qDebug() << "Ready";

    return app.exec();
}
