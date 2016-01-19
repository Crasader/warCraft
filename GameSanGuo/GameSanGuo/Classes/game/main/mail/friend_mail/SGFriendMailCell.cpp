//
//  SGFriendMailCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#include "SGFriendMailCell.h"
#include "SGLayout.h"
#include "SGButton.h"
#include "SGTools.h"
#include "SGSystemMailCell.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "MailModule.pb.h"
#include "SGFriendMailLayer.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGFriendMailCell::SGFriendMailCell() : buttonDelete(NULL) , _mailData(NULL)
{
    labelContent = NULL;
    labelNickName = NULL;
    labelSendTime = NULL;
    spriteHead = NULL;
    spriteUnread = NULL;
    _isDeleteModel = false;
    
}

SGFriendMailCell::~SGFriendMailCell()
{
}

SGFriendMailCell* SGFriendMailCell::create(bool isDeleteModel,SGMailDataModel *mailData)
{
    SGFriendMailCell *cell = new SGFriendMailCell();
    cell->_isDeleteModel = isDeleteModel;
    cell->_mailData = mailData;
    
    if(cell &&cell->init())
    {
        cell->viewDidLoad();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;
    
}

void SGFriendMailCell::viewDidLoad()
{
    
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_friendMail);

    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMail);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_friendMail, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_friendMail);
	ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_friendMail);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMail);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_friendMail);

	
    //边框
    CCSprite *spriteBorder = CCSprite::createWithSpriteFrameName("item_smallbg.png");
    spriteBorder->setScaleY(1.1);
    this->addChild(spriteBorder);
    bgbgbg = spriteBorder;
    spriteBorder->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(50, 0)));
    //好友 头像 昵称 发送时间 是否未读 邮件内容概略
    spriteHead = SGButton::create("countryIcon_1.png", "huangkuang.png", this, NULL,CCPointZero,false,true);
//    spriteHead = CCSprite::create("headIcon/head1001.png");
//    spriteHead->setAnchorPoint(ccp(0, 0.5));
    spriteHead->setPosition(ccpAdd(spriteBorder->getPosition(),ccp(-spriteBorder->getContentSize().width/2 - 50,2)));
    
    addChild(spriteHead);
    
//    //头像边框
//    CCSprite *spriteHeadBorder = CCSprite::create("public/huangkuang.png");
//    spriteHeadBorder->setPosition(ccpAdd(spriteHead->getPosition(), ccp(spriteHead->getContentSize().width/2, 0)));
//    addChild(spriteHeadBorder);
    
    //昵称
    labelNickName = SGCCLabelTTF::create(str_SearchFriend_str11, FONT_BOXINFO, 24);
    labelNickName->setAnchorPoint(ccp(0, 0.5));
    labelNickName->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(-spriteBorder->getContentSize().width*.45,20)));
    addChild(labelNickName);
    
    //发送时间
    labelSendTime = SGCCLabelTTF::create(str_SearchFriend_str12, FONT_BOXINFO, 24);
    labelSendTime->setAnchorPoint(ccp(1, 0.5));
//    labelSendTime->setPosition(ccp(SGLayout::getSkewing(80),20));
    labelSendTime->setPosition(ccpAdd(labelNickName->getPosition(), ccp(spriteHead->getContentSize().width*2.5, 0)));
    addChild(labelSendTime);
    
    //未读
    spriteUnread = CCSprite::createWithSpriteFrameName("mail_unread.png");
//    spriteUnread->setAnchorPoint(ccp(1, 0));
    spriteUnread->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(spriteBorder->getContentSize().width*.42f, 0)));
    addChild(spriteUnread);
    spriteUnread->setVisible(false);
    
    //内容
    labelContent = SGCCLabelTTF::create(str_SearchFriend_str13, FONT_BOXINFO, 24);
    labelContent->setAnchorPoint(ccp(0, 0.5));
    labelContent->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(-spriteBorder->getContentSize().width*.45, -20)));
    addChild(labelContent);
    
    //删除按钮
    CCMenu *menu = CCMenu::create();
    buttonDelete = SGButton::create("friend_redButton.png","friend_delete.png", this, menu_selector(SGFriendMailCell::buttonClickDeleteMail),CCPointZero,false,true);
    buttonDelete->setAnchorPoint(ccp(1,0.5));
//    buttonDelete->setPosition(ccp(SGLayout::getSkewing(130),0));
    buttonDelete->setPosition(ccpAdd(spriteHead->getPosition(), ccp(spriteHead->getContentSize().width*4.2, 0)));
    menu->setPosition(CCPointZero);
    menu->addChild(buttonDelete);
    addChild(menu);
    
    update(_isDeleteModel,_mailData);
}

void SGFriendMailCell::buttonClickDeleteMail()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCLOG("-- 点击按钮，删除邮件 --");
    main::DeleteMailRequest *de = new main::DeleteMailRequest();
    de->set_mailid(_mailData->getMailSsid());
    SGFriendMailLayer::_waitBeDeletedMailId = _mailData->getMailSsid();
    SGSocketClient::sharedSocketClient()->send(MSG_MAIL_DELETEMESSAGE, de);
    SGMainManager::shareMain()->getMainLayer()->setMailSelectId(_mailData->getMailSsid());
}

void SGFriendMailCell::update(bool isDeleteModel,SGMailDataModel *mailData)
{
    _isDeleteModel = isDeleteModel;
    buttonDelete->setVisible(_isDeleteModel);
    labelSendTime->setVisible(!_isDeleteModel);
    spriteUnread->setVisible(false);
    _mailData = mailData;
    if (mailData->getMailItemId()) {
        spriteHead->setImage(CCString::createWithFormat("countryIcon_%d.png",mailData->getMailItemId())->getCString());
        spriteHead->setFontImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",mailData->getMailItemId())->getCString())->displayFrame());
        spriteHead-> setFontImageScale(1.08);

    }
        if(_mailData->getMailIsRead())
    {
//        spriteUnread->setDisplayFrame(CCSprite::create("mail/mail_read.png")->displayFrame());
    }else{
//        spriteUnread->setDisplayFrame(CCSprite::create("mail/mail_unread.png")->displayFrame());
        spriteUnread->setVisible(true);

    }
    labelNickName->setString(_mailData->getMailOtherNickname()->getCString());
    SGMailDetailDataModel *detail = (SGMailDetailDataModel*)_mailData->getMailContent()->lastObject();
    if (detail != NULL) {
        labelSendTime->setString(SGTools::getTimeDistanceNow(detail->getMailDeSendDate()).c_str());
        std::string contentString;
        contentString.append(SGSystemMailCell::getFirstTenWords(detail->getMailDeContent()->getCString(), 10));
        labelContent->setString(contentString.c_str());
    }

}
