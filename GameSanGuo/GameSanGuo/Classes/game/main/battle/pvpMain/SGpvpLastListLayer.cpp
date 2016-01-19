//
//  SGpvpLastListLayer.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#include "SGpvpLastListLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
//#include "SGMenu.h"
#include "PvpFight.pb.h"
#include "SGBattleData.h"
#include "SGBattleMainLayer.h"
#include "SGFriend.h"
#include "SGRankDetailLayer.h"
#include "SGStringConfig.h"


using namespace cocos2d;

SGpvpLastListLayer *SGpvpLastListLayer::create(int count,CCArray *ary)
{
    SGpvpLastListLayer *barracksLayer = new SGpvpLastListLayer();
    if (barracksLayer && barracksLayer->init(NULL, sg_pvplastlistlayer))
    {
//        barracksLayer->initMsg();
        barracksLayer->datas->addObjectsFromArray(ary);
        barracksLayer->initView(count);
        barracksLayer->autorelease();
        return barracksLayer;
    }
    CC_SAFE_DELETE(barracksLayer);
    return NULL;
}
SGpvpLastListLayer::SGpvpLastListLayer()
:str_ofr(NULL),
str_eqp(NULL),
str_soul(NULL),
str_prop(NULL),
str_skill(NULL),
str_pieces(NULL)
{
}
SGpvpLastListLayer::~SGpvpLastListLayer()
{

    CCLOG("~SGpvpLastListLayer");
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVPENTER_SEASONWAR);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_pvplastlistlayer);

}
void SGpvpLastListLayer::initMsg()
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
void SGpvpLastListLayer::initView(int count)
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_pvplastlistlayer, LIM_PNG_AS_PNG);
    
     SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//     notification->addObserver(MSG_PVPENTER_SEASONWAR, this, callfuncO_selector(SGpvpLastListLayer::showSeasonMainLayer));
     notification->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGpvpLastListLayer::showEmbattleInfoLayer));
    float b = SGMainManager::shareMain()->getBottomHeight();
    float headH = SGMainManager::shareMain()->getVisitHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    //float height = skewingY(337.5);
    if (s.height == 1136) {
        CCLOG("1111");        
        //height = skewing(337.5);
    }    
    
    
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
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_pvplastlistlayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width, s.height - 45 - gonggao_3_2->getContentSize().height - b);
    //bg->setScaleY(r.size.height / bg->getContentSize().height);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b)));

//    CCSprite *title = CCSprite::createWithSpriteFrameName("barrack_title.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(CCString::createWithFormat(str_pvpLastListLayer_str1,count)->getCString(), FONT_XINGKAI, 36 ,COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGpvpLastListLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
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

void SGpvpLastListLayer::showEmbattleInfoLayer(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest*)sender;
    main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)request->m_msg;
    //    if (roleInfo->role().roleid()) {
    //
    int typetype = 0;
    
    ///////////////////////////////////////////////////
    SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank));
    {
        SGMainManager::shareMain()->getMainScene()->removeChildByTag(box_rank);
    }
    SGRankDetailLayer*box=SGRankDetailLayer::create(playerInfo,typetype);
    SGMainManager::shareMain()->showBox(box);
    /////////////////////////存储排行类型  国家  人物id
//    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//    player->setrankType(typetype);
//   
//    player->setplayerID(playerplayerid);

}

void SGpvpLastListLayer::showSeasonMainLayer(CCObject *sender)
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

void SGpvpLastListLayer::backHandler()
{
//    main::PvpEntryRequest *request = new main::PvpEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
    SGMainManager::shareMain()->showPvpMatchLayer();

}

SNSTableViewCellItem* SGpvpLastListLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();

	LastlistItem *item = (LastlistItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

    if (NULL == item)
    {
		item = LastlistItem::create((SGRankPlayer* )datas->objectAtIndex(num));
        item->updateitem((SGRankPlayer* )datas->objectAtIndex(num));
    }
    else
        item->updateitem((SGRankPlayer* )datas->objectAtIndex(num));
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

unsigned int SGpvpLastListLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
   
	return datas->count();
}

///////
void SGpvpLastListLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
//    EFFECT_PLAY(MUSIC_ITEM);
//    SGMainManager::shareMain()->showblack();
//    int index = tableView->getItemPointerWithIndexPath(indexpath);
//    switch (index) {
//        case 0:
//            teamformationHandler();
//            break;
//        case 1:
//            generalHandler();
//            break;
//        case 2:
//            equipmentHandler();
//            break;
//		case 3:
//            piecesHandler();// 碎片
//            break;
//        case 4:
//            soldierHandler();
//            break;
//            //        case 5:
//            //            //propHandler();
//            //            break;
//        case 5:
//            skillHandler();
//            break;
//        case 6:
//            expandHandler();
//            break;
//            
//        default:
//            break;
//    }

    
    
}
///////
void SGpvpLastListLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
}

void SGpvpLastListLayer::setIsCanTouch(bool isTouch)
{
    SGBaseTableLayer::setIsCanTouch(isTouch);
    tableView->setTouchEnabled(isTouch);
}

LastlistItem::LastlistItem()
:countryicon(NULL),
kuang(NULL),
jiangzhang(NULL),
namebg(NULL),
name(NULL),
number(NULL),
servicename(NULL),
bgSize(CCSizeMake(0, 0)),
btn(NULL),
state(0),
menu(NULL),
_player(NULL)
{
    
}


