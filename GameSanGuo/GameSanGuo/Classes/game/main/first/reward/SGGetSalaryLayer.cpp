//
//  SGGetSalaryLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#include "SGGetSalaryLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "cocos-ext.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "ResourceManager.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"
#include "SGLottery9ViewItem.h"
#include "SGRewardLayer.h"
USING_NS_CC_EXT;
SGGetSalaryLayer::SGGetSalaryLayer()
:m_type(3),
lab_iscanget(NULL),
lab_iscanget1(NULL),
btn(NULL),
btn1(NULL),
nowTime(0),
nowTime1(0),
isShow(false),//显示上面领薪
isShowPvP(false)//显示下面的领薪信息
{
}
SGGetSalaryLayer::~SGGetSalaryLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_getsalaryLayer);
   
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_SALARY);
    notification->removeObserver(this, MSG_REWARD_SALARYENTER);
 
  
}
SGGetSalaryLayer *SGGetSalaryLayer::create(int a,int b,int c,int d,int e,int f,int g)
{
    SGGetSalaryLayer *Layer = new SGGetSalaryLayer();
    if (Layer && Layer->init(NULL , sg_getsalaryLayer))
    {
        Layer->initView(a,b,c,d,e,f,g);
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGGetSalaryLayer::initView(int a,int b,int c,int d,int e,int f,int g )
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_getsalaryLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_getsalaryLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_getsalaryLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_getsalaryLayer);
    
    
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_SALARY,
                              this,
                              callfuncO_selector(SGGetSalaryLayer::getCoinListener));
    notification->addObserver(MSG_REWARD_SALARYENTER,
                              this,
                              callfuncO_selector(SGGetSalaryLayer::salaryEnterListener));

    nowTime1 = e;
    nowTime = b;
    int currTime = b;
    int secs = currTime % 60;
    currTime /= 60;
    int minutes = currTime % 60;
    int hours = currTime / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, str_GetSalaryLayer_str1, hours, minutes, secs);
    
    int mm = nowTime1;
    int secs1 = mm % 60;
    mm /= 60;
    int minutes1 = mm % 60;
    mm /= 60;
    int hours1 = mm%24;
    int days = mm/24;
    char timeStr1[32];
    memset(timeStr1, 0, 32);
    snprintf(timeStr1, 32, str_GetSalaryLayer_str2, days,hours1, minutes1, secs1);

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    //float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    SGPlayerInfo *playerinfo = SGPlayerInfo::sharePlayerInfo();
    if(playerinfo->getPlayerLevel()>=f)
    {
        isShow = true;
    }else{
        isShow = false;
    }
    if(playerinfo->getPlayerLevel()>=g)
    {
        isShowPvP = true;
    }else{
        isShowPvP = false;
    }

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGGetSalaryLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
       
