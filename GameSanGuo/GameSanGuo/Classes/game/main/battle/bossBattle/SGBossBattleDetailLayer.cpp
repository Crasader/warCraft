//
//  SGBossBattleDetailLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-25.
//
//

#include "SGBossBattleDetailLayer.h"
#include "ResourceManager.h"
#include "cocos2d.h"

#include "SGMainManager.h"
#include "FightBaseModule.pb.h"
#include "SGBossBattleLayer.h"
#include "SGStoryRewardItem.h"
#include "SGStaticDataManager.h"

#include "AppMacros.h"
#include "FightBaseModule.pb.h"
#include "SGStringConfig.h"



//四个按钮和菜单TAG
#define MENU_TAG		34
#define BUTTON_1		1001
#define BUTTON_2		1002
#define BUTTON_3		1003
#define BUTTON_4		1004

#define TIME_GRAP		5.0f

SGBossBattleDetailLayer::SGBossBattleDetailLayer()
{
	backTimer = 0;
	pressAnyButton = false;
	atkNum = 0;//被攻击的次数
	hurtBlood = 0;//伤血量
	atkMultiple = 0;//攻击倍数
	bombMultiple = 0;//爆击倍数
	bombPercentage = 0;//爆击率
	goldNum1 = 0;
	goldNum2 = 0;
	goldNum3 = 0;
	goldNum4 = 0;
	isFightBoss = false;
	bossDatas = NULL;
	value1 = 0;
	value2 = 0;
	value3 = 0;
	startFight = NULL;
	exitButton = NULL;
    
    blood = NULL;//血条显示
	atkNumLabel = NULL;//攻击
	bombNumLabel = NULL;//爆击
	bombPerLabel = NULL;//爆击率
	bloodPercent = NULL;//血量百分比
	info4 = NULL;//伤害次数
    info3 = NULL;//伤害量
    enterType = 0;//预留
    headIcon = NULL;//向上飘飞的头像
    m_scrollview = NULL;//底部的排行榜滑动区
	backTimer = 0L;//复活倒计时
    timer = NULL;//倒计时显示
    pressAnyButton = false;//点击了任意四个按钮中的一个
    startFight = NULL;//开始战斗
	exitButton = NULL;
	
    atkNum = 0;//被攻击的次数
	hurtBlood = 0;//伤血量
	atkMultiple = 0;//攻击倍数
	bombMultiple = 0;//爆击倍数
	bombPercentage = 0;//爆击率
	
	//四个按钮下对应的元宝数字
	goldNum1 = 0;
	goldNum2 = 0;
	goldNum3 = 0;
	goldNum4 = 0;
	
	value1 = 0;
	value2 = 0;
	value3 = 0;
	//是否已经打过一次boss战
	isFightBoss = false;
	//
	bossDatas = NULL;
    
    bomb = NULL;
    bombPercent = NULL;
    
    memset(m_bufVipLevelLimit ,  0 , sizeof(int) * 4);
}
SGBossBattleDetailLayer::~SGBossBattleDetailLayer()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BOSSBATTLE_SHOW);
 	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GET_BOSS_BUFF);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FLUSH_BOSS_INFO);
}

SGBossBattleDetailLayer *SGBossBattleDetailLayer::create(int enterType, SGBossBattleData *bossData)
{
	SGBossBattleDetailLayer *bossLayer = new SGBossBattleDetailLayer();
	if (bossLayer && bossLayer->init(NULL, sg_bossbattledetailLayer))
	{
		bossLayer->enterType = enterType;
		bossLayer->bossDatas = bossData;
		bossLayer->initData(bossData);
		bossLayer->initView();
		bossLayer->autorelease();
		return bossLayer;
	}
	CC_SAFE_RELEASE(bossLayer);
	return NULL;
}

void SGBossBattleDetailLayer::initData(SGBossBattleData *data)
{
	backTimer = bossDatas->residueSec;
	atkNum = bossDatas->bossHurtTimes;//被攻击的次数
	hurtBlood = bossDatas->currentHurt;//伤血量
	atkMultiple = bossDatas->value1;//攻击倍数
	bombMultiple = bossDatas->value2;//爆击倍数
	bombPercentage = bossDatas->value3;//爆击率

//	static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(0))->buffNeedMoney;
	//buff所需要的钱数
	if (bossDatas->buffData->data && bossDatas->buffData->count())
	{
		goldNum1 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(0))->buffNeedMoney;
		goldNum2 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(1))->buffNeedMoney;
		goldNum3 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(2))->buffNeedMoney;
		goldNum4 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(3))->buffNeedMoney;
		//对应三个buff的加成值
		value1 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(1))->buffValue;
		value2 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(2))->buffValue;
		value3 = static_cast<SGBossBuffData *>(bossDatas->buffData->objectAtIndex(3))->buffValue;
	}
	//玩家所买的buff
//	value1 = bossDatas->value1;
//	value2 = bossDatas->value2;
//	value3 = bossDatas->value3;
}

//刷新刷新本页面Boss信息
void SGBossBattleDetailLayer::flushBossInfo(CCObject *sender)
{
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::BossCurrStateResponse *bossCur = (main::BossCurrStateResponse *)req->m_msg;
		if (bossCur)
		{
			//收到消息就卸载对应的消息
			this->unschedule(schedule_selector(SGBossBattleDetailLayer::sendFreshBossRankMsg));
			
			SGBossStateData *stateData = new SGBossStateData();
			stateData->autorelease();
			stateData->bossCurHp = bossCur->bosshp();
			stateData->bossMaxHp = bossCur->bossbasehp();
			//stateData->bossHurtTimes = bossCur->bossdamagecount();
			
			stateData->rankData = CCArray::create();
			stateData->rankData->retain();
			
			int rankSize = bossCur->bossrankmodel_size();
			//将新的排行信息填充
			for (int i = 0; i < rankSize; i++)
			{
				SGBossRankData *rank = new SGBossRankData();
				main::BossRankModel rankModel = bossCur->bossrankmodel(i);
				rank->autorelease();
				rank->roleId = rankModel.roleid();
				rank->roleName = rankModel.rolename();
				rank->roleLvl = rankModel.level();
				rank->roleHurt = rankModel.damage();
				stateData->rankData->addObject(rank);
			}
			
			this->refreshBossView(stateData);
//			SGMainManager::shareMain()->refreshBossInfo(stateData);
		}
	}
}




