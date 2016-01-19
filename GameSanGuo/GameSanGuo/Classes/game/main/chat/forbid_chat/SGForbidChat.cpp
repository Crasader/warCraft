//
//  SGForbidChat.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#include "SGForbidChat.h"
#include "SGPlayerInfo.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "MessageModule.pb.h"
#include "SGMainManager.h"
#include "ShopModule.pb.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGForbidChat::SGForbidChat()
{
    
}

SGForbidChat::~SGForbidChat()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORE_ENTER);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_forbidChat);
}

SGForbidChat* SGForbidChat::create(SGBoxDelegate *delegate)
{
    SGForbidChat *forbid = new SGForbidChat();
    if(forbid && forbid->init(delegate, box_forbidChat, CCSizeMake(535, 365)))
    {
        
        forbid->constructView();
        forbid->autorelease();
        return forbid;
    }
    
    CC_SAFE_DELETE(forbid);
    return NULL;
}

void SGForbidChat::constructView()
{
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
     ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_box_forbidChat, LIM_PNG_AS_PNG);
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_STORE_ENTER,
                              this,
                              callfuncO_selector(SGForbidChat::enterstorelayer));
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("chat_forbid_chat.png");
    CCLabelTTF *label_1 = CCLabelTTF::create(str_ForbidChat_str1, FONT_BOXINFO, 32);
    CCLabelTTF *label_2 = CCLabelTTF::create(str_ForbidChat_str2, FONT_BOXINFO, 32);
    CCLabelTTF *label_3 = CCLabelTTF::create(CCString::createWithFormat(str_ForbidChat_str3,SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString(), FONT_BOXINFO, 32);
    CCLabelTTF *label_4 = CCLabelTTF::create(str_ForbidChat_str4, FONT_BOXINFO, 32);
    label_4->setColor(COLOR_RED);
    spriteTitle->setPosition(biao->getPosition());
    
    label_1->setAnchorPoint(ccp(0, 0.5f));
    label_2->setAnchorPoint(ccp(0, 0.5f));
    label_3->setAnchorPoint(ccp(0, 0.5f));
    label_4->setAnchorPoint(ccp(0, 0.5f));
    
    label_1->setPosition(ccpAdd(biao->getPosition(), ccp(-205, -90)));
    label_2->setPosition(ccpAdd(label_1->getPosition(), ccp(0, -55)));
    label_3->setPosition(ccpAdd(label_2->getPosition(), ccp(0, -55)));
    label_4->setPosition(ccpAdd(label_3->getPosition(), ccp(label_3->getContentSize().width, 0)));

    label_4->setVisible(!isCanPay());
    
    this->addChild(spriteTitle);
    this->addChild(label_1);
    this->addChild(label_2);
    this->addChild(label_3);
    this->addChild(label_4);
    
    CCString *okFileNameStr = CCString::create("");
    SGButton *ok = NULL;
    SGButton *cancel = NULL;
    if(isCanPay())
    {
        okFileNameStr->setString("public_font_queding.png");
        ok = SGButton::create("box_btnbg.png",
                              okFileNameStr->getCString(),
                              this,
                              menu_selector(SGForbidChat::buttonClickOK),
                              ccp(0, 0),
                              false,
                              true);
        cancel = SGButton::create("box_btnbg.png",
                                  "public_font_quxiao.png",
                                  this,
                                  menu_selector(SGForbidChat::buttonClickCancel),
                                  ccp(0, 0),
                                  false,
                                  true);
        
    }else{
        
        
      ok = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGForbidChat::buttonClickOK),CCPointZero,FONT_PANGWA,ccWHITE,32);
        
        okFileNameStr->setString("store_chongz.png");
        
        cancel = SGButton::create("box_btn_newbgHui.png",
                                  "public_font_quxiao.png",
                                  this,
                                  menu_selector(SGForbidChat::buttonClickCancel),
                                  ccp(0, 0),
                                  false,
                                  true);
        
        
    }
    
    this->addBtn(ok);
    this->addBtn(cancel);
    
    ok->setPosition(ccpAdd(center, ccp(-145, -365/2+5 -40)));
    cancel->setPosition(ccpAdd(center, ccp(145, -365/2+5-40)));
}

//是否可以支付当前的小喇叭
bool SGForbidChat::isCanPay()
{
    return (SGPlayerInfo::sharePlayerInfo()->getPlayerGold() >= 100);
}

#pragma mark 按钮点击

void SGForbidChat::buttonClickCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
//    ((SGBaseLayer *)SGMainManager::shareMain()->getMainLayer()->getChildByTag(sg_mainLayer))->setIsCanTouch(true);
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
    this->boxCloseWithOutCallBack();
}

void SGForbidChat::buttonClickOK()
{

    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
    if(isCanPay())
    {
		 
    }
    else
    {
        if (SGMainManager::shareMain()->getMainLayer())
        {
            SGMainManager::shareMain()->getMainLayer()->requestStoreLayer();
        }

    }
}
void SGForbidChat::enterstorelayer(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopChargeEntryResponse *response = (main::ShopChargeEntryResponse *)sr->m_msg;
    if (sr)
    {
        main::VIPInfo vip = response->vipinfo();
        int leng = response->chargeitems_size();
        CCArray *arrary = CCArray::create();
        CCArray *strary = CCArray::create();
        for (int i = 0;i<leng;i++) {
            main::ChargeItem item = response->chargeitems(i);
            CCString *str = CCString::createWithFormat("%d-%d-%d-%d-%d-%d-%d-%d",item.showtype(),item.chargetype(),item.discount(),item.gold(),item.rmb(),item.goldperday(),item.leftday(),item.rewardday());
            arrary->addObject(str);
            CCString *str2 = CCString::createWithFormat("%s",item.code().c_str());
            strary->addObject(str2);
            
        }
        
        
        SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,0,strary);

    }
    this->boxCloseWithOutCallBack();

}