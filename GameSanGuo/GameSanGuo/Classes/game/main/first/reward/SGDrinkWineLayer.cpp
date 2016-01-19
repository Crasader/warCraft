//
//  SGDrinkWineLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#include "SGDrinkWineLayer.h"
#include "SGMainManager.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGLottery9ViewItem.h"
#include "SGRewardLayer.h"

SGDrinkWineLayer::SGDrinkWineLayer()
:recoverPower(0)
{
}
SGDrinkWineLayer::~SGDrinkWineLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_RECOVER);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_drinkWineLayer);
    
}
SGDrinkWineLayer *SGDrinkWineLayer::create(int power,int iscan)
{
    SGDrinkWineLayer *Layer = new SGDrinkWineLayer();
    if (Layer && Layer->init(NULL , sg_drinkWineLayer))
    {
        Layer->recoverPower = power;
        Layer->initView(iscan);
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGDrinkWineLayer::initView(int iscan)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_drinkWineLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_RECOVER,
                              this,
                              callfuncO_selector(SGDrinkWineLayer::recoverListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/reward_winebg.plist", RES_TYPE_LAYER_UI, sg_drinkWineLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("reward_winebg.png");
    bg->setScaleY((s.height - headhgt - btmhgt)/702);
    bg->setScaleX(s.width/768);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));
    
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGDrinkWineLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGDrinkWineLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
      
//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_drink.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_DrinkWineLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65*.48)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));

    
    CCSprite *fontbg = CCSprite::createWithSpriteFrameName("reward_hk.png");
    this->addChild(fontbg);
    fontbg->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(fontbg->getContentSize().width*.55, -headhgt - fontbg->getContentSize().height*.7)));
    
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(CCString::createWithFormat(str_DrinkWineLayer_str2,recoverPower)->getCString(), FONT_BOXINFO, 26);
    this->addChild(label1);
    label1->setPosition(fontbg->getPosition());
    
    CCSprite *fk = CCSprite::createWithSpriteFrameName("reward_fk.png");
    this->addChild(fk);
    fk->setPosition(ccp(fk->getContentSize().width*.6, btmhgt + fk->getContentSize().height));
    
    CCSprite *font = CCSprite::createWithSpriteFrameName("reward_font_drinktime.png");
    this->addChild(font);
    font->setAnchorPoint(ccp(0, 0.5));
    font->setPosition(ccpAdd(fk->getPosition(),ccp(-fk->getContentSize().width*.48, fk->getContentSize().height/2)));
    
    SGCCLabelTTF *label_time1 = SGCCLabelTTF::create(str_DrinkWineLayer_str3, FONT_BOXINFO, FONTSIZE(12));
    SGCCLabelTTF *label_time2 = SGCCLabelTTF::create(str_DrinkWineLayer_str4, FONT_BOXINFO, FONTSIZE(12));
    this->addChild(label_time1);
    this->addChild(label_time2);
    label_time1->setPosition(ccpAdd(fk->getPosition(), ccp(0, fk->getContentSize().height*.1)));
    label_time2->setPosition(ccpAdd(fk->getPosition(), ccp(0, -fk->getContentSize().height*.25)));
    
    SGButton *btn = SGButton::create("reward_btn_redbg.png", "font_drinkwine.png", this, menu_selector(SGDrinkWineLayer::confirmHandler),CCPointZero,false,true);
    this->addBtn(btn);
    btn->setPosition(ccpAdd(fk->getPosition(), ccp(s.width*.63, -17)));
    
    CCSprite *jiuhu = NULL;
    if (iscan) {
       jiuhu = CCSprite::createWithSpriteFrameName("reward_jiuhu.png");
        btn->setVisible(true);
        btn->setEnabled(true);
    }else
    {
        jiuhu = CCSprite::createWithSpriteFrameName("reward_jiuhu1.png");
        btn->setVisible(false);
        btn->setEnabled(false);
    }
    this->addChild(jiuhu,1,10);
    jiuhu->setAnchorPoint(ccp(0.5, 0));
    jiuhu->setScale(s.width/768);
    
    if (s.height == 1136)
    {
        if (iscan) {
        jiuhu->setPosition(ccpAdd(bg->getPosition(), ccp(17, skewingY(73))));
        }else
        {
            jiuhu->setPosition(ccpAdd(bg->getPosition(), ccp(-17, skewingY(73))));

        }
    }
    else
    {
        if (iscan) {
            jiuhu->setPosition(ccpAdd(bg->getPosition(), ccp(17, skewingY(65))));

        }else
        {
            jiuhu->setPosition(ccpAdd(bg->getPosition(), ccp(-17, skewingY(65))));
        }
    }
    
}
void SGDrinkWineLayer::recoverListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        const char *msg = "";
        main::RewardRecoverPowerPurchResponse *response = (main::RewardRecoverPowerPurchResponse *)sr->m_msg;
        if (response->result() == 1) {
			EFFECT_PLAY(MUSIC_37);//成功的时候播放祭天的音效

            //CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(4);
            //str->setString(CCString::createWithFormat("%d",0)->getCString());
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
            int nsum = array->count();
            for (int i = 0 ;i < nsum ;i++) {
                SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                if(item->gettypeItem()==Reward_Wine)
                {
                    item->setstateItem(0);
                    break;
                }
            }
            SGPlayerInfo::sharePlayerInfo()->setPlayerPower(SGPlayerInfo::sharePlayerInfo()->getPlayerPower() + recoverPower);
            msg = str_DrinkWineLayer_str5;
            
            CCSprite *a = (CCSprite*)this->getChildByTag(10);
            a->runAction(CCSequence::create(CCFadeOut::create(1),CCCallFuncN::create(this, callfuncN_selector(SGDrinkWineLayer::updatejiuhu)),NULL));
            
        }
        if (response->result() == 2) {
            msg = str_DrinkWineLayer_str6;
        }
        if (response->result() == 3) {
            msg = str_DrinkWineLayer_str7;
        }
        SGMainManager::shareMain()->showMessage(msg);
    }
}
void SGDrinkWineLayer::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::RewardRecoverPowerPurchRequest *request = new main::RewardRecoverPowerPurchRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_RECOVER, request);
}
void SGDrinkWineLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}
void SGDrinkWineLayer::updatejiuhu()
{
     ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_drinkWineLayer);
    CCSprite* a = CCSprite::createWithSpriteFrameName("reward_jiuhu1.png");
    a->setScale(skewing(320)/768);
    a->setAnchorPoint(ccp(0.5, 0));
    this->addChild(a);
    a->setPosition(ccpAdd(this->getChildByTag(10)->getPosition(), ccp(-34, 0)));
    a->runAction(CCFadeIn::create(1));
}