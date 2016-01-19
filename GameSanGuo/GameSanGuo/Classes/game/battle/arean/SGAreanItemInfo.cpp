//
//  SGAreanItemInfo.cpp
//  GameSanGuo
//
//  Created by angel on 14-12-18.
//
//

#include "SGAreanItemInfo.h"
#include "SGDrawCards.h"
#include "FriendModule.pb.h"
#include "ArenaModule.pb.h"
#include "SGStringConfig.h"
#include "SGGuideManager.h"



SGAreanItemInfo::SGAreanItemInfo():m_pLabelLevel(NULL),
m_pLabelName(NULL),
m_pLabelRank(NULL),
m_pButtonHeadIcon(NULL),
m_pButtonFight(NULL),
m_pButtonLook(NULL),
m_pItemBg(NULL),
m_type_arean(0),
m_pItemInfoData(NULL),
m_pLabelCombatValue(NULL),
m_pLabelReward(NULL)
{
}
SGAreanItemInfo::~SGAreanItemInfo()
{
}
SGAreanItemInfo * SGAreanItemInfo::create(SGAreanItemInfoData * infodata, int type)
{
    SGAreanItemInfo * itemInfo = new SGAreanItemInfo();
    if(itemInfo && itemInfo->init())
    {
        itemInfo->setItemInfoData(infodata);
        itemInfo->setType(type);
        itemInfo->initView();
        itemInfo->autorelease();
        return itemInfo;
    }
    CC_SAFE_DELETE(itemInfo);
    return NULL;
}
void SGAreanItemInfo::initView()
{
    m_pItemBg = CCSprite::createWithSpriteFrameName("arean_info_item.png");
    this->addChild(m_pItemBg);
    
   
    
    
    CCSprite * rankbg = CCSprite::createWithSpriteFrameName("rank_label.png");
    rankbg->setPosition(ccp(-220,-40));
    rankbg->setScaleY(0.9);
    this->addChild(rankbg);
    
    m_pLabelRank = SGCCLabelTTF::create(CCString::createWithFormat(str_sgareanlayer_str29,m_pItemInfoData->getRank())->getCString(),FONT_PANGWA,22,ccYELLOW);
    m_pLabelRank->setPosition(ccpAdd(rankbg->getPosition(), ccp(0,0)));
    this->addChild(m_pLabelRank);
    
   
    
    if(m_type_arean ==0)
    {
        m_pButtonFight = SGButton::create("arean_fight.png", "arean_fight.png", this, menu_selector(SGAreanItemInfo::onClickButtonFight));
        addBtn(m_pButtonFight);
        m_pButtonFight->setPosition(ccp(220,0));
        this->addChild(m_pButtonFight);
        //引导用的tag
        m_pButtonFight->setTag(11);
    }
    else
    {
        
        m_pButtonLook = SGButton::createFromLocal("store_exchangebtnbg.png", str_sgareanlayer_str30, this, menu_selector(SGAreanItemInfo::onClickButtonLook), CCPointZero, FONT_PANGWA,ccWHITE,26,false,true);
        m_pButtonLook->setPosition(ccp(220,0));
        this->addChild(m_pButtonLook);
        this->addBtn(m_pButtonLook);
    }

    
    
    m_pButtonHeadIcon = SGDrawCards::createNormalButton(m_pItemInfoData->getOfficeItemId(),BIT_OFFICER, sg_areanLayer, this,menu_selector(SGAreanItemInfo::onClickButtonLook));
    m_pButtonHeadIcon->setScale(0.8);
    addBtn(m_pButtonHeadIcon);
    m_pButtonHeadIcon->setPosition(ccp(-220,15));
    this->addChild(m_pButtonHeadIcon);
    
   
    
    
    SGCCLabelTTF * level = SGCCLabelTTF::create(str_sgareanlayer_str20, FONT_PANGWA, 22);
    level->setPosition(ccp(50,30));
    this->addChild(level);
    
    m_pLabelLevel = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pItemInfoData->getLevel())->getCString(),FONT_PANGWA, 22);
    m_pLabelLevel->setPosition(ccp(100,30));
    this->addChild(m_pLabelLevel);
    
    m_pLabelName = SGCCLabelTTF::create(m_pItemInfoData->getNickName()->getCString(), FONT_PANGWA, 22,ccGREEN);
    m_pLabelName->ignoreAnchorPointForPosition(false);
    m_pLabelName->setAnchorPoint(ccp(0.5,0));
    m_pLabelName->setPosition(ccpAdd(ccp(m_pLabelName->getContentSize().width/2,30), ccp(-70,0)));
    this->addChild(m_pLabelName);
    
    
    
    SGCCLabelTTF * combatValue = SGCCLabelTTF::create(str_sgareanlayer_str23, FONT_PANGWA, 22);
    combatValue->setPosition(ccp(-100,0));
    this->addChild(combatValue);
    
    m_pLabelCombatValue =SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pItemInfoData->getCombatValue())->getCString(),FONT_PANGWA,22);
    m_pLabelCombatValue->ignoreAnchorPointForPosition(false);
    m_pLabelCombatValue->setAnchorPoint(ccp(0.5,0));
    m_pLabelCombatValue->setPosition(ccp(-100 + combatValue->getContentSize().width/2 + m_pLabelCombatValue->getContentSize().width,0));
    this->addChild(m_pLabelCombatValue);
    
    SGCCLabelTTF * reward = SGCCLabelTTF::create(str_sgareanlayer_str24, FONT_PANGWA, 22,ccYELLOW);
    reward->setPosition(ccp(-100,-28));
    this->addChild(reward);
    
    CCSprite * areanIcon = CCSprite::createWithSpriteFrameName("arean_coin_small.png");
    areanIcon->setPosition(ccp(-60,-30));
    this->addChild(areanIcon);
    
    m_pLabelReward = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pItemInfoData->getAreanCoin())->getCString(), FONT_PANGWA, 22,ccYELLOW);
    m_pLabelReward->ignoreAnchorPointForPosition(false);
    m_pLabelReward->setAnchorPoint(ccp(0.5,0));
    m_pLabelReward->setPosition(ccp(-60 + areanIcon->getContentSize().width/2 + m_pLabelReward->getContentSize().width,-30));
    this->addChild(m_pLabelReward);
    
    
    
    
}

