//
//  SGAreanItemFightLog.cpp
//  GameSanGuo
//
//  Created by angel on 14-12-18.
//
//

#include "SGAreanItemFightLog.h"
#include "SGDrawCards.h"
#include "SGStringConfig.h"


SGAreanItemFightLog::SGAreanItemFightLog():m_pLabelDeltaRank(NULL),
m_pLabelForeRank(NULL),
m_pFightResult(NULL),
m_pRankState(NULL),
m_pButtonHeadIcon(NULL),
m_pLabelTime(NULL),
m_pItemFightLogData(NULL),
m_pLabelRank(NULL),
m_pLabelName(NULL),
m_pLabelLevel(NULL),
m_pLabelBackRank(NULL)
{
}
SGAreanItemFightLog::~SGAreanItemFightLog()
{
}
SGAreanItemFightLog *SGAreanItemFightLog::create(SGAreanFightLogData * itemFightLogData)
{
    SGAreanItemFightLog * itemFightlog = new SGAreanItemFightLog();
    if(itemFightlog && itemFightlog->init())
    {
        itemFightlog->setItemFightLogData(itemFightLogData);
        itemFightlog->initView();
        itemFightlog->autorelease();
        return itemFightlog;
        
    }
    
    CC_SAFE_DELETE(itemFightlog);
    return NULL;
}
void SGAreanItemFightLog::initView()
{
    CCSprite * itemBg = CCSprite::createWithSpriteFrameName("arean_log_item2.png");
    this->addChild(itemBg);
    
    m_pFightResult = CCSprite::createWithSpriteFrameName("fight_win.png");
    m_pFightResult->setPosition(ccp(250,0));
    this->addChild(m_pFightResult);
    
    m_pRankState = CCSprite::createWithSpriteFrameName("arean_up.png");
    m_pRankState->setPosition(ccp(180,0));
    this->addChild(m_pRankState);
    
    
    m_pButtonHeadIcon = SGDrawCards::createNormalButton(m_pItemFightLogData->getOfficeItemId(),BIT_OFFICER, sg_areanLayer, this,menu_selector(SGAreanItemFightLog::onClickButtonLook));
    m_pButtonHeadIcon->setScale(0.8);
    addBtn(m_pButtonHeadIcon);
    m_pButtonHeadIcon->setPosition(ccp(-220,15));
    this->addChild(m_pButtonHeadIcon);
    
    
    SGCCLabelTTF * level = SGCCLabelTTF::create(str_sgareanlayer_str20, FONT_PANGWA, 22);
    level->setPosition(ccp(-30,30));
    this->addChild(level);
    
    m_pLabelLevel = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pItemFightLogData->getLevel())->getCString(),FONT_PANGWA, 22);
    m_pLabelLevel->setPosition(ccp(20,30));
    this->addChild(m_pLabelLevel);
    
    m_pLabelName = SGCCLabelTTF::create(m_pItemFightLogData->getNickName()->getCString(), FONT_PANGWA, 22,ccGREEN);
    
    m_pLabelName->setPosition(ccp(-220,-40));
    this->addChild(m_pLabelName);
    
    
    CCSprite * arrows = CCSprite::createWithSpriteFrameName("arrows_right.png");
    this->addChild(arrows);
    
    m_pLabelForeRank = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pItemFightLogData->getForeRank())->getCString(),FONT_PANGWA,22);
    m_pLabelForeRank->setPosition(ccp(-50,0));
    this->addChild(m_pLabelForeRank);
    
    m_pLabelBackRank = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pItemFightLogData->getBackRank())->getCString(),FONT_PANGWA,22);
    m_pLabelBackRank->setPosition(ccp(50,0));
    this->addChild(m_pLabelBackRank);
    
    m_pLabelDeltaRank = SGCCLabelTTF::create("",FONT_PANGWA,22,ccYELLOW);
    m_pLabelDeltaRank->setPosition(m_pRankState->getPosition());
    this->addChild(m_pLabelDeltaRank);
    
    m_pLabelTime = SGCCLabelTTF::create(calcTime(m_pItemFightLogData->getTime())->getCString(), FONT_PANGWA, 22, ccYELLOW);
    m_pLabelTime->setPosition(ccp(0,-30));
    this->addChild(m_pLabelTime);
    
