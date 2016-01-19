//
//  GameConfig.h
//  GameSanGuo
//
//  Created by wenlong w on 12-11-19.
//
//

#ifndef __GameSanGuo__GameConfig__
#define __GameSanGuo__GameConfig__

#include <iostream>
#include "cocos2d.h"
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"

USING_NS_CC_EXT;

USING_NS_CC;


/*
 
 演示关结束(10,"演示关结束"),
 选择国家完成(20,"选择国家完成"),
 起名字完成(30,"起名字完成"),
 第一章第一关开始(40,"第一章第一关开始"),
 第一章第一关结束(50,"第一章第一关结束"),
 第二关开始(60,"第二关开始"),
 第二关结束(70,"第二关结束"),
 第三关开始(80,"第三关开始"),
 第三关结束(90,"第三关结束"),
 进入每日任务(100,"进入每日任务"),
 ;
 */


//新手引导对应每一步消息
typedef enum guide_step
{
	default_step = 0,			//默认预留
	register_success,			//注册成功
	show_fight_demo = 10,		//演示关结束
	select_country = 20,			//选择国家
	take_nickname = 30,			//起名字
	first_fight_start = 40,		//第一场战斗开始
	first_fight_end = 50,		//每一场战斗结束
	second_fight_start = 60,		//第二场战斗开始
	second_fight_end = 70,		//第二场战斗结束
	streng_start,				//强化开始
	streng_1,					//强化第一步
	streng_2,					//强化第二步
	streng_3,					//强化第三步
	streng_end,					//强化结束
	advance_start,				//转生开始
	advance_1,					//转生第一步
	advance_2,					//转生第二步
	advance_3,					//转生第三步
	advance_end = 80,			//转生结束
	third_fight_start = 90,		//第三场战斗开始
	third_fight_end = 100,		//第三场战斗结束
	visit_start,				//探访开始
	visit_1,						//探访第一步
	visit_end,					//探访结束
	set_formation_start,		//布阵开始
	set_formation_1,			//布阵第一步
	set_formation_2,			//布阵第二步
	set_formation_end,			//布阵结束
	set_lord_start,				//设置主将开始
	set_lord_end,				//设置主将结束
	current_day_task = 110		//当时任务
	
}GUIDE_STEP;




#define UDID GameConfig::getUdit()

#define         INTEGRATION_AP_ADDITION 2
#define         CHAIN_AP_ADDITION 0.2
#define         SHOW_SB_DEF 0
#define         SHOW_MEM_INFO 0//游戏中显示内存信息
#define         IS_IAP_STORE 1 //是否开启苹果商店购买功能






struct WuPinIndex {
    WuPinIndex() : i(0), j(0)
    {};
    int i;
    int j;
};

struct GameIndex
{
    GameIndex() : i(0), j(0)
    {};
    int i;
    int j;
};

class GameSbIndex  :public cocos2d::CCObject{
public:
    GameSbIndex() : i(0), j(0), _time(0), is_sanbing(false)
    {};
    int i;
    int j;
    float _time;
    bool is_sanbing;   //是否是散兵, added by cgp
};

class GameBattleArray :public cocos2d::CCObject {
    
public:
    GameBattleArray() : gameFY(NULL), gameGJ(NULL), gameSPGJ(NULL), refreshGameGJ(NULL), refreshGameSPGJ(NULL)
    {};
    
    cocos2d:: CCArray *gameFY;
    cocos2d:: CCArray *gameGJ;
    cocos2d:: CCArray *gameSPGJ;
    
    cocos2d:: CCArray *refreshGameGJ;
    cocos2d:: CCArray *refreshGameSPGJ;
    
};

class removeOperate {

public:
    removeOperate() : _time(0)
    {};
    
    GameIndex index;
    float _time;
    
};


class GameConfig
{
public:
    static cocos2d::CCSize getWinSize();
    static float getWidthMov(float size_);
    static float getHeightMov(float size_);
    static float getFontSize(float size_);
    static WuPinIndex getWuPinIndex(int i,int j);
    static GameIndex getGameIndex(int i, int j);
    static bool getIsIndexEquate(GameIndex index1, GameIndex index2);
    static cocos2d::CCSize getGridSize();
    static bool isipad();
    static CCPoint getGridPoint(int i, int j, bool ismy = true);
    static std::string getUid();
    static int getAloneNum(int number);
    static cocos2d::CCPoint getObjPos(cocos2d::CCPoint pos);
    static int halfPrice(int number);
    static int attleAddAp(int ap, int roundv);
    static float getRiverWide();
    static bool isExist();
    
    static CCAnimate* getAnimate(const char* fileName, int startNumber, int endNumber, float duration);
    static void prohibitLockScreen();
    
    static void severImage();

    static std::string getUdit();
    static ccColor3B colorWithHexString(const char *stringToConvert);
    static const char*longRangePath(const char* name);
    static double usedMemory();
    static double availableMemory();
    static const char* bundleVersion();
    //static bool memoryInfo(vm_statistics_data_t *vmStats);
    static void logMemoryInfo();
    static bool chackExistenceNetwork();
    static bool isContainsEmoji(const char *str);
    static void showTip(const char* str);
        
private:
    static std::string m_sFilePath;
    
public:
    static void writeLogToPlist(const char *log);
    static void deleteLog();
public:
    static const char *getStringByTime(int leftTime);
    static void gcForAndroid();
};


#define GET_DEVICE_UUID GameConfig::getUid()
#define ISIPAD GameConfig::isipad()
#define MOV(size) (GameConfig::getWidthMov(size))
#define FONTSIZE(size) (GameConfig::getFontSize(size))
#define screenwidth ((GameConfig::getWinSize()).width)
#define screenheight ((GameConfig::getWinSize()).height)
#define wuPin_index(i, j) (GameConfig::getWuPinIndex(i,j))
#define gameIndex(i, j) (GameConfig::getGameIndex(i,j))
#define isIndexEquate(index1,index2) (GameConfig::getIsIndexEquate(index1, index2))
#define GET_ALONE_NUM(number) (GameConfig::getAloneNum(number))
#define GET_OBJ_POS(pos)  GameConfig::getObjPos(pos)
#define HALF_FRICE(number) GameConfig::halfPrice(number)
#define ATTLE_ADD_AP(ap, roundv) GameConfig::attleAddAp(ap, roundv) 
#define BUNDLE_VERSION GameConfig::bundleVersion()
#define riverWide (GameConfig::getRiverWide())//河的宽度 
#define peopleNumberMax 32
#define mapRow 6
#define mapList 8
#define gridWidth ((GameConfig::getGridSize()).width)
#define gridHeight ((GameConfig::getGridSize()).height)
#define nomalAttackLength 3
#define spAttackLength 2
#define chubing_moveSpeed 600 //before is 500//出兵士兵移动数度
#define moveSpeed 3000 //before is 2500//士兵移动数度
//#define attackSpeed 1.0//近程攻击速度
#define remote_attackSpeed 1//远程攻击速度  before is 0.65
//cgpSpine
//#define attackSpeed 1.5//近程攻击速度 时间
#define attackSpeed 1.5
//#define attackSpeed 3
//#define remote_attackSpeed 0.9//远程攻击速度


//#define shiftOutSpeed 50
#define sbMoveSpeed 0
//control

#endif /* defined(__GameSanGuo__GameConfig__) */
