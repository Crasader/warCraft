//
//  SGCountryLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#include "SGCountryLayer.h"
#include "SGCountryInfoLayer.h"
#include "SGMainManager.h"
#include "SGPopWord.h"
#include "cocos-ext.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
USING_NS_CC_EXT;

#define  BGCHILDTAG  10000
#define  CCLAYERCOLORTAG  20
SGCountryLayer::SGCountryLayer()
:buttonWei(NULL), //魏
buttonShu(NULL),//蜀
buttonWu(NULL),  //吴
spriteOther(NULL), //群英
s(CCSizeZero),
hoshi(NULL),
mmmm(NULL)
{
}

SGCountryLayer::~SGCountryLayer()
{
     ResourceManager::sharedInstance()->unBindLayerTexture(sg_countryLayer);
}


SGCountryLayer *SGCountryLayer::create()
{
    SGCountryLayer *countryLayer = new SGCountryLayer();
    if (countryLayer && countryLayer->init(NULL, sg_countryLayer))
    {
        
        countryLayer->constructView();
        countryLayer->autorelease();
        return countryLayer;
    }
    
    
    CC_SAFE_DELETE(countryLayer);
    return NULL;
}
void SGCountryLayer::constructView()
{

    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist",RES_TYPE_LAYER_UI ,sg_countryLayer);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    
     s = CCDirector::sharedDirector()->getWinSize();
    

    CCSprite *title_bg_ll = CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_bg_ll->setAnchorPoint(ccp(0, 1));
    title_bg_ll->setPosition(ccpAdd( SGLayout::getPoint(kUpLeft), ccp(0, s.height*0.3)));
   // title_bg_ll->setPosition(ccp(0,0));
    this->addChild(title_bg_ll,1,100);
   // CCSprite *title_bg_rr = CCSprite::createWithSpriteFrameName("country_titlebg_l.png");
    CCSprite *title_bg_rr = CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_bg_rr->setFlipX(true);
    title_bg_rr->setAnchorPoint(ccp(1, 1));
    title_bg_rr->setPosition(ccpAdd( SGLayout::getPoint(kUpRight), ccp(0, s.height*0.3)));
   // title_bg_rr->setPosition(ccp(title_bg->getContentSize().width*1.0, 0));
    this->addChild(title_bg_rr,1,101);
    
    
    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("choosecountry.png");
    spriteTitle->setAnchorPoint(ccp(0.5, 1));
    spriteTitle->setPosition(ccpAdd( upCenter, ccp(0, s.height*0.3)));
    this->addChild(spriteTitle,1,102);
    spriteTitle->setVisible(true);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_countryLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/country_bg_l.plist", RES_TYPE_LAYER_UI, sg_countryLayer);

    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("storyBg.png");
    spriteBackground->setScaleX(s.width / spriteBackground->getContentSize().width);
    this->addChild(spriteBackground,-1);
    spriteBackground->setPosition(center);
    
    CCScale9Sprite *bgl = CCScale9Sprite::createWithSpriteFrameName("country_bg_l.png");
    this->addChild(bgl,-1);
    //bgl->setAnchorPoint(ccp(0, 0.5));
    bgl->setPreferredSize(CCSizeMake(s.width/3, s.height/2*3));
    bgl->setPosition(ccpAdd(center, ccp(-bgl->getContentSize().width, 0)));
    
    CCScale9Sprite *bgm = CCScale9Sprite::createWithSpriteFrameName("country_bg_l.png");
    this->addChild(bgm,-1);
    //bgm->setAnchorPoint(ccp(0, 0.5));
    bgm->setPreferredSize(CCSizeMake(s.width/3, s.height/2*3));
    bgm->setPosition(ccpAdd(center, ccp(0, 0)));
    
    CCScale9Sprite *bgr = CCScale9Sprite::createWithSpriteFrameName("country_bg_l.png");
    this->addChild(bgr,-1);
   // bgr->setAnchorPoint(ccp(0, 0.5));
    bgr->setPreferredSize(CCSizeMake(s.width/3, s.height/2*3));
    bgr->setPosition(ccpAdd(center, ccp(bgr->getContentSize().width, 0)));


    
    buttonWei = SGButton::create("country_wei.png", "country_wei.png", this, menu_selector(SGCountryLayer::weiHandler),CCPointZero,false,true);
    this->addBtn(buttonWei);
//    buttonWei->setPosition(ccpAdd(center, ccp(6, 290)));
    buttonWei->setPosition(ccpAdd(center, ccp(6-s.width, 290)));

    buttonShu = SGButton::create("country_shu.png", "country_shu.png", this, menu_selector(SGCountryLayer::shuHandler),CCPointZero,false,true);
    this->addBtn(buttonShu);
//    buttonShu->setPosition(ccpAdd(center, ccp(6, 118)));
     buttonShu->setPosition(ccpAdd(center, ccp(6+s.width, 118)));
    
    buttonWu = SGButton::create("country_wu.png", "country_wu.png", this, menu_selector(SGCountryLayer::wuHandler),CCPointZero,false,true);
    this->addBtn(buttonWu);
//    buttonWu->setPosition(ccpAdd(center, ccp(6, -50)));
    buttonWu->setPosition(ccpAdd(center, ccp(6-s.width, -50)));
    
    spriteOther = CCSprite::createWithSpriteFrameName("country_other.png");
    this->addChild(spriteOther);
//    spriteOther->setPosition(ccpAdd(center, ccp(6, -216)));
     spriteOther->setPosition(ccpAdd(center, ccp(6+s.width, -216)));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 210), s.width, 135);
    this->addChild(layer,1,CCLAYERCOLORTAG);
    layer->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(0, -s.height*0.3) ) );
    /////////////////////////////////////////////////////////////
    //两条线
    CCSprite *spriteLine1 = CCSprite::createWithSpriteFrameName("country_line.png");
    CCSprite *spriteLine2 = CCSprite::createWithSpriteFrameName("country_line.png");
    spriteLine1->setAnchorPoint(ccp(0, 0.5));
    spriteLine2->setAnchorPoint(ccp(0, 0.5));
    spriteLine1->setPosition( ccp(0,layer->getContentSize().height));
    spriteLine2->setPosition(ccp(0, 0));
    spriteLine1->setScaleX(s.width/spriteLine1->getContentSize().width);
    spriteLine2->setScaleX(s.width/spriteLine2->getContentSize().width);
