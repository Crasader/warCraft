//
//  SGBattleLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-20.
//
//

#include "SGBattleLayer.h"
//#include "MapEditLayer.h"
#include "SGLayout.h"
//#include "SGSocket.h"
#include "SGNormalAttackList.h"
#include "SGAIManager.h"
#include "SGBattleChangeRoundLayer.h"
#include "ResourceManager.h"
#include "SGGuideManager.h"
#include "SGPlayerInfo.h"
#include "SGBattleSpeedLayer.h"
#include "SGMainManager.h"
#include "ArenaModule.pb.h"
#include "SGStringConfig.h"
#include "ExtClass.h"


#define EFFECT_LAYER    20

#define FUCK_TAG 9

#define BATTLE_LAYERCOLOR_TAY 1111

using namespace cocos2d;
SGBattleLayer::SGBattleLayer()
:ismyRound(true)
,m_effectsLayer(NULL)
,isRepairMap(false)
,operationName(NULL)
,layerColor(NULL)
,layerGradient(NULL)
,layerGradient1(NULL)
,battleSp(NULL)
,battleSp1(NULL)
,isOver(false)
,m_battleCardNum(0)
,isCgPve(false)
,isStopAi(false)
,con(NULL)
,myHeroLayer(NULL)
,mySpeed(0)
,enemySpeed(0)
,item(NULL)
,m_ismy(false)
,skipFightButton(NULL)
{
    //GPCCLOG("SGBattleLayer()");
}

SGBattleLayer::~SGBattleLayer()
{
   // GPCCLOG("~SGBattleLayer()");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleLayer);
    //MMDEBUG: 在这里移除非常驻新手引导。
    SGGuideManager::shareManager()->clearGuideLayer();
    ExtClassOfSDK::sharedSDKInstance()->battleEndDealing();
}

SGBattleLayer * SGBattleLayer::create(const char *fileName, bool isCg)
{
    SGBattleLayer *battleLayer = new SGBattleLayer();
    if (battleLayer && battleLayer->init(NULL, sg_battleLayer,false))
    {
        battleLayer->m_pBackgroundFileName = fileName;
        battleLayer->isCgPve = isCg;
        battleLayer->viewDidLoad();
        battleLayer->autorelease();
        return battleLayer;
    }
    CC_SAFE_DELETE(battleLayer);
    return NULL;
}

void SGBattleLayer::initMsg()
{

}

