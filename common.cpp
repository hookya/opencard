#include "common.h"

Common::Common()
{

}

void Common::HexStringToByte(const char * source,char * dest,int length) {

    // 如果长度是单数，推出循环
    if (length%2) {
        return;
    }
    unsigned char h,l;
    for (int i = 0; i < length; i+=2) {
        h = ::toupper(source[i]);
        l = ::toupper(source[i+1]);
        if (h > 0x39)
//            h = h - 'A' - 10;
            h = h - 0x37;
        else
            h = h - 0x30;

        if (l > 0x39)
            l = l - 0x37;
        else
            l = l - 0x30;
        dest[i/2] = (h<<4)|l;
    }
    return;
}

void Common::ByteToHexString(char *source, char *dest,int length) {
//    int len = sizeof (source) /sizeof (source[0]);
    unsigned char h,l;
    for (int i = 0; i < length; i++) {
        unsigned char temp = source[i];
        h = temp >> 4;
        l = temp&0xf;
        if (h > 0x9)
            h = h + 0x37;
        else
            h = h + 0x30;

        if (l > 0x9)
            l = l + 0x37;
        else
            l = l + 0x30;
        dest[i*2] = h;
        dest[i*2+1] = l;
    }
    dest[length*2] = '\0';
    return;
}

QByteArray Common::UintToByteArray(unsigned int number) {
    QByteArray data;
    data.clear();
    data.resize(0);
    for (int i = 0;i < 4;i++) {
        data.prepend(0xFF&(number>>(i*8)));
    }
    return data;
}

QByteArray Common::UintToByteArrayBig(unsigned int number) {
    QByteArray data;
    data.clear();
    data.resize(0);
    for (int i = 0;i < 4;i++) {
        data.append(0xFF&(number>>(i*8)));
    }
    return data;
}

unsigned int ByteArrayToUInt(QByteArray &data) {
    unsigned int value = 0;
    for (int i = 0;i < 4;i++) {
        value = (value>>(i*8))|(unsigned char)data.at(i);
    }
    return value;
}
