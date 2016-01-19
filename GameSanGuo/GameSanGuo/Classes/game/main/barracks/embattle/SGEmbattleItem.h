//
//  SGEmbattleItem.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-21.
//
//

#ifndef __GameSanGuo__SGEmbattleItem__
#define __GameSanGuo__SGEmbattleItem__

#include "cocos2d.h"
#include "SGOfficerCard.h"
USING_NS_CC;

class MoveDelegate;

class SGEmbattleItem : public CCLayer
{
private:
    bool isRock;
    int timer;
    int emtype;
public:
    bool isSelect;
    SGOfficerCard *officerCard;
    int index;
    int _islord;
    CCSprite *face;
    CCPoint firstPoint;
    bool isCanMove;
    MoveDelegate *delegate;
private:
    void addTimer();
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
 
public:
    SGEmbattleItem();
    ~SGEmbattleItem();
    
    static SGEmbattleItem *create(MoveDelegate *dg,  SGOfficerCard *card, int emtype,int islord = false);
    static SGEmbattleItem *create(MoveDelegate *dg, int index_,int emtype);
    virtual bool init(MoveDelegate *dg, SGOfficerCard *card,int a);
    
    void startRock();
    void stopRock();
    bool testIsCanMove(CCPoint point);    
    
    float getWidth();
    float getHeight();
    void setIsLord(bool isLord);
    SGOfficerCard *getOfficerCard();
    CCSprite *namebg;
    CCLabelTTF *namelabel;
};

class MoveDelegate
{
public:
    virtual void setIsSelect(bool isSelect){};
    virtual void moveToPoint(CCPoint point,int index){};
    virtual void moveToLordItemPoint(CCPoint point,int index){};
    virtual bool trade(){};
    virtual void selectedIndex(SGEmbattleItem *item){};
};

#endif /* defined(__GameSanGuo__SGEmbattleItem__) */