void SGBattleLayer::viewDidLoad()
{
    ExtClassOfSDK::sharedSDKInstance()->battleBeginDealing();
    
    ResourceManager::sharedInstance()->bindTexture("bg/background_1.plist", RES_TYPE_LAYER_UI, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("bg/background_2.plist", RES_TYPE_LAYER_UI, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("bg/background_3.plist", RES_TYPE_LAYER_UI, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("bg/background_4.plist", RES_TYPE_LAYER_UI, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("bg/background_5.plist", RES_TYPE_LAYER_UI, sg_battleLayer);

    
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    //战斗背景图
    CCSprite *spriteBG = CCSprite::createWithSpriteFrameName(m_pBackgroundFileName.c_str());
    if (NULL == spriteBG)
    {
        spriteBG = CCSprite::createWithSpriteFrameName("background_1.png");
    }
    spriteBG->setScaleX(SGLayout::getSkewing(320)/spriteBG->getContentSize().width);
    spriteBG->setScaleY(SGLayout::getSkewingY(480)/spriteBG->getContentSize().height);
    spriteBG->setPosition(SGLayout::getPoint(kMiddleCenter));
    addChild(spriteBG, -5);
    battleSp = CCLayer::create();
    battleSp1 = CCLayer::create();
    addChild(battleSp, -4);
    addChild(battleSp1, -4);
    battleSp->setVisible(false);
    battleSp1->setVisible(false);
    if(SGBattleManager::sharedBattle()->getBattleType() == 103)
    {//如果是竞技场,添加跳过战斗按钮
        
        skipFightButton = SGButton::createFromLocal("box_btnbg.png",str_sgareanlayer_str25,this,menu_selector(SGBattleLayer::onClickSkipArenaFightButton),CCPointZero, FONT_PANGWA,ccWHITE,26,false,true);
        skipFightButton->setPosition(SGLayout::getPoint(kMiddleCenter));
        this->addBtn(skipFightButton);
        skipFightButton->setZOrder(10000);
        skipFightButton->setOpacity(0);
        skipFightButton->setEnabled(false);
        
        const float dTime = 5;
        const float fiTime = 2;
        
        //按钮动作
        CCDelayTime * delayTime = CCDelayTime::create(dTime);
        CCFadeIn * fadin = CCFadeIn::create(fiTime);
        CCCallFunc * callFunc = CCCallFunc::create(this, callfunc_selector(SGBattleLayer::callSetSkipButton ) );
        CCSequence * sequence = CCSequence::create(delayTime, fadin, callFunc, NULL );
        skipFightButton->runAction(sequence);
        
        //字体动作
        if (true)
        {
            skipFightButton->getNormalLabel()->getMainLabel()->setOpacity(0);
            CCDelayTime * delayTime = CCDelayTime::create(dTime);
            CCFadeIn * fadin = CCFadeIn::create(fiTime);
            CCSequence * sequence = CCSequence::create(delayTime, fadin, NULL);
            skipFightButton->getNormalLabel()->getMainLabel()->runAction(sequence);
        }
    }

    ResourceManager* rm = ResourceManager::sharedInstance();
    //武将阵列组成动画。原在AttackList里各种加载。
    rm->bindTexture("animationFile/wjhb2.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    rm->bindTexture("animationFile/wjhb4_1.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    rm->bindTexture("animationFile/wjhb4_2.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    //人物脚底阴影。原在ShiBing里各种加载。
    rm->bindTexture("role/role_shadow_1.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    rm->bindTexture("role/role_shadow_2.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    //城墙的资源，原在ShiBing里各种加载。
    rm->bindTexture("role/wall_1.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    rm->bindTexture("role/wall_2.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    rm->bindTexture("role/wall_3.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    
    //灰度的格子
    for (int k = 1; k < 3; k++)
    {
        for (int i = 0; i < mapRow; i++)
        {
            int j = 0;
            if (i % 2 != 0)
            {
                j = 1;
            }
            for ( ; j < mapList; j += 2 )
            {
                CCLayerColor *gridColor = CCLayerColor::create(ccc4(0, 0, 0, 255 * 0.25), gridWidth, gridHeight);
                gridColor->setPosition(ccpSub(GameConfig::getGridPoint( i, j, k == 2 ? false : true), ccp(gridWidth * 0.5, 0) ));
                if (k == 1)
                {
                    battleSp->addChild(gridColor);
                }
                else
                {
                    battleSp1->addChild(gridColor);
                }
            }
        }
    }
}
void SGBattleLayer::callSetSkipButton()
{
    skipFightButton->setEnabled(true);
}
void SGBattleLayer::closeAI()
{
    
}
//初始化地图
void SGBattleLayer::initMap(HeroObj *heroObj, CCArray *sbs, bool ismy)
{
    //GPCCLOG("SGBattleLayer::initMap:%d",ismy);
    m_ismy = ismy;
    myHeroLayer = SGHeroLayer::create(heroObj, sbs, ismy);
    BattleLayerTag heroTag = ismy ? sg_myHeroLayerTag : sg_enemyHeroLayerTag;
    this->addChild(myHeroLayer, ksg_myHeroLayer, heroTag);
    
    //removed by cgp
//    setSpeedUi();
//    //vs界面完成后再允许点击下方一系列按钮，新手引导下
//    if (SGGuideManager::shareManager()->isGuide)
//    {
//        if (myHeroLayer)
//        {
//            myHeroLayer->setBtnEnable(true);
//        }
//    }
    
}
void SGBattleLayer::setSpeedUiNew(bool ismy)
{
   // GPCCLOG("setSpeedUi\n");
    if(SGPlayerInfo::sharePlayerInfo()->getPlotId() == 4)
    {
        SGGuideManager::shareManager()->isGuide = false;
    }
    
    if (!SGGuideManager::shareManager()->isGuide)     //不是新手引导
    {
        if (SGGuideManager::shareManager()->isFightDesc && SGGuideManager::shareManager()->isGuide)
        {
            SGAIManager::shareManager()->isAi = false;
        }
        SGHeroLayer* myLayer = (SGHeroLayer*)(this->getChildByTag(ismy ? sg_myHeroLayerTag : sg_enemyHeroLayerTag));
        
        m_ismy = ismy;
        myLayer->initBattleMap();
        
        
        if (ismy)
        {
			myLayer->changeRound(true, ismyRound, true);
            
            if (!SGBattleManager::sharedBattle()->isSecond)
            {
                SGBattleManager::sharedBattle()->isSecond=false;
                //速度条的加载
                CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::showFirstActionLayer));
                CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::firstLoading));
                
                this->runAction(CCSequence::create(speedFun,CCDelayTime::create(1.0),speedFun2,NULL));  //modified by cgp
            }
            else
            {
                CCLog("不显示速度比较");
                SGBattleManager::sharedBattle()->isSecond=false;
                
            }
        }
        else
        {
            myLayer->changeRound(true,!ismyRound , true);
            
        }
    }
    else    //新手引导
    {
        
        if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
        {
            CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::showFirstActionLayer));
            CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::firstLoading));
            CCCallFunc *fun3=CCCallFunc::create(this, callfunc_selector(SGBattleLayer::addGuide));
            
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(5.0),speedFun2,CCDelayTime::create(1),fun3,NULL));
        }
    }
    
}

