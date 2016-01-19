//
//  SGSetItem.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-11.
//
//

#ifndef __GameSanGuo__SGSetItem__
#define __GameSanGuo__SGSetItem__

#include "SNSTableViewCellItem.h"
#include "SGButton.h"
class SGSetItem : public SNSTableViewCellItem
{
private:
    SGButton *on;
    SGButton *off;
public:
    void turnOnHandler(CCNode *sender);
    void turnOffHandler(CCNode *sender);
    CCMenu *menu1;
public:    
     SGSetItem();
    ~SGSetItem();
	virtual bool initWithSize(int num);
	static SGSetItem* create(int num);
};

#endif /* defined(__GameSanGuo__SGSetItem__) */
