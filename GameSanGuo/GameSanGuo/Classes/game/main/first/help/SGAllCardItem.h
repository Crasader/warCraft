//
//  SGAllCardItem.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-10.
//
//

#ifndef __GameSanGuo__SGAllCardItem__
#define __GameSanGuo__SGAllCardItem__

#include "SNSTableViewCellItem.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGBaseMilitaryCard.h"

class SGAllCardItem : public SNSTableViewCellItem
{
private:
    
    SGBaseMilitaryCard *_card;
    CCSprite *icon;
    CCSprite *kuang;
    //武將名字
    SGCCLabelTTF * m_name;
    CCScale9Sprite * m_namebg;
private:
    void initView(SGBaseMilitaryCard *card,int num);
public:
    SGAllCardItem();
    ~SGAllCardItem();
    static SGAllCardItem* create(int num);
    static SGAllCardItem *create(SGBaseMilitaryCard *card,int num);
    void updataCard(SGBaseMilitaryCard *card,int state);
};

#endif /* defined(__GameSanGuo__SGAllCardItem__) */
