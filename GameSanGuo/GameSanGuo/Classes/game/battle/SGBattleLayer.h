//
//  SGBattleLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-20.
//
//

#ifndef __GameSanGuo__SGBattleLayer__
#define __GameSanGuo__SGBattleLayer__

#include <iostream>
#include "cocos2d.h"
#include "GameConfig.h"
#include "SGHeroLayer.h"
#include "SGBattleMap.h"
#include "SGBaseLayer.h"

enum LayerZ
{
    ksg_enemybattlemap = 17,
    ksg_mybattlemap,
   
    ksg_myHeroLayer ,
    ksg_enemyHeroLayer,

    ksg_tipLayer,

    
};
//cgp
//enum LayerZ
//{
//    ksg_myHeroLayer,
//    ksg_enemyHeroLayer,
//    ksg_tipLayer,
//    ksg_mybattlemap = 17,
//    ksg_enemybattlemap = 18,
//};


enum BattleLayerTag
{
    sg_myBattleMapTag,
    sg_enemyBattleMapTag,
    sg_myHeroLayerTag,
    sg_enemyHeroLayerTag,
};
class SGBattleLayer : public SGBaseLayer
{

public:
    static SGBattleLayer* create(const char* fileName, bool isCg = false);
    
    CC_SYNTHESIZE(CCLayer*, m_effectsLayer,EffectsLayer);
    std::string m_battleId;
    CC_SYNTHESIZE(int, m_battleCardNum, BattleCardNum);
    
public:
    SGBattleLayer();
    ~SGBattleLayer();
    GameConfig *con;
    
    virtual void initMsg();

    void initMap(HeroObj *heroObj, CCArray *sbs, bool ismy);
    void setSpeedUi();
    
    void setSpeedUiNew(bool isMe);
    
    void viewDidLoad();
    
    void addEffectLayer();
    void effectPin(CCPoint pin);
//    void attack();
//    void attackBack(SGAttackList * attackList);
    void changeRound();
    void changeRound_needVerify(int type);//需要验证的战斗切回合调用此方法 type 是让谁能动
    void showColorSp(bool isHide = false);
    void setIsRepairMap(bool repair);
    bool getIsRepairMap();
    //得到是否为自己的回合
    bool getIsMyRound();
    bool getIsCgPve();
    void setIsMyRound(bool ismyround_);
    //显示是否胜利
    void showIsWin(bool isWin = true);
    
    SGHeroLayer *getOpponent(bool isme);
    
    void showHeroAttribute(bool ismy);
    virtual void setIsCanTouch(bool isTouch);
    
    void showBattleId(std::string str);
    
    void giveupMessage();
    
    void fuckLayer(int act = 1, bool isY = false );
    void setOperationName( const char *name );
    void setSpeed(int my_speed,int enemy_speed);//设置我方和敌人的速度
    void showFirstActionLayer();
    void firstLoading(); //第一次加载动画
    void addGuide();
    void textNewEff();
    void setIsOver(bool over);
    bool getIsOver();
    void setStopAi(bool isS);
    bool getStopAi();
    void changeColorSp(bool needRound = true);
    void onClickSkipArenaFightButton();//跳过竞技场战斗
    void callSetSkipButton();
    
private:
    void showTurnNum(bool turn, bool isMessage);
    void turnShowOver(CCNode *node);
    void chuWuPin();
    void resuleShowOver(CCNode *node);
    void changeRoundResult();
    void machineAction();
    
    void closeAI();
private:
    SGHeroLayer* myHeroLayer;
    bool ismyRound;
    int mySpeed;//我方速度
    int enemySpeed;//地方速度
//    bool isRefreshMap;
//    
//    bool isBattleOver;
    bool isRepairMap;
    
    CCLabelTTF *operationName;
    CCMenuItemFont *item;
    std::string m_pBackgroundFileName;
    CCLayerColor *layerColor;
    bool isOver;
    bool isCgPve;
    bool isStopAi;
    bool m_ismy;
    SGButton * skipFightButton;
    
public:
//    CCArray *battleDropData;
    CCLayerGradient *layerGradient;
    CCLayerGradient *layerGradient1;
    
    CCLayer *battleSp;   //城墙血量 me
    CCLayer *battleSp1;  //城墙血量 him
    
};
#endif /* defined(__GameSanGuo__SGBattleLayer__) */
