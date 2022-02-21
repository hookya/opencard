#ifndef TSC_H
#define TSC_H

extern "C" {
    int about();
    int openport(const char *a);
    void closeport();
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
    int setup(const char *width,const char *height,const char *speed,const char *p,const char *sensor,const char *space);
    int sendcommand(const char *command);
    /**
      打印标签
     * @brief printlabel
     * @param set 个数
     * @param copy 分数
     */
    void printlabel(const char* set,const char * copy);
    void clearbuffer();
}

#endif // TSC_H
