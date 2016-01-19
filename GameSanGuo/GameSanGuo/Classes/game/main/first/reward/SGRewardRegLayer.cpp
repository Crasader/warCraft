//
//  SGRewardRegLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-20.
//
//

#include "SGRewardRegLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGRewardLayer.h"
SGRewardRegLayer::SGRewardRegLayer()
:leftcount(-1),
lab_iscanget(NULL),
lab_notice(NULL),
lab_sec(NULL),
btn(NULL),
nowTime(0),
chance(0),
cost(0),
addvalue(0),
scrollHeight(0),
scrollLayer(NULL),
scroll(NULL),
scrollArray(NULL),
lab_reward_notice(NULL),
lab_once_notice(NULL)
, isCanClicked(true)
{
    ;
}

SGRewardRegLayer::~SGRewardRegLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_regLotteryLayer);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_REG);
    scrollArray->release();
    
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardRegPlayers();
    if (array->count()/2>=scrollHeight)
    {
        for (int i = scrollHeight*2-1 ; i>=0; i--)
        {
            array->removeObjectAtIndex(i);
        }
    }
}
SGRewardRegLayer *SGRewardRegLayer::create(int a,int b,int c,CCArray *bcsArr)
{
    SGRewardRegLayer *Layer = new SGRewardRegLayer();
    if (Layer && Layer->init(NULL , sg_regLotteryLayer))
    {
        Layer->chance = a;
        Layer->cost = c;
        Layer->nowTime = b;
        
        SGPlayerInfo::sharePlayerInfo()->setRewardRegPlayers(bcsArr);
        
        Layer->initView(a,b,c);
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGRewardRegLayer::initView(int a,int b,int c)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_regLotteryLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jinb.plist", RES_TYPE_LAYER_UI, sg_regLotteryLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/reward_caishen.plist", RES_TYPE_LAYER_UI, sg_regLotteryLayer);
    
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_REG,
                              this,
                              callfuncO_selector(SGRewardRegLayer::regListener));
    
    int m = nowTime;
    int secs1 = m % 60;
    m /= 60;
    int minutes1 = m % 60;
    m /= 60;
    int hours1 = m%24;
    int days = m/24;
    char timeStr1[32];
    memset(timeStr1, 0, 32);
#if (PLATFORM == IOS)
    snprintf(timeStr1, 32, "%d    %02d       %02d", days,hours1, minutes1);
#else
	snprintf(timeStr1, 32, " %d     %02d        %02d", days,hours1, minutes1);
#endif
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/reward_caishenbg.plist", RES_TYPE_LAYER_UI, sg_regLotteryLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("reward_caishenbg.png");
    bg->setScaleY((s.height - headhgt - btmhgt)/702);
    bg->setScaleX(s.width/768);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));
	
	//    SGButton *backBtn = SGButton::create("chatbtn.png",
	//                                         "public_font_fanhui.png",
	//                                         this,
	//                                         menu_selector(SGRewardRegLayer::backHandler),
	//                                         ccp(0, 0),
	//                                         false,
	//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGRewardRegLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setTag(88);
    menu->setZOrder(10);
    
    title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 0));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt-3)));
    this->addChild(title_bg,8);
    
	//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_ycs.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_RewardBindLayer_str8, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title,10);
    
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
    
    CCSprite *timebg = CCSprite::createWithSpriteFrameName("reward_timebg.png");
    this->addChild(timebg);
    timebg->setPosition(ccpAdd(titlecenter->getPosition(), ccp(0, -37)));
    timebg->setScaleX(s.width/timebg->getContentSize().width);
    
    SGCCLabelTTF *font1 = SGCCLabelTTF::create(str_RewardRegLayer_str1, FONT_BOXINFO, 26 , COLOR_ORANGE);
    this->addChild(font1);
    font1->setAnchorPoint(ccp(0, 0.5));
    font1->setPosition(ccpAdd(timebg->getPosition(), ccp(-s.width*.49, 0)));
    //modify by:zyc. merge into create.
    //font1->setColor(COLOR_ORANGE);
    
    SGCCLabelTTF *font2 = SGCCLabelTTF::create(str_RewardRegLayer_str2, FONT_BOXINFO, 26);
    this->addChild(font2);
    font2->setAnchorPoint(ccp(0, 0.5));
    font2->setPosition(ccpAdd(font1->getPosition(), ccp(font1->getContentSize().width +32, 0)));
    
    lab_iscanget = SGCCLabelTTF::create(" 00     00        00", FONT_BOXINFO, 30 , COLOR_YELLOW);
    this->addChild(lab_iscanget);
    //modify by:zyc. merge into create.
    //lab_iscanget->setColor(COLOR_YELLOW);
    lab_iscanget->setAnchorPoint(ccp(0, 0.5));
	
