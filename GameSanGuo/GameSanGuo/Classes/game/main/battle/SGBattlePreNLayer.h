//
//  SGBattlePreNLayer.h
//  GameSanGuo
//
//  Created by lizi on 13-6-28.
//
//

#ifndef __GameSanGuo__SGBattlePreNLayer__
#define __GameSanGuo__SGBattlePreNLayer__

#include "SGBaseLayer.h"
#include "SGLoadData.h"
#include "PlatformAdp.h"

#if (PLATFORM == IOS)

class CCHLSprite : public CCSprite
{
public:
    CCHLSprite(void);
    ~CCHLSprite(void);
    
    void setHighLight(bool highLight);
    
    static CCHLSprite* create(const char* fileName);
    static CCSprite* graylightWithCCSprite(CCSprite *oldSprite, bool isLight);
public:
    virtual void draw(void);
};
#endif

class OfficerSprite : public CCSprite
{
public:
    OfficerSprite(void);
    ~OfficerSprite(void);
    
    static OfficerSprite* create(const char* name, int roleId, int roleType, bool isMoff);
    void initViewWithInfo(const char* name, int roleId, int level, bool isMoff);
    
    void setOpacity(GLubyte opacity);
};

class SGBattlePreNLayer : public SGBaseLayer
{
private:
    float IconWid;
    float IconHig;
    
    CCSprite *vsImg;
    
    CCSprite *headIconD;
    CCSprite *headIconU;
    
    CCLabelTTF *selfName;
    CCLabelTTF *armyName;
    CCSprite *selfCountry;
    CCSprite *armyCountry;
    CCSprite *selfCtyBg;
    CCSprite *armyCtyBg;
    
    CCArray *selfArray;
    CCArray *selfLArray;
    CCArray *armyArray;
    CCArray *armyLArray;
    CCArray *aniArray;
    CCArray *aniArrayN;
    
    CCArray *selfBArray;
    CCArray *armyBArray;
    CCArray *selfNumArray;
    CCArray *armyNumArray;
    
public:
    SGBattlePreNLayer(void);
    ~SGBattlePreNLayer(void);
    
    static SGBattlePreNLayer* create(SGLoadData *self, SGLoadData *army);
    void initView(SGLoadData *self, SGLoadData *army);
    void initArrays();
    void initData(int sType, int aType);
    
public:
    void initBgImgs();
    void showIcons();
    void showMainIcon();
    void showHighLight();
    void dismissIcons();
    void dismissIcons2(); //继续显示
    void dismissSelfIconBgs(CCNode *node);
    void dismissArmyIconBgs(CCNode *node);
    void removeLights(CCNode *node);
    void gotoFightLayer();
    
    void showNextNum(int type);
    
    void dodismiss1(); // 双方武将都大于5
    void dodismiss2(); // 双方武将都小于5
    void dodismiss3(); // 自己武将小于5，敌方武将大于5
    void dodismiss4(); // 敌方武将小于5，自己武将大于5
};

#endif /* defined(__GameSanGuo__SGBattlePreNLayer__) */
