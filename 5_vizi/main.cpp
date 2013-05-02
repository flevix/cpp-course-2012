#include <QtGui/QApplication>
#include "mainwindow.h"
#include <crtdbg.h>

int main(int argc, char *argv[]) {
    int tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmp |= _CRTDBG_LEAK_CHECK_DF;
    int b = 0 ;
    {
        QApplication a(argc, argv);

        MainWindow w;
        w.show();
        b= a.exec();
    }


    _CrtSetDbgFlag(tmp);
            return b;

}
