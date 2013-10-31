#include <QApplication>
#include <QObject>
#include "mainwindow.h"
#include "VKAuth.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("List Of Friends");
    QApplication::setApplicationVersion("0.01a");

    MainWindow window;
    VKAuth vkAuth("3936584");

    QObject::connect(&vkAuth, SIGNAL( unsuccess() ),
                     &app   , SLOT( quit() )
                     );
    QObject::connect(&vkAuth, SIGNAL( success(QDomDocument) ),
                     &window, SLOT( slotSuccess(QDomDocument) )
                     );
    vkAuth.show();
    return app.exec();
}