void SGBattleLayer::setSpeedUi()
{
    // GPCCLOG("setSpeedUi\n");
    if (!SGGuideManager::shareManager()->isGuide)
    {
        if (SGGuideManager::shareManager()->isFightDesc && SGGuideManager::shareManager()->isGuide)
        {
            SGAIManager::shareManager()->isAi = false;
        }
        myHeroLayer->initBattleMap();
        
        
        if (m_ismy)
        {
            myHeroLayer->changeRound(true, ismyRound, true);
            
            if (!SGBattleManager::sharedBattle()->isSecond)
            {
                SGBattleManager::sharedBattle()->isSecond=false;
                //速度条的加载
                CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::showFirstActionLayer));
                CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::firstLoading));
                
                this->runAction(CCSequence::create(speedFun,CCDelayTime::create(5.0),speedFun2,NULL));
            }
            else
            {
                CCLog("不显示速度比较");
                SGBattleManager::sharedBattle()->isSecond=false;
                
            }
        }
        else
        {
            myHeroLayer->changeRound(true,!ismyRound , true);
            
        }
    }
    else
    {
        
        if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
        {
            CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::showFirstActionLayer));
            CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGBattleLayer::firstLoading));
            CCCallFunc *fun3=CCCallFunc::create(this, callfunc_selector(SGBattleLayer::addGuide));
            
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(5.0),speedFun2,CCDelayTime::create(1),fun3,NULL));
        }
    }
    
}


