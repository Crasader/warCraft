//
//  SGExchangeCodeLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-1-6.
//
//

#include "SGExchangeCodeLayer.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"
SGExchangeCodeLayer ::SGExchangeCodeLayer(){
    _desprition=NULL;
}
SGExchangeCodeLayer::~SGExchangeCodeLayer(){
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_exchangeCodeLayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_EXCHANGECODE_REARD);
    
}

SGExchangeCodeLayer * SGExchangeCodeLayer::create(CCString *desprition){
    
    SGExchangeCodeLayer *layer=new SGExchangeCodeLayer();
    if (layer && layer->init(NULL, sg_exchangeCodeLayer)) {
        layer->autorelease();
        layer->_desprition=CCString::create(desprition->getCString());
        layer->initView();
       
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;

}
void SGExchangeCodeLayer::initView(){

     SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_EXCHANGECODE_REARD, this, callfuncO_selector(SGExchangeCodeLayer::exchangeListener));


    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI, sg_exchangeCodeLayer);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_exchangeCodeLayer);
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);

    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ExchangeCodeLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGExchangeCodeLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65*.48)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(upCenter, ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
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
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    title->setPosition(ccpAdd(upCenter, ccp(0,  -headhgt + 65*.48)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));
    
    
    float fixHight=0;
    if (s.height==960) {
        fixHight=-20;
    }
    else if (s.height==1136)
    {}
    else
        fixHight=-10;
    
    //
    CCScale9Sprite *soldierframe = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    this->addChild(soldierframe,1);
    soldierframe->setPreferredSize(CCSize(598, 150+fixHight));
    soldierframe->setPosition(ccpAdd(upCenter, ccp(0, -s.height*0.35+fixHight)));
    
     spriteBackground1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(spriteBackground1);
    spriteBackground1->setPreferredSize(CCSize(600, 150+fixHight));
    spriteBackground1->setPosition(soldierframe->getPosition());
//    CCScale9Sprite *btou2 = CCScale9Sprite::createWithSpriteFrameName("bantou.png");
//    this->addChild(btou2,-1);
//    btou2->setPreferredSize(CCSize(615, 21));
//    btou2->setAnchorPoint(ccp(0.5, 1));
//    btou2->setPosition(ccpAdd(soldierframe->getPosition(), ccp(15, -soldierframe->getContentSize().height*.5f)));
    
    
    editNikeName = CCEditBox::create(CCSizeMake(500,42), CCScale9Sprite::createWithSpriteFrameName("friend_searchinput.png"));
    editNikeName->setAnchorPoint(ccp(0.5, 0.5));
    editNikeName->setPosition(ccpAdd(spriteBackground1->getPosition(),ccp(0, s.width*0.0)));
    editNikeName->setPlaceHolder(str_ExchangeCodeLayer_str2);
    editNikeName->setDelegate(this);
    this->addChild(editNikeName);
    
    //SGButton * SGButton::createFromLocal(const char *spFile, const char *font, CCObject*target, SEL_MenuHandler selector,CCPoint mov,const char *fontName,const ccColor3B &color,float fontSize,bool isFilx,bool isFrame)
    SGButton *buttonSearch1 =SGButton::createFromLocal("loginbtnbg.png", str_PlayerInfoLayer_str5, this, menu_selector(SGExchangeCodeLayer::exchangeCode),CCPointZero,FONT_BOXINFO,ccWHITE, 34, true, true);
    buttonSearch1->setAnchorPoint(ccp(0.5, 1));
    buttonSearch1->setPosition(ccpAdd(editNikeName->getPosition(), ccp(0, -buttonSearch1->getContentSize().height*2-fixHight)));
    buttonSearch1->setScale(1.2);
    this->addBtn(buttonSearch1);
    
    
    
    //说明
    
    CCScale9Sprite *soldierframe2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    this->addChild(soldierframe2,1);
    soldierframe2->setPreferredSize(CCSize(598, 270+fixHight));
    soldierframe2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -s.height*0.2)));
    
    CCScale9Sprite *spriteBackground2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(spriteBackground2);
    spriteBackground2->setPreferredSize(CCSize(600, 270+fixHight));
    spriteBackground2->setPosition(soldierframe2->getPosition());
//    CCScale9Sprite *btou3 = CCScale9Sprite::createWithSpriteFrameName("bantou.png");
//    this->addChild(btou3,-1);
//    btou3->setPreferredSize(CCSize(615, 21));
//    btou3->setAnchorPoint(ccp(0.5, 1));
//    btou3->setPosition(ccpAdd(soldierframe2->getPosition(), ccp(15, -soldierframe2->getContentSize().height*.5f)));
    
    SGCCLabelTTF *exchangeTitleLabel=SGCCLabelTTF::create(str_ExchangeCodeLayer_str3, FONT_PANGWA, 30 , ccYELLOW);
    addChild(exchangeTitleLabel);
    //exchangeTitleLabel->setColor(ccYELLOW);
    
