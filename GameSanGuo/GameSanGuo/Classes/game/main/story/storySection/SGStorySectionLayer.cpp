//
//  SGStorySectionLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGStorySectionLayer.h"
#include "SGMainManager.h"
#include "SGButton.h"
#include "SGCantAdvanceBox.h"
#include "SGStorySceneBean.h"
#include "SGStaticDataManager.h"
#include "SGBattleManager.h"
#include "SGGuideManager.h"

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "CooperationModule.pb.h"
#include "SGPveHelper.h"
#include "SGStaticDataManager.h"
#include "ShopModule.pb.h"
#include "PlotModule.pb.h"
#include "SimpleAudioEngine.h"
#include "SGCreamBattleLayer.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
#include "SGShowRewardBox.h"
#include "SGQFightBox.h"
#include "AppMacros.h"
#include "SGStringConfig.h"

USING_NS_CC;

#define SECTIONINFOFONTSIZE   11
#define SECTIONTABLEVIEWBOTTOM   8
#define SECTIONINFOVIEWTAG -1000
#define SECTIONCELLGAP 40
#define SKIPHELPERINDEX  0
#define AUTOREWARDID  9

SGStorySectionLayer::SGStorySectionLayer(void):m_starRewardCell(NULL)
,storyBean(NULL)
,mydatas(NULL)
, selectIndex(0)
, lastSelectIndex(0)
, lastMoveDis(0)
,sectiontableView(NULL)
, infoViewPosY(0)
,m_lastShowSection(NULL)
, buyTimes(0)
, storySecenId(0)
, isAutoReward(0)
,isPopRewardWindow(false)
, m_nChallengeCount(0)
, m_nChallengeMaxcount(0)
{
    datas = NULL;
    size = CCSizeMake(960, 640);
}
SGStorySectionLayer::~SGStorySectionLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_storySectionLayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_FINDHELPER);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_XINREWARD);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CREAM_STORY);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CREAM);
    storyBean->release();
    storyBean = NULL;
    CC_SAFE_RELEASE(datas);
    
    SGMainManager::static_plotId = -1;
    SGMainManager::sourceToStory = false;
}

//设置对应的menu在新手引导时不会产生穿透//11.20
void SGStorySectionLayer::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(10);//设置menu的优先级
}

SGStorySectionLayer* SGStorySectionLayer::create(SGStoryBean* bean)
{
    SGStorySectionLayer *actionLayer = new SGStorySectionLayer();
    if (actionLayer && actionLayer->init(NULL, sg_storySectionLayer))
    {
        actionLayer->initData(bean);
        actionLayer->initView();
        actionLayer->autorelease();
        return actionLayer;
    }
    CC_SAFE_DELETE(actionLayer);
    return NULL;
}

void SGStorySectionLayer::initData(SGStoryBean* bean)
{
    this->datas = CCArray::create();
    this->datas->retain();
    this->m_nChallengeCount = bean->getChallengeCount();
    this->m_nChallengeMaxcount = bean->getMaxChallengeCount();
    isMovingSection = false;
    m_starRewardCell = NULL;
    
    if (bean->getStoryStars() > 0 && bean->getStoryId() < 10000)
    {
        SGStorySceneBean *scene = SGStorySceneBean::create(bean->getStoryId(), bean->getStoryId(), MaxSceneId,-10, bean->getStoryName()->getCString());
        scene->setVisible(true);
        scene->setRank(0);
        scene->setOpenState(bean->getCanReward());
        scene->setPower(bean->getStoryStars());
        scene->setZhandou(bean->getMaxStar());
        scene->setExp(STARWARDTAG);
        
        this->datas->addObject(scene);
    }
    
    int size = bean->getStorySections()->count();
    bool haveSet = false;
    for (int ii = size-1; ii >= 0; ii--)
    {
        SGStorySceneBean *obj = (SGStorySceneBean *)bean->getStorySections()->objectAtIndex(ii);
        if (obj->getVisible() == true || ii == 0)
        {
            this->datas->addObject(obj);
            
             //刷新最后的关卡数
            if (!haveSet)
            {
                haveSet = true;
                SGPlayerInfo::sharePlayerInfo()->setMaxPlotId(obj->getPlotId());
            }
        }
    }

    storyBean  = bean;
    storyBean->retain();
    
    if (datas->count() > 0)
    {
        storySecenId = ((SGStorySceneBean *)datas->objectAtIndex(0))->getPlotId();
    }
}



//精英副本购买监听
void SGStorySectionLayer::buyCreamListener(CCObject *sender)
{
	//CCLOG("购买成功了——————");
	SGSocketRequest *request = (SGSocketRequest *)sender;
	if (request)
	{
		main::BossPlotBuyCountResponse *response = (main::BossPlotBuyCountResponse *)request->m_msg;
		//成功
		if (response->state() == 1)
		{
			/*
			 this->isFromBuy = true;
			 int selectIndex = 0;
			 if (delegate)
			 {
			 selectIndex = delegate->getSelectIndex();
			 }
			 storyBean = (SGStorySceneBean *)data->objectAtIndex(selectIndex);
			 if (delegate)
			 delegate->entrySectionHandler(this);
			 */
			//如果购买成功，刷新story数据
			SGMainManager::shareMain()->refreshCreamBattleList(storyBean->getStoryId() );
			
		}
		else if (response->state() == 2)
		{
			SG_SHOW_WINDOW(str_ShouJingZhou_str13);
		}
		else if (response->state() == 3)
		{
			SG_SHOW_WINDOW(str_ShouJingZhou_str14);
		}
	}
	
}


//购买成功后，刷新story数据
void SGStorySectionLayer::freshStoryScenceData(CCObject *sender)
{
	SGSocketRequest *req = (SGSocketRequest *)sender;
    main::BossPlotListResponse *response = (main::BossPlotListResponse *)req->m_msg;
    CCLOG("scene=================%d", response->bossplotmodel_size());
    if (response) {
		
        int size = response->bossplotmodel_size();
        if (size > 0) {
            for (int ii = 0; ii < size; ii++) {
                main::BossPlotModel model = response->bossplotmodel(ii);
                CCLOG("scene ID=================%d", model.plotid());
                SGStorySceneBean *scene = this->storyBean->getSceneBeanById(model.plotid());
                if (scene == NULL) {
                    continue;
                }else {
                    scene->setVisible(true);
                    //scene->setRank(model.ra);
					scene->setPower(model.power());
					scene->setGold(model.gold());
					scene->setExp(model.exp());
					scene->setZhandou(model.zhandou());
					scene->setRank(model.rank());
					scene->setCreamCanJoin(model.canjoin());
					
                    //数据存储在playerinfo中
                    SGPlayerInfo::sharePlayerInfo()->setCreamData(model.plotid(), model.canjoincount(), model.canjoin());

                    
					scene->setCreamDayTimes(model.canjoincount()); //當前可挑戰次數
					scene->setCreamBuyTimes(model.canbuycount()); //剩餘可重置的次數
					scene->setBuyPrice(model.price());
					scene->setBuyCompTimes(model.buycount());// 已经重置的次数
					scene->setDropInfo(model.exp(), model.gold());
					int itemNum = model.itemids_size();
					scene->itemIds.clear();
					for (int jj =0;jj<itemNum; jj++) {
						scene->itemIds.push_back(model.itemids(jj));
					}
					
                }
                SGPlayerInfo::sharePlayerInfo()->setMaxBossPlotId(model.plotid());
            }
        }
        SGMainManager::shareMain()->m_trackIsPve = false;
        //购买精英时 只刷数据 返回上层列表，
        this->refreshCreamOnBuy();
//        SGMainManager::shareMain()->showCreamBattleLayer();
    }
	
}



void SGStorySectionLayer::initView()
{
    size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_STORY_FINDHELPER, this, callfuncO_selector(SGStorySectionLayer::showHelperLayer));
    notification->addObserver(MSG_STORY_XINREWARD, this, callfuncO_selector(SGStorySectionLayer::rewardSuccess));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_BUY_CREAM, this, callfuncO_selector(SGStorySectionLayer::buyCreamListener));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CREAM_STORY, this, callfuncO_selector(SGStorySectionLayer::freshStoryScenceData));
    
