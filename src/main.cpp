#include <QApplication>
#include "dialogimpl.h"
//
int main(int argc, char ** argv)
{
    QApplication app( argc, argv );
    app.setQuitOnLastWindowClosed(true);
    app.setOrganizationName("phrasis");
    app.setOrganizationDomain("phrasis.jacobrideout.net");
    app.setApplicationName("Phrasis");

    DialogImpl win;
    win.show();
    return app.exec();
}
