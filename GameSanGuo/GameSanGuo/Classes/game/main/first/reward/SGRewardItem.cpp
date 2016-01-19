//
//  SGRewardItem.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-19.
//
//

#include "SGRewardItem.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGRewardItem::SGRewardItem()
:label(NULL),
spr_notice(NULL),
delegate(NULL),
tableview(NULL)
{
    
}

SGRewardItem::~SGRewardItem()
{
}

void SGRewardItem::setView(itemObj *obj)
{
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_rewardsLayer, LIM_PNG_AS_PNG);
 
    SGButton*item=SGButton::create("item_bigbg.png", NULL, this, menu_selector(SGRewardItem::buttonClick),CCPointZero,false,true);
    this->addChild(item);
    addBtn(item);
    ////////
    spr_notice = CCSprite::createWithSpriteFrameName("publc_notice.png");//!号
    this->addChild(spr_notice);
    spr_notice->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.32, 0)));
    label = SGCCLabelTTF::create("", FONT_PANGWA, FONTSIZE(18));
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.3, 0)));  
    this->addChild(label);
    updateOfficerCard(obj);
}
SGRewardItem* SGRewardItem::create(itemObj *obj,SGRewardItemDelegate*del,SNSTableView*table)
{
	SGRewardItem *instance = new SGRewardItem();
	if (instance &&instance->init()) {
        instance->delegate=del;
        instance->tableview=table;
        instance->setView(obj);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

//刷新
void SGRewardItem::updateOfficerCard(itemObj *obj)
{
    spr_notice->setVisible(false);
	//原来是6和5直接跳过,后来最前面加了一个月卡,猜测应该向后推移一个,by liuxiaofei,大坑,原来没注释.不知道为什么是6和5.
    if (obj->getstate() == 1 ) {
        spr_notice->setVisible(true);
    }
    label->setString("");
    if (obj->getitem() == Reward_YueKa)
    {
        label->setString(str_RewardBindLayer_str6);
    }
    else if (obj->getitem() == Reward_LvlUpGold)
    {
        label->setString(str_RewardBindLayer_str7);
    }
    else if (obj->getitem() == Reward_Caishen)
    {
        label->setString(str_RewardBindLayer_str8);
    }
    else if (obj->getitem() == Reward_Salary)
    {
        label->setString(str_RewardBindLayer_str9);
    }
    else if (obj->getitem() == Reward_Wine)
    {
        label->setString(str_RewardBindLayer_str10);
    }
    else if (obj->getitem() == Reward_Hope)
    {
        label->setString(str_RewardBindLayer_str11);
    }
    else if (obj->getitem() == Reward_Inviter)
    {
        label->setString(str_InviterLayer_str1);
    }
    else if (obj->getitem() == Reward_Invitee)
    {
        label->setString(str_InviteeLayer_str1);
    }
    else if(obj->getitem() == Reward_OpenServer)
    {
         label->setString(str_RewardBindLayer_str17);
    }
    else if (obj->getitem()== Reward_Bind)
    {
        label->setString(str_RewardBindLayer_str12);
    }
    else if(obj->getitem()== Reward_ExchangeCode)
    {
        label->setString(str_ExchangeCodeLayer_str1);
    }
    else if (obj->getitem() == Reward_MonthlySign)
    {
        label->setString(str_monthly_sign_str1);
    }
    else
    {
        if (obj->getishave())
            label->setString(CCString::createWithFormat(str_RewardBindLayer_str13,obj->getcount()+1)->getCString());
        else
            label->setString(str_RewardBindLayer_str14);
    }
}

//////////
void SGRewardItem::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);
}


