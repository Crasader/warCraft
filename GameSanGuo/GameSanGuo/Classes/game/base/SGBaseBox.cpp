//
//  SGBaseBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-27.
//
//

#include "SGBaseBox.h"
#include "cocos-ext.h"
#include "GameMusicmanage.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
SGBaseBox::SGBaseBox()
:data(NULL)
,deletage(NULL)
,biao(NULL)
,fontbg(NULL)
,boxSize(CCSizeZero)
{
//     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/bsxx.plist");
	
}
SGBaseBox::~SGBaseBox()
{
    CCLOG("~SGBaseBox");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_baseBox);
}
bool SGBaseBox::init(SGBoxDelegate *dg, BoxTag bt, CCSize size, bool ishaveinfobg,bool isHaveBg,CCSize bottomsize,bool useDefaultBg)
{
    if (!SGBaseShow::init(NULL, bt, false))
    {
        return false;
    }
    
 
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_baseBox, LIM_PNG_AS_PNG);

    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_baseBox, LIM_PNG_AS_PNG);

    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_baseBox);
    
	
    CCSize s = CCDirector::sharedDirector() -> getWinSize();
    boxSize = size;
    deletage = dg;
    if (isHaveBg)
    {
        CCPoint center = ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 0));
        CCSprite *bg1 = NULL;
        if (useDefaultBg)
        {
            biao=CCSprite::createWithSpriteFrameName("box_bian.png");
            this->addChild(biao, -1);
            biao->setPosition(ccpAdd(center, ccp(0, size.height*.5 -45)));
            
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_baseBox);

			cocos2d::extension::CCScale9Sprite *frame2 = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("Tips.png");
            //cocos2d::extension::CCScale9Sprite *frame2 = cocos2d::extension::CCScale9Sprite::create("sanguobigpic/Tips.png");

			this->addChild(frame2,0);
            frame2->setScaleX(size.width/frame2->getContentSize().width);
            frame2->setScaleY(size.height/frame2->getContentSize().height);
			frame2->setPosition(ccpAdd(center, ccp(0,0)));

            
            CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 180), skewing(320)*3, skewingY(960)*3);
            this->addChild(bgLayer,-3);
            bgLayer->setPosition(ccpAdd(CCPointZero, ccp(-bgLayer->getContentSize().width*.5, -bgLayer->getContentSize().height*.5)));
            
        }
        else
        {
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_baseBox);

            bg1 = CCSprite::createWithSpriteFrameName("barrack_bg.png");
            this->addChild(bg1 ,-2);

            bg1->setScaleY(s.height/bg1->getContentSize().height);
            bg1->setScaleX(s.width/bg1->getContentSize().width);
            bg1->setPosition(center);
        }
        
        if (ishaveinfobg)
        {
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_baseBox);

            fontbg = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
            this->addChild(fontbg,0);
            fontbg->setPreferredSize(CCSizeMake(size.width-55, size.height-162));//-112

            fontbg->setPosition(ccpAdd(center, ccp(0, 0)));//-27
        }


    }
    this->runactionwithscale();
    return true;
}

//关闭窗口不回调函数
void SGBaseBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    if(deletage){
        deletage->setIsCanTouch(true);
    }
    this->removeFromParentAndCleanup(true);
    SGMainManager::shareMain()->getMainScene()->nowBoxTag = box_null;
}

void SGBaseBox::boxClose()
{
    if (deletage)
    {
        deletage->setIsCanTouch(true);
        deletage->showBoxCall(data);
    }
    this->removeFromParentAndCleanup(true);
    SGMainManager::shareMain()->getMainScene()->nowBoxTag = box_null;
}

void SGBaseBox::setIsCanTouch(bool isTouch)
{
    menu->setTouchEnabled(isTouch);
    
}

void SGBaseBox::runactionwithscale()
{
//    //CCScaleTo *scale1=CCScaleTo::create(0.2,1.07);     //cgp root
//    //CCScaleTo *scale2=CCScaleTo::create(0.1,0.95);
//    CCScaleTo *scale3=CCScaleTo::create(0.3,1.0);
//    //CCScaleBy *scale2=CCScaleBy::create(2,0.5);
//    //CCSequence*action=CCSequence::create(scale1,scale2,scale3,NULL);
////    CCSequence*action=CCSequence::create(scale1,scale3,NULL);
//     CCSequence*action=CCSequence::create(scale3,NULL);
//    this->runAction(action);
}





