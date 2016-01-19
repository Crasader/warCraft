//
//  SGFightRewardLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-7-11.
//
//

#include "SGFightRewardLayer.h"

#include "SGStoryRewardBox.h"
#include "SGStoryRewardItem.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGStaticDataManager.h"
#include "SGAIManager.h"
#include "SGBattleManager.h"
#include "SGPvpMainLayer.h"
#include "SGHeroLayer.h"
#include "SGMainManager.h"
#include "SGFriend.h"
#include "SGGeneralBox.h"
#include "SGCantAdvanceBox.h"
#include "SGLevelUpLayer.h"
#include "SGTestSwitch.h"
#include "SimpleAudioEngine.h"
#include "PlotModule.pb.h"
#include "SGGuideManager.h"
#include "SGNotificationCenter.h"
#include "ResourceManager.h"
#include "SurvivalModule.pb.h"
#include "SGFirstLayer.h"
#include "AppMacros.h"
#include "SGStringConfig.h"
#include "SGShareBox.h"
#include "ActivityModule.pb.h"
#include "SpecialBattleModule.pb.h"
#include "SGFightOverNoticeBox.h"
#include "SGExoticMerchantLayer.h"
#include "SGLogicManager.h"

#define runTime 0.000000000005f

int actPlotId = -1;

SGFightRewardLayer::SGFightRewardLayer(void) : labelRate(NULL), dhzl(NULL), jxcg(NULL), entertype(0), proTimer(NULL), currCoin(0), currGold(0),
currExps(0), currExps2(0), playInfo(NULL), mainLayer(NULL), cMaxExp1(0), cMaxExp2(0), cMaxExpTmp(0), upLevel(0), currLevel(0),
star1(NULL), star2(NULL), star3(NULL), m_rewardScrollView(NULL), scrollview(NULL), rateNum(0), m_reqPvpMain(0), isMoved(false)
{
    ldanCount = -1;
    _isFirstTouch = true;
    _isFinishStar=false;
    _sjzcannottouch=false;
 
    m_jump=false;
    
    _addExp = 0;
    _addCoin = 0;
    _addGold = 0;
    _addJun = 0;
    
    _totalExp2 = 0;
    _totalExp = 0;
    _totalCoin = 0;
    _totalGold = 0;
    _totalJun = 0;
    _addExp1=0;
	soulNum = 0;
    
    labelExp = NULL;
    labelExpA = NULL;
    labelCoin = NULL;
    labelGold = NULL;
//    labelJun = NULL;
    
    conSprite = NULL;
    winSprite = NULL;
    m_rewardScrollView = NULL;
    
    _rewardData = NULL;
    
    
    m_isShowExoticMerchant = false;

}

SGFightRewardLayer::~SGFightRewardLayer(void)
{

	ResourceManager::sharedInstance()->unBindLayerTexture(sg_fightrewardLayer);
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_SCENE_ENTER);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CREAM_STORY);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_ACT_SCENE);
    //这里释放的是由SGOverBattleBox 、SGMainLayer::showRewardBox 中  retain的。
    CC_SAFE_RELEASE(_rewardData);
}

SGFightRewardLayer* SGFightRewardLayer::create(SGStoryReward *reward,int type , bool isShowExoticMerchant)
{
    SGFightRewardLayer *layer = new SGFightRewardLayer();
    if (layer && layer->init(NULL, sg_fightrewardLayer))
    {
        layer->entertype=type;
        layer->m_isShowExoticMerchant = isShowExoticMerchant;
        layer->initView(reward);
        layer->autorelease();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return NULL;
    }
}

void SGFightRewardLayer::initView(SGStoryReward *reward)
{
    GPCCLOG("SGFightRewardLayer\n");
    //MMHACK: SPEED 在这里把自动3倍速关闭，防止可能出现的游戏加速问题。
    SGLogicManager::gi()->setGameSpeed(1.0f);
    
    //持有奖励消息，setAllLabel需要使用。
    _rewardData = reward;//已在创建时retain。此处无需retain，但需要在析构中release。
    
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this,
																  callfuncO_selector(SGFightRewardLayer::getFriendOfficerListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CREAM_STORY, this,
																  callfuncO_selector(SGFightRewardLayer::showTheStorySectionListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_STORY_ACT_SCENE, this, callfuncO_selector(SGFightRewardLayer::activityListener));

	
    CCSize size = CCDirector::sharedDirector()->getWinSize();
     CCPoint center = SGLayout::getPoint(kMiddleCenter);
    //CCPoint center = SGLayout::getPoint(kMiddleCenter);
    float headH = H_TOP;//SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = H_BOTTOM;//SGMainManager::shareMain()->getBottomHeight();
    ldanCount = reward->getLianDan();
    
    this->datas =  CCArray::create();
    this->datas->retain();
    for (int ii = 0; ii < reward->getItems()->count(); ii++)
	{
        CCObject *obj = reward->getItems()->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
#pragma mark "------------------守荆州加入军魂------------------------"
	//如果军魂不为0,
	if (reward->getSoul())
	{
		//构造一个新的军魂的奖励对象, 加入datas中
		SGStoryRdItem *soulItem = SGStoryRdItem::create(-1, -1, 11);//前二个忽略
        soulItem->setCount(reward->getSoul());
		CCArray* arrayItem =(CCArray*)datas->objectAtIndex(0);
        arrayItem->addObject(soulItem);
		soulNum = reward->getSoul();
        SGStoryRdItem *coinItem = SGStoryRdItem::create(-1, -1, 12);
        coinItem->setCount(reward->getCoin());
        arrayItem->addObject(coinItem);
        int ncoin = _addCoin+_totalCoin;
	}
 
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfightrewardlayer/sgfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);
	ResourceManager::sharedInstance()->bindTexture("sgshoujingzhou/sgshoujingzhou.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfindhelplayer/sgfindhelplayer.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer, LIM_PNG_AS_PNG);
    
    
    float discY = size.height-headH-bottomH+65;
//    CCRect rect = CCRectMake(768*.5f-size.width*.5f, (1136-discY)*.5f, size.width, discY);
	
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
	CCRect rect = CCRectMake(0, (1136-discY)*.5f, bgImg->getContentSize().width, discY);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -15);
    bgImg->setVisible(false);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("fight_over_titlebg.png");
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height*.81f)));//,6
    this->addChild(title_bg, -1);
    
    CCSprite *title_Lbg = CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Lbg->setAnchorPoint(ccp(0, 0.5f));
    title_Lbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(-title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Lbg, -1);
    CCSprite *title_Rbg =CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Rbg->setAnchorPoint(ccp(1, 0.5f));
    title_Rbg->setFlipX(true);
    title_Rbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Rbg, -1);
    
    CCSprite *title = CCSprite::createWithSpriteFrameName("fight_over_title_n.png");
    title->setPosition(title_bg->getPosition());
    this->addChild(title);

    
    CCSprite *infoBg = CCSprite::createWithSpriteFrameName("fight_over_bg_n.png");
    infoBg->setPosition(ccp(size.width*.5f, bottomH + discY*.49f-20));//.45
    if(CCDirector::sharedDirector()->getWinSize().height < 1024 )
    {
        infoBg->setPosition(ccp(size.width*.5f, bottomH + discY*.49f + 14));//.45
    }