//    CCString *subTitle=CCString::create("1.兑换码会在部分活动中给广大玩家发放!\n 2.您可以在网站 www.pengyou.com! 中经常关注兑换码的发布情况！\n 3.每种类型的兑换码，一个帐号只能使用一次。\n ");
    
    
    SGCCLabelTTF *exchangeSubTitleLabel=SGCCLabelTTF::create(_desprition->getCString(), FONT_BOXINFO, 26, CCSizeMake(580, 270));
    addChild(exchangeSubTitleLabel);
    
    exchangeTitleLabel->setPosition(ccpAdd(spriteBackground2->getPosition(), ccp(-spriteBackground2->getContentSize().width/2+exchangeTitleLabel->getContentSize().width/2+20, spriteBackground2->getContentSize().height/2-25)));
    exchangeSubTitleLabel->setPosition(ccpAdd(spriteBackground2->getPosition(), ccp(5, -60)));
    

}

void SGExchangeCodeLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}
void SGExchangeCodeLayer::exchangeCode(){
//    
//    CCArray *ary=CCArray::create();
//    
//    for(int i=0; i<2; i++)
//    {
//        SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
//        datamodel->setType(0);
//        datamodel->setCount(111);
//        ary->addObject(datamodel);
//    }
//   
    std::string string=editNikeName->getText();
   
    if (GameConfig::isContainsEmoji(string.c_str())) {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str4);
        return;
    }
    
    if (string=="") {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str5);
        return;
    }
    
    if (string.length()>10) {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str6);
        return;
    }
  
    
//    SGShowRewardBox *box=SGShowRewardBox::create(this,ary ,3)
//    ;
//    SGMainManager::shareMain()->showBox(box);
    
    main::ExchangeCodeInfoRequest *exchangeCodeInfo= new main::ExchangeCodeInfoRequest;
    exchangeCodeInfo->set_code(editNikeName->getText());
    
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGECODE_REARD , exchangeCodeInfo);
    
}
void SGExchangeCodeLayer::exchangeListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest *)obj;
    main::ExchangeCodeInfoResponse *response = (main::ExchangeCodeInfoResponse *)request->m_msg;
    if (response->state()==1)
    {
        int length = response->rewardinfo_size();
        CCArray *ary=CCArray::create();
        
        for(int i=0; i<length; i++)
        {
            main::RewardInfoProto ss = response->rewardinfo(i);
            
            SGMailSystemAccessoryDataModel *datamodel = new SGMailSystemAccessoryDataModel;
            //无需判断奖励的类型，最终会由SGShowRewardBox里面的逻辑进行解析。
            datamodel->setType(ss.type());
            datamodel->setCount(ss.number());
            datamodel->setItemId(ss.value());
            ary->addObject(datamodel);
        }
        
        editNikeName->removeFromParentAndCleanup(true);
        SGShowRewardBox *box=SGShowRewardBox::create(this, ary, 3);
        SGMainManager::shareMain()->showBox(box);
    }
    else if (response->state()==3)
    {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str7);
    }
    else if (response->state()==2)
    {
         SG_SHOW_WINDOW(str_ExchangeCodeLayer_str8);
    }
    else if (response->state()==4)
    {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str9);
    }
    else if(response->state()==7)
    {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str10);
    }
    else
    {
        SG_SHOW_WINDOW(str_ExchangeCodeLayer_str11);
    }
}

void SGExchangeCodeLayer::showBoxCall(CCObject *obj){
    
//    
    editNikeName = CCEditBox::create(CCSizeMake(500,42), CCScale9Sprite::createWithSpriteFrameName("friend_searchinput.png"));
    editNikeName->setAnchorPoint(ccp(0.5, 0.5));
    editNikeName->setPosition(ccpAdd(spriteBackground1->getPosition(),ccp(0, 0.0)));
    editNikeName->setPlaceHolder(str_ExchangeCodeLayer_str2);
    editNikeName->setDelegate(this);
    this->addChild(editNikeName);
    

}

void SGExchangeCodeLayer::editBoxEditingDidBegin(CCEditBox* editBox){}



void SGExchangeCodeLayer::editBoxEditingDidEnd(CCEditBox* editBox){}

void SGExchangeCodeLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text){


}

void SGExchangeCodeLayer::editBoxReturn(CCEditBox* editBox){
    
}

