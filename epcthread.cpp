#include "epcthread.h"

EpcThread::EpcThread(QObject *parenter) :
    QThread(parenter)
{

}

EpcThread::~EpcThread() {
    m_mux.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mux.unlock();
    wait();
}

bool EpcThread::Open(QString &name) {
    QMutexLocker locker(&m_mux);
    portName = name;
    return true;
}

void EpcThread::Close() {
    QMutexLocker locker(&m_mux);
    if (serialPort.isOpen()) {
        serialPort.close();
        m_open = false;
    }
}

void EpcThread::WriteEpc(QString &number) {
    QMutexLocker locker(&m_mux);
    auto real_number = number.remove(0,2).toUInt();
    // 重置待发送的数据
    m_data.resize(0);
    auto num_data = Common::UintToByteArray(real_number);
    QByteArray data;
    // 卡号的长度
    data.append(0x02);
    // 密码
    data.append(Common::UintToByteArray(PASS_WORD));
    // 卡号
    data.append(num_data);
    m_data = handlerData(WRITE_EPC,ADR,&data);
    qDebug() << "write epc :" << m_data.toHex(' ').toUpper();
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void EpcThread::ReadEpc() {
    QMutexLocker locker(&m_mux);

    m_data = handlerData(READ_EPC,ADR);
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void EpcThread::run() {
    int times;
    while(!m_quit) {
        m_mux.lock();
        QByteArray sendData = m_data;
        QString name = portName;
        QSerialPort *serial = new QSerialPort();
        times = 0;
        QString msg;
        serial->setPortName(name);
        serial->setParity(Parity);
        serial->setStopBits(StopBits);
        serial->setBaudRate(Baud);
        serial->setDataBits(DataBits);
        m_open = serial->open(QSerialPort::ReadWrite);
        if(m_open) {
            while(true) {
                if (times > 50) {
                    break;
                }
                times++;
                serial->write(sendData);
                if(serial->waitForReadyRead(1000)) {
                       QByteArray data = serial->readAll();
                       int respLen = data.at(0);
                       if (respLen != (data.size()-1)) {
                           msg = "read size err data[0]:";
                           emit error(msg);
                           continue;
                       }
                       // 执行的cmd 命令
                       unsigned char cmd = (unsigned char)data.at(2);
                       // 返回的状态
                       unsigned char status = (unsigned char)data.at(3);

                       qDebug() << data.toHex(' ').toUpper();
                       if (status == 0x00 || status == 0x01) {
                           if (cmd == WRITE_EPC) {
                               emit write_success();
                           }else if(cmd == READ_EPC) {
                               if ((respLen - RESP_COM_LEN) < 1) {
                                   msg = "read resp len error";
                                   emit error(msg);
                                   continue;
                               }
                               if ((unsigned char)data.at(4) != 1) {
                                   msg = "found over one epc card found";
                                   emit error(msg);
                                   continue;
                               }
                               QByteArray num;
                               for (int i = 0; i < 4; i++) {
                                   num.append(data.at(6+i));
                               }
                               emit read_success(num);
                           }else {
                               qDebug() << "unsport cmd";
                           }
                           // 查询到正确的数据，直接break跳出死循环
                           break;
                       }
                       switch (status) {
                       case 0x02:
                           msg = "read size err data[0]:";
                           break;
                       case 0x04:
                           msg = "memory out flover";
                           break;
                       case 0xF9:
                           msg = "execute cmd error";
                           break;
                       case 0xFA:
                           msg = "have epc but poor communication";
                           break;
                       case 0xFB:
                           msg = "no epc card";
                           break;
                       case 0xFD:
                           msg = "command data len error";
                           break;
                       case 0xFE:
                           msg = "an illegal order";
                           break;
                       case 0xFF:
                           msg = "paramter error";
                           break;
                       default:
                           msg = "unkonw error";
                       }
                       emit error(msg);
                }else {
                    msg = "read timeout";
                    emit error(msg);

                }
                // 线程休眠100ms
                QThread::msleep(100);
            }

        }
        else {
            msg = "请打开正确的端口";
            emit error(msg);
        }
        // 关闭端口

        if (serial->isOpen()) {
            serial->clear();
            serial->close();
        }
        m_cond.wait(&m_mux);
        m_mux.unlock();
    }

}

QByteArray EpcThread::handlerData(char cmd,char addr,QByteArray *data){
    unsigned char dataLen = (data == nullptr)?0:data->size();
    QByteArray d;
    char len = COM_LEN + dataLen;
    // 长度
    d.append(len);
    // 地址
    d.append(addr);
    // 命令
    d.append(cmd);
    // 数据
//    d.append(data);
    if (data != nullptr && dataLen>0) {
        d.append(*data);
    }
    // crc16 校验码
    crc16(&d);
    return d;
}

void EpcThread::crc16(QByteArray *bytes){
    unsigned short int value = PRESET_VALUE;
    for(int i = 0; i < bytes->size(); i++) {
        value = value ^ (unsigned char)bytes->at(i);
        for(int j = 0; j < 8; j++) {
            if (value & 0x0001)
                value = (value>>1)^PLOY_NORMAL;
            else
                value = value>>1;

        }
    }
    bytes->append(value&0xFF);
    bytes->append(value>>8);
}
