//
//  SGOverBattleBox.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-17.
//
//

#include "SGOverBattleBox.h"
#include "SGStoryRewardBox.h"
#include "SGFightRewardLayer.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGHeroLayer.h"
#include "SGBattleManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGPvpFightRewardLayer.h"
#include "FightBaseModule.pb.h"
#include "SGAIManager.h"
#include "SGArenaFightRewardLayer.h"
#include "SGLogicManager.h"

//PLIST-失败
#define PLIST_FAIL1 "battle/fail.plist"

//动画-失败
#define ANIM_FAIL "battle/fail.scml"
//动画循环-失败
#define ANIM_FAIL_98 "battle/fail98.scml"
//循环动画延迟退出时间，秒
#define ANIM_TIME 4

// SGQFightBox
SGOverBattleBox::SGOverBattleBox(void):m_isRunAway(0), _reward(NULL), t_delegate(NULL), winLetter(NULL), winBg(NULL), anim98(NULL),m_pvpRewardData(NULL),m_arenaRewardData(NULL)
{
    m_isShowExoticMerchant = false;
}

SGOverBattleBox::~SGOverBattleBox(void)
{
//    CC_SAFE_RELEASE(_reward);
    
    ResourceManager* rm = ResourceManager::sharedInstance();
    
    rm->unBindLayerTexture(sg_overBattleBox);
  
}

