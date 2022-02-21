#ifndef GPRINTER_H
#define GPRINTER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QWaitCondition>
#include <QLibrary>
#include <QTextCodec>
//#include "tsc.h"
#include "tsclib.h"

#define WIDTH   "80"
#define HEIGHT  "90"
#define SPEED   "10.0"
#define POTENCY "7.0"
#define SENSOR  "0"
#define SPACE   "2.0"
#define COMPANY "鼎蜀科技绑卡卡包"
//#define COMPONENT "绑卡卡包"
class Gprinter : public QThread
{
    Q_OBJECT
public:
    explicit Gprinter(QObject *partener = nullptr);
    ~Gprinter();
    bool Open(QString &name);
    void PrintQrCode(QString &link);
    bool Close();
    int getStatus();
private:
    void printQrCode(QString &link);
    void run() override;
    QMutex m_mutex;
    QWaitCondition m_con;
    QString m_link;
    bool m_quit = false;
    bool m_open = false;
    QString m_name;

    OpenPort openport;
    SendCommand sendcommand;
    ClearBuffer clearbuffer;
    SetUp setup;
    ClosePort closeport;
    PrintLabel printlabel;
    WindowsFont windowsfont;
    UsbPortQueryPrinter usbPortQueryPrinter;
};

#endif // GPRINTER_H