//    float posX = 20.0f;
//    float posY = infoBg->getContentSize().height - 35;
//    const char* fontName = FONT_BOXINFO;
//    float fontSize = 28;
    rateNum = reward->getRating();
    
    
    playInfo = SGPlayerInfo::sharePlayerInfo();
    mainLayer = SGMainManager::shareMain()->getMainLayer();
    currGold = playInfo->getPlayerGold();
    currCoin = playInfo->getPlayerCoins();

    //升级时会提前收到用户状态更新的消息，所以必须先找到之前等级的一些信息，因为这里的动画是根据之前的数据进行展示的，非常的坑。
    if (playInfo->getPlayerLevel() != reward->getBeforeLevel()) //升级了。
    {
        currExps = reward->getExp();
        currExps2 = reward->getExp();
        currLevel = reward->getBeforeLevel();
        _addExp=currExps;
    }
    else //未升级。
    {
        currExps = reward->getExp();//hehehe
        currExps2 = reward->getExp();//hehehe
        currLevel = reward->getBeforeLevel();
        _addExp=currExps;
    }
    
    //    bool isLevelUp = updateLevel(reward->getExp()+reward->getExtExp());
    
    float tpx = infoBg->getPositionX() - infoBg->getContentSize().width*.475f;
    float tpy = infoBg->getPositionY() - infoBg->getContentSize().height*.5f;
    tableView->setFrame(CCRectMake(0, 0, infoBg->getContentSize().width*.95f, 120));
    tableView->setPosition(ccp(tpx, tpy-18));
    tableViewHeight = skewing(55);
    tableView->setVisible(false);

    //根据战斗类型来判断奖励界面，以后可能添加PVE:闯关/守荆州+PVP:天梯/竞技场等不同的导向。
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    bool win = SGBattleManager::sharedBattle()->isWin;
    
    if( true /* btp == XXXX */) //通用路线。
    {
        //ZYC: 该消息统一在mainlayer中处理。
//        SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_STORY_SCENE_ENTER, this, callfuncO_selector(SGFightRewardLayer::showSceneLayer));
		
        this->addChild(infoBg, -2,1000);
        infoBg->setScaleY(1.1);//1.2
        
        
        float posX = 30.0f;
        float posY = infoBg->getContentSize().height - 75+size.height/3;
        
        if (size.height==1136)
            posY = infoBg->getContentSize().height - 82+size.height/3;
        else if(size.height==960)
            posY = infoBg->getContentSize().height - 105+size.height/3;
        else
        {
            posY = infoBg->getContentSize().height - 100+size.height/3;
             posX=90;
        }
        const char* fontName = FONT_BOXINFO;
        float fontSize = 28;
        rateNum = reward->getRating();
        
        for (int ii = 1; ii <= 4; ii++) {
            if ( ii==2 || ii==3) continue;
           // CCSprite *line = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
           // line->setPosition(ccp(infoBg->getContentSize().width*.5f, posY-17 - ii*47));
           // this->addChild(line);
        }

        int posx=0;
        int posy =0;

        CCString *string = CCString::createWithFormat(str_ActivityMainCell_str12, (reward->getHpRate()*100));
        if (reward->getHpRate()==0 && reward->getRating()==3)
            string = CCString::create(str_ActivityMainCell_str13);
        if(reward->getHpRate()==0 && reward->getRating()==2){
            float randNum=rand() %50+1;
            string = CCString::createWithFormat(str_ActivityMainCell_str12, randNum);
        }
        
		if (btp == BT_SURVIVAL)
		{
			fontName = FONT_PANGWA;
		}
        CCLayer * layerscroll = CCLayer::create();
		scrollview = SGScrollViewSubview::create(ccp(0,0), CCSize(300,200));
        scrollview->setPosition(0,300);
        m_rewardScrollView = CCScrollView::create( CCSize(infoBg->getContentSize().width*.95f, 440));
        //m_rewardScrollView->setAnchorPoint(ccp(0.5,1));
        m_rewardScrollView->setPosition(ccpAdd(infoBg->getPosition(),ccp(-infoBg->getContentSize().width/2+10,-infoBg->getContentSize().height/2)));
        this->addChild(m_rewardScrollView,1000);
       // m_rewardScrollView->setContentSize(CCSizeMake(infoBg->getContentSize().width*.95f, 420*7));
        
        m_rewardScrollView->setContentOffset(CCPoint(0,0));
        m_rewardScrollView->setBounceable(true);
        layerscroll->addChild(scrollview);
        //layerscroll->setPosition(ccp(0,-900));
        SGCCLabelTTF *labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
        if(datas->count()>1)
            labelTTF->setVisible(false);
        labelTTF->setAnchorPoint(ccp(0, 0.5f));
        labelTTF->setPosition(ccp(0, 0-20));
        scrollview->addChild(labelTTF);
        m_rewardScrollView->setDirection(kCCScrollViewDirectionVertical);
        //m_rewardScrollView->setTouchEnabled(false);
        
        
        CCSprite *line1 = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
        line1->setPosition(ccp(posx+line1->getContentSize().width/2, posy-50));
        scrollview->addChild(line1);
        CCSprite *line2 = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
        line2->setPosition(ccp(posx+line2->getContentSize().width/2, posy-220));
        scrollview->addChild(line2);

        
        float heightdeta = 0;
        float fheight = 0;
        
        if (datas->count()>1)
        {
            //十连扫
            CCSprite *m_arrowLeft = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
            
            m_arrowLeft->setPosition(ccp(infoBg->getContentSize().width - m_arrowLeft->getContentSize().width,m_arrowLeft->getContentSize().height/2));
            m_arrowLeft->setVisible(true);
            m_arrowLeft->setRotation(-90);
            CCRepeatForever* actLeft = CCRepeatForever::create( CCSequence::create( CCMoveBy::create(0.08, ccp(0, -m_arrowLeft->getContentSize().height/4)), CCDelayTime::create(0.4), CCMoveBy::create(0.08, ccp(0, m_arrowLeft->getContentSize().height/4)), CCDelayTime::create(0.4), NULL));
            m_arrowLeft->runAction(actLeft);
            
            infoBg->addChild(m_arrowLeft, 100, 200);
        }
        
        for(int i= 0;i<datas->count();i++)
        {
        
            SGStoryRewardItem *item = SGStoryRewardItem::create(soulNum,reward->getCoin(), sg_fightrewardLayer);
            
            if(datas->count()>1)
            {
                item->setDataWithArray((CCArray*)datas->objectAtIndex(i),2,datas->count()-i);
            }
            else
            {
                item->setDataWithArray((CCArray*)datas->objectAtIndex(i),1,0);
            }
            fheight = item->getContentSize().height;
            layerscroll->addChild(item);
            heightdeta = (i+1)*item->getContentSize().height;
            item->setAnchorPoint(ccp(0,0));
            item->ignoreAnchorPointForPosition(false);
            if(datas->count()==1)
            item ->setPosition(ccp(0,heightdeta*0));
            else{
                item ->setPosition(ccp(0,heightdeta));
            }
        }
        //heightdeta = heightdeta+scrollview->getContentSize().height*1;
        if(datas->count()<=0)
        {
            heightdeta = 320;
            fheight = 320;
        }
        scrollview->setAnchorPoint(ccp(0,0));
        scrollview->ignoreAnchorPointForPosition(false);
        if(datas->count()>1)
        scrollview->setPosition(0,heightdeta+320);
        else{
            scrollview->setPosition(0,heightdeta-fheight+320);
        }
        if(datas->count()>1)
            heightdeta = heightdeta+fheight;
        heightdeta+=scrollview->getContentSize().height;
        layerscroll->setAnchorPoint(ccp(0,0));
        layerscroll->ignoreAnchorPointForPosition(false);
        if(fheight<320){
            layerscroll->setPosition(ccp(0,-heightdeta+440+20));
        }
        else{
            layerscroll->setPosition(ccp(0,-heightdeta+440+160));
        }
        layerscroll->setContentSize(m_rewardScrollView->getContentSize());
        m_rewardScrollView->setContentOffset(CCPoint(0,0));
		SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        
        m_rewardScrollView->setContainer(layerscroll);
        m_rewardScrollView->setContentSize(CCSizeMake(infoBg->getContentSize().width*.95f, heightdeta+20));
        mainScene->removeChildByTag(1314);
		//不是守荆州的时候显示下面的结算描述
		if (btp != BT_SURVIVAL)
		{
            //int posx=0;
            //int posy =0;
			labelTTF = SGCCLabelTTF::create(str_ActivityMainCell_str14, fontName, fontSize);
			labelTTF->setAnchorPoint(ccp(0, 0.5f));
			labelTTF->setPosition(ccp(posx, posy-47-50));
			scrollview->addChild(labelTTF);
			
			_totalExp = /*reward->getExp()+*/reward->getExtExp();//hehehe
			_totalExp2= /*reward->getExp()+*/reward->getExtExp();//hehehe
			labelExp = SGCCLabelTTF::create("0", fontName, fontSize);
			labelExp->setAnchorPoint(ccp(0, 0.5f));
			labelExp->setPosition(ccpAdd(labelTTF->getPosition(), ccp(labelTTF->getContentSize().width, 0)));
			scrollview->addChild(labelExp);
			if(datas->count()>1)
            {
                SGCCLabelTTF* labelJiesuan = SGCCLabelTTF::create(str_OverBattleBox_str1013, FONT_PANGWA, 50 , ccc3(255, 223, 54));
                labelJiesuan->setAnchorPoint(ccp(0, 0.5f));
                labelJiesuan->cocos2d::CCNode::setPosition(ccp(posx+labelJiesuan->getContentSize().width, posy-15));
                scrollview->addChild(labelJiesuan);
            }
			
            ///////////////////
            int playerNowLevel = playInfo->getPlayerLevel();
            
            //获得exp当前需要显示的数值
            if (playInfo->getPlayerLevel() != reward->getBeforeLevel()) //升级了。
            {
                currExps = reward->getExp();
            }
            else //未升级
            {
                currExps = reward->getExp();
            }
            cMaxExp1 = SGStaticDataManager::shareStatic()->getPlayerByLevel(currLevel)->getPlayerExp();
            
            /*
            //这一段是老的为了计算升级和未升级情况下，初始该显示的exp数值文本。
            if (playInfo->getPlayerExp() - _totalExp > 0)
            {
                //未升级
                currExps = playInfo->getPlayerExp() - _totalExp;
            }
            else
            {
                //升级了
                currExps = _totalExp - playInfo->getPlayerExp();
                while (currExps >= 0 && playerNowLevel > 1) {
                    playerNowLevel--;
                    currExps -=  SGStaticDataManager::shareStatic()->getPlayerByLevel(playerNowLevel)->getPlayerExp();
                }
                currExps = -currExps;
            }
            */
			
			CCSprite *proTimerBg = CCSprite::createWithSpriteFrameName("progress.png");
			proTimerBg->setAnchorPoint(ccp(1, 0.5f));
			proTimerBg->setPosition(ccpAdd(labelTTF->getPosition(),ccp(600, 0)));
			proTimerBg->setScaleX(0.78f);
			scrollview->addChild(proTimerBg);
			
			proTimer = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("progress1.png"));
			proTimer->setType(kCCProgressTimerTypeBar);
			proTimer->setAnchorPoint(ccp(1, 0.5f));
			proTimer->setScaleX(proTimerBg->getScaleX());
			proTimer->setMidpoint(ccp(0,0));
			proTimer->setBarChangeRate(ccp(1, 0));
			proTimer->setPosition(ccpAdd(proTimerBg->getPosition(), ccp(-4.5f, 0)));
			scrollview->addChild(proTimer);
			
			float rates = (float)currExps/cMaxExp1;
			proTimer->setPercentage(rates*100);
			
			float wid = proTimerBg->getContentSize().width*.5f * 0.78f;
			string = CCString::createWithFormat("%d/%d", currExps, cMaxExp1);
			labelRate = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
			labelRate->setPosition(ccpAdd(proTimer->getPosition(), ccp(-wid, 0)));
			scrollview->addChild(labelRate);
			
			
			labelTTF = SGCCLabelTTF::create(str_BossRewardLayer_str3, fontName, fontSize);
			labelTTF->setAnchorPoint(ccp(0, 0.5f));
			labelTTF->setPosition(ccp(posx, posy-47*2-50));
			scrollview->addChild(labelTTF);
			//    int currCoins = playInfo->getPlayerCoins();
			//    if (!isLevelUp) playInfo->setPlayerCoins((currCoins+reward->getCoin()));
			
			_totalCoin = reward->getCoin();
			labelCoin = SGCCLabelTTF::create("0", fontName, fontSize);
			labelCoin->setAnchorPoint(ccp(0, 0.5f));
			labelCoin->setPosition(ccpAdd(labelTTF->getPosition(), ccp(labelTTF->getContentSize().width+35, 0)));
			scrollview->addChild(labelCoin);
			
			CCSprite *coincon = CCSprite::createWithSpriteFrameName("coinicon.png");
			float iconPx = labelTTF->getContentSize().width - coincon->getContentSize().width*1.0f;
			coincon->setPosition(ccpAdd(labelTTF->getPosition(), ccp(iconPx+50, 0)));
			scrollview->addChild(coincon);
			
			labelTTF = SGCCLabelTTF::create(str_ActivityMainCell_str15, fontName, fontSize);
			labelTTF->setAnchorPoint(ccp(0, 0.5f));
			labelTTF->setPosition(ccpAdd(ccp(posx, posy-47*2), ccp(320, -50)));
			scrollview->addChild(labelTTF);
			//    int currGolds = playInfo->getPlayerGold();
			//    if (!isLevelUp) playInfo->setPlayerGold((currGolds+reward->getGold()));
			
			_totalGold = reward->getGold();
			labelGold = SGCCLabelTTF::create("0", fontName, fontSize);
			labelGold->setAnchorPoint(ccp(0, 0.5f));
			labelGold->setPosition(ccpAdd(labelTTF->getPosition(), ccp(labelTTF->getContentSize().width+45, 0)));
			scrollview->addChild(labelGold);
			
			CCSprite *goldicon =  CCSprite::createWithSpriteFrameName("goldicon.png");
			goldicon->setPosition(ccpAdd(labelTTF->getPosition(), ccp(iconPx+50, 0)));
			scrollview->addChild(goldicon);
		   
			
//			labelTTF = SGCCLabelTTF::create(str_ActivityMainCell_str16, fontName, fontSize);
//			labelTTF->setAnchorPoint(ccp(0, 0.5f));
//			labelTTF->setPosition(ccp(posx, posy-47*3-50));
//			scrollview->addChild(labelTTF);
			
			//int currJexp = playInfo->getplayerAward();
			//playInfo->setplayerAward((currJexp+reward->getJexp()));
			mainLayer->updateUserMsg();
//			_totalJun = reward->getJexp();
//			labelJun = SGCCLabelTTF::create("0", fontName, fontSize);
//			labelJun->setAnchorPoint(ccp(0, 0.5f));
//			labelJun->setPosition(ccpAdd(labelTTF->getPosition(), ccp(labelTTF->getContentSize().width, 0)));
//			scrollview->addChild(labelJun);
			
		}
		else if (btp == BT_SURVIVAL)//是守荆州的结算描述 by bugcode
		{
#pragma mark "==========================================守荆州奖励界面==========================================="
			//#FEC309
			SGCCLabelTTF *residueBlood = SGCCLabelTTF::create(str_ActivityMainCell_str17, FONT_PANGWA, 26 , ccc3(0xfe, 0xc3, 0x09));
            //modify by:zyc. merge into create.
			//residueBlood->setColor(ccc3(0xfe, 0xc3, 0x09));
			
			residueBlood->setAnchorPoint(ccp(0, 0.5f));
			residueBlood->setPosition(ccp(posx, posy-87));
			scrollview->addChild(residueBlood);
			
			CCSprite *proTimerBg = CCSprite::createWithSpriteFrameName("progress.png");
			proTimerBg->setAnchorPoint(ccp(1, 0.5f));
			proTimerBg->setPosition(ccpAdd(residueBlood->getPosition(), ccp(proTimerBg->getContentSize().width + 55, -5)));
			proTimerBg->setScaleX(0.78f);
			scrollview->addChild(proTimerBg);
			
			proTimer = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("progress1.png"));
			proTimer->setType(kCCProgressTimerTypeBar);
			proTimer->setAnchorPoint(ccp(1, 0.5f));
			proTimer->setScaleX(proTimerBg->getScaleX());
			proTimer->setMidpoint(ccp(0,0));
			proTimer->setBarChangeRate(ccp(1, 0));
			proTimer->setPosition(ccpAdd(proTimerBg->getPosition(), ccp(-4.5f, 0)));
			scrollview->addChild(proTimer);
			
			float rates = (float)reward->getCurHp()	/ reward->getTotalHp();
			proTimer->setPercentage(rates*100);
			
			// 进度条中间的数字
			float wid = proTimerBg->getContentSize().width*.5f * 0.78f;
			string = CCString::createWithFormat("%d/%d", reward->getCurHp(), reward->getTotalHp());
			labelRate = SGCCLabelTTF::create(string->getCString(), FONT_BOXINFO, 22);
			labelRate->setPosition(ccpAdd(proTimer->getPosition(), ccp(-wid, 0)));
			scrollview->addChild(labelRate);
			
			
