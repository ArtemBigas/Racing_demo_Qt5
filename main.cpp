#include <QApplication>
#include "gameplay.h"

int main(int argc, char *argv[])
{
    QApplication racing(argc,argv);

    gameplay window;

    window.setWindowTitle("Racing");
    window.show();

    return racing.exec();
}
