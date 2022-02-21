#ifndef COMMON_H
#define COMMON_H

#include <cctype>
#include <QByteArray>

class Common
{
public:
    Common();
    static void HexStringToByte(const char * source,char * dest,int length);
    static void ByteToHexString(char * source,char * dest,int length);
    static QByteArray UintToByteArray(unsigned int number);
    static QByteArray UintToByteArrayBig(unsigned int number);
    static unsigned int ByteArrayToUInt(QByteArray &data);
};

#endif // COMMON_H
