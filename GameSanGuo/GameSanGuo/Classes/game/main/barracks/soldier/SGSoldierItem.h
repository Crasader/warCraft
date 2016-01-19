//
//  SGSoldierItem.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#ifndef __GameSanGuo__SGSoldierItem__
#define __GameSanGuo__SGSoldierItem__

#include "cocos2d.h"
#include "SGSoldierCard.h"
#include "SGButton.h"

#define  MENUTAG 101
USING_NS_CC;

class SGSoldierItem : public CCSprite
{
private:
    SGSoldierCard *card;
    int  _cardId;
    int _colorId;
    SGButton *soldieritem;
    CCSprite *m_noticeFlag;
    bool soldierCanStreng;
public:
    SGSoldierItem();
	~SGSoldierItem();
    static SGSoldierItem *create(int cardId,int colorId);
    virtual bool init(int cardId,int colorId);
    int getCardId();
    int getColorId();
    void setSoldierItem(CCSprite *temp);
    void setNoticeFlag(bool b);
    CCLayerGradient *getGeadientLayer(float ght);
    void strengSoldier(CCNode *node);
    void showSoldier(CCNode *node);

};
#endif /* defined(__GameSanGuo__SGSoldierItem__) */
