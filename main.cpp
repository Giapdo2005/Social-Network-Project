#include "socialnetworkwindow.h"

#include <QApplication>
#include "network.h"
#include "socialnetworkwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SocialNetworkWindow w;
    w.show();
    return a.exec();
}