//			CCSprite *getReward = CCSprite::createWithSpriteFrameName("get_reward.png");
//			getReward->setAnchorPoint(ccp(0, 0.5f));
//			getReward->setPosition(ccp(scrollview->getContentSize().width / 2, posy-47*3-10));
//			scrollview->addChild(getReward);
            
            SGCCLabelTTF *labelReward = SGCCLabelTTF::create(str_OverBattleBox_str1012, FONT_PANGWA, 26 ,ccc3(0xfe , 0xc3 , 0x09));
            //modify by:zyc. merge into create.
			//labelReward->setColor(ccc3(0xfe, 0xc3, 0x09));
			
			labelReward->setAnchorPoint(ccp(0, 0.5f));
			labelReward->setPosition(ccp(posx, posy-187));
			scrollview->addChild(labelReward);

			
			
			
			
		}
	
        tableView->setShowSlider(false);
        //if(datas->count()==1)
        
        this->runRateSp(rateNum);
        
        
        
        SGPlayerInfo::sharePlayerInfo()->setstarnum(rateNum);
    
        SGPlayerInfo::sharePlayerInfo()->setisshowed(false);
        if(rateNum<3)
        
        {
        int id=SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
        
        CCString* starnum= CCString::createWithFormat("starnum%d",id);
        
        int num=CCUserDefault::sharedUserDefault()->getIntegerForKey(starnum->getCString());
     
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey(starnum->getCString(),num+1);
            CCUserDefault::sharedUserDefault()->flush();
        
        }
        
        
        
        this->setTouchEnabled(true);
    }
    
    if ( (btp == BT_SURVIVAL) && win) //守荆州，并且胜利，添加按钮
    {
        
       int  ismax= playInfo->getismaxlevel();
        
        if(ismax==1) ////守荆州最后一关了  没有下一关
        {
            //"点击屏幕继续"
            conSprite =  CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
            tpx = infoBg->getPositionX();
            tpy = bottomH + (infoBg->getPositionY() - infoBg->getContentSize().height*.5f-bottomH)*.5f;
            conSprite->setPosition(ccp(tpx, tpy-20));//-35
            conSprite->setVisible(false);
            this->addChild(conSprite);

            SGCCLabelTTF* labelmax=SGCCLabelTTF::create(str_ActivityMainCell_str18, FONT_BOXINFO, 28 , ccRED);
            labelmax->setPosition(ccpAdd(center, ccp(0, -size.height*0.1 )));

            this->addChild(labelmax);
            SGButton * share = SGButton::create("sharefacebook-3.png",
                                                "sharefacebook-3.png",
                                                this,
                                                menu_selector(SGFightRewardLayer::onClickShareJinZhouButton),
                                                ccp(0, 0),
                                                false,
                                                true);
            //button1->setScale(0.7);
            this->addBtn(share);
            share->setPosition(ccpAdd(conSprite->getPosition(), ccp(0, 20)));
            if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
            {
                share->setVisible(true);
            }else{
            share->setVisible(false);}


        }
        else
        {
            //这是那张老图的尺寸
            float wid = 523;
            float hgt = 423;
			
			

            
            jxcg= SGButton::createFromLocal("an.png", str_ActivityMainCell_str19, this, menu_selector(SGFightRewardLayer::continuechuangguan),
																						CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
			

			
            float scaleY = size.height / 1140;

            this->addBtn(jxcg);
            jxcg->setTag(500);
            jxcg->setPosition(ccp(center.x -wid*0.3 - 30,bottomH - 10 + (infoBg->getPositionY() - infoBg->getContentSize().height*.5f-bottomH)*.5f)) ;
            jxcg->setScaleY(scaleY);
            //jxcg->setScale(1,1);
            
            dhzl= SGButton::createFromLocal("an.png", str_ActivityMainCell_str20, this, menu_selector(SGFightRewardLayer::waitfornext),
																				CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
            dhzl->setPosition(ccp(center.x + wid*0.3 + 30,jxcg->getPosition().y)) ;
            //dhzl->setScale(0.7,0.7);
            dhzl->setScaleY(scaleY);
            dhzl->setTag(501);
            this->addBtn(dhzl);
            conSprite = NULL;
            SGButton * share = SGButton::create("sharefacebook-3.png",
                                                "sharefacebook-3.png",
                                                this,
                                                menu_selector(SGFightRewardLayer::onClickShareJinZhouButton),
                                                ccp(0, 0),
                                                false,
                                                true);
            //button1->setScale(0.7);
            share->setScaleY(scaleY);
            this->addBtn(share);
            share->setPosition(ccp(center.x, jxcg->getPosition().y));
            if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
            {
                share->setVisible(true);
            }else{
                share->setVisible(false);
            }
        }

    }
	else if ( (btp == BT_SURVIVAL) && !win)//守荆州,失败了,直接显示点击继续
	{
		conSprite =  CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
        tpx = infoBg->getPositionX();
        tpy = bottomH + (infoBg->getPositionY() - infoBg->getContentSize().height*.5f-bottomH)*.5f;
        conSprite->setPosition(ccp(tpx, tpy-20));//-35
        conSprite->setVisible(true);
        this->addChild(conSprite);
		
		_isFirstTouch = false;//TouchEnded里直接进confHandler
		
		if (conSprite)
		{
			conSprite->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 2)));
		}
	}
    else if ( (btp != BT_SURVIVAL) && win)//非守荆州胜利则显示点击屏幕继续
    {
        //"点击屏幕继续"
        conSprite =  CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
        tpx = infoBg->getPositionX();
        tpy = bottomH + (infoBg->getPositionY() - infoBg->getContentSize().height*.5f-bottomH)*.5f;
        conSprite->setPosition(ccp(tpx, tpy-20));//-35
        conSprite->setVisible(false);
        this->addChild(conSprite);
        
        if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
        {
            conSprite->setVisible(true);
        }
        else
        {
            int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
            if (guideId >= UIG_MAX1)
            {
                showShareLayer();
            }
            else
            {
                conSprite->setVisible(true);
            }

        }

    }
    else
    {
        //"点击屏幕继续"
        conSprite =  CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
        tpx = infoBg->getPositionX();
        tpy = bottomH + (infoBg->getPositionY() - infoBg->getContentSize().height*.5f-bottomH)*.5f;
        conSprite->setPosition(ccp(tpx, tpy-20));//-35
        conSprite->setVisible(true);
        this->addChild(conSprite);
    }
    
}
void SGFightRewardLayer::continuefight()
{
    confHandler();
}

