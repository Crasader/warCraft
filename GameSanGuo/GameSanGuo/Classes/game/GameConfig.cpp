//
//  GameConfig.cpp
//  GameSanGuo
//
//  Created by wenlong w on 12-11-19.
//
//

#include "GameConfig.h"
#include "CCApplication.h"
//#import "PYUserSDKLib.h"
//#include <Foundation/Foundation.h>
//#include <sys/sysctl.h>
//#include "Reachability.h"
#include "SGMainManager.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#endif

#if (PLATFORM == ANDROID)
#define YES		true
#define NO		false
#include "AndroidSDKAdp.h"
#include <jni.h>
//#include "../../../../../../cocos2dx/platform/android/jni/JniHelper.h"
#include "platform/android/jni/JniHelper.h"
#endif
#include "GlobalConfig.h"



using namespace cocos2d;
using namespace std;

string GameConfig::m_sFilePath = string("");

bool GameConfig::isExist()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey("isExist");
}

float GameConfig::getWidthMov(float size_)
{
    return (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width/320.0f)*size_;
}
float GameConfig::getHeightMov(float size_)
{
    return (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width/320.0f)*size_;
}
CCSize GameConfig::getWinSize()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    return winSize;
}

CCSize GameConfig::getGridSize()
{
    CCSize gridSize = CCSizeZero;
    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
    switch (platform)
    {
        case kTargetIpad:
        {
            gridSize = CCSizeMake(76, 63);
        }
            break;
        case kTargetAndroid:
        case kTargetIphone:
        {
            gridSize = CCSizeMake(76, 63);
            if (screenheight == 1136) {
                gridSize = CCSizeMake(76, 70);
            }
        }
            break;
        default:
            break;
    }
    return gridSize;
}
float GameConfig::getRiverWide()
{
    float rw = 0.0;
    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
    switch (platform)
    {
			//        case kTargetIpad:
			//        {
			//            rw = 20;
			//        }
			//            break;
        case kTargetIpad:
        {
            rw = 30;
        }
            break;
        case kTargetAndroid:
        case kTargetIphone:
        {
            rw = 20;
            
            if (screenheight == 1136) {
                rw = 70;
            }
        }
            break;
        default:
            break;
    }
    return rw;
}
float GameConfig:: getFontSize(float size_)
{
    return (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / 320.0f) * size_;
}

WuPinIndex GameConfig::getWuPinIndex(int i, int j)
{
    
    WuPinIndex index;
    index.i = i;
    index.j = j;
    return index;
}

GameIndex GameConfig::getGameIndex(int i,int j)
{
	//    if (i < 0) {
	//        i = 0;
	//    }-
    
    GameIndex index;
    index.i = i;
    index.j = j;
    
    return index;
}
bool GameConfig::getIsIndexEquate(GameIndex index1,GameIndex index2)
{
    if (index1.i == index2.i && index1.j == index2.j)
    {
        return true;
    }
    return false;
}

//通过gameIndex 获取位置
CCPoint GameConfig::getGridPoint(int i, int j, bool ismy)
{
    
    CCPoint point = CCPointZero;
    int ismysele = ismy?-1:1;
    int y = 0;
    if (ismy)
    {
        point.x = ((screenwidth - (gridWidth * mapList)) * 0.5) + (gridWidth *(j+1)) - (gridWidth * 0.5);
        y = riverWide * .5;
    }
    else
    {
        j = abs(mapList -1 -j);
        point.x = ((screenwidth - (gridWidth * mapList)) * 0.5) + (gridWidth *(j+1)) - (gridWidth * 0.5);
        y = (riverWide * .5 - gridHeight);
    }
    point.y = (screenheight * 0.5) + (ismysele* (gridHeight * (i+1) + y));
    return point;
}
bool GameConfig::isipad()
{
    if (CCApplication::sharedApplication()->getTargetPlatform() == kTargetIpad)
    {
        return true;
    }
    return false;
}

std::string GameConfig::getUid()
{
#if (PLATFORM == IOS)
	return OcCaller::getInstance()->getUdid();
#else
	//android code
    return "";
#endif
}

int GameConfig::halfPrice(int number)
{
    int leftattack= round( number * 0.5 );
    return leftattack;
}

int GameConfig::attleAddAp(int ap, int roundv)
{
    int a = round(ap * 0.5);
    int b = a / roundv;
    return b;
}


