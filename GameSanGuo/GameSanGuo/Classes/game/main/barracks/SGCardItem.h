//
//  SGCardItem.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGCardItem__
#define __GameSanGuo__SGCardItem__

#include "SNSTableViewCellItem.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGCCLabelTTF.h"
#include "SGTeamgroup.h"
#include "SGBaseLayer.h"


USING_NS_CC;
enum CardType {
    cardType_officer,
    cardType_soldier,
    cardType_equip,
    cardType_prop,
    cardType_skill,
    cardType_streng
    };
//////////////
class SGCardItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
///////////////
class SGCardItem : public SNSTableViewCellItem
{
private:
    //CCSprite *itembg;
    SGButton*btn;
    SGCCLabelTTF *notice;
    SGCCLabelTTF *notice_bak;
    int timer;
    int emtype;
    CCSprite *spriteItem;
    SGBaseMilitaryCard *_card;
    SGCCLabelTTF *labelIsUse;
    CCSprite *spriteIsUseBG;
    CCSprite *lock;
    CCSprite *kuang;
    SGCCLabelTTF *name;
    //CCSprite *huise;
    CCArray *arrayStar;
    CCArray *bluestar;
    SGCCLabelTTF *label_lvl;
    SGCCLabelTTF *advance;
    CCSprite *countryName;
    SGCCLabelTTF *advancenumber;
    //////
    SGCardItemDelegate *delegate;
    SNSTableView *tableview;
    CCMenu* menu;
    CC_SYNTHESIZE(int, m_itemid, cardItemId);
    int _entertype;
    CCSprite *jnbg;

	CCSprite *coins;// 铜钱图标
	SGCCLabelTTF *cost;// 卡所值的钱数
	
	//在选择上阵武将中的选择武将复选框
	CCSprite *isSelectSpr;
	bool isSel;// 是否被选中
    CCSprite *namebg;
    
    CCSprite *lock_sprite;
    SGCCLabelTTF *lessGov;
    SGCCLabelTTF *LabelCurrEmbattle;//当前阵容标签
    SGCCLabelTTF *LabelAlternateEmbattle;//替补标签
    
private:

    void initView(bool issoldier);
    void showPlayerStar();
    void setCard(SGBaseMilitaryCard *newCard);
    ///////////////
    void buttonClick(CCObject*obj);
	
public:
    SGCardItem();
    ~SGCardItem();
	void setItemColor(ccColor3B color);//设置item的背景颜色
	void setIsSelected(bool isSelect);
	void setCardCost(int cos); //设置每个卡牌对应的钱数
	bool getIsSelected();//是否被选中
	
	SGBaseMilitaryCard *getItemCard();
    static SGCardItem*  create(bool issoldier= false,int emtype=0,SGCardItemDelegate*del=NULL,SNSTableView*table=NULL,int entertype = 0);
    //是否交替显示统御力、战斗力 需要重载create
    static SGCardItem*  create(bool isLabelWithAction , bool issoldier= false,int emtype=0 , SGCardItemDelegate*del=NULL,SNSTableView*table=NULL,int entertype = 0);
    //<---
    static void addCardItemRes(LayerTag cardInLayer);
    //---->
    void updataCard(SGBaseMilitaryCard *card, CardType type,bool ishui = false,bool isAdvance = true,bool enough = true, int isFromNormal = -1);
    bool checkCanUpdate(SGBaseMilitaryCard *card,int type);
    bool checkCanStrength(SGBaseMilitaryCard *checkCard,CardType cardType);
public:
    void setIsCanSelect(bool isCanSelecdt,bool isupBattel=false,bool isEnoughPostion=true, bool haveSameName = false); //选中,上阵,位置不足
    
public:
    CC_SYNTHESIZE(bool, _islocked, isLocked);
    
    //是否交替显示统御力、战斗力
    bool m_isLabelWithAction;
    
    void changeSelected(bool isSelected);
    
};
#endif /* defined(__GameSanGuo__SGCardItem__) */
