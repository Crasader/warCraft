//
//  SGStrengEffectLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-5-6.
//
//

#include "SGStrengEffectLayer.h"

#include "SGMainManager.h"
#include "SGGuideManager.h"
#include "SGStaticDataManager.h"
SGStrengEffectLayer::SGStrengEffectLayer():
card(NULL),
successLvl(0),
enterType(0),
lvlupAct(NULL)
{
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(false);
//    this->setTouchMode(kCCTouchesOneByOne);
//    this->setTouchPriority(-200);
//    this->setTouchEnabled(true);
}
SGStrengEffectLayer::~SGStrengEffectLayer()
{
	//  ResourceManager::sharedInstance()->unBindLayerTexture(sg_strengEffLayer);
    lvlupAct->release(); //需要释放，因为回调之前进行了持有操作。
    
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , true);
//    this->setTouchEnabled(false);
//    menu->setTouchEnabled(false);
}


SGStrengEffectLayer *SGStrengEffectLayer::create(int enterType, int successLvl, SGBaseMilitaryCard *card)
{
	SGStrengEffectLayer *eff = new SGStrengEffectLayer();
	
	if (eff && eff->init(NULL, box_strengEffectLayer, CCSizeMake(100, 100), false, false))
	{
		eff->enterType = enterType;
		eff->successLvl = successLvl;
		eff->card = card;
		eff->initView();
		eff->autorelease();
		return eff;
	}
	CC_SAFE_RELEASE(eff);
	return NULL;
}


void SGStrengEffectLayer::initView()
{
	CCPoint pos;
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	if (s.height == 960)
	{
		pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -451));
	}
	else if (s.height == 1136)
	{
		pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -537));
	}
	else
	{
		pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -537));
	}
	
	CCLayerColor *layerColor = CCLayerColor::create(ccc4(0, 0, 0, 0), s.width, s.height);
	this->addChild(layerColor, 90);

	
	lvlupAct = CCSpriterX::create(CCString::createWithFormat("animationFile/success%d.scml",successLvl)->getCString(),true, CCCallFunc::create(this, callfunc_selector(SGStrengEffectLayer::refreshLayer)),true);
    lvlupAct->retain();
	lvlupAct->setanimaID(0);
	lvlupAct->setisloop(false);
	addChild(lvlupAct,100);
	lvlupAct->setPosition(ccpAdd(pos, ccp(0, 200)));
	lvlupAct->play();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true); //INT32_MAX
    
	this->setIsCanTouch(true);
}

bool SGStrengEffectLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	//this->stopAllActions();
    lvlupAct->stop();
    SGMainManager::shareMain()->showStrengLayer(card, enterType);
	// 如果在动画界面直接点击,直接跳回强化页面
	SGMainManager::shareMain()->closeBox();
    //boxClose();
	
    //this->autorelease();
    SGGuideManager::shareManager()->checkIsDone(guide_tag_17);
    
    //显示调用析构，否则出现sgbasebox加载的三张图和lvlupAct出现无法释放的情况，如果出现问题也要显式调用SGBaseBox::~SGBaseBox();
    //SGBaseBox::~SGBaseBox();
	return true;
}

void SGStrengEffectLayer::refreshLayer()
{  
    int sumFrame = lvlupAct->getFrameNuminAnimator(0);
    CCLog("5566%d",sumFrame);
    lvlupAct->stopAllActions();
    CCSpriterX *LastFrame = NULL;
    

    if(successLvl==1)
    {
        LastFrame = CCSpriterX::create("animationFile/chenggong2.scml",true, true);

    }
    else if(successLvl==2)
    {
        LastFrame = CCSpriterX::create("animationFile/dachengong2.scml",true, true);
    }
    else if(successLvl==3)
    {
        LastFrame = CCSpriterX::create("animationFile/chaochenggong2.scml",true, true);
    }
	LastFrame->setanimaID(0);
	LastFrame->setisloop(true);
	addChild(LastFrame,102);
	LastFrame->setAnchorPoint(ccp(0.5,0.5));
	LastFrame->setPosition(lvlupAct->getPosition());

	LastFrame->play();

}


















