//
//  SGShouJingZhouRank.cpp
//  GameSanGuo
//
//  Created by kanata on 13-11-14.
//
//

#include "SGShouJingZhouRank.h"
#include "SGMainManager.h"
#include "SimpleAudioEngine.h"
#include "SGShowJingZhouRankCell.h"
#include "SurvivalModule.pb.h"
#include "AppMacros.h"
#include "SGStringConfig.h"

SGShouJingZhouRank::SGShouJingZhouRank()

:newindex(0),
tickTime(NULL),
_rewardNum(0),
_residueTime(0)
{
    _selectIndex = 0;
}

SGShouJingZhouRank::~SGShouJingZhouRank()
{
    //移除
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->removeObserver(this, MSG_SURVIVAL_RANK_LIST);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_sjzrank);
}


SGShouJingZhouRank *SGShouJingZhouRank::create(CCArray *array, int pageItemsNum, int pageMax, int nowRank, int coins, int rewardNum, int time)
{
    SGShouJingZhouRank *layer = new SGShouJingZhouRank();
    if (layer && layer->init(NULL, sg_sjzrank))
    {
        layer->initDatas(array, pageItemsNum, pageMax, nowRank, coins, rewardNum, time);
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGShouJingZhouRank::initDatas(CCArray *array, int pageItemsNum, int pageMax, int nowRank, int coins, int rewardNum, int time)
{
    this->datas = CCArray::create();
    this->datas->retain();
    for (int ii = 0; ii < array->count(); ii++) {
        CCObject *obj = array->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
    _pageItemsNum = pageItemsNum;
    _pageTotalNum = pageMax;
    _rankCur = nowRank;
    _rewardCoins = coins;
    _pageCur = 1;
	_rewardNum = rewardNum;
	_residueTime = time;
	
    //最大页数
    int mod = _pageTotalNum % _pageItemsNum;
    int page = _pageTotalNum / _pageItemsNum;
    if (mod == 0)
        _pageMaxReal = page;
    else
        _pageMaxReal = page + 1;
}


void SGShouJingZhouRank::initView()
{
    //监听
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_SURVIVAL_RANK_LIST, this, callfuncO_selector(SGShouJingZhouRank::flushData));
    
//    ResourceManager::sharedInstance()->bindTexture("public/public.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getVisitHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
//    CCRect rect = CCRectMake(stdSize.width/2-size.width/2, stdSize.height/2-(size.height-headH-bottomH)/2,
//                             size.width, size.height-headH-bottomH);

    
 
    float mmm=size.height-headH-bottomH;
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_sjzrank);
    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    
    CCRect rect = CCRectMake(0, stdSize.height/2-(size.height-headH-bottomH)/2,bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    
    ////////返回按钮
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png",str_back, this, menu_selector(SGShouJingZhouRank::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.05);
    
//    backBtn->setPosition(ccpAdd(title_bg->getPosition(),ccp(-title_bg->getContentSize().width*0.4,-backBtn->getContentSize().width*0.28))) ;
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*1.55)));
    
    this->addBtn(backBtn);
    ///今日排名 次日可领后的背景图
	
	
    CCScale9Sprite *xiaobg=CCScale9Sprite::createWithSpriteFrameName("xiaobg.png");
    xiaobg->setAnchorPoint(ccp(0, 1));