void SGBattleLayer::addGuide()
{
    if (SGBattleManager::sharedBattle()->getBattleType() != BT_ADVENTURE)
        return;
    int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    if (story == 1 || (story == 4 && (plotId == 11 || plotId == 10)) || (story == 6 && (plotId == 17 || plotId == 18 )))
    {
        SGGuideManager::shareManager()->startFightGuide(1);
    }
    else if (story == 2 && SGPlayerInfo::sharePlayerInfo()->getPlotId() == 4)
    {
        SGGuideManager::shareManager()->startFightGuide(1);
    }
}
//首次
void SGBattleLayer::firstLoading()
{
    //GPCCLOG("firstLoading 敌方回合\n");
    this->showTurnNum(ismyRound, false);
    //初始化时设置为true，没有攻击阵列
    SGAIManager::shareManager()->setIsCompleteAttack(true);
    
    //MM: 在这里延迟开启竞技场自动战斗。
    if(SGBattleManager::sharedBattle()->getBattleType() == BT_ARENA)
    {
        CCDelayTime *delay = CCDelayTime::create(2.0f);
        CCCallFunc *call = CCCallFunc::create(myHeroLayer, callfunc_selector(SGHeroLayer::autoBattleWithAI));
        CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
        this->runAction(sequence);
    }
    
}
//显示特效
void SGBattleLayer::addEffectLayer()
{
    CCLayer *layer = CCLayer::create();
    this->setEffectsLayer(layer);
    this->addChild(getEffectsLayer(), EFFECT_LAYER, EFFECT_LAYER);
}

SGHeroLayer *SGBattleLayer::getOpponent(bool isme)
{
    BattleLayerTag layerTag = isme?sg_enemyHeroLayerTag:sg_myHeroLayerTag;
    return (SGHeroLayer*)this->getChildByTag(layerTag);
}
//显示战斗ID
void SGBattleLayer::showBattleId(std::string str)
{
    return;
    CCLabelTTF *label = CCLabelTTF::create(str.c_str(), "", FONTSIZE(12));
    this->addChild(label);//1118删除两方中界的数字
    label->setColor(ccBLACK);
    label->setPosition(ccp(screenwidth * 0.5, screenheight * 0.5));

}

//显示武将的属性
void SGBattleLayer::showHeroAttribute(bool ismy)
{
//    SGHeroLayer *hero = getOpponent(ismy);

}
//切换动画
void SGBattleLayer::showTurnNum(bool turn, bool isMessage)
{
    //切换回合动画
    SGBattleChangeRoundLayer *layer = SGBattleChangeRoundLayer::create(turn==1);
    CCCallFuncO *call = CCCallFuncO::create(this, callfuncO_selector(SGBattleLayer::turnShowOver), layer);
    //这里1.9f是动画播放的时间，写个死时间
    CCDelayTime *delay = CCDelayTime::create(1.0f);
    //CCDelayTime *delay = CCDelayTime::create(0.1f);
    CCFiniteTimeAction *sequence = CCSequence::create(call,delay,NULL);
    
    if (isMessage)
    {
        CCCallFunc *call1 = CCCallFuncN::create(this, callfuncN_selector(SGBattleLayer::changeRoundResult));
        this->runAction(CCSequence::create(sequence,call1, NULL));
    }
    else
    {
        this->runAction(sequence);
    }
    
}
void SGBattleLayer::turnShowOver(CCNode *node)
{
    this->addChild(node, INT32_MAX);
}