//刷新页面显示
void SGBossBattleDetailLayer::refreshBossView(SGBossStateData *data)
{
	bloodPercent->setString(CCString::createWithFormat("%.2f%%", ((data->bossCurHp * 100.0) / data->bossMaxHp))->getCString());
	//info4->setString(CCString::createWithFormat("%d", data->bossHurtTimes)->getCString());
	//info3->setString(CCString::createWithFormat("%d", data->bossMaxHp - data->bossCurHp)->getCString());
	blood->setScaleX((data->bossCurHp * 1.0) / data->bossMaxHp);
	
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	
	int scrollShift = 0;
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		scrollShift = -15;
	}
	else if (CCEGLView::sharedOpenGLView()->getFrameSize().height == iphonehdResource.size.height)
	{
		scrollShift = -18;
	}
    else
    {
        scrollShift = -10;
    }
	//移除上一个滑动区，再再构造一个新的滑动区
	CCScrollView *last = (CCScrollView *)this->getChildByTag(103);
	last->removeFromParentAndCleanup(true);
	
	CCScrollView *m_scrollview = CCScrollView::create(CCSizeMake(s.width - 10, 150));
	m_scrollview->setViewSize(CCSizeMake(s.width - 10, 150));
    CCLayerColor *container = CCLayerColor::create(ccc4(0, 255, 0, 0));
    container->setContentSize(CCSizeMake(s.width, 150));
    m_scrollview->setContainer(container);
	m_scrollview->setPosition(ccpAdd(ccp(0, 0), ccp(0, 185 + scrollShift)));//设置scrollview位置
	
	m_scrollview->setDirection(kCCScrollViewDirectionVertical);
    addChild(m_scrollview);
	m_scrollview->setTag(103);
	
	//

	int finalLen = 0;
	int rankSize = data->rankData->count();
	for (int i = rankSize - 1; i >= 0; i--)
	{
		SGBossRankData *dataRank = static_cast<SGBossRankData *>(data->rankData->objectAtIndex(i));
		SGRateItem *item = SGRateItem::create(CCString::create(dataRank->roleName),  i + 1, dataRank->roleLvl, dataRank->roleHurt);
		container->addChild(item);
		item->setPosition(ccpAdd(container->getPosition(), ccp(item->getContentSize().width / 2.5,
															   (item->getContentSize().height + 10) * (rankSize - i))));
		finalLen += (item->getContentSize().height + 10);
	}
	m_scrollview->setContentOffset(ccp(0, -finalLen + m_scrollview->getContentSize().height - 10));
	m_scrollview->setContentSize(CCSizeMake(s.width, finalLen));
	m_scrollview->getContainer()->setContentSize(CCSizeMake(s.width, finalLen + 10));
	//刷新完之后再次加入定时器
	this->schedule(schedule_selector(SGBossBattleDetailLayer::sendFreshBossRankMsg), TIME_GRAP);
}
// 刷新buff信息
void SGBossBattleDetailLayer::freshBuff(int v1, float v2, int v3)
{
	atkNumLabel->setString(CCString::createWithFormat("+%d%%", v1)->getCString());
	bombNumLabel->setString(CCString::createWithFormat("+%.1f", v2)->getCString());
	bombPerLabel->setString(CCString::createWithFormat("+%d%%", v3)->getCString());
    
	bomb->setPosition(ccpAdd(atkNumLabel->getPosition(), ccp(atkNumLabel->getContentSize().width/2 +bomb->getContentSize().width/2 - 8, 0)));
	bombPercent->setPosition(ccpAdd(bombNumLabel->getPosition(), ccp(bombNumLabel->getContentSize().width/2 + bombPercent->getContentSize().width/2 - 16, 0)));
    
    
	backTimer = 0;//直接复活
}

void SGBossBattleDetailLayer::initView()
{
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_BOSSBATTLE_SHOW, this,
																  callfuncO_selector(SGBossBattleDetailLayer::startbossFightListener));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_GET_BOSS_BUFF, this,
																  callfuncO_selector(SGBossBattleDetailLayer::getBossBuffAndChange));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FLUSH_BOSS_INFO, this,
																  callfuncO_selector(SGBossBattleDetailLayer::flushBossInfo));
	
	CCMenu *menu = CCMenu::create();
	this->addChild(menu, 2, MENU_TAG);
	menu->setPosition(CCPointZero);
	
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary * dicEle = NULL;
    //4个buf的vip等级限制  在controlvalue.xls中 id~21-24  如果出错 ，请注意这里。
    for (int i =0 ; i < 4 ; i++)
    {
        dicEle = (CCDictionary *)dic->objectForKey(CCString::createWithFormat("%d" , 21 + i)->m_sString );
        m_bufVipLevelLimit[i] = dicEle->valueForKey("value")->intValue();
    }
    //CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("9");
    //m_nSaodangVipLevel = dic1->valueForKey("value")->intValue();

	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_bossbattledetailLayer);
	ResourceManager::sharedInstance()->bindTexture("updatelayer/updatelayer.plist", RES_TYPE_LAYER_UI, sg_bossbattledetailLayer);
    //ResourceManager::sharedInstance()->bindTexture("sanguobigpic/boss_fight_detail.plist", RES_TYPE_LAYER_UI, sg_bossbattledetailLayer);

	CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/boss_fight_detail.plist", RES_TYPE_LAYER_UI, sg_bossbattledetailLayer);
    
	CCSprite *bg = CCSprite::createWithSpriteFrameName("boss_fight_detail.png");
	bg->setScaleY(1.24);
	int bgShift = 0;
	int contentShift = 0;
	int scrollShift = 0;
	int ip4Shift = 0;
	
	//CCLOG("%f==%f", CCEGLView::sharedOpenGLView()->getFrameSize().width, CCEGLView::sharedOpenGLView()->getFrameSize().height);
    CCSize viewSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	if (viewSize.width == ipadResource.size.width || viewSize.width == ipadhdResource.size.width)
	{
		bg->setScaleY(1.09);
		bg->setScaleX(1.2);
		bgShift = 5;
		contentShift = 40;
		scrollShift = -15;
	}
	else if (viewSize.height == iphonehdResource.size.height && viewSize.width!=540)//540 是一个奇葩 540*960.
	{
		bg->setScaleY(1.02);
		bgShift = 5;
		scrollShift = -18;
		contentShift = 52;
		ip4Shift = 10;
	}
	else
	{
        bg->setScaleY(1.26);
        bgShift = 5;
		scrollShift = -10;
		contentShift = -8;
	}
	this->addChild(bg, 0);
	bg->setPosition(ccpAdd(ccp(s.width / 2, s.height / 2), ccp(0, 55 + bgShift)));
	
	//CCLabelTTF *blood = CCLabelTTF::create("----------Boss血条----------", FONT_BOXINFO, 32);
	CCSprite *bloodBg = CCSprite::createWithSpriteFrameName("loadingbg.png");
	blood = CCSprite::createWithSpriteFrameName("blood_bar.png");
	blood->setScaleX((bossDatas->bossCurrHp * 1.0) / bossDatas->bossMaxHp);
	blood->setAnchorPoint(ccp(0, 0.5));
	this->addChild(bloodBg, 2);
	this->addChild(blood, 3);
	bloodBg->setPosition(ccpAdd(ccp(s.width / 2, s.height / 2), ccp(10, s.height / 2 - 100)));
	blood->setPosition(ccpAdd(bloodBg->getPosition(), ccp(-blood->getContentSize().width / 2, 0)));
	