int GameConfig::getAloneNum(int number)
{
    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
    switch (platform)
    {
        case kTargetIpad:
        {
            number *= 2;
        }
            break;
        case kTargetAndroid:
        case kTargetIphone:
        {
            
        }
            break;
        default:
            break;
    }
    
    return number;
}

CCPoint GameConfig::getObjPos(cocos2d::CCPoint pos)
{
    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
    switch (platform)
    {
        case kTargetIpad:
        {
            pos.x *= 2;
            pos.y *= 2;
        }
            break;
        case kTargetIphone:
        {
            
        }
            break;
        case kTargetAndroid:
        {
			
        }
            break;
            
        default:
            break;
    }
    
    return pos;
}

CCAnimate* GameConfig::getAnimate(const char* fileName, int startNumber, int endNumber, float duration)
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	// 构造每一个帧的实际图像数据
	CCAnimation *animation = CCAnimation::create();
	CCString *frameName = NULL;
	CCSpriteFrame* frame = NULL;
	for (int i = startNumber; i <= endNumber; i++) {
		frameName = CCString::createWithFormat(fileName, i);
		frame = cache->spriteFrameByName(frameName->getCString());
		if (frame) {
			animation->addSpriteFrame(frame);
		}
	}
	animation->setDelayPerUnit(duration);
	return CCAnimate::create(animation);
}


void GameConfig::prohibitLockScreen()
{
#if (PLATFORM == IOS)
	
	OcCaller::getInstance()->prohibitLockScreen();
#else
	//android code
#endif
	//    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
	//    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}

bool GameConfig::chackExistenceNetwork()
{
	//    return YES;
	bool isExistenceNetwork = YES;
	/*
	 Reachability * rea = [Reachability reachabilityWithHostName:@"www.baidu.com"];
	 */
    //switch ([rea currentReachabilityStatus]) {
	//检测网络状态
#if (PLATFORM == IOS)
	NetStatus st = OcCaller::getInstance()->checkNetWorkStatus();
#else
	//android code
	NetStatus st = ReachableWiFi;//临时添加,Android具体的还没有实现
#endif
	switch (st) {
        case NotReach:
        {
            isExistenceNetwork=NO;
        }
            break;
        case ReachableWWAN:
        case ReachableWiFi:
        {
            isExistenceNetwork=YES;
        }
            break;
    }
    return isExistenceNetwork;
}
const char* GameConfig::longRangePath(const char* name)
{
#if (PLATFORM == IOS)
	return OcCaller::getInstance()->longRangePath(name, m_sFilePath);
#else
	//android code
#endif
	/*
	 NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
	 NSString *documentsDirectory = [paths objectAtIndex:0];
	 
	 m_sFilePath = [documentsDirectory UTF8String];
	 m_sFilePath.append("/");
	 
	 m_sFilePath += name;
	 return m_sFilePath.c_str();
	 */
}

void GameConfig::severImage()
{
	std::string fullpath = CCFileUtils::sharedFileUtils()->getWritablePath() + "sanGuoBattlephoto.png";
#if (PLATFORM == IOS)
	OcCaller::getInstance()->saveScreenImage(fullpath);
#else
	//android code
#endif
	/*
	 NSString *fullPath = [NSString stringWithUTF8String:fullpath.c_str()];
	 UIImage *image = [UIImage imageWithContentsOfFile:fullPath];
	 UIImageWriteToSavedPhotosAlbum(image, NULL, NULL, NULL);
	 */
}

std::string GameConfig::getUdit()
{
#if (PLATFORM == IOS)
	return OcCaller::getInstance()->getUdid();
#else
	//android code
	
	//通过jni的java调用获取安卓下mac地址
	std::string strMac="";
	
	JniMethodInfo methodInfo; //用于获取函数体
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,AndroidSDKAdp::getInstance()->getPackageName("GameSanGuo").c_str(), "getMacID", "()Ljava/lang/String;");
	
	if (isHave)
	{
		jstring jstr;
		jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);
        CCLOG("==== getMacID found jstr[%p] ====", &jstr);
        strMac = JniHelper::jstring2string(jstr);
        CCLOG("==== getMacID mac address has gotten [%s] ====", strMac.c_str());
	}
	else
	{
		CCLOG("java层getMacID 函数不存在;");
	}
	return strMac;
	
