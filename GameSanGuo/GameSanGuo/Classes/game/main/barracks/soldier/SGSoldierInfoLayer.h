//
//  SGSoldierInfoLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-9.
//
//

#ifndef __GameSanGuo__SGSoldierInfoLayer__
#define __GameSanGuo__SGSoldierInfoLayer__

#include "SGBaseStuffLayer.h"
#include "SGSoldierCard.h"
#include "CCSpriterX.h"
#include "SGTouchLayer.h"
class SGSoldierInfoLayer : public SGBaseStuffLayer
{
private:
    bool isfromfirst;
    SGSoldierCard *_before;
    SGSoldierCard *_card;
    int enterType;
    CCPoint pos;
    CCProgressTimer *expprog;
    int leftTimes;
    bool isback;
    bool isbreak;
    CCSprite *admsBkg;
    SGTouchLayer *skillLayer;
    SGButton *strengBtn;
    SGButton *advanceBtn;
    SGButton *sellBtn;
    int soilderLimitLevel;
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
private:
    void initView();
    void backHandler();
    void showinfo();
    //强化
    void strengHandler();
    void addLabel(stuffType type, cocos2d::CCPoint point);
    void aaaa();
    void setcanttouch();
    void setcantouch();
    void removeSpX();
    void showSkillInfo();
    
    CCDictionary *Direc;
    CCSpriterX *lvlupAct;
    void playAction();
    void sellHandler();
public:
    static SGSoldierInfoLayer *create(SGSoldierCard *card_,int type,SGSoldierCard *before = NULL,bool isfirst = false,CCDictionary *dic = NULL);
    SGSoldierInfoLayer();
    ~SGSoldierInfoLayer();
    void setIsCanTouch(bool iscan);
    
    ////////////经验条函数三巨头
    void progressCallback(CCNode*node,SGBaseMilitaryCard *card);
    void progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call);
    void changeexplabel();
};
#endif /* defined(__GameSanGuo__SGSoldierInfoLayer__) */
