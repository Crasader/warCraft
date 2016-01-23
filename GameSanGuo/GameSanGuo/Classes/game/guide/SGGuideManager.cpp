

//
//  SGGuideManager.cpp
//  GameSanGuo
//
//  Created by geng on 13-5-6.
//
//

#include "SGGuideManager.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGFirstSubLayer.h"
#include "SGFirstLayer.h"
#include "SGGuideVO.h"
#include "SGStaticDataManager.h"
#include "SGTaskLayer.h"
#include "SGCantAdvanceBox.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "RoleModule.pb.h"
#include "SGBattleManager.h"
#include "SGTestSwitch.h"
#include "UpdateLayer.h"
#include "SGUpdateManager.h"
#include "GlobalConfig.h"

#define SEND_PROCESS(p) SGMainManager::shareMain()->sendNewPlayerProcess(p)

#define Open_Guide (1)

static SGGuideManager *mananger = NULL;

SGGuideManager::~SGGuideManager()
{
	//SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GUIDE_STEP);
}

SGGuideManager::SGGuideManager(void) : limitStep(0), step(0), autoStep(0), actionType(0)
{
    uilayer = NULL;
    isDisMiss = false;
    fightGuideLayer = NULL;
    autoGuideLayer = NULL;
    storySpeaker = NULL;
    fourGuide = NULL;
    _story = NULL;
    isFightDesc = false;
    isShowTask = false;
    limitlayer = NULL;
    limitStartStep = 0;
    m_limitGuideList.clear();
    isGuide = true;
    isWanXDL = false;
}

SGGuideManager *SGGuideManager::shareManager()
{
    if (!mananger)
    {
        mananger = new SGGuideManager();
        mananger->initData();
        mananger->openGuide(true);
        mananger->step = 0;
        mananger->autoStep = 0;
        mananger->actionType = 0;
    }
    return mananger;
}

//是否进行开关新手引导
void SGGuideManager::openGuide(bool open)
{
    isGuide = open;
    if (open) {
        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        //如果没有到选择援军这一步，说明第一大步引导没有完成
        if (guideId < guide_tag_6)
        {
            isDisMiss = true;
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_0);
        }
        if (guideId >= guide_tag_6 && guideId <= guide_tag_30)//战斗教学完成，要进行探访
        {
            isDisMiss = true;
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_30);
        }
        if (guideId >= guide_tag_52)
        { // 教学全部完成
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_99);
        }
    }else
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_99);
    }
    CCUserDefault::sharedUserDefault()->flush();
}

void SGGuideManager::resetGuideId(int guideId)
{
    
#if Open_Guide
    this->startUiGuide(guideId);
#else
    CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_99);
#endif
}

void SGGuideManager::startUiGuide(int guideId)
{
    GPCCLOG("startUiGuide %d ", guideId);
    //开始引导，没有进入战斗，重置到第一步引导
    if (guideId <= guide_tag_6 && guideId >= guide_tag_0) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_0);
    }
    if (guideId <= guide_tag_9 && guideId > guide_tag_6) { // 进入剧情界面，继续第二剧情
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_6);
    }
    if (guideId <= guide_tag_11 && guideId > guide_tag_9) { // 进入剧情界面, 继续对话
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_9);
    }
    if (guideId <= guide_tag_16 && guideId > guide_tag_11) { // 进入首页界面, 继续强化
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_11);
    }
    if (guideId < guide_tag_24 && guideId >= guide_tag_17) { // 强化完成，进入首页，开始第三关
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_17);
    }

    if (guideId < guide_tag_32 && guideId >= guide_tag_25)
    { // 进入首页界面，继续探访
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_30);
    }
    if (guideId <= guide_tag_38 && guideId >= guide_tag_32) { // 进入军营界面，继续布阵
        if (SGPlayerInfo::sharePlayerInfo()->getOfficerCards()->count() < 2)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_30);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_32);
        }
        
    }
    if (guideId <= guide_tag_41 && guideId > guide_tag_38) { // 进入布阵界面，设为主将
        
        int generalCount = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(SGTeamgroup::shareTeamgroupInfo()->getiscurrent())->count();
        
        if (generalCount < 2) {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_32);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_38);
        }
    }

    //如果是在最后一个中断引导，直接完成引导
    if (guideId >= guide_tag_50)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", UIG_MAX1);
    }
    
    if (guideId < guide_tag_43 && guideId > guide_tag_41)
    { //布阵
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_41);
    }
    if (guideId < guide_tag_48 && guideId >= guide_tag_43)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_43);
    }

    CCUserDefault::sharedUserDefault()->flush();
}

void SGGuideManager::initData()
{

    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();  
    uilayer = SGUIGuideLayer::create();
    uilayer->setTag(999);
    uilayer->setVisible(false);
    uilayer->setTouchEnabled(false);
    mainScene->addChild(uilayer, 101);

}

void SGGuideManager::showSpeaker(int mIndex, SGBoxDelegate *delegate)
{
    // set mainlayer touch to false
    this->setMainTouchEnable(false);
    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
    
    //暂时暴力解决，如果uilayer为null，再init一次
    if (!uilayer)
    {
        this->initData();
    }
    
    uilayer->setVisible(true);
    uilayer->runAction(CCFadeIn::create(0.5f));
    uilayer->setTouchEnabled(true);
        
    SGGuideVO *model = manager->getGuideModel(mIndex);
    uilayer->setDatas(model, delegate);
}

void SGGuideManager::showStorySpeaker(int plotId, SGBoxDelegate *delegate, SGStoryBean *bean)
{
    // set mainlayer touch to false
    this->setMainTouchEnable(false);
    
    if (storySpeaker)
    {
        return;
    }
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    storySpeaker = SGStorySpeakerLayer::create();
    storySpeaker->setTouchEnabled(false);
    mainScene->addChild(storySpeaker);
    storySpeaker->runAction(CCFadeIn::create(0.5f));
    storySpeaker->setData(plotId);
    storySpeaker->setTouchEnabled(true);
    
    
    if (_story )
    {
        _story->release();
        _story = NULL;
    }
    _story = bean;
    _story->retain();
}