//显示分享按钮
void SGFightRewardLayer::showShareLayer()
{
    if(datas->count()>1){
        SGButton *btnButton = SGButton::createFromLocal("an.png", str_OverBattleBox_str1011, this, menu_selector(SGFightRewardLayer::continuefight),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(btnButton);
    btnButton->setPosition(ccpAdd(conSprite->getPosition(), ccp(0, 0)));
    btnButton->setScale(0.9);
    return;
    }
    return;     //modified by cgp
    //不开启分享BOX
    if( SGPlayerInfo::sharePlayerInfo()->getShowvip()!=SHOW_SHAREBOX_ID )
    {
        return;
    }
        
    
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    if(btp != BT_SURVIVAL)
    {
        CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
        CCDictionary *dic2 =(CCDictionary *) dic->objectForKey("25");
        int nyuanbao =dic2->valueForKey("value")->intValue();
        CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("26");
        int ntongqian =dic1->valueForKey("value")->intValue();

    SGCCLabelTTF*label3 = NULL;
    
    label3 = SGCCLabelTTF::create(str_SGShareBox_str3, FONT_PANGWA, 30 , ccWHITE , ccBLACK);
    //modify by:zyc  merge into create.
    //label3->setOutSideColor(ccc3(0,0,0));
    label3->setPosition(ccpAdd(conSprite->getPosition(), ccp(20, 30)));
    this->addChild(label3);
    CCSprite *yubao = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yubao);
    yubao->setPosition(ccpAdd(label3->getPosition(),ccp(-label3->getContentSize().width/2,-35)));
    yubao->setScale(1);
    
    SGCCLabelTTF*label4 = NULL;
    
    label4 = SGCCLabelTTF::create(CCString::createWithFormat("x%d",nyuanbao)->getCString(), FONT_PANGWA, 30 , ccGREEN , ccBLACK);
        //modify by:zyc . merge into create.
    //label4->setOutSideColor(ccc3(0,0,0));
    //label4->setColor(ccc3(0,255,0));
    label4->setPosition(ccpAdd(yubao->getPosition(), ccp(label4->getContentSize().width/2+yubao->getContentSize().width/2, 0)));
    this->addChild(label4);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int level = playerInfo->getPlayerLevel();
    
    CCSprite *tongqian = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(tongqian);
    tongqian->setPosition(ccpAdd(label4->getPosition(),ccp(label4->getContentSize().width/2+tongqian->getContentSize().width/2,0)));
    tongqian->setScale(1);
    
    SGCCLabelTTF*label5 = NULL;
    
    label5 = SGCCLabelTTF::create(CCString::createWithFormat("x%d",ntongqian)->getCString(), FONT_PANGWA, 30 , ccGREEN , ccBLACK);
//modify by:zyc . merge into create.
//    label5->setOutSideColor(ccc3(0,0,0));
//    label5->setColor(ccc3(0,255,0));
    label5->setPosition(ccpAdd(tongqian->getPosition(), ccp(label5->getContentSize().width/2+tongqian->getContentSize().width/2, 0)));
    this->addChild(label5);
    }
    
    SGButton * share = SGButton::create("sharefacebook-3.png",
                                                            "sharefacebook-3.png",
                                                            this,
                                                            menu_selector(SGFightRewardLayer::onClickShareButton),
                                                            ccp(0, 0),
                                                            false,
                                                            true);
    share->setScale(0.7);
    this->addBtn(share);
    if(btp != BT_SURVIVAL)
    {
        share->setPosition(ccpAdd(conSprite->getPosition(), ccp(-200, 15)));
    }else{
        share->setPosition(ccpAdd(conSprite->getPosition(), ccp(0, 20)));

    }

    

}
//响应分享按钮点击
void SGFightRewardLayer::onClickShareJinZhouButton()
{
    CCLOG("创建分享对话框");
    if(!SGGuideManager::shareManager()->isGuide)
    {
        SGShareBox *box = SGShareBox::create(this,SGShareTypeShoujingzhou);
        SGMainManager::shareMain()->showBox(box);
    }
}

void SGFightRewardLayer::onClickShareButton()
{
    CCLOG("创建分享对话框");
        if(!SGGuideManager::shareManager()->isGuide)
    {
        SGShareBox *box = SGShareBox::create(this,SGShareTypeGeneralFight);
        SGMainManager::shareMain()->showBox(box);
    }
}
//收取到好友武将信息
void SGFightRewardLayer::getFriendOfficerListener(CCObject *sender)
{
	SGMainManager *sGMainManager = SGMainManager::shareMain();
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::FriendInfoResponse *friRes = (main::FriendInfoResponse *)req->m_msg;
		if (friRes)
		{
			SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(friRes->role());
			SGOfficerCard *officer = playerInfo->getLordOfficer();
            
            HelpFriendRoleName=playerInfo->getNickName()->getCString();
            HelpFriendItemLevel=playerInfo->getLevel();
            HelpFriendItemId=playerInfo->getLordid();
            HelpFriendItemLevel=playerInfo->getLevel();
            if(!officer)
            {
                this->gobackToStoryLayer();
                return;
            }
            
			officer->retain();
			
			// 此处获得好友武将卡的信息
			SGGeneralBox *box = SGGeneralBox::create(NULL, 1, officer);
			box->setLdIndex(ldanCount);
			sGMainManager->closeBox();
			sGMainManager->showBox(box);
		}
	}
	
}