//    ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayer1.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    ResourceManager::sharedInstance()->bindTexture(SGMainManager::shareMain()->getPlistWithChannel().c_str(), RES_TYPE_LAYER_UI, sg_storySectionLayer);

    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    
    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, 1136/2-(size.height-headH-bottomH)/2, bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    bgImg->setVisible(false);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,1);//george
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,1);//george
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(storyBean->getStoryName()->getCString(), FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
    //title->setColor(ccc3(0xff, 0x95, 0x0c));
    
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(title);
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGStorySectionLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    
    //这里开始引导领取奖励之后的逻辑，点击返回按钮
    int tag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_50);
    backBtn->setTag(tag);
    
	backBtn->setScale(1.05);
	
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.65f, title->getPosition().y));
	
	SGButton *creamButton = NULL;
	//精英副本
	if (storyBean->getStoryId() >= 20000)
	{
		creamButton = SGButton::create("pt.png", NULL, this, menu_selector(SGMainLayer::storyHandler), ccp(0, 0), false, true);
		this->addBtn(creamButton);
		creamButton->setPosition(ccp(size.width-creamButton->getContentSize().width * 0.45f, title->getPosition().y+5));
	}
	else if (storyBean->getStoryId() <= 10000)//限时副本中不能显示精英副本按钮
	{
		//10.29//george 子列表里的加入挑战精英按钮
		creamButton = SGButton::create("jingying.png", NULL, this, menu_selector(SGMainLayer::creamBattleHandler), ccp(0, 0), false, true);
		this->addBtn(creamButton);
		creamButton->setPosition(ccp(size.width-creamButton->getContentSize().width * 0.45f, title->getPosition().y+5));
	}else if(storyBean->getStoryId() > 10000&&storyBean->getStoryId() < 20000)
    {
        SGCCLabelTTF* labelChallengeTimes = SGCCLabelTTF::create(CCString::createWithFormat(str_SGStorySectionLayer_str101,m_nChallengeCount,m_nChallengeMaxcount)->getCString(), FONT_PANGWA, FONTSIZE(12) , COLOR_UNKNOW_TAN);
        //modify by:zyc. merge into create.
        //labelChallengeTimes->setColor(ccc3(0xff, 0x95, 0x0c));
        labelChallengeTimes->setPosition(ccpAdd(title->getPosition(),ccp(title->getContentSize().width /2+labelChallengeTimes->getContentSize().width * 0.55f, 0)));
        this->addChild(labelChallengeTimes);

    }
	
	
    int modelTag = 0;

    //闯关 节列表的tableview的tag
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_4);
 
    if (storyBean->getStoryId() >= 10000 && datas->count() > 6) {
        tableView->iscreateall=true;
    }
    
    tableView->setTag(modelTag);
	tableView->setDown(-25);//设置故事模式中漩涡特效不遮挡
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(12) + 20));//设置故事模式中漩涡特效不遮挡
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
	tableView->setPageNEnable(false);
    //如果是在引导中就不再自动弹出扫荡
    
    if( (SGPlayerInfo::sharePlayerInfo()->getsaodangType() == 1||SGPlayerInfo::sharePlayerInfo()->getsaodangType() == 2)
       && !SGGuideManager::shareManager()->isGuide)
    {
        SGPlayerInfo::sharePlayerInfo()->setsaodangType(0);
        int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
        for(int i = 0 ; i< datas->count(); i++)
        {
            SGStorySceneBean *sceneBean = (SGStorySceneBean *)datas->objectAtIndex(i);
            if(sceneBean->getPlotId() == plotId)
            {
                 SGStorySectionCell *item = SGStorySectionCell::create(this,sceneBean);
                selectIndex = i;
                this->entrySectionHandler(item);
                break;
            }
        }
        
    }

}



void SGStorySectionLayer::backHandler(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
	if (plotId >= 20000)//精英副本
	{
        //请求1次，刷新精英副本
        if (SGMainManager::shareMain()->getMainLayer())
        {
            SGMainManager::shareMain()->getMainLayer()->requestCream();
        }
        else
        {
            SGMainManager::shareMain()->showCreamBattleLayer();//返回精英副本
        }
		//SGMainManager::shareMain()->showCreamBattleLayer();//返回精英副本
	}
    else if (plotId >= 10000)
	{ // activity model
		//精英副本返回
		SGMainManager::shareMain()->showActivityLayer();
    }
	else
	{ // story model
        if (storyBean->getIsWin()>0)
        {
            
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
            SGStoryBean *story = NULL;
            int storyId = storyBean->getStoryId();
            if (array == NULL)//战斗中断
            {
                story = SGStaticDataManager::shareStatic()->getStoryDataById(1);
            }
            else
            {
                for (int ii = 0; ii < array->count(); ii++)
                {
                    story = (SGStoryBean *)array->objectAtIndex(ii);
                    if (storyId == story->getStoryId())
                    {
                        break;
                    }
                }
            }

            int storyStar = 3;
            int size = story->getStorySections()->count();
            for (int ii = size-1; ii >= 0; ii--)
            {
                SGStorySceneBean *obj = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(ii);
                if (obj->getRank() < storyStar)
                {
                    storyStar = obj->getRank();
                }
            }
            if (storyStar == 3 && story->getRankReward() == 0 && storyBean->getCanReward() != 0)
            {
                story->setRankReward(1);
            }
            else if (story->getRankReward() == 1 && storyBean->getCanReward() == 0)
            {
                story->setRankReward(2);
            }
            story->setRankStars(storyStar);
        }
        SGMainManager::shareMain()->showStoryMainLayer();
    }
    
    
    //引导第guide_tag_50，触发第51步
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId >= guide_tag_50)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_51);
    }
    
	
}

void SGStorySectionLayer::dealFirstSectionInfo()
{
    SNSTableViewCell *cell = NULL;
    int rowCount = tableView->getRowCount();
    size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    
    for (int ii = 0; ii < rowCount; ii++) {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            pos = cell->getPosition();
            cell->stopAllActions();
            cell->setPosition(ccp(0,pos.y));
        }
    }
    SNSTableView::showScrollItemMultiMoveFromRight(tableView);//1126效果优化
    CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-size.width*1.05, 0));
    CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(size.width*0.1, 0));
    CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-size.width*0.05, 0));

    CCAction *action = CCSequence::create((CCDelayTime::create(0.15f)),moveby1,moveby2,moveby3,NULL);
  

   sectiontableView->runAction(action);
}

void SGStorySectionLayer::showFirstSectionInfo()
{
    ////是否从来源页面进入闯关 每次使用完成 置false
    if(SGMainManager::sourceToStory && storySecenId < 20000)
    {
        return ;
    }
    
    SGStorySceneBean *sceneBean = (SGStorySceneBean *)datas->objectAtIndex(0);
    //添加精英闯关 无论打过与否  即展开。 大于20000即为精英闯关
    int stroyId = sceneBean->getStoryId();
    if ( (sceneBean && sceneBean->getRank() == 0) || (storySecenId >=20000) ) {
        if ( sceneBean->getExp() != STARWARDTAG || sceneBean->getOpenState() >0) {
            SNSTableViewCell *cell = NULL;
            int rowCount = tableView->getRowCount();
            size = CCDirector::sharedDirector()->getWinSize();
            CCPoint pos = CCPointZero;
            
            for (int ii = 0; ii < rowCount; ii++) {
                cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
                if (cell != NULL)
                {
                    pos = cell->getPosition();
                    cell->stopAllActions();
                    cell->setPosition(ccpAdd(ccp(0,pos.y), ccp(size.width, 0)));
                }
            }

            selectIndex = 0;
            SGStorySectionCell *item = (SGStorySectionCell*)tableView->dequeueReusableCellItemForIndexPath(SNSIndexPath::create(0, 0, 1));
            setSectionFirstInfo(item);
            CCPoint sectionInfoPos = sectiontableView->getPosition();
            sectiontableView->setPosition(ccp(sectionInfoPos.x + size.width,sectionInfoPos.y));
            this->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME),
                                               CCCallFunc::create(this, callfunc_selector(SGStorySectionLayer::dealFirstSectionInfo)),NULL));
            return;
        }
    }
    SNSTableView::showScrollItemMultiMoveFromRight(tableView);
}

//返回对应storyId的boss关是否打过
bool SGStorySectionLayer::getBossSectionBeaten(int storyId)
{
	char storyIdKey[32] = "\0";
	sprintf(storyIdKey, "%s%d", SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString(), storyId);
	
	return CCUserDefault::sharedUserDefault()->getBoolForKey(storyIdKey);
}

void SGStorySectionLayer::sendRewardRequest(int storyId)
{
    main::StoryRewardRequest *request = new main::StoryRewardRequest();
    request->set_storyid(storyId);
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_XINREWARD, request);
}


