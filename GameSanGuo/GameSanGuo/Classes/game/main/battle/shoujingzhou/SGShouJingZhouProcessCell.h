//
//  SGShouJingZhouProcessCell.h
//  GameSanGuo
//
//  Created by kanata on 14-2-14.
//
//

#ifndef __GameSanGuo__SGShouJingZhouProcessCell__
#define __GameSanGuo__SGShouJingZhouProcessCell__

#include <iostream>
#include "cocos-ext.h"
#include "SNSTableViewCellItem.h"
#include "SGShouJingZhouProcessData.h"


USING_NS_CC_EXT;
class SGJingZhouProcessDelegate;
class  SGShouJingZhouProcessCell:public SNSTableViewCellItem
{
    
public:
    SGShouJingZhouProcessCell();
    ~SGShouJingZhouProcessCell();
    
    static SGShouJingZhouProcessCell* create(SGJingZhouProcessDelegate*del,SGShouJingZhouProcessData*data,int index);
    
private:
    void initView(SGShouJingZhouProcessData*data,int index);
    void showSectionInfoHandler(CCNode*node);
    void entrySectionHandler(CCNode*node);

public:
    
    SGJingZhouProcessDelegate *delegate;
    CCSize bgSize;
    SGButton*btn;
    SGButton*zhan;
};




class SGJingZhouProcessDelegate
{
public:
    virtual void entrySectionHandler(SGShouJingZhouProcessCell *giftItem,int index){};
    virtual void showSectionInfoHandler(SGShouJingZhouProcessCell *giftItem,int index){};
};

#endif /* defined(__GameSanGuo__SGShouJingZhouProcessCell__) */
