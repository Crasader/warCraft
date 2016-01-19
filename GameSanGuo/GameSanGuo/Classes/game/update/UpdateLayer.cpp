//
//  UpdateLayer.cpp
//  WarCraftCardGame
//
//  Created by canglang on 13-2-18.
//
//

#include "UpdateLayer.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "AppMacros.h"
#include "SGStringConfig.h"
#include "SGUpdateManager.h"
#include "SGTools.h"
#include "SGTouchLayer.h"

//每0.2秒更新一次文本进度。
#define UPDATE_TICK_TIME 0.2f

//调整位置动作的速度大小。
const static float SLIDE_SPEED = 1600.0;

UpdateLayer::UpdateLayer()
{
	updateProgress = NULL;//进度条
    labelSchedule = NULL;
    labelResourceBag = NULL;
    m_plabeDot1 = NULL;
    m_plabeDot2 = NULL;
    m_plabeDot3 = NULL;
    scalebg= NULL;
    labelTitle = NULL;
    m_showDotCount = 0;
    
    _updFrame = NULL;
    _updBkg = NULL;
    _updTouchLayer = NULL;
    _updPostersPanel = NULL;
    _updSpotsPanel = NULL;
    _updMaskLayer = NULL;
    _updCurPosterIndex = 0;
    _updIsAutoPlaying = false;
}

UpdateLayer::~UpdateLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_updateLayer);
    this->unschedule(schedule_selector(UpdateLayer::checkingUpdate));
}