void SGBattleLayer::changeRoundResult()
{
    //GPCCLOG("changeRoundResult\n");
    SGHeroLayer* myHeroLayer = (SGHeroLayer*)this->getChildByTag(sg_myHeroLayerTag);
    myHeroLayer->playBuffEffect();
    myHeroLayer->changeRound(true,ismyRound);
    
    
    SGHeroLayer* enmyHeroLayer = (SGHeroLayer*)this->getChildByTag(sg_enemyHeroLayerTag);
    enmyHeroLayer->changeRound(true,!ismyRound);
}
//显示是否胜利了
void SGBattleLayer::showIsWin(bool isWin)
{
   // GPCCLOG("显示是否胜利了\n");
    if (isOver) {
        return;
    }
    isOver = true;
    bool showWin=false;
    bool _me = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer* _hero = SGBattleManager::sharedBattle()->getHeroLayer(!_me);
    if (_hero->getBlood() <= 0) {
        _hero->battleMap->closeMap = true;
        _hero->battleMap->getAttackList()->removeAllObjects();
        for (int i = 0; i < mapRow; i++) {
            for (int j = 0; j < mapList; j++) {
                float t = (i * 0.15) + (j * 0.15);
                _hero->battleMap->deleteSbObj(i, j, t);
                if (i == 2 &&
                    j == 3 &&
                    _hero->isInitPve &&
                    _me ) {
                    _hero->battleEnd(t);
                    showWin=true;
                }
            }
        }
    }
    
    SGHeroLayer* __hero = SGBattleManager::sharedBattle()->getHeroLayer(_me);
    if (getIsMyRound()) {
        __hero->battleMap->hideFriendlyHelp(false);
    }
    if (__hero->getBlood() <= 0) {
        __hero->battleMap->closeMap = true;
        __hero->battleMap->getAttackList()->removeAllObjects();
        for (int i = 0; i < mapRow; i++) {
            for (int j = 0; j < mapList; j++) {
                float t = (i * 0.15) + (j * 0.15);
                __hero->battleMap->deleteSbObj(i, j, t);
            }
        }
        
    }
    
    if (SGGuideManager::shareManager()->isGuide)
    {
        SGBattleManager::sharedBattle()->guideBattleOver();
    }
    else
    {
        SGBattleManager::sharedBattle()->battleOver();
    }
    
}
void SGBattleLayer::resuleShowOver(CCNode *node)
{
    
}
bool SGBattleLayer::getIsMyRound()
{
    return ismyRound;
}

bool SGBattleLayer::getIsCgPve()
{
    return isCgPve;
}
//停止AI
void SGBattleLayer::setStopAi(bool isS)
{
    isStopAi = isS;
}
//是否停止AI
bool SGBattleLayer::getStopAi()
{
    return isStopAi;
}
void SGBattleLayer::setIsMyRound(bool ismyround_)
{
    this->ismyRound = ismyround_;
}
void SGBattleLayer::setIsOver(bool over)
{
    isOver = over;
}

bool SGBattleLayer::getIsOver()
{
    return isOver;
}