//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_salary.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_GetSalaryLayer_str3, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));;
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

    
    
    CCSprite *kuang1 = CCSprite::createWithSpriteFrameName("barrack_item.png");
    kuang1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt*1.5)));
    kuang1->setScaleY(2.8);
    this->addChild(kuang1);
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    this->addChild(frame1);
    frame1->setPreferredSize(CCSizeMake(kuang1->getContentSize().width, kuang1->getContentSize().height*2.8));
    frame1->setPosition(kuang1->getPosition());
    
    CCSprite *kuang2 = CCSprite::createWithSpriteFrameName("barrack_item1.png");
    
    kuang2->setPosition(ccpAdd(frame1->getPosition(), ccp(0, -frame1->getContentSize().height*.57)));
    kuang2->setScaleY(2.8);
    kuang2->setAnchorPoint(ccp(0.5, 1));
    this->addChild(kuang2);
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    this->addChild(frame2);
    frame2->setAnchorPoint(ccp(0.5, 1));
    frame2->setPreferredSize(CCSizeMake(kuang2->getContentSize().width,kuang2->getContentSize().height*2.8));
    frame2->setPosition(kuang2->getPosition());
    
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str4,
                                                                       playerinfo->getPlayerLevel())->getCString(), FONT_BOXINFO, FONTSIZE(13));
    this->addChild(label1);
    label1->setAnchorPoint(ccp(0, 0.5));
    SGCCLabelTTF *label2 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str5,
                                                                       a)->getCString(), FONT_BOXINFO, FONTSIZE(13));
    this->addChild(label2);
    label2->setAnchorPoint(ccp(0, 0.5));
    
    SGCCLabelTTF *label22 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str501,
                                                                           f)->getCString(), FONT_BOXINFO, FONTSIZE(13));
    this->addChild(label22);
    label22->setAnchorPoint(ccp(0, 0.5));
    

    lab_iscanget = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(13));
    this->addChild(lab_iscanget);
    lab_iscanget->setAnchorPoint(ccp(0, 0.5));
    
    label1->setPosition(ccpAdd(frame1->getPosition(), ccp(-frame1->getContentSize().width*.43, frame1->getContentSize().height*.3)));
    label2->setPosition(ccpAdd(frame1->getPosition(), ccp(-frame1->getContentSize().width*.43, frame1->getContentSize().height*.15)));
    lab_iscanget->setPosition(ccpAdd(frame1->getPosition(), ccp(-frame1->getContentSize().width*.43, 0)));
    label22->setPosition(label2->getPosition());
    SGCCLabelTTF *label4 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str6,
                                                                       c)->getCString(), FONT_BOXINFO, FONTSIZE(13));
    this->addChild(label4);
    label4->setAnchorPoint(ccp(0, 0.5));
    
    SGCCLabelTTF *label5 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str7,
                                                                       d)->getCString(), FONT_BOXINFO, FONTSIZE(13));
    this->addChild(label5);
    label5->setAnchorPoint(ccp(0, 0.5));
    
    lab_iscanget1 = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(13));
    this->addChild(lab_iscanget1);
    lab_iscanget1->setAnchorPoint(ccp(0, 0.5));
    
    lab_iscanget1->setPosition(ccpAdd(frame2->getPosition(), ccp(-frame1->getContentSize().width*.43, -frame2->getContentSize().height/2)));
    label5->setPosition(ccpAdd(lab_iscanget1->getPosition(), ccp(0, frame1->getContentSize().height*.15)));
    label4->setPosition(ccpAdd(lab_iscanget1->getPosition(), ccp(0, frame1->getContentSize().height*.3)));
    
    SGCCLabelTTF *label44 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str10,
                                                                           g)->getCString(), FONT_BOXINFO, FONTSIZE(13));
    SGCCLabelTTF *label55 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str11
                                                                           )->getCString(), FONT_BOXINFO, FONTSIZE(13));
    SGCCLabelTTF *label66 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str12
                                                                           )->getCString(), FONT_BOXINFO, FONTSIZE(13));
    label44->setAnchorPoint(ccp(0, 0.5));
    label55->setAnchorPoint(ccp(0, 0.5));
    label66->setAnchorPoint(ccp(0, 0.5));
    this->addChild(label44);
    this->addChild(label55);
    this->addChild(label66);
    label44->setPosition(label4->getPosition());
    label55->setPosition(label5->getPosition());
    label66->setPosition(lab_iscanget1->getPosition());
    



    
    btn = SGButton::create("store_lank.png", "reward_font_lingqu.png", this, menu_selector(SGGetSalaryLayer::getCoins),CCPointZero,false,true);
    this->addBtn(btn);
    btn->setPosition(ccpAdd(frame1->getPosition(), ccp(frame1->getContentSize().width*.36, -frame1->getContentSize().height*.3)));
    
    if (nowTime>0)
    {
        btn->setImage("store_huik.png");
        btn->setEnabled(false);
        lab_iscanget->setString(timeStr);
    }
    else
    {
        lab_iscanget->setString(str_GetSalaryLayer_str8);
    }
    btn1 = SGButton::create("store_lank.png", "reward_font_lingqu.png", this, menu_selector(SGGetSalaryLayer::getCoins1),CCPointZero,false,true);
    this->addBtn(btn1);
    btn1->setPosition(ccpAdd(frame2->getPosition(), ccp(frame1->getContentSize().width*.36, -frame2->getContentSize().height*.8)));
    
    if (nowTime1>0) {
        if (c>0)
        {
            btn1->setImage("store_huik.png");
            btn1->setEnabled(false);
            lab_iscanget1->setString(timeStr1);
        }
        else
        {
            btn1->setImage("store_huik.png");
            btn1->setEnabled(false);
            lab_iscanget1->setString(str_GetSalaryLayer_str9);
        }     
   
    }
    else
    {
        lab_iscanget1->setString(str_GetSalaryLayer_str8);
    }
    
    
    this->schedule(schedule_selector(SGGetSalaryLayer::updateTime), 1.0);
    if (c>0&&isShowPvP)
    {
        this->schedule(schedule_selector(SGGetSalaryLayer::updateTime1),1.0);
    }
    if(isShow)
    {
        label2->setVisible(true);
         label22->setVisible(false);
        lab_iscanget->setVisible(true);
    }else{
         label2->setVisible(false);
         label22->setVisible(true);
        lab_iscanget->setVisible(false);
        btn->setImage("store_huik.png");
        btn->setEnabled(false);
    }
    if(isShowPvP)
    {
        label4->setVisible(true);
        label5->setVisible(true);
        lab_iscanget1->setVisible(true);
        label44->setVisible(false);
        label55->setVisible(false);
        label66->setVisible(false);
        
    }
    else
    {
        label4->setVisible(false);
        label5->setVisible(false);
        lab_iscanget1->setVisible(false);
        label44->setVisible(true);
        label55->setVisible(true);
        label66->setVisible(true);
        btn1->setImage("store_huik.png");
        btn1->setEnabled(false);

    }

    if (d <= 0)
    {
        label4->setVisible(false);
        label5->setVisible(false);
        label44->setVisible(false);
        label55->setVisible(false);
        label66->setVisible(false);
        btn1->setVisible(false);
        btn1->setEnabled(false);
        lab_iscanget1->setVisible(false);
        
        SGCCLabelTTF *labeltemp = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str14)->getCString(), FONT_BOXINFO, FONTSIZE(13));
        labeltemp->setAnchorPoint(ccp(0, 0.5));
        this->addChild(labeltemp);
        SGCCLabelTTF *labeltemp1 = SGCCLabelTTF::create(CCString::createWithFormat(str_GetSalaryLayer_str15)->getCString(), FONT_BOXINFO, FONTSIZE(13));
        labeltemp1->setAnchorPoint(ccp(0, 0.5));
        this->addChild(labeltemp1);
         labeltemp->setPosition(label4->getPosition());
        labeltemp1->setPosition(label5->getPosition());
    }
}
void SGGetSalaryLayer::updateTime()
{
    int m = nowTime;
    int secs = m % 60;
    m /= 60;
    int minutes = m % 60;
    int hours = m / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, str_GetSalaryLayer_str1, hours, minutes, secs);
    
    if (nowTime == 0&&isShow)
    {
        btn->setImage("store_lank.png");
        CCSprite *font = CCSprite::createWithSpriteFrameName("reward_font_lingqu.png");
        btn->setFontImage(font->displayFrame());
        
        lab_iscanget->setString(str_GetSalaryLayer_str8);
        btn->setEnabled(true);
        return;
    }
    nowTime--;
    lab_iscanget->setString(timeStr);
}

