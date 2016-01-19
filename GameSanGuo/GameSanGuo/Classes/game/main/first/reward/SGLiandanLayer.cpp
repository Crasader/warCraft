//
//  SGLiandanLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#include "SGLiandanLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCantAdvanceBox.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "SGStaticDataManager.h"
#include "cocos-ext.h"
#include "SGRewardLdBox.h"
#include "ResourceManager.h"
#include "SGRewardLayer.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"
#include "SGLottery9ViewItem.h"
#define  BTN_TAG 100

USING_NS_CC_EXT;
SGLiandanLayer::SGLiandanLayer():haveDanLable(NULL),
time(NULL),
nowTime(0),
select(0),
btn(NULL)
{
}
SGLiandanLayer::~SGLiandanLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_LIANDAN);
     
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_liandanLayer);
    
}
SGLiandanLayer *SGLiandanLayer::create(int time,int index)
{
    SGLiandanLayer *Layer = new SGLiandanLayer();
    if (Layer && Layer->init(NULL , sg_liandanLayer))
    {
        Layer->select = index;
        Layer->nowTime = time;
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGLiandanLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_liandanLayer);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_liandanLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_LIANDAN,
                              this,
                              callfuncO_selector(SGLiandanLayer::liandanListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/reward_liandanbg.plist", RES_TYPE_LAYER_UI, sg_liandanLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("reward_liandanbg.png");
    bg->setScaleX(s.width/768);
    bg->setScaleY((s.height - headhgt - btmhgt)/702);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));
    
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGLiandanLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGLiandanLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_CantAdvance_tips60, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
    //title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    
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
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65/2)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));

    
    CCSprite *baidi = CCSprite::createWithSpriteFrameName("store_baidi.png");
    baidi->setAnchorPoint(ccp(0, 0.5));
    baidi->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -headhgt- baidi->getContentSize().height/2 -1)));
   // this->addChild(baidi);


   
    CCSprite *hd = CCSprite::createWithSpriteFrameName("hd.png");
    this->addChild(hd,-1);
//    hd->setScaleY();
    hd->setPosition(ccp(s.width/2,190));

    CCLabelTTF *label1 = CCLabelTTF::create(str_LiandanLayer_str1, FONT_BOXINFO, FONTSIZE(11));
    this->addChild(label1);
    label1->setAnchorPoint(ccp(0, 0.5));
    label1->setPosition(ccpAdd(ccp(0, 0), ccp(50, 185)));
	
	haveDanLable = CCLabelTTF::create( CCString::createWithFormat(str_LiandanLayer_str2,
											SGPlayerInfo::sharePlayerInfo()->getPlayerliandan())->getCString(), FONT_PANGWA, FONTSIZE(11));
    this->addChild(haveDanLable,1,10);
    haveDanLable->setAnchorPoint(ccp(0, 0.5));
    haveDanLable->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width + haveDanLable->getContentSize().width / 6, 5)));
/////////////
   if( SGPlayerInfo::sharePlayerInfo()->getPlayerliandan())
   {
       btn = SGButton::create("reward_btn_bigred.png", "reward_font_lqgjjyd.png",
							  this, menu_selector(SGLiandanLayer::confirmHandler),CCPointZero,false,true);
  
   }
   else
   {
	   btn = SGButton::create("reward_btn_huibg.png", "reward_font_lqgjjyd.png",
							  this, menu_selector(SGLiandanLayer::confirmHandler),CCPointZero,false,true);
   }
    this->addBtn(btn);
    btn->setPosition(ccp(s.width*.7,270));

	
    SGButton *btn1 = SGButton::create("reward_btn_redbg.png", "reward_font_lqjyd.png",
									  this, menu_selector(SGLiandanLayer::okHandler),CCPointZero,false,true);
    this->addBtn(btn1);
    btn1->setPosition(ccp(s.width*.3, 270));

    int currTime = nowTime;
    int secs = currTime % 60;
    currTime /= 60;
    int minutes = currTime % 60;
    int hours = currTime / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, "%02d:%02d:%02d", hours, minutes, secs);
    time = SGCCLabelTTF::create(timeStr, FONT_PANGWA, FONTSIZE(15) , ccRED);
    time->setAnchorPoint(ccp(1, 0.5));
    this->addChild(time,10);
    //modify by:zyc. merge into create.
    //time->setColor(ccRED);
    time->setPosition(ccpAdd(baidi->getPosition(), ccp(CCDirector::sharedDirector()->getWinSize().width - 30, -20)));
	
	if (nowTime <= 0)
	{
		time->setString(str_LiandanLayer_str3);
		time->setInsideColor(ccGREEN);
	}
	
    this->schedule(schedule_selector(SGLiandanLayer::updateTime), 1.0);
    
}
void SGLiandanLayer::updateTime()
{
    int m = nowTime;
    int secs = m % 60;
    m /= 60;
    int minutes = m % 60;
    int hours = m / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, "%02d:%02d:%02d", hours, minutes, secs);
    
    if (nowTime <= 0)
    {
        time->setString(str_LiandanLayer_str3);
		time->setInsideColor(ccGREEN);
        this->unschedule(schedule_selector(SGLiandanLayer::updateTime));
        return;
    }
    nowTime--;
    time->setString(timeStr);
}