void SGBattleLayer::showColorSp(bool isHide)
{
    if (isHide) {
        battleSp->setVisible(false);
        battleSp1->setVisible(false);
        return;
    }
    
}
void SGBattleLayer::changeColorSp(bool needRound)
{
    battleSp->setVisible(ismyRound == needRound);
    battleSp1->setVisible((!ismyRound) == needRound);

}
void SGBattleLayer::onClickSkipArenaFightButton()
{
    if(skipFightButton)
    {
        skipFightButton->setEnabled(false);
    }
    SGAIManager::shareManager()->isBattleOver = false;
    main::ArenaEndFightRequest  * req = new main::ArenaEndFightRequest();
   
    req->set_quick(true);
    SGSocketClient::sharedSocketClient()->send( MSG_ARENA_END_GIGHT, req);
}
//是否已根据服务器数据修正过地图
void SGBattleLayer::setIsRepairMap(bool repair)
{
    this->isRepairMap = repair;
}
bool SGBattleLayer::getIsRepairMap()
{
    return this->isRepairMap;
}
 void SGBattleLayer::changeRound_needVerify(int type)//需要验证的战斗切回合调用此方法使可动方行动
{
    if(!SGBattleManager::sharedBattle()->getIsBattlePve())
    {
        //老的方法可看下面注释掉的代码，现在统一为，只要有切换回合消息，则可以移动标识为true，因为如果在PVP打真人，这个标识的值不重要。
        SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
        if (type==1) //我可以行动.
        {
            //MMDBG: CHANGEROUND HACK一下，原有逻辑不切换棋盘，在这里直接切换。
            this->battleSp->setVisible(true);
            this->battleSp1->setVisible(false);
//            SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
        }
        else //对方可以行动.
        {
            this->battleSp->setVisible(false);
            this->battleSp1->setVisible(true);
//            if (SGAIManager::shareManager()->isPVE)
//            {
//                SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
//            }
        }
        
                
    }

}
//切换回合
void SGBattleLayer::changeRound()
{
        //MMDBG: AIBATTLE 所有需要验证的战斗，不论新手，人人PVP，人机PVP，都要有唯一确认标识。
        if(!SGBattleManager::sharedBattle()->getIsBattlePve() /*&&SGAIManager::shareManager()->isPVE*/)
        {
            SGAIManager::shareManager()->m_isVarifyActiviteAI = false;
        }
        ismyRound = !ismyRound;

        changeColorSp();
        //GPCCLOG("开始切换回合了 ");

        //切换回合的时候重置全部攻击阵列攻击完成的标记
        if (!ismyRound)
        {
            SGAIManager::shareManager()->setIsCompleteAttack(false);
            CCLOG("battleMap ChangeRound setIsCompleteAttack || %d", SGAIManager::shareManager()->getIsCompleteAttack());
        }
        
        if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsCgPve())
        
        SGBattleManager::sharedBattle()->startCountingForHelp(false);
        
    #pragma mark "==========================切换回合==================================="
        if (!ismyRound) {
             this->showTurnNum(ismyRound, false);
        }
       
        SGHeroLayer* myHeroLayer = (SGHeroLayer*)this->getChildByTag(sg_myHeroLayerTag);
        myHeroLayer->changeRound(true, ismyRound);

        SGHeroLayer* enmyHeroLayer = (SGHeroLayer*)this->getChildByTag(sg_enemyHeroLayerTag);
        
        enmyHeroLayer->playBuffEffect();
        if (SGAIManager::shareManager()->isPVE)
        {
            enmyHeroLayer->changeRound(true, !ismyRound);
        }else{
            enmyHeroLayer->changeRound(false, !ismyRound);
        }
        //GPCCLOG("开始切换完成回合了 ");
    
}

//放弃回合消息
void SGBattleLayer::giveupMessage()
{
    if (!SGAIManager::shareManager()->isPVE)
	{
		ismyRound = !ismyRound;
     
        this->showTurnNum(ismyRound, true);
    }
	else//为了即时显示5回合判定血量, 可能还会有未知bug,目前未测出2014-04-24
	{
		this->showTurnNum(ismyRound, false);
	}

}

void SGBattleLayer::setIsCanTouch(bool isTouch)
{
    SGHeroLayer* myHeroLayer = (SGHeroLayer*)this->getChildByTag(sg_myHeroLayerTag);
    if (myHeroLayer) {
        myHeroLayer->setIsCanTouch(isTouch);
    }
    
    
}


