//
//  SGSeasonListLayer.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#include "SGSeasonListLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
//#include "SGMenu.h"
#include "PvpFight.pb.h"
#include "SGBattleData.h"
#include "SGBattleMainLayer.h"
#include "SGPvpGiftInfoBox.h"
#include "SGStringConfig.h"


using namespace cocos2d;

SGSeasonListLayer *SGSeasonListLayer::create(int count,CCArray *ary)
{
    SGSeasonListLayer *barracksLayer = new SGSeasonListLayer();
    if (barracksLayer && barracksLayer->init(NULL, sg_seasonlistlayer))
    {
//        barracksLayer->initMsg();
        barracksLayer->initView(count,ary);
        barracksLayer->autorelease();
        return barracksLayer;
    }
    CC_SAFE_DELETE(barracksLayer);
    return NULL;
}
SGSeasonListLayer::SGSeasonListLayer()
:str_ofr(NULL),
str_eqp(NULL),
str_soul(NULL),
str_prop(NULL),
str_skill(NULL),
str_pieces(NULL)
{
}
SGSeasonListLayer::~SGSeasonListLayer()
{

    CCLOG("~SGSeasonListLayer");
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVPENTER_SEASONWAR);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVP_SEASON_GIFTGET);

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_seasonlistlayer);

}
void SGSeasonListLayer::initMsg()
{
    this->datas = CCArray::create();
    this->datas->retain();
//    for (int i=0;i<9;i++) {
//        SGJunbuff *buff = SGStaticDataManager::shareStatic()->getbuffbyid(i);
//        datas->addObject(buff);
//        
//    }

}


///////////
void SGSeasonListLayer::initView(int count,CCArray* ary)
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_seasonlistlayer, LIM_PNG_AS_PNG);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->addObserver(MSG_PVPENTER_SEASONWAR, this, callfuncO_selector(SGSeasonListLayer::showSeasonMainLayer));
    notification->addObserver(MSG_PVP_SEASON_GIFTGET, this, callfuncO_selector(SGSeasonListLayer::aftergetgiftseason));
    
    float b = SGMainManager::shareMain()->getBottomHeight();
    float headH = SGMainManager::shareMain()->getVisitHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    //float height = skewingY(337.5);
    if (s.height == 1136) {
        CCLOG("1111");        
        //height = skewing(337.5);
    }    
    datas->addObjectsFromArray(ary);
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,2);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,3);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_seasonlistlayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width, s.height - 45 - gonggao_3_2->getContentSize().height - b);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b)));

    SGCCLabelTTF *title = SGCCLabelTTF::create( str_SeasonListLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSeasonListLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	
    this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, title_bg->getPosition().y-title_bg->getContentSize().height/2));


    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - b  - 10-headH));//headH
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-s.width/2, b)));
    tableViewHeight = 180;
    if(s.width == 640)
        tableViewHeight = 150;
    tableView->setDown(-2);

   
    
    EFFECT_PLAY(MUSIC_ITEM);
}

