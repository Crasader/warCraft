//
//  SGAdvancedEffectLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-23.
//
//

#ifndef __GameSanGuo__SGAdvancedEffectLayer__
#define __GameSanGuo__SGAdvancedEffectLayer__

#include <iostream>
#include "cocos2d.h"
#include "SGBaseLayer.h"

USING_NS_CC;


class SGAdvancedEffectLayer : public SGBaseLayer
{
public:
    SGAdvancedEffectLayer();
    ~SGAdvancedEffectLayer();
    
    void initView();
    //officerFrom:-1=装备转生，从装备详情界面过来；1=武将转生，2=武将召唤；3=武将碎裂；4=装备转生，从SGCommonInfoBox的转生按钮过来。
    static SGAdvancedEffectLayer *create(SGBaseItemType itemType, int beforeAdvanceItemId, int afterAdvanceItemId , int officerFrom = -1, int extArg = 0);
    void setPictureVisiable(CCObject *obj);
    void setSuccessTipsVisiable(CCObject *obj);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void setisPlayCompleted(CCObject *obj);
private:
    SGBaseItemType itemType;
    int beforeAdvanceItemId;
    int afterAdvanceItemId;
    CCSprite *beforeAdvancedItemPic;
    CCSprite *afterAdvancedItemPic;
    SGCCLabelTTF *itemName;
    SGCCLabelTTF *successTips;
    bool isPlayCompleted;
    SGEquipCard *afterAdvancedCard;
    SGCCLabelTTF *adNumLabel;
    
    //-1=装备转生，从装备详情界面过来；1=武将转生，2=武将召唤；3=武将碎裂；4=装备转生，从SGCommonInfoBox的转生按钮过来。
    int m_officerFrom;
    //当m_officerFrom为3时候，这个代表碎裂成多少碎片，或许用于提示。
    int _extArg;
};


#endif /* defined(__GameSanGuo__SGAdvancedEffectLayer__) */
