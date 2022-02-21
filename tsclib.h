#ifndef TSCLIB_H
#define TSCLIB_H
extern "C" {
    typedef __stdcall int (*About)(); // 显示DLL版本号
    typedef __stdcall void (*OpenPort)(const char *); // 打开电脑端的端口
    typedef __stdcall void (*ClosePort)(); // 关闭电脑端的端口
    /**
     * 说明：设定标签的宽度，高度，打印速度，打印浓度，感应器类别
     * 参数：
     * a: 字符串型别，设定标签宽度，单位 mm
     * b: 字符串型别，设定标签高度，单位 mm
     * c: 字符串型别，设定打印速度，(打印速度随机型不同而有不同的选项) 1.0: 每秒1.0 英寸打印速度 1.5: 每秒1.5 英寸打印速度 2.0: 每秒2.0 英寸打印速度 3.0: 每秒3.0 英寸打印速度 4.0: 每秒4.0 英寸打印速度 5.0: 每秒5.0 英寸打印速度 6.0: 每秒6.0 英寸打印速度
     * d: 字符串型别，设定打印浓度， 0~15，数字越大打印结果越黑
     * e: 字符串型别，设定使用传感器类别 0 表示使用垂直间距传感器(gap sensor) 1 表示使用黑标传感器(black mark sensor)
     * f: 字符串型别，设定gap/black mark 垂直间距高度，单位: mm g: 字符串型别，设定gap/black mark 偏移距离，单位: mm，此参数若使用一般标签 时均设为0
     */
    typedef __stdcall int (*SetUp)(const char * a,const char * b,const char * c,const char * d,const char * e,const char * f);

    /**
     * 清除
     */
    typedef __stdcall void (*ClearBuffer)();

    /**
     * 说明: 使用条形码机内建条形码打印
     * 参数:
     * a: 字符串型别，条形码X 方向起始点，以点(point)表示。 (200 DPI，1 点=1/8 mm, 300 DPI，1 点=1/12 mm)
     * b: 字符串型别，条形码Y 方向起始点，以点(point)表示。 (200 DPI，1 点=1/8 mm, 300 DPI，1 点=1/12 mm)
     * c: 字符串型别， 128
     *  Code 128, switching code subset A, B, C automatically 128M
     *  Code 128, switching code subset A, B, C manually. EAN128
     *  Code 128, switching code subset A, B, C automatically 25
     *  Interleaved 2 of 5 25C
     *  Interleaved 2 of 5 with check digits 39
     *  Code 39 39C
     *  Code 39 with check digits 93
     *  Code 93 EAN13
     *  EAN 13 EAN13+2
     *  EAN 13 with 2 digits add-on EAN13+5
     *  EAN 13 with 5 digits add-on EAN8
     *  EAN 8 EAN8+2
     *  EAN 8 with 2 digits add-on EAN8+5
     *  EAN 8 with 5 digits add-on CODA
     *  Codabar POST
     *  Postnet UPCA
     *  UPC-A UPCA+2
     *  UPC-A with 2 digits add-on UPCA+5
     *  UPC-A with 5 digits add-on UPCE
     *  UPC-E UPCE+2
     *  UPC-E with 2 digits add-on UPCE+5
     *  UPC-E with 5 digits add-on
     * d: 字符串型别，设定条形码高度，高度以点来表示
     * e: 字符串型别，设定是否打印条形码码文 0: 不打印码文 1: 打印码文
     * f: 字符串型别，设定条形码旋转角度 0: 旋转0 度 90: 旋转90 度 180: 旋转180 度 270: 旋转270 度
     * g: 字符串型别，设定条形码窄bar 比例因子，请参考 编程手册
     * h: 字符串型别，设定条形码窄bar 比例因子，请参考 编程手册
     * i: 字符串型别，条形码内容
     */
    typedef __stdcall void (*BarCode)(const char * a,const char * b,const char * c,const char * d,const char * e,const char * f,const char * g,const char * h,const char * i);

    /**
     * 说明: 使用条形码机内建文字打印
     * 参数:
     *  a: 字符串型别，文字X 方向起始点，以点(point)表示。 (200 DPI，1 点=1/8 mm, 300 DPI，1 点=1/12 mm)
     *  b: 字符串型别，文字Y 方向起始点，以点(point)表示。 (200 DPI，1 点=1/8 mm, 300 DPI，1 点=1/12 mm)
     *  c: 字符串型别，内建字型名称，共8 种。
     *      1: 8*12 dots
     *      2: 12*20 dots
     *      3: 16*24 dots
     *      4: 24*32 dots
     *      5: 32*48 dots TST24.BF2: 繁体中文24*24 TSS24.BF2: 简体中文24*24 K: 韩文 24*24
     *  d: 字符串型别，设定文字旋转角度 0: 旋转0 度 90: 旋转90 度 180: 旋转180 度 270: 旋转270 度
     *  e: 字符串型别，设定文字X 方向放大倍数，1~8
     *  f: 字符串型别，设定文字X 方向放大倍数，1~8
     *  g: 字符串型别，打印文字内容
     */
    typedef __stdcall int (*PrintFont)(const char * a,const char * b,const char * c,const char * d,const char * e,const char * f,const char * g);

    /**
     *  说明: 送内建指令到条形码打印机
     *  参数: 详细指令请参考编程手册
     */
    typedef __stdcall int (*SendCommand)(const char * a);

    /**
     * 说明: 打印标签内容
     * 参数:
     *  a: 字符串型别，设定打印标签个数(set)
     *  b: 字符串型别，设定打印标签份数(copy)
     */
    typedef __stdcall int (*PrintLabel)(const char * a,const char * b);

    /**
     *  说明:下载单色PCX 格式图文件至打印机
     *  参数:
     *      a: 字符串型别，文件名(可包含路径)
     *      b: 字符串型别，下载至打印机内存内之文件名(请使用大写文件名)
     */
    typedef __stdcall int (*DownLoadPcx)(const char * a,const char * b);

    /**
     *  说明: 跳页，该函数需在setup 后使用
     */
    typedef __stdcall void (*FormFeed)();

    /**
     *  说明: 设定纸张不回吐
     */
    typedef __stdcall void (*NoBackFeed)();

    /**
     *  说明: 使用Windows TTF 字型打印文字
     *  参数:
     *      a: 整数型别，文字X 方向起始点，以点(point)表示。
     *      b: 整数型别，文字Y 方向起始点，以点(point)表示。
     *      c: 整数型别，字体高度，以点(point)表示。
     *      d: 整数型别，旋转角度，逆时钟方向旋转 0 -> 0 degree 90-> 90 degree 180-> 180 degree 270-> 270 degree
     *      e: 整数型别，字体外形 0-> 标准(Normal) 1-> 斜体(Italic) 2-> 粗体(Bold) 3-> 粗斜体(Bold and Italic)
     *      f: 整数型别, 底线 0-> 无底线 1-> 加底线
     *      g: 字符串型别，字体名称。如: Arial, Times new Roman, 细名体, 标楷体
     *      h: 字符串型别，打印文字内容
     */
    typedef __stdcall int (*WindowsFont)(int a,int b,int c,int d,int e,int f,const char * g,const char * h);

    /**
     * 说明: 查询标签打印机状态
     * 参数: 无
     */
    typedef __stdcall int (*UsbPortQueryPrinter)();
}
#endif // TSCLIB_H