//    m_pLabelRank = SGCCLabelTTF::create(CCString::createWithFormat(str_sgareanlayer_str29,m_pItemFightLogData->getBackRank())->getCString(),FONT_PANGWA,22,ccYELLOW);
//    m_pLabelRank->setPosition(ccp(-220,-40));
//    this->addChild(m_pLabelRank);
    
    
    resetElemnet();
    
    
    
}
CCString * SGAreanItemFightLog::calcTime(int time)
{
    int day = time / 86400;
    if(day >= 1)
    {
        return  CCString::create(str_sgareanlayer_str21);
    }
    else
    {
        int hour = time / 3600;
        return  CCString::create(CCString::createWithFormat(str_sgareanlayer_str22,hour)->getCString());
    }
}
void SGAreanItemFightLog::refreshItem()
{
    resetElemnet();
    if (m_pButtonHeadIcon)
    {
        this->getAllBtn()->removeObject(m_pButtonHeadIcon);
        m_pButtonHeadIcon->removeFromParent();
    }
    
    m_pButtonHeadIcon = SGDrawCards::createNormalButton(m_pItemFightLogData->getOfficeItemId(),BIT_OFFICER, sg_areanLayer, this,menu_selector(SGAreanItemFightLog::onClickButtonLook));
    m_pButtonHeadIcon->setScale(0.8);
    addBtn(m_pButtonHeadIcon);
    m_pButtonHeadIcon->setPosition(ccp(-220,15));
    this->addChild(m_pButtonHeadIcon);
    
    m_pLabelBackRank->setString(CCString::createWithFormat("%d",m_pItemFightLogData->getBackRank())->getCString());
    m_pLabelForeRank->setString(CCString::createWithFormat("%d",m_pItemFightLogData->getForeRank())->getCString());
    m_pLabelLevel->setString(CCString::createWithFormat("%d",m_pItemFightLogData->getLevel())->getCString());
    //m_pLabelRank->setString(CCString::createWithFormat(str_sgareanlayer_str29,m_pItemFightLogData->getBackRank())->getCString());
    
    m_pLabelName->setString(m_pItemFightLogData->getNickName()->getCString());
    m_pLabelTime->setString(calcTime(m_pItemFightLogData->getTime())->getCString());
    
    
    
}
void SGAreanItemFightLog::resetElemnet()
{
    if(m_pItemFightLogData)
    {
        int foreRank = m_pItemFightLogData->getForeRank();
        int backRank = m_pItemFightLogData->getBackRank();
        int deltaRank = abs(foreRank - backRank);
        if(deltaRank == 0)
        {
            if(m_pItemFightLogData->getIsWin())
            {
                m_pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("equal_win.png")->displayFrame());
                m_pLabelDeltaRank->setString("");
            }
            else
            {
                m_pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("equal_fail.png")->displayFrame());
                m_pLabelDeltaRank->setString("");
            }
        }
        else
        {
            if(m_pItemFightLogData->getIsWin())
            {
                m_pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("arean_up.png")->displayFrame());
                m_pLabelDeltaRank->setString(CCString::createWithFormat("%d",deltaRank)->getCString());
            }
            else
            {
                m_pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("arean_down.png")->displayFrame());
                m_pLabelDeltaRank->setString(CCString::createWithFormat("%d",deltaRank)->getCString());
            }

        }
        
        if(m_pItemFightLogData->getIsWin() && m_pItemFightLogData->getIsAttack())
        {
            m_pFightResult->setDisplayFrame(CCSprite::createWithSpriteFrameName("fight_win.png")->displayFrame());
        }
        else if(m_pItemFightLogData->getIsWin() && !m_pItemFightLogData->getIsAttack())
        {
             m_pFightResult->setDisplayFrame(CCSprite::createWithSpriteFrameName("def_win.png")->displayFrame());
        }
        else if(!m_pItemFightLogData->getIsWin() && m_pItemFightLogData->getIsAttack())
        {
            m_pFightResult->setDisplayFrame(CCSprite::createWithSpriteFrameName("fight_fail.png")->displayFrame());
        }
        else if(!m_pItemFightLogData->getIsWin() && !m_pItemFightLogData->getIsAttack())
        {
            m_pFightResult->setDisplayFrame(CCSprite::createWithSpriteFrameName("def_fail.png")->displayFrame());
        }
        
    }
}

void SGAreanItemFightLog::onClickButtonLook()
{
}
void SGAreanItemFightLog::setItemFightLogData(SGAreanFightLogData * itemFightLogData)
{
    m_pItemFightLogData = itemFightLogData;
}
SGAreanFightLogData * SGAreanItemFightLog::getItemFightLogData()
{
    return m_pItemFightLogData;
}


////////////////////////////////
SGAreanFightLogData::SGAreanFightLogData():m_roleId(0),
m_nickName(NULL),
m_nationId(0),
m_officeItemId(0),
m_level(0),
m_foreRank(0),
m_backRank(0),
m_isWin(false),
m_isAttack(false),
m_time(0)
{
}
SGAreanFightLogData::~SGAreanFightLogData()
{
    if(m_nickName)
    {
        CC_SAFE_RELEASE(m_nickName);
    }
}
SGAreanFightLogData * SGAreanFightLogData::create(int roleId, CCString * nickName, int nationId,
                                                  int officeItemId, int level, int foreRank,
                                                  int backRank, bool isWin, bool isAttack, int time)
{
    SGAreanFightLogData * logData = new SGAreanFightLogData();
    if(logData)
    {
        logData->setRoleId(roleId);
        logData->setNickName(nickName);
        logData->setNationId(nationId);
        logData->setOfficeItemId(officeItemId);
        logData->setLevel(level);
        logData->setForeRank(foreRank);
        logData->setBackRank(backRank);
        logData->setIsWin(isWin);
        logData->setIsAttack(isAttack);
        logData->setTime(time);
        logData->autorelease();
        
        return logData;
        
        
    }
    CC_SAFE_DELETE(logData);
    return NULL;
}