//    this->addChild(spriteLine1,10,20);
//    this->addChild(spriteLine2,10,21);
    layer->addChild(spriteLine1);
    layer->addChild(spriteLine2);
    

    
    SGCCLabelTTF *notice = SGCCLabelTTF::create(str_CGMovieLayer_str10, FONT_FZXK, 24 , ccc3(0xe7, 0xc4, 0x6a));
    //modify by:zyc. merge into create.
    //notice->setColor(ccc3(0xe7, 0xc4, 0x6a));
    //    notice->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 85)));
    notice->setPosition( ccp(layer->getContentSize().width*.5,layer->getContentSize().height*.5));
    layer->addChild(notice);
    
    
    
    this->setTouchEnabled(false);
    menu->setEnabled(true);
//   /////切入效果
    this->showeffect();
}

void SGCountryLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool SGCountryLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
}

void SGCountryLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    removeChildByTag(BGCHILDTAG, true);
    this->setTouchEnabled(false);
    menu->setEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    //touchend时show这个干什么？
    /////切入效果
//    this->showeffect();
}


void SGCountryLayer::weiHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    selectCountry(2);
}
void SGCountryLayer::shuHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
     selectCountry(1);
}
void SGCountryLayer::wuHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
     selectCountry(3);
}
void SGCountryLayer::selectCountry(int countryid)
{
    SGCountryInfoLayer *countryInfoLayer = SGCountryInfoLayer::create(this,countryid);
    SGMainManager::shareMain()->showBox(countryInfoLayer);
}
void SGCountryLayer::confirmHandler()
{
    this->selectCountry(1);
}



void SGCountryLayer::showeffect()
{

    


    
    CCSequence* seq=CCSequence::create(CCCallFunc::create(this, callfunc_selector(SGCountryLayer::wei)),
                                       CCDelayTime::create(0.15f),
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::shu)),
                                       CCDelayTime::create(0.15f),
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::wu)),
                                       CCDelayTime::create(0.15f),
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::qun)),
                                        CCCallFunc::create(this, callfunc_selector(SGCountryLayer::bgaction)),
                                       CCDelayTime::create(0.9f),
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::lowerlabelaction)),
                                       CCDelayTime::create(0.15f),
                                       CCSpawn::create(
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::uplabelaction1)),
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::uplabelaction2)),
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::uplabelaction3)),
                                                        NULL)
                                     
                                       ,
                                       CCCallFunc::create(this, callfunc_selector(SGCountryLayer::staraction)),
                                       NULL);
    
    this->runAction(seq);
    
}


void SGCountryLayer::wei()////魏国图片动作
{
    
    CCMoveBy*movefromleft=CCMoveBy::create(0.25f, ccp(s.width*1.1, 0));///从左面切入
    CCMoveBy*doudong1=CCMoveBy::create(0.15f, ccp(-s.width*0.2, 0));//抖动效果
    CCMoveBy*doudong2=CCMoveBy::create(0.15f, ccp(s.width*0.1, 0));
    CCSequence*action=CCSequence::create(movefromleft,doudong1,doudong2,NULL);
    buttonWei->runAction(action);

}

