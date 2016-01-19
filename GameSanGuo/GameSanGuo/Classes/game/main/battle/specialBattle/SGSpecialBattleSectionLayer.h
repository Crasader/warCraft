//
//  SGSpecialBattleSectionLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-12-12.
//
//

#ifndef __GameSanGuo__SGSpecialBattleSectionLayer__
#define __GameSanGuo__SGSpecialBattleSectionLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGSpecialBattleLayer.h"


class SGSpecialBattleSectionLayer : public SGBaseTableLayer
{
public:
    SGSpecialBattleSectionLayer();
    ~SGSpecialBattleSectionLayer();
    
    static SGSpecialBattleSectionLayer *create(CCArray *sArray, SpecialBattleData *chapInfo);
    void initView();
    
    
    SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    void buyPlayTimes(CCObject *obj);
    void backHandler(CCObject *obj);
    
    void specialBattlePlotCheckValid(CCObject *obj);
    
    //购买次数确定消息
    void buyCountEntryListener(CCObject *obj);
    //购买消息
    void buyCountListener(cocos2d::CCObject *obj);
    void freshBuyCount(SpecialBattleData *info);

private:
    //选择是哪个节
    int selectIndex;
    //这一章的信息
    SpecialBattleData *chapInfo;
    //积分显示
    SGCCLabelTTF *points;
    //购买次数
    SGCCLabelTTF *residueTimesTTF;
    
};



class SGBuySpBattleTimes : public SGBaseBox
{
private:
	
public:
	SGBuySpBattleTimes();
	~SGBuySpBattleTimes();
	
	static SGBuySpBattleTimes *create(SGBoxDelegate *del = NULL, float price = 0.0f, int canBuyTimes = 0, int maxBuyTimes = 0);
	void initView();
	
	void pressOKButton();
	void pressCancelButton();
	virtual void boxClose();
    void closebox();
private:
    float price;
    int canBuyTimes;
    int maxBuyTimes;
};





#endif /* defined(__GameSanGuo__SGSpecialBattleSectionLayer__) */
