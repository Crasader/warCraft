//
//  SGObtainTiliCell.cpp
//  GameSanGuo
//
//  Created by zenghui on 14-6-3.
//
//

#include "SGObtainTiliCell.h"
#include "SGLayout.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGFriendMailContent.h"
#include "SGMailDataModel.h"
#include "ResourceManager.h"
#include "SGShowString.h"

SGObtainTiliCell::SGObtainTiliCell()
: itemSendLetter(NULL)
, m_giftPower(NULL)
{
  
}

SGObtainTiliCell::~SGObtainTiliCell()
{
 
}

SGObtainTiliCell* SGObtainTiliCell::create(SGGiftPower *giftPower,SGFriendBaseDelegate *friendDelegate)
{
    SGObtainTiliCell *cell = new SGObtainTiliCell();
    cell->m_giftPower = giftPower;
    cell->_friendDelegate = friendDelegate;

    if(cell && cell->viewDidLoad())
    {
        cell->constructView();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;
}

void SGObtainTiliCell::constructView()
{
    itemSendLetter = SGButton::create("tiliNormal.png","tiliGetting.png",this, menu_selector(SGObtainTiliCell::buttonClickObtainTili),ccp(0,-18),false,true);
    itemSendLetter->setPosition(ccp(200, 0));
    addBtn(itemSendLetter);
    //menu1->addChild(itemSendLetter);

    this->updateCellBySGamePlayer(m_giftPower);
}

void SGObtainTiliCell::upDate(SGGiftPower *giftPower)
{
    m_giftPower = giftPower;
    this->updateCellBySGamePlayer(giftPower);
}

void SGObtainTiliCell::updateCellBySGamePlayer(SGGiftPower *giftPower)
{
    m_giftPower = giftPower;
    CCString *fileName = CCString::createWithFormat("countryIcon_%d.png",m_giftPower->getImage());
    CCSprite *sprite = CCSprite::createWithSpriteFrameName(fileName->getCString());
    if(NULL == sprite)
    {
        fileName->setString("countryIcon_3.png");
    }
    
    itemHeadPic->setImage(fileName->getCString());
    itemHeadPic->setFontImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",m_giftPower->getImage())->getCString())->displayFrame());
    itemHeadPic->setFontImageScale(1.08);
    labelLevel->setString(CCString::createWithFormat(str_Format_ji,m_giftPower->getLevel())->getCString());
    
    long long giftTime = m_giftPower->getGiftTime();
    
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    giftTime =  now.tv_sec - giftTime/1000;
    
    if(giftTime == 0)
    {
        labelLoginTime->setInsideColor(ccGREEN);
        labelLoginTime->setString(str_FriendSendTiLi_str14);
    }
    else
    {
        labelLoginTime->setInsideColor(ccRED);
        if (giftTime>0 && giftTime < 3600) {
            labelLoginTime->setString(CCString::createWithFormat(str_FriendSendTiLi_str17,(int)(giftTime/600))->getCString());
        }
        else if (giftTime>= 3600 && giftTime < 60*60*24) {
            labelLoginTime->setString(CCString::createWithFormat(str_FriendSendTiLi_str18,(int)(giftTime/3600))->getCString());
        }
        else if (giftTime >= 60*60*24) {
            labelLoginTime->setString(CCString::createWithFormat(str_FriendSendTiLi_str19,(int)(giftTime/(60*60*24)))->getCString());
        }else{
            labelLoginTime->setString(CCString::createWithFormat(str_FriendSendTiLi_str20,(int)(giftTime/(60*60*24*30)))->getCString());
        }
    }
    labelName->setString(giftPower->getFriendName()->getCString());
    setHeadTouchEnable(false);
}

void SGObtainTiliCell::buttonClickObtainTili()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_friendDelegate)
    {
        _friendDelegate->giftCellCallBack(m_giftPower, 0);
    }

}


