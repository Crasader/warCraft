//
//  SGStoryRewardBox.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#ifndef __GameSanGuo__SGStoryRewardBox__
#define __GameSanGuo__SGStoryRewardBox__

#include "SGBaseTableBox.h"
#include "SGStoryReward.h"
#include "PvpFight.pb.h"
#include "SGBaseMilitaryCard.h"
#define MAXSTAR 6

class SGStoryRewardBox : public SGBaseTableBox
{
public:
    SGStoryRewardBox(void);
    ~SGStoryRewardBox(void);
    
    static SGStoryRewardBox* create(SGBoxDelegate *delegate, SGStoryReward *reward);
    //static void gobackToStoryLayer(); //回到原来的界面
    
private:
    void initView(SGStoryReward *reward);
    void confHandler();
    void addCardsToBag(); //添加卡片到背包中
    void getFriendOfficerListener(CCObject *sender);
    void delayShowPvpMain();
public:
//    bool isInitPve;
    int ldanCount;
    
public:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
	// 返回tableView的列数
	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section);
};


bool updateLevel(int exp);
CCSprite *spriteWithStars(int stars);

SGBaseMilitaryCard* createOfficerCardWithInfo(main::OfficerCardItemProto proto);
SGBaseMilitaryCard* createEquipCardWithInfo(main::EquipmentCardItemProto proto);
SGBaseMilitaryCard* createPropCardWithInfo(main::PropsCardItemProto proto);
SGBaseMilitaryCard* createPiecesCardWithInfo(main::OfficerFragmentCardItemProto proto);
SGBaseMilitaryCard* createEmbattleSoldierCardWithInfo(main::SoldierCardItemProto cardInfo);

SGBaseMilitaryCard *createConsumeCardWithInfo(main::ConsumableCardItemProto cardInfo);
SGBaseMilitaryCard *createMaterialCardWithInfo(main::MaterialItemProto cardInfo);
SGBaseMilitaryCard* getCardWithIdAndType(int itemId, int type);

#endif /* defined(__GameSanGuo__SGStoryRewardBox__) */