#pragma mark -
#pragma mark - -----tableView actions-----
SNSTableViewCellItem* SGStorySectionLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    DDLog("path2=================%d, %d, %d", indexPath->getColumn(), indexPath->getRow(), indexPath->getSection());
	SGStorySectionCell *item = (SGStorySectionCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGStorySceneBean *sceneBean = (SGStorySceneBean *)datas->objectAtIndex(index);
	
	
	//检测每一个boss关,获取boss关的方法: 获取本章内所有含有的节的数量,当前节的id等于章的数,这节就是boss关
	int beanCount = storyBean->getStorySections()->count();//节的数量
	int currScenceId = sceneBean->getSceneId();
	
	//现在开启的关卡数量
	int storyCount = 0;
	if (SGPlayerInfo::sharePlayerInfo()->getStoryData())
	{
		storyCount = SGPlayerInfo::sharePlayerInfo()->getStoryData()->count();
	}
	//做弹出提示的逻辑
	if (currScenceId == beanCount && storyBean->getStoryId() <= 15
        && storyBean->getStoryId() != 1 && storyBean->getStoryId() != 6
		&& storyBean->getStoryId() != 4 && !this->getBossSectionBeaten(storyBean->getStoryId())
        && sceneBean->getRank() != 0 && (storyBean->getStoryId() == storyCount - 1)
        && SGGuideManager::shareManager()->getLimitGuideSize() <= 0)
		//则本关是BOSS关, 不是引导关,不是第十关, 而且是15关之前, 之前也没有打过这一关, 并且还没有进行评级,且当前关不是最新的关
	{
		//本章当前获得的星星数
		int beanCurrStars = 0;
		for (int i = 0; i < datas->count(); i++)
		{
			beanCurrStars += static_cast<SGStorySceneBean *>(datas->objectAtIndex(i))->getRank();
		}
		//本章星星总数
		int beanTotalStars = 3 * beanCount;
		
		
		SGTipsBox *box = NULL;
		//弹出提示 popTips
		if (beanCurrStars == beanTotalStars)//可以得到奖励
		{
			//标题: 新关卡开启
			//您已经胜利闯过此章，并获得了全部的星星，是否立即领取通关奖励，并开始挑战下一章节？
			box = SGTipsBox::create(this, true, sceneBean->getStoryId());
			
			isPopRewardWindow = true;//不弹出领奖励的框
		}
		else//不能获取奖励
		{
			//提示框标题：新关卡开启
			//提示框内容：您已经胜利闯过此章，新的关卡已经开启。是否立即去挑战？
			//（小提示：如果您满星通过本章的所有章节，将会获得丰厚的通关奖励哟）
			box = SGTipsBox::create(this, false);
		}
		SGMainManager::shareMain()->showBox(box);
		
		//标记为已经打过
		char storyIdKey[32] = "\0";
		sprintf(storyIdKey, "%s%d", SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString(),storyBean->getStoryId());
		CCUserDefault::sharedUserDefault()->setBoolForKey(storyIdKey, true);
	}
				
	
	
	// 自动奖励被上面的替代,自动奖励暂时保留 *_& by bugcode
	/*
	
	//3.05
	//奖励框自动弹出,是奖励框,并且还是没有领取过的情况下
	if (sceneBean->getExp() == STARWARDTAG && sceneBean->getOpenState() == 1) {
		//领奖
        if (sceneBean->getStoryId() < AUTOREWARDID && sceneBean->getStoryId() > 1) {
            main::StoryRewardRequest *request = new main::StoryRewardRequest();
            request->set_storyid(sceneBean->getStoryId());
            SGSocketClient::sharedSocketClient()->send(MSG_STORY_XINREWARD, request);
			isAutoReward = -1;
        }

	}
	*/
	
    if (item == NULL) {
        item = SGStorySectionCell::create(this,sceneBean);
        //item->setTag(index);
    }
    if (index < datas->count()) {
        item->setVisible(sceneBean->getVisible());
        item->setData(sceneBean, datas);
        //item->setTag(index);
        item->resetBgImg(index);
    }
    else
    {
        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGStorySectionLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    if (datas == NULL || datas->data == NULL) {
        return 0;
    }
    return datas->count();
}

void SGStorySectionLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
     
    EFFECT_PLAY(MUSIC_BTN);
    //CCLOG("-----test tableBViewSelect ");
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);

     CCLOG("debug == || selectIndex == %d", selectIndex);

	SGStorySectionCell *item = (SGStorySectionCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
	SGStorySceneBean *sceneBean = (SGStorySceneBean *)datas->objectAtIndex(selectIndex);

	CCLOG("debug == || storySecenBean->plotId == %d", sceneBean->getPlotId());

	if (sceneBean->getStoryId() >= 20000)
	{
		item->setData(sceneBean, datas);
	}
    storySecenId = ((SGStorySceneBean *)datas->objectAtIndex(selectIndex))->getPlotId();
}

void SGStorySectionLayer::gotoFirendRequest()
{
    int num = 0;
    if (datas && datas->data && datas->data->arr)
    {
        num = datas->count();
    }
    
    SGMainManager::shareMain()->setChap2Only1(num);
    main::CooperationRequest *request = new main::CooperationRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_FINDHELPER, request);
}

void SGStorySectionLayer::gotoPurchPower()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        //有引导时不弹购买提示
        return;
    }
    main::ShopPurchPvEPowerEntryRequest *request = new main::ShopPurchPvEPowerEntryRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_PURCHPVEPOWE_RENTER, request);
}
void SGStorySectionLayer::rewardSuccess(CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    main::StoryRewardResponse *response = (main::StoryRewardResponse *)sr->m_msg;
    if (response)
    {
        if (response->state() == 1)
        {
            EFFECT_PLAY(MUSIC_34);
            if (storyBean)
            {
                storyBean->setCanReward(0);
            }
            SGStorySceneBean *scene = NULL;
            CCObject *child = NULL;
            CCARRAY_FOREACH(datas, child)
            {
                scene = (SGStorySceneBean *)child;
                if (scene->getExp() == STARWARDTAG)
				{
                    scene->setOpenState(0);
                    break;
                }
            }
            SGStorySectionCell *item = (SGStorySectionCell*)tableView->dequeueReusableCellItemForIndexPath(SNSIndexPath::create(0, 0, 1));
            if (item->getGift() == scene) {
                item->getGift()->setOpenState(0);
                item->backBtn->setEnabled(false);
                item->backBtn->removeChildByTag(SECTIONCELLEFFECTAG);
                item->backBtn->setImageWithFrame("reward_font_ylq.png");
            }
            
            
			if (!isPopRewardWindow)
			{
				//领奖弹出框
				showRewardBox();
			}
			else
			{
				this->backHandler(this); //前15关不弹出领取奖励的框
			}
        
        }
        else if( 0 == response->state()) //星星不够
        {
            SGMainManager::shareMain()->showMessage(str_StorySectionCell_str10);
        }
        else if( 2 == response->state())
        {
            SGMainManager::shareMain()->showMessage(str_StorySectionCell_str23);
        }
        else
        {
            CCAssert(false, "状态错误，请检查。");
        }
    }
}

void SGStorySectionLayer::showHelperLayer(CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    main::CooperationResponse *response = (main::CooperationResponse *)sr->m_msg;
    if (response)
    {
        if (selectIndex >= 0)
        {
            int size = response->cooperationmodel_size();
            if (size != 0)
            {
                CCArray *array = CCArray::create();
                array->retain();
                for (int ii = 0; ii < size; ii++)
                {
                    main::CooperationModel model = response->cooperationmodel(ii);
                    int var1 = model.roleid();
                    int var3 = model.type();
                    int var4 = model.itemid();
                    int var6 = model.itemlevel();
                    int var8 = model.rolelv();
					int isInMyFriendList = model.inlist();
					
                    SGOfficerDataModel *itemModel = SGStaticDataManager::shareStatic()->getOfficerById(var4);
                    if (itemModel)
                    {
                        int var7 = itemModel->getOfficerCurrStarLevel();
                        const char* var2 = model.name().c_str();
                        const char* var5 = itemModel->getOfficerName()->getCString();
                        
                        SGPveHelper *helper = SGPveHelper::create(var1, var2, var3, var4, var5, var6, var7, var8, isInMyFriendList);
                        array->addObject(helper);
                    }
                }


                SGMainManager::shareMain()->showFindHelpLayer(storySecenId, array);
                
                //如果只有2-1,
                SGGuideManager::shareManager()->setLimitGuideTag(SGPlayerInfo::sharePlayerInfo()->checkStartLimitGuide(5,storySecenId));
                if (SGMainManager::shareMain()->getChap2Only1() == 1)
                {
                    SGGuideManager::shareManager()->startLimitGuide();//告知
                }
                
            }
            else
            {
                CCLOG("!!!++++++++++++++++ no helpers");
            }
        }
    }
    else
    {
        // get helper error
    }
}

void SGStorySectionLayer::setIsCanTouch(bool isTouch)
{
    tableView->setIsTouchEnabled(isTouch);
    menu->setEnabled(isTouch);
}

int SGStorySectionLayer::getStorySectionCount(void)
{
    if (datas) {
        return datas->count();
    }
    return 0;
}

void SGStorySectionLayer::setSectionFirstInfo(SGStorySectionCell *section)
{
    if (section == NULL) {
        return;
    }
    creatSectionInfoView(section);

    if (sectiontableView) {
        selectIndex = 0;
        m_lastShowSection = section;
        SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
        SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
        CCPoint selectViewCellPos = tableViewCell->getPosition();
        float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
            
        sectiontableView->setVisible(true);
        CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
        lastSelectIndex = selectIndex;
        lastMoveDis = moveDis;
        float posX = (section->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
        if(moveDis<0)
        {
            sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
            
            SNSTableViewCell *cell = NULL;
            for (int ii = 1; ii < tableView->getRowCount(); ii++) {
                cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
                if (cell != NULL) {
                    cell->stopAllActions();
                    cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,-sectiontableView->getContentSize().height)));
                }
            }
        }
        
        CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
        sectionInfoView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        resetMovingSectiontableView();
        tableView->resetDownLimit = sectionInfoView->getContentSize().height;
        sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
    }
}