UpdateLayer* UpdateLayer::create(bool b)
{
	UpdateLayer* pRet = new UpdateLayer();
	if ( pRet && pRet->initWithData())
    {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool UpdateLayer::initWithData()
{
    if (!SGBaseLayer::init(NULL, sg_updateLayer)) {
        return false;
    }

    viewDidLoad();
    setUpdateLayerShow();
    
    this->schedule(schedule_selector(UpdateLayer::checkingUpdate), UPDATE_TICK_TIME);
	
	return true;
}

//界面加载
void UpdateLayer::viewDidLoad()
{
    ResourceManager::sharedInstance()->bindTexture("updatelayer/updatelayer.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/logo.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/login_ip5.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("login_ip5.png");
    
    spriteBackground->setScaleY(CCDirector::sharedDirector()->getWinSize().height / spriteBackground->getContentSize().height);
    spriteBackground->setScaleX(CCDirector::sharedDirector()->getWinSize().width / spriteBackground->getContentSize().width);
    
    labelTitle = CCLabelTTF::create(str_UpdateFail_str5, FONT_PANGWA, 36);
    labelResourceBag = SGCCLabelTTF::create(str_UpdateFail_str13, FONT_BOXINFO, 32);
    
    //CCSprite *logo = CCSprite::createWithSpriteFrameName("logo.png");
    //this->addChild(logo,10);
    //logo->setPosition(ccpAdd(center, ccp(0, spriteBackground->getContentSize().height * 0.11)));
    
    spriteBackground->setPosition(center);
    labelTitle->setPosition(ccpAdd(upCenter, ccp(0, -100)));
    labelResourceBag->setPosition(ccpAdd(center, ccp(-55, -350)));
    
    int dotSize = 26;
	m_plabeDot1 = SGCCLabelTTF::create(".", FONT_PANGWA, dotSize);
    m_plabeDot2 = SGCCLabelTTF::create(".", FONT_PANGWA, dotSize);
    m_plabeDot3 = SGCCLabelTTF::create(".", FONT_PANGWA, dotSize);
    m_plabeDot1->setPosition(ccpAdd(labelResourceBag->getPosition(), ccp(labelResourceBag->getContentSize().width/2+12, 3)));
    m_plabeDot2->setPosition(ccpAdd(labelResourceBag->getPosition(), ccp(labelResourceBag->getContentSize().width/2+32, 3)));
    m_plabeDot3->setPosition(ccpAdd(labelResourceBag->getPosition(), ccp(labelResourceBag->getContentSize().width/2+52, 3)));
    
    labelSchedule = CCLabelTTF::create("", FONT_BOXINFO, 35);
    labelSchedule->setAnchorPoint(ccp(0, 0.5));
    labelSchedule->setPosition(ccpAdd(m_plabeDot3->getPosition(), ccp(m_plabeDot3->getContentSize().width, 0)));
    
    //灰色蒙版。
    _updMaskLayer = CCLayerColor::create(ccc4(0,0,0,180));
    //_updMaskLayer->setVisible(false);
    
    const int eleZorder = 120;
    
    addChild(spriteBackground,-1);
    addChild(_updMaskLayer, 15);
    addChild(labelTitle, eleZorder);
    addChild(labelSchedule, eleZorder);
    addChild(labelResourceBag, eleZorder);
    addChild(m_plabeDot1, eleZorder);
    addChild(m_plabeDot2, eleZorder);
    addChild(m_plabeDot3, eleZorder);
    m_plabeDot1->setVisible(true);
    m_plabeDot2->setVisible(true);
    m_plabeDot3->setVisible(true);
    //进度条和进度条背景
    scalebg =  CCSprite::createWithSpriteFrameName("loadingbg.png");
	CCSprite *progressSprite = CCSprite::createWithSpriteFrameName("loading_progress.png");
    updateProgress = CCProgressTimer::create(progressSprite);
	updateProgress->setType(kCCProgressTimerTypeBar);
	updateProgress->setPercentage(0);
	updateProgress->setMidpoint(ccp(0, 0));
	updateProgress->setBarChangeRate(ccp(1, 0));
	
    
    scalebg->setAnchorPoint(ccp(0, 0.5));
    updateProgress->setAnchorPoint(ccp(0, 0.5));
    
    scalebg->setPosition(ccpAdd(bottomCenter, ccp(-SGLayout::getSkewing(140), 180)));
    updateProgress->setPosition(ccpAdd(bottomCenter, ccp(-SGLayout::getSkewing(114), 182)));
    
    if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)//ipad,ipad2
    {
        scalebg->setPosition(ccpAdd(bottomCenter, ccp(-SGLayout::getSkewing(110), 190)));
        updateProgress->setPosition(ccpAdd(bottomCenter, ccp(-SGLayout::getSkewing(88), 192)));
    }
    
    //初始化顯示控制。
    bool isShowSth = true;
    labelSchedule->setVisible(isShowSth);
    scalebg->setVisible(isShowSth);
    updateProgress->setVisible(isShowSth);
    
    addChild(scalebg, eleZorder + 2);
    addChild(updateProgress, eleZorder + 5);
}

void UpdateLayer::setUpdateLayerShow()
{
    this->setVisible(true);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(false);
    }
}

//更新检测函数，每0.2秒触发一次。
void UpdateLayer::checkingUpdate()
{
    SGUpdateManager* sgum = SGUpdateManager::gi();
    if (sgum->getUpdateState() != SGUM_UPDATE_SUCCESS) //若未完成更新。
    {
        //设置动态文本。
        setTheLabelString();
        //调整幻灯片。
        playUpdPosters();
    }
    else
    {
        CCLOG("UpdateLayer::checkingUpdate() finished.");
        setUpdateProgress(100.0);
        this->unschedule(schedule_selector(UpdateLayer::checkingUpdate));
        
        //进入游戏。
        CCLOG("autoLogin=update completed!");
        sgum->autoLogin();
    }
}

//是否开始了触控。
static bool m_isTouched = false;

//多久切换一张图。
const static float CHANGE_POSTER_TIME = 6.0f;

//当前已进行的秒数
static float curCpTime = 0.0f;

//动态调整幻灯片。
void UpdateLayer::playUpdPosters()
{
    if (_updFrame && _updIsAutoPlaying)
    {
        curCpTime += UPDATE_TICK_TIME;
        if (curCpTime > CHANGE_POSTER_TIME)
        {
            if (_updCurPosterIndex >= 0 && _updCurPosterIndex < SLIDING_IMAGES_NUM - 1)
            {
                m_isTouched = true; //这样阻止玩家在播放中对posters进行操作。
                float dura = fabs( (_updPostersPanelPos[_updCurPosterIndex].x - _updPostersPanelPos[_updCurPosterIndex + 1].x) / SLIDE_SPEED );;
                _updCurPosterIndex++;
                _updPostersPanel->runAction( CCSequence::create(CCMoveTo::create(dura, _updPostersPanelPos[_updCurPosterIndex]), CCCallFunc::create(this, callfunc_selector(UpdateLayer::resetHelloParam)), NULL) );
            }
            else
            {
                _updIsAutoPlaying = false;
            }
            curCpTime = 0;
        }
    }
}

//上次更新完成的文件数量的数字。
static int lastDNum = -1;

void UpdateLayer::setTheLabelString()
{
    //闪动的"..."
    m_plabeDot1->setVisible(m_showDotCount == 1||m_plabeDot1->isVisible());
    m_plabeDot2->setVisible(m_showDotCount == 2||m_plabeDot2->isVisible());
    m_plabeDot3->setVisible(m_showDotCount == 3||m_plabeDot3->isVisible());
    
    m_showDotCount++;
    if (m_showDotCount > 4)
    {
        m_plabeDot1->setVisible(false);
        m_plabeDot2->setVisible(false);
        m_plabeDot3->setVisible(false);
        m_showDotCount = 0;
    }
    
    //百分比
    SGUpdateManager* sgum = SGUpdateManager::gi();
    int dNum = sgum->getDoneResNum();
    int rNum = sgum->getRemainingResNum();
    
    if (dNum == lastDNum)
    {
        return;
    }
    
    lastDNum = dNum;
    
    float percent = 0.0f;
    int denom = rNum + dNum;
    
    if (denom > 0)
    {
        percent = ( (float) dNum / (float) denom ) * 100;
    }

    setUpdateProgress(percent);
}

//设置资源更新进度
void UpdateLayer::setUpdateProgress(float percent)
{
    //进度没有满
    updateProgress->setPercentage(percent);
    
    CCString* str = CCString::createWithFormat("%d", (int) percent);
    str->m_sString.append("%");
    labelSchedule->setString(str->getCString());
}

//开启幻灯片更新。
void UpdateLayer::switchUpdFrame(bool onoff)
{
    _updIsAutoPlaying = true;
    _updMaskLayer->setVisible(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("updatelayer/film1.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    rm->bindTexture("updatelayer/film2.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    rm->bindTexture("updatelayer/film3.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    rm->bindTexture("updatelayer/film4.plist", RES_TYPE_LAYER_UI, sg_updateLayer);
    
    //pos
    _updBasePos = ccp(0, winSize.height * 0.5 - 200);
    
    //frame
    _updFrame = CCLayerColor::create(ccc4(0,255,0,0), winSize.width, winSize.height);
    _updFrame->ignoreAnchorPointForPosition(false);
    _updFrame->setAnchorPoint(ccp(0.5, 0.5));
    _updFrame->setPosition(winSize * 0.5);
    
    //touchlayer
    _updTouchLayer = SGTouchLayer::create(winSize.width, 400, -128, ccc4(0,0,255,0));
    _updTouchLayer->setAnchorPoint(ccp(0,0));
    _updTouchLayer->setPosition(_updBasePos);
    _updTouchLayer->setControlMode(this, pf_began_selector(UpdateLayer::helloBegan), pf_moved_selector(UpdateLayer::helloMoved), pf_ended_selector(UpdateLayer::helloEnded), pf_cancelled_selector(UpdateLayer::helloCancelled));
    
    _updFrame->addChild(_updTouchLayer, 10);
    
    //star bkg
//    _updBkg = CCSprite::createWithSpriteFrameName("starSky.png");
//    _updBkg->setOpacity(150);
//    _updBkg->setScaleX(winSize.width / _updBkg->getContentSize().width);
//    _updBkg->setScaleY(winSize.height / _updBkg->getContentSize().height);
//    _updBkg->setPosition(winSize * 0.5);
//    _updFrame->addChild(_updBkg, 5);
    
    //posters' panel
    _updPostersPanel = CCLayerColor::create(ccc4(255,0,0,0), winSize.width, 200);
    _updPostersPanel->ignoreAnchorPointForPosition(false);
    _updPostersPanel->setAnchorPoint(ccp(0,0));
    _updPostersPanel->setPosition(_updBasePos);
    _updFrame->addChild(_updPostersPanel, 15);
    
    //posters
    _updCurPosterIndex = 0;
    CCSprite* spt;
    char sptName[20];
    float sptPosX = 0;
    CCSize sptSize;
    for (int k = 0; k < SLIDING_IMAGES_NUM; ++k)
    {
        sprintf(sptName, "poster%d.png", k);
        spt = CCSprite::createWithSpriteFrameName(sptName);
        spt->setAnchorPoint(ccp(0,0));
        sptSize = spt->getContentSize();
        //适配spt大小。
        float scale = 1;
        if (sptSize.width < winSize.width)
        {
            scale = winSize.width / sptSize.width;
            spt->setScale(scale);
        }
        float showWidth = sptSize.width * scale;
        
        spt->setPosition(ccp(sptPosX, 0));
        _updPostersPanel->addChild(spt, 10);
        //注意这里panel的移动方向和sptPosX是正好反过来的。
        _updPostersPanelPos[k] = ccp( -sptPosX - (showWidth - winSize.width) * 0.5, _updBasePos.y);
        sptPosX += showWidth;
        _updPosters[k] = spt;
    }
    
    //spots
    _updSpotsPanel = PointLayer::create(SLIDING_IMAGES_NUM, winSize.width, "anDian.png", "liangDian.png");
    _updSpotsPanel->setPosition(ccp(winSize.width * 0.5, _updBasePos.y - 50));
    _updFrame->addChild(_updSpotsPanel, 20);
    
    
    this->addChild(_updFrame, 100);
}

//首次触控点x。
static float m_beganX = 0;

//是否可以开始移动，防止小的移动。
static bool m_canMove = false;

//本次移动的总距离。
static float m_dist = 0;

//触控时panel的原位置X。
static float m_panelOldPosX = 0;

//移动超过多少则开始移动。
const static float SLIDE_LIMIT = 10;

//移动超过多少则切换幻灯片。
const static float SLIDE_CHANGE_DISTANCE = 70;

//began
bool UpdateLayer::helloBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_isTouched)
        return false;
    //    CCLOG(">>>>武将详情ccTouchBegan");
    m_isTouched = true;
    m_beganX = pTouch->getLocation().x;
    m_panelOldPosX = _updPostersPanel->getPositionX();
    return true;
}

//moved
void UpdateLayer::helloMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isTouched)
        return;
    CCPoint tp = pTouch->getLocation();
    m_dist = (tp.x - m_beganX);
    //X向移动距离>10，切未在移动中，则允许移动
    if (fabs(m_dist) >= SLIDE_LIMIT && !m_canMove)
    {
        m_canMove = true;
        _updIsAutoPlaying = false;
    }
    if (m_canMove)
    {
        this->_updPostersPanel->setPositionX(m_panelOldPosX + m_dist);
    }
}

