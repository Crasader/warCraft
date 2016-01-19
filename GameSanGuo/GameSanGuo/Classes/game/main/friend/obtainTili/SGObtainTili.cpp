//
//  SGObtainTili.cpp
//  GameSanGuo
//
//  Created by zenghui on 14-6-3.
//
//

#include "SGObtainTili.h"
#include "SGObtainTiliCell.h"
#include "SGMainManager.h"
#include "SGFriend.h"
#include "SGGiftPower.h"
#include "SGPlayerInfo.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGStaticDataManager.h"

SGObtainTili::SGObtainTili() : obtainCountMsg(NULL)
{
    buttonSend = NULL;
    _arrayFriend = NULL;

    _touch = true;
    m_obtainCount = 20;
    eatedGiftPower = NULL;
}

SGObtainTili::~SGObtainTili()
{
    _arrayFriend->release();
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_obtainTili);
    
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_EATPWOWERGIFT);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_EATALLPWOWERGIFT);
}

SGObtainTili* SGObtainTili::create(CCArray *arrayFriend)
{
    SGObtainTili *myFriend = new SGObtainTili();
    myFriend->_arrayFriend = arrayFriend;
    myFriend->_arrayFriend->retain();
    
    if(myFriend && myFriend->init(NULL, sg_obtainTili))
    {
        myFriend->constructView();
        myFriend->autorelease();
        return myFriend;
    }
    
    CC_SAFE_DELETE(myFriend);
    return NULL;
}

void SGObtainTili::constructView()
{
    this->datas = _arrayFriend;
    shownoFriendTip(str_FriendSendTiLi_str3);

    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_EATPWOWERGIFT, this, callfuncO_selector(SGObtainTili::eatGiftPowerListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_EATALLPWOWERGIFT, this, callfuncO_selector(SGObtainTili::eatAllGiftPowerListener));

	ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI, sg_obtainTili);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_obtainTili);
	ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_obtainTili);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_obtainTili);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_obtainTili);
    
    
    float h = SGMainManager::shareMain()->getTotleHdHgt();
	CCPoint upCenter = SGLayout::getPoint(kUpCenter);

    SGCCLabelTTF *sprite = SGCCLabelTTF::create(str_FriendSendTiLi_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//sprite->setColor(ccc3(0xff, 0x95, 0x0c));
    sprite->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(sprite);
    spriteTitle->setVisible(false);

    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(upCenter, ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    CCSize s=CCDirector::sharedDirector()->getWinSize();

    buttonSend = SGButton::createFromLocal("tiliGetAll.png", str_FriendGetTiLiAll_str1, this, menu_selector(SGObtainTili::quickEatTili),ccp(0,3),FONT_PANGWA,ccWHITE,20);
    
    buttonSend->setAnchorPoint(ccp(1, 0.5));
    buttonSend->setPosition(ccp(SGLayout::getPoint(KMiddleRight).x, buttonBack->getPosition().y));
   
    this->addBtn(buttonSend);

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();

    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(upCenter, ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(upCenter, ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(upCenter, ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
   
    
    m_obtainCount = SGPlayerInfo::sharePlayerInfo()->getPowerGiftTimes();
    

    obtainCountMsg = SGCCLabelTTF::create(CCString::createWithFormat("%s%d",str_FriendSendTiLi_str4,m_obtainCount)->getCString(), FONT_XINGKAI, 30 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
    //obtainCountMsg->setColor(ccc3(0xff, 0x95, 0x0c));
    obtainCountMsg->setPosition(ccp(upCenter.x, SGMainManager::shareMain()->getBottomHeight() + 20));
    this->addChild(obtainCountMsg,100);
    //tableView->m_status = ScrollViewStatusCanNotTouch;
    
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
//    float headH = SGMainManager::shareMain()->getTotleHdHgt();
//    float bottomH = SGMainManager::shareMain()->getBottomHeight();
//    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(12)-16));//设置故事模式
//    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH + 36)));
}

SNSTableViewCellItem* SGObtainTili::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGObtainTiliCell *cell = (SGObtainTiliCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if(NULL == cell)
    {
        cell = SGObtainTiliCell::create((SGGiftPower*)this->datas->objectAtIndex(indexPath->getRow()),this);
    }else{
        cell->upDate((SGGiftPower*)this->datas->objectAtIndex(indexPath->getRow()));
    }
    cell->setIsCanTouch(_touch);
    cell->setParentLayer(this);
    return cell;
}

void SGObtainTili::eatGiftPowerListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::GetPowerGiftResponse *crr = (main::GetPowerGiftResponse *)sr->m_msg;
        if (crr->state() == 1 || crr->state() == 2)
        {
            //成功
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            m_obtainCount = playerInfo->getPowerGiftTimes() - 1;
            playerInfo->setPowerGiftTimes(m_obtainCount);
            obtainCountMsg->setString(CCString::createWithFormat("%s%d",str_FriendSendTiLi_str4,m_obtainCount)->getCString());
            
            if (eatedGiftPower)
            {
                SGPlayerInfo::sharePlayerInfo()->getFriendPowerList()->removeObject(eatedGiftPower);
                eatedGiftPower = NULL;
            }
            updateData();
            CCString *msgFriend = CCString::createWithFormat((crr->state() == 1 ? str_FriendSendTiLi_str23 :str_FriendSendTiLi_str24),2);
            SGMainManager::shareMain()->showMessage(msgFriend->getCString());
            //SGMainManager::shareMain()->showMessage(crr->state() == 1 ? str_FriendSendTiLi_str23 :str_FriendSendTiLi_str24);
        }
        else if (crr->state() == 3)
        {
            //体力满
             SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str21);
        }
        else if (crr->state() == 4)
        {
            //每次数
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str22);
        }
        else
        {
            //其他
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str25);
        }
        
    }
}

