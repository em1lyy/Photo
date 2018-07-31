#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (argc == 2)
    {
        std::string path(argv[1]);
        w.loadImage(path);
    }
    else
    {
        std::cout << "ARGUMENT LENGTH MUST BE 1!\n EXITING!" << endl;
        return 1;
    }

    return a.exec();
}