//    xiaobg->setPreferredSize(CCSizeMake(size.width,backBtn->getContentSize().height*1.6));
    xiaobg->setPreferredSize(CCSizeMake(size.width,mmm*0.12));
    xiaobg->setPosition(ccpAdd(backBtn->getPosition(), ccp(-backBtn->getContentSize().width*0.60, -backBtn->getContentSize().height*0.5)));
    this->addChild(xiaobg);
	

	
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    
    
    //////////////////各种数据
  
    SGSjzData* sjzData = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    //////////////////
    CCLabelTTF* label1 = CCLabelTTF::create(str_ShouJingZhouRank_str1, FONT_BOXINFO, FONTSIZE(20));
    //    label->setPosition(ccp(backBtn->getPosition())
    label1->setPosition(ccpAdd(backBtn->getPosition(), ccp(size.width*0.45,-backBtn->getContentSize().height*0.0)));
    label1->setColor(ccc3(0xe7, 0xb6, 0x7b));
    this->addChild(label1);
	
	
	// 显示排名前多少名可以领取奖励, 名次动态指定 //#FDBB09
	SGCCLabelTTF *rewardInfo = SGCCLabelTTF::create(CCString::createWithFormat(str_ShouJingZhouRank_str2, _rewardNum)->getCString(), FONT_PANGWA, FONTSIZE(11) , ccc3(0xfd, 0xbb, 0x09));
    //modify by:zyc. merge into create.
	//rewardInfo->setColor(ccc3(0xfd, 0xbb, 0x09));
	rewardInfo->setAnchorPoint(ccp(0, 0.5));
	this->addChild(rewardInfo);
	rewardInfo->setPosition(ccpAdd(backBtn->getPosition(), ccp(-25, -mmm*0.12 * 0.65)));
	
    //我的排名
    SGCCLabelTTF* label2 = SGCCLabelTTF::create(str_ShouJingZhouRank_str3, FONT_PANGWA, FONTSIZE(11));
    label2->setAnchorPoint(ccp(1.0, 0.5));
    label2->setPosition(ccpAdd(rewardInfo->getPosition(), ccp(rewardInfo->getContentSize().width + label2->getContentSize().width + 30, 0)));
    this->addChild(label2);
	
    ////今日排名后的数字 //#3995F1
    SGCCLabelTTF* label21 = SGCCLabelTTF::create(CCString::createWithFormat("%d", sjzData->getDailyRank())->getCString(), FONT_BOXINFO, FONTSIZE(13) , ccc3(0x39, 0x95, 0xf1));
    //modify by:zyc. merge into create.
	//label21->setColor(ccc3(0x39, 0x95, 0xf1));
    label21->setAnchorPoint(ccp(0, 0.5));
    label21->setPosition(ccpAdd(label2->getPosition(), ccp(size.width*0.0,0)));
    this->addChild(label21);
	
	//发奖倒计时
	SGCCLabelTTF *timeTips = SGCCLabelTTF::create(str_ShouJingZhouRank_str4, FONT_PANGWA, FONTSIZE(11));
	timeTips->setAnchorPoint(ccp(0, 0.5));
	this->addChild(timeTips);
	timeTips->setPosition(ccpAdd(rewardInfo->getPosition(), ccp(-5, -timeTips->getContentSize().height - mmm*0.005)));
	
	//倒计时的数字
	long long residueTimes = _residueTime;
	long long sec = residueTimes % 60;// 秒
	residueTimes /= 60;
	long long mins = residueTimes % 60;//分
	long long hours = residueTimes / 60;//小时
	
	char timeString[32] = "\0";
	sprintf(timeString, "%02lld: %02lld: %02lld", hours, mins, sec);

	tickTime = CCLabelTTF::create(timeString, FONT_BOXINFO, FONTSIZE(13));
	this->addChild(tickTime);
	tickTime->setAnchorPoint(ccp(0, 0.5));
	tickTime->setPosition(ccpAdd(timeTips->getPosition(), ccp(timeTips->getContentSize().width, 0)));
	
	// 今日参战的次数, 次数动态指定
	
	SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
	int fightedCount = data->getCountCur() + data->getVIPCountCur();
	
	SGCCLabelTTF *joinTimes = SGCCLabelTTF::create(str_ShouJingZhouRank_str5 , FONT_PANGWA, FONTSIZE(11));
	joinTimes->setAnchorPoint(ccp(0, 0.5));
	this->addChild(joinTimes);
	joinTimes->setPosition(ccpAdd(label2->getPosition(), ccp(-label2->getContentSize().width - 3, -joinTimes->getContentSize().height -  mmm*0.005 )));
	
	//#39D630
	SGCCLabelTTF *joinCount = SGCCLabelTTF::create(CCString::createWithFormat("%d", fightedCount)->getCString(), FONT_BOXINFO, FONTSIZE(13) , ccc3(0x39, 0xd6, 0x30) );
    //modify by:zyc. merge into create.
	//joinCount->setColor(ccc3(0x39, 0xd6, 0x30));
	this->addChild(joinCount);
	joinCount->setAnchorPoint(ccp(0, 0.5));
	joinCount->setPosition(ccpAdd(joinTimes->getPosition(), ccp(joinTimes->getContentSize().width + 5, 0)));
	
	
	 
    ///”今日守荆州排名“  几个字
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ShouJingZhouRank_str6, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    title->setPosition(ccpAdd(backBtn->getPosition(), ccp(size.width*0.40,-backBtn->getContentSize().height*0.0)));
    
	float downShift = 0.89f;
	if (CCEGLView::sharedOpenGLView()->getFrameSize().height == iphonehdResource.size.height)
	{
		downShift = 0.86;
		xiaobg->setScaleY(1.3);

	}
    //tableview各种设置
    tableView->setFrame(CCRectMake(0, 0, size.width, mmm*downShift));//headH
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    tableViewHeight = 150;
    
	//加入倒计时定时器
	schedule(schedule_selector(SGShouJingZhouRank::updateTimeInfo), 1.0f);

	
}