//	float per = ((bossDatas->bossCurrHp * 1.0) / bossDatas->bossMaxHp);
	bloodPercent = SGCCLabelTTF::create(CCString::createWithFormat("%.2f%%", ((bossDatas->bossCurrHp * 100.0) / bossDatas->bossMaxHp))->getCString(), FONT_PENGYOU, 20);
    this->addChild(bloodPercent, 3);
	bloodPercent->setPosition(ccpAdd(bloodBg->getPosition(), ccp(0, 2)));
	
	//通知按钮
	SGButton *notice = SGButton::create("publc_notice.png", NULL, this, menu_selector(SGBossBattleDetailLayer::bossBattleReadMe), CCPointZero, false, true);
	this->addBtn(notice);
	notice->setPosition(ccpAdd(bloodBg->getPosition(), ccp(-bloodBg->getContentSize().width / 2 - 16, 0)));
	
	CCLabelTTF *getPro = CCLabelTTF::create(str_BossBattleDetailLayer_str1, FONT_PENGYOU, 24);
	getPro->setAnchorPoint(ccp(0, 0.5));
//	getPro->setColor(ccBLACK);
	this->addChild(getPro, 2);
	getPro->setPosition(ccpAdd(bloodBg->getPosition(), ccp(-bloodBg->getContentSize().width / 2 - 10, -bloodBg->getContentSize().height * 1.5)));
	
	CCLabelTTF *atk = CCLabelTTF::create(str_BossBattleDetailLayer_str2, FONT_PENGYOU, 24);
	//atk->setColor(ccGREEN);
	atk->setAnchorPoint(ccp(0, 0.5));
	this->addChild(atk, 2);
	atk->setPosition(ccpAdd(getPro->getPosition(), ccp(getPro->getContentSize().width, 0)));
	
	atkNumLabel = CCLabelTTF::create(CCString::createWithFormat("+%d%%", atkMultiple)->getCString(), FONT_PENGYOU, 24);
	atkNumLabel->setColor(ccGREEN);
	atkNumLabel->setAnchorPoint(ccp(0, 0.5));
	this->addChild(atkNumLabel, 2);
	atkNumLabel->setPosition(ccpAdd(atk->getPosition(), ccp(atk->getContentSize().width, 0)));
	
	
	bomb = CCLabelTTF::create(str_BossBattleDetailLayer_str3, FONT_PENGYOU, 24);
//	bomb->setColor(ccGREEN);
	bomb->setAnchorPoint(ccp(0, 0.5));
	this->addChild(bomb, 2);
	bomb->setPosition(ccpAdd(atkNumLabel->getPosition(), ccp(atkNumLabel->getContentSize().width/2 + bomb->getContentSize().width/2 - 8, 0)));
	
	bombNumLabel = CCLabelTTF::create(CCString::createWithFormat("+%.1f", bombMultiple)->getCString(), FONT_PENGYOU, 24);
	bombNumLabel->setColor(ccGREEN);
	bombNumLabel->setAnchorPoint(ccp(0, 0.5));
	this->addChild(bombNumLabel, 2);
	bombNumLabel->setPosition(ccpAdd(bomb->getPosition(), ccp(bomb->getContentSize().width, 0)));
	
	
	bombPercent = CCLabelTTF::create(str_BossBattleDetailLayer_str4, FONT_PENGYOU, 24);
//	bombPercent->setColor(ccGREEN);
	bombPercent->setAnchorPoint(ccp(0, 0.5));
	this->addChild(bombPercent, 2);
	bombPercent->setPosition(ccpAdd(bombNumLabel->getPosition(), ccp(bombNumLabel->getContentSize().width/2 + bombPercent->getContentSize().width/2 - 16, 0)));

	bombPerLabel = CCLabelTTF::create(CCString::createWithFormat("+%d%%", bombPercentage)->getCString(), FONT_PENGYOU, 24);
	bombPerLabel->setColor(ccGREEN);
	bombPerLabel->setAnchorPoint(ccp(0, 0.5));
	this->addChild(bombPerLabel, 2);
	bombPerLabel->setPosition(ccpAdd(bombPercent->getPosition(), ccp(bombPercent->getContentSize().width, 0)));
	
	
	
	SGCCLabelTTF *info = SGCCLabelTTF::create(CCString::createWithFormat(str_BossBattleDetailLayer_str5)->getCString(), FONT_PENGYOU, 26);
	info->setAnchorPoint(ccp(0, 0.5));
	this->addChild(info, 2);
	info->setPosition(ccpAdd(ccp(s.width / 2, s.height / 2), ccp(-info->getContentSize().width * 1.2 - 40, 50 + contentShift)));
	
	info4 = SGCCLabelTTF::create(CCString::createWithFormat("%d", atkNum)->getCString(), FONT_PENGYOU, 26 , ccGREEN);
	info4->setAnchorPoint(ccp(0, 0.5));
    //modify by:zyc. merge into create.
	//info4->setColor(ccGREEN);
	this->addChild(info4, 2);
	info4->setPosition(ccpAdd(info->getPosition(), ccp(info->getContentSize().width, 0)));
	
	SGCCLabelTTF *info2 = SGCCLabelTTF::create(str_BossBattleDetailLayer_str6, FONT_PENGYOU, 26);
	info2->setAnchorPoint(ccp(0, 0.5));
	this->addChild(info2, 2);
	info2->setPosition(ccpAdd(info4->getPosition(), ccp(info4->getContentSize().width, 0)));
	
	info3 = SGCCLabelTTF::create(CCString::createWithFormat("%d", hurtBlood)->getCString(), FONT_PENGYOU, 26 , ccGREEN);
	info3->setAnchorPoint(ccp(0, 0.5));
    //modify by:zyc. merge into create.
	//info3->setColor(ccGREEN);
	this->addChild(info3, 2);
	info3->setPosition(ccpAdd(info2->getPosition(), ccp(info2->getContentSize().width, 0)));
    
    //开始战斗的按钮
	//#34190B
	if (backTimer <= 0)
	{
		startFight = SGButton::createFromLocal("an.png", str_GeneralInfoLayer_str6, this,  menu_selector(SGBossBattleDetailLayer::startBossFight),
											   ccp(0,3),FONT_PANGWA,ccc3(0x34, 0x19, 0x0b),32);
	}
	else
	{
		startFight = SGButton::createFromLocal("hui.png", str_GeneralInfoLayer_str6, this,  menu_selector(SGBossBattleDetailLayer::cantPressStartFight),
											   ccp(0,3),FONT_PANGWA,ccc3(0x80, 0x80, 0x80),32);
	}
	
	startFight->setOutsideColor(ccWHITE);
	startFight->setScale(1);