void SGAreanItemInfo::updateItem()
{
    if(m_pButtonHeadIcon)
    {
        this->getAllBtn()->removeObject(m_pButtonHeadIcon);
        m_pButtonHeadIcon->removeFromParent();
    }
    m_pButtonHeadIcon = SGDrawCards::createNormalButton(m_pItemInfoData->getOfficeItemId(),BIT_OFFICER, sg_areanLayer, this,menu_selector(SGAreanItemInfo::onClickButtonLook));
    addBtn(m_pButtonHeadIcon);
    m_pButtonHeadIcon->setScale(0.8);
    m_pButtonHeadIcon->setPosition(ccp(-220,15));
    this->addChild(m_pButtonHeadIcon);
    
    
    m_pLabelRank->setString(CCString::createWithFormat(str_sgareanlayer_str29,m_pItemInfoData->getRank())->getCString());
    m_pLabelCombatValue->setString(CCString::createWithFormat("%d",m_pItemInfoData->getCombatValue())->getCString());
    m_pLabelLevel->setString(CCString::createWithFormat("%d",m_pItemInfoData->getLevel())->getCString());
    m_pLabelReward->setString(CCString::createWithFormat("%d",m_pItemInfoData->getAreanCoin())->getCString());
    
    m_pLabelName->setString(m_pItemInfoData->getNickName()->getCString());
    
    if(m_type_arean ==0)
    {
        if(!m_pButtonFight)
        {
            m_pButtonFight = SGButton::create("arean_fight.png", "arean_fight.png", this, menu_selector(SGAreanItemInfo::onClickButtonFight));
            addBtn(m_pButtonFight);
            m_pButtonFight->setPosition(ccp(220,0));
            this->addChild(m_pButtonFight);
        }
        if(m_pButtonLook)
        {
            this->getAllBtn()->removeObject(m_pButtonLook);
            m_pButtonLook->removeFromParent();
        }
    }
    else
    {
        if(!m_pButtonLook)
        {
            m_pButtonLook = SGButton::createFromLocal("store_exchangebtnbg.png", str_sgareanlayer_str30, this, menu_selector(SGAreanItemInfo::onClickButtonLook), CCPointZero, FONT_PANGWA,ccWHITE,26,false,true);
            m_pButtonLook->setPosition(ccp(220,0));
            this->addChild(m_pButtonLook);
            this->addBtn(m_pButtonLook);
        }
        if(m_pButtonFight)
        {
            this->getAllBtn()->removeObject(m_pButtonFight);
            m_pButtonFight->removeFromParent();
        }
    }

    
    
}
void SGAreanItemInfo::onClickButtonFight()
{
    //如果引导中而且不是高亮战字时，均不响应战斗
    if (SGGuideManager::shareManager()->isGuide && SGGuideManager::shareManager()->limitStep != 21)
    {
        return ;
    }
    
    CCLOG("进入战斗");
    main::ArenaStartRequest *req = new main::ArenaStartRequest();
    req->set_rank(m_pItemInfoData->getRank());
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_START, req);
}
void SGAreanItemInfo::onClickButtonLook()
{
    main::FriendInfoRequest *info = new main::FriendInfoRequest();
    info->set_friendid(m_pItemInfoData->getRoleId());
    info->set_type(0);
    info->set_origin(1);
    
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);
    CCLOG("查看阵容");
}


void SGAreanItemInfo::setItemInfoData(SGAreanItemInfoData * itemInfoData)
{
    m_pItemInfoData = itemInfoData;
    //m_pItemInfoData->retain();
}
SGAreanItemInfoData * SGAreanItemInfo::getItemInfoData()
{
    return m_pItemInfoData;
}


SGAreanItemInfoData::SGAreanItemInfoData():m_roleId(0),
m_nickName(NULL),
m_nationId(0),
m_officeItmeId(0),
m_level(0),
m_rank(0),
m_combatValue(0),
m_areanCoin(0)
{
}
SGAreanItemInfoData::~SGAreanItemInfoData()
{
    if(m_nickName)
    {
        CC_SAFE_RELEASE(m_nickName);
    }
}
SGAreanItemInfoData * SGAreanItemInfoData::create(int roleId, CCString * nickName, int nationId, int officeItemId,
                                                  int level, int rank, int combatValue, int areanCoin)
{
    SGAreanItemInfoData * itemData = new SGAreanItemInfoData();
    if(itemData)
    {
        itemData->setRoleId(roleId);
        itemData->setNickName(nickName);
        itemData->setNationId(nationId);
        itemData->setOfficeItemId(officeItemId);
        itemData->setLevel(level);
        itemData->setRank(rank);
        itemData->setCombatValue(combatValue);
        itemData->setAreanCoin(areanCoin);
        itemData->autorelease();
        
        return itemData;
    }
    CC_SAFE_DELETE(itemData);
    return NULL;

}
