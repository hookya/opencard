#ifndef EPCTHREAD_H
#define EPCTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QSerialPort>
#include <QDebug>
#include <QWaitCondition>
#include "common.h"

#define PRESET_VALUE  0xFFFF
#define PLOY_NORMAL   0x8408
#define ADR           0xFF
#define READ_EPC      0x01
#define WRITE_EPC     0x04
#define COM_LEN       4
#define RESP_COM_LEN  5
#define NUM_BYTE_LEN  4
#define PASS_WORD     0x00

#define Baud QSerialPort::Baud57600
#define Parity QSerialPort::NoParity
#define DataBits QSerialPort::Data8
#define StopBits QSerialPort::OneStop

class EpcThread : public QThread
{

    Q_OBJECT
public:
    EpcThread(QObject *partener = nullptr);
    ~EpcThread();
    bool Open(QString &name);
    void Close();
    void ReadEpc();
    void WriteEpc(QString &number);
signals:
    void error(QString &msg);
    void success(uint32_t type,QByteArray &data);
    void write_success();
    void read_success(QByteArray &data);
private:
    void crc16(QByteArray *bytes);
    QByteArray handlerData(char cmd,char addr,QByteArray *data = nullptr);
    void run() override;
    QMutex m_mux;
    QString portName;
    QSerialPort serialPort;
    bool m_open = false;
    bool m_quit = false;
    QByteArray m_data;
    QWaitCondition m_cond;
};

#endif // EPCTHREAD_H