void SGSeasonListLayer::getHandler(SGVipGift* gift)
{
    if (gift->getHaveBuy()==2) {
        SG_SHOW_WINDOW(str_SeasonListLayer_str2);
    }
    else if (gift->getHaveBuy()==3)
    {
        SG_SHOW_WINDOW(str_SeasonListLayer_str3);
    }
    else if (gift->getHaveBuy()==1)
    {
        main::PvPSeasonAwardGiftGetRequest *request = new main::PvPSeasonAwardGiftGetRequest();
        request->set_key(gift->getPvpgifttype());
        SGSocketClient::sharedSocketClient()->send(MSG_PVP_SEASON_GIFTGET, request);
        
    }

}
void SGSeasonListLayer::aftergetgiftseason(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvPSeasonAwardGiftGetResponse *response = (main::PvPSeasonAwardGiftGetResponse *)request->m_msg;
    if (response->state()==1) {
        SG_SHOW_WINDOW(str_Get_succeed);
        SGVipGift * seasongift = (SGVipGift*)datas->objectAtIndex(response->cout()-1);
        seasongift->setHaveBuy(3);
        tableView->reloadData(false);
    }
    else
    {
        SG_SHOW_WINDOW(str_SeasonListLayer_str4);
    }


}
void SGSeasonListLayer::backHandler()
{
//    main::PvpEntryRequest *request = new main::PvpEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
    SGMainManager::shareMain()->showPvpMatchLayer();
}
void SGSeasonListLayer::showSeasonMainLayer(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvpEntryResponse *response = (main::PvpEntryResponse *)request->m_msg;
    SGBattleData *battleData = NULL;
    SGRankPlayer *selfPlayer = NULL;
    if (response) {
        
        float var1 = response->winratio();//当前胜率
        int var2 = response->prestige();//天梯声望
        int var3 = response->continuouswin();//连胜
        int var4 = response->maxcontinuouswin();//最大连胜
        int var5 = response->awardscore();//天梯积分
        int var6 = response->prestigerank();//声望排名
        int var7 = response->pvpcount();//第几界
        int var8 = response->pvprank();//天梯排名
        int var9 = response->pvpscore();//天梯等级
        int var10= response->pvpfightcount();//参战次数
        
        bool hasDayReward = response->hasdayreward();//每日奖励是否可领取
        bool hasSeasonReward = response->hasseasonreward();//赛季奖励是否可领取
        
        std::string str = response->pvprankname();//当前军衔
        std::string str2 = response->pvpenddate();//截止时间
        
        CCString *name = SGPlayerInfo::sharePlayerInfo()->getNickName();
        int iconId = SGPlayerInfo::sharePlayerInfo()->getPlayerImageID();
        std::string servername = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
        std::string serverid = CCUserDefault::sharedUserDefault()->getStringForKey("serverid");
        CCString *str3 = CCString::createWithFormat("%s",serverid.c_str());
        // create(const char *name, int hid, int rank, int levl, int ftime, float rwin)
        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var8,var9,var10, var1,str3->intValue(),servername.c_str());
        battleData = SGBattleData::create(str2.c_str(), var4, var3, var5, var2,var1,var6,str.c_str(),var7 , hasDayReward , hasSeasonReward);
        
        
        //        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, 0, 0, var4, winr);
        battleData->setStartTime(CCString::create(response->specstarttime()));
        battleData->setEndTime(CCString::create(response->specendtime()));
        SGPlayerInfo::sharePlayerInfo()->setPvpRank(selfPlayer);
        if (selfPlayer) {
            SGPlayerInfo::sharePlayerInfo()->setplayerPvpLevel(selfPlayer->getLevel());
        }
        SGPlayerInfo::sharePlayerInfo()->setplayerPvpNum(response->awardscore());
    }
    if (battleData != NULL) {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
        if (!layer) {
            layer = SGBattleMainLayer::create(battleData);
        }
        mainScene->setVisible(true);
        mainScene->addShowLayer(layer);
    }
}


SNSTableViewCellItem* SGSeasonListLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	//CCSize size = tableView->getItemSize();

	SeasonItem *item = (SeasonItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

    if (NULL == item)
    {
		item = SeasonItem::create(this,num);
        item->updateitem((SGVipGift* )datas->objectAtIndex(num));
    }
    else
        item->updateitem((SGVipGift* )datas->objectAtIndex(num));
//    switch (num) {
//        case 0:            
//            break;
//        case 1:
//            item->label->setString(str_ofr->getCString());            
//            break;
//        case 2:            
//            item->label->setString(str_eqp->getCString());
//            break;
//		case 3:
//            item->label->setString(str_pieces->getCString());//碎片
//			break;
//        case 4:
//            item->label->setString(str_soul->getCString());
//            break;
////        case 5:
////            //item->label->setString(str_prop->getCString());
////            break;
//        case 5:
//            item->label->setString(str_skill->getCString());
//            break;
//        case 6:
//            break;
//        default:
//     
//            break;
//    }
	return item;
}

unsigned int SGSeasonListLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
   
	return datas->count();
}

///////
void SGSeasonListLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    
    
}
///////
void SGSeasonListLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
}

void SGSeasonListLayer::setIsCanTouch(bool isTouch)
{
    SGBaseTableLayer::setIsCanTouch(isTouch);
    tableView->setTouchEnabled(isTouch);
//    tableView->menu1
}
bool SeasonItem::initWithSize()
{
	
    
    if (!SNSTableViewCellItem::init()) {
		return false;
	}
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_seasonlistlayer);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getVisitHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();

     bgSize = CCSizeMake(600, 125);
//    bgSize=CCSizeMake(skewing(300), skewing(65));
    float viewWid = bgSize.width;
    float viewH = bgSize.height;
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(viewWid, viewH));
    this->addChild(frame2);
