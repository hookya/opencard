#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QPrinterInfo>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "tsc.h"
#include "common.h"
#include "gprinter.h"
#include "epcthread.h"

#define Baud QSerialPort::Baud57600
#define Parity QSerialPort::NoParity
#define DataBits QSerialPort::Data8
#define StopBits QSerialPort::OneStop

#define Title "开卡助手"

// 获取epc号
//#define NextEpc "http://evehicle-mg-api.ec-horse.com/api/cardbag/batchGeneratorEpcNumber"
//#define AddCardBag "http://evehicle-mg-api.ec-horse.com/api/cardbag/addNewCardBagNoInfo"
//#define LINK "http://evehicle-mg-api.ec-horse.com/cardbind?cardbagno="


//#define NextEpc "http://park-mg-api.liyanweilai.com/api/cardbag/batchGeneratorEpcNumber"
//#define AddCardBag "http://park-mg-api.liyanweilai.com/api/cardbag/addNewCardBagNoInfo"
//#define LINK "http://park-mg-api.liyanweilai.com/cardbind?cardbagno="

//scdswlkj.com
#define NextEpc "http://park-mg-api.scdswlkj.com/api/cardbag/batchGeneratorEpcNumber"
#define AddCardBag "http://park-mg-api.scdswlkj.com/api/cardbag/addNewCardBagNoInfo"
#define LINK "http://park-mg-api.scdswlkj.com/cardbind?cardbagno="

#define Token "987654321ZT"
#define MsgTime 3000

#define OpenSerial "打开串口"
#define CloseSerial "关闭串口"

#define OpenPrinter "连接打印机"
#define ClosePrinter "断开打印机"

#define CardTypeIc 1
#define CardTypeEpc 2

//About about = (About)tsclib.resolve("about");

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

protected:
    void timerEvent(QTimerEvent *);

private slots:
    void on_btn_conn_serial_clicked();
    void on_btn_refresh_serial_clicked();
    void on_btn_epc_read_clicked();
    void on_btn_epc_write_clicked();
    void on_btn_conn_print_clicked();
    void on_btn_open_card_clicked();

    void on_btn_epc_get_clicked();

    void on_edit_ic_editingFinished();

    void on_btn_reset_clicked();

    void on_enable_epc_clicked();

    void on_disable_epc_clicked();

private:
    void searchSerial();
//    QSerialPort *sport;
    Ui::MainWindow *ui;
    void setComList(QList<QString> protList);

    void showMsg(QString msg);
    bool openSerialPort();
    void closeSerialPort();
    void searchPrinter();

    void enableEpc();
    void disableEpc();

    Gprinter m_printer;
    EpcThread m_epc;
    QString m_info;
    QString epcNumber;
    QString icNumber;
    QString cardBag;
    QString m_link;
//    QNetworkRequest m_request;
    QNetworkAccessManager m_manager;
    bool epcWriteSuc = false;
    bool epcReadSuc = false;
    bool icReadSuc = false;
    // 默认不开epc卡
    bool m_enable_epc = false;
    void reset();

};
#endif // MAINWINDOW_H