//震屏效果
void SGBattleLayer::fuckLayer(int act, bool isY)
{
   // GPCCLOG("fuckLayer");
    this->stopAllActions();
   
    this->setPosition(ccp(0, 0));
    this->stopActionByTag(FUCK_TAG);
    
    CCMoveTo *move0 = NULL;
    CCMoveTo *move1 = NULL;
    CCMoveTo *move2 = NULL;
    CCMoveTo *move3 = NULL;
    CCMoveTo *move4 = NULL;

    if (!isY)
    {
        int a, b, c , d;
        int ay, by ,cy,dy;
        switch (act)
        {
            case 1:
            {
                move1=CCMoveTo::create(0.0232, ccp(0, 10));
                move2=CCMoveTo::create(0.0232, ccp(0, -10));
                move3=CCMoveTo::create(0.0242, ccp(0, 0));
                CCSequence *action = CCSequence::create(move1, move3,move2,move3 ,NULL);
                
                CCRepeat *repeat=CCRepeat::create(action, 4);
                action->setTag(FUCK_TAG);
                this->runAction(repeat);
            }
                break;
            case 2:
            {
                move1=CCMoveTo::create(0.022, ccp(0, 7));
                move2=CCMoveTo::create(0.022, ccp(0, -7));
                move3=CCMoveTo::create(0.022, ccp(0, 0));
                CCSequence *action = CCSequence::create( move1,move3, move2,move3 ,NULL);
                
                CCRepeat *repeat=CCRepeat::create(action, 3);
                action->setTag(FUCK_TAG);
                this->runAction(repeat);
            }
                break;
            case 3:
            {
                move1=CCMoveTo::create(0.022, ccp(0, 7));
                move2=CCMoveTo::create(0.022, ccp(0, -7));
                move3=CCMoveTo::create(0.022, ccp(0, 0));
                CCSequence *action = CCSequence::create( move1, move3,move2,move3 ,NULL);
                
                CCRepeat *repeat=CCRepeat::create(action, 3);
                
                action->setTag(FUCK_TAG);
                this->runAction(repeat);
            }
                break;
                
            case 4:
            {
                a = (int)(CCRANDOM_0_1() * 7);
                b = (int)(CCRANDOM_0_1() * 7);
                c = (int)(CCRANDOM_0_1() * 7);
                d = (int)(CCRANDOM_0_1() * 7);
                ay = (int)(CCRANDOM_0_1() * 7);
                by = (int)(CCRANDOM_0_1() * 7);
                cy = (int)(CCRANDOM_0_1() * 7);
                dy = (int)(CCRANDOM_0_1() * 7);
                move0 = CCMoveTo::create(0.05, ccp(0, 0));
                move1 = CCMoveTo::create(0.035, ccp(a, ay));
                move2 = CCMoveTo::create(0.035, ccp(-b, by));
                move3 = CCMoveTo::create(0.025, ccp(-c, -cy));
                move4 = CCMoveTo::create(0.025, ccp(d, -dy));

                this->runAction(CCSequence::create(move1,  move2,move3, move4, move0, nullptr ));
            }
                break;
                
            default:
            {
                
            }
                break;
        }
        
        
    }
    else
    {

//        CCMoveTo *move7 = CCMoveTo::create(0.035, ccp( -4, 0));
//        CCMoveTo *move8 = CCMoveTo::create(0.035, ccp( 4, 0) );
//        CCMoveTo *move2 = CCMoveTo::create(0.025, ccp( -2, 0));
//        CCMoveTo *move3 = CCMoveTo::create(0.025, ccp( 2,0 ));
//        CCMoveTo *move6 = CCMoveTo::create(0.005, ccp( 0, 0));
//        CCAction *act = (CCAction*)CCSequence::create( move7, move8, move7, move8, move2, move3, move6, NULL);
//        act->setTag(FUCK_TAG);
//        this->runAction(act);
        
        CCMoveTo *move7 = CCMoveTo::create(0.06, ccp( 0, -10));
        CCMoveTo *move8 = CCMoveTo::create(0.06, ccp(0,  10) );
        CCMoveTo *move2 = CCMoveTo::create(0.08, ccp(0, -6));
        CCMoveTo *move3 = CCMoveTo::create(0.08, ccp(0, 6 ));
        CCMoveTo *move6 = CCMoveTo::create(0.04, ccp(0, 0));
        CCAction *act = (CCAction*)CCSequence::create( move7, move8, move7, move8, move2, move3, move6, NULL);
        act->setTag(FUCK_TAG);
        this->runAction(act);
        
        
    }
}
//设置操作的名称
void SGBattleLayer::setOperationName(const char *name)
{
    return;
    operationName->setString(name);
}

void SGBattleLayer::textNewEff()
{

}
//敌方和我方的速度
void SGBattleLayer::setSpeed(int my_speed, int enemy_speed)
{
    mySpeed=my_speed;
    enemySpeed=enemy_speed;
}
//显示第一个动作
void SGBattleLayer::showFirstActionLayer()
{
    SGBattleSpeedLayer *layer = SGBattleSpeedLayer::create(mySpeed, enemySpeed,this->getIsMyRound());
    this->addChild(layer, INT32_MAX);
}

 