// confirm action
void SGFightRewardLayer::confHandler()
{
    //TODO: 守荆州的特殊奖励逻辑！
    // 先弹是否升级界面
    // 如果没升级，弹是否添加援军为好友
    // 如果没升级，也不需添加好友，弹是否进入炼丹炉
    // 如果没有以上，①若是守荆州胜利，则等待响应按钮。②若是守荆州失败，直接进入军魂换礼界面。
    // 如果以上界面都没有，那么重新进入关卡界面
    
//    if (entertype==1) {
//        showBossDetailLayer();
//        return;
//    }
    
    SGMainManager *sGMainManager = SGMainManager::shareMain();
    
    //升级了,升级界面
    int currLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if (currLev > HelpFriendPlayLev && 1) {
        DDLog("ccc1=================playLev");
        SGLevelUpLayer *layer = SGLevelUpLayer::create();
        layer->_index = ldanCount;
        
//        layer->setScale(.4);
//        layer->runAction(CCScaleTo::create(ITEMACTIONTIME, 1));
        sGMainManager->showLayer(layer);
        sGMainManager->trackLevel(currLevel);
        //        SGGeneralBox *box = SGGeneralBox::create(NULL, 3);
        //        box->setLdIndex(ldanCount);
        //        SGMainManager::shareMain()->closeBox();
        //        SGMainManager::shareMain()->showBox(box,false);
  
        //检测等级的时候，如果在引导之中，则不再激活这个引导
        SGGuideManager::shareManager()->setLimitGuideTag(SGPlayerInfo::sharePlayerInfo()->checkStartLimitGuide(1));
        return;
    }
    
    //守荆州，建议与炼丹炉交换层级
    //根据战斗类型来判断奖励界面，以后可能添加PVE:闯关/守荆州+PVP:天梯/竞技场等不同的导向。
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    bool win = SGBattleManager::sharedBattle()->isWin;
    if (btp == BT_SURVIVAL)
    {
        if (win) //赢了等待玩家点击
        {
            return;
        }
        else //败了进入军魂界面
        {
            //发送获取详情
            SGMainManager::shareMain()->sendGetSjzInfoRequest(true);
        }
    }
    else
    {
        //非好友，添加好友
        if (SGAIManager::shareManager()->isInitPve && SGFriend::isFriendOfPlayer(HelpFriendRoleId) != SGFRIEND_FRIEND \
																		&& HelpFriendRoleId > 0 && isInMyFriendList)
		{
            DDLog("ccc2=================playLev");
			SGMainManager::shareMain()->getFriendOfficerInfo();// 发送消息
			
//            SGGeneralBox *box = SGGeneralBox::create(NULL, 1);
//            box->setLdIndex(ldanCount);
//            sGMainManager->closeBox();
//            sGMainManager->showBox(box);
            return;
        }
        
        //是否西域商人
        if(m_isShowExoticMerchant)
        {
            SGBaseBox * box = SGFightOverNoticeBox::create(NULL, 3);
            SGExoticMerchantLayer::setNoticeTime(3600); //MM: 这个HACK是为了让红点走正时逻辑。
            SGExoticMerchantLayer::setIsForcedOpen(true);
            sGMainManager->closeBox();//???也许是防止同时出现2个box.
            sGMainManager->showBox(box);
            return ;
        }
        
        //可以开启炼丹炉;
        if (ldanCount >= 0)
        {
            DDLog("ccc3=================playLev");
//            SGGeneralBox *box = SGGeneralBox::create(NULL, 2);
            SGFightOverNoticeBox * box = SGFightOverNoticeBox::create(NULL, 4);
            box->setLdIndex(ldanCount);
            sGMainManager->closeBox();
            sGMainManager->showBox(box);
            return;
        }
        
        DDLog("ccc4=================playLev");
        // 如果上面这些box都没有，直接返回到原来的界面
        this->gobackToStoryLayer();
    }
    
  
}
void SGFightRewardLayer::showTheStorySectionListener(CCObject * sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::BossPlotListResponse *response = (main::BossPlotListResponse *)req->m_msg;
    CCLOG("scene=================%d", response->bossplotmodel_size());
    if (response) {
        int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
				//精英副本
		
			CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
            for (int ii = 0; ii < array->count(); ii++) {
                 SGStoryBean *story = (SGStoryBean *)array->objectAtIndex(ii);
                if (story->getSceneBeanById(plotId) != NULL) {
                    
                    int size = response->bossplotmodel_size();
                    if (size > 0) {
                        for (int iii = 0; iii < size; iii++) {
                            main::BossPlotModel model = response->bossplotmodel(iii);
                            CCLOG("scene ID=================%d", model.plotid());
                            SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
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
                                
                                scene->setCreamDayTimes(model.canjoincount());
                                scene->setCreamBuyTimes(model.canbuycount());
                                scene->setBuyPrice(model.price());
                                scene->setBuyCompTimes(model.buycount());// 已经购买次数
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
                    SGMainManager::shareMain()->showStorySectionLayer(story);
                    
                    
                    break;
                }
            }

               
    }

}


//每次打完活动副本要重新向服务器要新的数据
//actPlotId是全局变量是为了给这个函数使用请求新的数据使用的
void SGFightRewardLayer::activityListener(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::ActivityListResponse *response = (main::ActivityListResponse *)req->m_msg;
    if (response)
    {
        SGStoryBean *story = NULL;
        
        
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getActivityData();
        array->retain();
        for (int ii = 0; ii < array->count(); ii++)
        {
            SGStoryBean *activity = (SGStoryBean *)array->objectAtIndex(ii);
            if (activity->getSceneBeanById(actPlotId) != NULL)
            {
                story = activity;
                break;
            }
        }
        
        int size = response->plotmodel_size();
        if (size > 0)
        {
            for (int ii = 0; ii < size; ii++)
            {
                main::PlotModel model = response->plotmodel(ii);
                CCLog("scene ID=================%d", model.plotid());
                SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                if (scene == NULL)
                {
                    continue;
                }
                else
                {
                    scene->setVisible(true);
                    scene->setRank(model.rank());
                    scene->setOpenState(model.openstate());
                    scene->setPower(model.power());
                    scene->setZhandou(model.zhandou());
                    scene->setExp(model.exp());
                    scene->setGold(model.gold());
                    
                    //加入对应每个场景的可挑战次数
                    story->setChallengeCount(model.joincount());
                    scene->setCreamDayTimes(model.joincount());
                    
                    int itemNum = model.itemids_size();
                    scene->itemIds.clear();
                    for (int jj =0;jj<itemNum; jj++)
                    {
                        scene->itemIds.push_back(model.itemids(jj));
                    }
                }
            }
        }
        SGMainManager::shareMain()->m_trackIsPve = false;
        SGMainManager::shareMain()->showStorySectionLayer(story);
    }
}


void SGFightRewardLayer::gobackToStoryLayer()
{
    SGMainManager *sgMainManager = SGMainManager::shareMain();
    // back to story-scene layer, go on battle
    sgMainManager->closeBox();
    
    // if lost the game, pop lost guide box
    if (SGBattleManager::sharedBattle()->isWin == false)
    {
        /*boxState: 1=战斗失败 2=战斗未三星 3=西域商人 4=炼丹秘境*/
        //removed by cgp to delete the gotoAction of fail.
        //SGFightOverNoticeBox * box = SGFightOverNoticeBox::create(NULL, 1);
        //sgMainManager->showBox(box);
        
        SGMainManager::shareMain()->showFirstLayer();  //added by cgp
        
        SGBattleManager::sharedBattle()->isWin = true;
        return;
    }
    ////////////////////////////////////////////////////////////////////////
    int mmmm= SGPlayerInfo::sharePlayerInfo()->getstarnum();
    int PlotId =  SGPlayerInfo::sharePlayerInfo()->getPlotId();
    
    //在pvp的情况下不显示三星未满的强化提示, 只有在pve的情况下才会出这个三星未满的提示
    if(SGPlayerInfo::sharePlayerInfo()->getisshowed()==false &&
       SGAIManager::shareManager()->isPVE  &&
       SGGuideManager::shareManager()->getLimitGuideSize() <= 0)
    {
        if(mmmm<3&&PlotId<20000)
        {
            int  nShowtipBox  =  CCUserDefault::sharedUserDefault()->getIntegerForKey("isShowtipBoxTimes");
            if(nShowtipBox == 0)
                nShowtipBox = 6;
            if(nShowtipBox>1)
            {
                CCUserDefault::sharedUserDefault()->setIntegerForKey("isShowtipBoxTimes", --nShowtipBox);
                CCUserDefault::sharedUserDefault()->flush();
                SGPlayerInfo::sharePlayerInfo()->setisshowed(true);
                
                //removed by cgp
//                SGBaseBox * box = SGFightOverNoticeBox::create(NULL, 2);
//                sgMainManager->showBox(box);
                SGMainManager::shareMain()->showFirstLayer();  //added by cgp
                return;
            }
            
        }
    }
    
    
    if (SGAIManager::shareManager()->isInitPve)
    {
        int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
		bool haveShowPlot = false;
        
        //特殊战斗
        if (plotId >= 30000)
        {
            //重新请求对应的章
            int storyId = SGMainManager::shareMain()->getSpBattleChapId();
            main::SpecialBattlePlotListRequest *plotList = new main::SpecialBattlePlotListRequest;
            plotList->set_storyid(storyId);
            SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_SEC, plotList);
        }
		//精英副本
		else if(plotId >= 20000)
		{
			CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
            for (int ii = 0; ii < array->count(); ii++) {
                SGStoryBean *bossplot = (SGStoryBean *)array->objectAtIndex(ii);
                if (bossplot->getSceneBeanById(plotId) != NULL)
                {

                    main::BossPlotListRequest *request = new main::BossPlotListRequest();
                    request->set_storyid(bossplot->getStoryId());
                    SGSocketClient::sharedSocketClient()->send(MSG_CREAM_STORY, request);
                    
                    haveShowPlot = true;
                    break;
                }
            }
            if (!haveShowPlot)
            {
				//请求最新的精英副本列表
				main::BossStoryListRequest *request = new main::BossStoryListRequest();
				SGSocketClient::sharedSocketClient()->send(MSG_CREAM_BATTLE, request);
            }
            //这场战斗的bean
            sgMainManager->trackMsnCompleted(plotId,false);
            
		}
        else if (plotId > 10000)
        {
            // 活动模式
            actPlotId = plotId;
            //首先请求activity的最新数据
            
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getActivityData();
            array->retain();
            for (int ii = 0; ii < array->count(); ii++)
            {
                SGStoryBean *activity = (SGStoryBean *)array->objectAtIndex(ii);
                if (activity->getSceneBeanById(plotId) != NULL)
                {
                    SGMainManager::shareMain()->m_trackIsPve = false;
                    
                    haveShowPlot = true;
                    
                    //重新请求活动关卡数据
                    main::ActivityListRequest *request = new main::ActivityListRequest();
                    request->set_storyid(activity->getStoryId());
                    SGSocketClient::sharedSocketClient()->send(MSG_STORY_ACT_SCENE, request);
                    
                    break;
                }
            }
            if (!haveShowPlot) {
                sgMainManager->showStorySectionLayer(NULL);
            }
            //这场战斗的bean
            sgMainManager->trackMsnCompleted(plotId,false);
            
        } else { // 故事模式
            SGStoryBean *bean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(plotId);
            if (bean != NULL) {
                CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
                SGStoryBean *story = NULL;
                if (array == NULL) { //战斗中断
                    story = SGStaticDataManager::shareStatic()->getStoryDataById(1);
                }else {
                    for (int ii = 0; ii < array->count(); ii++) {
                        story = (SGStoryBean *)array->objectAtIndex(ii);
                        if (bean->getStoryId() == story->getStoryId()) {
                            break;
                        }
                    }
                }
                if(story != NULL) {
                    main::PlotListRequest *request = new main::PlotListRequest();
                    request->set_storyid(story->getStoryId());
                    request->set_poltaskid(story->getStoryId());
                    SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
                }
            }else {
                sgMainManager->showStoryMainLayer();
            }
            //这场战斗的bean
            sgMainManager->trackMsnCompleted(plotId-1);
        }
    }
    else
    {
        SGBattleLayer *btlayer = (SGBattleLayer *)SGBattleManager::sharedBattle()->getBattleLayer();
        if (btlayer) {
            //            btlayer->removeAllChildrenWithCleanup(true); //masaga?
            SGMainScene *mainScene =  sgMainManager->getMainScene();
            mainScene->removeChildByTag(sg_battleMainLayer, true); // 删掉，重新刷新
            
            SGFightRewardLayer*  rewardLayer = (SGFightRewardLayer*) sgMainManager->getMainScene()->getChildByTag(sgMainManager->getMainScene()->nowLayerTag);
            
            if (rewardLayer)
            {
                rewardLayer->setReqPvpMain(0);
                rewardLayer->schedule(schedule_selector(SGFightRewardLayer::delayShowPvpMain), 0.2);
            }
           
        }
        else
        {
            // do nothing
        }
        
        //        main::PvpEntryRequest *request = new main::PvpEntryRequest();
        //        SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
        
        // 重新登陆原来的IP
        std::string serverip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
        short serverpost = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
        SGSocketClient::sharedSocketClient()->startConn(serverip.c_str(), serverpost);
        
    }
    
    
}
void SGFightRewardLayer::stopRequestPvpMain()
{
  this->unschedule(schedule_selector(SGFightRewardLayer::delayShowPvpMain));
}

void SGFightRewardLayer::delayShowPvpMain()
{
    m_reqPvpMain++;
    if (m_reqPvpMain >= 6)
    {
        this->unschedule(schedule_selector(SGFightRewardLayer::delayShowPvpMain));
        SGMainManager::shareMain()->showFirstLayer();
    }
    else
    {
        SGMainManager::shareMain()->showPvpMainLayer();
    }
    
}

//@NOTICE: 守荆州&胜利，也添加触控，但是后续流程交由玩家处理。
void SGFightRewardLayer::registerWithTouchDispatcher()
{
//    if (SGBattleManager::sharedBattle()->getBattleType() == BT_SURVIVAL && SGBattleManager::sharedBattle()->isWin)
//        return;
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);//11.17
}