void SGStorySectionLayer::showSectionInfoHandler(SGStorySectionCell *section)
{
    if (isMovingSection || section == NULL) {
        return;
    }
    creatSectionInfoView(section);

    if (sectiontableView) {
        
        if (sectiontableView->isVisible())
        {
            if (m_lastShowSection == section) {
                if (lastMoveDis>0) {
                    tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                }
                else{
                    tableView->moveCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                }
                tableView->resetDownLimit = 0;
                moveSectiontableViewDown(false);
                lastMoveDis = 0;
                lastSelectIndex = 0;
            }
            else
            {
                CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
                sectiontableView->stopAllActions();
                sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height+4));
                hideSectiontableView();
                
                if (lastMoveDis>0) {
                    tableView->setCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                }
                else{
                    tableView->setCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                }
                
                tableView->resetDownLimit = 0;
                lastMoveDis = 0;
                lastSelectIndex = 0;
                
                SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
                SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
                CCPoint selectViewCellPos = tableViewCell->getPosition();
                float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
                
                sectiontableView->setVisible(true);
                CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
                lastSelectIndex = selectIndex;
                lastMoveDis = moveDis;
                float posX = (section->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
                if(moveDis>0)
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
                    tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
                }
                else
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                    tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
                }
                tableView->resetDownLimit = sectiontableView->getContentSize().height;
                moveSectiontableViewDown(true);
                sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
                
            }
        }
        else
        {
            SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
            SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
            CCPoint selectViewCellPos = tableViewCell->getPosition();
            float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
            
            sectiontableView->setVisible(true);
            CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
            lastSelectIndex = selectIndex;
            lastMoveDis = moveDis;
            float posX = (section->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
            if(moveDis>0)
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
                tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
            }
            else
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
            }
            tableView->resetDownLimit = sectiontableView->getContentSize().height;
            moveSectiontableViewDown(true);
            sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
        }
        
        m_lastShowSection = section;
    }
}

void SGStorySectionLayer::moveSectiontableViewDown(bool isDown)
{
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    sectionInfoView->stopAllActions();
    
    if (isDown)
    {
        sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height));
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME,CCPointZero));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGStorySectionLayer::resetMovingSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    else
    {
        sectiontableView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME, ccp(0,sectiontableView->getContentSize().height+4)));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGStorySectionLayer::hideSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    isMovingSection = true;
}

void SGStorySectionLayer::hideSectiontableView()
{
    if (sectiontableView != NULL) {
        sectiontableView->setVisible(false);
        isMovingSection = false;
    }
}

void SGStorySectionLayer::resetMovingSectiontableView()
{
    if (sectiontableView != NULL) {
        isMovingSection = false;
    }
}

void SGStorySectionLayer::creatSectionInfoView(SGStorySectionCell *section)
{
    SGStorySceneBean *storyBean = (SGStorySceneBean *)datas->objectAtIndex(selectIndex);
    if (storyBean == NULL) {
        return;
    }
    int itemNum = storyBean->itemIds.size();
    CCSprite *sectionInfoView = NULL;
    //是否是通关奖励
    bool isStarReward = (storyBean->getExp() == STARWARDTAG);
    float contentHeight = 0;
    float fontWid = 24;//FONTSIZE(SECTIONINFOFONTSIZE);
    float fontWidNew = 22;
    float winRate = 640/size.width;
    
	
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer, LIM_PNG_AS_PNG);
    
    
    if (sectiontableView == NULL)
    {
        if (isStarReward) {
            sectionInfoView = CCSprite::createWithSpriteFrameName("yban.png");
        }
        else
        {
            sectionInfoView = CCSprite::createWithSpriteFrameName("bban.png");
        }
        sectionInfoView->setAnchorPoint(CCPointZero);
        sectionInfoView->ignoreAnchorPointForPosition(false);
        sectionInfoView->setPosition(ccp((section->bgSize.width - sectionInfoView->getContentSize().width)/2,150));
        sectionInfoView->setOpacity(255*0.6);
        sectiontableView = new NewScrollView();
        if (sectiontableView && sectiontableView->initGSScrollViewFrame(CCRectMake(0, 0, section->bgSize.width, sectionInfoView->getContentSize().height)))
        {
            sectiontableView->autorelease();
            CCPoint tableViewPos = tableView->getParent()->convertToWorldSpace(tableView->getPosition());
             sectiontableView->setParentScissorFrame(CCRectMake(tableViewPos.x, tableViewPos.y, tableView->getScissorFrame().size.width, tableView->getScissorFrame().size.height));
        }
        
        sectiontableView->setAnchorPoint(CCPointZero);
        sectiontableView->ignoreAnchorPointForPosition(false);
        sectiontableView->addChild(sectionInfoView,1,SECTIONINFOVIEWTAG);

        contentHeight = sectionInfoView->getContentSize().height;
        if (storyBean->itemIds.size() <1 && !isStarReward) {
            contentHeight *= 0.7;
        }
        else if (isStarReward) {
            contentHeight *= 0.9;
        }
        SGCCLabelTTF* infoLabel1 = NULL;
        SGCCLabelTTF* infoLabel2 = NULL;
        SGCCLabelTTF* infoLabel3 = NULL;
        CCSprite*   infoSprite = NULL;
        
        infoLabel1 = SGCCLabelTTF::create(str_StorySectionCell_str11, FONT_BOXINFO, fontWid , ccYELLOW);
        infoLabel1->setPosition(ccp(30,contentHeight- SECTIONCELLGAP));
        infoLabel1->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel1,1,-9);
        
        infoLabel2 = SGCCLabelTTF::create(CCString::createWithFormat(" %d",storyBean->getPower())->getCString(), FONT_PANGWA, fontWidNew);
        infoLabel2->setPosition(ccp(30 + fontWid*4.5,contentHeight- SECTIONCELLGAP));
        infoLabel2->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel2,1,-10);
        
        infoLabel3 = SGCCLabelTTF::create(str_StorySectionCell_str12, FONT_BOXINFO, fontWid , ccYELLOW);
        infoLabel3->setPosition(ccp(260,contentHeight - SECTIONCELLGAP));
        infoLabel3->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel3,1,-19);
        
        infoLabel2 = SGCCLabelTTF::create(CCString::createWithFormat(" %d",storyBean->getZhandou())->getCString(), FONT_PANGWA, fontWidNew);
        infoLabel2->setPosition(ccp(260 + fontWid*5.5,contentHeight- SECTIONCELLGAP));
        infoLabel2->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel2,1,-20);
        
        infoLabel1 = SGCCLabelTTF::create(str_StorySectionCell_str13, FONT_BOXINFO, fontWid ,ccYELLOW);
        infoLabel1->setPosition(ccp(30,contentHeight - SECTIONCELLGAP -30));
        infoLabel1->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel1,1,-28);
        
        infoSprite = CCSprite::createWithSpriteFrameName("expFont.png");
        infoSprite->setPosition(ccp(30 + fontWid*4.5,contentHeight- SECTIONCELLGAP - 26));
        infoSprite->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoSprite,1,-29);
        
        infoLabel2 = SGCCLabelTTF::create(CCString::createWithFormat("  %d",storyBean->getExp())->getCString(), FONT_PANGWA, fontWidNew);
        infoLabel2->setPosition(ccp(30+fontWid*7,contentHeight- SECTIONCELLGAP-30));
        infoLabel2->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel2,1,-30);
        
        infoSprite = CCSprite::createWithSpriteFrameName("coinicon.png");
        infoSprite->setPosition(ccp(260 +fontWid*3.5,contentHeight- SECTIONCELLGAP - 30));
        infoSprite->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoSprite,1,-39);

        infoLabel2 = SGCCLabelTTF::create(CCString::createWithFormat("   %d",storyBean->getGold())->getCString(), FONT_PANGWA, fontWidNew);
        infoLabel2->setPosition(ccp(260+fontWid*5,contentHeight - SECTIONCELLGAP-30));
        infoLabel2->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel2,1,-40);
		
		//精英副本加入二个上限显示
		int shift = 0;
		if (storyBean->getStoryId() >= 20000)
		{
			buyTimes = storyBean->getCreamBuyTimes();
			shift = -50;
			SGCCLabelTTF *info = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str14, buyTimes)->getCString(), FONT_BOXINFO, fontWid , ccYELLOW);
			info->setAnchorPoint(ccp(0, 0));
            info->setPosition(ccp(30,contentHeight - SECTIONCELLGAP*2 - 20));
			sectionInfoView->addChild(info, 1, -88);

		}
        
        infoLabel1 = SGCCLabelTTF::create(str_StorySectionCell_str15, FONT_BOXINFO, fontWid , ccYELLOW);
        infoLabel1->setPosition(ccp(30,contentHeight - SECTIONCELLGAP*2+shift));
        infoLabel1->setAnchorPoint(CCPointZero);
        infoLabel1->setVisible(itemNum>0?true:false);
        sectionInfoView->addChild(infoLabel1,1,-41);
        
        if (storyBean->getStoryId() >= 20000) {
            infoLabel1->setPosition(ccp(30,contentHeight - SECTIONCELLGAP*2+shift));
        }
        else
        {
            infoLabel1->setPosition(ccp(30,contentHeight - SECTIONCELLGAP*2-20+shift));
        }
        
        
        infoLabel1 = SGCCLabelTTF::create(str_StorySectionCell_str16, FONT_BOXINFO, FONTSIZE(18*winRate) , ccc3(0xff, 0xc2, 0x00));
        infoLabel1->setPosition(ccp(20,contentHeight- SECTIONCELLGAP));
        infoLabel1->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel1,1,STARWARDTAG - 1);
        
        infoSprite = CCSprite::createWithSpriteFrameName("xx.png");
        infoSprite->setPosition(ccp(20 +FONTSIZE(18)*4*winRate,contentHeight- SECTIONCELLGAP-0.16));
        infoSprite->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoSprite,1,STARWARDTAG - 2);
        
        infoLabel3 = SGCCLabelTTF::create(CCString::createWithFormat("%d",storyBean->getZhandou())->getCString(), FONT_BOXINFO, FONTSIZE(18*winRate));
        infoLabel3->setPosition(ccp(20+FONTSIZE(18)*5.6*winRate,contentHeight - SECTIONCELLGAP + 3));
        infoLabel3->setAnchorPoint(CCPointZero);
        sectionInfoView->addChild(infoLabel3,1,STARWARDTAG - 3);
        
        
        sectionInfoView->getChildByTag(-9)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-10)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-19)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-20)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-28)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-29)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-30)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-39)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-40)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-41)->setVisible((!isStarReward) && (itemNum>0?true:false));
        
        sectionInfoView->getChildByTag(STARWARDTAG - 1)->setVisible(isStarReward);
        sectionInfoView->getChildByTag(STARWARDTAG - 2)->setVisible(isStarReward);
        sectionInfoView->getChildByTag(STARWARDTAG - 3)->setVisible(isStarReward);

        // 0 武将 1 装备 2 士兵 3 道具
        addSectionItem(sectiontableView->getContentSize().height,isStarReward);

        sectiontableView->setVisible(false);
        tableView->m_body->addChild(sectiontableView);
        
        
    }
    else
    {
        sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
        contentHeight = sectiontableView->getContentSize().height;
        if (storyBean->itemIds.size() <1 && !isStarReward) {
            contentHeight *= 0.7;
        }
        else if (isStarReward) {
            contentHeight *= 0.9;
        }
        
        sectionInfoView->getChildByTag(-9)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-10)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-19)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-20)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-28)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-29)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-30)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-39)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-40)->setVisible(!isStarReward);
        sectionInfoView->getChildByTag(-41)->setVisible((!isStarReward) && (itemNum>0?true:false));
        
        sectionInfoView->getChildByTag(STARWARDTAG - 1)->setVisible(isStarReward);
        sectionInfoView->getChildByTag(STARWARDTAG - 2)->setVisible(isStarReward);
        sectionInfoView->getChildByTag(STARWARDTAG - 3)->setVisible(isStarReward);
        
        if (isStarReward) {
            sectionInfoView->setDisplayFrame(CCSprite::createWithSpriteFrameName("yban.png")->displayFrame());
            ((SGCCLabelTTF*)sectionInfoView->getChildByTag(STARWARDTAG - 3))->setString(CCString::createWithFormat("%d",storyBean->getZhandou())->getCString());
            
            sectionInfoView->getChildByTag(STARWARDTAG - 1)->setPosition(ccp(20,contentHeight- SECTIONCELLGAP));
            sectionInfoView->getChildByTag(STARWARDTAG - 2)->setPosition(ccp(20 +FONTSIZE(18)*4*winRate,contentHeight- SECTIONCELLGAP-0.16));
            sectionInfoView->getChildByTag(STARWARDTAG - 3)->setPosition(ccp(20+FONTSIZE(18)*5.6*winRate,contentHeight - SECTIONCELLGAP + 3));
        }
        else
        {
			int shift = 0;
			if (storyBean->getStoryId() >= 20000)
			{
				shift = -30;
				buyTimes = storyBean->getCreamBuyTimes();
				((SGCCLabelTTF *)sectionInfoView->getChildByTag(-88))->setString(CCString::createWithFormat(str_StorySectionCell_str14, buyTimes)->getCString());
			}
            sectionInfoView->setDisplayFrame(CCSprite::createWithSpriteFrameName("bban.png")->displayFrame());
            
            ((SGCCLabelTTF*)sectionInfoView->getChildByTag(-10))->setString(CCString::createWithFormat(" %d",storyBean->getPower())->getCString());
            ((SGCCLabelTTF*)sectionInfoView->getChildByTag(-20))->setString(CCString::createWithFormat(" %d",storyBean->getZhandou())->getCString());
            ((SGCCLabelTTF*)sectionInfoView->getChildByTag(-30))->setString(CCString::createWithFormat("  %d",storyBean->getExp())->getCString());
            ((SGCCLabelTTF*)sectionInfoView->getChildByTag(-40))->setString(CCString::createWithFormat("   %d",storyBean->getGold())->getCString());
            
            sectionInfoView->getChildByTag(-9)->setPosition(ccp(30,contentHeight- SECTIONCELLGAP));
            sectionInfoView->getChildByTag(-10)->setPosition(ccp(30 + fontWid*4.5,contentHeight- SECTIONCELLGAP));
            sectionInfoView->getChildByTag(-19)->setPosition(ccp(260,contentHeight - SECTIONCELLGAP));
            sectionInfoView->getChildByTag(-20)->setPosition(ccp(260 + fontWid*5.5,contentHeight- SECTIONCELLGAP));
            
            sectionInfoView->getChildByTag(-28)->setPosition(ccp(30,contentHeight - SECTIONCELLGAP -30));
            sectionInfoView->getChildByTag(-29)->setPosition(ccp(30 + fontWid*4.5,contentHeight- SECTIONCELLGAP -26));
            sectionInfoView->getChildByTag(-30)->setPosition(ccp(30+fontWid*7,contentHeight- SECTIONCELLGAP-30));
            sectionInfoView->getChildByTag(-39)->setPosition(ccp(260+fontWid*3.5,contentHeight- SECTIONCELLGAP-30));
            sectionInfoView->getChildByTag(-40)->setPosition(ccp(260+fontWid*5,contentHeight - SECTIONCELLGAP-30));
            sectionInfoView->getChildByTag(-41)->setPosition(ccp(30,contentHeight - SECTIONCELLGAP*2-20+shift));
        }
        // 0 武将 1 装备 2 士兵 3 道具
        addSectionItem(sectiontableView->getContentSize().height,isStarReward);
    }
}

