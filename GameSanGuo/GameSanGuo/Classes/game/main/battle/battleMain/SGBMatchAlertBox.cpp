//
//  SGBMacthAlertBox.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#include "SGBMatchAlertBox.h"

#include "SGBMatchStartBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGCantAdvanceBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "PvpFight.pb.h"
#include "SGBMatchEndBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGBMatchAlertBox::SGBMatchAlertBox(void)
:t_delegate(NULL),
label(NULL), //10秒倒计
counter(NULL),
isLater(NULL)
{

}
SGBMatchAlertBox::~SGBMatchAlertBox(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bMatchAlertBox);
}

SGBMatchAlertBox* SGBMatchAlertBox::create(SGBoxDelegate *delegate)
{
    SGBMatchAlertBox *box = new SGBMatchAlertBox();
    if (box && box->init(delegate)) {
        box->t_delegate = delegate;
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

bool SGBMatchAlertBox::init(SGBoxDelegate *delegate)
{
    if (!SGBaseBox::init(delegate, box_matchAlert, CCSizeMake(530, 320))) {
        return false;
    }
    return true;
}

void SGBMatchAlertBox::initView()
{
 
    
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_bMatchAlertBox, LIM_PNG_AS_PNG);
    
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    // box title
//    CCSprite *title = CCSprite::createWithSpriteFrameName("upwar_box_match3.png");
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_BMatchAlertBox_str1, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(biao->getPosition());
    this->addChild(title);
    
    const char *fontName = FONT_PANGWA;
    const char *desc1 = str_BMatchAlertBox_str2;
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(desc1, fontName, 30);
    label1->setPosition(ccpAdd(title->getPosition(), ccp(6, -105)));
    this->addChild(label1);
    
    SGCCLabelTTF *label2 = SGCCLabelTTF::create(str_BMatchAlertBox_str3, fontName, 30 , ccYELLOW);
    //modify by:zyc. merge into create.
    //label2->setColor(ccYELLOW);
    label2->setPosition(ccpAdd(title->getPosition(), ccp(7, -158)));
//    this->addChild(label2);
    SGCCLabelTTF *label3 = SGCCLabelTTF::create("", fontName, 30 , ccYELLOW);
    //modify by:zyc. merge into create.
    //label3->setColor(ccYELLOW);
    label3->setAnchorPoint(ccp(0, .5f));
    label3->setPosition(ccpAdd(label2->getPosition(), ccp(-label2->getContentSize().width*.5f, -30)));
//    this->addChild(label3);
    
    SGButton *goonBtn = SGButton::create("box_btn_newbg.png",
                                         "upwar_button_shi.png",
                                         this,
                                         menu_selector(SGBMatchAlertBox::macthGoonAction),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(goonBtn);
    float btnWid = goonBtn->getContentSize().width;
    float posY = -size.height/2+40;
    goonBtn->setPosition(ccpAdd(center, ccp(-btnWid*0.75f, posY)));
    
    SGButton *stopBtn = SGButton::create("box_btn_newbg.png",
                                         "upwar_button_match3.png",
                                         this,
                                         menu_selector(SGBMatchAlertBox::macthLaterAction),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(stopBtn);
    stopBtn->setPosition(ccpAdd(center, ccp(btnWid*0.75f, posY)));
    
//    SGButton *laterBnt = SGButton::create("box_btnbg.png",
//                                         "upwar_button_match3.png",
//                                         this,
//                                         menu_selector(SGBMatchAlertBox::macthStopAction),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
//    this->addBtn(laterBnt);
//    laterBnt->setPosition(ccpAdd(center, ccp(btnWid*1.05f, posY)));
    
    // init time label
    counter = 6;

    CCString *ctime = CCString::createWithFormat("%d", counter);
    label = CCLabelTTF::create(ctime->getCString(), FONT_PANGWA, 32);
    label->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width * 0.7, 0)));
    label->setColor(ccRED);
    
    CCAction *newAction = CCSequence::create(CCDelayTime::create(1.0f),
                                             CCCallFunc::create(this, callfunc_selector(SGBMatchAlertBox::showCounter)),
                                             NULL);
    label->runAction(newAction);
    this->addChild(label);
}

