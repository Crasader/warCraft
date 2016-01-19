//
//  SGRewardYueKaLayer.cpp
//  GameSanGuo
//
//  Created by 曾慧 on 14-2-11.
//
//

#include "SGRewardYueKaLayer.h"
#include "SGMainManager.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "cocos-ext.h"
#include "MonthlyCardModule.pb.h"
#include "SGMenu.h"
#include "SimpleAudioEngine.h"
#include "SGCantAdvanceBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGShowRewardBox.h"
USING_NS_CC_EXT;

#define REWARDFONTSIZE 14
#define YUEKAITEMBASETAG 200

SGRewardYueKaLayer::SGRewardYueKaLayer()

{
}

SGRewardYueKaLayer::~SGRewardYueKaLayer()
{
    _array->release();
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REWARD_YUEKAGETGOLD);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_yuekaRewardLayer);
}

SGRewardYueKaLayer *SGRewardYueKaLayer::create(CCArray *array)
{
    SGRewardYueKaLayer *rewardLayer = new SGRewardYueKaLayer();
    if (rewardLayer && rewardLayer->init(NULL, sg_yuekaRewardLayer))
    {
        rewardLayer->_array = array;
        rewardLayer->_array->retain();
        rewardLayer->initView();
        rewardLayer->autorelease();
        return rewardLayer;
    }
    CC_SAFE_DELETE(rewardLayer);
    return NULL;
}
void SGRewardYueKaLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_yuekaRewardLayer);
    //ResourceManager::sharedInstance()->bindTexture("sgtasklayer/sgtasklayer.plist",RES_TYPE_LAYER_UI ,sg_yuekaRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_yuekaRewardLayer);
	
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->isPostData = true;

    notification->addObserver(MSG_REWARD_YUEKAGETGOLD,this,callfuncO_selector(SGRewardYueKaLayer::yueKaRewardListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGRewardYueKaLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    
	SGCCLabelTTF *invite_friend = SGCCLabelTTF::create(str_RewardYueKaLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//invite_friend->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(invite_friend);
    
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
    invite_friend->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65/2)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, invite_friend->getPosition().y));
    
    //文字说明

    
    //奖励Item
    CCPoint basePos = ccpAdd(SGLayout::getPoint(kMiddleCenter),ccp(0 ,skewingY(60)));
    int yueKaType =  0;
    int yueKaGold = 0;
    int yueKaState = 0;
    int yueKaDays = 0;
    
    int rewardsListNum  = _array->count();
    YueKa_Item *item = NULL;
    for (int i=0; i<rewardsListNum; i++) {
        sscanf(((CCString*)_array->objectAtIndex(i))->getCString(), "%d - %d - %d - %d", &yueKaType,&yueKaGold,&yueKaState,&yueKaDays);
        removeChildByTag(YUEKAITEMBASETAG+yueKaType, true);
        
        item = YueKa_Item::create(this,yueKaGold,yueKaState,yueKaType,yueKaDays);
        if (item != NULL) {
            item->setAnchorPoint(ccp(0.5,0));
            item->ignoreAnchorPointForPosition(false);
            item->setPosition(ccpAdd(basePos,ccp(item->getContentSize().width/2+s.width,(-item->getContentSize().height-60)*i)));
            
            
            addChild(item,1,YUEKAITEMBASETAG+yueKaType);

            CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-s.width*1.05, 0));
            CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(s.width*0.1, 0));
            CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-s.width*0.05, 0));

            CCAction *action = CCSequence::create(CCDelayTime::create(0.15f*(i/10.0f+1)),moveby1,moveby2,moveby3,NULL);
            item->runAction(action);
        }
    }
}

void SGRewardYueKaLayer::getGoldHandler(YueKa_Item *YueKa_Item)
{
    EFFECT_PLAY(MUSIC_34);
    main::MonthlyCardRewardRequest *request = new main::MonthlyCardRewardRequest();
    request->set_type(YueKa_Item->type);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_YUEKAGETGOLD, request);
}
void SGRewardYueKaLayer::yueKaRewardListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;    
    if (sr)
    {
        main::MonthlyCardRewardResponse *response = (main::MonthlyCardRewardResponse *)sr->m_msg;
        if (response->state() > 0)
        {
            int yueKaType = response->type();
            lvlupItem = (YueKa_Item*)getChildByTag(YUEKAITEMBASETAG+yueKaType);
            
            int yueKaGold = lvlupItem->gold;
            //SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 40,yueKaGold);
            //SGMainManager::shareMain()->showBox(cantadvanceBox);
            
            /////////////////////////
            CCArray *rewards = CCArray::create();
            rewards->retain();
            
            SGMailSystemAccessoryDataModel *temp = new SGMailSystemAccessoryDataModel();
            temp->setItemId(0);
            temp->setType(0);
            temp->setCount(yueKaGold);
            rewards->addObject(temp);
            
            
            SGShowRewardBox *rewardBox = SGShowRewardBox::create(this, rewards);
            SGMainManager::shareMain()->showBox(rewardBox);
            
            delete temp;

            
            
            
            
            
        
            if (lvlupItem != NULL) {
                ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_yuekaRewardLayer);
                lvlupItem->btn->init("reward_font_ylq.png", NULL, lvlupItem, menu_selector(YueKa_Item::getGoldHandler),CCPointZero,false,true);
                lvlupItem->state = 1;
            }
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_RewardYueKaLayer_str2);
        }
    }
}

void SGRewardYueKaLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}


YueKa_Item::YueKa_Item()
:
state(0),
gold(0),
type(0),
days(0),
item(NULL),
btn(NULL)
{
    
}

YueKa_Item::~YueKa_Item()
{

}

YueKa_Item* YueKa_Item::create(SGYueKaDelegate *dlg,int gold,int station,int type,int days)
{
	YueKa_Item *instance = new YueKa_Item();
	if (instance&&instance->init()) {
        instance->gold = gold ;
        instance->delegate = dlg;
        instance->state = station;
        instance->type = type;
        instance->days = days;
        instance->setView(station);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

void YueKa_Item::setView(int station)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    
    item = CCSprite::createWithSpriteFrameName("item_bg.png");
    bgbgbg =item;
    this->addChild(item,-1);
    this->setContentSize(item->getContentSize());
    
    SGCCLabelTTF *lab_up = SGCCLabelTTF::create(str_RewardYueKaLayer_str3,FONT_PANGWA,28);
    this->addChild(lab_up,0,10);
    lab_up->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width *.2, 0)));

//    CCSprite *yb = CCSprite::createWithSpriteFrameName("store_yb.png");
//    yb->setPosition(ccpAdd(lab_up->getPosition(), ccp(lab_up->getContentSize().width/2 + 30 , 0)));
//    this->addChild(yb,0,14);
 
    SGCCLabelTTF *font_num = SGCCLabelTTF::create("123", FONT_PANGWA, 26 , ccGREEN);
    this->addChild(font_num,0,15);
    font_num->setAnchorPoint(ccp(0, 0.5));
    font_num->setPosition(ccpAdd(lab_up->getPosition(), ccp(lab_up->getContentSize().width/2+4, 0)));
    //modify by:zyc. merge into create.
    //font_num->setColor(ccGREEN);
    
    SGCCLabelTTF *font_days = SGCCLabelTTF::create("123", FONT_PANGWA, 26 , ccYELLOW);
    this->addChild(font_days,0,22);
    font_days->setAnchorPoint(ccp(1, 0.5));
    font_days->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width/2, -item->getContentSize().height/2 -8)));
    //modify by:zyc. merge into create.
    //font_days->setColor(ccYELLOW);
    
    btn = SGButton::create("reward_lingq.png", NULL, this, menu_selector(YueKa_Item::getGoldHandler),CCPointZero);//,false,true);

    //SGMenu *menu1 = SGMenu::create(CCRectMake(0, btmhgt, s.width, s.height - btmhgt - headhgt));
    CCMenu *menu1 = CCMenu::create();
    menu1->setTag(111);
    menu1->setPosition(CCPointZero);
    this->addChild(menu1,5);
    menu1->addChild(btn);
    btn->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.4 - 10, 8)));

    updateitem(station);
}

void YueKa_Item::updateitem(int station)
{
    state = station;
    SGCCLabelTTF *font_num =  (SGCCLabelTTF *)this->getChildByTag(15);
    SGCCLabelTTF *font_days =  (SGCCLabelTTF *)this->getChildByTag(22);
    

    item->setVisible(true);
    btn->setVisible(true);

    font_num->setVisible(true);
    font_num->setString(CCString::createWithFormat(str_InviterLayer_str8,gold)->getCString());
    
    //font_num =  (SGCCLabelTTF *)this->getChildByTag(22);
    if (days > 0) {
        font_days->setString(CCString::createWithFormat(str_RewardYueKaLayer_str4,days)->getCString());
    }
    else if(days == 0)
    {
       font_days->setString(str_RewardYueKaLayer_str5);
    }else if(days < 0 )
    {
        font_days->setString(str_RewardYueKaLayer_str101);
    }
    
    
    if (station == 1)
    {
        //btn->init("reward_font_ylq.png", NULL, this, menu_selector(YueKa_Item::getGoldHandler),CCPointZero,false,true);
        btn->setImage("reward_font_ylq.png");
    }
    else if(station == 2)
    {
        //btn->init("reward_lingq_hui.png", NULL, this, menu_selector(YueKa_Item::getGoldHandler),CCPointZero,false,true);
        btn->setImage("reward_lingq_hui.png");
    }
}

void YueKa_Item::getGoldHandler(cocos2d::CCNode *sender)
{
    if (state == 1)
    {
        SGMainManager::shareMain()->showMessage(str_RewardYueKaLayer_str6);
        EFFECT_PLAY(MUSIC_BTN);
    }else if (state == 2)
    {
        SGMainManager::shareMain()->showMessage(str_RewardYueKaLayer_str7);
        EFFECT_PLAY(MUSIC_BTN);
    }else if(delegate) {
        delegate->getGoldHandler(this);
    }
}