void SGGuideManager::showFourGuide()
{
    return;//先去掉
    this->setMainTouchEnable(false);

    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    fourGuide = SGFourGuideLayer::create();
    fourGuide->setFguide();
    fourGuide->setTouchEnabled(false);
    mainScene->addChild(fourGuide);
    fourGuide->runAction(CCFadeIn::create(0.5f));
    fourGuide->setTouchEnabled(true);

}

void SGGuideManager::showPKWords(CCString *word, SGBattleLayer *layer)
{
    this->setMainTouchEnable(false);
    if (uilayer != NULL)
    {
        uilayer->setTouchEnabled(false);
        uilayer->setVisible(false);
    }
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGFightDescLayer *flayer = SGFightDescLayer::create();
    flayer->showPKwords(word, layer);
    flayer->setTouchEnabled(false);
    mainScene->addChild(flayer);
    flayer->runAction(CCFadeIn::create(0.5f));
    flayer->setTouchEnabled(true);
}

void SGGuideManager::stepListener()
{
	CCLOG("Step Listener");
}

// 得到新手进度并发送给你服务器
void SGGuideManager::checkAndSendProcessMsg(int guideTag)
{
	switch (guideTag)
    {
		case guide_tag_5://第一章战斗开始
		{
			CCLOG("1 start");
			SEND_PROCESS(first_fight_start);
		}
			break;
		case guide_tag_7://第一章战斗结束
		{
			CCLOG("1 end");
			SEND_PROCESS(first_fight_end);
		}
			break;
		case guide_tag_9://第二章战斗开始
		{
			CCLOG("2 start");
			SEND_PROCESS(second_fight_start);
		}
			break;
		case guide_tag_10://第二章战斗结束
		{
			CCLOG("2 end");
			SEND_PROCESS(second_fight_end);
		}
			break;
		case guide_tag_13://强化开始
		{
			CCLOG("streng start");
			SEND_PROCESS(streng_start);
		}
			break;
		case guide_tag_14://强化开始第一步
		{
			CCLOG("streng 1");
			SEND_PROCESS(streng_1);
		}
			break;
		case guide_tag_15://强化开始第二步
		{
			CCLOG("streng 2");
			SEND_PROCESS(streng_2);
		}
			break;
		case guide_tag_16://强化开始第三步
		{
			CCLOG("streng 3");
			SEND_PROCESS(streng_3);
		}
			break;
		case guide_tag_17://强化结束
		{
			CCLOG("streng end");
			SEND_PROCESS(streng_end);
		}
			break;
		case guide_tag_21://第三剧情开始
		{
			CCLOG("3 start");
			SEND_PROCESS(third_fight_start);
		}
			break;
		case guide_tag_30://第三剧情结束
		{
			CCLOG("3 end");
			SEND_PROCESS(third_fight_end);
		}
			break;
		case guide_tag_31://探访开始
		{
			CCLOG("visit start");
			SEND_PROCESS(visit_start);
		}
			break;
		case guide_tag_32://探访开始第一步
		{
			CCLOG("visit 1");
			SEND_PROCESS(visit_1);
		}
			break;
		case guide_tag_33://探访结束
		{
			CCLOG("visit end");
			SEND_PROCESS(visit_end);
		}
			break;
		case guide_tag_36://布阵开始
		{
			CCLOG("embattle start");
			SEND_PROCESS(set_formation_start);
		}
			break;
		case guide_tag_37://布阵开始第一步
		{
			CCLOG("embattle 1");
			SEND_PROCESS(set_formation_1);
		}
			break;
		case guide_tag_38://布阵开始第二步
		{
			CCLOG("embattle 2");
			SEND_PROCESS(set_formation_2);
		}
			break;
		case guide_tag_39://布阵结束
		{
			CCLOG("embattle end");
			SEND_PROCESS(set_formation_end);
		}
			break;
		case guide_tag_41://设置主将开始
		{
			CCLOG("setlord start");
			SEND_PROCESS(set_lord_start);
		}
			break;
		case guide_tag_42://设置主将结束
		{
			CCLOG("setlord end");
			SEND_PROCESS(set_lord_end);
		}
			break;
		case guide_tag_43://点击当日任务开始
		{
			CCLOG("current day task start");
			SEND_PROCESS(current_day_task);
		}
			break;
		default:
		{
			//其他中间步骤
			CCLOG("default steps");
		}
			break;
	}
}

void SGGuideManager::checkIsDone(int gid)
{
    //return;  //removed by cgp
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId < gid )
    {
		//检测并向服务器发送消息
		this->checkAndSendProcessMsg(gid);
        SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
        SGGuideManager::shareManager()->showSpeaker(gid, layer);
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", gid);
        CCUserDefault::sharedUserDefault()->flush();
        
        main::BeginnersGuideRequest *request = new main::BeginnersGuideRequest();
        request->set_id(gid);
        SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_ASK, request, false);
        
        
        //记录新手引导步骤   1300号消息
        //
        if ( gid >=3  )
        {
            switch ( gid)
            {
                case 3:
                    SGMainManager::sendOperatorRecord(201000);//进入游戏后第一次引导，点击闯关
                    break;
                case 9:
                    SGMainManager::sendOperatorRecord(202000);//1-1胜利之后 点击首页按钮
                    break;
                case 10:
                    SGMainManager::sendOperatorRecord(202001);//9 step 之后点击闯关
                    break;
                case 15:
                    SGMainManager::sendOperatorRecord(203000);//1-2胜利之后点击首页
                    break;
                case 16:
                    SGMainManager::sendOperatorRecord(203001);//接step15 当前阵容武将
                    break;
                case 18:
                    SGMainManager::sendOperatorRecord(203002);//接step16 点击强化
                    break;
                case 19:
                    SGMainManager::sendOperatorRecord(203003);//接step17 选经验石、丹
                    break;
                case 20:
                    SGMainManager::sendOperatorRecord(203004); //强化之后点击确定
                    break;
                case 22:
                    SGMainManager::sendOperatorRecord(204000);//强化确定之后点击首页
                    break;
                case 23:
                    SGMainManager::sendOperatorRecord(204001);//接上 点击闯关
                    break;
                    
                case 33:
                    SGMainManager::sendOperatorRecord(205000);//点击探访
                    break;
                case 34:
                    SGMainManager::sendOperatorRecord(205001);//免费探访
                    break;
                    
                case 37:
                    SGMainManager::sendOperatorRecord(206000);//布阵
                    break;
                case 38:
                    SGMainManager::sendOperatorRecord(206001);//空格子
                    break;
                case 39:
                    SGMainManager::sendOperatorRecord(206002);//选将
                    break;
                case 40:
                    SGMainManager::sendOperatorRecord(206003);//click ok
                    break;
                case 43:
                    SGMainManager::sendOperatorRecord(206004);//set lorder
                    break;
                
                case 45:
                    SGMainManager::sendOperatorRecord(207000);//设置主将之后点击首页
                    break;
                case 46:
                    SGMainManager::sendOperatorRecord(207001);//点击闯关
                    break;
                case 49:
                    SGMainManager::sendOperatorRecord(207003);//点击领奖
                    break;
                case 50:
                    SGMainManager::sendOperatorRecord(207004);//确定领奖
                    break;
                case 52:
                    SGMainManager::sendOperatorRecord(207005);//返回
                    break;

                
                
                    //1-1 win
                case 7:
                    SGMainManager::sendOperatorRecord(201009);
                    break;
                    //1-2 win
                case 13:
                    SGMainManager::sendOperatorRecord(202010);
                    break;
                    //1-3 win
                case 31:
                    SGMainManager::sendOperatorRecord(204015);
                    break;
                    
                }
        }
    }
}