void SGBMatchAlertBox::showCounter()
{
    if (isLater) return;
    
    counter -= 1;
    if (counter >= 0) {
        CCString *cstr = CCString::createWithFormat("%d", counter);
        label->setString(cstr->getCString());
        label->setScale(2.0f);
        CCAction *action = CCSequence::create(CCScaleTo::create(0.3f, 1.0f),
                                              CCDelayTime::create(1.0f),
                                              CCCallFunc::create(this, callfunc_selector(SGBMatchAlertBox::showCounter)),
                                              NULL);
        label->runAction(action);
    }
    if (counter == 0) {
        this->macthGoonAction();
    }
}

void SGBMatchAlertBox::macthStopAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGPlayerInfo::sharePlayerInfo()->setIspvp(false);
    main::PvpExitApplyRequest *request = new main::PvpExitApplyRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_SAPPLY, request, false);
    
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
}

void SGBMatchAlertBox::macthStopRespone(CCObject *sender)
{
    // no respone
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvpApplyResponse *response = (main::PvpApplyResponse *)request->m_msg;
    
    if (response) {
        SGBaseBox::boxClose();
        this->resetMainTouch(true);
    }
}

void SGBMatchAlertBox::macthLaterAction()
{
    main::PvpExitApplyRequest *request = new main::PvpExitApplyRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_SAPPLY, request, false);
    SGPlayerInfo::sharePlayerInfo()->setIspvp(false);

    SGBaseBox::boxClose();
    this->resetMainTouch(true);
    
//    SGPlayerInfo::sharePlayerInfo()->set_Wait(1);
//    EFFECT_PLAY(MUSIC_BTN);
//    isLater = true;
//    
//    this->setVisible(false);
//    this->resetMainTouch(true);
//    
//    // 先停止速配，3分钟后重新触发速配
//    main::PvpExitApplyRequest *request = new main::PvpExitApplyRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_SAPPLY, request, false);
//    
//    CCAction *action = CCSequence::create(CCDelayTime::create(10.0f*3),
//                                          CCCallFunc::create(this, callfunc_selector(SGBMatchAlertBox::matchAction)),
//                                          NULL);
//    this->runAction(action);
}

void SGBMatchAlertBox::macthGoonAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//    int count = player->getOfficerCards()->count() + player->getEquipCards()->count() + player->getPropCards()->count() +player->getSoldierCards()->count();
    int count = player->getGoodsNumInBag();
    if (count >= player->getPlayerBagSize()) {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getMainLayer(), NULL, 12, count);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
        
        main::PvpExitApplyRequest *request = new main::PvpExitApplyRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_SAPPLY, request, false);
        SGPlayerInfo::sharePlayerInfo()->setIspvp(false);
        
        SGBaseBox::boxClose();
        this->resetMainTouch(true);
    }
    else
    {
    // 等待获取对手数据，先设置自己数据
    SGBMatchEndBox *box = SGBMatchEndBox::create(t_delegate);
    SGPlayerInfo::sharePlayerInfo()->setIspvp(false);
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
    
    main::PvpGetReadyRequest *request = new main::PvpGetReadyRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_READYS, request);
    SGRankPlayer *selfInfo = SGPlayerInfo::sharePlayerInfo()->getPvpRank();
    box->setSelfData(selfInfo);
    SGMainManager::shareMain()->showBox(box);
    }
}

void SGBMatchAlertBox::matchAction()
{
    SGPlayerInfo::sharePlayerInfo()->set_Wait(0);
    main::PvpApplyRequest *request = new main::PvpApplyRequest();
    request->set_type(SGPlayerInfo::sharePlayerInfo()->getBattleType());
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_APPLY, request);
    
//    // 显示 cant box
//    SGBMatchStartBox *box = SGBMatchStartBox::create(NULL);
//    SGMainManager::shareMain()->showBox(box);
}

void SGBMatchAlertBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer && nowLayer->getTag()!= sg_visitShowCard) {
        nowLayer->setIsCanTouch(touch);
    }
    
}