SNSTableViewCellItem* SGFightRewardLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	SGStoryRewardItem *item = (SGStoryRewardItem *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL)
	{
        item = SGStoryRewardItem::create(soulNum, 0, sg_fightrewardLayer);
    }
    if (index < datas->count())
	{
        SGStoryRdItem *model = (SGStoryRdItem *)datas->objectAtIndex(index);
        //item->setData(model->getItemId(), model->getItemTag(), model->getItemType(), soulNum);
        //item->setDataWithArray(datas);
    }
	else
	{
        item->setVisible(false);
    }
    return item;
}

unsigned int SGFightRewardLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}
void SGFightRewardLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    // 显示卡牌信息
}
unsigned int SGFightRewardLayer::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
    return 1;
	
	if (SGBattleManager::sharedBattle()->getBattleType() == BT_SURVIVAL)//如果战斗类型是守荆州
	{
		int num = datas->count();
		
		if (num >= 5 || num == 0)
		{
			return 5;
		}
		else
		{
			return num;
		}
	}
	else
	{
		return 5;
	}
	

}
bool SGFightRewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    isMoved=false;
    return true;
    
}
void SGFightRewardLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

    isMoved=true;
}

#define ADJUST_ANIM_VELOCITY 1000
void SGFightRewardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
    //////////////////////////////
//    m_rewardScrollView->unscheduleAllSelectors();
//    
//    int y = m_rewardScrollView->getContentOffset().y;
//    int offset = (int) y % 220;
//    // 调整位置
//    CCPoint adjustPos;
//    // 调整动画时间
//    float adjustAnimDelay;
//    
//    if (offset < -100) {
//        // 计算下一页位置，时间
//        adjustPos = ccpSub(m_rewardScrollView->getContentOffset(), ccp(0, 220 + offset));
//        adjustAnimDelay = (float) (220 + offset) / ADJUST_ANIM_VELOCITY;
//    }
//    else {
//        // 计算当前页位置，时间
//        adjustPos = ccpSub(m_rewardScrollView->getContentOffset(), ccp(0, offset));
//        // 这里要取绝对值，否则在第一页往左翻动的时，保证adjustAnimDelay为正数
//        adjustAnimDelay = (float) abs(offset) / ADJUST_ANIM_VELOCITY;
//    }
//    
//    // 调整位置
//    m_rewardScrollView->setContentOffsetInDuration(adjustPos, adjustAnimDelay);
////////////////////////////////////////////
    
    
    
    
    CCPoint startpoint=pTouch->getStartLocation();
    CCPoint endpoint=pTouch->getLocation();
    CCRect rect=CCRectMake(tableView->getPosition().x, tableView->getPosition().y, tableView->getContentSize().width,
                           tableView->getContentSize().height);
    if (isMoved && datas->count()>0)
    {
        if (rect.containsPoint(startpoint))
        {
            return;
        }
        
    }
    if(datas->count()>1)
    {
        return;
    }
    if (_isFirstTouch  && _isFinishStar)
    {
        _isFirstTouch = false;

        _isFinishStar=true;
        
		// 如果是守荆州,没有这些动作
		if (SGBattleManager::sharedBattle()->getBattleType() != BT_SURVIVAL)
		{
			this->setAllLabels();
		}
        
        if(m_jump)
        {
            _isFirstTouch=true;//////守荆州多次跳过
            _sjzcannottouch=true;
        }

        
        //if (conSprite)
            //conSprite->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 2)));

        int  ismax= playInfo->getismaxlevel();
        
        if(ismax==1) ////守荆州最后一关了  没有下一关
        {
            _isFirstTouch=false;
        }
        
    }
    else if(!_isFirstTouch && _isFinishStar)
    {
        DDLog("cc========================touch");
        if (conSprite)
        {
            //conSprite->stopAllActions();
            conSprite->setVisible(false);
        }
        
        
        
        int  ismax= playInfo->getismaxlevel();
        if(ismax==1) ////守荆州最后一关了  没有下一关
        {
            ////////重设数据  不然可能会悲剧
            playInfo->setismaxlevel(0);
            this->sjztongguan();

        }
        else
        {
			this->confHandler();
  
            this->setTouchEnabled(false);
            
        }
    }

}

void SGFightRewardLayer::setAllLabels()
{
    if(_sjzcannottouch)
    {
        return;
    }
    this->stopActionByTag(10010);
    this->stopActionByTag(10011);
    this->stopActionByTag(10012);
    this->stopActionByTag(10013);
    this->stopActionByTag(10014);
    this->stopActionByTag(10015);
    this->stopActionByTag(10016);
    this->stopActionByTag(10017);
    this->stopActionByTag(10018);
//     setStar(rateNum);
    
    CCString *string = NULL;
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    int coin =0;
    int gold =0;
    if (btp==BT_SURVIVAL) {
        coin = player->getPlayerCoins()+ _addCoin+_totalCoin;/////kanata
        gold = player->getPlayerGold() + _addGold+_totalGold;

    }
    else
    {
        coin = player->getPlayerCoins()+ _addCoin+_totalCoin;/////kanata
        gold = player->getPlayerGold()+ _addGold+_totalGold;

    }
    SGMainLayer *mainLayer = SGMainManager::shareMain()->getMainLayer();
    string = CCString::createWithFormat("%d", coin);
    //mainLayer->copper->setString(string->getCString());
    string = CCString::createWithFormat("%d", gold);
    mainLayer->gold->setString(string->getCString());
    
    //mm: 由于玩家"107信息更新"消息在获得奖励和升级前后的接收时间不同，故这里需要重新计算最终的数exp数值。
    SGStaticDataManager* sgsdm = SGStaticDataManager::shareStatic();
    int level = _rewardData->getBeforeLevel();
    int curExp = _rewardData->getExp();
    int addExp = _rewardData->getExtExp();
    
    while (addExp > 0)
    {
        int temp = sgsdm->getPlayerByLevel(level)->getPlayerExp() - curExp;
        if (temp > addExp)
        {
            curExp += addExp;
            addExp = 0;
        }
        else //升级
        {
            level++;
            addExp -= temp;
            curExp = 0;
        }
    }
    int nowExps = curExp;
    int levelExps = sgsdm->getPlayerByLevel(level)->getPlayerExp();
    
    mainLayer->level->setString(CCString::createWithFormat("%d", level)->getCString());
    
    float rates = 100.0 * nowExps / levelExps ;
    string=CCString::createWithFormat("%d/%d", nowExps,levelExps);
    labelRate->setString(string->getCString());
    proTimer->setPercentage(rates);
    mainLayer->expprog->setPercentage(rates);
    
    //playInfo->setPlayerCoins(coin);
    playInfo->setPlayerGold(gold);
    
    string = CCString::createWithFormat("%d",_totalExp2);
    labelExp->setString(string->getCString());
    string = CCString::createWithFormat("%d", _addCoin+_totalCoin);
    labelCoin->setString(string->getCString());
    string = CCString::createWithFormat("%d", _addGold+_totalGold);
    labelGold->setString(string->getCString());
    string = CCString::createWithFormat("%d", _addJun+_totalJun);
//    labelJun->setString(string->getCString());
    mainLayer->updateUserMsg();
    
    
    //////////////
    _sjzcannottouch=false;
}