void SGStorySectionLayer::addSectionNormalItem(float contentHeight)
{
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    SGMainManager * sgMainManager = SGMainManager::shareMain();
    CCSprite* itemIcon = NULL;
    CCSprite* itemBG = NULL;
    SGCCLabelTTF* infoLabel = NULL;
    int itemId = 0;
    SGStorySceneBean *storyBean = (SGStorySceneBean *)datas->objectAtIndex(selectIndex);
	int shift = 0;
	if (storyBean->getStoryId() >= 20000)
	{
		shift = 15;
	}
    int itemNum = storyBean->itemIds.size();
    CCString* itemName = NULL;
    SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    float posX = 0;
    for (int i = 0; i<5 && i<itemNum; i++) {
        itemId = storyBean->itemIds[i];
		
        
        //材料
        if (itemId >= 400000)
        {
            SGMaterialDataModel *material = SGStaticDataManager::shareStatic()->getMaterialById(itemId);
            SGMainManager::shareMain()->addHeadIconPlistByNum(material->getMaterialIconId(), sg_storySectionLayer);
            int startLvl = material->getMaterialBorderColor();
            if (startLvl > 1)
            {
                startLvl -= 1;
            }
            itemBG = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", startLvl)->getCString());
            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png", material->getMaterialIconId())->getCString());
            itemName = material->getMaterialName();
        }
        else if (itemId >= 350000)//消耗品
		{
			SGConsumableDataModel *con = staticDataManager->getConsumableById(itemId);
            int starLv = con->getConsumeStarLvl();
            SGMainManager::shareMain()->addHeadIconPlistByNum(con->getIconId(), sg_storySectionLayer);
            if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
            {
                starLv -= 1;
            }
            
			itemBG = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", starLv)->getCString());
            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png", con->getIconId())->getCString());
            itemName = con->getConsumeName();
			
		}
		else if (itemId >= 300000)//碎片
		{
			SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(itemId);
            int starLv = pieces->getStarLevel();
            if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
            {
                starLv -= 1;
            }
            
			if (pieces->getCallingCardId() == -1)//如果是通用碎片
			{
				itemBG  = CCSprite::createWithSpriteFrameName("common_pieces_border.png");
			}
			else if (pieces->getPiecesType() == 1)//装备碎片
			{
				itemBG  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", starLv)->getCString());
			}
			else if (pieces->getPiecesType() == 0)//武将碎片
			{
				itemBG  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starLv)->getCString());
			}
			if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
			{
				CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
				mask->setPosition(ccp(itemBG->getContentSize().width / 2, itemBG->getContentSize().height / 2));
				itemBG->addChild(mask);
			}
			
		}
        else if (itemId > 4000) {
            itemBG  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",staticDataManager->getEquipById(itemId)->getEquipStarPinzhi())->getCString());
            if (staticDataManager->getEquipById(itemId)->getAdvNum() > 0) {
                SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",staticDataManager->getEquipById(itemId)->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advNum->setInsideColor(ccGREEN);
                advNum->setAnchorPoint(ccp(1,1));
                advNum->ignoreAnchorPointForPosition(false);
                advNum->setPosition(ccp(itemBG->getContentSize().width*0.8,itemBG->getContentSize().height - advNum->getContentSize().height));
                itemBG->addChild(advNum,40,40);
            }

            
        }
        else if (itemId >1000){
            int starlvl = staticDataManager->getOfficerById(itemId)->getOfficerCurrStarLevel();
            if (starlvl>1) {
                starlvl-=1;
            }
            itemBG  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
            //                itemBG  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",staticDataManager->getOfficerById(itemId)->getOfficerVisitType())->getCString());
            if (itemBG) {
                SGOfficerDataModel * temp = staticDataManager->getOfficerById(itemId);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(itemBG->getContentSize().width*0.35,itemBG->getContentSize().height - countryName->getContentSize().height));
                itemBG->addChild(countryName,30,30);
                
                if (temp->getAdvNum() > 0) {
                    SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                    advNum->setInsideColor(ccGREEN);
                    advNum->setAnchorPoint(ccp(1,1));
                    advNum->ignoreAnchorPointForPosition(false);
                    advNum->setPosition(ccp(itemBG->getContentSize().width*0.8,itemBG->getContentSize().height - advNum->getContentSize().height));
                    itemBG->addChild(advNum,40,40);
                }
            }
        }
        else
        {
             SGPropsDataModel *card =  SGStaticDataManager::shareStatic()->getPropsById(itemId);
            if (card != NULL && card->getPropsPropsType() == 3)
            {
                 //兵魂卡
                int starlvl = card->getPropsCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                itemBG  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
            }
            else
            {
                if (card->getPropsCountry()) {
                    int starlvl = card->getPropsCurrStarLevel();
                    if (starlvl>1) {
                        starlvl-=1;
                    }
                    CCString* str = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
                    
                    itemBG = CCSprite::createWithSpriteFrameName(str->getCString());
                    
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",card->getPropsCountry()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(itemBG->getContentSize().width*0.35,
												 itemBG->getContentSize().height - countryName->getContentSize().height));
                    itemBG->addChild(countryName,30,30);
                }
				else//此处忘记了经验石,边框按装备的
				{
					int starLvl = card->getPropsCurrStarLevel();
					if (starLvl > 1)
						starLvl -= 1;
					
					CCString *border = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl);
					
					itemBG = CCSprite::createWithSpriteFrameName(border->getCString());
				}

            }
            
        }
        if (itemBG == NULL) {
            return;
        }
        
        //材料
        if (itemId >= 400000)
        {
            SGMaterialDataModel *material = SGStaticDataManager::shareStatic()->getMaterialById(itemId);
            SGMainManager::shareMain()->addHeadIconPlistByNum(material->getMaterialIconId(), sg_storySectionLayer);
            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png", material->getMaterialIconId())->getCString());

        }
        else if(itemId >= 350000)
        {
        }
		else if (itemId >= 300000)//如果是碎片，特殊处理
		{
			int iconId = -1;
			SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(itemId);
			
			if (temp->getPiecesType() == 0)//武将
			{
				SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(temp->getCallingCardId());
				iconId = of->getIconId();
			}
			else if (temp->getPiecesType() == 1)//装备
			{
				SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(temp->getCallingCardId());
				iconId = eq->getIconId();
			}
			
			sgMainManager->addHeadIconPlistByNum(iconId,sg_storySectionLayer);
			itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
		}
		else if (itemId > 4000)//装备
		{
			SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(itemId);
			
			sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_storySectionLayer);
			itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
		}
		else if (itemId > 1000)//武将
		{
			SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
			sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_storySectionLayer);
			itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
			
		}
		else if (itemId > 0 && itemId <= 1000)
		{
			SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(itemId);
			sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_storySectionLayer);
			
			itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
		}
		else
		{
			sgMainManager->addHeadIconPlistByNum(itemId,sg_storySectionLayer);
			
			itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",itemId)->getCString());
		}
        
        if (itemIcon) {
            itemIcon->setAnchorPoint(ccp(0.5,0.5));
            itemIcon->ignoreAnchorPointForPosition(false);
            itemIcon->setPosition(ccp(itemBG->getContentSize().width/2,itemBG->getContentSize().height/2));
            itemBG->addChild(itemIcon,-1);
        }
        
		//材料
        if (itemId >= 400000)
        {
            SGMaterialDataModel *material = SGStaticDataManager::shareStatic()->getMaterialById(itemId);
            itemName = material->getMaterialName();

        }else if(itemId >= 350000)
        {
        }

		else if (itemId >= 300000) {//碎片显示名称
			itemName = SGStaticDataManager::shareStatic()->getPiecesDictById(itemId)->getPiecesName();
		}
        else if (itemId > 4000) {
            itemName = SGStaticDataManager::shareStatic()->getEquipById(itemId)->getEquipName();
        }
        else if (itemId >1000){
            itemName = SGStaticDataManager::shareStatic()->getOfficerById(itemId)->getOfficerName();
        }
        else {
            itemName = SGStaticDataManager::shareStatic()->getPropsById(itemId)->getPropsName();
        }
		
		int upShift = 0;
		if (CCEGLView::sharedOpenGLView()->getFrameSize().width == iphonehdResource.size.width)
		{
			upShift = -15;
		}
        else
        {
            upShift = -10;
        }
        std::string itemMsg(itemName->getCString());
        if(itemMsg.find("(")== std::string::npos)
        {
            infoLabel = SGCCLabelTTF::create(itemName->getCString(), FONT_BOXINFO, 20,CCSizeMake(itemBG->getContentSize().width*0.9, 0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
            
            infoLabel->setPosition(ccp(itemBG->getContentSize().width /2,shift+22 -infoLabel->getContentSize().height/2 + upShift));
            itemBG->addChild(infoLabel);
        }
        else
        {
			
            std::string itemName1 = itemMsg.substr(0,itemMsg.find_first_of("("));
            infoLabel = SGCCLabelTTF::create(itemName1.c_str(), FONT_BOXINFO, 20);
            infoLabel->setPosition(ccp(itemBG->getContentSize().width/2, -12 + shift+22 + upShift));
            itemBG->addChild(infoLabel);
            
            std::string itemName2 = itemMsg.substr(itemMsg.find_first_of("("),itemMsg.find_last_of(")") - itemMsg.find_first_of("(")+1);
            infoLabel = SGCCLabelTTF::create(itemName2.c_str(), FONT_BOXINFO, 20);
            infoLabel->setPosition(ccp(itemBG->getContentSize().width/2,-12 -infoLabel->getContentSize().height + shift+24 + upShift));
            itemBG->addChild(infoLabel);
            
        }
        itemBG->setScale(0.8);
        if (i == 0) {
            posX = itemBG->getContentSize().width*0.5;
        }
        CCSprite *cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
        cardBg->setPosition(ccp(itemBG->getContentSize().width/2, itemBG->getContentSize().height/2));
        itemBG->addChild(cardBg,-10,-10);
        itemBG->setPosition(ccp(posX,contentHeight - SECTIONCELLGAP*4 - shift));
        sectionInfoView->addChild(itemBG,9,-200+i);
        posX += itemBG->getContentSize().width * 0.7;
    }
    //居中设置
//    if (itemNum < 5) {
//        CCNode* item = NULL;
//        float itemsWid = posX - 0.2*posX/(0.7*itemNum -0.2);
//        float cellwid = sectionInfoView->getContentSize().width;
//        float addX = (cellwid - itemsWid)/2;
//        
//        for (int i = 0; i<itemNum; i++) {
//            item = sectionInfoView->getChildByTag(-200+i);
//            item->setPositionX(item->getPositionX() + addX);
//        }
//    }

}