void SGCountryLayer::shu()////蜀国图片动作
{
    CCMoveBy*movefromright=CCMoveBy::create(0.25f, ccp(-s.width*1.1, 0)); ///从右面切入
    CCMoveBy*doudong1=CCMoveBy::create(0.15f, ccp(s.width*0.2, 0));//抖动效果
    CCMoveBy*doudong2=CCMoveBy::create(0.15f, ccp(-s.width*0.1, 0));
     CCSequence*action=CCSequence::create(movefromright,doudong1,doudong2,NULL);
    buttonShu->runAction(action);
}



void SGCountryLayer::wu()////吴国图片动作
{
    CCMoveBy*movefromleft=CCMoveBy::create(0.25f, ccp(s.width*1.1, 0));///从左面切入
    CCMoveBy*doudong1=CCMoveBy::create(0.15f, ccp(-s.width*0.2, 0));//抖动效果
    CCMoveBy*doudong2=CCMoveBy::create(0.15f, ccp(s.width*0.1, 0));
    CCSequence*action=CCSequence::create(movefromleft,doudong1,doudong2,NULL);
    buttonWu->runAction(action);
    
}


void SGCountryLayer::qun()////群英图片动作
{
    CCMoveBy*movefromright=CCMoveBy::create(0.25f, ccp(-s.width*1.1, 0)); ///从右面切入
    CCMoveBy*doudong1=CCMoveBy::create(0.15f, ccp(s.width*0.2, 0));//抖动效果
    CCMoveBy*doudong2=CCMoveBy::create(0.15f, ccp(-s.width*0.1, 0));
    CCSequence*action=CCSequence::create(movefromright,doudong1,doudong2,NULL);
    spriteOther->runAction(action);
}


void SGCountryLayer::lowerlabelaction()
{
    CCMoveBy*movefromdown=CCMoveBy::create(0.15f, ccp(0, s.height*0.3)); ///从下面切入
    CCLayerColor* layer=(CCLayerColor*)this->getChildByTag(CCLAYERCOLORTAG);
    layer->runAction(movefromdown);
}

void SGCountryLayer::uplabelaction1()
{
    CCMoveBy*movefromup=CCMoveBy::create(0.15f, ccp(0, -s.height*0.3)); ///从上面切入
    CCSprite* sp=(CCSprite*)this->getChildByTag(100);
    sp->runAction(movefromup);
  
    
}
void SGCountryLayer::uplabelaction2()
{
    CCMoveBy*movefromup=CCMoveBy::create(0.15f, ccp(0, -s.height*0.3)); ///从上面切入
    CCSprite* sp=(CCSprite*)this->getChildByTag(101);
    sp->runAction(movefromup);
    
    
}
void SGCountryLayer::uplabelaction3()
{
    CCMoveBy*movefromup=CCMoveBy::create(0.15f, ccp(0, -s.height*0.3)); ///从上面切入
    CCSprite* sp=(CCSprite*)this->getChildByTag(102);
    sp->runAction(movefromup);
    
    
}



void SGCountryLayer::staraction()
{
    
    CCSprite* sp=(CCSprite*)this->getChildByTag(102);
    CCSize size=sp->getContentSize();
    //星星
    hoshi=CCSprite::createWithSpriteFrameName("xingxing.png");

    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    hoshi->setPosition(ccpAdd(upCenter,ccp(-sp->getContentSize().width*0.5, -sp->getContentSize().height*0.3) ));
    this->addChild(hoshi,999);
    
    CCRotateBy *rotate=CCRotateBy::create(0.15f, 360);
    CCMoveBy *move=CCMoveBy::create(0.70f, ccp(size.width, 0));
    CCSequence*action=CCSequence::create(CCSpawn::create(rotate,move,NULL),CCCallFunc::create(this, callfunc_selector(SGCountryLayer::setmiss)),NULL);
    
    hoshi->runAction(action);
    
    
    
}


void SGCountryLayer::setmiss(CCObject*obj) //设置星星不可见
{
   
    hoshi->setVisible(false);
  
}

void  SGCountryLayer::bgaction()
{
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/country_blackbg.plist", RES_TYPE_LAYER_UI, sg_countryLayer);

    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCScale9Sprite *blackbg = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("country_blackbg.png");
    blackbg->setPreferredSize(CCSizeMake(570,735));
    this->addChild(blackbg,-1);
    blackbg->setPosition(ccpAdd(center, ccp(0, 15)));
    //CCHide*hide=CCHide::create();
    CCFadeIn*fadein=CCFadeIn::create(0.5f);
    //CCFadeOut*fadeout=CCFadeOut::create(10.0f);
    CCSequence*seq=CCSequence::create(fadein,NULL);
    blackbg->runAction(seq);
    
    

}

