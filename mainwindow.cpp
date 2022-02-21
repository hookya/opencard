#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 所有可用的串口
    this->searchSerial();
    this->searchPrinter();
    this->init();

    connect(&m_epc,&EpcThread::error,[=](QString &msg){
       m_info = msg;
    });
    connect(&m_epc,&EpcThread::write_success,[=](){
       epcWriteSuc = true;
       m_info = "write epc success";
    });
    connect(&m_epc,&EpcThread::read_success,[=](QByteArray &data){
       m_info = "read epc success";
       qDebug() << data.toHex(' ').toUpper();
    });
    connect(&m_manager,&QNetworkAccessManager::finished,[=](QNetworkReply *reply){
        if(reply->error()!=QNetworkReply::NoError){
            m_info = reply->errorString();
        }else {
            const QByteArray reply_data=reply->readAll();
            //解析json
            QJsonParseError json_error;
            QJsonDocument document = QJsonDocument::fromJson(reply_data,&json_error);
            if (json_error.error == QJsonParseError::NoError) {
                if (document.isObject()) {
                    const QJsonObject obj = document.object();
                    qDebug() << obj;
                    if (obj.contains("code")) {
                        const QJsonValue code = obj.value("code");
                        const QJsonValue msg = obj.value("msg");
                        if (code.toInt() == 0) {
                            if (obj.contains("data")) {
                                const QJsonObject data = obj.value("data").toObject();
                                if (reply->url() == QUrl(NextEpc)) {
                                    if (data.contains("cardBag") && data.contains("epc")) {
                                        cardBag = data.value("cardBag").toString();
                                        epcNumber = data.value("epc").toString();
                                        ui->label_epc_number->setText(epcNumber);
                                        ui->label_card_bag_number->setText(cardBag);
                                        m_info = "get epc cardBag number success";
                                    }else {
                                        m_info = "requet server resposne error";
                                    }
                                }else if(reply->url() == QUrl(AddCardBag)) {
                                    if (m_link.isEmpty()) {
                                        m_info = "sys error,url is empty";
                                    }else {
                                        m_printer.PrintQrCode(m_link);
                                        this->reset();
                                        qDebug() << AddCardBag;
                                    }
                                }else {
                                    m_info = "unsupport url";
                                }
                            }else {
                                m_info = "http request error no response data";
                            }

                        }else {
                            m_info = QString("request error code %1 msg %2").arg(code.toInt()).arg(msg.toString());
                        }
                    }else {
                        m_info = "http request error no response code";
                    }

                }else
                    m_info = "http request error response not a object";
            }else{
                qDebug()<<"json error:"<<json_error.errorString();
            }

        }
    });
    startTimer(100);
}

void MainWindow::init() {
    // 设置标题
    this->setWindowTitle(Title);
    // 设置icon
    this->setWindowIcon(QIcon(":/resources/icons/icon.png"));
    ui->btn_conn_serial->setText(tr(OpenSerial));
    ui->btn_conn_print->setText(tr(OpenPrinter));
//    sport = new QSerialPort(this);
    //qDebug() << "open port " << openport((char *)"COM3");
    // 设置单选按钮
    ui->enable_epc->setChecked(false);
    ui->disable_epc->setChecked(true);
    this->disableEpc();
}

void MainWindow::enableEpc(){
    qDebug() << "enableEpc";
    // 设置按钮启用
    ui->btn_epc_write->setEnabled(true);
    ui->btn_epc_read->setEnabled(true);
    ui->btn_epc_cancel->setEnabled(true);



    this->m_enable_epc = true;
}

void MainWindow::disableEpc(){
    qDebug() << "disableEpc";

    ui->btn_epc_write->setDisabled(true);
    ui->btn_epc_read->setDisabled(true);
    ui->btn_epc_cancel->setDisabled(true);

    this->m_enable_epc = false;
}

MainWindow::~MainWindow()
{
//    if (sport != nullptr) {
//        this->closeSerialPort();
//    }
    delete ui;
}

void MainWindow::searchSerial() {
    QList<QString> portList;
    foreach(const QSerialPortInfo &portInfo,QSerialPortInfo::availablePorts()) {
        QSerialPort port;
        port.setPort(portInfo);
        if (port.open(QSerialPort::ReadWrite)) {
            portList.append(portInfo.portName());
        }
        port.close();
    }

    // 更新ui
    this->setComList(portList);
}

void MainWindow::setComList(QList<QString> protList) {
    ui->com_ports->clear();
    foreach (QString portName,protList) {
        ui->com_ports->addItem(portName);
    }
}

void MainWindow::on_btn_conn_print_clicked() {
    if (ui->btn_conn_print->text() == OpenPrinter) {
        QString name = "Gprinter  GP-1324D";
        if(m_printer.Open(name))
            ui->btn_conn_print->setText(tr(ClosePrinter));
    }else {
        ui->btn_conn_print->setText(tr(OpenPrinter));
        m_printer.Close();
    }

}