void SGStorySectionLayer::addSectionStarItem(float contentHeight)
{
    SGRewardDataModel* rewardModel = SGStaticDataManager::shareStatic()->getRewardById(storyBean->getRewardId());
    if (rewardModel == NULL) {
        return;
    }
    
    int type = 0;
    int value = 0;
    int number = 0;
    CCSprite* itemIcon = NULL;
    CCSprite *jinkuang = NULL;
    SGCCLabelTTF * itemName = NULL;
    CCSprite *cardBg = NULL;
    SGMainManager * sgMainManager = SGMainManager::shareMain();
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    float posX = 0;
    int rewardCount = rewardModel->rewardArray->count();
    for(int i=0; i<rewardCount && i<4; i++)
    {
        //type 、id 、number
        sscanf(((CCString*)rewardModel->rewardArray->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
        SGBaseItemType enumType = (SGBaseItemType)type;
        switch (enumType)
        {
            case BIT_INGOT:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("big_yb.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_yuanbao,number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_GOLD:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                sgMainManager->addHeadIconPlistByNum(9999,sg_storySectionLayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_tongMoney,number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_GRADE:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                sgMainManager->addHeadIconPlistByNum(9999,sg_storySectionLayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_pvpScore,number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_FEAT:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                sgMainManager->addHeadIconPlistByNum(9997,sg_storySectionLayer);
                jinkuang = CCSprite::createWithSpriteFrameName("jinkuang.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9997.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_jungong,number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_HORN:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                sgMainManager->addHeadIconPlistByNum(9998,sg_storySectionLayer);
                jinkuang = CCSprite::createWithSpriteFrameName("jinkuang.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_smalll_laba,number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_RECIPE:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                sgMainManager->addHeadIconPlistByNum(9998,sg_storySectionLayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_liandian,number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_OFFICER:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(value);
                sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_storySectionLayer);
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                jinkuang->addChild(countryName,30,30);
                
                if (temp->getAdvNum() > 0) {
                    SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                    advNum->setInsideColor(ccGREEN);
                    advNum->setAnchorPoint(ccp(1,1));
                    advNum->ignoreAnchorPointForPosition(false);
                    advNum->setPosition(ccp(jinkuang->getContentSize().width*0.8,jinkuang->getContentSize().height - advNum->getContentSize().height));
                    jinkuang->addChild(advNum,40,40);
                }

            }
                break;
            case BIT_EQUIP:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_storySectionLayer);
               
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                if (temp->getAdvNum() > 0) {
                    SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                    advNum->setInsideColor(ccGREEN);
                    advNum->setAnchorPoint(ccp(1,1));
                    advNum->ignoreAnchorPointForPosition(false);
                    advNum->setPosition(ccp(jinkuang->getContentSize().width*0.8,jinkuang->getContentSize().height - advNum->getContentSize().height));
                    jinkuang->addChild(advNum,40,40);
                }

            }
                break;
            case BIT_PROP:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                sgMainManager->addHeadIconPlistByNum(value,sg_storySectionLayer);
                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(value);
                int starLev = temp->getPropsCurrStarLevel();
                starLev = starLev>0 ? (starLev-1):0;
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png" , starLev)->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",value)->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
                //碎片
            case BIT_PIECE:
            {
                ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(value);
                SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(temp->getCallingCardId());
                SGMainManager::shareMain()->addHeadIconPlistByNum(of->getIconId(),sg_storySectionLayer);
                
                int starLev = temp->getStarLevel();
                starLev = starLev>0 ? (starLev-1):0;
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starLev)->getCString());
                CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                mask->setPosition(ccp(cardBg->getContentSize().width/2,cardBg->getContentSize().height/2));
                cardBg->addChild(mask , 10);
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",of->getIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPiecesName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
                //军魂
            case BIT_SOUL:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("soul_icon.png");
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",str_Format_junhun__ , number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_CONSUMABLE:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(value);
                sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_storySectionLayer);
                int starLev = temp->getConsumeStarLvl();
                starLev = starLev>0 ? (starLev-1):0;
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starLev)->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getConsumeName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
                