//    frame2->setPosition(spriteBg->getPosition());
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg, -1);
    frame2bg->setScaleX(viewWid/frame2bg->getContentSize().width);
    frame2bg->setScaleY(viewH/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
    const char* fontName = FONT_BOXINFO;
    const int  fontsize = 26;
    switch(this->giftType)
    
    {
        case 0:
            gift = CCSprite::createWithSpriteFrameName("gift_guanjun.png");
            break;
        case 1:
            gift = CCSprite::createWithSpriteFrameName("gift_yajun.png");
            break;
        case 2:
            gift = CCSprite::createWithSpriteFrameName("gift_jijun.png");
            break;
        default:
            gift = CCSprite::createWithSpriteFrameName("gift_4-10.png");
            break;
            
    }
    gift->setPosition(ccp(-bgSize.width/2+gift->getContentSize().width/2+35, 0));
    this->addChild(gift);
    kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");
    kuang->setPosition(gift->getPosition());
    this->addChild(kuang);
    ylq = CCSprite::createWithSpriteFrameName("pvpyilingqu.png");
    ylq->setPosition(ccpAdd(gift->getPosition(),ccp(-gift->getContentSize().width*.4,gift->getContentSize().height*.4)));
    this->addChild(ylq);
    
    giftname = SGCCLabelTTF::create(CCString::createWithFormat(str_SeasonListLayer_str5,1)->getCString(), fontName, fontsize+4 , ccYELLOW);
    giftname->setAnchorPoint(ccp(0,0.5));
    giftname->setPosition(ccp(-150,25));
    this->addChild(giftname);
    
    number = SGCCLabelTTF::create(CCString::createWithFormat(str_SeasonListLayer_str6)->getCString(), fontName, fontsize-2,CCSizeMake(fontsize*11, fontsize*3),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    number->setAnchorPoint(ccp(0,0.5));
    number->setPosition(ccpAdd(giftname->getPosition(),ccp(0,-fontsize*2)));
    this->addChild(number);
    
    condition = SGCCLabelTTF::create("" , fontName , fontsize -2);
    condition->setAnchorPoint(ccp(0, 0.5));
    condition->setPosition(ccpAdd(number->getPosition(),ccp(0 , -3)));
    this->addChild(condition);
    
    btn = SGButton::createFromLocal("store_exchangebtnbg.png", str_SeasonListLayer_str7, this, menu_selector(SeasonItem::btnclick),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    btn->setPosition(ccpAdd(ccp(200,0),ccp(0,8)));
    btn->setOutsideColor(ccWHITE);

    
    this->addChild(btn);
    this->addBtn(btn);
    
    return true;
}

void SeasonItem::btnclick(SGButton *btn)
{
    
    SGVipGift* gift = (SGVipGift*)(btn->getUserObject());
    SGPvpGiftInfoBox *GiftInfoBox = SGPvpGiftInfoBox::create((SGSeasonListLayer*)delegate, gift,2);
    SGMainManager::shareMain()->showBox(GiftInfoBox);

}

SeasonItem* SeasonItem::create(SeasonListItemDelegate *del,int type)
{
	SeasonItem *instance = new SeasonItem();
    
	if (instance && instance->initWithSize()) {
        instance->delegate = del;
        instance->giftType = type;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}

void SeasonItem::updateitem(SGVipGift *gifts)
{
    if (gifts->getHaveBuy()==1) {//能领
        
        btn->thisinit("reward_lingq.png", "", this, menu_selector(SeasonItem::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);
        btn->setUserObject(gifts);
        gift->setColor(ccWHITE);
        kuang->setColor(ccWHITE);
        ylq->setVisible(false);
        
    }
    else if (gifts->getHaveBuy()==2) {//不能领
        btn->thisinit("store_exchangebtnbg.png", str_SeasonListLayer_str7, this, menu_selector(SeasonItem::btnclick),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
        btn->setOutsideColor(ccWHITE);
        gift->setColor(ccWHITE);
        kuang->setColor(ccWHITE);
        ylq->setVisible(false);
        btn->setUserObject(gifts);
        
    }
    else if (gifts->getHaveBuy()==3) {//领过了
        btn->thisinit("reward_lingq_hui.png", "", this, menu_selector(SeasonItem::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);
        btn->setUserObject(gifts);
        gift->setColor(ccGRAY);
        kuang->setColor(ccGRAY);
        ylq->setVisible(true);
        
    }
    giftname->setString(CCString::createWithFormat("%s",gifts->getGiftName()->getCString())->getCString());
    number->setString(CCString::createWithFormat("%s",gifts->getGiftInfo()->getCString())->getCString());
    condition->setString(gifts->getGiftCondition()->getCString() );
   
}


