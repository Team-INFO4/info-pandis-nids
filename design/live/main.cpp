#include "livestatus.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Livestatus w;
    w.show();
    return a.exec();
}