bool LastlistItem::initWithSize()
{
	
    
    if (!SNSTableViewCellItem::init()) {
		return false;
	}
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_pvplastlistlayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_pvplastlistlayer);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_pvplastlistlayer);
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
    
    countryicon = CCSprite::createWithSpriteFrameName("countryIcon_1.png");
    countryicon->setPosition(ccp(-bgSize.width/2+countryicon->getContentSize().width/2+45, 0));
    this->addChild(countryicon);
    kuang = CCSprite::createWithSpriteFrameName("country_1.png");
    kuang->setPosition(ccpAdd(countryicon->getPosition(),ccp(-4,-4)));
    this->addChild(kuang);
   
    jiangzhang = CCSprite::createWithSpriteFrameName("pvpno1.png");
    jiangzhang->setPosition(ccp(-bgSize.width/2+jiangzhang->getContentSize().width*.3, -jiangzhang->getContentSize().height*.3));
    this->addChild(jiangzhang);
    
 
    namebg = CCSprite::createWithSpriteFrameName("name_bg.png");
    namebg->setAnchorPoint(ccp(0,0.5));
    namebg->setPosition(ccp(-125,25));
    this->addChild(namebg);
    
    
    name = CCLabelTTF::create(str_pvpLastListLayer_str2, FONT_PANGWA, fontsize+4);
    name->setAnchorPoint(ccp(0,0.5));
    name->setPosition(ccpAdd(namebg->getPosition(),ccp(10,0)));
    this->addChild(name);
    name->setColor(ccYELLOW);
    
    CCLabelTTF* hdtj = CCLabelTTF::create(str_pvpLastListLayer_str3, fontName, fontsize);
    hdtj->setAnchorPoint(ccp(0,0.5));
    hdtj->setPosition(ccpAdd(name->getPosition(),ccp(0,-55)));
    this->addChild(hdtj);
    
    number = CCLabelTTF::create(CCString::createWithFormat("%d",1)->getCString(), FONT_PANGWA, fontsize+4);
    number->setAnchorPoint(ccp(0,0.5));
    number->setPosition(ccpAdd(hdtj->getPosition(),ccp(hdtj->getContentSize().width+4,0)));
    this->addChild(number);
  
   
    

    btn = SGButton::createFromLocal("store_exchangebtnbg.png", str_pvpLastListLayer_str4, this, menu_selector(LastlistItem::btnclick),ccp(0,3),FONT_PANGWA,ccYELLOW,22,false,true);
    btn->setPosition(ccpAdd(ccp(200,-10),ccp(0,0)));
//    btn->setOutsideColor(ccWHITE);
//    menu1->addChild(btn);
    this->addChild(btn);
    this->addBtn(btn);
    
    servicename = CCLabelTTF::create(CCString::createWithFormat(str_pvpLastListLayer_str5,1,str_pvpLastListLayer_str6)->getCString(), fontName, fontsize);
    servicename->setAnchorPoint(ccp(1,1));
    servicename->setPosition(ccp(bgSize.width/2-30, bgSize.height/2-10));
    this->addChild(servicename);
    

 
    
    


    
    //////////
	return true;
}

void LastlistItem::btnclick(SGButton *btn)
{
    main::FriendInfoRequest *info = new main::FriendInfoRequest();
    info->set_friendid(_player->getgetRoleId());
    info->set_type(1);
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);
}



LastlistItem* LastlistItem::create(SGRankPlayer *player)
{
	LastlistItem *instance = new LastlistItem();
	if (instance && instance->initWithSize()) {
//        instance->_player =player;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}

void LastlistItem::updateitem(SGRankPlayer *player)
{
    _player = player;
    CCString *str=CCString::createWithFormat(str_pvpLastListLayer_str5,player->getServerId(),player->getServerName()->getCString());
    servicename->setString(str->getCString());
    
    name->setString(player->getName()->getCString());
    
    str = CCString::createWithFormat("%d",player->getRank());
    number->setString(str->getCString());
    
    btn->setTag(player->getgetRoleId());
    
    str = CCString::createWithFormat("countryIcon_%d.png",player->getHeadId());
    countryicon->setDisplayFrame((CCSprite::createWithSpriteFrameName(str->getCString()))->displayFrame());
    
    str = CCString::createWithFormat("country_%d.png",player->getHeadId());
    kuang ->setDisplayFrame((CCSprite::createWithSpriteFrameName(str->getCString()))->displayFrame());
    
    switch (player->getRank()) {
        case 1:
        {
            jiangzhang ->setDisplayFrame((CCSprite::createWithSpriteFrameName("pvpno1.png")->displayFrame()));
            jiangzhang->setVisible(true);
        }
            break;
        case 2:
        {
            jiangzhang ->setDisplayFrame((CCSprite::createWithSpriteFrameName("pvpno2.png")->displayFrame()));
            jiangzhang->setVisible(true);
        }
            break;
        case 3:
        {
            jiangzhang ->setDisplayFrame((CCSprite::createWithSpriteFrameName("pvpno3.png")->displayFrame()));
            jiangzhang->setVisible(true);
        }
            break;
            
        default:
            jiangzhang->setVisible(false);
            break;
    }
    
    
   
}



