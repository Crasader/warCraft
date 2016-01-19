//
//  SGRewardItem.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-19.
//
//

#ifndef __GameSanGuo__SGRewardItem__
#define __GameSanGuo__SGRewardItem__
#include "SNSTableViewCellItem.h"
#include "SGRewardLayer.h"
#include "SGCCLabelTTF.h"



class SGRewardItem : public SNSTableViewCellItem
{
private:    
//    CCSprite *sprite;

    SGCCLabelTTF *label;
    CCSprite *spr_notice;
    SGRewardItemDelegate *delegate;
    SNSTableView *tableview;
public:
    void updateOfficerCard(itemObj *obj);
    SGRewardItem();
    ~SGRewardItem();
    
public:    
	void setView(itemObj *obj);
	static SGRewardItem* create(itemObj *obj,SGRewardItemDelegate*del,SNSTableView*table);
    void buttonClick(CCObject*obj);

};


#endif /* defined(__GameSanGuo__SGRewardItem__) */