#if (PLATFORM == IOS)
    lab_iscanget->setPosition(ccpAdd(font1->getPosition(), ccp(font1->getContentSize().width +20,0)));
#else
	lab_iscanget->setPosition(ccpAdd(font1->getPosition(), ccp(font1->getContentSize().width,0)));
#endif
	// lab_iscanget->setPosition(ccpAdd(font1->getPosition(), ccp(font1->getContentSize().width +20,0)));
    
    lab_sec = SGCCLabelTTF::create("", FONT_PANGWA, 30 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //lab_sec->setColor(COLOR_YELLOW);
    this->addChild(lab_sec);
#if (PLATFORM == IOS)
    lab_sec->setPosition(ccpAdd(lab_iscanget->getPosition(), ccp(200,0)));
#else
	lab_sec->setPosition(ccpAdd(lab_iscanget->getPosition(), ccp(230,0)));
#endif
    //lab_sec->setPosition(ccpAdd(lab_iscanget->getPosition(), ccp(200,0)));
    
    btn = SGButton::create("reward_csbtnbg.png","reward_csbtn_font1.png", this, menu_selector(SGRewardRegLayer::getCoins),CCPointZero,false,true);
    this->addBtn(btn);
    btn->setPosition(ccpAdd(bg->getPosition(), ccp(0, 185*bg->getScaleY())));
    
    btn->setTag(2013);
    
    CCLabelAtlas *labcost = CCLabelAtlas::create(CCString::createWithFormat("%d",cost)->getCString(), "sanguobigpic/cs_shuzi_1.png", 21, 28, '0');
    this->addChild(labcost,10,100);
    labcost->setPosition(btn->getPosition());
    labcost->setAnchorPoint(ccp(0.5, 0.5));
    
    lab_iscanget->setString(timeStr1);
    lab_sec->setString(CCString::createWithFormat("%02d",secs1)->getCString());
	
    SGCCLabelTTF *font3 = SGCCLabelTTF::create(str_RewardRegLayer_str3,  FONT_PANGWA, 30);
    this->addChild(font3);
    font3->setAnchorPoint(ccp(0, 0.5));
    font3->setPosition(ccpAdd(font2->getPosition(), ccp(font2->getContentSize().width+40, 0)));
    
    lab_notice = SGCCLabelTTF::create(CCString::createWithFormat("%d",chance)->getCString(), FONT_PANGWA, 30 , COLOR_YELLOW);
    this->addChild(lab_notice);
    //modify by:zyc. merge into create.
    //lab_notice->setColor(COLOR_YELLOW);
    lab_notice->setAnchorPoint(ccp(0, 0.5));
    lab_notice->setPosition(ccpAdd(font3->getPosition(), ccp(font3->getContentSize().width, 0)));
    if (chance == 0) {
        btn->setVisible(false);
        SGButton *a = SGButton::create("reward_csbtnhuibg.png", "reward_csbtn_font2.png", this, NULL,CCPointZero,false,true);
        a -> setTag(2013);
        this->addChild(a,20);
        a->setPosition(btn->getPosition());
    }
	
	//    if (chance == 4) {
	//
	//        SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 18, NULL);
	//        SGMainManager::shareMain()->showBox(box);
	//        setIsCanTouch(false);
	//    }
    
    this->schedule(schedule_selector(SGRewardRegLayer::updateTime), 1.0);
    
    CCSprite *goldbg = CCSprite::createWithSpriteFrameName("cs_goldbg.png");
    this->addChild(goldbg,5,55);
    goldbg->setScale(1.3);
    goldbg->setPosition(ccpAdd(btn->getPosition(), ccp(0,skewingY(48))));
    
    CCLabelAtlas *getyb1 = CCLabelAtlas::create("0", "sanguobigpic/cs_shuzi_1.png", 21, 28, '0');
    this->addChild(getyb1,10,3);
    getyb1->setPosition(ccpAdd(goldbg->getPosition(), ccp(-getyb1->getContentSize().width/2-2, -getyb1->getContentSize().height/2-5)));
    
    CCLabelAtlas *getyb2 = CCLabelAtlas::create("0", "sanguobigpic/cs_shuzi_1.png", 21, 28, '0');
    this->addChild(getyb2,10,2);
    getyb2->setPosition(ccpAdd(getyb1->getPosition(), ccp(-24*1.3, 0)));
    
    CCLabelAtlas *getyb3 = CCLabelAtlas::create("0", "sanguobigpic/cs_shuzi_1.png", 21, 28, '0');
    this->addChild(getyb3,10,1);
    getyb3->setPosition(ccpAdd(getyb1->getPosition(), ccp(-48*1.3, 0)));
    
    CCLabelAtlas *getyb4 = CCLabelAtlas::create("0", "sanguobigpic/cs_shuzi_1.png", 21, 28, '0');
    this->addChild(getyb4,10,4);
    getyb4->setPosition(ccpAdd(getyb1->getPosition(), ccp(24*1.3, 0)));
    
    CCLabelAtlas *getyb5 = CCLabelAtlas::create("0", "sanguobigpic/cs_shuzi_1.png", 21, 28, '0');
    this->addChild(getyb5,10,5);
    getyb5->setPosition(ccpAdd(getyb1->getPosition(), ccp(48*1.3, 0)));
    
    getyb1->setScale(1.3);
    getyb2->setScale(1.3);
    getyb3->setScale(1.3);
    getyb4->setScale(1.3);
    getyb5->setScale(1.3);
	
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+40,91));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmhgt + frame2->getContentSize().height*.5 +5)));
    
    CCScale9Sprite *framebg = CCScale9Sprite::createWithSpriteFrameName("reward_infobg.png");
    framebg->setPreferredSize(CCSizeMake(568+44,95));
    this->addChild(framebg,5);
    framebg->setPosition(frame2->getPosition());
    
    CCArray* array = SGPlayerInfo::sharePlayerInfo()->getrewardRegPlayers();
    
    scrollArray = CCArray::create();
    scrollArray->retain();
    for (int i = 0; i<10; i++) {
        if (i<array->count()/2) {
            scrollArray->addObject(array->objectAtIndex(i*2));
            scrollArray->addObject(array->objectAtIndex(i*2+1));
        }else
        {
            break;
        }
    }
    
	
    
    
    scrollHeight = (scrollArray->count()+1)/2;
    scrollLayer = CCLayer::create();
    for (int  i = 0; i<scrollHeight; i++) {
        CCString* strname = (CCString*)scrollArray->objectAtIndex(i*2);
        CCString* strvalue = (CCString*)scrollArray->objectAtIndex(i*2+1);
        
        SGCCLabelTTF*name = SGCCLabelTTF::create(CCString::createWithFormat(str_RewardRegLayer_str4,strname->getCString())->getCString(), FONT_BOXINFO, 26 , ccWHITE);
        //modify by:zyc. merge into create.
        //name->setColor(ccWHITE);
        scrollLayer->addChild(name,5);
        name->setAnchorPoint(ccp(1, 0.5));
		//        name->setPosition(ccp(skewing(160)+170, 62 -40*(i+1)));
		name->setPosition(ccp(s.width*0.5+name->getContentSize().width*0.5, 62 -40*(i+1)));
        
        SGCCLabelTTF*addgold = SGCCLabelTTF::create(CCString::createWithFormat("%d",strvalue->intValue())->getCString(), FONT_BOXINFO,26 , COLOR_YELLOW);
        //modify by:zyc. merge into create.
        //addgold->setColor(COLOR_YELLOW);
        scrollLayer->addChild(addgold);
        addgold->setPosition(ccpAdd(name->getPosition(), ccp(-83, 0)));//-115
    }
    scroll = CCScrollView::create(CCSizeMake(skewing(320),91));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(scroll,6);
    scroll->setPosition(ccp(0,frame2->getPosition().y-frame2->getContentSize().height*.5));
    scroll->setContainer(scrollLayer);
    scroll->setTouchEnabled(false);
    
    scrollLayer->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveBy::create(1, ccp(0, 40)),CCCallFuncN::create(this, callfuncN_selector(SGRewardRegLayer::updatecount)),NULL)));
	
    if(chance==4){
        lab_reward_notice = SGCCLabelTTF::create(str_RewardRegLayer_str5, FONT_BOXINFO, 26 , ccWHITE);
        //modify by:zyc. merge into create.
        //lab_reward_notice->setColor(ccWHITE);
        lab_reward_notice->setPosition(ccpAdd(frame2->getPosition(),ccp(0,18)));
        addChild(lab_reward_notice,7);
        
        
        lab_once_notice = SGCCLabelTTF::create(str_RewardRegLayer_str10, FONT_BOXINFO, 26 , ccWHITE);
        //modify by:zyc. merge into create.
        //lab_once_notice->setColor(ccWHITE);
        lab_once_notice->setPosition(ccpAdd(frame2->getPosition(),ccp(0,-18)));
        addChild(lab_once_notice,7);
    }
    
    this->updatePlayers();
	this->schedule(schedule_selector(SGRewardRegLayer::updatePlayers),5);
    
}
void SGRewardRegLayer::updateTime()
{
    CCLOG("updateTime");
    int m = nowTime;
    int secs = m % 60;
    m /= 60;
    int minutes = m % 60;
    m /= 60;
    int hours = m%24;
    int days = m/24;
    char timeStr[32];
    memset(timeStr, 0, 32);
	
#if (PLATFORM == IOS)
    snprintf(timeStr, 32, "%d    %02d       %02d", days,hours, minutes);
#else
	snprintf(timeStr, 32, " %d     %02d        %02d", days,hours, minutes);
#endif
	
    lab_sec->setString(CCString::createWithFormat("%02d",secs)->getCString());
    lab_iscanget->setString(timeStr);
	
    if (nowTime == 0)
    {
        this->unschedule(schedule_selector(SGRewardRegLayer::updateTime));
        return;
    }
    nowTime--;
}