void SGGuideManager::closeSpeaker()
{
    storySpeaker->setVisible(false);
    storySpeaker->removeFromParentAndCleanup(true);
    storySpeaker  = NULL;
    this->setMainTouchEnable(true);
    SGMainManager::shareMain()->m_trackIsPve = true;
    SGMainManager::shareMain()->showStorySectionLayer(_story);
    _story->release();
}

void SGGuideManager::closeGuide()
{
    if (uilayer != NULL) {
        uilayer->setVisible(false);
        uilayer->removeHighLightRect();
        uilayer->removeFromParentAndCleanup(true);
        uilayer = NULL;
        this->setMainTouchEnable(true);
        this->openGuide(false);
    }
}

SGUIGuideLayer* SGGuideManager::getGuideLayer()
{
    return uilayer;
}

//检查大包组装以及资源更新这两项是否完成。
void SGGuideManager::checkIsAllResouce()
{
    
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    int guideId = ccud->getIntegerForKey("guideId");
    
    //新手引导结束之前，无视资源更新是否完成。
    if (guideId < 46)
    {
        return;
    }
    else
    {
        SGUpdateManager* sgum = SGUpdateManager::gi();
        if (GlobalConfig::gi()->getPackageVersion()) //大包，理论上大包到这里资源一定已经提前更新完成了，不会出现不通过情况。
        {
            if (sgum->getUpdateState() != SGUM_UPDATE_SUCCESS) //这种情况不会出现，否则就是有问题。
            {
                CCLOG("Oops! bigPackage was not finished?!");
            }
        }
        else //小包，可能到这里还没更新完成。
        {
            //是否执行过重启。
            if (sgum->getLocalIsRelaunchDone())
            {
                if (sgum->getUpdateState() != SGUM_UPDATE_SUCCESS) //这种情况不会出现，否则就是有问题。
                {
                    CCLOG("Oops! smallBigPackage was not finished?!");
                }
            }
            else
            {
                //这里新手引导已经完成。
                if (sgum->getUpdateState() != SGUM_UPDATE_SUCCESS) //继续等待更新。
                {
                    //去往更新界面。
                    sgum->switchUpdateLayer(true);
                }
                else //提示其重启。
                {
                    //重启弹框开启之前，必须将“已完成重启”标识设置为true。
                    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
                    ccud->setBoolForKey(SGUM_LOCAL_IS_RELAUNCH_DONE_KEY, true);
                    ccud->flush();
                    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 68, 0, NULL);
                    SGMainManager::shareMain()->showBox(cantadvanceBox);
                }
            }
        }
    }

}

void SGGuideManager::dismissLayer()
{
    //if (isDisMiss) return;
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guideId);
    uilayer->removeChildByTag(modelTag, true);
    uilayer->setVisible(false);
    uilayer->removeHighLightRect();
    uilayer->setTouchEnabled(false);
    this->setMainTouchEnable(true);
    isDisMiss = true;
    checkIsAllResouce();
}

void SGGuideManager::showTaskWork()
{
    // 显示任务闪烁
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGTaskLayer *taskLayer = (SGTaskLayer *)mainScene->getChildByTag(sg_taskLayer);
    if (taskLayer != NULL) {
        taskLayer->showTaskBlank();
    }
}