SGOverBattleBox* SGOverBattleBox::create(SGBoxDelegate *delegate, SGStoryReward *reward,int isRunAway , bool isShowExoticMerchant)
{
    CCAssert(BT_LADDER != SGBattleManager::sharedBattle()->getBattleType() ,"\n\n\nzyc debug for pvp ...此处不是pvp结算入口。\n\n\n\n");
    SGOverBattleBox *box = new SGOverBattleBox();
    if (box && box->init(NULL, sg_battlePreLayer)) {
        box->t_delegate = delegate;
        box->_reward = reward;
        box->m_isRunAway = isRunAway;
        box->m_isShowExoticMerchant = isShowExoticMerchant;
        if (reward != NULL)
            box->_reward->retain();
        
        bool isWin = false;
        if (reward!=NULL && reward->getRating()>0) {
            isWin = true;
        }
        
        box->initView(isWin);
        
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

/*新pvp战斗结束*/
SGOverBattleBox * SGOverBattleBox::create(SGBoxDelegate *delegate , SGPvpFightRewardData * pvpRewardData)
{
    CCAssert(NULL!=pvpRewardData, "\n\n\nzyc debug for pvp ...不能为空。\n\n\n\n");
    SGOverBattleBox *box = new SGOverBattleBox();
    if (box && box->init(NULL, sg_battlePreLayer))
    {
        box->t_delegate = delegate;
        box->m_pvpRewardData = pvpRewardData;
        box->initView(pvpRewardData->getIsWin());
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

SGOverBattleBox * SGOverBattleBox::create(SGBoxDelegate *delegate , SGArenaFightRewardData * arenaRewardData)
{
    CCAssert(NULL!=arenaRewardData, "\n\n\n angel debug for arena ...不能为空。\n\n\n\n");
    SGOverBattleBox *box = new SGOverBattleBox();
    if (box && box->init(NULL, sg_battlePreLayer))
    {
        box->t_delegate = delegate;
        box->m_arenaRewardData = arenaRewardData;
        box->initView(arenaRewardData->getIsWin());
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;

}

void SGOverBattleBox::initView(bool isWin)
{
    
    //战斗结束，自动战斗标记还原
    SGAIManager::shareManager()->setIsOpenAutoBattle(false);
    //全局动画播放速度还原
    SGLogicManager::gi()->setGameSpeed(1.0f);
    //出现胜利提示，不再允许使用自动战斗
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(false);
    
    if (t_delegate)
    {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    playAnims();
	//playWinAnim();
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true); //myHeroLayer
    SGHeroLayer *enemyHero=SGBattleManager::sharedBattle()->getHeroLayer(false);  //hisHeroLayer
    
    if (m_isRunAway == 0) {
        //普通胜利提示
        if (hero->getCntrolRoundNum() == 0 && hero->getMorale()!=0 && enemyHero->getMorale()!=0) {
            CCScale9Sprite *tipbj = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
            tipbj->setContentSize(CCSizeMake( screenwidth * 0.6f, screenheight * 0.2f));
            CCLabelTTF *la = CCLabelTTF::create( isWin ? str_OverBattleBox_str1 : str_OverBattleBox_str2 ,
                                                FONT_BOXINFO,
                                                FONTSIZE(16),
                                                CCSize(tipbj->getContentSize().width * 0.8, tipbj->getContentSize().height * 0.7),
                                                kCCTextAlignmentLeft);
            tipbj->addChild(la);
            la->setPosition(ccp(tipbj->getContentSize().width * 0.5, tipbj->getContentSize().height * 0.5));
            CCSprite *ss = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
            ss->setScaleX(tipbj->getContentSize().width / ss->getContentSize().width);
            ss->setScaleY(tipbj->getContentSize().height / ss->getContentSize().height);
            ss->setPosition(ccp(tipbj->getContentSize().width * 0.5f, tipbj->getContentSize().height * 0.5));
            tipbj->addChild(ss, -1);
            this->addChild(tipbj, 9999);
            tipbj->setPosition(ccp(screenwidth * 0.5f, screenheight * 0.3f));
        }
    }
    else if (m_isRunAway == 1)
    {
        //逃跑胜利提示
        CCScale9Sprite *tipbj = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
        tipbj->setContentSize(CCSizeMake( screenwidth * 0.6f, screenheight * 0.2f));
        
        CCLabelTTF *la1 = CCLabelTTF::create( isWin ? str_OverBattleBox_str3 : str_OverBattleBox_str4 ,
                                            FONT_BOXINFO,
                                            FONTSIZE(16),
                                            CCSize(tipbj->getContentSize().width * 0.8, tipbj->getContentSize().height * 0.7),
                                            kCCTextAlignmentCenter);
        tipbj->addChild(la1);
        la1->setPosition(ccp(tipbj->getContentSize().width * 0.5, tipbj->getContentSize().height * 0.5 - 30));
        
        CCLabelTTF *la2 = CCLabelTTF::create( isWin ? str_OverBattleBox_str5 :str_OverBattleBox_str6,
                                             FONT_BOXINFO,
                                             FONTSIZE(16),
                                             CCSize(tipbj->getContentSize().width * 0.8, tipbj->getContentSize().height * 0.7),
                                             kCCTextAlignmentCenter);
        tipbj->addChild(la2);
        la2->setPosition(ccp(tipbj->getContentSize().width * 0.5, tipbj->getContentSize().height * 0.5 - 72));
        
        CCSprite *ss = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
        ss->setScaleX(tipbj->getContentSize().width / ss->getContentSize().width);
        ss->setScaleY(tipbj->getContentSize().height / ss->getContentSize().height);
        ss->setPosition(ccp(tipbj->getContentSize().width * 0.5f, tipbj->getContentSize().height * 0.5));
        tipbj->addChild(ss, -1);
        this->addChild(tipbj, 9999);
        tipbj->setPosition(ccp(screenwidth * 0.5f, screenheight * 0.3f));
        
    }
    else
    {
        //do nothing
    }
}
//设置胜利背景光可见
void SGOverBattleBox::setWinBgVisible()
{
	winBg->setVisible(true);
}


//代码实现的胜利动画
void SGOverBattleBox::playWinAnim()
{
    ResourceManager::sharedInstance()->bindTexture("battle/battle_win_eff.plist", RES_TYPE_LAYER_UI, sg_overBattleBox);
    ResourceManager::sharedInstance()->bindTexture("battle/battle_win.plist", RES_TYPE_LAYER_UI, sg_overBattleBox);

	winLetter = CCSprite::createWithSpriteFrameName("battle_win.png");// “胜利“ 字体
    
    
	winBg = CCSprite::createWithSpriteFrameName("battle_win_eff.png");//旋转的光效底图
	if (!winBg || !winLetter)
	{
		return ;
	}
	this->addChild(winLetter, 10);
	this->addChild(winBg, 1);
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//设置胜利的位置，底部光效的位置
	winLetter->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	winBg->setPosition(winLetter->getPosition());
	winBg->setVisible(false);//特效在胜利特效后出现
	
	//“胜利”字放大
	winLetter->setScale(3.0);
	//缩放“胜利“字体
	CCScaleTo *winToSmall = CCScaleTo::create(0.5, 0.5);
	CCScaleTo *winToNormal = CCScaleTo::create(0.06, 1.0);
	//设置胜利特效出现
	CCCallFunc *setBg = CCCallFunc::create(this, callfunc_selector(SGOverBattleBox::setWinBgVisible));
	//胜利光效旋转
	CCRotateBy *bgRotate = CCRotateBy::create(6.0, 360);
	//永久旋转
	CCRepeatForever *rotateForever = CCRepeatForever::create(bgRotate);
	//创建播放队列
	CCSequence *playSeqWin = CCSequence::create(winToSmall, winToNormal,CCDelayTime::create(0.15), setBg, NULL);
	//CCSequence *playSeqBg = CCSequence::create(, rotateForever, NULL);
	
	winLetter->runAction(playSeqWin);
	winBg->runAction(rotateForever);
	
}

//播放循环胜利动画
void SGOverBattleBox::playAnim98()
{
    anim98 = NULL;
    if (SGBattleManager::sharedBattle()->isWin)
    {
        GPCCLOG("播放循环胜利动画\n");
        //anim98 = CCSpriterX::create(ANIM_WIN_98, true);
		playWinAnim();
    }
    else
    {
         GPCCLOG("播放循环失败动画\n");
        anim98 = CCSpriterX::create(ANIM_FAIL_98, true);
		if (!anim98)
			return;
		anim98->setanimaID(0);
		anim98->setisloop(true);
		anim98->setVisible(true);
		anim98->setPosition(SGLayout::getPoint(kMiddleCenter));
		this->addChild(anim98, 6);
		anim98->play();
    }

}

void SGOverBattleBox::playAnims()
{
    bool isWin = false;
    if (_reward!=NULL && _reward->getRating()>0) {
        isWin = true;
    }
    //pvp
    if(m_pvpRewardData)
        isWin = m_pvpRewardData->getIsWin();
    if(m_arenaRewardData)
    {
        isWin = m_arenaRewardData->getIsWin();
    }

    CCSpriterX* csx = NULL;

    ResourceManager* rm = ResourceManager::sharedInstance();
    float delay = 0.0f;
	
    if (isWin) //加载胜利
    {

    }
    else //加载失败
    {
        GPCCLOG("加载失败 game is lose\n");
        rm->bindTexture(PLIST_FAIL1, RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        
        csx = CCSpriterX::create(ANIM_FAIL, true);
		
		csx->setanimaID(0);
		csx->setisloop(false);
		csx->setVisible(true);
		csx->setPosition(SGLayout::getPoint(kMiddleCenter));
		this->addChild(csx, 5);
		csx->play();
		delay = csx->getAnimaTimeLength(0, 0);
    }

    //标记战斗结果状态
    SGBattleManager::sharedBattle()->isWin = isWin;
    //设置延迟动作
    
    CCAction *action = CCSequence::create(CCDelayTime::create(delay + 0.1), CCCallFunc::create(this, callfunc_selector(SGOverBattleBox::playAnim98)), CCDelayTime::create(ANIM_TIME), CCCallFunc::create(this, callfunc_selector(SGOverBattleBox::goonAction)), NULL);
    this->runAction(action);
    
}

void SGOverBattleBox::goonAction()
{
    SGMainScene *mainScen = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScen->getChildByTag(sg_mainLayer);
    mainLayer->setVisible(true);
    
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    
    //这时候的nowlayertag是sg_battlelayer，因为这个overbattlebox是直接添加到mainscene上的！
    /*
     为新的 pvp战斗结算页面
     */
    if (BT_LADDER == btp)
    {
        CCAssert(NULL!=m_pvpRewardData, "\n\n\nzyc debug for pvp ...不能为空。\n\n\n\n");
        SGPvpFightRewardLayer * layer1 = SGPvpFightRewardLayer::create(m_pvpRewardData);
        SGMainManager::shareMain()->getMainScene()->addShowLayer(layer1);
        
        /*测试代码
        CCLOG("\n\n\n战斗id = %s , %lld\n\n\n\n" ,SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId.c_str(), atoll(SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId.c_str()) );
        main::FightPvpRewardRequest *res = new main::FightPvpRewardRequest();
        res->set_fightid(atoll( SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId.c_str() ) );
        SGSocketClient::sharedSocketClient()->send(MSG_PVP_FIGHT_REWARD, res );
        
        SGPvpFightRewardData * data = new SGPvpFightRewardData();
        data->setShengWang(3);
        data->setScore(3);
        data->setRank(10000);
        data->setJoinCount(14);
        data->setIsWin(true);
        SGPvpFightRewardLayer * layer1 = SGPvpFightRewardLayer::create(data);
        
        SGMainManager::shareMain()->getMainScene()->addShowLayer(layer1 , true);
         */
    }
    else if (BT_ARENA == btp)
    {
       // SGArenaFightRewardLayer * layer1 = SGArenaFightRewardLayer::create(m_arenaRewardData);
       // SGMainManager::shareMain()->getMainScene()->addShowLayer(layer1);
        SGMainManager::shareMain()->goToArenaLayerRequest();
    }
    else
    {
        SGFightRewardLayer *layer = SGFightRewardLayer::create(_reward,1 , m_isShowExoticMerchant); /////1115
        SGMainManager::shareMain()->showLayer(layer);
    }
    
    //停止循环动画
    {
    	if (anim98)
    	{
    		anim98->stop();
    		anim98 = NULL;
    	}
    }
    this->removeFromParentAndCleanup(true);
}

void SGOverBattleBox::closeBox()
{
    this->resetMainTouch(true);
}

void SGOverBattleBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}