void SGRewardRegLayer::getCoins()
{
    EFFECT_PLAY(MUSIC_34);
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold() >= cost && chance !=0)
    {
        /*迎财神快速点击，能把自己的元宝变成负的,so...*/
        if(isCanClicked)
        {
            main::RewardRegLotteryGetRequest *request = new main::RewardRegLotteryGetRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_REWARD_REG, request);
            isCanClicked = false;
        }
    }
    else
    {
        SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 19, cost);
        SGMainManager::shareMain()->showBox(box);
    }
}

void SGRewardRegLayer::regListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        /*迎财神快速点击，能把自己的元宝变成负的*/
        isCanClicked = true;
        
        if (lab_reward_notice) {
            removeChild(lab_reward_notice);
        }
        if (lab_once_notice) {
             removeChild(lab_once_notice);
        }
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
        main::RewardRegLotteryGetResponse *response = (main::RewardRegLotteryGetResponse *)sr->m_msg;
        playerInfo->setPlayerGold(playerInfo->getPlayerGold() + response->returngold() );
        playerInfo->setPlayerGold(playerInfo->getPlayerGold() - cost );
        cost = response->nextcostgold();
        addvalue = response->returngold();
        chance--;
        if (chance>= 0) {
            labanimation();
        }
        lab_notice->setString(CCString::createWithFormat("%d",chance)->getCString());
        if (chance !=0) {
            
			//            if (chance<4) {
			//                CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(1);
			//                str->setString(CCString::createWithFormat("%d",1)->getCString());
			//            }
        }else
        {
			// CCString *str = (CCString *)playerInfo->getrewardNotice()->objectAtIndex(2);
			// str->setString(CCString::createWithFormat("%d",0)->getCString());
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
            int nsum = array->count();
            for (int i = 0 ;i < nsum ;i++) {
                SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                if(item->gettypeItem()==Reward_Caishen)
                {
                    item->setstateItem(0);
                    break;
                }
            }
			
			
			//            lab_notice->setString("拜神次数已用完,请等待下次活动");
        }
        
    }
}
void SGRewardRegLayer::labanimation()
{
    this->schedule(schedule_selector(SGRewardRegLayer::jinbiAction),0.2f);
    this->schedule(schedule_selector(SGRewardRegLayer::jinbiAction1),0.2f);
    this->schedule(schedule_selector(SGRewardRegLayer::jinbiAction2),0.2f);
    this->runAction(CCSequence::create(CCDelayTime::create(2),CCCallFunc::create(this, callfunc_selector(SGRewardRegLayer::stopjinbiAct)),NULL));
	
    CCSpriterX* fermEffect = CCSpriterX::create("animationFile/reward_caishen.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect, 50, 103);
    fermEffect->setPosition(ccpAdd(this->getChildByTag(55)->getPosition(), ccp(0,-23)));
    fermEffect->play();
    
    CCSpriterX* fermEffect1 = CCSpriterX::create("animationFile/reward_caishen.scml", true, true);
    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    addChild(fermEffect1, 50, 101);
    fermEffect1->setPosition(ccpAdd(fermEffect->getPosition(), ccp(-48*1.3,0)));
    fermEffect1->play();
    
    CCSpriterX* fermEffect2 = CCSpriterX::create("animationFile/reward_caishen.scml", true, true);
    fermEffect2->setanimaID(0);
    fermEffect2->setisloop(true);
    addChild(fermEffect2, 50, 102);
    fermEffect2->setPosition(ccpAdd(fermEffect->getPosition(), ccp(- 24*1.3,0)));
    fermEffect2->play();
    
    CCSpriterX* fermEffect4 = CCSpriterX::create("animationFile/reward_caishen.scml", true, true);
    fermEffect4->setanimaID(0);
    fermEffect4->setisloop(true);
    addChild(fermEffect4, 50, 104);
    fermEffect4->setPosition(ccpAdd(fermEffect->getPosition(), ccp(24*1.3,0)));
    fermEffect4->play();
    
    CCSpriterX* fermEffect5 = CCSpriterX::create("animationFile/reward_caishen.scml", true, true);
    fermEffect5->setanimaID(0);
    fermEffect5->setisloop(true);
    addChild(fermEffect5, 50, 105);
    fermEffect5->setPosition(ccpAdd(fermEffect->getPosition(), ccp(48*1.3,0)));
    fermEffect5->play();
    
    this->runAction(CCSequence::create(CCDelayTime::create(.6f),CCCallFunc::create(this, callfunc_selector(SGRewardRegLayer::animation5)),CCDelayTime::create(.6f),CCCallFunc::create(this, callfunc_selector(SGRewardRegLayer::animation4)),CCDelayTime::create(.6f),CCCallFunc::create(this, callfunc_selector(SGRewardRegLayer::animation3)),CCDelayTime::create(.6f),CCCallFunc::create(this, callfunc_selector(SGRewardRegLayer::animation2)),CCDelayTime::create(.6f),CCCallFunc::create(this, callfunc_selector(SGRewardRegLayer::animation1)),NULL));
    btn->setEnabled(false);
    
}
void SGRewardRegLayer::animation1()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_regLotteryLayer);
    CCLOG("animation1");
    CCLabelAtlas *a = (CCLabelAtlas*)this->getChildByTag(1);
    a->setString(CCString::createWithFormat("%d",addvalue/10000)->getCString());
    this->removeChildByTag(101, true);
    
    ((CCLabelAtlas *)this->getChildByTag(100))->setString(CCString::createWithFormat("%d",cost)->getCString());
    SGMainManager::shareMain()->updataUserMsg();
    if (chance > 0) {
		btn->setEnabled(true);
    }else
    {
        btn->setVisible(false);
        SGButton *a = SGButton::create("reward_csbtnhuibg.png", "reward_csbtn_font2.png", this, NULL,CCPointZero,false,true);
        this->addChild(a,20);
        a->setPosition(btn->getPosition());
    }
    
    SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_RewardRegLayer_str6,addvalue)->getCString());
	
}
void SGRewardRegLayer::animation2()
{
    CCLabelAtlas *a = (CCLabelAtlas*)this->getChildByTag(2);
    a->setString(CCString::createWithFormat("%d",addvalue%10000/1000)->getCString());
    this->removeChildByTag(102, true);
}
void SGRewardRegLayer::animation3()
{
    CCLabelAtlas *a = (CCLabelAtlas*)this->getChildByTag(3);
    a->setString(CCString::createWithFormat("%d",addvalue%1000/100)->getCString());
    this->removeChildByTag(103, true);
	
}
void SGRewardRegLayer::animation4()
{
    CCLabelAtlas *a = (CCLabelAtlas*)this->getChildByTag(4);
    a->setString(CCString::createWithFormat("%d",addvalue%100/10)->getCString());
    this->removeChildByTag(104, true);
}
void SGRewardRegLayer::animation5()
{
    CCLabelAtlas *a = (CCLabelAtlas*)this->getChildByTag(5);
    a->setString(CCString::createWithFormat("%d",addvalue%10)->getCString());
    this->removeChildByTag(105, true);
}
void SGRewardRegLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}
void SGRewardRegLayer::setIsCanTouch(bool isTouch)
{
    btn->setEnabled(isTouch);
    ((SGButton *)this->menu->getChildByTag(88))->setEnabled(isTouch);
	
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(isTouch);
    layer->setPortraitMenuCanTouch(isTouch);
}
void SGRewardRegLayer::updatePlayers()
{
    CCLOG("updatePlayers");
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardRegPlayers();
    if ((array->count()+1)/2 > scrollHeight)
    {
        for (int i = scrollHeight; i< scrollHeight+(20 - scrollArray->count())/2; i++)
        {
            if (i< (array->count()+1)/2) {
				CCLog("scroll i is %d    arraycount is%d",i,array->count()/2);
				scrollArray->addObject(array->objectAtIndex(i*2));
				scrollArray->addObject(array->objectAtIndex(i*2+1));
				CCString* strname = (CCString*)array->objectAtIndex(i*2);
				CCString* strvalue = (CCString*)array->objectAtIndex(i*2+1);
				SGCCLabelTTF*name = SGCCLabelTTF::create(CCString::createWithFormat(str_RewardRegLayer_str4,strname->getCString())->getCString(), FONT_BOXINFO, 26 , ccWHITE);
                //modify by:zyc. merge into create.
				//name->setColor(ccWHITE);
				scrollLayer->addChild(name,5);
				name->setAnchorPoint(ccp(1, 0.5));
				//            name->setPosition(ccp(skewing(160)+170, 62 -40*(i+1)));
				name->setPosition(ccp(s.width*0.5+name->getContentSize().width*0.5, 62 -40*(i+1)));
				SGCCLabelTTF*addgold = SGCCLabelTTF::create(CCString::createWithFormat("%d",strvalue->intValue())->getCString(), FONT_BOXINFO,26 , COLOR_YELLOW);
                //modify by:zyc. merge into create.
				//addgold->setColor(COLOR_YELLOW);
				scrollLayer->addChild(addgold);
				addgold->setPosition(ccpAdd(name->getPosition(), ccp(-83, 0)));
				scrollHeight++;
            }
        }
        if ((scrollHeight - leftcount )>2)
        {
            this->runAction(CCSequence::create(CCDelayTime::create(1.2),CCCallFuncN::create(this, callfuncN_selector(SGRewardRegLayer::resumeAction)),NULL));
        }
    }
}
void SGRewardRegLayer::updatecount()
{
    CCLOG("updatecount");
    leftcount++;
    if (scrollHeight<2) {
        scrollLayer->pauseSchedulerAndActions();
        return;
    }
    if (scrollArray->count()>1) {
        scrollArray->removeObjectAtIndex(0);
        scrollArray->removeObjectAtIndex(1);
    }
    if ((scrollHeight - leftcount) == 2) {
        scrollLayer->pauseSchedulerAndActions();
    }
    
}
void SGRewardRegLayer::resumeAction()
{
    CCLOG("resumeAction");
	scrollLayer->resumeSchedulerAndActions();
}
void SGRewardRegLayer::jinbiAction()
{
    int rangeX = (int)(skewing(310) - 20);
	int actualX = ( rand() % rangeX ) + 20;
    int randact = 4;
    int actualAct = ( rand() % randact );
	
    CCSpriterX* fermEffect = CCSpriterX::create(CCString::createWithFormat("animationFile/jinbi%d.scml",actualAct)->getCString(), true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect,-1);
    fermEffect->setPosition(ccpAdd(title_bg->getPosition(),ccp(actualX - skewing(160),-15)));
    fermEffect->play();
    
    fermEffect->runAction(CCSequence::create(CCMoveTo::create(1, ccp(fermEffect->getPosition().x, 155)),CCCallFuncN::create(this, callfuncN_selector(SGRewardRegLayer::removejinb)),NULL));
    
}
void SGRewardRegLayer::jinbiAction1()
{
    int rangeX = (int)(skewing(310) - 20);
	int actualX = ( rand() % rangeX ) + 20;
    int randact = 4;
    int actualAct = ( rand() % randact );
    
    CCSpriterX* fermEffect = CCSpriterX::create(CCString::createWithFormat("animationFile/jinbi%d.scml",actualAct)->getCString(), true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect,-1);
    fermEffect->setPosition(ccpAdd(title_bg->getPosition(),ccp(actualX - skewing(160),-15)));
    fermEffect->play();
    
    fermEffect->runAction(CCSequence::create(CCMoveTo::create(2, ccp(fermEffect->getPosition().x, 155)),CCCallFuncN::create(this, callfuncN_selector(SGRewardRegLayer::removejinb)),NULL));
}
void SGRewardRegLayer::jinbiAction2()
{
    int rangeX = (int)(skewing(310) - 20);
	int actualX = ( rand() % rangeX ) + 20;
    int randact = 4;
    int actualAct = ( rand() % randact );
    CCSpriterX* fermEffect = CCSpriterX::create(CCString::createWithFormat("animationFile/jinbi%d.scml",actualAct)->getCString(), true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect,-1);
    fermEffect->setPosition(ccpAdd(title_bg->getPosition(),ccp(actualX - skewing(160),-15)));
    fermEffect->play();
    
    fermEffect->runAction(CCSequence::create(CCMoveTo::create(3, ccp(fermEffect->getPosition().x, 155)),CCCallFuncN::create(this, callfuncN_selector(SGRewardRegLayer::removejinb)),NULL));
}
void SGRewardRegLayer::removejinb(cocos2d::CCNode *node)
{
    CCLOG("remove jinb");
    this->removeChild(node, true);
}
void SGRewardRegLayer::stopjinbiAct()
{
    CCLOG("stop all schedule");
    this->unschedule(schedule_selector(SGRewardRegLayer::jinbiAction));
    this->unschedule(schedule_selector(SGRewardRegLayer::jinbiAction1));
    this->unschedule(schedule_selector(SGRewardRegLayer::jinbiAction2));
    CCLOG("stop all schedule");
	//    this->unscheduleAllSelectors();
	
}
