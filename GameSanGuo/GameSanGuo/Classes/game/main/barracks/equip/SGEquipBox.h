//
//  SGEquipBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-7.
//
//

#ifndef __GameSanGuo__SGEquipBox__
#define __GameSanGuo__SGEquipBox__

#include "SGBaseMilitaryCard.h"
#include "SGBaseTableBox.h"
class SGEquipBox : public SGBaseTableBox
{
private:
    void confirmHandler();  
public:
    SGEquipBox();
    ~SGEquipBox();
    static SGEquipBox *create(SGBoxDelegate *delegate);
    void initView();
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);

};
#pragma mark Sell_Item

class Equip_Item : public SNSTableViewCellItem
{
private:
    SGBaseMilitaryCard *_card;    
    CCLabelTTF *labelName;
    CCLabelTTF * labelLevel;


public:
    Equip_Item();
    ~Equip_Item();
    
public:
	void initView(int num);
	static Equip_Item* create(SGBaseMilitaryCard *card,int num);
    void updateOfficerCard(SGBaseMilitaryCard *card,int index);
};

#endif /* defined(__GameSanGuo__SGEquipBox__) */
