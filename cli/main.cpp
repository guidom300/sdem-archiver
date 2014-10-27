#include <QCoreApplication>

#include "myqobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyQObject my;

    my.sayHi();

    return a.exec();
}