void SGLiandanLayer::okHandler()
{
    if (nowTime != 0) {
        EFFECT_PLAY(MUSIC_BTN);
        SGMainManager::shareMain()->showMessage(str_LiandanLayer_str4);
    }else
    {
        main::RewardLianDanRequest *request = new main::RewardLianDanRequest();
        request->set_index(select);
        request->set_ishigh(0);
        SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LIANDAN, request);
    }
    
}
void SGLiandanLayer::confirmHandler()
{  ////逻辑变化1224
//    EFFECT_PLAY(MUSIC_BTN);
//    if (SGPlayerInfo::sharePlayerInfo()->getPlayerliandan() == 0) {
//        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 17, NULL);
//        SGMainManager::shareMain()->showBox(cantadvanceBox);
//    }else
//    {    
//    main::RewardLianDanRequest *request = new main::RewardLianDanRequest();
//    request->set_index(select);
//    request->set_ishigh(1);
//    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LIANDAN, request);
//    }
   
    
    if (nowTime != 0) {
        EFFECT_PLAY(MUSIC_BTN);
        SGMainManager::shareMain()->showMessage(str_LiandanLayer_str4);
    }
    else
    {
        if (SGPlayerInfo::sharePlayerInfo()->getPlayerliandan() == 0)
        {
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 17, NULL);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }
        else
        {
          main::RewardLianDanRequest *request = new main::RewardLianDanRequest();
          request->set_index(select);
          request->set_ishigh(1);
          SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LIANDAN, request);
        }
            

    
    
    
    }
}
void SGLiandanLayer::liandanListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        //CCString *str = (CCString *)SGPlayerInfo::sharePlayerInfo()->getrewardNotice()->objectAtIndex(select + Reward_OtherLianDan);
        //str->setString(CCString::createWithFormat("%d",0)->getCString());
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
        int nsum = array->count();
        for (int i = 0 ;i < nsum ;i++) {
            SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
            if(item->gettypeItem()==Reward_OtherLianDan)
            {
                item->setstateItem(0);
                break;
            }
        }

        main::RewardLianDanResponse *response = (main::RewardLianDanResponse *)sr->m_msg;
      if (response->state() == 1)
       {
           SGPropsCard *propCard = new SGPropsCard();
           main::PropsCardItemProto cardInfo = response->propscarditem();
           SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
           
           propCard->setSsid(cardInfo.ssid());
           propCard->setItemId(cardInfo.itemid());
//           propCard->setIsProt(cardInfo.isprotection());
           
           propCard->setSell(prop->getpropsSell());
           propCard->setValue(prop->getpropsValue());
           propCard->setRace(prop->getPropsCountry());
           propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
           propCard->setType(prop->getPropsPropsType());
           propCard->setItemType(prop->getPropsType());
           propCard->setCurrStar(prop->getPropsCurrStarLevel());
		   propCard->setHeadSuffixNum(prop->getIconId());//12.05
		   
           //SGPlayerInfo::sharePlayerInfo()->addPropCard(propCard);
           propCard->autorelease();
           
           CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
           int nsum = array->count();
           for (int i = 0 ;i < nsum ;i++) {
               SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
               if(item->gettypeItem()==Reward_OtherLianDan)
               {
                   array->removeObjectAtIndex(i);
                   break;
               }
           }
           //int index = select + Reward_OtherLianDan ;
     
           //array->removeObjectAtIndex(index);
           
           EFFECT_PLAY(MUSIC_34);
           
           SGMailSystemAccessoryDataModel *datamodel= new SGMailSystemAccessoryDataModel() ;
           datamodel->setType(0);
           datamodel->setCount(5);

           CCArray *data=CCArray::create();
           data->retain();
           data->addObject(propCard);
          // SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data,1,113);
          // SGMainManager::shareMain()->showBox(rewardBox);
           SGRewardLdBox *box = SGRewardLdBox::create(this, propCard,1);
           SGMainManager::shareMain()->showBox(box);
           
           SGPlayerInfo::sharePlayerInfo()->setPlayerliandan(response->liandan());
           refreshDanCount();
           
       }
    }
}
void SGLiandanLayer::refreshDanCount()
{
    if (haveDanLable)
    {
        haveDanLable->setString( CCString::createWithFormat(str_LiandanLayer_str2,
                                                            SGPlayerInfo::sharePlayerInfo()->getPlayerliandan())->getCString());
    }
}

void SGLiandanLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}