//            case BIT_MEDAL:
//            {
//                
//            }
            case BIT_MATERIAL:
            {
                cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                SGMaterialDataModel * model = SGStaticDataManager::shareStatic()->getMaterialById(value);
                SGMainManager::shareMain()->addHeadIconPlistByNum(model->getMaterialIconId(), sg_storySectionLayer);
                int starLvl = model->getMaterialBorderColor();
                if (starLvl > 1) //因为绿色品质填表为3，但是图片名字里是2。
                    starLvl -= 1;
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starLvl)->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",model->getMaterialIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",model->getMaterialName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case BIT_MEDAL:
            default:
                break;
        }
        
        
        if (cardBg && itemIcon && jinkuang)
        {
            if (i == 0) {
                posX = cardBg->getContentSize().width;
            }
            cardBg->setPosition(ccp(posX,contentHeight - SECTIONCELLGAP*4));
            if (sectionInfoView)
            sectionInfoView->addChild(cardBg,9,-200+i);
            
            itemIcon->setPosition(ccp(cardBg->getContentSize().width/2,cardBg->getContentSize().height/2));
            jinkuang->setPosition(itemIcon->getPosition());
            if (rewardCount == 1 && type == 1) {
                
                itemName->setPosition(ccp(cardBg->getContentSize().width*1.6,cardBg->getContentSize().height/2));
            }
            else
            {
               itemName->setPosition(ccp(cardBg->getContentSize().width/2,-12));
            }
            cardBg->addChild(itemIcon);
            cardBg->addChild(jinkuang);
            cardBg->addChild(itemName);
            
            posX += cardBg->getContentSize().width *1.2;
            cardBg = NULL;
        }
        else
        {
            CCLOG("addSectionStarItem type===%d",type);
        }
    }
}

void SGStorySectionLayer::addSectionItem(float contentHeight,bool isStarReward)
{
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
    // 0 武将 1 装备 2 士兵 3 道具
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    if (sectionInfoView == NULL) {
        return;
    }
    for (int i = 0; i<5; i++) {
        sectionInfoView->removeChildByTag(-200+i, true);
     }
    if (isStarReward) {
        addSectionStarItem(contentHeight);
    }
    else
    {
        addSectionNormalItem(contentHeight);
    }
}
void SGStorySectionLayer::dealHideSingleMove()
{
    int rowCount = tableView->getRowCount();
    SGStorySectionCell *item = NULL;
    SNSTableViewCell *cell =  NULL;
    for (int ii = 0; ii < rowCount; ii++) {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL) {
           item = (SGStorySectionCell *)(cell->getChildByTag(0));
            if (item && item->_bigBGBtn) {
                item->_bigBGBtn->setEnabled(false);
            }
        }
    }
    hideScrollItemSingleMove();
}


void SGStorySectionLayer::limitMenuTouch()
{
    schedule(schedule_selector(SGStorySectionLayer::resetMenuTouch) , 2);
    if (menu)
    {
        menu->setTouchEnabled(false);
        tableView->setTouchEnabled(false);
    }
}

void SGStorySectionLayer::resetMenuTouch()
{
    unschedule(schedule_selector(SGStorySectionLayer::resetMenuTouch));
    if (menu)
    {
        menu->setTouchEnabled(true);
        tableView->setTouchEnabled(true);
    }
}

void SGStorySectionLayer::delayShowSectionInfoHandler(SGStorySectionCell *section)
{
    if (sectiontableView && sectiontableView->isVisible())
    {
        m_lastShowSection = section;
        showSectionInfoHandler(section);
    }
}


void SGStorySectionLayer::entrySectionHandler(SGStorySectionCell *section) //cgp_pro
{
    if (section == NULL )
    {
        return;
    }
    if (isMovingSection) {
        return;
    }
    if (selectIndex < datas->count()) {
        SGStorySceneBean *sceneBean = (SGStorySceneBean *)datas->objectAtIndex(selectIndex);
        SGPlayerInfo::sharePlayerInfo()->setPlotId(sceneBean->getPlotId());
        if (sceneBean->getExp() == STARWARDTAG) {
            //领奖
            
            this->sendRewardRequest(sceneBean->getStoryId());
            return;
        }
        else
        {
            if (checkGovernNeed())
            {
                CCLOG("统御力超出");
                SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 39, NULL);
                SGMainManager::shareMain()->showBox(cantadvanceBox);
                return;
            }
            
            SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
            int count = player->getGoodsNumInBag();
            if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
            {
                SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
                SGMainManager::shareMain()->showBox(cantadvanceBox);
                this->setIsCanTouch(false);
                return;
            }
            
            int power = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();
            int pSread = power - sceneBean->getNeedPower();
            
            if (pSread >= 0)
            {
                // ÃÂ¡¶OK
                if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
                {
                    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
                    SGMainManager::shareMain()->showBox(cantadvanceBox);
                }
                else
                {
                    int vipLv = SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel();
                    int plotId = section->getGift()->getPlotId();
                    int rank = section->getGift()->getRank();
                    SGPlayerInfo::sharePlayerInfo()->setVictory(rank);
                    

                    CCLOG("debug == || plotId == %d, rank == %d", plotId, rank);

                    if ((rank>2 || (vipLv>3 && rank>=2)||(rank>=1&&plotId>=20000))&&(plotId!=10&&plotId!=11&&plotId!=17&&plotId!=18))   //cgp_pro
                    { // 完胜，提示快速战斗
                        storySecenId = plotId;
                        SGQFightBox *box = SGQFightBox::create(this, SKIPHELPERINDEX);
                        box->setDatas(plotId, SKIPHELPERINDEX, storyBean->getStoryId(),(rank>2 || (vipLv>3 && rank>=2)||(rank>=1&&plotId>=20000)),sceneBean->getPower(),sceneBean->getCreamDayTimes());
                        SGMainManager::shareMain()->showBox(box);
                    }
                    else
                    {
                        
                        CCLOG("debug == || not 3 stars selectIndex == %d", selectIndex);
                        
                        CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGStorySectionLayer::dealHideSingleMove));
                        CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGStorySectionLayer::gotoFirendRequest));
                        CCCallFuncND *speedFun3 = CCCallFuncND::create(this, callfuncND_selector(SGStorySectionLayer::delayShowSectionInfoHandler), section);
                        
                        this->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME),speedFun3,speedFun,CCDelayTime::create(0.3f*(tableView->getChildrenCount()/10.0f+1)), speedFun2,  NULL));

                        limitMenuTouch();
                    }

                }
            }
            else
            {
                selectIndex = -1;
                int currLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
                int powerLimit = SGStaticDataManager::shareStatic()->getPlayerByLevel(currLevel)->getPlayerPower();;
                
                if (sceneBean->getNeedPower() > powerLimit)
                { 
                    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 24, NULL);
                    SGMainManager::shareMain()->showBox(cantadvanceBox);
                }
                else
                {
                    gotoPurchPower();
                }
            }
        }
    }
}


void SGStorySectionLayer::refreshCreamOnBuy()
{
    SNSIndexPath * indexPath = SNSIndexPath::create(0, 0, 1);
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);
    SGStorySectionCell *item = (SGStorySectionCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    showSectionInfoHandler(item);
    SGStorySceneBean *sceneBean = (SGStorySceneBean *)datas->objectAtIndex(selectIndex);
    item->setData(sceneBean, datas);
    scheduleOnce(schedule_selector(SGStorySectionLayer::refreshCreamOnBuy), 0.4);
}



int SGStorySectionLayer::getSelectIndex()
{
	return selectIndex;
}

