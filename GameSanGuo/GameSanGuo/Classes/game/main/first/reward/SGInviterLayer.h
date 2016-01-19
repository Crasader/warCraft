//
//  SGInviterLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#ifndef __GameSanGuo__SGInviterLayer__
#define __GameSanGuo__SGInviterLayer__

#include "SGBaseTableLayer.h"
class SGInvietDelegate;
class Invite_Item : public SNSTableViewCellItem
{
private:
    SGInvietDelegate *delegate;       
public:    
    int state; 
    int lvl;
    int gold;
    CCSprite *item;
    SGButton *btn;
    
    const char *iconName;//头像
	const char *borderName;//边框
	const char *countryName;//国家
	int advanceNum;//+1, +2, +3
    
	SGButton *icon;
	CCSprite *borderIcon;//边框
	CCSprite *country;//国家图标
	SGCCLabelTTF *advNum;//+1 +2 +3
    SGCCLabelTTF *nums;
    
    
    int itemType;
    int itemId;
    int itemNum;
private:
   
public:
    Invite_Item();
    ~Invite_Item();
public:
    void showCardDetail(CCObject *obj);
    void getSomeIconWithItemType(int itemType, int itemId);
    void getGoldHandler(CCNode *sender);
	void setView(int station);
	static Invite_Item* create(SGInvietDelegate *dlg,int num,int gold,int station, int itemId, int itemNum, int itemType);
    void updateitem(int station, int num, int itemId, int itemNum, int itemType);
};

class SGInvietDelegate
{
public:
    virtual void getGoldHandler(Invite_Item *LvlGold_Item){};
};

class SGInviterLayer : public SGBaseTableLayer ,public SGInvietDelegate
{
private:
    Invite_Item *lvlupItem;
    CCArray *_array;
    CCString* inviteCode;
    int inviteNum;
    int inviteMax;
    void initView();
    void backHandler();
    void showGetGold();
    void showGetEverydayGold();
    void showWine();
    void showHope();
    void showliandan();
    void showcaishen();
    void inviteGoldListener(cocos2d::CCObject *obj);
public:
    SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
public:
    virtual void getGoldHandler(Invite_Item *LvlGold_Item);
    SGInviterLayer();
    ~SGInviterLayer();
    static SGInviterLayer *create( CCString* inviteCode,int inviteNum,int inviteMax,CCArray *array);
};

#endif /* defined(__GameSanGuo__SGInviterLayer__) */