//	this->addBtn(startFight);
	menu->addChild(startFight);
	startFight->setPosition(ccpAdd(ccp(s.width / 2, s.height / 2), ccp(startFight->getContentSize().width * 1.2, -startFight->getContentSize().height * 0.2 + contentShift)));
	
	exitButton = SGButton::createFromLocal("an.png", str_BossBattleDetailLayer_str7, this,  menu_selector(SGBossBattleDetailLayer::exitBossFight),
													 ccp(0,3),FONT_PANGWA,ccc3(0x34, 0x19, 0x0b),32);
	exitButton->setOutsideColor(ccWHITE);
	exitButton->setScale(1);
	menu->addChild(exitButton);
//	this->addBtn(exitButton);
	exitButton->setPosition(ccpAdd(startFight->getPosition(), ccp(-startFight->getContentSize().width * 2.4, 0)));
	
	//四个按钮
	SGButton *back1 = SGButton::createFromLocal("store_exchangebtnbg.png",str_BossBattleDetailLayer_str8, this,  menu_selector(SGBossBattleDetailLayer::setButtonState),
												ccp(0,3),FONT_PANGWA,ccWHITE,24);
	SGButton *back2 = SGButton::createFromLocal("store_exchangebtnbg.png", str_BossBattleDetailLayer_str9, this,  menu_selector(SGBossBattleDetailLayer::setButtonState),
												ccp(0,3),FONT_PANGWA,ccWHITE,24);
	SGButton *back3 = SGButton::createFromLocal("store_exchangebtnbg.png", str_BossBattleDetailLayer_str10, this,  menu_selector(SGBossBattleDetailLayer::setButtonState),
												ccp(0,3),FONT_PANGWA,ccWHITE,24);
	SGButton *back4 = SGButton::createFromLocal("store_exchangebtnbg.png",str_BossBattleDetailLayer_str11, this,  menu_selector(SGBossBattleDetailLayer::setButtonState),
												ccp(0,3),FONT_PANGWA,ccWHITE,24);
	
