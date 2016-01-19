//
//  SGBossBattleRankListLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-25.
//
//

#include "SGBossBattleRankListLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGRankCell.h"
#include "SGFirstLayer.h"
#include "SGBossRankItem.h"
#include "FightBaseModule.pb.h"
#include "SGFriend.h"
#include "SGRankDetailLayer.h"
#include "SGPlayerInfo.h"
#include "SGStringConfig.h"


SGBossBattleRankListLayer::SGBossBattleRankListLayer()
:enterType(0),//预留
ranklabel(NULL),
shengwang(NULL),
rank(NULL),
ranknum(NULL),
name(NULL),
selectindex(0),
playerplayerid(0),
ranktypetag(0),
countrytypetag(0),
beatBossPlayer(""),//打败boss的玩家名字
beatMinute(0),//所用的分钟数
hurtBlood(0),//伤害血量
isHaveBossKiller(0),//是否有boss的最终击杀者
selfDamage(0)//自己的击杀值
{
	beatMinute = 0;//所用的分钟数
	hurtBlood = 0;//伤害血量
	isHaveBossKiller = true;
	selfDamage = 0;
}
SGBossBattleRankListLayer::~SGBossBattleRankListLayer()
{
    CC_SAFE_RELEASE(datas);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_bossbattleranklayer);
}

SGBossBattleRankListLayer *SGBossBattleRankListLayer::create(CCArray *data, SGKillBossInfo *killInfo, int enterType)
{
	SGBossBattleRankListLayer *rankLayer = new SGBossBattleRankListLayer();
	if (rankLayer && rankLayer->init(NULL, sg_bossbattleranklayer))
	{
		rankLayer->enterType = enterType;
		rankLayer->beatBossPlayer = killInfo->playerName;
		rankLayer->beatMinute = killInfo->mins;
		rankLayer->hurtBlood = killInfo->killDamage;
		rankLayer->selfDamage = killInfo->selfDamage;
		rankLayer->initData(data);
		rankLayer->initView();
		
		rankLayer->autorelease();
		return rankLayer;
	}
	CC_SAFE_RELEASE(rankLayer);
	return NULL;
}

void SGBossBattleRankListLayer::initData(CCArray *data)
{
	datas = CCArray::create();
	datas->retain();
	datas->initWithArray(data);
	
	// 是boss的击杀者,有可能没有,但是服务器会发过来
	SGBossRankData *rankInfo = static_cast<SGBossRankData *>(datas->objectAtIndex(0));
	//移除这个其实不存在的击杀者
	if (rankInfo->roleId == 0)
	{
		datas->removeObjectAtIndex(0);
		isHaveBossKiller = false;
	}
	
}