void SGGetSalaryLayer::updateTime1()
{
    int m = nowTime1;
    int secs = m % 60;
    m /= 60;
    int minutes = m % 60;
    m /= 60;
    int hours = m%24;
    int days = m/24;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, str_GetSalaryLayer_str2, days,hours, minutes, secs);
    
    if (nowTime1 == 0&&isShowPvP)
    {
        btn1->setImage("store_lank.png");
        CCSprite *font = CCSprite::createWithSpriteFrameName("reward_font_lingqu.png");
        btn1->setFontImage(font->displayFrame());
        lab_iscanget1->setString(str_GetSalaryLayer_str8);
        if (btn->isVisible()) {
            btn1->setEnabled(true);
        }
        return;
    }
    nowTime1--;
    lab_iscanget1->setString(timeStr);
}

void SGGetSalaryLayer::getCoins()
{
    main::RewardGetSalaryRequest *request = new main::RewardGetSalaryRequest();
    m_type = 0;
    request->set_type(0);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_SALARY, request);
}
void SGGetSalaryLayer::getCoins1()
{    
    main::RewardGetSalaryRequest *request = new main::RewardGetSalaryRequest();
    m_type = 1;
    request->set_type(1);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_SALARY, request);
}
void SGGetSalaryLayer::getCoinListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::RewardLevelUpPurchResponse *response = (main::RewardLevelUpPurchResponse *)sr->m_msg;
        if (response->result() == 1) {
            
            main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_REWARD_SALARYENTER, request);          
        }
    }    

}
void SGGetSalaryLayer::salaryEnterListener(cocos2d::CCObject *obj)
{
	ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_getsalaryLayer);
	
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::RewardSalaryEntryResponse *response = (main::RewardSalaryEntryResponse *)sr->m_msg;
    nowTime = response->levelrewardtime();
    nowTime1 = response->pvprewardtime();
    
    if (nowTime!=0 && nowTime1!= 0 ) {
       // CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(3);
       // str->setString(CCString::createWithFormat("%d",0)->getCString());
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
        int nsum = array->count();
        for (int i = 0 ;i < nsum ;i++) {
            SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
            if(item->gettypeItem()==Reward_Salary)
            {
                item->setstateItem(0);
                break;
            }
        }
    }
    
    if (nowTime!=0 && nowTime1== 0 && response->pvplevel() == 0) {
       // CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(3);
       // str->setString(CCString::createWithFormat("%d",0)->getCString());
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
        int nsum = array->count();
        for (int i = 0 ;i < nsum ;i++) {
            SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
            if(item->gettypeItem()==Reward_Salary)
            {
                item->setstateItem(0);
                break;
            }
        }
    }
    
    if (nowTime1 != 0)
    {
        
        btn1->setImage("store_huik.png");
        CCSprite *font = CCSprite::createWithSpriteFrameName("reward_font_yiling.png");
        btn1->setFontImage(font->displayFrame());
        //modify by:zyc. 服务器已经更改背包数据   客户端无需操作。
        //SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(SGPlayerInfo::sharePlayerInfo()->getPlayerCoins() + response->pvplevelcoin());
        EFFECT_PLAY(MUSIC_34);
//        SGMainManager::shareMain()->showMessage(CCString::createWithFormat("成功领取了%d",response->pvplevelcoin())->getCString());
 
        if ( m_type == 1 && response->pvplevelcoin() > 0 )
        {
            //弹出框显示奖励
            CCArray *data=CCArray::create();
            data->retain();
            SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
            datamodel->setType(1);
            datamodel->setCount(response->pvplevelcoin());
            data->addObject(datamodel);
            
            SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
            SGMainManager::shareMain()->showBox(rewardBox);
        }
        
        btn1->setEnabled(false);
    }
    if (nowTime != 0)
    {
        btn->setImage("store_huik.png");
        CCSprite *font = CCSprite::createWithSpriteFrameName("reward_font_yiling.png");
        btn->setFontImage(font->displayFrame());
        //modify by:zyc. 服务器已经更改背包数据   客户端无需操作。
        //SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(SGPlayerInfo::sharePlayerInfo()->getPlayerCoins() + response->levelcoin());
        EFFECT_PLAY(MUSIC_34);
//        SGMainManager::shareMain()->showMessage(CCString::createWithFormat("成功领取了%d",response->levelcoin())->getCString());
        
        if (m_type == 0 &&response->levelcoin() > 0)
        {
            //弹出框显示奖励
            CCArray *data=CCArray::create();
            data->retain();
            SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
            datamodel->setType(1);
            datamodel->setCount(response->levelcoin());
            data->addObject(datamodel);
            
            SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
            SGMainManager::shareMain()->showBox(rewardBox);
        }
        

        
        btn->setEnabled(false);
    }
    SGMainManager::shareMain()->updataUserMsg();
    
}

void SGGetSalaryLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}