void SGFightRewardLayer::finishShowStar(){
    _isFinishStar=true;
}
void SGFightRewardLayer::runRateSP1(CCObject *obj,int num){
    
    CCLog("%dddddddddddddddddddd",num);
    if (num==1) {
        star3->setVisible(true);

        star3->setScale(0.1);
        CCScaleTo *scaleTo=CCScaleTo::create(0.1, 1.5);
        CCScaleTo *scaleTo1=CCScaleTo::create(0.05, 1.0);
        star3->runAction(CCSequence::create(scaleTo,CCDelayTime::create(0.01f),scaleTo1,NULL));
        
    }else if(num==2)
    {
        star2->setVisible(true);
        star2->setScale(0.1);
        CCScaleTo *scaleTo=CCScaleTo::create(0.1f, 1.5f);
        CCScaleTo *scaleTo1=CCScaleTo::create(0.05f, 1.0f);
        CCMoveTo *moveTo1=CCMoveTo::create(0.05f, ccpAdd(star2->getPosition(), ccp(50, 0)));
        CCSpawn *span=CCSpawn::create(scaleTo,moveTo1,NULL);
        
        CCSequence *seq=CCSequence::create(span,CCDelayTime::create(0.02f),scaleTo1,NULL);
        star2->runAction(seq);
        
    }else if(num==3)
    {
        star1->setVisible(true);
        star1->runAction(CCMoveTo::create(0.05f, ccpAdd(star1->getPosition(), ccp(-50, 0))));
        
        
    }else if(num==4)
    {
        star2->setVisible(true);
        star2->runAction(CCMoveTo::create(0.05f, ccpAdd(star2->getPosition(), ccp(50, 0))));
        
    }else
    {
        star1->setVisible(true);
        star1->setScale(0.1);
        CCScaleTo *scaleTo=CCScaleTo::create(0.1f, 1.5);
        CCScaleTo *scaleTo1=CCScaleTo::create(0.05f, 1.0);
        star1->runAction(CCSequence::create(scaleTo,CCDelayTime::create(0.01),scaleTo1,NULL));
        
    }
   
    
}
void SGFightRewardLayer::setStar(int rate){
    
    star1->setVisible(false);
    star2->setVisible(false);
    star3->setVisible(false);
    
    star1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(100, -130)));
    star2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(100, -130)));
    star3->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(100, -130)));
    
    
    if (rate==0) {
        return;
    }else if (rate==1)
    {
        star1->setVisible(true);
        star1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -230)));
    }
    else if(rate==2){
    
        star1->setVisible(true);
        star2->setVisible(true);
        star1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-50, -230)));
        star2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(50, -230)));
      
    }
    
    else
      {
          star1->setVisible(true);
          star2->setVisible(true);
          star3->setVisible(true);
       
          star1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-125, -230)));
          star2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(100, -230)));
          star3->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -230)));
      }

}
void SGFightRewardLayer::runRateSp(int rate)
{
    if(datas->count()>1)
    {
        _isFinishStar=true;
        setAllLabels();
        return;
    }

	//守荆州里没有经验和金币
	if (SGBattleManager::sharedBattle()->getBattleType() != BT_SURVIVAL)
	{
		fuckLayer();
    }
	if (rate==0) {
        _isFinishStar=true;
        return;
    }
    
    star1=CCSprite::createWithSpriteFrameName("reward_star.png");
    star1->setPosition(ccp(400, -10));
    star2=CCSprite::createWithSpriteFrameName("reward_star.png");
    star2->setPosition(ccp(400, -10));
    star3=CCSprite::createWithSpriteFrameName("reward_star.png");
    star3->setPosition(ccp(400, -10));
   
    star3->setVisible(false);
    star2->setVisible(false);
    star1->setVisible(false);
    
 
    scrollview->addChild(star1,3,1001);
    scrollview->addChild(star2,2,1002);
    scrollview->addChild(star3,1,1003);

    if (rate==3) {
        CCCallFuncND *call=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)9);
        CCCallFuncND *call1=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)3);
        CCCallFuncND *call2=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)2);
        CCCallFuncND *call3=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)3);
        CCCallFuncND *call4=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)4);
        CCCallFuncND *call5=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)1);
        CCCallFunc *call6=CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::finishShowStar));
        CCSequence *sequence=CCSequence::create(call,CCDelayTime::create(0.05),call1,CCDelayTime::create(0.35),call2,CCDelayTime::create(0.3),call3,call4,CCDelayTime::create(0.1),call5,CCDelayTime::create(0.3),call6,NULL);
        this->runAction(sequence);

        CCLog("33333333333333333");
        
    }else if(rate==2){
        CCCallFuncND *call=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)9);
        CCCallFuncND *call1=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)3);
        CCCallFuncND *call2=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)2);
         CCCallFunc *call3=CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::finishShowStar));
        CCSequence *sequence=CCSequence::create(call,CCDelayTime::create(0.3),call2,CCDelayTime::create(0.05),call1,CCDelayTime::create(0.3),call3,NULL);
        this->runAction(sequence);
        
      CCLog("222222222222222");
    }
    else{
   
        CCCallFuncND *call=CCCallFuncND::create(this, callfuncND_selector(SGFightRewardLayer::runRateSP1), (void *)9);
        CCCallFunc *call3=CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::finishShowStar));
        this->runAction(CCSequence::create(call,CCDelayTime::create(0.3),call3,NULL));
      CCLog("11111111111111111");
    }
//
  
    
   
//    winSprite->setScale(2.5);
//    CCAction *ac = CCSequence::create(CCDelayTime::create(0.15f),
//                                      CCFadeIn::create(0.0005f),
//                                      CCScaleTo::create(0.15f, 1.0f),
//                                      CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::fuckLayer)),
//                                      NULL);
//    winSprite->runAction(ac);
}

void SGFightRewardLayer::runExpLabel()
{
    int randNum = 1;
    float randN = CCRANDOM_0_1();
    int value = _addExp + _totalExp;
    
    if (value >= 10000) {
        randNum = randN*10 + 100;
    }else if (value >= 7500 && value < 10000) {
        randNum = randN*10 + 75;
    }else if (value >= 5000 && value < 7500) {
        randNum = randN*10 + 50;
    }else if (value >= 2500 && value < 5000) {
        randNum = randN*10 + 35;
    }else if (value >= 1000 && value < 2500) {
        randNum = randN*10 + 25;
    }else if (value >= 500 && value < 1000) {
        randNum = randN*10 + 3;
    }else if (value >= 250 && value < 500) {
        randNum = randN*5 + 3;
    }else if (value >= 100 && value < 250) {
        randNum = randN*3 + 1;
    }
    
//    if (runExpIdx < _expPrgVec.size()) //还需runexp
//    {
//        int left = 0;
//        int right = 0;
//
//        if (randNum > _expPrgVec.at(runExpIdx).first)
//        {
//            left = randNum;
//            right = _expPrgVec.at(runExpIdx).second;
//        }
//        else
//        {
//            left =
//        }
//    }
//    else //结束runexp
//    {
//        
//    }
//    

    
    if (_totalExp > 0) {
        
        if (_totalExp-randNum<=0) {
            _addExp+= _totalExp;
            _addExp1+=_totalExp;
            _totalExp=0;
            
        }else
        {
            _addExp += randNum;//增加的经验
            _totalExp -= randNum;//增加的总经验
            _addExp1=_addExp1+randNum;
        }
      
        CCString *str = CCString::createWithFormat("%d", _addExp1);
        labelExp->setString(str->getCString());
        
        
        float rates = 0;
        int currExp = _addExp;
        int showExp=SGStaticDataManager::shareStatic()->getPlayerByLevel(currLevel)->getPlayerExp();
        
        
        if (currExp>=showExp) {//11.25
            currLevel++;
            currExp-=showExp;
            _addExp-=showExp;
            currExps=currExp;
            showExp=SGStaticDataManager::shareStatic()->getPlayerByLevel(currLevel)->getPlayerExp();
            mainLayer->updateUserMsg();
            
        }
        rates=(currExp/(float)showExp);
        str=CCString::createWithFormat("%d/%d", currExp , showExp);
        currExps=currExp;
        
        labelRate->setString(str->getCString());
        proTimer->setPercentage(100*rates);
        mainLayer->expprog->setPercentage(100*rates);
       

        CCAction *action = CCSequence::create(CCDelayTime::create(runTime),
                                              CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runExpLabel)),
                                              NULL);
        action->setTag(10011);
        this->runAction(action);
    }
    else
    {
        //playInfo->setPlayerExp(currExps); 
        mainLayer->updateUserMsg();
  
//        this->stopAllActions();
        CCAction *action1 = CCSequence::create(CCDelayTime::create(runTime),
                                               CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runCoinLabel)),
                                               NULL);
        action1->setTag(10012);
        this->runAction(action1);
        CCAction *action2 = CCSequence::create(CCDelayTime::create(runTime),
                                               CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runGoldLabel)),
                                               NULL);
        action2->setTag(10013);
        this->runAction(action2);
    }
  
}
void SGFightRewardLayer::runCoinLabel()
{
    
    int randNum = 1;
    float randN = CCRANDOM_0_1();
    int value = _addCoin + _totalCoin;
    if (value >= 1000000) {
        randNum = randN*10 + (int)(value/5);
    }else if (value >= 100000) {
        randNum = randN*10 + (int)(value/10);
    }else if (value >= 10000) {
        randNum = randN*10 + (int)(value/100);
    }else if (value >= 7500 && value < 10000) {
        randNum = randN*10 + 75;
    }else if (value >= 5000 && value < 7500) {
        randNum = randN*10 + 50;
    }else if (value >= 2500 && value < 5000) {
        randNum = randN*10 + 35;
    }else if (value >= 1000 && value < 2500) {
        randNum = randN*10 + 25;
    }else if (value >= 500 && value < 1000) {
        randNum = randN*10 + 3;
    }else if (value >= 250 && value < 500) {
        randNum = randN*5 + 3;
    }else if (value >= 100 && value < 250) {
        randNum = randN*3 + 1;
    }
    
    if (_totalCoin > 0) {
        _addCoin += randNum;
        _totalCoin -= randNum;
        CCString *str = CCString::createWithFormat("%d", _addCoin);
        labelCoin->setString(str->getCString());
        //kanata
        str = CCString::createWithFormat("%d", _addCoin + currCoin);
        mainLayer->copper->setString(str->getCString());
        
        CCAction *action = CCSequence::create(CCDelayTime::create(runTime),
                                              CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runCoinLabel)),
                                              NULL);
        action->setTag(10014);
        this->runAction(action);
    }else {
        CCString *str = CCString::createWithFormat("%d", _addCoin+_totalCoin);
        labelCoin->setString(str->getCString());
    //kanata
        int playCoin = currCoin+_addCoin+_totalCoin;
        str = CCString::createWithFormat("%d", playCoin);
        //mainLayer->copper->setString(str->getCString());
        //playInfo->setPlayerCoins(playCoin);
        mainLayer->updateUserMsg();
        
        if ((_totalGold+_addGold) <= (_totalCoin+_addCoin)) {
            CCAction *action = CCSequence::create(CCDelayTime::create(runTime),
                                                  CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runJunLabel)),
                                                  NULL);
            action->setTag(10015);
            this->runAction(action);
        }
    }
}
void SGFightRewardLayer::runGoldLabel()
{
    if (_totalGold > 0) {
        _addGold += 1;
        _totalGold -= 1;
        CCString *str = CCString::createWithFormat("%d", _addGold);
        labelGold->setString(str->getCString());
        
        str = CCString::createWithFormat("%d", currGold+_addGold);
        mainLayer->gold->setString(str->getCString());

        CCAction *action = CCSequence::create(CCDelayTime::create(runTime),
                                              CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runGoldLabel)),
                                              NULL);
         action->setTag(10016);
        this->runAction(action);
    }else {
        CCString *str = CCString::createWithFormat("%d", _addGold+_totalGold);
        labelGold->setString(str->getCString());
        
        int playGold = currGold+_addGold+_totalGold;
        str = CCString::createWithFormat("%d", playGold);
        mainLayer->gold->setString(str->getCString());
        playInfo->setPlayerGold(playGold);

        
        if ((_totalGold+_addGold) > (_totalCoin+_addCoin)) {
            CCAction *action = CCSequence::create(CCDelayTime::create(runTime),
                                                  CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runJunLabel)),
                                                  NULL);
             action->setTag(10017);
            this->runAction(action);
        }
    }
}
void SGFightRewardLayer::runJunLabel()//军功
{
    if (_totalJun > 0) {
        _addJun += 1;
        _totalJun -= 1;
        CCString *str = CCString::createWithFormat("%d", _addJun);
//        labelJun->setString(str->getCString());
        CCAction *action = CCSequence::create(CCDelayTime::create(runTime),
                                              CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runJunLabel)),
                                              NULL);
        action->setTag(10018);
        this->runAction(action);
    }else {
        _isFirstTouch = false;
        //if (conSprite)
           // conSprite->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 2)));
    }
}