//	back1->setScale(1);
//	back2->setScale(1);
//	back3->setScale(1);
//	back4->setScale(1);
	
	back1->setTag(BUTTON_1);
	back2->setTag(BUTTON_2);
	back3->setTag(BUTTON_3);
	back4->setTag(BUTTON_4);
	
	menu->addChild(back1);
	menu->addChild(back2);
	menu->addChild(back3);
	menu->addChild(back4);
	
	back1->setPosition(ccpAdd(exitButton->getPosition(), ccp(-back1->getContentSize().width * 0.3, -back1->getContentSize().height * 1.7 - 5)));
	back2->setPosition(ccpAdd(back1->getPosition(), ccp(back1->getContentSize().width * 1.3, 0)));
	back3->setPosition(ccpAdd(back2->getPosition(), ccp(back2->getContentSize().width * 1.3, 0)));
	back4->setPosition(ccpAdd(back3->getPosition(), ccp(back3->getContentSize().width * 1.3, 0)));
	
	if (backTimer <= 0)//第一次进入此界面
	{
		back1->setImage("barrack_anniu_hui.png");
		back1->setFontColor(ccc3(0x80, 0x80, 0x80));
		back1->setCallBack(this, menu_selector(SGBossBattleDetailLayer::firstEntryBossBattle));
		
		back2->setImage("barrack_anniu_hui.png");
		back2->setFontColor(ccc3(0x80, 0x80, 0x80));
		back2->setCallBack(this, menu_selector(SGBossBattleDetailLayer::firstEntryBossBattle));
		
		back3->setImage("barrack_anniu_hui.png");
		back3->setFontColor(ccc3(0x80, 0x80, 0x80));
		back3->setCallBack(this, menu_selector(SGBossBattleDetailLayer::firstEntryBossBattle));
		
		back4->setImage("barrack_anniu_hui.png");
		back4->setFontColor(ccc3(0x80, 0x80, 0x80));
		back4->setCallBack(this, menu_selector(SGBossBattleDetailLayer::firstEntryBossBattle));
	}
	else//复活界面
	{
		
	}
	
	
	
	
	CCSprite *gold1 = CCSprite::createWithSpriteFrameName("goldicon.png");
	this->addChild(gold1, 2);
	gold1->setPosition(ccpAdd(back1->getPosition(), ccp(-back1->getContentSize().width / 16, -back1->getContentSize().height / 1.5 - 10)));
	CCLabelTTF *num1 = CCLabelTTF::create(CCString::createWithFormat("%d", goldNum1)->getCString(), FONT_PENGYOU, 22);
	this->addChild(num1, 2);
	num1->setPosition(ccpAdd(gold1->getPosition(), ccp(gold1->getContentSize().width / 2, 0)));

	
	
	CCSprite *gold2 = CCSprite::createWithSpriteFrameName("goldicon.png");
	gold2->setAnchorPoint(ccp(0, 0.5));
	this->addChild(gold2, 2);
	gold2->setPosition(ccpAdd(back2->getPosition(), ccp(-back1->getContentSize().width / 1.5, -back1->getContentSize().height / 1.5 - 10)));
	CCLabelTTF *num2 = CCLabelTTF::create(CCString::createWithFormat("%d", goldNum2)->getCString(), FONT_PENGYOU, 22);
	num2->setAnchorPoint(ccp(0, 0.5));
	this->addChild(num2, 2);
	num2->setPosition(ccpAdd(gold2->getPosition(), ccp(gold2->getContentSize().width / 1.2, 0)));
	
	CCSprite *buffIcon1 = CCSprite::createWithSpriteFrameName("atk_icon.png");
	buffIcon1->setAnchorPoint(ccp(0, 0.5));
	this->addChild(buffIcon1, 2);
	buffIcon1->setPosition(ccpAdd(num2->getPosition(), ccp(num2->getContentSize().width + 3, 0)));
	
	CCLabelTTF *atkPercent = CCLabelTTF::create(CCString::createWithFormat("%d%%", value1)->getCString(), FONT_PENGYOU, 22);
	atkPercent->setAnchorPoint(ccp(0, 0.5));
	this->addChild(atkPercent, 2);
	atkPercent->setPosition(ccpAdd(buffIcon1->getPosition(), ccp(buffIcon1->getContentSize().width, 0)));
	
	
	CCSprite *gold3 = CCSprite::createWithSpriteFrameName("goldicon.png");
	gold3->setAnchorPoint(ccp(0, 0.5));
	this->addChild(gold3, 2);
	gold3->setPosition(ccpAdd(back3->getPosition(), ccp(-back1->getContentSize().width / 1.5, -back1->getContentSize().height / 1.5 - 10)));
	CCLabelTTF *num3 = CCLabelTTF::create(CCString::createWithFormat("%d", goldNum3)->getCString(), FONT_PENGYOU, 22);
	num3->setAnchorPoint(ccp(0, 0.5));
	this->addChild(num3, 2);
	num3->setPosition(ccpAdd(gold3->getPosition(), ccp(gold3->getContentSize().width / 1.2, 0)));
	
	CCSprite *buffIcon2 = CCSprite::createWithSpriteFrameName("boss_multiple.png");
	buffIcon2->setAnchorPoint(ccp(0, 0.5));
	this->addChild(buffIcon2, 2);
	buffIcon2->setPosition(ccpAdd(num3->getPosition(), ccp(num3->getContentSize().width + 3, 0)));
	
	CCLabelTTF *boPercent = CCLabelTTF::create(CCString::createWithFormat("%d%%", value2)->getCString(), FONT_PENGYOU, 22);
	boPercent->setAnchorPoint(ccp(0, 0.5));
	this->addChild(boPercent, 2);
	boPercent->setPosition(ccpAdd(buffIcon2->getPosition(), ccp(buffIcon2->getContentSize().width, 0)));
	
	
	CCSprite *gold4 = CCSprite::createWithSpriteFrameName("goldicon.png");
	gold4->setAnchorPoint(ccp(0, 0.5));
	this->addChild(gold4, 2);
	gold4->setPosition(ccpAdd(back4->getPosition(), ccp(-back1->getContentSize().width / 1.5, -back1->getContentSize().height / 1.5 - 10)));
	CCLabelTTF *num4 = CCLabelTTF::create(CCString::createWithFormat("%d", goldNum4)->getCString(), FONT_PENGYOU, 22);
	num4->setAnchorPoint(ccp(0, 0.5));
	this->addChild(num4, 2);
	num4->setPosition(ccpAdd(gold4->getPosition(), ccp(gold4->getContentSize().width / 1.2, 0)));
	
	CCSprite *buffIcon3 = CCSprite::createWithSpriteFrameName("bomb_icon.png");
	buffIcon3->setAnchorPoint(ccp(0, 0.5));
	this->addChild(buffIcon3, 2);
	buffIcon3->setPosition(ccpAdd(num4->getPosition(), ccp(num4->getContentSize().width + 3, 0)));
	
	CCLabelTTF *mbPercent = CCLabelTTF::create(CCString::createWithFormat("%d%%", value3)->getCString(), FONT_PENGYOU, 22);
	mbPercent->setAnchorPoint(ccp(0, 0.5));
	this->addChild(mbPercent, 2);
	mbPercent->setPosition(ccpAdd(buffIcon3->getPosition(), ccp(buffIcon3->getContentSize().width, 0)));
	
	
	//伤血排行
	CCSprite *rankList = CCSprite::createWithSpriteFrameName("hurt_rank.png");
//	SGCCLabelTTF *list = SGCCLabelTTF::create("伤血排行",  FONT_PANGWA, 32);
//	rankList->setColor(ccRED);
	this->addChild(rankList, 2);
	rankList->setPosition(ccpAdd(ccp(s.width / 2, s.height / 2), ccp(0, -210 + contentShift + ip4Shift)));
	
	//排行的背景
//	CCSprite *rankBg = CCSprite::createWithSpriteFrameName("");
	CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(s.width - 50, 200));
    this->addChild(frame2);
	frame2->setPosition(ccpAdd(ccp(s.width / 2, frame2->getContentSize().height / 2), ccp(0, 180 + scrollShift)));
	
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg, 0);
    frame2bg->setScaleX((s.width - 50)/frame2bg->getContentSize().width);
    frame2bg->setScaleY(200/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());

	//排行滑动区
	CCScrollView *m_scrollview = CCScrollView::create(CCSizeMake(s.width - 10, 150));
	m_scrollview->setViewSize(CCSizeMake(s.width - 10, 150));
    CCLayerColor *container = CCLayerColor::create(ccc4(0, 255, 0, 0));
    container->setContentSize(CCSizeMake(s.width, 150));
    m_scrollview->setContainer(container);
	m_scrollview->setPosition(ccpAdd(ccp(0, 0), ccp(0, 185 + scrollShift)));//设置scrollview位置
	
	m_scrollview->setDirection(kCCScrollViewDirectionVertical);
    addChild(m_scrollview);
	m_scrollview->setTag(103);
	
	//测试数据
	int finalLen = 0;
	int rankSize = bossDatas->rankData->count();
	for (int i = rankSize - 1; i >= 0; i--)
	{
		SGBossRankData *data = static_cast<SGBossRankData *>(bossDatas->rankData->objectAtIndex(i));
		SGRateItem *item = SGRateItem::create(CCString::create(data->roleName), i + 1, data->roleLvl, data->roleHurt);
		container->addChild(item);
		item->setPosition(ccpAdd(container->getPosition(), ccp(item->getContentSize().width / 2.5,
															   (item->getContentSize().height + 10) * (rankSize - i))));
		finalLen += (item->getContentSize().height + 10);
	}
	m_scrollview->setContentOffset(ccp(0, -finalLen + m_scrollview->getContentSize().height - 10));
	m_scrollview->setContentSize(CCSizeMake(s.width, finalLen));
	m_scrollview->getContainer()->setContentSize(CCSizeMake(s.width, finalLen + 10));
	
	//初始化倒计时
	if (backTimer != 0)//如果打过了一次boss战，那才出现倒计时
	{
		CCLabelTTF *preTimeStr = CCLabelTTF::create(str_BossBattleDetailLayer_str12, FONT_PENGYOU, 30);
		preTimeStr->setAnchorPoint(ccp(0, 0.5));
		this->addChild(preTimeStr, 2);
		preTimeStr->setPosition(ccpAdd(exitButton->getPosition(), ccp(exitButton->getContentSize().width / 1.2, 0)));
		
		timer = CCLabelTTF::create(CCString::createWithFormat("%lld", backTimer)->getCString(), FONT_PENGYOU, 30);
		timer->setAnchorPoint(ccp(0, 0.5));
		timer->setColor(ccGREEN);
		this->addChild(timer, 2);
		timer->setPosition(ccpAdd(preTimeStr->getPosition(), ccp(preTimeStr->getContentSize().width + 5, 0)));
		
		this->schedule(schedule_selector(SGBossBattleDetailLayer::updateTimer), 1.0);
	}
	
	//定时刷新榜单消息
	this->schedule(schedule_selector(SGBossBattleDetailLayer::sendFreshBossRankMsg), TIME_GRAP);
