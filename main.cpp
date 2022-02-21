#include "mainwindow.h"

#include <QApplication>
#include <QPrinterInfo>
//#include "common.h"


int main(int argc, char *argv[])
{

//    char c[4] = {0};
    QApplication a(argc, argv);
//    char s[9] = {0};
//    Common::HexStringToByte("2259cfca",c,8);
//    Common::ByteToHexString(c,s,sizeof (c)/sizeof (c[0]));
//    qDebug() << "HexStringToByte = " << s;

    MainWindow w;

    w.show();
    return a.exec();
}
