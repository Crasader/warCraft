//
//  SGSkillCardInfoLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#ifndef __GameSanGuo__SGSkillCardInfoLayer__
#define __GameSanGuo__SGSkillCardInfoLayer__

#include "SGBaseStuffLayer.h"
#include "SGBaseMilitaryCard.h"
#include "CCSpriterX.h"
class SGSkillCardInfoLayer : public SGBaseStuffLayer
{
public:
    static SGSkillCardInfoLayer *create(SGBaseMilitaryCard *card,int type,bool state);
    SGSkillCardInfoLayer();
    ~SGSkillCardInfoLayer();
private:
    void initView();
    void backHandler();
    void protectHandler();
    void disprotectHandler();
    void sellHandler();
    void getHandler();
    void disprotectListener(CCObject *obj);
    void propprotectListener(CCObject *obj);
    virtual void showBoxCall(CCObject *obj);
    void showLayerByTag(LayerTag layerTag);
    void playAct();
    void updatewater();
protected:

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
    
private:
    SGButton *sellBtn;
    int enterType;
    SGBaseMilitaryCard *_card;
    SGButton *protectBtn;
    float btnwidth;
    float btnwid;
    CCSprite *lock;
    bool issuccess;
    CCPoint pos;
    int time;
};



#endif /* defined(__GameSanGuo__SGSkillCardInfoLayer__) */