//	this->sendFreshBossRankMsg();
	
}


//定时发刷新榜单消息
void SGBossBattleDetailLayer::sendFreshBossRankMsg()
{
	main::BossCurrStateRequest *stateReq = new main::BossCurrStateRequest();
	SGSocketClient::sharedSocketClient()->send(MSG_FLUSH_BOSS_INFO, stateReq, false);
}


//倒计时未结束的回调
void SGBossBattleDetailLayer::cantPressStartFight()
{
	SG_SHOW_WINDOW(str_BossBattleDetailLayer_str13);
}

//更新复活秒数
void SGBossBattleDetailLayer::updateTimer()
{
	backTimer--;
	if (backTimer <= 0)
	{
		backTimer = 0;
		this->unschedule(schedule_selector(SGBossBattleDetailLayer::updateTimer));
		startFight->setImage("an.png");
		startFight->setFontColor(ccc3(0x34, 0x19, 0x0b));
		startFight->setCallBack(this, menu_selector(SGBossBattleDetailLayer::startBossFight));
		
		//倒计时清0的时候，四个按钮变灰
		CCMenu *menu = (CCMenu *)getChildByTag(MENU_TAG);
		for (int i = BUTTON_1; i <= BUTTON_4; i++)
		{
			SGButton *item = (SGButton *)menu->getChildByTag(i);
			item->setImage("barrack_anniu_hui.png");
			item->setFontColor(ccc3(80, 80, 80));//字体蒙灰
			item->setCallBack(this, menu_selector(SGBossBattleDetailLayer::unpressBuffButton));
		}

	}
	
	timer->setString(CCString::createWithFormat("%lld", backTimer)->getCString());
	
}

//boss战说明
void SGBossBattleDetailLayer::bossBattleReadMe()
{
	SGBossTips *tipBox = SGBossTips::create();
	SGMainManager::shareMain()->showBox(tipBox);
	
}


//购买buff后对应的属性改变
void SGBossBattleDetailLayer::getBossBuffAndChange(CCObject *sender)
{
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::GetBossBuffResponse *buffRes = (main::GetBossBuffResponse *)req->m_msg;
		if (buffRes)
		{
			if (buffRes->state() == 1)
			{
				SG_SHOW_WINDOW(str_BossBattleDetailLayer_str14);
				int v1 = buffRes->value1();
				float v2 = buffRes->value2();
				int v3 = buffRes->value3();
				
				//更新页面显示
				this->freshBuff(v1, v2, v3);
				this->startBossFight();//购买完成后直接进入战斗
			}
			else if (buffRes->state() == 2)//失败的情况
			{
				SG_SHOW_WINDOW(str_BossBattleDetailLayer_str15);
				CCMenu *menu = (CCMenu *)getChildByTag(MENU_TAG);
				//遍历放在menu中的四个按钮，哪个被按下，则其他都改变状态
				for (int i = BUTTON_1; i <= BUTTON_4; i++)
				{
					//如果是按下的这个按钮，不做处理
					SGButton *item = (SGButton *)menu->getChildByTag(i);
					item->setImage("store_exchangebtnbg.png");
					item->setFontColor(ccWHITE);//字体蒙灰
					
                
                    item->setCallBack(this, menu_selector(SGBossBattleDetailLayer::setButtonState));
				}

				return;
			}
			else if (buffRes->state() == 0)//boss战结束,但是玩家还是在页面中,点击购买按钮就直接去排行榜
			{
				SGMainManager::shareMain()->sendGetLastBossRankList();
			}
		}
	}
}

//首次进入boss战，下排四个按钮不可用
void SGBossBattleDetailLayer::firstEntryBossBattle()
{
    SG_SHOW_WINDOW(str_BossBattleDetailLayer_str16);
}

int SGBossBattleDetailLayer::needGold(int tag)
{
	int gold = 0;
	switch (tag) {
		case BUTTON_1:
			gold = goldNum1;
			break;
		case BUTTON_2:
			gold = goldNum2;
			break;
		case BUTTON_3:
			gold = goldNum3;
			break;
		case BUTTON_4:
			gold = goldNum4;
			break;
			
		default:
			break;
	}
	return gold;
}