void SGBossBattleRankListLayer::initView()
{
	
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this,
																  callfuncO_selector(SGBossBattleRankListLayer::checkEmbattle));
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);
	ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayer2.plist",RES_TYPE_LAYER_UI ,sg_bossbattleranklayer);
	ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist",RES_TYPE_LAYER_UI ,sg_bossbattleranklayer);
    
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);
	ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);

	
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,1);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,2);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,2);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,2);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,2);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,2);
	
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGBossBattleRankListLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.1);
	this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title_bg->getPosition().y - backBtn->getContentSize().height / 2 - 5));

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_bossbattleranklayer);

	CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width,s.height - 65 - gonggao_3_2->getContentSize().height - b);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    bg->setTextureRect(r);
	
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b)));
	
	SGCCLabelTTF *title = SGCCLabelTTF::create(str_BossBattleRankListLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title, 2);
	
	
	CCSprite *blackBar = CCSprite::createWithSpriteFrameName("visit_heitiao.png");
    if (!blackBar) {
        return;
    }
	blackBar->setScaleY(2.1);
    this->addChild(blackBar,0);
    blackBar->setPosition(ccpAdd(title_bg_l->getPosition(), ccp(s.width*.5f, -blackBar->getContentSize().height + 10)));

	CCSprite *titlecenter1 = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter1->setAnchorPoint(ccp(0.5, 0));
    titlecenter1->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -1- blackBar->getContentSize().height * 2.1)));
    this->addChild(titlecenter1,2);
    titlecenter1->setScaleX(4);
    
    CCSprite *title_bg_l1 = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l1->setAnchorPoint(ccp(0, 0));
    title_bg_l1->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -1- blackBar->getContentSize().height * 2.1)));
    this->addChild(title_bg_l1,2);
    
    CCSprite *title_bg_r1 = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r1->setFlipX(true);
    title_bg_r1->setAnchorPoint(ccp(1, 0));
    title_bg_r1->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -1 - blackBar->getContentSize().height * 2.1)));
    this->addChild(title_bg_r1,2);
	
	//有可能没有最终击杀的boss
	if (hurtBlood != 0)
	{
		SGCCLabelTTF *bossTips = SGCCLabelTTF::create(CCString::createWithFormat(str_BossBattleRankListLayer_str2, beatMinute,
																				 beatBossPlayer.c_str(), hurtBlood)->getCString(), FONT_PENGYOU, 21, CCSizeMake(s.width - 200, 90));
		this->addChild(bossTips, 2);
		bossTips->setPosition(ccpAdd(title->getPosition(), ccp(0, -title->getContentSize().height * 2.08)));
	}
	else
	{
		SGCCLabelTTF *rankInfo = SGCCLabelTTF::create(str_BossBattleRankListLayer_str3, FONT_PENGYOU, 21, CCSizeMake(s.width - 200, 40));
		this->addChild(rankInfo, 2);
		rankInfo->setPosition(ccpAdd(title->getPosition(), ccp(0, -title->getContentSize().height * 2)));
	}
	
	/////自己的排名信息
    CCSprite*bottomBorder = CCSprite::createWithSpriteFrameName("guide_board_new.png");
    bottomBorder->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-s.width/2, b)));
    bottomBorder->setAnchorPoint(ccp(0, 0));
    bottomBorder->setScaleX(s.width/bottomBorder->getContentSize().width);
    this->addChild(bottomBorder,1,100001);
	
	this->selfRank(selfDamage, selfDamage);
	//tableview各种设置
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - 540));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-s.width/2, b+bottomBorder->getContentSize().height)));
    tableViewHeight = skewing(73);
	
	
}


void SGBossBattleRankListLayer::checkEmbattle(CCObject *sender)
{
    SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    mainlayer->setPortraitMenuCanTouch(false);
    
    SGSocketRequest *request = (SGSocketRequest*)sender;
    
	if (request)
	{
		main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)request->m_msg;
		if (roleInfo->role().roleid())
		{
			SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
            if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank));
            {
                SGMainManager::shareMain()->getMainScene()->removeChildByTag(box_rank);
            }
			SGRankDetailLayer*box=SGRankDetailLayer::create(playerInfo,2);
			SGMainManager::shareMain()->showBox(box);
		}
	}


}

