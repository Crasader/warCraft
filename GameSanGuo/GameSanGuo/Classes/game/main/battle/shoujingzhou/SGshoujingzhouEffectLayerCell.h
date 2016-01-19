//
//  SGshoujingzhouEffectLayerCell.h
//  GameSanGuo
//
//  Created by kanata on 13-12-19.
//
//守荆州军魂特效弹出页面cell

#ifndef __GameSanGuo__SGshoujingzhouEffectLayerCell__
#define __GameSanGuo__SGshoujingzhouEffectLayerCell__

#include <iostream>
#include "SNSTableView.h"
#include "SGSoulExchangeLayer.h"
#include "SGStaticDataManager.h"
class SGshoujingzhouEffectLayerCell: public SNSTableViewCellItem
{
public:
    SGshoujingzhouEffectLayerCell();
    ~SGshoujingzhouEffectLayerCell();
    
    static SGshoujingzhouEffectLayerCell* create(std::vector<SeRewardTypeId*>vector,int number);
    static SGshoujingzhouEffectLayerCell* create(CCArray*array,int number);
    void  initView();
    void  setData(SeRewardTypeId*obj);
private:
    std::vector<SeRewardTypeId*>vec;
    int num;
    CCArray*datas;
    CCSprite*  item;
    CCSprite*  kuang;
    CCSprite*  country;
    SGCCLabelTTF*  label;

    
};

#endif /* defined(__GameSanGuo__SGshoujingzhouEffectLayerCell__) */
