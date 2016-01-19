//
//  SGCrditCardCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-26.
//
//

#ifndef __GameSanGuo__SGCrditCardCell__
#define __GameSanGuo__SGCrditCardCell__

#include "SGStoryBaseCell.h"
#include "SGCreditCardModel.h"


class SGCreditCardCell : public SNSTableViewCellItem
{
private:
    void viewDidLoad();
    
public:
    SGCreditCardCell();
    ~SGCreditCardCell();
    static SGCreditCardCell* create();
    virtual bool init();
    void setData(SGCreditCardModel* model);
    void resetBgImg(int index);
    
private:
    void showGeneralInfo();
    void drawStars(int count,int star = 0);
public:
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    CCSprite *spriteIcon;       // 卡牌头像
    CCSprite *spriteIconselect;     // 卡牌头选中
    SGButton *headIcon;         // 可点击头像 武将／装备／道具
    SGCCLabelTTF *labelOriCount;    // 原价
    SGCCLabelTTF *labelSLevel;    // 卡牌星级
    CCSprite *spriteStar;       // 主将星级
    SGCCLabelTTF *labelName;      // 主将星级
    SGCCLabelTTF *labelCount;     // 道具消耗类个数
    SGCCLabelTTF *labelCanNum;     // 可兑换次数
    SGCCLabelTTF *labelCLevel;     // 所需等级
    SGCCLabelTTF *labelCost;       // 兑换所需积分
    CCSprite *redline;
    CCSprite *spriteBackground; //bg
    
    CCSize bgSize;
    int itemType; 
    int itemId;   // ItemId, 通过Id显示Item信息
    int cardType; // 卡牌类型 // 0 officer 1 equip 2 soldier 3 prop
};

#endif /* defined(__GameSanGuo__SGCrditCardCell__) */
