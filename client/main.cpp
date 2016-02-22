
#include "justalkclient.h"
#include <QApplication>
#include "time.h"

/*
 * This is your main() function. Very simple.
 */
int main(int argc, char **argv)
{
    //Reset timer
    srand(time(NULL));

    // Every Qt GUI needs a QApplication.
    QApplication app(argc, argv);

    // This is your MainWindow that you created with Designer
    // Declare it and show it.
    JusTalkClient mainWindow;
    mainWindow.show();

    // This will not return until the last window is closed.
    // This runs the GUI's event loop handling painting and
    // user input and stuff like that.
    return app.exec();
}
