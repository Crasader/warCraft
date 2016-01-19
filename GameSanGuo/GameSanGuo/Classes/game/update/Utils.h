//
//  Utils.h
//  WarCraftCardGame
//
//  Created by orix on 12/10/12.
//
//

#ifndef WarCraftCardGame_Utils_h
#define WarCraftCardGame_Utils_h

#include "cocos2d.h"

USING_NS_CC;

enum {
	SNSDeviceTypeINVALID = 0,
	SNSDeviceTypeIphone3gs,
	SNSDeviceTypeIphone4,
	SNSDeviceTypeIphone5,
	SNSDeviceTypeIpad,
};
typedef int SNSDeviceType;

typedef struct _tTimeType {
	int hour;   //小时
	int minute; //分钟
	int second; //秒
} tTimeType;

//判断读取合适的资源的资源
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define getResource(_file_, _dir_) (getFileResource(_file_))
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define getResource(_file_, _dir_) (getAndroidFileResource(_file_, _dir_))
#endif

// 判断指针的类型是否是指定类（这里有前提，父类中必须有虚函数，请注意使用）
#define isKindOfClass(_pointer_, _class_) (dynamic_cast<_class_*>(_pointer_) != NULL)

// 判断指针指向的类是否与指定类相符（同一个类）
#define isClass(_pointer_, _class_) (typeid(*_pointer_) == typeid(_class_))

//重写log
#ifdef DEBUG
# define log(fmt, ...) CCLOG(("%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
# define log(...);
#endif

// 设置deviceType
void setDeviceType(float width, float height);
// 返回是否是ipad(old and new)
bool isiPad();
// 返回是否是iphone5
bool isIphone5();
// 返回是否是newPad
bool isNewPad();

CCNode* initCustomFrame(CCPoint _point, CCSize _ccSize, int _bgType, int _frameType,
                        int _adornT, int _adornTType, int _adornTX, int _adornTY,
                        int _adornB, int _adornBType, int _adornBX, int _adornBY,
                        int _cornerLT, int _cornerRT, int _cornerLB, int _cornerRB,
                        int _edgeInby,int _edgeL, int _edgeR, int _edgeT, int _edgeB);

/**
 *  生成矩形纹理
 *  size:矩形大小
 *  rect:矩形颜色
 *  edge:边线颜色
 *  position:矩形坐标（锚点为中心点）
 *  edgeW:边线宽度
 */
CCSprite* initRectSprite(CCSize size,ccColor4B rect,ccColor4B edge,CCPoint position,CCPoint _anchorPoint,int edgeW);

/**
 * 获取当前设备已用内存(byte)
 * _type：0：byte 、 1：kb 、 2：M
 */
double getUsedMemory(int _type = 1);

/**
 * 获取当前设备未用内存(byte)
 * _type：0：byte 、 1：kb 、 2：M
 */
double getNotUsedMemory(int _type = 1);

CCPoint autoPosition(CCPoint p);
CCSize autoSize(CCSize s);
CCRect autoRect(CCRect r);

// 获得mac地址
const char* getMacAddress();
// 根据高清普清判断读取哪种资源
const char* getFileResource(const char* file);
// 读取android资源
const char* getAndroidFileResource(const char* file, const char* dir);
// 返回动画的函数
CCAnimate* getAnimate(const char* fileName, int startNumber, int endNumber, float duration = 0.1f);
// 执行粒子围绕一个rect旋转的动画
void runParticleAroundRect(CCRect rect, CCParticleSystemQuad* particle, float duration = 1.0f);
// 拆分字符串
CCArray* split(const char* str, const char* split);

// 将int转为k进制
const char* tran(int num,int k);
unsigned long ato16(const char* str);//字符串转16进制数

const char* GetStrFromVector(std::vector<char> * _vecbuf);

/**
 * 日期时间函数开始
 */
//返回从1970年1月1日0时0分0秒到此时的秒数
long getTimeIntervalSince1970(long secs);
//返回从xxxx年x月x日x时x分x秒字符串
const char* getCurTimeString();
const char* getCurTimeStringTo1970(long n);
//根据指定的时间秒数，返回时间字符串(00(00(00)
const char* getTimeString(int seconds);
//根据指定的时间秒数，返回时间结构
tTimeType getTime(int seconds);
//返回一个微秒级时间
long long getLongTimes();
//获取当前系统时区
int getTimeZone();
//返回一个毫秒级时间
long long get1970Milliscond();
//根据指定的时间秒数，返回加速需要钻石数
int getTimeDiamond(float seconds);
int getRandom(int min, int max);
/*
 * 字符串处理函数
 */
// 大写转小写函数
char* strToLower(char* str);
// 小写转大写函数
char* strToUpper(char* str);
/*
 * 块string替换函数
 * src:原始字符串
 * oldStr:需要替换的内容
 * newStr:替换之后的内容
 */
CCString* replaceString(const char* src, const char* oldStr, const char* newStr);
/*
 * 脏话过滤
 */
CCString* filterString(const char* str);


void SDLOG(const char* _data);

CCSprite* drawnum(char* showchar,char* numberfile);

void writeToFile(const char* pszFileName, unsigned char* pData, unsigned long len);


CCString* getLibrarypath(const char* addname);
//void LogSystiemTiem(char * tag = "" );
void LogSystiemTiem(char * tag  );

/*返回utf-8编码的字符的长度。*/
extern int getUtf8Length(const std::string & str);
#endif