#endif
	
}


ccColor3B GameConfig::colorWithHexString(const char *stringToConvert)
{
#if (PLATFORM == IOS)
	
	return OcCaller::getInstance()->colorWithHexString(stringToConvert);
#else
	//android code
#endif

}

double GameConfig::usedMemory()
{
    
#if (PLATFORM == IOS)
	
	OcCaller::getInstance()->usedMem();
#else
	//android code
	return 0.0f;
#endif

}

double GameConfig::availableMemory()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#if (PLATFORM == IOS)
	
	return OcCaller::getInstance()->getAvaliableMem();
#else
	//android code
#endif
#else
    return 0;
#endif
}

const char* GameConfig::bundleVersion()
{
    return CCString::createWithFormat("%s", GlobalConfig::gi()->getClientVersion().c_str())->getCString(); ;
    
    /*
#if (PLATFORM == IOS)
	return OcCaller::getInstance()->bundleVersion();
#else
	//android code
	
	return CCString::createWithFormat("%s", "3.0.0")->getCString();
#endif
     */
	/*
	 NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey];
	 return [version UTF8String];
	 */
}

/*
 bool GameConfig::memoryInfo(vm_statistics_data_t *vmStats) {
 mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
 kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)vmStats, &infoCount);
 
 return kernReturn == KERN_SUCCESS;
 }
 */
void GameConfig::logMemoryInfo() {
	
#if (PLATFORM == IOS)
	OcCaller::getInstance()->logMemInfo();
#else
	//android code
#endif
	/*
	 vm_statistics_data_t vmStats;
	 
	 if (memoryInfo(&vmStats)) {
	 NSLog(@"free: %u\nactive: %u\ninactive: %u\nwire: %u\nzero fill: %u\nreactivations: %u\npageins: %u\npageouts: %u\nfaults: %u\ncow_faults: %u\nlookups: %u\nhits: %u",
	 vmStats.free_count * vm_page_size,
	 vmStats.active_count * vm_page_size,
	 vmStats.inactive_count * vm_page_size,
	 vmStats.wire_count * vm_page_size,
	 vmStats.zero_fill_count * vm_page_size,
	 vmStats.reactivations * vm_page_size,
	 vmStats.pageins * vm_page_size,
	 vmStats.pageouts * vm_page_size,
	 vmStats.faults,
	 vmStats.cow_faults,
	 vmStats.lookups,
	 vmStats.hits
	 );
	 }
	 */
}


