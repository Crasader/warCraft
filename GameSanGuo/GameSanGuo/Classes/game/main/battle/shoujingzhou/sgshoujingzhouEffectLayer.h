//
//  sgshoujingzhouEffectLayer.h
//  GameSanGuo
//
//  Created by kanata on 13-12-19.
//
//

#ifndef __GameSanGuo__SGshoujingzhouEffectLayer__
#define __GameSanGuo__SGshoujingzhouEffectLayer__

#include <iostream>
#include "SGshoujingzhouEffectLayerCell.h"
#include "SGBaseTableLayer.h"
#include "SGBaseTableBox.h"
#include "SGSoulExchangeLayer.h"





class SGshoujingzhouEffectLayer : public SGBaseTableBox
{
public:
    SGshoujingzhouEffectLayer();
    ~SGshoujingzhouEffectLayer();
    static SGshoujingzhouEffectLayer *create( std::vector<SeRewardTypeId*>vector);
    
private:
    void initView();
    void boxClose(CCObject*obj);
    void initDatas(std::vector<SeRewardTypeId*>vector);
private:
    std::vector<SeRewardTypeId*>vec;
    
    
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的列数
	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section);
    
private:
    
};

#endif /* defined(__GameSanGuo__sgshoujingzhouEffectLayer__) */