bool SGStorySectionLayer::checkGovernNeed()
{
//    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();
    int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
    CCObject *obj = NULL;
    int mor = 0;
    int speed = 0;
    int totleGovern = 0;
    CCARRAY_FOREACH(array, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        mor += card->getMorale();
        speed += card->getSpeed();
        totleGovern += card->getGoverning();
        if (card->getOfficerEquips()) {
            for (int i = 0; i <card->getOfficerEquips()->count(); i++) {
                SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
//                mor += equip->getMorale();
//                speed += equip->getSpeed();
                if ((card->getGoverning()+equip->getGoverning())<0) {
                    totleGovern -= card->getGoverning();
                }
                else{
                    totleGovern += equip->getGoverning();
                }
            }
        }
    }

    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    int allGovern=SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering()+ player->getPlayerGoverning();
    return totleGovern > allGovern ? true:false;
}
void  SGStorySectionLayer::showRewardBox()
{

    CCArray *data=CCArray::create();
    data->retain();
    int type = 0;
    int value = 0;
    int number = 0;
    //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14天梯积分 15 消耗品
    SGRewardDataModel* rewardModel = SGStaticDataManager::shareStatic()->getRewardById(storyBean->getRewardId());
    int rewardCount = rewardModel->rewardArray->count();
    for(int i=0; i<rewardCount && i<4; i++)
    {
         SGMailSystemAccessoryDataModel *access=new SGMailSystemAccessoryDataModel();
        sscanf(((CCString*)rewardModel->rewardArray->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
        switch (type) {
            case 0:
            {
                access->setType(0);
                access->setCount(number);
                
            }
                break;
            case 1:
            {
                access->setType(1);
                access->setCount(number);
            }
                break;
            case 2:
            {
                access->setType(14);
                access->setCount(number);
            }
                break;
            case 3:
            {
                access->setType(3);
                access->setCount(number);
            }
                break;
            case 4:
            {
                access->setType(4);
                access->setCount(number);

            }
                break;
            case 5:
            {
                access->setType(5);
                access->setCount(number);
            }
                break;
            case 6:
            {
                access->setType(6);
                access->setCount(number);
                access->setItemId(value);
                

            }
                break;
            case 7:
            {
                
                access->setType(7);
                access->setCount(number);
                access->setItemId(value);
            }
                break;
            case 9:
            {
                access->setType(9);
                access->setCount(number);
                access->setItemId(value);
                
            }
                break;
			case 15://消耗品掉落
            {
                access->setType(15);
                access->setCount(number);
                access->setItemId(value);
            }
                break;
            case 16:
            {
                access->setType(type);
                access->setCount(number);
                access->setItemId(value);
            }
            case 17:
            {
                access->setType(type);
                access->setCount(number);
                access->setItemId(value);
            }
            default:
                break;
        }
        if (access->getCount()>0) {
            data->addObject(access);
        }
    }
    
	SGShowRewardBox *rewardBox = NULL;
    if (isAutoReward == 0)//奖励没有自动领取的情况
	{
		rewardBox = SGShowRewardBox::create(this, data,4,STORYREWARDTAG);
        

        
        
	}
    else//奖励自动领取,需要在点击弹框确定之后返回闯关列表,这里用SGShowRewardBox的enterType
	{
		//使用此处,刷新星星
		this->backHandler(this);
		rewardBox = SGShowRewardBox::create(this, data,4,STORYREWARDAUTO);
		
	}
    
    SGMainManager::shareMain()->showBox(rewardBox);
    //显示对应的奖励弹出之后， 引导触发
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId >= guide_tag_47)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_48);
    }
}

NewScrollView::NewScrollView()
{
    parentScissorFrame = CCRect(0,0,0,0);
}

NewScrollView::~NewScrollView()
{
    
}

NewScrollView* creatNewScrollView(CCRect frames)
{
    NewScrollView* instance = new NewScrollView();
    if (instance && instance->initGSScrollViewFrame(frames))
    {
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}
bool NewScrollView::initGSScrollViewFrame(CCRect frames)
{
    if ( !SNSScrollView::initScrollViewFrame(frames) ) {
        return false;
    }
    setIsTouchEnabled(true);
    return true;
}

void NewScrollView::visit()
{
    glEnable(GL_SCISSOR_TEST);
    CCRect frame = CCRectMake(m_frame.origin.x, m_frame.origin.y, m_frame.size.width, m_frame.size.height);
    
    m_pos = getParent()->convertToWorldSpace(this->getPosition());
    
	float s = this->getScale();
	if (getParent()) {
		s *= this->getParent()->getScale();
	}

    if (parentScissorFrame.size.width > 0) {
         CCRect myScissorFrame = CCRect(m_pos.x, m_pos.y, frame.size.width * s, frame.size.height * s);
        float posX = myScissorFrame.getMinX()>parentScissorFrame.getMinX()?myScissorFrame.getMinX():parentScissorFrame.getMinX();
        float posy = myScissorFrame.getMinY()>parentScissorFrame.getMinY()?myScissorFrame.getMinY():parentScissorFrame.getMinY();
        float posXM = myScissorFrame.getMaxX()>parentScissorFrame.getMaxX()?parentScissorFrame.getMaxX():myScissorFrame.getMaxX();
        float posYM = myScissorFrame.getMaxY()>parentScissorFrame.getMaxY()?parentScissorFrame.getMaxY():myScissorFrame.getMaxY();
        CCEGLView::sharedOpenGLView()->setScissorInPoints(posX, posy, posXM - posX, posYM - posy);
    }
    else
    {
        CCEGLView::sharedOpenGLView()->setScissorInPoints(m_pos.x, m_pos.y, frame.size.width * s, frame.size.height * s);
    }
    SNSView::visit();
	glDisable(GL_SCISSOR_TEST);
}

void NewScrollView::setParentScissorFrame(CCRect parentScissor)
{
    parentScissorFrame = parentScissor;
}
void NewScrollView::updateFrames(CCRect frames){
    setFrame(frames);
}


//#pragma mark "================打完boss关, 出现的提示新关卡天启的提示框"

SGTipsBox::SGTipsBox():canGetReward(false)
,del(NULL)
,storyId(0)
{

}

SGTipsBox::~SGTipsBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_tipsBox);
}

SGTipsBox *SGTipsBox::create(SGBoxDelegate *del, bool canGetReward, int storyId)
{
	SGTipsBox *tipsBox = new SGTipsBox();
	tipsBox->canGetReward = canGetReward;
	tipsBox->del = del;
	tipsBox->storyId = storyId;
	
	int hgt = 350;
	if (tipsBox->canGetReward)
	{
		hgt = 360;
	}
	
	if (tipsBox && tipsBox->init(del, box_tipsBox,CCSizeMake(500, hgt),true))
	{
		tipsBox->initView();
		tipsBox->autorelease();
		return tipsBox;
	}
	
	CC_SAFE_RELEASE(tipsBox);
	return NULL;
	
}

void SGTipsBox::initView()
{
	
	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_box_tipsBox);
	
	CCPoint center = SGLayout::getPoint(kMiddleCenter);
	fontbg->setPreferredSize(CCSize(400, this->canGetReward ? 200 : 200));
	fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 15)));
	SGButton *okButton = SGButton::create("gonggao_an.png", "public_font_queding.png", this, menu_selector(SGTipsBox::pressOkButton),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::create("gonggao_an.png", "public_font_quxiao.png", this, menu_selector(SGTipsBox::pressCancelButton),CCPointZero,false,true);
	this->addBtn(okButton);
	this->addBtn(cancelButton);
//	okButton->setPosition(ccpAdd(center, ccp(-110, -324/2+(!this->canGetReward ? 5 : 25))));
//    cancelButton->setPosition(ccpAdd(center, ccp(110, -324/2+(!this->canGetReward ? 5 : 25))));

	okButton->setPosition(ccpAdd(center, ccp(-110, -324/2 + 25 )));
    cancelButton->setPosition(ccpAdd(center, ccp(110, -324/2 + 25)));
    
	SGCCLabelTTF *title = SGCCLabelTTF::create(str_StorySectionCell_str17, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
	this->addChild(title, 1);
	title->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width / 30 + 10, fontbg->getContentSize().height / 1.5 - 10)));
	
	std::string msgTips;
	if (this->canGetReward)
	{
		msgTips = str_StorySectionCell_str18;
	}
	else
	{
		msgTips = str_StorySectionCell_str19;
	}
	
	SGCCLabelTTF *tips = SGCCLabelTTF::create(msgTips.c_str(), FONT_PANGWA, 30, CCSizeMake(25*15, 200));
    tips->setAnchorPoint(ccp(.5, 1));
	//tips->setColor(ccRED);
    tips->setPosition(ccpAdd(biao->getPosition(), ccp(10, -60)));
	this->addChild(tips, 1);

}


//确定回调
void SGTipsBox::pressOkButton()
{
	CCLOG("press Ok!");
	if (this->del)
	{
		SGStorySectionLayer *layer = static_cast<SGStorySectionLayer *>(del);
		
		if (this->canGetReward)// 可领奖,发送消息
		{
			layer->sendRewardRequest(storyId);
		}
		else//不可领奖,返回章列表
		{
			layer->backHandler(layer);
		}
		SGMainManager::shareMain()->closeBox();
	}
	
	SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
}
//取消
void SGTipsBox::pressCancelButton()
{
	CCLOG("press Cancel!");
	SGMainManager::shareMain()->closeBox();
	
	SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);

}



