bool GameConfig::isContainsEmoji(const char *str) {
	
#if (PLATFORM == IOS)
	return OcCaller::getInstance()->isContainEmoji(str);
#else
	//android code
	return false;
#endif
	/*
	 __block BOOL isEomji = NO;
	 
	 NSString *string = [NSString stringWithUTF8String:str];
	 
	 [string enumerateSubstringsInRange:NSMakeRange(0, [string length]) options:NSStringEnumerationByComposedCharacterSequences usingBlock:
     
     ^(NSString *substring, NSRange substringRange, NSRange enclosingRange, BOOL *stop) {
	 
	 
	 const unichar hs = [substring characterAtIndex:0];
	 
	 // surrogate pair
	 
	 if (0xd800 <= hs && hs <= 0xdbff) {
	 
	 if (substring.length > 1) {
	 
	 const unichar ls = [substring characterAtIndex:1];
	 
	 const int uc = ((hs - 0xd800) * 0x400) + (ls - 0xdc00) + 0x10000;
	 
	 if (0x1d000 <= uc && uc <= 0x1f77f) {
	 
	 isEomji = YES;
	 
	 }
	 
	 }
	 
	 } else if (substring.length > 1) {
	 
	 const unichar ls = [substring characterAtIndex:1];
	 
	 if (ls == 0x20e3) {
	 
	 isEomji = YES;
	 
	 }
	 
	 
	 
	 } else {
	 
	 // non surrogate
	 
	 if (0x2100 <= hs && hs <= 0x27ff && hs != 0x263b) {
	 
	 isEomji = YES;
	 
	 } else if (0x2B05 <= hs && hs <= 0x2b07) {
	 
	 isEomji = YES;
	 
	 } else if (0x2934 <= hs && hs <= 0x2935) {
	 
	 isEomji = YES;
	 
	 } else if (0x3297 <= hs && hs <= 0x3299) {
	 
	 isEomji = YES;
	 
	 } else if (hs == 0xa9 || hs == 0xae || hs == 0x303d || hs == 0x3030 || hs == 0x2b55 || hs == 0x2b1c || hs == 0x2b1b || hs == 0x2b50|| hs == 0x231a ) {
	 
	 isEomji = YES;
	 
	 }
	 
	 }
	 
     }];
	 
	 
	 
	 return isEomji;
	 */
}
void GameConfig::deleteLog(){
#if (PLATFORM == IOS)
	
	OcCaller::getInstance()->delLog();
#else
	//android code
#endif
	/*
	 //创建文件管理器
	 NSFileManager * fileManager = [NSFileManager defaultManager];
	 NSString *documentsDirectory =[NSHomeDirectory()stringByAppendingPathComponent:@"Documents"];
	 NSString *fileName=@"ailog.txt";
	 NSString *finalPath = [documentsDirectory stringByAppendingPathComponent:fileName];
	 
	 //判断文件是否存在
	 if ([[NSFileManager defaultManager] fileExistsAtPath:finalPath]) {//如果文件存在则创建
	 //        [fileManager copyItemAtPath:copypath toPath:finalPath error:nil];
	 [fileManager createFileAtPath:finalPath contents:NULL attributes:NULL];
	 }
	 */
}
void GameConfig::writeLogToPlist(const char *log){
	
#if (PLATFORM == IOS)
	
	OcCaller::getInstance()->writeLogToXML(log);
#else
	//android code
#endif
	/*
	 //创建文件管理器
	 NSFileManager * fileManager = [NSFileManager defaultManager];
	 NSString *documentsDirectory =[NSHomeDirectory()stringByAppendingPathComponent:@"Documents"];
	 NSString *fileName=@"ailog.txt";
	 NSString *finalPath = [documentsDirectory stringByAppendingPathComponent:fileName];
	 
	 //判断文件是否存在
	 if (![[NSFileManager defaultManager] fileExistsAtPath:finalPath]) {//如果文件不存在则创建
	 [fileManager createFileAtPath:finalPath contents:NULL attributes:NULL];
	 }
	 
	 
	 
	 FILE *pf;//定义一个文件指针   大写通常定义的变量都是指针  封装好的_t才是整数类型
	 char buf[100];//定义一个缓冲区是100的变量
	 
	 pf = fopen([finalPath UTF8String],"a+");//以追加的方式打开文件
	 
	 if(pf == NULL)
	 {
	 perror("open");
	 exit(1);
	 }
	 
	 
	 strcpy(buf,"\n");//将word 拷贝到 buf中
	 fputs(log,pf);//将world  追加到  pf文件中.
	 fputs(buf, pf);
	 
	 fclose(pf);
	 
	 */
}

void GameConfig::showTip(const char *str)
{
    return;
#if (PLATFORM == IOS)
	
	OcCaller::getInstance()->showPopWindow(str);
#else
	//android code
#endif
	/*
	 CCNative::createAlert("加载失败", str, NULL);
	 CCNative::addAlertButton("~ ~");
	 CCNative::showAlert();
	 */
}
const char *GameConfig::getStringByTime(int leftTime)
{
    int time_hour = leftTime/3600;
    int time_mimute = leftTime%3600/60;
    int time_secend = leftTime%3600%60;
    
    char buf_hour[8];
    char buf_mimute[8];
    char buf_secend[8];
    
    if(time_hour < 10)
    {
        sprintf(buf_hour, "0%d",time_hour);
    }else{
        sprintf(buf_hour, "%d",time_hour);
    }
    
    if(time_mimute < 10)
    {
        sprintf(buf_mimute, "0%d",time_mimute);
    }else{
        sprintf(buf_mimute, "%d",time_mimute);
    }
    
    if(time_secend < 10)
    {
        sprintf(buf_secend, "0%d",time_secend);
    }else{
        sprintf(buf_secend, "%d",time_secend);
    }
    
    std::string timeStr;
//    if(time_hour != 0)
//    {
        timeStr.append(buf_hour);
        timeStr.append(":");
//    }
    
    timeStr.append(buf_mimute);
    timeStr.append(":");
    timeStr.append(buf_secend);
    
    return timeStr.c_str();
}

void GameConfig::gcForAndroid()
{
    ;
}