//设置四个按钮的选中状态
void SGBossBattleDetailLayer::setButtonState(CCObject *obj)
{
	int tag = ((SGButton *)obj)->getTag();//被点击的按钮
    int index = tag -1001 ; //获取buf等级限制数组的下标。
    
    if ( (index == 0) && (m_bufVipLevelLimit[index] < 1) ) //点击第一个buf  且 其对应vipLevelLimit值为0  即普通账号也可使用该buf ,,假定等级限制递增
    {
        
    }
    else
    {
        if (0 == SGPlayerInfo::sharePlayerInfo()->getShowvip() ) //服务器关闭vip状态
        {
            //return ;
        }
        else  //if(SGPlayerInfo::sharePlayerInfo()->getShowvip())
        {
            if(tag>1001)
            {
                if( SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() < m_bufVipLevelLimit[index] )
                {
                    SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_BossBattleDetailLayer_str1601,m_bufVipLevelLimit[index])->getCString() );
                    return;
                }
            }
            
        }
        if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold() <= 0 || SGPlayerInfo::sharePlayerInfo()->getPlayerGold() < needGold(tag))
        {
            SG_SHOW_WINDOW(str_BossBattleDetailLayer_str17);
            return ;
        }
    }
    
	CCMenu *menu = (CCMenu *)getChildByTag(MENU_TAG);
	//遍历放在menu中的四个按钮，哪个被按下，则其他都改变状态
	for (int i = BUTTON_1; i <= BUTTON_4; i++)
	{
		//如果是按下的这个按钮，不做处理
		if (BUTTON_4 == tag && bombPercentage >= 100)
		{
			SG_SHOW_WINDOW(str_BossBattleDetailLayer_str18);
			return;
		}
		SGButton *item = (SGButton *)menu->getChildByTag(i);
		item->setImage("barrack_anniu_hui.png");
		item->setFontColor(ccc3(80, 80, 80));//字体蒙灰
		item->setCallBack(this, menu_selector(SGBossBattleDetailLayer::unpressBuffButton));
	}
	main::GetBossBuffRequest *getBuff = new main::GetBossBuffRequest();
	getBuff->set_key(tag - 1000);
	SGSocketClient::sharedSocketClient()->send(MSG_GET_BOSS_BUFF, getBuff);
}
//点击一个buff之后，不可再点其他的
void SGBossBattleDetailLayer::unpressBuffButton()
{
    SG_SHOW_WINDOW(str_BossBattleDetailLayer_str16);
}

#pragma mark "===============开始战斗入口==============="
void SGBossBattleDetailLayer::startBossFight()
{
	CCLOG("Start Fight!");
    
    main::AtkBossRequest *request = new main::AtkBossRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_SHOW, request);
     BossBattleData *bossData;
//    CCArray *array=CCArray::create();
//    array->retain();
   
//    SGStoryReward *storyReward= SGStoryReward::create(100, 10, 10, 10, array);
//    storyReward->setExtExp(10);
//    storyReward->setGold(10);
//    storyReward->setHpRate(10);
//    storyReward->setLianDan(0);
//    storyReward->setBeforeLevel(10);
//    
//    BossReward *bossreward=new BossReward();
//    bossreward->damage=10;
//    bossreward->gold=10;
//    bossreward->other=100;
// 
//
//     SGMainManager::shareMain()->showBossBattleLayer(bossData,bossreward);
//    SGBossBattleLayer *layer=(SGBossBattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossBattleLayer);
//    if (layer) {
//        layer->startAction();
//    }
    
}
void  SGBossBattleDetailLayer::startbossFightListener(CCObject *obj)
{
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::AtkBossResponse *response = (main::AtkBossResponse *)sr->m_msg;
    if (sr)
    {
        if (response->state()) {
            BossBattleData *bossData=new BossBattleData();
            bossData->bossItemId=response->bossitemid();
            bossData->bossHp=response->bosshp();
            bossData->bossBaseHp=response->bossbasehp();
            bossData->lordItemId=response->roleitemid();
            bossData->lorddamge=response->roledamage();
            bossData->lordType=response->lordtype();
            bossData->isCrit=response->iscrit();
//            CCArray *array=CCArray::create();
//            array->retain();
//
//            SGStoryReward *storyReward= SGStoryReward::create(0, 0, 0, 0, array);
//            storyReward->setExtExp(0);
//            storyReward->setGold(0);
//            storyReward->setHpRate(0);
//            storyReward->setLianDan(0);
//            storyReward->setBeforeLevel(0);
            
            BossReward *bossreward =new BossReward();
            bossreward->damage=response->roledamage();
            bossreward->coinNumber=response->coins();
            bossreward->MedalNumber=response->medal();
            SGMainManager::shareMain()->showBossBattleLayer(bossData,bossreward);

            
//            int var1 = response->rating();
//            int var2 = response->exp();
//            int var3 = response->gold();
//            CCArray *var4 = CCArray::create();
//            SGStoryReward *reward = SGStoryReward::create(var1, var2, var3, jexp, var4);
//            reward->setLianDan(response->liandan());
//            reward->setExtExp(response->addexp());
//            int size = response->plotdropitems_size();
//            if (size > 0) { // 先将卡片加入背包中
//                CCArray *rItems = reward->getItems();
//                for (int ii = 0; ii < size; ii++) {
//                    SGStoryRdItem *rdModel = NULL;
//                    SGBaseMilitaryCard *card = NULL;
//                    main::PlotDropItems item = response->plotdropitems(ii);
//                    if (item.officercards().itemid() > 0) {
//                        int id = item.officercards().itemid();
//                        SGOfficerDataModel *off = SGStaticDataManager::shareStatic()->getOfficerById(item.officercards().itemid());
//                        //int tag = off->getOfficerRace();
//                        int tag = off->getOfficerVisitType();
//                        rdModel = SGStoryRdItem::create(id, tag, 0);
//                        card = createOfficerCardWithInfo(item.officercards());
//                        playInfo->addOfficerCard((SGOfficerCard *)card);
//                    }else if (item.equipmentcards().itemid() > 0) {
//                        int id = item.equipmentcards().itemid();
//                        SGEquipmentDataModel *equ = SGStaticDataManager::shareStatic()->getEquipById(id);
//                        //int tag = equ->getEquipType();
//                        int tag = equ->getEquipVisitType();
//                        rdModel = SGStoryRdItem::create(id, tag, 1);
//                        card = createEquipCardWithInfo(item.equipmentcards());
//                        playInfo->addEquipCard((SGEquipCard *)card);
//                    }else if (item.propscards().itemid() > 0) {
//                        int id = item.propscards().itemid();
//                        rdModel = SGStoryRdItem::create(id, 0, 3);
//                        card = createPropCardWithInfo(item.propscards());
//                        playInfo->addPropCard((SGPropsCard *)card);
//                    }else if (item.consumablecards().itemid() > 0)//消耗品
//                    {
//                        int id = item.consumablecards().itemid();
//                        rdModel = SGStoryRdItem::create(id, 0, 6);
//                        card = createConsumeCardWithInfo(item.consumablecards());
//                        playInfo->addConsumableCard((SGConsumableCard *)card);
//                    }
//                    
//                    rItems->addObject(rdModel);
//                }
//            }

            SGMainManager::shareMain()->showBossBattleLayer(bossData,bossreward);
            SGBossBattleLayer *layer=(SGBossBattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossBattleLayer);
               if (layer) {
                   layer->startAction();
            }
        }
        else
        {
            SGMainManager::shareMain()->showMessage(response->errmsg().c_str());
        }
        
    }

   
}
void SGBossBattleDetailLayer::exitBossFight()
{
	CCLOG("Exit Fight!");
	SGMainManager::shareMain()->showFirstLayer();//回到首页
}