void SGGuideManager::setMainTouchEnable(bool enable)
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGFirstLayer *firstLayer = (SGFirstLayer*)mainScene->getChildByTag(sg_firstLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
    SGBaseBox *nowBox = NULL;
    if (mainScene->nowBoxTag > 0)
    {
        nowBox =(SGBaseBox*)mainScene->getChildByTag(mainScene->nowBoxTag);
    }
    
    
    if (mainLayer) {
        mainLayer->setIsCanTouch(enable);
        if (mainScene->nowLayerTag==sg_taskLayer) {
            mainLayer->setPortraitMenuCanTouch(false);
        }else {
            mainLayer->setPortraitMenuCanTouch(enable);
        }
    }
    if (firstLayer)
        firstLayer->setIsCanTouch(enable);
    if (nowLayer)
        nowLayer->setIsCanTouch(enable);
    if (nowBox)
        nowBox->setIsCanTouch(enable);
}
void SGGuideManager::changeRound(bool isme)
{
    fightGuideLayer->setTouchEnabled(isme);
}
void SGGuideManager::startFightGuide(int from)
{
    actionType = 0;
    int tmpFrom = from;
    int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    
    if ((story == 4 && (plotId == 11 || plotId == 10)) || (story == 6 && (plotId == 17 || plotId == 18 )))
    {
        isGuide = true;
    }

    
    if(isGuide)
    {

        
        int nextStep = step+1;
        int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
        int scene = SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();
        
        //为了与表里的对应from，值，而确定下一步操作是什么，很有意思的用法
        from = story*100+scene*10+from;
        
        if (nextStep == 20 && plotId == 1)
        {
            isWanXDL = true;
        }
        
        SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
        SGFightGuideVO *model = manager->getFightGuideModel(story, scene, nextStep);
        if (!model)
        {
            return;
        }

        GPCCLOG("\nstory::%d, scene::%d, nextStep::%d,from::%d, getFrom::%d,  modelType::%d" ,
                story, scene, nextStep, tmpFrom, model->getFrom(),  model->getModelType());
        //GPCCLOG("ModelType::::%d",model->getModelType());
        //GPCCLOG("From::::%d",model->getFrom());
        
        switch (model->getModelType())
        {
            case 0:
            {
                step++;
            }
                break;
            case 1://对话
            {
                if (model->getFrom() == from)
                {
                    step++;
                    fightGuideLayer->setVisible(true);
                    fightGuideLayer->setTouchEnabled(true);
                    
                    fightGuideLayer->initTips(model->tips);
                    fightGuideLayer->initMsg(model->getModelMsg());
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->setIsCanTouch(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    fightGuideLayer->setPlace(model->getPlace());
                }
            }
                break;
            case 2://初始化自己兵
            {
                step++;
                
                fightGuideLayer->setVisible(false);
                fightGuideLayer->setTouchEnabled(false);

                SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                SGHeroLayer *heroLayer = battleLayer->getOpponent(true);
                heroLayer->initBattleMap();
            }
                break;
            case 3: //初始化对方兵
            {
                step++;
                
                fightGuideLayer->setVisible(false);
                fightGuideLayer->setTouchEnabled(false);
                SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                heroLayer->initBattleMap();
            }
                break;
            case 4://闪动兵
            {
                if (model->getFrom() == from)
                {
                    step++;
                    actionType = model->getPlace();
                    
                    fightGuideLayer->setTouchEnabled(false);
                    if (model->getModelMsg()&&model->getModelMsg()->count() >0) {
                        fightGuideLayer->initMsg(model->getModelMsg(),false);
                    }
                    else
                    {
                        fightGuideLayer->setVisible(false);
                    }
                    
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    CCPoint point = model->getPoint();
					//11.18------------------------------------------------------------------------------
					//如果格子处有兵,显示手指指引

                    CCPoint temp = GameConfig::getGridPoint(point.x, point.y);
                    bm->displayTipsFinger(ccp(temp.x + 10, temp.y + 10));
                    
					if (bm->getShiBingWithIndex(gameIndex(point.x, point.y)) != NULL)
					{
                        //高亮列 2013.11.25
                        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                        if(hero->statusBarLayer)
                        {
                            hero->statusBarLayer->setVisible(true);
                            hero->statusBarLayer->setColor(ccGRAY);
                            hero->statusBarLayer->setPosition(ccpAdd(ccp(-hero->statusBarLayer->getContentSize().width/2, -hero->statusBarLayer->getContentSize().height*0.6), GameConfig::getGridPoint(mapRow-1, gameIndex(point.x, point.y).j)));
                        }
                        
                    }
					//-----------------------------------------------------------------------------------
                    bm->flashIndex(gameIndex(point.x, point.y),1024,true);
                }
            }
                break;
            case 6://切换回合
            {
                if (model->getFrom() == from)
                {
                    step++;
                    
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                    SGBattleManager::sharedBattle()->giveupRequest();
                    SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
                }
                
            }
                break;
            case 7://剪头指引
            {
                if (model->getFrom() == from)
                {
                    actionType = 1;
                    
                    step++; //直接在前面检测
                    fightGuideLayer->setVisible(true);
                    fightGuideLayer->setTouchEnabled(false);
                    fightGuideLayer->showTouchTip(model->getTouch());

                }
            }
                break;
            case 8://战斗结束
            {
                if (model->getFrom() == from)
                {
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                    SGBattleManager::sharedBattle()->battleOver();
                    fightGuideLayer->deleteFreeTip();
                }
                
            }
                break;
            case 9://初始化全部
            {
                if (model->getFrom() == from)
                {
                    step++;
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer1 = battleLayer->getOpponent(true);
                    heroLayer1->initBattleMap();
                    heroLayer1->getMenu()->setTouchEnabled(false);
                    SGHeroLayer *heroLayer2 = battleLayer->getOpponent(false);
                    heroLayer2->initBattleMap();
                    heroLayer2->getMenu()->setTouchEnabled(false);
                }
                
            }
                break;
            case 10://引导结束
            {
                if (model->getFrom() == from)
                {
                    isGuide = false;
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                    
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    
                    if (story == 1 && scene == 2) {
                        fightGuideLayer->showFreeTip();
                        SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                        heroLayer->zjj_btn->setEnabled(false);
                        heroLayer->end_btn->setEnabled(false);
                        
                        SGBattleMap *bm = heroLayer->getBattleMap();
                        bm->showMoveArrow();
                    }
                    else
                    {
                        SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                        heroLayer->setIsCanTouch(true);
                        heroLayer->getMenu()->setTouchEnabled(true);
                    }
                }
            }
                break;
                case 11:
            {
                if (model->getFrom() == from) {
                    step++;
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                }
            }
                break;
            case 12:
            {
                if (model->getFrom() == from) {
                    fightGuideLayer->setTouchEnabled(false);
                    
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(true);
                    heroLayer->zjj_btn->setEnabled(false);
                    
                }
            }
                break;
            case 13:
            {
                if (model->getFrom() == from)
                {
                    step++;
                    actionType = model->getPlace();
                    
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
        
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    gameIndex = bm->findLastShiBin();
                    bm->flashIndex(gameIndex);
                   
                    CCPoint temp = GameConfig::getGridPoint(gameIndex.i, gameIndex.j);
                    bm->displayTipsFinger(ccp(temp.x + 10, temp.y + 10));
                    
                    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                    if(hero->statusBarLayer)
                    {
                        hero->statusBarLayer->setColor(ccGRAY);
                        hero->statusBarLayer->setVisible(true);
                        hero->statusBarLayer->setPosition(ccpAdd(ccp(-hero->statusBarLayer->getContentSize().width/2, -hero->statusBarLayer->getContentSize().height*0.6), GameConfig::getGridPoint(mapRow-1, gameIndex.j)));
                    }
                }
            }
                break;
            case 14:
            {
                if (model->getFrom() == from)
                {
                    step++;
                    actionType = model->getPlace();
                    
                    fightGuideLayer->setVisible(false);
                    fightGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    gameIndex = bm->findFirstShiBin();
					
                    bm->flashIndex(gameIndex);
                    
                    CCPoint temp = GameConfig::getGridPoint(gameIndex.i, gameIndex.j);
                    bm->displayTipsFinger(ccp(temp.x + 10, temp.y + 10));
                    
                    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                    if(hero->statusBarLayer)
                    {
                        hero->statusBarLayer->setVisible(true);
                        hero->statusBarLayer->setColor(ccGRAY);
                        hero->statusBarLayer->setPosition(ccpAdd(ccp(-hero->statusBarLayer->getContentSize().width/2, -hero->statusBarLayer->getContentSize().height*0.6), GameConfig::getGridPoint(mapRow-1, gameIndex.j)));
                    }
                }
            }
                break;
            case 15:
            {
                
                step++;
                actionType = model->getPlace();
                
                fightGuideLayer->setVisible(false);
                fightGuideLayer->setTouchEnabled(false);
                SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                battleLayer->setIsCanTouch(true);
                SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                heroLayer->getMenu()->setTouchEnabled(false);
                SGBattleMap *bm = heroLayer->getBattleMap();
                
                model = manager->getFightGuideModel(story, scene, step-1);
                model->setPoint(ccp(gameIndex.i, gameIndex.j));
                SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                if(hero->statusBarLayer)
                {
                    hero->statusBarLayer->setColor(ccWHITE);
                    hero->statusBarLayer->setVisible(false);
                }
                
                bm->flashIndex(gameIndex);

                CCPoint temp = GameConfig::getGridPoint(gameIndex.i, gameIndex.j);
                bm->displayTipsFinger(ccp(temp.x + 10, temp.y + 10));
            }
                break;
            case 16:
            {
                
                if (model->getFrom() == from)
                {
                    step++;
                    actionType = model->getPlace();
                    
                    fightGuideLayer->setTouchEnabled(false);

                    if (model->getModelMsg()&&model->getModelMsg()->count() >0) {
                        fightGuideLayer->setVisible(true);
                        fightGuideLayer->initMsg(model->getModelMsg(),false);
                    }
                    else
                    {
                        fightGuideLayer->setVisible(false);
                    }
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    CCPoint point = model->getPoint();
                    
                    while (bm->getChildByTag(1024)) {
                        bm->removeChildByTag(1024, true);
                    }
                    GameIndex gameIndex = gameIndex(point.x, point.y);
                    bm->flashIndex(gameIndex,1024,true);
                    
                    CCPoint nextPoint = ccp(point.x +200, point.y);
                    SGFightGuideVO *nextModel = manager->getFightGuideModel(story, scene, step+1);
                    if (nextModel)
                    {
                        nextPoint = nextModel->getPoint();
                    }
                    
                    GameIndex gameNexIndex = gameIndex(nextPoint.x, nextPoint.y);
                    bm->flashIndex(gameNexIndex,1024,true);
                    fightGuideLayer->creatTouchTipMoving(GameConfig::getGridPoint(gameIndex.i, gameIndex.j),
                                                         GameConfig::getGridPoint(gameNexIndex.i, gameNexIndex.j),
                                                         fightGuideLayer->isVisible());

                    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                    if(hero->statusBarLayer)
                    {
                        hero->statusBarLayer->setVisible(true);
                        hero->statusBarLayer->setPosition(ccpAdd(ccp(-hero->statusBarLayer->getContentSize().width/2, 0), ccp(GameConfig::getGridPoint(gameIndex.i, gameIndex.j).x,0)));
                    }
                    
                }
            }
                break;
            case 17://武将队列示意
            {
                if (model->getFrom() == from)
                {
                    step++;
                    fightGuideLayer->setVisible(true);
                    fightGuideLayer->setTouchEnabled(true);
                    fightGuideLayer->creatWujiangDemo(model->getModelMsg());
                    
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->setIsCanTouch(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                }
            }
                break;
            case 18://可移动小兵的引导示意
            {
                if (model->getFrom() == from)
                {
                    step++;
                    fightGuideLayer->setVisible(true);
                    fightGuideLayer->setTouchEnabled(true);
                    fightGuideLayer->creatWujiangDemo(model->getModelMsg(),false);
                    
                    SGBattleLayer *battleLayer = fightGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->setIsCanTouch(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                }
            }
                break;
            default:
                break;
        }
    }
}
void SGGuideManager::startAutoGuide(int from)
{
    actionType = 0;
    if(isGuide)
    {
        int nextAutoStep = autoStep+1;
        int story = 1;
        int scene = 1;
        from = story*100+scene*10+from;
        CCLOG("from--->:%d",from);
        CCLOG("story:::%d",story);
        CCLOG("scene:::%d",scene);
        CCLOG("nextStep::::%d",nextAutoStep);
        
        SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
        SGFightGuideVO *model = manager->getAutoGuideModel(nextAutoStep);
        if (!model)
        {
            return;
        }

        CCLOG("ModelType::::%d",model->getModelType());
        CCLOG("From::::%d",model->getFrom());
        
        switch (model->getModelType())
        {
            case 0:
            {
                autoStep++;
            }
                break;
            case 1://对话
            {
                if (model->getFrom() == from)
                {
                    
                    autoStep++;
                    autoGuideLayer->setVisible(true);
                    autoGuideLayer->setTouchEnabled(true);
                    
                    autoGuideLayer->initTips(model->tips);
                    autoGuideLayer->initMsg(model->getModelMsg());
                    
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->setIsCanTouch(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    autoGuideLayer->setPlace(model->getPlace());
                }
            }
                break;
            case 2://初始化自己兵
            {
                if (model->getFrom() == from)
                {
                autoStep++;
                
                autoGuideLayer->setVisible(false);
                autoGuideLayer->setTouchEnabled(false);
                
                SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                SGHeroLayer *heroLayer = battleLayer->getOpponent(true);
                heroLayer->initBattleMap();
                }
            }
                break;
            case 3: //初始化对方兵
            {
                if (model->getFrom() == from)
                {
                    autoStep++;
                    
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->initBattleMap();
                }

            }
                break;
            case 4://移出兵
            {
                if (model->getFrom() == from)
                {
                    autoStep++;
                    actionType = model->getPlace();
                    
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    CCPoint point = model->getPoint();
                    bm->initTouchGrid(point.y);
                }
            }
                break;
            case 5://移入兵
            {
                if (model->getFrom() == from)
                {
                    autoStep++;
                    actionType = model->getPlace();
                    
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    CCPoint point = model->getPoint();
                    bm->moveTouchGrid(point.y);
					CCLOG("autoGuide in [%s-%d]", __FUNCTION__, __LINE__);
                }
            }
                break;
            case 6://切换回合
            {
                if (model->getFrom() == from)
                {
                    autoStep++;
                    
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    
                    SGBattleManager::sharedBattle()->giveupRequest();
                    SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
                }
                
            }
                break;
            case 7://剪头指引
            {
                if (model->getFrom() == from)
                {
                    autoStep++; //直接在前面检测
                    autoGuideLayer->setVisible(true);
                    autoGuideLayer->setTouchEnabled(false);
                    autoGuideLayer->showTouchTip(model->getTouch());
                }
            }
                break;
            case 8://战斗结束
            {               
                if (model->getFrom() == from)
                {
                    //新手引导   发送  过场动画   战斗结束消息。
                    SGMainManager::sendOperatorRecord(100060);
                    
                    autoStep++;
                    autoGuideLayer->removeFromParent();
                    autoGuideLayer = NULL;
                    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
                    mainScene->showCountryLayer();
                }
            }
                break;
            case 9://初始化全部
            {
                if (model->getFrom() == from)
                {
                    autoStep++;
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer1 = battleLayer->getOpponent(true);
                    heroLayer1->initBattleMap();
                    heroLayer1->getMenu()->setTouchEnabled(false);
                    SGHeroLayer *heroLayer2 = battleLayer->getOpponent(false);
                    heroLayer2->initBattleMap();
                    heroLayer2->getMenu()->setTouchEnabled(false);
                }
                
            }
                break;
            case 10://引导结束
            {
                if (model->getFrom() == from)
                {
                    isGuide = false;
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    

                }
            }
                break;
            case 11:
            {
                if (model->getFrom() == from) {
                    autoStep++;
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                }
            }
                break;
            case 12:
            {
                if (model->getFrom() == from) {
                    autoGuideLayer->setTouchEnabled(false);
                    
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    battleLayer->setIsCanTouch(true);
                    
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(true);
                    heroLayer->zjj_btn->setEnabled(false);
                }
            }
                break;
            case 15://黑屏对话
            {
                if (model->getFrom() == from) {
                    autoStep++;
                    autoGuideLayer->setVisible(true);
                    autoGuideLayer->setTouchEnabled(true);
                    
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->setIsCanTouch(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    autoGuideLayer->initFinalMsg(model->getModelMsg());
                }
            }
                break;
            case 16://发动主将计
            {
                if (model->getFrom() == from) {
                    autoStep++;
                    autoGuideLayer->setVisible(false);
                    autoGuideLayer->setTouchEnabled(false);
                    
                    SGBattleLayer *battleLayer = autoGuideLayer->getBattleLayer();
                    SGHeroLayer *heroLayer = battleLayer->getOpponent(false);
                    heroLayer->setIsCanTouch(false);
                    heroLayer->getMenu()->setTouchEnabled(false);
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    autoGuideLayer->autoAttackSkill(bm);
                }
            }
                break;
            default:
                break;
        }
    }
}

int SGGuideManager::getLimitGuideSize()
{
     return m_limitGuideList.size();
}

//要引导的某一大步的id
void SGGuideManager::setLimitGuideTag(int limitStart)
{
    if (limitStart > 0)
    {
        m_limitGuideList.push_back(limitStart);
    }
}
void SGGuideManager::closeLimitGuide()
{
    limitlayer->setVisible(false);
    limitlayer->setTouchEnabled(false);
    limitlayer->removeHighLightRect();
    
    //引导出现lootwiki的时候，做特殊处理
    if (SGMainManager::shareMain()->getMainScene()->nowBoxTag == box_lootWikiMain)
    {
        this->setMainTouchEnable(false);
        if (SGMainManager::shareMain()->getMainScene()->nowBoxTag > 0)
        {
            SGBaseBox *nowBox =(SGBaseBox*)SGMainManager::shareMain()->getMainScene()->getChildByTag(SGMainManager::shareMain()->getMainScene()->nowBoxTag);
            nowBox->setIsCanTouch(true);

        }
    }
    else
    {
        this->setMainTouchEnable(true);
    }
    isGuide = false;
    
    if (m_limitGuideList.size() <= 1)
    {
        m_limitGuideList.clear();
        limitlayer->removeFromParentAndCleanup(true);
        limitlayer = NULL;
    }
    else
    {
        m_limitGuideList.erase(m_limitGuideList.begin());
        startLimitGuide();
    }

}

int SGGuideManager::getCurrentGuideId()
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    return guideId;
}


void SGGuideManager::startLimitGuide(int startStep)
{
    //如果是startStep为大于0，表示从头引导，小于0的时候则是继续引导
    if (startStep > 0)
    {
        //初始化
        if (m_limitGuideList.size() > 0)
        {
            limitStartStep = m_limitGuideList[0];
            //开始引导，根据limitStartStep也就是triggerdata中的id，判定达成条件
            //未达成，直接不引导
            if (!this->conditionEnoughForLimitGuide(limitStartStep))
            {
                
                m_limitGuideList.clear();
                isGuide = false;
                return;
            }
            
            if (limitlayer == NULL)
            {
                limitlayer = SGLimitGuideLayer::create();
                limitlayer->setTag(-999);
                limitlayer->setVisible(false);
                limitlayer->setTouchEnabled(false);
                SGMainManager::shareMain()->getMainScene()->addChild(limitlayer, 502);
            }
        }
        else
        {
            return;
        }
        

        

        limitStep = 0;
        isGuide = true;
    }

    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    if(isGuide)
    {
        //limitStep标记当前一大步阶段引导中的某一小步
        int nextStep = limitStep+1;
        SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
        //limitStartStep 某个单个的阶段引导，用作索引表中某一大步的引导，nextStep大引导中的单个步骤
        SGLimitGuideVO *model = manager->getLimitGuideModel(limitStartStep,nextStep);
        //如果找不到对应的数据原型，直接返回，并关掉对应的阶段引导
        if (!model)
        {
            this->closeLimitGuide();
            return;
        }
        
        //type表示每个单步引导的layer怎么显示，对话，高亮之类的，具体不清楚-_@
        switch (model->getModelType())
        {
            case 1:
            case 8:
            case 10:
            case 13:
            case 14:
            case 15://当前页
            case 16:
            case 17:
            case 18:
            case 19:
            {
                if ((model->getModelId() == 13 || model->getModelId() == 11 ) && model->getModelType() == 13)
                {
                    SGMainManager::shareMain()->showStoryMainLayer();
                }

                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);

                // set mainlayer touch to false
                this->setMainTouchEnable(false);
                if (model->getModelType() == 17)
                {
                    limitlayer->setTouchPriority(-256);
                }
                else
                {
                    limitlayer->spriteBlackbg->setOpacity(179);
                    limitlayer->spriteTag->setVisible(true);
                }
                limitlayer->setVisible(true);
                limitlayer->setTouchEnabled(true);
                limitlayer->runAction(CCFadeIn::create(0.5f));
                
                limitlayer->setDatas(model, layer);
                
                limitStep++;
            }
                break;
            case 2:
            case 7://到主页
            {
                // set mainlayer touch to false
                this->setMainTouchEnable(false);
                limitlayer->setVisible(true);
                limitlayer->setTouchEnabled(true);
                limitlayer->runAction(CCFadeIn::create(0.5f));
                SGMainManager::shareMain()->showFirstLayer();
                ((SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer))->resetMoveBtns(limitStartStep);
                if(limitStartStep == 18 && model->getModelId() == 16)
                {
                    ((SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer))->resetMoveBtns(3);
                }
                
                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                
                limitlayer->setDatas(model, layer);
                
                ((SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer))->setIsCanTouch(false);
                limitStep++;
                
            }
                break;
            case 3:
            {
                // set mainlayer touch to false
                this->setMainTouchEnable(false);
                limitlayer->setVisible(true);
                limitlayer->setTouchEnabled(true);
                limitlayer->runAction(CCFadeIn::create(0.5f));
                if (limitStartStep == limitXianshiFuben) {
                    SGMainManager::shareMain()->getMainLayer()->sendActivityHandler();
                }
                else if (limitStartStep == limitPvpBattle)
                {
                     SGMainManager::shareMain()->showPvpMainLayer();
                }
                else if (limitStartStep == limitJinyinFuben)
                {
                    SGMainManager::shareMain()->getMainLayer()->creamBattleHandler();
                }
                else if (limitStartStep == limitShouJinZhou)
                {
                    if (mainScene->getChildByTag(sg_firstLayer)) {
                        ((SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer))->sjzInfoRequest();
                    }
                }
                else if (limitStartStep == limitBossBattle)
                {
                    if (mainScene->getChildByTag(sg_firstLayer))
                    {
                        ((SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer))->bossHandler();
                    }
                }
                
                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                limitlayer->setDatas(model, layer);
                limitStep++;
                
            }
                break;
            case 4:
            {
                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                layer->setIsCanTouch(true);
                limitStartStep = 0;
                limitStep = 0;

                closeLimitGuide();
                

            }
                break;
            case 5:
            {
                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                layer->setIsCanTouch(true);
                
                limitStartStep = 0;
                limitStep = 0;
                
                closeLimitGuide();
                
                SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
                

            }
                break;
            case 6:
            {
                // set mainlayer touch to false
                this->setMainTouchEnable(false);
                limitlayer->setVisible(true);
                limitlayer->setTouchEnabled(true);
                limitlayer->runAction(CCFadeIn::create(0.5f));
                if (limitStartStep == limitJinyinFuben)
                {
                    SGMainManager::shareMain()->showStoryMainLayer();
                }
                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                limitlayer->setDatas(model, layer);
                limitStep++;
            }
                break;
            case 9:
            {
                this->setMainTouchEnable(false);
                limitlayer->setVisible(true);
                limitlayer->setTouchEnabled(true);
                limitlayer->runAction(CCFadeIn::create(0.5f));
                
                limitlayer->setDatas(model,NULL);
                
                limitStep++;
            }
                break;
            case 11:
            case 12:
            {
                // set mainlayer touch to false
                this->setMainTouchEnable(false);
                limitlayer->setVisible(true);
                limitlayer->setTouchEnabled(true);
                limitlayer->runAction(CCFadeIn::create(0.5f));
                
                SGMainManager::shareMain()->showFirstLayer();

                SGBaseLayer *layer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                
                limitlayer->setDatas(model, layer);
                
                ((SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer))->setIsCanTouch(false);
                limitStep++;
                
            }
                break;
            default:
                break;
        }
    }
}

void SGGuideManager::setFightGuideLayer(SGFightGuideLayer *layer)
{
    fightGuideLayer = layer;
}
SGFightGuideLayer* SGGuideManager::getFightGuideLayer(){
    return fightGuideLayer;
}

void SGGuideManager::setAutoGuideLayer(SGAutoGuideLayer *layer)
{
    autoGuideLayer = layer;
}
SGAutoGuideLayer* SGGuideManager::getAutoGuideLayer(){
    return autoGuideLayer;
}

CCPoint SGGuideManager::getPoint()
{
    int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    int scene = SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();

    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
    SGFightGuideVO *model = manager->getFightGuideModel(story, scene, step);
    return model->getPoint();
}
bool SGGuideManager::getIsChangeRound()
{
    int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    int scene = SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();

    if (story == 1 && scene == 1)
    {
        if (step == 11)
        {
            return false;
        }
    }
    if (story == 1 && scene == 2)
    {
        if (step == 9)
        {
            return false;
        }
    }
    return true;
}

void SGGuideManager::clearGuideLayer()
{
    if (uilayer)
    {
        uilayer->setVisible(false);
    }
    if (limitlayer)
    {
        limitlayer->removeFromParent();
        limitlayer = NULL;
    }
    if (fightGuideLayer)
    {
        fightGuideLayer->removeFromParent();
        fightGuideLayer = NULL;
    }
    if (autoGuideLayer)
    {
        autoGuideLayer->removeFromParent();
        autoGuideLayer = NULL;
    }
}

//判定阶段引导的达成条件是否足够
//@checkId 对应triggerdata中的id（哪个limitGuide引导）
bool SGGuideManager::conditionEnoughForLimitGuide(int checkId)
{
    //装备转生条件判定
    if(checkId == LIT_GUID_EUIQP_ADVANCED)
    {
        //获取当前是哪个阵
        int currentEmbattleId = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        //获取当前阵容上的所有的武将
        CCArray *embattleOfficers = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(currentEmbattleId);
        
        //获取第一个武将
        if (embattleOfficers)
        {
            SGOfficerCard *officerCard = static_cast<SGOfficerCard *>(embattleOfficers->objectAtIndex(0));
            if (officerCard)
            {
                //检测第一个武将的位置
                if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(currentEmbattleId, officerCard) == 1)
                {
                    //test for printing
                    CCLOG("当前阵上第一个武将 || id >>> %d || name %s", officerCard->getItemId(), officerCard->getOfficerName()->getCString());
                    //获取当前武将身上的装备
                    CCArray *officerEquips = officerCard->getOfficerEquips();
                    
                    int equipId = -1;
                    //检测对应需要转生的装备
                    //读取controlValue 需要的条件值，因为只引导一次，不再写入公用函数，只用一次 @id为51的表明是阶段引导的需要条件id
                    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
                    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("52");
                    if (dic1)
                    {
                        equipId = dic1->valueForKey("value")->intValue();
                        CCLOG("引导条件达成！需要装备 || id >>> %d", equipId);

                    }
                    //有装备，并且读取表中值正确
                    if (officerEquips && equipId != -1)
                    {
                        for (int i = 0; i < officerEquips->count(); i++)
                        {
                            SGEquipCard *equipCard = static_cast<SGEquipCard *>(officerEquips->objectAtIndex(i));
                            if (equipCard)
                            {
                                //如果与表中一致，条件达成
                                if (equipCard->getItemId() == equipId)
                                {
                                    CCLOG("引导条件达成！需要装备 || id >>> %d", equipId);
                                    return true;
                                }
                            }
                        }
                    }
                
                }
            }
            
        }

    }
    else if (checkId == LIT_GUID_OFFICER_ADVANCED)//武将转生条件引导判定
    {
        //获取当前是哪个阵
        int currentEmbattleId = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        //获取当前阵容上的所有的武将
        CCArray *embattleOfficers = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(currentEmbattleId);
        
        //获取第一个武将
        if (embattleOfficers)
        {
            SGOfficerCard *officerCard = static_cast<SGOfficerCard *>(embattleOfficers->objectAtIndex(0));
            if (officerCard)
            {
                //检测第一个武将的位置
                if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(currentEmbattleId, officerCard) == 1)
                {
                    //读取controlValue 条件
                    //读取controlValue 需要的条件值，因为只引导一次，不再写入公用函数，
                    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
                    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("53");
                    if (dic1)
                    {
                        CCString *condition = (CCString *)dic1->valueForKey("value");
                        CCArray *cdts = split(condition->getCString(), ",");
                        if (cdts)
                        {
                            //这个武将的id
                            int officerId = officerCard->getItemId();
                            //角色的国家 蜀: 1 魏: 2 吴: 3
                            int countryId = SGPlayerInfo::sharePlayerInfo()->getPlayerCountry();
                            //数组放条件
                            int needOfficerIds[3] = {-1, -1, -1};
                            for (int i = 0; i < cdts->count(); i++)
                            {
                                CCArray *tmp = split(static_cast<CCString *>(cdts->objectAtIndex(i))->getCString(), ":");
                                if (tmp)
                                {
                                    needOfficerIds[static_cast<CCString *>(tmp->objectAtIndex(0))->intValue() - 1]
                                    = static_cast<CCString *>(tmp->objectAtIndex(1))->intValue();
                                }
                            }
                            //对应国家的需要的哪个武将id已在needOfficerIds，直接根据needOfficerIds[countryId-1]与officerId比对即可
                            if (needOfficerIds[countryId - 1] == officerId)
                            {
                                return true;
                            }
                        }
                    }

                }
            }
        }

    }
    else if (checkId == LIT_GUID_SUMMON_OFFICER)//武将召唤
    {
        int protoOfficerId = -1;
        //得到当前所有的武将
        CCArray *officers = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
        //读取controlValue中的武将的protoId
        CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
        CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("64");
        if (dic1)
        {
            protoOfficerId = dic1->valueForKey("value")->intValue();
        }
        //是否开启引导
        bool isGuideThis = true;
        //遍历所有武将，找到对应的原型id
        if (officers)
        {
            CCObject *card = NULL;
            CCARRAY_FOREACH(officers, card)
            {
                SGOfficerCard *temp = static_cast<SGOfficerCard *>(card);
                //发现这个对应原型id的武将
                if (temp->getProtoId() == protoOfficerId)
                {
                    //找到这个对应原型id的武将，不引导
                    isGuideThis = false;
                }
            }
        }
        return isGuideThis;
    }
    else if (checkId == LIT_GUID_ENETR_AREAN)//竟技场，如果当前阵上第一个武将是小于10级的，不开始引导
    {
        //获取当前是哪个阵
        int currentEmbattleId = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        //获取当前阵容上的所有的武将
        CCArray *embattleOfficers = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(currentEmbattleId);
        
        //获取第一个武将
        if (embattleOfficers)
        {
            SGOfficerCard *officerCard = static_cast<SGOfficerCard *>(embattleOfficers->objectAtIndex(0));
            //第一个武将
            if (officerCard)
            {
                int lvl = officerCard->getCurrLevel();//获取这个武将卡的等级
                if (lvl < 10)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }

    }
    else
    {
        return true;//不是需要检测的引导，则统一返回true lzy 2014-11-08
    }
    CCLOG("引导条件未达成！");
    return false;
}








