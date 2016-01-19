//
//  SGJunxianLayer.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#include "SGJunxianLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "PvpFight.pb.h"
#include "SGBattleData.h"
#include "SGBattleMainLayer.h"
#include "SGStringConfig.h"

using namespace cocos2d;

SGJunxianLayer *SGJunxianLayer::create()
{
    SGJunxianLayer *barracksLayer = new SGJunxianLayer();
    if (barracksLayer && barracksLayer->init(NULL, sg_jitianLayer))
    {
//        barracksLayer->initMsg();
        barracksLayer->initView();
        barracksLayer->autorelease();
        return barracksLayer;
    }
    CC_SAFE_DELETE(barracksLayer);
    return NULL;
}
SGJunxianLayer::SGJunxianLayer()
{
}
SGJunxianLayer::~SGJunxianLayer()
{

    CCLOG("~SGJunxianLayer");	

}
void SGJunxianLayer::initMsg()
{
    this->datas = CCArray::create();
    this->datas->retain();
    int x = SGStaticDataManager::shareStatic()->getJunxianSize();
    for (int i=0;i<x;i++) {
        SGJunbuff *buff = SGStaticDataManager::shareStatic()->getbuffbyid(i);
        datas->addObject(buff);
        
    }

}


///////////
void SGJunxianLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_jitianLayer, LIM_PNG_AS_PNG);
    
//    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->addObserver(MSG_PVPENTER_SEASONWAR, this, callfuncO_selector(SGJunxianLayer::showSeasonMainLayer));
    
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
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_junxianlayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, b,bg->getContentSize().width, s.height - 45 - gonggao_3_2->getContentSize().height - b);
    //bg->setScaleY(s.height / bg->getContentSize().height);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b)));

//    CCSprite *title = CCSprite::createWithSpriteFrameName("barrack_title.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_JunxianLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGJunxianLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
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
//    m_tableView->setTag(modelTag);
//    tableView->setPageNEnable(true);///1126效果优化
//    tableView->setPageVertical(true);
//    this->addChild(tableView);
   
    
    EFFECT_PLAY(MUSIC_ITEM);
}

void SGJunxianLayer::backHandler()
{
//    main::PvpEntryRequest *request = new main::PvpEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
    
    SGMainManager::shareMain()->showPvpMatchLayer();
}

void SGJunxianLayer::showSeasonMainLayer(CCObject *sender)
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


SNSTableViewCellItem* SGJunxianLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();

	JunxianItem *item = (JunxianItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

    if (NULL == item)
    {
		item = JunxianItem::create();
        item->updateitem((SGJunbuff* )datas->objectAtIndex(num));
    }
    else
        item->updateitem((SGJunbuff* )datas->objectAtIndex(num));
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

unsigned int SGJunxianLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
   
	return datas->count();
}

///////
void SGJunxianLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
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
void SGJunxianLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
}

//unsigned int SGJunxianLayer::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
//{
//	return 1;
//}
//void SGJunxianLayer::teamformationHandler()
//{   
//    SGMainManager::shareMain()->showEmbattleLayer(0);
//}
//void SGJunxianLayer::generalHandler()
//{
//    SGMainManager::shareMain()->showGeneralsLayer(0);
//}
//void SGJunxianLayer::soldierHandler()
//{
//    SGMainManager::shareMain()->showSoldierslayer();
//}
//void SGJunxianLayer::equipmentHandler()
//{
//    SGMainManager::shareMain()->showEquipsLayer(NULL,0,0);
//}
////碎片/////////////////////////////////////////
//void SGJunxianLayer::piecesHandler()
//{
//    CCLOG("Pieces");
//	SGMainManager::shareMain()->showPiecesLayer();
//}
//void SGJunxianLayer::propHandler()
//{
//    SGMainManager::shareMain()->showPropsLayer();
//}
//void SGJunxianLayer::skillHandler()
//{
//    SGMainManager::shareMain()->showSkillCardsLayer(0);
//}
//void SGJunxianLayer::expandHandler()
//{
//    SGMainManager::shareMain()->showExpandLayer();
//}
void SGJunxianLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
    tableView->setTouchEnabled(isTouch);
}
bool JunxianItem::initWithSize()
{
	
    
    if (!SNSTableViewCellItem::init()) {
		return false;
	}
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_junxianlayer);
    

    
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
    condition = CCLabelTTF::create("", fontName, fontsize);
    condition->setAnchorPoint(ccp(0,0.5));
   
    buff = CCLabelTTF::create(str_JunxianLayer_str2, fontName, fontsize,CCSizeMake(fontsize*11,fontsize*3),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    buff->setAnchorPoint(ccp(0,1));
    
    CCLabelTTF* hdtj = CCLabelTTF::create(str_JunxianLayer_str3, fontName, fontsize);
    hdtj->setPosition(ccp(-100,25));
    this->addChild(hdtj);
    
    condition->setPosition(ccpAdd(hdtj->getPosition(),ccp(hdtj->getContentSize().width/2,0)));
    this->addChild(condition);
    
    CCLabelTTF* jxjc = CCLabelTTF::create(str_JunxianLayer_str4, fontName, fontsize);
    jxjc->setPosition(ccpAdd(hdtj->getPosition(),ccp(0,-32)));
    jxjc->setColor(ccYELLOW);
    this->addChild(jxjc);
    
//    buff->setPosition(ccp(0,0));
//    buff->setString("sgadfsdafssfsafsafdfsafseagfdsfdsf");
    buff->setPosition(ccpAdd(jxjc->getPosition(),ccp(jxjc->getContentSize().width/2,jxjc->getContentSize().height/2)));
    buff->setColor(ccYELLOW);
    this->addChild(buff);
    
    


    
    //////////
	return true;
}
JunxianItem* JunxianItem::create()
{
	JunxianItem *instance = new JunxianItem();
	if (instance && instance->initWithSize()) {
//        instance->tableview=table;
//        instance->delegate=del;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}

void JunxianItem::updateitem(SGJunbuff *jun)
{
//    CCSprite *icon = NULL;
    if (icon) {
        this->removeChild(icon);
    }
    switch (jun->getBuffid()) {
        case 9:
        {
            icon = CCSprite::createWithSpriteFrameName("buff4.png");
            
        }
            break;
        case 8:
        case 7:
        {
            icon = CCSprite::createWithSpriteFrameName("buff3.png");
           
        }
            break;
        case 6:
        case 5:
        case 4:
        case 3:
        case 2:
        {
            icon = CCSprite::createWithSpriteFrameName("buff2.png");
           
        }
            break;
        case 1:
        {
            icon = CCSprite::createWithSpriteFrameName("buff1.png");
        }
            break;
        default:
            break;
    }
    icon->setPosition(ccp(-bgSize.width/2+icon->getContentSize().width/2+35, 0));
    this->addChild(icon);
    if (name) {
        this->removeChild(name);
    }
    name = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("ph%d.png",jun->getBuffid()))->getCString());
    name->setPosition(ccpAdd(icon->getPosition(),ccp(0,-40)));
    this->addChild(name);
    
    condition->setString(jun->getJunCondition()->getCString());
    buff->setString(jun->getJunbuff()->getCString());
 
   
}
JunxianItem::JunxianItem()
:label(NULL),
tableview(NULL),
bgSize(CCSizeMake(0, 0)),
spriteBg(NULL),
icon(NULL),
name(NULL),
condition(NULL),
buff(NULL)
{
    
}
/////////////////
//
//void SeasonListItem::buttonClick(CCObject* obj)
//{
//    SNSIndexPath *indexpath = this->getIndexPath();
//    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
//    delegate->itemselect(tableview,ind);
//
//}