//ended
void UpdateLayer::helloEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isTouched)
        return;
    movePoster();
}

//cancelled
void UpdateLayer::helloCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_isTouched)
        return;
    movePoster();
}

//移动layer
void UpdateLayer::movePoster()
{
    if (m_isTouched && m_canMove)
    {
        float dura = 0.0;
        if (m_dist > SLIDE_CHANGE_DISTANCE) //向右移动，则向左展现
        {
            if (_updCurPosterIndex <= 0 ) //第一张
            {
                dura = fabs( m_dist / SLIDE_SPEED );
            }
            else
            {
                dura = fabs( (_updPostersPanelPos[_updCurPosterIndex].x - _updPostersPanelPos[_updCurPosterIndex - 1].x) / SLIDE_SPEED );
                _updCurPosterIndex--;
            }
            
        }
        else if (m_dist < -SLIDE_CHANGE_DISTANCE) //向左移动，则向右展现
        {
            if (_updCurPosterIndex >= SLIDING_IMAGES_NUM - 1) //最后一张
            {
                dura = fabs( m_dist / SLIDE_SPEED );
            }
            else
            {
                dura = fabs( (_updPostersPanelPos[_updCurPosterIndex].x - _updPostersPanelPos[_updCurPosterIndex + 1].x) / SLIDE_SPEED );
                _updCurPosterIndex++;
            }
        }
        else //回归原位，不用动。
        {
            dura = fabs( m_dist / SLIDE_SPEED );
        }
        _updPostersPanel->runAction( CCSequence::create(CCMoveTo::create(dura, _updPostersPanelPos[_updCurPosterIndex]), CCCallFunc::create(this, callfunc_selector(UpdateLayer::resetHelloParam)), NULL) );
    }
    else
    {
         m_isTouched = m_canMove = false;
    }
}

//重置滑动参数。
void UpdateLayer::resetHelloParam()
{
    m_isTouched = m_canMove = false;
    //切记！！这里要传入自然索引！！
    this->_updSpotsPanel->setLight(_updCurPosterIndex + 1);
}

//修改提示玩家的文本，从“检查更新”到“系统更新”。
void UpdateLayer::setLabelInfo(std::string str)
{
    if (labelResourceBag)
    {
        labelResourceBag->setString(str.c_str());
    }
//    //虽然费解，但是实际上，使用这个API的话，一定是将“检查更新”变成了“系统更新”，所以在这里将更新的进度指示相关的元件全部显示出来，算是一个业务HACK。
//    bool isShowSth = true;
//    scalebg->setVisible(isShowSth);
//    updateProgress->setVisible(isShowSth);
//    labelSchedule->setVisible(isShowSth);
}
