#include "gprinter.h"

Gprinter::Gprinter(QObject *partent) :
    QThread(partent)
{
    QLibrary lib("TSCLIB.dll");
    openport=(OpenPort)lib.resolve("openport");
    sendcommand=(SendCommand)lib.resolve("sendcommand");
    clearbuffer=(ClearBuffer)lib.resolve("clearbuffer");
    setup=(SetUp)lib.resolve("setup");
    closeport=(ClosePort)lib.resolve("closeport");
    printlabel=(PrintLabel)lib.resolve("printlabel");
    windowsfont=(WindowsFont)lib.resolve("windowsfont");
    usbPortQueryPrinter=(UsbPortQueryPrinter)lib.resolve("usbportqueryprinter");
}

Gprinter::~Gprinter() {
    qDebug() << "~Gprinter";
    m_mutex.tryLock();
    m_quit = true;
    m_con.wakeOne();
    m_mutex.unlock();
    wait();
}

void Gprinter::run() {
    m_mutex.lock();
    qDebug() << "m_quit" << m_quit;
    qDebug() << "thread runing";
    QString name = m_name;
    QString link = m_link;
    m_mutex.unlock();
    QLibrary lib("TSCLIB.dll");
    if (lib.load()) {
//        qDebug() << currentThread();
//        qDebug() << &m_mutex;
        openport(name.toStdString().c_str());
        qDebug() << "Open Printer res:";
        setup(WIDTH,HEIGHT,SPEED,POTENCY,SENSOR,SPACE);
        clearbuffer();
        qDebug() << link;
        this->printQrCode(link);
        closeport();
    }
}

bool Gprinter::Open(QString &name) {
    m_mutex.lock();
    m_name = name;
    m_mutex.unlock();
    return true;
}

void Gprinter::PrintQrCode(QString &link) {
    m_mutex.lock();
    m_link = link;
    m_mutex.unlock();

    if (!isRunning()){
        qDebug() << "start()";
        start();
    }else {
        qDebug() << "m_con.wakeOne()";
        m_con.wakeOne();
    }
}
void Gprinter::printQrCode(QString &link) {
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
//    windowsfont(0,0,18,0,0,0,(unsigned char *)"Arial",(unsigned char *)"充电马绑卡卡包");
    windowsfont(120,64,60,0,0,0,"Arial",gbk->fromUnicode(COMPANY));
    QString codeLink = link;
    QString number = link.remove(0,link.size()-12);
    windowsfont(180,600,40,0,0,0,"Arial",gbk->fromUnicode(number));
    qDebug() << number;
    qDebug() << "printQrCode" << link;
//    clearbuffer();
    QString x = "134";
    QString y = "194";
    QString height = "9";
    QString deg = "0";
    QString level = "H";
    QString A = "A";
    QString M = "M2"; //M1,M2
    QString S = "S1"; // S1~S7
    QString data = "QRCODE " + x + "," + y + "," + level + "," + height +"," + A + "," + deg + "," + M + "," + S + ",\"" + codeLink + "\"";
    qDebug() << data;
    sendcommand(data.toStdString().c_str());
    printlabel("1","1");
    clearbuffer();
}
bool Gprinter::Close() {
    m_mutex.lock();
//    closeport();
    m_name = "";
    m_open = false;
    m_mutex.unlock();
    return true;
}

int Gprinter::getStatus() {
    return usbPortQueryPrinter();
}
