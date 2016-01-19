//
//  SGStoreItem.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#ifndef __GameSanGuo__SGStoreItem__
#define __GameSanGuo__SGStoreItem__

#include "SNSTableViewCellItem.h"
#include "SNSTableView.h"
class SGStoreItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};



class SGStoreItem : public SNSTableViewCellItem
{
private:
//    CCArray *_arrary;
    CCLabelTTF *rmb;
    CCLabelAtlas *labelyb;
    CCSprite *font;
    CCLabelAtlas *labelsh;
    CCLabelTTF *wyh;
    CCSprite *font1;
    CCSprite *yb;
    CCSprite *jz ;
    CCLabelTTF *yukaT1;
    CCLabelTTF *yukaT2;
    CCLabelTTF *yukaD1;
    CCLabelTTF *yukaD2;
    CCLabelTTF *yukaGold;
    CCSprite *dik;
public:
    SGStoreItemDelegate *delegate;
    SNSTableView *tableview;
    int type;

    
public:
    SGStoreItem();
    ~SGStoreItem();
	virtual bool initWithSize(int num,CCArray *ary);
    void updatedata(int num ,CCArray *ary);
//	static SGStoreItem* create(int num);
    static SGStoreItem* create(int num,CCArray *ary,bool isshow = false,SGStoreItemDelegate*del=NULL,SNSTableView*table=NULL);
    void buttonClick(CCObject*obj);
};
#endif /* defined(__GameSanGuo__SGStoreItem__) */