void SGBossBattleDetailLayer::setIsCanTouch(bool isTouch)
{
    isCanTouch = isTouch;
    menu->setTouchEnabled(isTouch);
    
    CCMenu *menu = (CCMenu *)getChildByTag(MENU_TAG);
	//遍历放在menu中的四个按钮，哪个被按下，则其他都改变状态
    CCObject *buttons = NULL;
    CCARRAY_FOREACH(menu->getChildren(), buttons)
	{
		SGButton *item = (SGButton *)buttons;
		item->setEnabled(isTouch);
	}
    
    CCScrollView *last = (CCScrollView *)this->getChildByTag(103);
    if (last)
    {
        last->setTouchEnabled(isTouch);
    }
}


#pragma mark "-----------说明提示------------"
SGBossTips::SGBossTips()
{
	
}
SGBossTips::~SGBossTips()
{
	
}

SGBossTips *SGBossTips::create(SGBoxDelegate *del, int enter)
{
	SGBossTips *tips = new SGBossTips();
	if (tips && tips->init(del, box_bossTips, CCSizeMake(450, 550)))
	{
		tips->initView();
		tips->autorelease();
		return tips;
	}
	CC_SAFE_RELEASE(tips);
	return NULL;
}

void SGBossTips::initView()
{
	CCPoint center = SGLayout::getPoint(kMiddleCenter);

	fontbg->setPreferredSize(CCSizeMake(380, 380));
	fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 25)));
	SGCCLabelTTF *tips = SGCCLabelTTF::create(str_BossBattleDetailLayer_str19, FONT_PANGWA, 32 ,ccRED);
    //modify by:zyc. merge into create.
	//tips->setColor(ccRED);
	this->addChild(tips, 2);
	tips->setPosition(ccpAdd(biao->getPosition(), ccp(0, 0)));
	
	//1.起死回生：复活等待时间变为0立即复活
	//2.涅槃重生：立即复活并使自身攻击力增加20%
	//3.浴火重生：立即复活并使爆击伤害倍数增加0.2
	//4.王者归来：立即复活并使自身爆击率增加20%
	
	SGCCLabelTTF *content1 = SGCCLabelTTF::create(str_BossBattleDetailLayer_str20, FONT_BOXINFO, 26, CCSizeMake(310, 80));
	this->addChild(content1, 2);
	content1->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 120)));
	
	SGCCLabelTTF *content2 = SGCCLabelTTF::create(str_BossBattleDetailLayer_str21, FONT_BOXINFO, 26, CCSizeMake(310, 80));
	this->addChild(content2, 2);
	content2->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 40)));
	
	SGCCLabelTTF *content3 = SGCCLabelTTF::create(str_BossBattleDetailLayer_str22, FONT_BOXINFO, 26, CCSizeMake(310, 80));
	this->addChild(content3, 2);
	content3->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -40)));
	
	SGCCLabelTTF *content4 = SGCCLabelTTF::create(str_BossBattleDetailLayer_str23, FONT_BOXINFO, 26, CCSizeMake(310, 80));
	this->addChild(content4, 2);
	content4->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -120)));
	
	SGButton *okButton = SGButton::createFromLocal("box_btnbg.png", str_Ok, this, menu_selector(SGBossTips::pressOkButton),
												   ccp(0, 0), FONT_PANGWA, ccWHITE, 24);
	this->addBtn(okButton);
	okButton->setPosition(ccpAdd(center, ccp(0, -324/2+5 -70)));
}
void SGBossTips::pressOkButton()
{
	SGMainManager::shareMain()->closeBox();
	//this->boxClose();
}



//排行内的item
SGRateItem::SGRateItem()
{
	name = NULL;
	order = 0;
	lvl = 0;
	fightPower = 0;
}
SGRateItem::~SGRateItem()
{
	
}

SGRateItem *SGRateItem::create(CCString *name, int order, int level, int fightPower)
{
	SGRateItem *item = new SGRateItem();
	if (item)
	{
		item->name = name;
		item->order = order;
		item->lvl = level;
		item->fightPower = fightPower;
		item->initView();
		item->autorelease();
		return item;
	}
	CC_SAFE_RELEASE(item);
	return NULL;
}

void SGRateItem::initView()
{
	CCLayer *bg = CCLayer::create();
	this->addChild(bg, 1);
	
	//名字与排名数字
	SGCCLabelTTF *playerName = SGCCLabelTTF::create(CCString::createWithFormat("%3d.	%s", order, name->getCString())->getCString(), FONT_PENGYOU, 24);
	playerName->setAnchorPoint(ccp(0, 0.5));
//	playerName->setColor(ccYELLOW);
	bg->addChild(playerName, 2);
	playerName->setPosition(ccpAdd(playerName->getPosition(), ccp(-100, 0)));
	
	//等级
	SGCCLabelTTF *level = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d", lvl)->getCString(), FONT_PENGYOU, 24);
//	level->setColor(ccYELLOW);
	level->setAnchorPoint(ccp(0, 0.5));
	bg->addChild(level, 1);
	level->setPosition(ccpAdd(playerName->getPosition(), ccp(225, 0)));
	
	//伤害数值
	SGCCLabelTTF *hurtNum = SGCCLabelTTF::create(CCString::createWithFormat("%d", fightPower)->getCString(), FONT_PENGYOU, 24);
//	hurtNum->setColor(ccYELLOW);
	hurtNum->setAnchorPoint(ccp(0, 0.5));
	bg->addChild(hurtNum, 1);
	hurtNum->setPosition(ccpAdd(level->getPosition(), ccp(165, 0)));
	
	//设置item的大小
	this->setContentSize(CCSizeMake(CCDirector::sharedDirector()->getWinSize().width - 230, 30));
}