void MainWindow::on_btn_open_card_clicked() {
    // TODO 检测打印机状态 tsclib返回-1问题未知
//    int res = m_printer.getStatus();
//    qDebug() << "printer status res:" << res;
//    if (res != 0x00) {
//        QString msg;
//        switch(res) {
//        case 0x01:
//            msg = "印字头开放";
//            break;
//        case 0x02:
//            msg = "卡纸";
//            break;
//        case 0x03:
//            msg = "卡纸并开放印字头";
//            break;
//        case 0x04:
//            msg = "缺纸";
//            break;
//        case 0x05:
//            msg = "缺纸并开放印字头";
//            break;
//        case 0x08:
//            msg = "缺碳带";
//            break;
//        case 0x09:
//            msg = "缺碳带并开放印字头";
//            break;
//        case 0x0A:
//            msg = "缺碳带并卡纸";
//            break;
//        case 0x0B:
//            msg = "缺碳带并卡纸及印字头开放";
//            break;
//        case 0x0C:
//            msg = "缺碳带并缺纸";
//            break;
//        case 0x0D:
//            msg = "缺碳带并缺纸及印字头开放";
//            break;
//        case 0x10:
//            msg = "暂停";
//            break;
//        case 0x20:
//            msg = "列队中";
//            break;
//        }
//        qDebug() << msg;
//        return;
//    }
    if (!icReadSuc) {
        m_info = "没有IC卡号";
        return;
    }
    // 只有启用epc功能，才去检查epc有没有正确写入
    if (this->m_enable_epc) {
        if (!epcWriteSuc) {
            m_info = "EPC号没有成功写入";
            return;
        }
    }

    QNetworkRequest request;
    request.setUrl(QUrl(AddCardBag));
    request.setRawHeader("Content-Type","application/json;charset=utf-8");
    QJsonArray cards;
    QJsonObject ic;
    ic.insert("cardNo",icNumber);
    ic.insert("cardType",CardTypeIc);
    cards.append(ic);
    QJsonObject epc;
    epc.insert("cardNo",epcNumber);
    epc.insert("cardType",CardTypeEpc);
    cards.append(epc);
    QJsonObject data;
    data.insert("cardBagNo",cardBag);
    data.insert("token",Token);
    data.insert("cards",cards);
    QJsonDocument doc;
    doc.setObject(data);
    QByteArray post_array = doc.toJson(QJsonDocument::Compact);
    m_link = LINK;
    m_link.append(cardBag);
    qDebug() << m_link;
    m_manager.post(request,post_array);
}

void MainWindow::showMsg(QString msg)  {
    m_info = msg;
}

void MainWindow::on_btn_conn_serial_clicked() {

    if (ui->btn_conn_serial->text() == OpenSerial) {
        if (this->openSerialPort()) {
            ui->btn_conn_serial->setText(tr(CloseSerial));
            ui->com_ports->setEnabled(false);
            showMsg("open epc succ");
        }else {
            showMsg("open epc fail");
        }
    }else {
        showMsg("close epc succ");
        ui->btn_conn_serial->setText(tr(OpenSerial));
        ui->com_ports->setEnabled(true);
        this->closeSerialPort();
    }

    qDebug() << "on_btn_conn_serial_clicked";
}

void MainWindow::on_btn_refresh_serial_clicked() {
    this->searchSerial();
    this->searchPrinter();
    qDebug() << "on_btn_refresh_serial_clicked";
}
//@decaler
void MainWindow::searchPrinter() {
//    ui->com_print_ports->clear();
//    QList<QPrinterInfo> list = QPrinterInfo::availablePrinters();
//    for (int i = 0 ;i<list.size();i++)
//    {
//        //ui->com_print_ports->addItem(list[i].printerName());
//    }
}

bool MainWindow::openSerialPort() {
    QString name = ui->com_ports->currentText();
    return m_epc.Open(name);
}

void MainWindow::closeSerialPort() {
    //sport->close();
}

void MainWindow::on_btn_epc_read_clicked() {
    qDebug() << "on_btn_refresh_serial_clicked";
    m_epc.ReadEpc();
}

void MainWindow::on_btn_epc_write_clicked() {
    QString number = epcNumber;
    if (number.isEmpty()) {
        m_info = "请先获取EPC号码";
        return;
    }
    m_epc.WriteEpc(number);
}

void MainWindow::on_enable_epc_clicked() {
    this->enableEpc();
}

void MainWindow::on_disable_epc_clicked() {
    this->disableEpc();
}

void MainWindow::timerEvent(QTimerEvent *) {
    ui->statusbar->showMessage(m_info);
    update();
}

void MainWindow::on_btn_epc_get_clicked(){
    qDebug() << "on_btn_epc_get_clicked";
    QNetworkRequest request;
    request.setUrl(QUrl(NextEpc));
    request.setRawHeader("Content-Type","application/json");
    m_manager.get(request);
}

void MainWindow::on_edit_ic_editingFinished()
{
    QByteArray sourceData = Common::UintToByteArrayBig(ui->edit_ic->text().toUInt());
//    auto number = Common::ByteArrayToUInt(sourceData);
    unsigned int number = 0;
    for (int i = 0;i < 4;i++) {
        number = number<<8;
        number = number|(unsigned char)sourceData.at(i);
    }
    qDebug() << sourceData.toHex(' ').toUpper();
    qDebug() << "on_edit_ic_editingFinished";

    QString numberString = QString::number(number);
    int size = 10 - numberString.size();
    qDebug() << size;
    for(int i = 0;i < size;i++) {
        numberString.prepend('0');
        qDebug() << numberString;
    }
    numberString.prepend('0');
    numberString.prepend('1');
    icNumber = numberString;
    icReadSuc = true;
    ui->label_ic_number->setText(numberString);
}

void MainWindow::reset(){
    icNumber = "";
    icReadSuc = false;
    ui->label_ic_number->setText("");

    epcNumber = "";
    epcReadSuc = false;
    ui->label_epc_number->setText("");

    cardBag = "";
    ui->label_card_bag_number->setText("");

    ui->edit_ic->setText("");

    m_link = "";
}

void MainWindow::on_btn_reset_clicked(){
    this->reset();
}