//刷新时间显示
void SGShouJingZhouRank::updateTimeInfo()
{
	long long residueTimes = _residueTime;
	long long sec = residueTimes % 60;// 秒
	residueTimes /= 60;
	long long mins = residueTimes % 60;//分
	long long hours = residueTimes / 60;//小时
	
	char timeString[32] = "\0";
	sprintf(timeString, "%02lld: %02lld: %02lld", hours, mins, sec);
	if (_residueTime == 0)
	{
		this->unschedule(schedule_selector(SGShouJingZhouRank::updateTimeInfo));//移除定时器
		return ;
	}
	
	_residueTime--;
	tickTime->setString(timeString);
}

void SGShouJingZhouRank::backHandler(CCObject*obj)
{
    SGMainManager::shareMain()->showshoujingzhou();
}

#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGShouJingZhouRank::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);//从0开始
	CCSize size = tableView->getItemSize();
    
	SGShowJingZhouRankCell *item = (SGShowJingZhouRankCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    //        tableView->moveToIndexPath()
    if (item == NULL) {
        item = SGShowJingZhouRankCell::create();
    }
    //item->resetBgImg(index);
    
    if (index < datas->count())
	{
        SGShowJingZhouRankPlayerLayer *player =  (SGShowJingZhouRankPlayerLayer *)datas->objectAtIndex(index);
        //更新显示cell
		item->setData(player);
		
        if ( (index == datas->count()-1) && (index < _pageTotalNum - 1) )
		{
            newindex = index-1;
            main::SurvivalRankListRequest* req = new main::SurvivalRankListRequest();
            int a = datas->count();
            int b = a / _pageItemsNum;
            req->set_page(b + 1);
            SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_RANK_LIST, req);
            
//            main::PvpTopRequest *request = new main::PvpTopRequest();
//            request->set_page(datas->count()/25+1);
//            request->set_type(type);
//            SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_RANK, request);
        }
    }else {
        //        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGShouJingZhouRank::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGShouJingZhouRank::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    // get player info and show on layer
}


void SGShouJingZhouRank::setSelecIndex(int index)
{
    CCMenuItemSprite *menuItem = NULL;
    if (_selectIndex != 0) {
        menuItem = (CCMenuItemSprite *)menu->getChildByTag(_selectIndex);
        if (menuItem) menuItem->unselected();
    }
    _selectIndex = index;
    menuItem = (CCMenuItemSprite *)menu->getChildByTag(_selectIndex);
    if (menuItem) menuItem->selected();
}

//追加刷新本页面
void SGShouJingZhouRank::flushData(CCObject* obj)
{
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    main::SurvivalRankListResponse* rsp = (main::SurvivalRankListResponse*) sr->m_msg;
    if (rsp)
    {
        main::SurvivalRankPlayerMessage temp;
        SGShowJingZhouRankPlayerLayer* data;
        for (int k = 0; k < rsp->rankplayers_size(); k++)
        {
            temp = rsp->rankplayers(k);
            data = SGShowJingZhouRankPlayerLayer::create(temp.playerid(), temp.nickname().c_str(), temp.rankcount(),
														 temp.survivallevel(), temp.maxdamage(), temp.iconid(), temp.acquiregold());
            datas->addObject(data);
        }
        tableView->reloadData(false);
	}
}

