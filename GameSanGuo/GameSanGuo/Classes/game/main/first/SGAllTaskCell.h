//
//  SGAllTaskCell.h
//  GameSanGuo
//
//  Created by kanata on 14-3-27.
//
//

#ifndef __GameSanGuo__SGAllTaskCell__
#define __GameSanGuo__SGAllTaskCell__

#include <iostream>

#include "cocos-ext.h"
#include "SNSTableViewCellItem.h"



USING_NS_CC_EXT;
class SGTaskDelegate;

class  SGAllTaskCell:public SNSTableViewCellItem
{
    
public:
    SGAllTaskCell();
    ~SGAllTaskCell();
    
    static SGAllTaskCell* create(SGTaskDelegate*del,CCString*string,int type);
    void updateview(int state,int tagtag,int entertype);
     CCSize bgSize;
    
private:
    void initView();
    void showSectionInfoHandler(CCNode*node);
    void entrySectionHandler(CCNode*node);
    void lingling(CCNode*node);
public:
    
    SGTaskDelegate *delegate;
private:
    CCString*labelstring;
    SGButton*ling;
    int entertype;
    int statestate;

};


class SGTaskDelegate
{
public:
    int m_questId;
    virtual void entrySectionHandler(SGAllTaskCell *giftItem,int index){};
    virtual void showSectionInfoHandler(SGAllTaskCell *giftItem,int index){};
};




















#endif /* defined(__GameSanGuo__SGAllTaskCell__) */