void SGBossBattleRankListLayer::selfRank(int value ,int ranking)
{
	CCSprite*sp=(CCSprite*) getChildByTag(100001);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
	
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    
    CCSprite*icon=CCSprite::createWithSpriteFrameName(CCString::createWithFormat( "countryIcon_%d.png",playerInfo->getPlayerCountry())->getCString());
    this->addChild(icon,2);
    icon->setPosition(ccpAdd(sp->getPosition(), ccp(icon->getContentSize().width*1.1,sp->getContentSize().height*0.5)));
    //spr->setPosition(ccpAdd(sp->getPosition(), ccp(itemHeadPic->getContentSize().width*1.0,-sp->getContentSize().height*0.5)));
    CCSprite*kuang=CCSprite::createWithSpriteFrameName(CCString::createWithFormat( "country_%d.png",playerInfo->getPlayerCountry())->getCString());
    kuang->setPosition(ccpAdd(icon->getPosition(), ccp(-2, -5))   );
    this->addChild(kuang,2);
    
    SGButton *btn = SGButton::createFromLocal("unhot.png", str_BossBattleRankListLayer_str4, this, menu_selector(SGMainLayer::onEmbattleBtnClicked),CCPointZero,FONT_BOXINFO,ccWHITE,32);
//    SGButton *btn = SGButton::createFromLocal("unhot.png", str_BossBattleRankListLayer_str4, this, menu_selector(SGFirstLayer::gotoEmbattleLayer),CCPointZero,FONT_BOXINFO,ccWHITE,32);
    btn->setAnchorPoint(ccp(1, 0.5));
    btn->setPosition(ccp(size.width*0.95, kuang->getPositionY()-kuang->getContentSize().height*0.2));
    btn->setFontColor(ccc3(0xf3,0x89,00));
    addBtn(btn);
    
	name = SGCCLabelTTF::create(playerInfo->getNickName()->getCString(), FONT_BOXINFO, 32);
	name->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width*0.7, kuang->getContentSize().height*0.3)));
	name->setAnchorPoint(ccp(0, 0.5));
	this->addChild(name,20);
	
    rank = SGCCLabelTTF::create(CCString::createWithFormat(str_BossBattleRankListLayer_str5, SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())->getCString(), FONT_BOXINFO, 32 , ccc3(0xff, 0xc0, 0x00));
    rank->setAnchorPoint(ccp(1, 0.5));
    rank->setPosition(ccpAdd(name->getPosition(), ccp(name->getContentSize().width + rank->getContentSize().width * 1.5, rank->getContentSize().height / 2)));
    this->addChild(rank,20);
    //modify by:zyc. merge into create.
    //rank->setColor(ccc3(0xff, 0xc0, 0x00));
    
	CCString *ranktypestring=CCString::create(str_BossBattleRankListLayer_str6);

    
    shengwang = SGCCLabelTTF::create(ranktypestring->getCString(), FONT_BOXINFO, 32);
    shengwang->setPosition(ccpAdd(name->getPosition(), ccp(kuang->getContentSize().width*0.0, -kuang->getContentSize().height*0.5)));
    this->addChild(shengwang,20);
    shengwang->setAnchorPoint(ccp(0, 0.5));
    
    CCString*mmm;
    if(ranking==0)
    {
        mmm= CCString::create(str_BossBattleRankListLayer_str7);
        
    }
    else if (ranking>0)
    {
        mmm=CCString::createWithFormat("%d",selfDamage);
    }
    else
    {
        mmm=CCString::create(str_BossBattleRankListLayer_str7);
    }
	
    ranklabel = SGCCLabelTTF::create(mmm->getCString(), FONT_BOXINFO, 32);
    ranklabel->setAnchorPoint(ccp(0, 0.5));
    ranklabel->setPosition(ccpAdd(shengwang->getPosition(), ccp(shengwang->getContentSize().width+10, 0)));
    this->addChild(ranklabel,20);
}


void SGBossBattleRankListLayer::backHandler()
{
	CCLOG("Back");
 
    if (enterType==0) {
        main::MainBossRequest *request=new main::MainBossRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
    }
 
//	SGMainManager::shareMain()->showBossBattleJoinInLayer();
 
}


SNSTableViewCellItem *SGBossBattleRankListLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	int index = tableView->getItemPointerWithIndexPath(indexPath);//从0开始
	SGBossRankItem *item = (SGBossRankItem *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBossRankListData *rankInfo =  (SGBossRankListData *)datas->objectAtIndex(index);
	
	
	//   playerplayerid=player->getplayerId();
    if (item == NULL) {
        item = SGBossRankItem::create(rankInfo, rankInfo->loc + 1);
    }
	if (index < datas->count())
	{
		item->initData(rankInfo, rankInfo->loc + 1, isHaveBossKiller);
	}
    else
	{
        item->setVisible(false);
	}
    
    return item;
}

unsigned int SGBossBattleRankListLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	return datas->count();
}
void SGBossBattleRankListLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	
	
}