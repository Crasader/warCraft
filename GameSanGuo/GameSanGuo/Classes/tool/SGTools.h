//
//  SGTools.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-8.
//
//

#ifndef __GameSanGuo__SGTools__
#define __GameSanGuo__SGTools__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

//各种版本的设备
enum TargetDevice {
    
    kDeviceIphone=0,
    kDeviceIphone4,
    kDeviceIphone5,
    kDeviceIpad,
    KDeviceIpadRetina,
    kDeviceNull
};


class SGTools : public CCObject
{
public:
    //通过时间戳获取时间字符串
    static std::string getTimeStringByTimeStamp(long long timeStamp);
    
    //判断是否是中文字符
    static bool charIsChinese(char ch);
    
    //获得一个汉字的宽度
    static float getWidthOfOneChineseWord(const char* fontName,int fontSize);
    
    //获得一个英文字母的宽度
    static float getWidthOfOneEnglishWord(char ch,const char* fontName,int fontSize);
    
    //获得一个汉字的高度
    static float getHeightOfOneWord(const char* fontName ,int fontSize );

    //获取一个时间戳距离现在的时间长度
    static std::string getTimeDistanceNow(long long timeStamp);
    
    //获取当前设备
    static TargetDevice getDeviceNow();
    
    /*根据物品品质返回 名字颜色 ， 品质区间[1-6]*/
    static ccColor3B getColorByQuality(int quality);
    
    //往buffer里写入倒计时，形如01:02:03。
    static void calClockString(int leftTime,char buffer[]);

};

//暗亮点点点控件。
class PointLayer : public CCLayer
{
    public :
    PointLayer(int arrSize);
    ~PointLayer();
    //返回layer，返回的layer需要设置位置
    //参数：点的总数 、总横间距、暗点文件名称、亮点
    static PointLayer * create(int count,float width ,const char * fileNormal,const char * fileLight);
    
    void inital(int count,float width ,const char * fileNormal,const char * fileLight);
    //设置亮点位置，注意传入order的是自然数！从1开始！
    void setLight(int order);
private:
    int _count;
    int _width;
    CCSprite * _light;
    CCArray * _arrayNormal;
    
};


#endif /* defined(__GameSanGuo__SGTools__) */