void SGObtainTili::eatAllGiftPowerListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::GetAllPowerGiftResponse *crr = (main::GetAllPowerGiftResponse *)sr->m_msg;
        
        if (crr->state() == 1 || crr->state() == 2)
        {
            //成功
            SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
            SGMainScene *mainSence = SGMainManager::shareMain()->getMainScene();
            int friendTiliCOunt = _arrayFriend->count();
            
            CCArray * giftPowerList = playerInfo->getFriendPowerList();
            m_obtainCount = playerInfo->getPowerGiftTimes() + crr->powergift_size() - giftPowerList->count();
            playerInfo->setPowerGiftTimes(m_obtainCount);
            obtainCountMsg->setString(CCString::createWithFormat("%s%d",str_FriendSendTiLi_str4,m_obtainCount)->getCString());
            
            giftPowerList->removeAllObjects();
            
            int giftPowerCount = crr->powergift_size();
            for(int i=0;i<giftPowerCount;i++)
            {
                main::PowerGiftProto _giftPower = crr->powergift(i);
                SGGiftPower *giftPower = mainSence->getSGGiftPower(_giftPower);
                playerInfo->addFriendPower(giftPower);
            }
            updateData();
            
            
            CCString *msgFriend = CCString::createWithFormat((crr->state() == 1 ? str_FriendSendTiLi_str23 :str_FriendSendTiLi_str24),friendTiliCOunt*2);
            SGMainManager::shareMain()->showMessage(msgFriend->getCString());
        }
        else if (crr->state() == 3)
        {
            //体力满
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str21);
        }
        else if (crr->state() == 4)
        {
            //每次数
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str22);
        }
        else
        {
            //其他
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str25);
        }
    }
}

void SGObtainTili::quickEatTili()
{
   EFFECT_PLAY(MUSIC_BTN);
    
    if (_arrayFriend->count() > 0)
    {
        SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
        int tiliMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel())->getPlayerPower();
        
        if (playerInfo->getPlayerPower() < tiliMax)
        {
            main::GetAllPowerGiftRequest *eatAll = new main::GetAllPowerGiftRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_EATALLPWOWERGIFT, eatAll);
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str21);
        }
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_FriendGetTiLiAll_str2);
    }


}

void SGObtainTili::giftCellCallBack(SGGiftPower *giftPower , bool type)
{
    if (m_obtainCount > 0)
    {
        SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
        int tiliMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel())->getPlayerPower();
        
        if (playerInfo->getPlayerPower() < tiliMax)
        {
            main::GetPowerGiftRequest *eatPowerGiftRequest = new main::GetPowerGiftRequest();
            eatPowerGiftRequest->set_giftid(giftPower->getGiftId());
            eatPowerGiftRequest->set_gifttime(giftPower->getGiftTime());
            SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_EATPWOWERGIFT, eatPowerGiftRequest);
            eatedGiftPower = giftPower;
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str21);
        }
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str22);
    }
}

void SGObtainTili::updateData()
{
    this->datas = SGPlayerInfo::sharePlayerInfo()->getFriendPowerList();
    this->tableView->reloadData();
    _arrayFriend->release();
    _arrayFriend = SGPlayerInfo::sharePlayerInfo()->getFriendPowerList();
    _arrayFriend->retain();
    shownoFriendTip(str_FriendSendTiLi_str3);
}