void SGFightRewardLayer::fuckLayer()
{
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGFightRewardLayer::runExpLabel));
    CCAction *act1=CCSequence::create(call,NULL);
    act1->setTag(10010);
    this->runAction(act1);
}

void SGFightRewardLayer::showSceneLayer(CCObject *sender)
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId <= guide_tag_10 || guideId == guide_tag_30) return;
    
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PlotListResponse *response = (main::PlotListResponse *)req->m_msg;
    if (response) {
        int storyId = response->storyid();
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
        SGStoryBean *story = NULL;
        if (array == NULL) { //战斗中断
            story = SGStaticDataManager::shareStatic()->getStoryDataById(1);
        }else {
            for (int ii = 0; ii < array->count(); ii++) {
                story = (SGStoryBean *)array->objectAtIndex(ii);
                if (storyId == story->getStoryId()) {
                    break;
                }
            }
        }
        
        if (story ==NULL) {
            return;
        }
        story->setStoryStars(response->star());
        story->setMaxStar(response->maxstar());
        story->setCanReward(response->canreward());
        story->setRewardId(response->rewardid());
        if (response->state() == 0) {
            // story 场景为空，开启第一个场景
            int size = response->plotmodel_size();
            if (size > 0) {
                for (int ii = 0; ii < size; ii++) {
                    main::PlotModel model = response->plotmodel(ii);
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL) {
                        continue;
                    }else {
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
            }
        }else if (response->state() == 1) {
            // 初始化 场景数值章节
            int size = response->plotmodel_size();
            if (size > 0) {
                int maxSceneId = 0;
                for (int ii = 0; ii < size; ii++) {
                    main::PlotModel model = response->plotmodel(ii);
                    
                    CCLOG("scene ID=================%d", model.plotid());
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL) {
                        continue;
                    }else {
                        if (maxSceneId < model.plotid()) {
                            maxSceneId = model.plotid();
                        }
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
                
            }
        }
        else
        {
            return;
        }
        if (response->plotaskid() == 0 || story->getStoryId()==1 || story->getStoryId()==5) {
            SGMainManager::shareMain()->m_trackIsPve = true;
            SGMainManager::shareMain()->showStorySectionLayer(story);
        }else { // 有故事对话
            //SGStoryBean *bean = SGStaticDataManager::shareStatic()->getStoryDataById(1);
            SGStorySceneBean *scene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(0);
            SGGuideManager::shareManager()->showStorySpeaker(scene->getPlotId(), this, story);////序章问题
        }
    }
}





//继续闯关
void SGFightRewardLayer::continuechuangguan(CCObject*obj)
{
    main::SurvivalStartRequest* req = new main::SurvivalStartRequest();
    req->set_isjump(0);
    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_START, req);
}

//点击 待会再来 时候发送消息
void SGFightRewardLayer::sendContinue()
{
	//发送获取详情
    SGMainManager::shareMain()->sendGetSjzInfoRequest();
}

void SGFightRewardLayer::continueResp(CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if (sr)
    {
        main::SurvivalMainInfoResponse *rsp = (main::SurvivalMainInfoResponse *) sr->m_msg;
        if (rsp)
        {
            if (rsp->state() == 1) //如果有效，开启守荆州UI
            {
                //解析荆州详情数据
                SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
                data->setLevelCur(rsp->levels());
                data->setCountCur(rsp->fightcount());
                data->setCountMax(rsp->maxfightcount());
                data->setRankCur(rsp->rank());
                data->setDamageMax(rsp->maxdamage());
                data->setVIPCountMax(rsp->maxcanbuycount());
                data->setVIPCountCur(rsp->buycount());
                data->setVIPGold(rsp->price());
                
                //显示UI
                SGMainManager::shareMain()->showshoujingzhou();
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_ActivityMainCell_str21);
            }
        }
    }
}



//待会再来
// 需要向服务器发送消息, 通过回复的消息,确认是守城,还是继续战斗
void SGFightRewardLayer::waitfornext(CCObject*obj)
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
	
	//再次请求 守荆州界面
	this->sendContinue();
}

//军魂换礼
void SGFightRewardLayer::exchangeHandler(CCObject*obj)
{
//    //TODO: 记得进入军魂界面
//    waitfornext(NULL);
//    return;
    SGMainLayer* layer = SGMainManager::shareMain()->getMainLayer();
    if (layer)
        layer->sjzSoulExchangeRequest(2);
}




//////详情请见函数声明
void  SGFightRewardLayer::updateAnimation(SGStoryReward *reward)
{
    //////////////////////////////
    
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_fightrewardLayer);
    int  ismax= playInfo->getismaxlevel();
    
    
    
     CCPoint center = SGLayout::getPoint(kMiddleCenter);
     CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    /////////////////////////////////////获得每次最新数据
    
    currGold = playInfo->getPlayerGold();
    currCoin = playInfo->getPlayerCoins();
    currExps =playInfo->getPlayerExp();//hehehe
    currExps2 =playInfo->getPlayerExp();//hehehe
    currLevel = playInfo->getPlayerLevel() - 1;
    
    
    
    
    
    ////////////////////////////////////
    
    if(ismax==1) ////守荆州最后一关了  没有下一关
    {
        CCSprite *infobgbg=(CCSprite*)this->getChildByTag(1000);
        
        
      float  bottomHH = SGMainManager::shareMain()->getBottomHeight();
        
        
        //"点击屏幕继续"
        conSprite =  CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
      float    tpxx = infobgbg->getPositionX();
      float  tpyy = bottomHH + (infobgbg->getPositionY() - infobgbg->getContentSize().height*.5f-bottomHH)*.5f;
        conSprite->setPosition(ccp(tpxx, tpyy-20));//-35
        
        
        //conSprite->setPosition(center);
        conSprite->setVisible(false);
        this->addChild(conSprite);
        
        SGCCLabelTTF* labelmax=SGCCLabelTTF::create(str_ActivityMainCell_str22, FONT_BOXINFO, 28 , ccRED);
        labelmax->setPosition(ccpAdd(center, ccp(0, -size.height*0.1 )));
        //modify by:zyc. merge into create.
        //labelmax->setColor(ccRED);
        this->addChild(labelmax);
        //////////重新赋值  防止出事
      //  playInfo->setismaxlevel(0);
        
       //////////////////////////////////////连跳11场
        
     SGButton*temp_jxcg=  (SGButton*) this->getBtnByTag(500);
        if(temp_jxcg)
        {
        temp_jxcg->setVisible(false);
        temp_jxcg->setEnabled(false);
        }
     SGButton*temp_dhzl=  (SGButton*) this->getBtnByTag(501);
        if(temp_dhzl)
        {
        temp_dhzl->setVisible(false);
        temp_dhzl->setEnabled(false);
        
        }
        
        
        
        
        /////////////////////////////////////
    }

    
    
    
    
    
    
    
    
    _sjzcannottouch=false;

    
    
    
    ////////////////////////////////
    /////设置收荆州可第N次跳过可触
    this->setjump(true);
  
    CCString *str = CCString::create("0");
    labelCoin->setString(str->getCString());
    ////铜钱
    _totalCoin=0;
    _addCoin=0;
  
    _totalCoin = reward->getCoin();
    
    ////
 
     //经验
    _addExp=0;
    _totalExp=0;
    currExps=0;
    
    //元宝
    _totalGold=0;
    _addGold=0;
    _totalGold = reward->getGold();
   
    
    //军工
    _totalJun=0;
    _addJun=0;
    _totalJun = reward->getJexp();
    
    
    
    //待续...
    rateNum = reward->getRating();
    //if(datas->count()==1)
    this->runRateSp(rateNum);

}

void  SGFightRewardLayer::sjztongguan() ///守荆州通关坑爹走军魂页面逻辑
{
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    data->setIsShowBonus(true);
    //发送获取详情
    SGMainManager::shareMain()->sendGetSjzInfoRequest(true);
}


 



