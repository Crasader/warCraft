//
//  SGVisitShowCardLayer.cpp
//  GameSanGuo
//
//  Created by zenghui on 13-11-9.
//
//

#include "SGVisitShowCardLayer.h"
#include "SGBaseMilitaryCard.h"
#include "SGOfficerCard.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGEquipInfoLayer.h"
#include "SGPropInfoLayer.h"
#include "SGStaticDataManager.h"
#include "SGVisitLayer.h"
#include "SGGuideManager.h"
#include "ResourceManager.h"
#include "SGShareBox.h"
#include "SGStringConfig.h"


#define   CARDGAP           10
#define   CARDLIMIT         6
#define   CARDTOPRATE       0.8
#define   CARDMOVEUPRATE    0.1
#define   SHOWCARDGAPTIME   0.09
#define   MENUGROPTAG       20
#define   STARGROPTAG       60
#define   DARROWTAG         100
#define   CLICKEFFTAG       200
#define   STARACTIONGAP     2
#define   DOORANCOPONTY     0.6
#define   MIDPOSCOUNTTAG    2
#define   PIECESSTARTTAG    1000
#define   PIECEMOVESPEEDMIN 0.8
#define   PIECEMOVESPEEDMAX 1.6


SGVisitShowCardLayer::SGVisitShowCardLayer():visitDoorBt(NULL)
,visitCardName(NULL)
,selectCardBt(NULL)
,visitCardPng(NULL)
,cardPngSprite1(NULL)
,cardPngSprite2(NULL)
,visitItemstar(NULL)
,_array(NULL)
,_huoArray(NULL)
,m_nCardId(0)
,m_pCardArray(NULL)
,m_nCardCount(0)
,showCardTag(0)
,showStarNum(0)
,m_MoveHaHaTime(0)
,_comboCard(NULL)
,_progressCard(NULL)
, useGoldDoor(false)
, _moveUp(0)
,la(NULL)
, movedis(0)
,_visitPiece(NULL)
, setViewWid(0)
,piecesName(NULL)
,scrollview(NULL)
,piecesLable(NULL)
,givenPieceBg(NULL)
,ok(NULL)
,buttonShare(NULL)
,piecesCoverLeft(NULL)
,piecesCoverRight(NULL)
,finaleSprite(NULL)
,propBuyCard(NULL)
,_crashNoticeLabel(NULL)
{
    _array = NULL;

    m_nCardId = 0;
    m_nCardCount = 0;

    m_pCardArray = CCArray::create();
    m_pCardArray->retain();
    
    SGBaseLayer *mlayer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    mlayer->setIsCanTouch(false);
}
SGVisitShowCardLayer::~SGVisitShowCardLayer()
{
    CC_SAFE_RELEASE(_array);
    CC_SAFE_RELEASE(m_pCardArray);
    CC_SAFE_RELEASE(visitItemstar);
    unschedule(schedule_selector(SGVisitShowCardLayer::movePiecesHaHa));
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_visitShowCard);
    
}
SGVisitShowCardLayer *SGVisitShowCardLayer::create(CCArray *array,CCArray *huoArray,SGBaseMilitaryCard *comboCard,SGBaseMilitaryCard *progressCard,bool useGoldDoor, SGPropsCard *buyPropCard)
{
    SGVisitShowCardLayer *Layer = new SGVisitShowCardLayer();
    
    if (Layer && Layer->init(NULL, sg_visitShowCard))
    {
        Layer->propBuyCard = buyPropCard;
        Layer->useGoldDoor = useGoldDoor;
        Layer->initView(array,huoArray,comboCard,progressCard);
        Layer->autorelease();
        
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}
void SGVisitShowCardLayer::initView(CCArray *array,CCArray *huoArray,SGBaseMilitaryCard *comboCard,SGBaseMilitaryCard *progressCard)
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/Darrow.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("animationFile/Doorflash.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("animationFile/Clickeff.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("animationFile/Lightspot.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("animationFile/VisitArray.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("animationFile/Cardlight.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayerlight.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    if (useGoldDoor) {
        ResourceManager::sharedInstance()->bindTexture("animationFile/visitGold.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
        ResourceManager::sharedInstance()->bindTexture("animationFile/GoldenGate.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    }
    else
    {
        ResourceManager::sharedInstance()->bindTexture("animationFile/visitSilver.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
        ResourceManager::sharedInstance()->bindTexture("animationFile/Silverdoo.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    }
    
    CCLayerColor *black = CCLayerColor::create(ccc4(0, 0, 0, 255), 920, 1200);
    this->addChild(black,-100);
    black->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-460, -600)));
    visitItemstar=CCArray::create();
    visitItemstar->retain();
    _array = CCArray::create();
    _array->retain();
    
    _moveUp = CARDTOPRATE;
    if (huoArray != NULL) {
        _huoArray = CCArray::create();
        _huoArray->retain();
        _moveUp = CARDTOPRATE + CARDMOVEUPRATE;
    }
    else
    {
        _huoArray = NULL;
    }

    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(array, obj)
    {
        _array->addObject(obj);
    }
    
    //10送1
    if(comboCard != NULL)
    {
        _array->addObject(comboCard);
    }

    if (_huoArray != NULL) {
        
        while(_huoArray->count() < 8)
        {
            CCARRAY_FOREACH(huoArray, obj)
            {
                _huoArray->addObject(obj);//自己添加猪头小泽
            }
        }
        if(progressCard!=NULL)
        {
            _huoArray->insertObject(progressCard, _huoArray->count()/2 - 1);
        }
    }
    else
    {
        if(progressCard!=NULL)
        {
            _array->addObject(progressCard);
        }
    }
    //进度条

    m_nCardCount = _array->count();
    
    _comboCard = comboCard;
    _progressCard = progressCard;
    
    ///////////////////////////////

    //创建中间的大图的按钮
    createCardShowInfo();
    
    //十连抽多个获取的卡牌
    createVisitCard();
    
    //显示门，（金色的还是银色的）
    showDoorBtStart();
    createLightBGEffect();
    if (_huoArray != NULL) {
        createPiecesCard();
    }
}
void SGVisitShowCardLayer::createLightBGEffect()//创建旋转光效
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int noffset = 100;
    m_plightCheng = CCSprite::createWithSpriteFrameName("show_visit_cheng.png");//旋转的光效底图
	m_plightCheng->setPosition(ccp(winSize.width / 2, winSize.height / 2+noffset));
    m_plightCheng->setScale(2);
	m_plightCheng->setVisible(false);
    CCRotateBy *bgRotate = CCRotateBy::create(6.0, 360);
	//永久旋转
	CCRepeatForever *rotateForever = CCRepeatForever::create(bgRotate);
	//创建播放队列
	m_plightCheng->runAction(rotateForever);
    
    m_plightZi = CCSprite::createWithSpriteFrameName("show_visit_zi.png");//旋转的光效底图
	m_plightZi->setPosition(ccp(winSize.width / 2, winSize.height / 2+noffset));
    m_plightZi->setScale(2);
	m_plightZi->setVisible(false);
    CCRotateBy *bgRotate1 = CCRotateBy::create(6.0, 360);
	//永久旋转
	CCRepeatForever *rotateForever1 = CCRepeatForever::create(bgRotate1);
	//创建播放队列
	m_plightZi->runAction(rotateForever1);
    
    m_plightLan = CCSprite::createWithSpriteFrameName("show_visit_lan.png");//旋转的光效底图
	m_plightLan->setPosition(ccp(winSize.width / 2, winSize.height / 2+noffset));
    m_plightLan->setScale(2);
	m_plightLan->setVisible(false);
    CCRotateBy *bgRotate2 = CCRotateBy::create(6.0, 360);
	//永久旋转
	CCRepeatForever *rotateForever2 = CCRepeatForever::create(bgRotate2);
	//创建播放队列
	m_plightLan->runAction(rotateForever2);
    
    m_plightLv = CCSprite::createWithSpriteFrameName("show_visit_lv.png");//旋转的光效底图
	m_plightLv->setPosition(ccp(winSize.width / 2, winSize.height / 2+noffset));
    m_plightLv->setScale(2);
	m_plightLv->setVisible(false);
    CCRotateBy *bgRotate3 = CCRotateBy::create(6.0, 360);
	//永久旋转
	CCRepeatForever *rotateForever3 = CCRepeatForever::create(bgRotate3);
	//创建播放队列
	m_plightLv->runAction(rotateForever3);
    this->addChild(m_plightLv);
    this->addChild(m_plightCheng);
    this->addChild(m_plightLan);
    this->addChild(m_plightZi);
}
void SGVisitShowCardLayer::setTheLightEffect(int nIndex)//设置旋转光效显隐
{
    //绿 蓝 紫 橙 对应转生次数 3 4 5 6
    ccColor3B color[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
    switch(nIndex)
    {
        case 5:
        {
             m_plightCheng->setVisible(false);
             m_plightLv->setVisible(false);
             m_plightLan->setVisible(false);
             m_plightZi->setVisible(false);
            visitCardName->setInsideColor(color[3]);
            visitDetailInfo->setInsideColor(color[3]);
        }
            break;
        case 4:
        {
            m_plightCheng->setVisible(false);
            m_plightLv->setVisible(false);
            m_plightLan->setVisible(false);
            m_plightZi->setVisible(false);
            visitCardName->setInsideColor(color[2]);
            visitDetailInfo->setInsideColor(color[2]);
        }
            break;
        case 3:
        {
            m_plightCheng->setVisible(false);
            m_plightLv->setVisible(false);
            m_plightLan->setVisible(false);
            m_plightZi->setVisible(false);
            visitCardName->setInsideColor(color[1]);
            visitDetailInfo->setInsideColor(color[1]);
        }
            break;
        case 2:
        {
            m_plightCheng->setVisible(false);
            m_plightLv->setVisible(false);
            m_plightLan->setVisible(false);
            m_plightZi->setVisible(false);
            visitCardName->setInsideColor(color[0]);
            visitDetailInfo->setInsideColor(color[0]);
        }
            break;
        default:
        {
            m_plightCheng->setVisible(false);
            m_plightLv->setVisible(false);
            m_plightLan->setVisible(false);
            m_plightZi->setVisible(false);
            visitCardName->setInsideColor(ccWHITE);
            visitDetailInfo->setInsideColor(ccWHITE);
        }
            break;
            
    }
}
void SGVisitShowCardLayer::setItemStars(int starNum,int maxstar,bool startAction)
{
    setTheLightEffect(starNum-1);
    if (visitItemstar &&visitItemstar->count()!=0) {
        for (int i = 0;i<visitItemstar->count();i++) {
           CCSprite*s = (CCSprite*)visitItemstar->objectAtIndex(i);
            this->removeChildByTag(s->getTag());
        }
        visitItemstar->removeAllObjects();
    }
    CCNode *lastStar = NULL;
    CCPoint starPos = ccp(winSize.width - CARDGAP*5,winSize.height*_moveUp - CARDGAP*8);
    //武将星星只有三个
    for (int i = 2;i>=0;i--) {
        CCSprite * star = CCSprite::createWithSpriteFrameName("xx.png");
        star->setPosition(starPos);
        addChild(star,1000,STARGROPTAG+i);
//        if (i>=starNum) {
//            star->setVisible(false);
//        }
        starPos.x -= star->getContentSize().width;
        visitItemstar->addObject(star);
        
    }
    
    
    
//    if (starNum > showStarNum)
//    {
//        lastStar = getChildByTag(STARGROPTAG + showStarNum -1);
//        if (lastStar) {
//            starPos = lastStar->getPosition();
//            starPos.x -= lastStar->getContentSize().width;
//        }
//        for (int i = showStarNum; i<starNum; i++) {
//            if (getChildByTag(STARGROPTAG +i)) {
//                visitItemstar = ( CCSprite *)getChildByTag(STARGROPTAG +i);
//                visitItemstar->setVisible(true);
//                
//            }
//            else
//            {
//                visitItemstar = CCSprite::createWithSpriteFrameName("star.png");
//                visitItemstar->setPosition(starPos);
//                addChild(visitItemstar,1000,STARGROPTAG+i);
//            }
//            starPos.x -= visitItemstar->getContentSize().width;
//        }
//    }
//    else if (starNum < showStarNum)
//    {
//        for (int i = starNum; i<showStarNum; i++) {
//            //removeChildByTag(STARGROPTAG + i);
//            lastStar = getChildByTag(STARGROPTAG+i);
//            lastStar->setVisible(false);
//            lastStar->stopAllActions();
//        }
//    }
//    else
//    {
//        //have same stars;
//    }
    
    
    showStarNum = starNum;
    
//    if (startAction)
//    {
//        for (int i = showStarNum -1; i>=0; i--) {
//            lastStar = getChildByTag(STARGROPTAG +i);
//            lastStar->stopAllActions();
//            lastStar->setScale(1);
//            lastStar->runAction(CCRepeatForever::create( CCSequence::create(CCDelayTime::create((showStarNum-i-1)*0.1),
//                                                                            CCScaleTo::create(0.2, 1.5),
//                                                                            CCScaleTo::create(0.2, 1.0),
//                                                                            CCDelayTime::create((showStarNum+i)*0.1+0.4+STARACTIONGAP),
//                                                                            NULL)) );
//        }
//    }
}

void SGVisitShowCardLayer::showDoorBtStart()
{
    //判断加载door
    const char *doorPng = NULL;
    if (useGoldDoor) {
        doorPng = "visitGoldDoor.png";
    }
    else
    {
        doorPng = "visitSilverDoor.png";
    }

    visitDoorBt = SGButton::create(doorPng,NULL,this,menu_selector(SGVisitShowCardLayer::visitDoorBtAction),ccp(0, 0),false,true);
    visitDoorBt->setScale(2.0f);
    visitDoorBt->setEnabled(false);
    visitDoorBt->setAnchorPoint(ccp(0.5,DOORANCOPONTY));
    visitDoorBt->ignoreAnchorPointForPosition(false);
    
    CCSprite *doorSprite1 = CCSprite::createWithSpriteFrameName(doorPng);
    CCSprite *doorSprite2 = CCSprite::createWithSpriteFrameName(doorPng);
    
    addBtn(visitDoorBt);
    addChild(doorSprite1,1,11);
    addChild(doorSprite2,2,22);
    
    CCPoint doorPos = ccp(winSize.width/2,winSize.height/2);
    visitDoorBt->setPosition(ccpAdd(doorPos, ccp(0, -80)));
    doorSprite1->setPosition(visitDoorBt->getPosition());
    doorSprite2->setPosition(visitDoorBt->getPosition());
    doorSprite1->setAnchorPoint(visitDoorBt->getAnchorPoint());
    doorSprite2->setAnchorPoint(visitDoorBt->getAnchorPoint());
    
    doorSprite1->setOpacity(0);
    doorSprite1->setScale(7.7);
    
    doorSprite2->setOpacity(0);
    doorSprite2->setScale(7.7);
    
    visitDoorBt->setOpacity(255*20);
    visitDoorBt->setScale(7.7);
    
    visitDoorBt->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME),
                                              CCScaleTo::create(SHOWCARDGAPTIME*3,1.62),
                                              CCScaleTo::create(SHOWCARDGAPTIME,2.36),
                                              CCScaleTo::create(SHOWCARDGAPTIME*2,2),
                                              CCDelayTime::create(SHOWCARDGAPTIME*2),
                                              CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::showDoorBtEnd)),NULL));
    visitDoorBt->runAction(CCFadeTo::create(SHOWCARDGAPTIME, 255));
    
    doorSprite1->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME),
                                              CCFadeTo::create(SHOWCARDGAPTIME, 255*0.2),
                                              CCScaleTo::create(SHOWCARDGAPTIME*3,1.62),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,2.36),
                                                 CCScaleTo::create(SHOWCARDGAPTIME*2,2),NULL));
    doorSprite1->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME*2),CCFadeTo::create(SHOWCARDGAPTIME*3, 255*0.6),NULL));
    
    
    doorSprite2->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME*2),
                                              CCFadeTo::create(SHOWCARDGAPTIME, 255*0.2),
                                              CCScaleTo::create(SHOWCARDGAPTIME*3,1.62),
                                              CCScaleTo::create(SHOWCARDGAPTIME,2.36),
                                              CCScaleTo::create(SHOWCARDGAPTIME*2,2),NULL));
    doorSprite2->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME*3),CCFadeTo::create(SHOWCARDGAPTIME*3, 255*0.4),NULL));
    
    ///////////////////////////////////////////////////////////////
    PAUSE_MUSIC;
    EFFECT_PLAY(MUSIC_51);
    
}
//探访（购买到的物品）
void SGVisitShowCardLayer::dispConsumableUpDoor()
{
    
    SGMainManager::shareMain()->addHeadIconPlistByNum(propBuyCard->getHeadSuffixNum(), sg_visitShowCard);
    
    //消耗品的头像
    CCSprite *prop = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png", propBuyCard->getHeadSuffixNum())->getCString());
    //边框
    CCSprite *border = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", propBuyCard->getCurrStar())->getCString());
    //提示信息
    SGCCLabelTTF *tipsInfo = SGCCLabelTTF::create(CCString::createWithFormat("成功購買 %s X %d 獲得贈送獎勵",propBuyCard->getOfficerName()->getCString(), propBuyCard->getCurNum())->getCString(), FONT_PANGWA, 28);
    
    
    prop->setTag(11);
    border->setTag(12);
    tipsInfo->setTag(13);
    
    //加入三个对应的消耗品图标， 这里就是买到的消耗品图标
    this->addChild(prop, 1);
    this->addChild(border, 2);
    this->addChild(tipsInfo, 2);
    
    
    //字在上方，物品的icon在下方
    prop->setPosition(ccpAdd(visitDoorBt->getPosition(), ccp(0, visitDoorBt->getContentSize().height + tipsInfo->getContentSize().height * 3)));
    //消耗品头像的位置
    tipsInfo->setPosition(ccpAdd(prop->getPosition(), ccp(0, -prop->getContentSize().height)));
    //边框的位置
    border->setPosition(ccpAdd(prop->getPosition(), ccp(0, 0)));
    
    
}

#pragma mark ==========================显示对应的门上部的东西===============================
void SGVisitShowCardLayer::showDoorBtEnd()
{
    //这里显示门完成，购买到的东西在这里显示，显示门上部上的东西
 
    //不是台湾的，才显示经验丹
    if (!SGMainManager::shareMain()->getCurrentChannelIsTW())
    {
        this->dispConsumableUpDoor();
    }
    removeChildByTag(11);
    removeChildByTag(22);
    visitDoorBt->setEnabled(true);
    visitDoorBt->setOpacity(255);
    //添加动画
    CCSpriterX *darrow = CCSpriterX::create("animationFile/Darrow.scml",true,true);
    darrow->setisloop(true);
    darrow->play();
    darrow->setanimaID(0);
    addChild(darrow,5,DARROWTAG);
    darrow->setPosition(ccp(visitDoorBt->getPositionX(),
                            visitDoorBt->getPositionY()+visitDoorBt->getContentSize().height * 2 *(1-visitDoorBt->getAnchorPoint().y)+20));
    
    //门框闪光
    CCSpriterX *doorFlash = CCSpriterX::create("animationFile/Doorflash.scml",true,true);
    doorFlash->setisloop(false);
    doorFlash->play();
    doorFlash->setanimaID(0);
    addChild(doorFlash,6);
    doorFlash->setPosition(ccp(visitDoorBt->getPositionX(),
                               visitDoorBt->getPositionY()+visitDoorBt->getContentSize().height * 2 *(visitDoorBt->getAnchorPoint().y - 0.56) + 20));
}

void SGVisitShowCardLayer::setDoorBtImg()
{
    //判断加载door
    const char *doorPng = NULL;
    if (useGoldDoor) {
        doorPng = "visitGoldOpen.png";
    }
    else
    {
        doorPng = "visitSilverOpen.png";
    }
    
    visitDoorBt->setVisible(true);
    visitDoorBt->setImage(doorPng);
    visitDoorBt->setScale(2.0f);
    visitDoorBt->setOpacity(100);
    
    float scaleTime = 1;
    if (getChildByTag(-33)) {
        scaleTime = ((CCSpriterX *) getChildByTag(-33))->getAnimaTimeLength(0, 0);
    }
    visitDoorBt->runAction(CCScaleTo::create(scaleTime,7.2));
    visitDoorBt->runAction(CCFadeOut::create(scaleTime));
    
    //虚影
    CCSprite *doorSprite1 = CCSprite::createWithSpriteFrameName(doorPng);
    CCSprite *doorSprite2 = CCSprite::createWithSpriteFrameName(doorPng);
    
    addChild(doorSprite1,1,-11);
    addChild(doorSprite2,2,-22);
    
    doorSprite1->setPosition(visitDoorBt->getPosition());
    doorSprite2->setPosition(visitDoorBt->getPosition());
    doorSprite1->setAnchorPoint(visitDoorBt->getAnchorPoint());
    doorSprite2->setAnchorPoint(visitDoorBt->getAnchorPoint());
    
    doorSprite1->setOpacity(100);
    doorSprite2->setOpacity(100);
    doorSprite1->runAction(CCSequence::create(CCDelayTime::create(scaleTime*0.25),CCScaleTo::create(scaleTime,4),NULL));
    doorSprite1->runAction(CCFadeOut::create(scaleTime));
    doorSprite2->runAction(CCSequence::create(CCDelayTime::create(scaleTime*0.5),CCScaleTo::create(scaleTime,4),NULL));
    doorSprite2->runAction(CCFadeOut::create(scaleTime));
    
}

void SGVisitShowCardLayer::visitDoorBtAction(CCObject* selectBt)
{
    
    //去掉显示买到的东西，门上面的东西
    
    if (this->getChildByTag(11))
    {
        this->removeChildByTag(11, true);
    }
    if (this->getChildByTag(12))
    {
        this->removeChildByTag(12, true);
    }
    if (this->getChildByTag(13))
    {
        this->removeChildByTag(13, true);
    }
    
    //隐藏箭头
    getChildByTag(DARROWTAG)->setVisible(false);
    visitDoorBt->setEnabled(false);
    visitDoorBt->runAction(CCSequence::create(CCScaleTo::create(SHOWCARDGAPTIME,2.1),
                                              CCScaleTo::create(SHOWCARDGAPTIME,2),
                                              CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::showVisitCardPre)),
                                              NULL));
    
    //添加点中效果
    CCSpriterX *clickEff = CCSpriterX::create("animationFile/Clickeff.scml",true,true);
    clickEff->setisloop(false);
    clickEff->play();
    clickEff->setanimaID(0);
    addChild(clickEff,10);
    clickEff->setPosition(ccp(visitDoorBt->getPositionX(), visitDoorBt->getPositionY()+visitDoorBt->getContentSize().height * 2*(visitDoorBt->getAnchorPoint().y - 0.56)));
}

void SGVisitShowCardLayer::showVisitCardPre()
{

   visitDoorBt->setVisible(false);
  //开门动画
    CCSpriterX *gateOpen = NULL;
    if (useGoldDoor) {
        //门打开金色
        gateOpen = CCSpriterX::create("animationFile/GoldenGate2.scml",true,CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::setDoorBtImg)),true);
    }
    else
    {
        //门打开银色
       gateOpen = CCSpriterX::create("animationFile/Silverdoo2.scml",true,CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::setDoorBtImg)),true); 
    }
 
  gateOpen->setisloop(false);
  gateOpen->play();
  gateOpen->setanimaID(0);
  addChild(gateOpen,5);
  gateOpen->setPosition(ccp(visitDoorBt->getPositionX(), visitDoorBt->getPositionY()+visitDoorBt->getContentSize().height * 2 *(visitDoorBt->getAnchorPoint().y - 0.56)));

  //旋门打开光效动画
  CCSpriterX *xuanGuang = CCSpriterX::create("animationFile/KMFG.scml",true,CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::showVisitCardStart)),true);
  xuanGuang->setisloop(false);
  xuanGuang->play();
  xuanGuang->setanimaID(0);
  addChild(xuanGuang,5,-33);
  xuanGuang->setPosition(ccp(winSize.width/2,winSize.height/2));
    
  EFFECT_PLAY(MUSIC_52);
    if (_array->count() > MIDPOSCOUNTTAG) {
        //十连抽
         EFFECT_PLAY(MUSIC_54);
    }
}


void SGVisitShowCardLayer::showVisitCardStart()
{
    removeBtn(visitDoorBt);
    removeChildByTag(-11);
    removeChildByTag(-22);
    visitDoorBt = NULL;
    
    char *spritData = new char[(int)winSize.width * (int)winSize.height *4];
    memset(spritData,0,(int)winSize.width * (int)winSize.height *4);
    int spriteDataSize = (int)winSize.width * (int)winSize.height*4;
    for (int j=0; j<spriteDataSize; j+=4) {
        spritData[j] = 255;
        spritData[j+1] = 255;
        spritData[j+2] = 255;
        spritData[j+3] = 255;
    }
    
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, (int)winSize.width, (int)winSize.height, winSize);
    
    CCSprite *whiteSprite = CCSprite::createWithTexture(pTexture);
    CCFadeIn::create(3);
    whiteSprite->runAction(CCFadeOut::create(SHOWCARDGAPTIME*12));
    whiteSprite->setPosition(CCPointZero);
    whiteSprite->setAnchorPoint(CCPointZero);
    this->addChild(whiteSprite);
    
    delete [] spritData;
    pTexture->autorelease();
    
    ////////////////////////////////////////////
    unschedule(schedule_selector(SGVisitShowCardLayer::dealShowVisitCard));
    schedule(schedule_selector(SGVisitShowCardLayer::dealShowVisitCard), SHOWCARDGAPTIME*4);
    showCardTag = 0;
    //添加动画 法阵旋转
    CCSpriterX *visitArray = CCSpriterX::create("animationFile/Array.scml",true,true);
    visitArray->setisloop(true);
    visitArray->play();
    visitArray->setanimaID(0);
    addChild(visitArray,-10);
    visitArray->setPosition(ccp(winSize.width/2, winSize.height*(_moveUp-0.28)));
    //法阵上升粒子
    CCSpriterX *lightSpot = CCSpriterX::create("animationFile/Lightspot.scml",true,true);
    lightSpot->setisloop(true);
    lightSpot->play();
    lightSpot->setanimaID(0);
    addChild(lightSpot,50);
    lightSpot->setPosition(ccp(winSize.width/2, winSize.height*(_moveUp-0.2)));
    
}

void SGVisitShowCardLayer::dealShowVisitCard()
{
    if(showCardTag >= _array->count())
    {
        unschedule(schedule_selector(SGVisitShowCardLayer::dealShowVisitCard));
        int num = _array->count();
        if (num>1) {
            SGButton *showCard = NULL;
            for (int i=0; i<num; i++) {
                showCard = (SGButton *)this->menu->getChildByTag(i);
                if (showCard) {
                    showCard->setEnabled(true);
                }
            }
        }
        showCardTag = _array->count()-1;
        //与layer一同释放，本UI瞬时内存会很高。
//        removeKmfgRes();
    }
    else
    {
        SGButton *showCard = (SGButton *)this->menu->getChildByTag(showCardTag);
        if (showCard) {
            showCard->setVisible(true);
        }
        
        if (showCardTag == _array->count() -1) {
            ok = SGButton::createFromLocal("box_btnbg.png",
                                            str_TongQueLayer_str37,
                                            this,
                                            menu_selector(SGVisitShowCardLayer::backHandler),
                                            ccp(0, 3),
                                            FONT_PANGWA,ccWHITE,32,
                                            false,
                                            true);
            addBtn(ok);
            ok->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp( ok->getContentSize().width, winSize.height*0.1)));
            
            buttonShare = SGButton::createFromLocal("box_btnbg.png",
                                                  str_TongQueLayer_str3701,
                                                  this,
                                                  menu_selector(SGVisitShowCardLayer::onClickShareButton),
                                                  ccp(0, 3),
                                                  FONT_PANGWA,ccWHITE,32,
                                                  false,
                                                  true);

            buttonShare->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp( -buttonShare->getContentSize().width, winSize.height*0.1)));
            if(!SGGuideManager::shareManager()->isGuide &&
               CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") >= UIG_MAX1 &&
               SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
            {
                buttonShare->setVisible(true);
                
            }
            else
            {
                buttonShare->setVisible(false);
                ok->setPosition(ccpAdd(ok->getPosition(), ccp(-ok->getContentSize().width,0)));
            }

            addBtn(buttonShare);
            if (_huoArray != NULL)
            {
                ok->setVisible(false);
                ok->setPosition(ccpAdd(ok->getPosition(), ccp(ok->getContentSize().width, 0)));
                buttonShare->setVisible(false);
                showPiecesCard();
            }
            getChildByTag(DARROWTAG)->setVisible(true);
            selectVisitCard(showCard);
            
            if (showCardTag == 0 && showCard) {
                //只有1张卡
                showCard->setVisible(false);
                getChildByTag(DARROWTAG)->setVisible(false);
                //////////////不添加卡牌闪光效果
                EFFECT_PLAY(MUSIC_53);
                showCardTag++;
                return;
            }
        }
        else
        {
            refreshShowInfo(showCardTag,false);
        }

        //卡牌闪光效果
        CCSpriterX *cardLight = NULL;
        if (showCard) {
            cardLight = CCSpriterX::create("animationFile/Cardlight.scml",true,true);
            cardLight->setisloop(false);
            cardLight->play();
            cardLight->setanimaID(0);
            addChild(cardLight,6);
            cardLight->setPosition(showCard->getPosition());
        
        
            if ((SGBaseMilitaryCard *)_array->objectAtIndex(showCardTag) == _comboCard) {
                CCSprite *songdeLable = CCSprite::createWithSpriteFrameName("visitSongFont.png");
                if (songdeLable) {
                    addChild(songdeLable);
                    songdeLable->setPosition(ccp(showCard->getPositionX(), showCard->getPositionY() - showCard->getContentSize().height));
                    createCardShadowAction(songdeLable,"visitSongFont.png",cardLight->getAnimaTimeLength(0, 0)*2.5);
                }
            }
            else if ((SGBaseMilitaryCard *)_array->objectAtIndex(showCardTag) == _progressCard)
            {
                CCSprite *songdeLable = CCSprite::createWithSpriteFrameName("visitManSongFont.png");
                if (songdeLable) {
                    addChild(songdeLable);
                    songdeLable->setPosition(ccp(showCard->getPositionX(), showCard->getPositionY() - showCard->getContentSize().height));
                    createCardShadowAction(songdeLable,"visitManSongFont.png",cardLight->getAnimaTimeLength(0, 0)*3);
                }
            }
        }
        if (showCardTag == 0) {
            //更新定时
            if (cardLight) {
                unschedule(schedule_selector(SGVisitShowCardLayer::dealShowVisitCard));
                schedule(schedule_selector(SGVisitShowCardLayer::dealShowVisitCard), cardLight->getAnimaTimeLength(0, 0)/2);
            }
            if (_array->count() <= MIDPOSCOUNTTAG) {
                //单张卡
                EFFECT_PLAY(MUSIC_53);
            }
        }
        //////////////
        showCardTag++;
    }
}
void SGVisitShowCardLayer::createCardShadowAction(CCSprite * mainCard,const char* pngName,float delaytime)
{
    cardPngSprite1 = CCSprite::createWithSpriteFrameName(pngName);
    cardPngSprite2 = CCSprite::createWithSpriteFrameName(pngName);
    cardPngSprite1->setPosition(mainCard->getPosition());
    cardPngSprite2->setPosition(mainCard->getPosition());
    addChild(cardPngSprite1);
    addChild(cardPngSprite2);
    
    cardPngSprite1->setOpacity(0);
    cardPngSprite1->setScale(2.95);
    cardPngSprite2->setOpacity(0);
    cardPngSprite2->setScale(2.95);
    mainCard->setOpacity(255*0.2);
    mainCard->setScale(2.95);
    
    mainCard->runAction(CCSequence::create(CCDelayTime::create(delaytime+SHOWCARDGAPTIME),
                                               CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                               CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                               CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                               CCScaleTo::create(SHOWCARDGAPTIME,1),
                                               CCDelayTime::create(SHOWCARDGAPTIME*2),
                                               NULL));
    mainCard->runAction(CCSequence::create(CCDelayTime::create(delaytime+SHOWCARDGAPTIME),CCFadeTo::create(SHOWCARDGAPTIME*2, 255),NULL));
    
    cardPngSprite1->runAction(CCSequence::create(CCDelayTime::create(delaytime+SHOWCARDGAPTIME),
                                                 CCFadeTo::create(SHOWCARDGAPTIME, 255*0.2),
                                                 CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1),NULL));
    cardPngSprite1->runAction(CCSequence::create(CCDelayTime::create(delaytime+SHOWCARDGAPTIME*2),CCFadeTo::create(SHOWCARDGAPTIME*2, 255*0.6),NULL));
    
    cardPngSprite2->runAction(CCSequence::create(CCDelayTime::create(delaytime+SHOWCARDGAPTIME*2),
                                                 CCFadeTo::create(SHOWCARDGAPTIME, 255*0.2),
                                                 CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1),NULL));
    cardPngSprite2->runAction(CCSequence::create(CCDelayTime::create(delaytime+SHOWCARDGAPTIME*3),CCFadeTo::create(SHOWCARDGAPTIME*3, 255*0.4),NULL));
}

void SGVisitShowCardLayer::movePiecesHaHa(float dt )
{
    CCObject *piece = NULL;
    SGButton *visitCardbg = NULL;
    m_MoveHaHaTime += dt;
    CCARRAY_FOREACH(la->getChildren(), piece)
    {
        visitCardbg = (SGButton *)piece;
        if (visitCardbg->getPositionX() < -visitCardbg->getContentSize().width*2) {
            visitCardbg->setPositionX(la->getContentSize().width + visitCardbg->getPositionX());
        }
        visitCardbg->setPositionX(visitCardbg->getPositionX() - movedis);
        
    }
    if (m_MoveHaHaTime < PIECEMOVESPEEDMIN )
    {
        movedis += 10;
    }
    else if (m_MoveHaHaTime < PIECEMOVESPEEDMAX)
    {
         movedis -= 5;

    }
    else
    {
        float hahaDis = setViewWid/2 -_visitPiece->getPositionX() - _visitPiece->getContentSize().width/2;
        if (hahaDis < 0)
        {
            CCARRAY_FOREACH(la->getChildren(), piece)
            {
                visitCardbg = (SGButton *)piece;
                if (visitCardbg->getPositionX() < 0) {
                    visitCardbg->setPositionX(la->getContentSize().width + visitCardbg->getPositionX());
                }
            }
            
            unschedule(schedule_selector(SGVisitShowCardLayer::movePiecesHaHa));
            la->runAction(CCSequence::create(CCMoveTo::create(-hahaDis / setViewWid * 0.8, ccp( setViewWid/2 -_visitPiece->getPositionX() ,la->getPositionY())),
                                                 CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::showPiecesName)),NULL));
        }
        //异常处理movedis < 0
    }
}

void SGVisitShowCardLayer::showPiecesName()
{
    CCString *itemName = SGStaticDataManager::shareStatic()->getPiecesDictById(_progressCard->getItemId())->getPiecesName();
    piecesName->setString(itemName->getCString());
    piecesName->setVisible(true);
    ok->setVisible(true);
    
    
    if(!SGGuideManager::shareManager()->isGuide && CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") >= UIG_MAX1 && SGPlayerInfo::sharePlayerInfo()->getShowvip())
    {
        buttonShare->setVisible(true);
        ok->setPosition(ccpAdd(ok->getPosition(), ccp(-ok->getContentSize().width,0)));
    }
    else
    {
        buttonShare->setVisible(false);
        ok->setPosition(ccpAdd(ok->getPosition(), ccp(-ok->getContentSize().width,0)));
    }
}

void SGVisitShowCardLayer::startMovePieces()
{
    m_MoveHaHaTime = 0;
    schedule(schedule_selector(SGVisitShowCardLayer::movePiecesHaHa), 0.06);
}

void SGVisitShowCardLayer::showPiecesCard()
{

    scrollview->setVisible(true);
    
    piecesLable->setVisible(true);
    givenPieceBg->setVisible(true);
    piecesLable->setOpacity(0);
    givenPieceBg->setOpacity(0);
    piecesLable->runAction(CCFadeIn::create(2));
    givenPieceBg->runAction(CCFadeIn::create(2));
    

    if (_array->count() > CARDLIMIT) {
        piecesCoverLeft->setVisible(true);
        piecesCoverRight->setVisible(true);
        finaleSprite->setVisible(true);
        finaleSprite->runAction(CCSequence::create(CCFadeOut::create(2),
                                                   CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::startMovePieces)),NULL));
    }
    else
    {
        startMovePieces();
    }
}

void SGVisitShowCardLayer::createPiecesCard()
{
    SGMainManager *mainManager = SGMainManager::shareMain();
    SGBaseMilitaryCard *visitCard = NULL;
    CCObject *obj = NULL;
    CCPoint cardStartPos = ccp(0,winSize.height*(_moveUp-0.64));
    CCString * str_kuang = NULL;
    SGButton *visitCardbg = NULL;
    SGMainManager * sgMianManager = SGMainManager::shareMain();
    
    int cardTag = 0;
    CCSprite *countryName = NULL;
    CCSprite *cardBg = NULL;
    
    /////////////////////////////////
    piecesLable = CCSprite::createWithSpriteFrameName("visitHuoShowLable.png");
    piecesLable->setPosition(ccp(winSize.width/2, winSize.height*(_moveUp-0.54)));
    addChild(piecesLable);
    
    //////////////////////
    la =  CCLayer::create();
    float wight = 0;
    SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
    CCARRAY_FOREACH(_huoArray, obj)
    {
        visitCard = (SGBaseMilitaryCard *)obj;
        CCString *str = CCString::create(visitCard->getClassName());
        sgMianManager->addHeadIconPlistByNum(visitCard->getHeadSuffixNum(),sg_visitShowCard);
        if (str->isEqual(CCString::create("SGOfficerCard")))
        {
            SGOfficerDataModel *temp = staticDataManager->getOfficerById(visitCard->getItemId());
            mainManager->addOfficerPng(temp->getIconId(),sg_visitShowCard);
            
			
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1) {
                starlvl-=1;
            }
            str_kuang = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
            
            countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",visitCard->getRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            
			//12.06
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   NULL, NULL,CCPointZero,false,true);
            if (temp->getAdvNum() > 0) {
                SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advNum->setInsideColor(ccGREEN);
                advNum->setAnchorPoint(ccp(1,1));
                advNum->ignoreAnchorPointForPosition(false);
                advNum->setPosition(ccp(visitCardbg->getContentSize().width,visitCardbg->getContentSize().height - advNum->getContentSize().height*0.1));
                visitCardbg->addChild(advNum,40,40);
            }
        }
        else if (str->isEqual(CCString::create("SGEquipCard")))
        {
            SGEquipmentDataModel *temp = staticDataManager->getEquipById(visitCard->getItemId());
            
            str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
            sgMianManager->addEquipPng(temp->getIconId(),sg_visitShowCard);
			
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   NULL, NULL,CCPointZero,false,true);
			
        }
        else if (str->isEqual(CCString::create("SGPiecesCard")))
        {
            SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(visitCard->getItemId());
    
            if (pieces->getCallingCardId() == -1)//如果是通用碎片
			{
                str_kuang = CCString::createWithFormat("common_pieces_border.png");
			}
			else if (pieces->getPiecesType() == 1)//装备碎片
			{
				str_kuang  = CCString::createWithFormat("equipPinzhi_%d_0.png",staticDataManager->getEquipById(pieces->getCallingCardId())->getEquipStarPinzhi());
			}
			else if (pieces->getPiecesType() == 0)//武将碎片
			{
				SGOfficerDataModel *temp = staticDataManager->getOfficerById(pieces->getCallingCardId());
				int starlvl = temp->getOfficerCurrStarLevel();
				if (starlvl>1) {
					starlvl-=1;
				}
				str_kuang  = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
			}
            

            int iconId = 0;
			if (pieces->getPiecesType() == 0)//武将
			{
				SGOfficerDataModel *of = staticDataManager->getOfficerById(pieces->getCallingCardId());
				iconId = of->getIconId();
			}
			else if (pieces->getPiecesType() == 1)//装备
			{
				SGEquipmentDataModel *eq = staticDataManager->getEquipById(pieces->getCallingCardId());
				iconId = eq->getIconId();
			}
            
            if (iconId < 0) {
                continue;
            }
            sgMianManager->addHeadIconPlistByNum(iconId,sg_visitShowCard);
            
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",iconId)->getCString(),str_kuang->getCString(),
										   NULL, NULL,CCPointZero,false,true);
            if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
			{
				CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
				mask->setPosition(ccp(visitCardbg->getContentSize().width / 2, visitCardbg->getContentSize().height / 2));
				visitCardbg->addChild(mask);
			}
        }
        else if (str->isEqual(CCString::create("SGConsumableCard")))
        {
            SGConsumableDataModel *temp = staticDataManager->getConsumableById(visitCard->getItemId());
            
            str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getConsumeStarLvl());
            sgMianManager->addHeadIconPlistByNum(temp->getIconId(), sg_visitShowCard);
			
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   NULL, NULL,CCPointZero,false,true);
        }
        
        else
        {
			//12.06
			SGPropsDataModel *temp = staticDataManager->getPropsById(visitCard->getItemId());
            str_kuang = CCString::createWithFormat("equipPinzhi_1_0.png");
            sgMianManager->addPropPng(temp->getIconId(),sg_visitShowCard);
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   NULL, NULL,CCPointZero,false,true);
        }
		visitCardbg->setTag(PIECESSTARTTAG + cardTag);
        cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
        cardBg->setPosition(ccp(visitCardbg->getContentSize().width/2, visitCardbg->getContentSize().height/2));
        visitCardbg->addChild(cardBg,-1,-1);
        if (countryName) {
            visitCardbg->removeChildByTag(30);
            countryName->setPosition(ccp(visitCardbg->getContentSize().width*0.28,visitCardbg->getContentSize().height));
            visitCardbg->addChild(countryName,30,30);
            countryName = NULL;
        }
        if (visitCard->getCanAddNum() > 1) {
            SGCCLabelTTF *pieceNum = SGCCLabelTTF::create(CCString::createWithFormat("X%d",visitCard->getCanAddNum())->getCString(), FONT_PANGWA,22);
            pieceNum->setPosition(ccp(visitCardbg->getContentSize().width*0.8, pieceNum->getContentSize().height/2));
            visitCardbg->addChild(pieceNum,10,10);
        }
    
        //enable
        visitCardbg->setEnabled(false);
        if (visitCard == _progressCard) {
            _visitPiece = visitCardbg;
        }
        //pos
        wight += visitCardbg->getContentSize().width + CARDGAP *2;
        cardStartPos.x += visitCardbg->getContentSize().width + CARDGAP *2;
        visitCardbg->setPosition(ccp(cardStartPos.x, visitCardbg->getContentSize().height/2 +4));
        la->addChild(visitCardbg);
    
        cardTag++;
    }
    
    float setPosY = (int)winSize.height*(_moveUp-0.6) - visitCardbg->getContentSize().height;
    setViewWid = visitCardbg->getContentSize().width*5+CARDGAP*8;
    float setViewheight = visitCardbg->getContentSize().height +20;
    
    scrollview = CCScrollView::create(CCSizeMake(setViewWid, setViewheight));
    scrollview->setPosition(ccp(winSize.width/2 , setPosY));
    scrollview->setAnchorPoint(ccp(0.5, 0));
    scrollview->ignoreAnchorPointForPosition(false);
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    scrollview->setTouchEnabled(false);
    this->addChild(scrollview,100,100);
    la->setContentSize(CCSizeMake(wight, visitCardbg->getContentSize().width + 4));
    scrollview->setContainer(la);
    
    
    piecesCoverLeft = CCSprite::createWithSpriteFrameName("visitHuoShowBian.png");
    piecesCoverLeft->setFlipX(true);
    piecesCoverLeft->setPosition(ccp(scrollview->getPosition().x - setViewWid/2 + piecesCoverLeft->getContentSize().width/2,
                                     (int)(scrollview->getPosition().y + piecesCoverLeft->getContentSize().height*0.2)));
    addChild(piecesCoverLeft,1000,1000);
    
    piecesCoverRight = CCSprite::createWithSpriteFrameName("visitHuoShowBian.png");
    piecesCoverRight->setPosition(ccp(scrollview->getPosition().x + setViewWid/2 - piecesCoverLeft->getContentSize().width/2,
                                      (int)(scrollview->getPosition().y + piecesCoverRight->getContentSize().height*0.2)));
    addChild(piecesCoverRight,1000,1000);
    
    givenPieceBg = CCSprite::createWithSpriteFrameName("visitHuoShowKuang.png");
    givenPieceBg->setPosition(ccp(winSize.width/2,(int)(setPosY + visitCardbg->getContentSize().height/2 + 4)));
    addChild(givenPieceBg,1000,1000);
    
    piecesName = SGCCLabelTTF::create(str_TongQueLayer_str38, FONT_PANGWA, FONTSIZE(14));
    piecesName->setPosition(ccp(winSize.width/2,  setPosY - 36));
    addChild(piecesName);
    
    
    char *spritData = new char[(int)setViewWid * (int)setViewheight *4];
    memset(spritData,0,(int)setViewWid * (int)setViewheight *4);
    int spriteDataSize = (int)setViewWid * (int)setViewheight*4;
    for (int j=0; j<spriteDataSize; j+=4) {
        spritData[j] = 0;
        spritData[j+1] = 0;
        spritData[j+2] = 0;
        spritData[j+3] = 255;
    }
    
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, (int)setViewWid, (int)setViewheight, CCSizeMake(setViewWid, setViewheight));
    
    finaleSprite = CCSprite::createWithTexture(pTexture);
    finaleSprite->setPosition(ccp(winSize.width/2 , setPosY));
    finaleSprite->setAnchorPoint(ccp(0.5, 0));
    finaleSprite->ignoreAnchorPointForPosition(false);
    this->addChild(finaleSprite,1000,1000);
    
    delete [] spritData;
    pTexture->autorelease();
    
    //////////////
    movedis = visitCardbg->getContentSize().height/8;
    finaleSprite->setVisible(false);
    piecesCoverLeft->setVisible(false);
    piecesCoverRight->setVisible(false);
    scrollview->setVisible(false);
    piecesLable->setVisible(false);
    givenPieceBg->setVisible(false);
    piecesName->setVisible(false);
}


//创建中间对应的大图的按钮，点击进入详情
void SGVisitShowCardLayer::createCardShowInfo()
{
    winSize = CCDirector::sharedDirector()->getWinSize();
    showStarNum = 0;
    visitCardName = SGCCLabelTTF::create(str_TongQueLayer_str39, FONT_PANGWA, FONTSIZE(20));
    visitCardName->setAnchorPoint(ccp(0,1));
    visitCardName->setPosition(ccp(CARDGAP*2, winSize.height*_moveUp - CARDGAP*6));
    addChild(visitCardName);
    visitCardName->setVisible(false);
    
    SGMainManager::shareMain()->addOfficerPng(1001,sg_visitShowCard);
    visitCardPng = SGButton::create(CCString::createWithFormat("officer_1004.png")->getCString(),NULL,this,menu_selector(SGVisitShowCardLayer::showinfoLayer),ccp(0, 0),false,true);
    visitCardPng->setPosition(ccp(winSize.width/2, winSize.height*(_moveUp - 0.2)));
    visitCardPng->setZOrder(100);
    addBtn(visitCardPng);
    visitCardPng->setTag(MENUGROPTAG);
    visitCardPng->setVisible(false);
    
    float rate = 1.7;
    int activityShift = 0;
    //ip4
    if (CCDirector::sharedDirector()->getWinSize().height == 960 && CCDirector::sharedDirector()->getWinSize().width != 540)
    {
        rate = 1.4;
        activityShift = 85;
    }
    else if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        rate = 1.7;
        activityShift = 100;
    }
    else if (CCDirector::sharedDirector()->getWinSize().height == 1024)
    {
        activityShift = 140;
    }
    else//默认按ip5
    {
        rate = 1.7;
        activityShift = 100;
    }
    
    //下方的显示获取的卡牌的信息
    visitDetailInfo = SGCCLabelTTF::create("", FONT_PANGWA, FONTSIZE(18));
    this->addChild(visitDetailInfo, 1);
    // 如果活动探访，文字直接向上移动
    if (_huoArray)
    {
        visitDetailInfo->setPosition(ccpAdd(visitCardPng->getPosition(), ccp(0, -visitCardPng->getContentSize().height * rate + activityShift)));
    }
    else
    {
        visitDetailInfo->setPosition(ccpAdd(visitCardPng->getPosition(), ccp(0, -visitCardPng->getContentSize().height * rate)));
    }
    
    //...清晰
    visitCardPng->setVisible(false);
    
    //武将碎裂可能的提示文本
    _crashNoticeLabel = SGCCLabelTTF::create("", FONT_PANGWA, 26);
    this->addChild(_crashNoticeLabel, 1);
    _crashNoticeLabel->setPosition(ccpAdd(visitCardPng->getPosition(), ccp(0, -visitCardPng->getContentSize().height * 1 )));
    
    
    addKmfgRes();
}

//所有的获取的卡牌， 由传入的array的放到成员的_array,再由这里的具体化如类卡牌
void SGVisitShowCardLayer::createVisitCard()
{
    SGMainManager *mainManager = SGMainManager::shareMain();
    SGBaseMilitaryCard *visitCard = NULL;
    CCObject *obj = NULL;
    CCPoint cardStartPos = CCPointZero;
    CCString * str_kuang = NULL;
    SGButton *visitCardbg = NULL;
    SGMainManager * sgMianManager = SGMainManager::shareMain();
    if (_array->count() <= MIDPOSCOUNTTAG) {
        cardStartPos = ccp(winSize.width/2-CARDGAP*15,winSize.height*_moveUp );
    }
    else
    {
        cardStartPos = ccp(-CARDGAP*4.36 + (winSize.width/640 -1)*(winSize.width-640)*2,winSize.height*_moveUp);
    }
    int cardTag = 0;
    CCSprite *countryName = NULL;
    CCSprite *cardBg = NULL;
    CCARRAY_FOREACH(_array, obj)
    {
        visitCard = (SGBaseMilitaryCard *)obj;
        CCString *str = CCString::create(visitCard->getClassName());
        sgMianManager->addHeadIconPlistByNum(visitCard->getHeadSuffixNum(),sg_visitShowCard);
        //武将
        if (str->isEqual(CCString::create("SGOfficerCard")))
        {
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(visitCard->getItemId());
            mainManager->addOfficerPng(temp->getIconId(),sg_visitShowCard);
            if (temp->getIconId() < 0) {
                continue;
            }
			
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1) {
                starlvl-=1;
            }
            str_kuang = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
            
            countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",visitCard->getRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            
			//12.06
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   this, menu_selector(SGVisitShowCardLayer::selectVisitCard),CCPointZero,false,true);
            if (temp->getAdvNum() > 0) {
                SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advNum->setInsideColor(ccGREEN);
                advNum->ignoreAnchorPointForPosition(false);
                advNum->setPosition(ccp(visitCardbg->getContentSize().width,visitCardbg->getContentSize().height - advNum->getContentSize().height*0.1));
                visitCardbg->addChild(advNum,40,40);
            }
        }
        //装备
        else if (str->isEqual(CCString::create("SGEquipCard")))
        {
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(visitCard->getItemId());
            
            str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
            sgMianManager->addEquipPng(temp->getIconId(),sg_visitShowCard);
			
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   this, menu_selector(SGVisitShowCardLayer::selectVisitCard),CCPointZero,false,true);
			
        }
        //碎片
        else if (str->isEqual(CCString::create("SGPiecesCard")))
        {
            SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
            SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(visitCard->getItemId());
            int starLv = pieces->getStarLevel();
            if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
            {
                starLv -= 1;
            }
            
            if (pieces->getCallingCardId() == -1)//如果是通用碎片
			{
                str_kuang = CCString::createWithFormat("common_pieces_border.png");
			}
			else if (pieces->getPiecesType() == 1)//装备碎片
			{
				str_kuang  = CCString::createWithFormat("equipPinzhi_%d_0.png", starLv);
			}
			else if (pieces->getPiecesType() == 0)//武将碎片
			{
				str_kuang  = CCString::createWithFormat("officerPinzhi_%d_0.png",starLv);
			}
            
            
            int iconId = 0;
			if (pieces->getPiecesType() == 0)//武将
			{
				SGOfficerDataModel *of = staticDataManager->getOfficerById(pieces->getCallingCardId());
				iconId = of->getIconId();
			}
			else if (pieces->getPiecesType() == 1)//装备
			{
				SGEquipmentDataModel *eq = staticDataManager->getEquipById(pieces->getCallingCardId());
				iconId = eq->getIconId();
			}
            
            if (iconId < 0) {
                continue;
            }
            sgMianManager->addHeadIconPlistByNum(iconId,sg_visitShowCard);
            
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",iconId)->getCString(),str_kuang->getCString(),
										   this, menu_selector(SGVisitShowCardLayer::selectVisitCard),CCPointZero,false,true);
            if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
			{
				CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
				mask->setPosition(ccp(visitCardbg->getContentSize().width / 2, visitCardbg->getContentSize().height / 2));
				visitCardbg->addChild(mask);
			}
			
        }
        //消耗品
        else if (str->isEqual(CCString::create("SGConsumableCard")))
        {
            //消耗品的对应的id获取数据
            SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(visitCard->getItemId());
            if (consumeModel)
            {
                int startLvl = consumeModel->getConsumeStarLvl();
                //一星与二星用的框是相同的
                if (startLvl > 1)
                {
                    startLvl -= 1;
                }
                int iconId = consumeModel->getIconId();
                 //如果图错误，直接略过
                if (iconId < 0)
                {
                    continue;
                }
                str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png", startLvl);
                
                visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png", iconId)->getCString(), str_kuang->getCString(), this, menu_selector(SGVisitShowCardLayer::selectVisitCard), CCPointZero, false, true);
                
            }
            
        }
        else if (str->isEqual(CCString::create("SGMaterialCard")))
        {
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(visitCard->getItemId());
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_visitShowCard);
            int starLvl = materialModel->getMaterialBorderColor();
            if (starLvl > 1)
            {
                starLvl -= 1;
            }
            str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl);
            
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",materialModel->getMaterialIconId())->getCString(),str_kuang->getCString(),
										   this, menu_selector(SGVisitShowCardLayer::selectVisitCard),CCPointZero,false,true);
            
        }
        else//道具（非消耗品）
        {
			//12.06
			SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(visitCard->getItemId());
            str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getPropsCurrStarLevel()-1);
            sgMianManager->addPropPng(temp->getIconId(),sg_visitShowCard);
			visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),str_kuang->getCString(),
										   this, menu_selector(SGVisitShowCardLayer::selectVisitCard),CCPointZero,false,true);
        }

        
		visitCardbg->setTag(cardTag);
        addBtn(visitCardbg);
        cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
        cardBg->setPosition(ccp(visitCardbg->getContentSize().width/2, visitCardbg->getContentSize().height/2));
        visitCardbg->addChild(cardBg,-1,-1);
        if (countryName) {
            visitCardbg->removeChildByTag(30);
            countryName->setPosition(ccp(visitCardbg->getContentSize().width*0.28,visitCardbg->getContentSize().height));
            visitCardbg->addChild(countryName,30,30);
            countryName = NULL;
        }
        
        //enable
        visitCardbg->setEnabled(false);
        visitCardbg->setVisible(false);
        //pos
        if (cardTag < CARDLIMIT) {
            cardStartPos.x += visitCardbg->getContentSize().width + CARDGAP;
        }
        else if (cardTag == CARDLIMIT) {
            
            if (_huoArray != NULL)
            {
                cardStartPos.y = winSize.height*(_moveUp - 0.5 + CARDMOVEUPRATE);
            }
            else
            {
              cardStartPos.y = winSize.height*(_moveUp-0.5);
            }
            
        }
        else
        {
            cardStartPos.x -= visitCardbg->getContentSize().width + CARDGAP;
        }
        visitCardbg->setPosition(cardStartPos);
        
        cardTag++;
    }
}
void SGVisitShowCardLayer::selectVisitCard(CCObject* selectBt)
{
    if (selectCardBt) {
        selectCardBt->setScale(1);
        selectCardBt->setZOrder(0);
    }
    if (selectBt) {
         selectCardBt = (SGButton *)selectBt;
    }
    if (selectCardBt == NULL) {
        return;
    }
    selectCardBt->setScale(1.14);
    selectCardBt->setZOrder(2);
	selectCardBt->setPosition(ccp(selectCardBt->getPositionX(), selectCardBt->getPositionY()));//11.22//修改遮挡
    refreshShowInfo(selectCardBt->getTag(),true);
    
    getChildByTag(DARROWTAG)->setPosition(ccp(selectCardBt->getPositionX(), selectCardBt->getPositionY()+ selectCardBt->getContentSize().height));
    if (showCardTag == 0) {
        EFFECT_PLAY(MUSIC_56);
    }
}



void SGVisitShowCardLayer::refreshShowInfo(int cardTag,bool isLastEff)
{
    if (visitCardPng->getChildByTag(111))
    {
        visitCardPng->removeChildByTag(111, true);
    }
    if (visitCardPng->getChildByTag(110))
    {
        visitCardPng->removeChildByTag(110, true);
    }
    
    bool isPiecesCard = false;
    //Name
    if (cardTag >= _array->count())
    {
        return;
    }

    visitCardName->setVisible(true);
    SGBaseMilitaryCard *visitCard = (SGBaseMilitaryCard *)_array->objectAtIndex(cardTag);
    visitCardName->setString(visitCard->getOfficerName()->getCString());
    visitCardPng->setVisible(true);
    visitCardPng->setEnabled(false);
    
     //边框
    CCSprite *border = NULL;
    CCSprite *mask = NULL;
    
    //卡牌的数量
    int cardNum = 1;
    //Png
    if (visitCard->getHeadSuffixNum() < 0) {
        return;
    }
    CCString *str = CCString::create(visitCard->getClassName());
    CCString *strPng = NULL;
    //是否显示超大虚化背景
    bool isIllusive = false;
    //特别针对碎裂的武将设计，如果武将碎裂，则需要提示玩家。
    int showCrashNum = 0;
    if (str->isEqual(CCString::create("SGOfficerCard")))
    {
        SGMainManager::shareMain()->addOfficerPng(visitCard->getHeadSuffixNum(), sg_visitShowCard);
        strPng = CCString::createWithFormat("officer_%d.png",visitCard->getHeadSuffixNum());
        isIllusive = true;
        //判断是否需要显示碎裂提示。
        SGOfficerCard* officer = dynamic_cast<SGOfficerCard*>(visitCard);
        CCAssert(officer != NULL, "how could it even be possible?!");
        showCrashNum = officer->getCrashNum();
        setItemStars(officer->getCurrStar());

    }
    else if (str->isEqual(CCString::create("SGEquipCard")))
    {
        SGMainManager::shareMain()->addEquipPng(visitCard->getHeadSuffixNum(), sg_visitShowCard);
        strPng = CCString::createWithFormat("equip%d.png",visitCard->getHeadSuffixNum());
        setItemStars(visitCard->getCurrStar());
        isIllusive = true;
    }
    else if (str->isEqual(CCString::create("SGPiecesCard")))
    {
        isPiecesCard = true;
        SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(visitCard->getItemId());

        if (pieces->getPiecesType() == 0)//武将碎片
        {
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
            SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(), sg_visitShowCard);
            strPng = CCString::createWithFormat("head%d.png",temp->getIconId());
            int starLvl = temp->getOfficerCurrStarLevel();
            if (starLvl > 1)
            {
                starLvl -= 1;
            }
            border = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png", starLvl)->getCString());
        }
        else if (pieces->getPiecesType() == 1)// 装备碎片
        {
            SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId());
            SGMainManager::shareMain()->addHeadIconPlistByNum(equipModel->getIconId(), sg_visitShowCard);
            strPng = CCString::createWithFormat("head%d.png", equipModel->getIconId());
            int starLvl = equipModel->getEquipStarPinzhi();
            border = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString());
        }
        ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_visitShowCard);
        mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");



        
        //下方显示的信息中的数量
        cardNum = static_cast<SGPiecesCard *>(visitCard)->getPiecesCurNum();
    }
    else if (str->isEqual(CCString::create("SGConsumableCard")))
    {
        SGMainManager::shareMain()->addPropPng(visitCard->getHeadSuffixNum(), sg_visitShowCard);
        strPng = CCString::createWithFormat("prop%d.png", visitCard->getHeadSuffixNum());
        cardNum = static_cast<SGConsumableCard *>(visitCard)->getCurrNum();
    }
    else if (str->isEqual(CCString::create("SGMaterialCard")))
    {
        SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(visitCard->getItemId());
        
        SGMainManager::shareMain()->addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_visitShowCard);
        strPng = CCString::createWithFormat("head%d.png", materialModel->getMaterialIconId());
        int starLvl = materialModel->getMaterialBorderColor();
        if (starLvl > 1)
        {
            starLvl -= 1;
        }
        border = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString());
        cardNum = dynamic_cast<SGMaterialCard *>(visitCard)->getMaterialCurrNum();
        setItemStars(starLvl+1);
    }
    else
    {
        SGMainManager::shareMain()->addPropPng(visitCard->getHeadSuffixNum(), sg_visitShowCard);
        strPng = CCString::createWithFormat("prop%d.png",visitCard->getHeadSuffixNum());
        cardNum = static_cast<SGPropsCard *>(visitCard)->getCurNum();
    }
    
    //武将碎裂的提示文本。
    if (_crashNoticeLabel)
    {
        char buffer[128];
        if (showCrashNum > 0)
        {
            sprintf(buffer, str_adv_eff_3, showCrashNum);
            _crashNoticeLabel->setVisible(true);
            _crashNoticeLabel->setString(buffer);
        }
        else
        {
            _crashNoticeLabel->setVisible(false);
        }
    }
    
    //中间大图下面显示对应的信息 如（获得某某 XXX个）
    if (visitDetailInfo)
    {
        visitDetailInfo->setVisible(true);
        visitDetailInfo->setString(CCString::createWithFormat(str_TongQueLayer_str43, visitCard->getOfficerName()->getCString(), cardNum)->getCString());
    }
    
    //移除对应的边框和蒙版?
    visitCardPng->setImageWithFrame(strPng->getCString());
    
    if (border && isPiecesCard)
    {
        visitCardPng->addChild(border, 1, 110);
        border->setPosition(ccpAdd(ccp(visitCardPng->getContentSize().width / 2, visitCardPng->getContentSize().height / 2), ccp(0, 0)));
    }
    if (mask && isPiecesCard)
    {
        visitCardPng->addChild(mask, 2, 111);
        mask->setPosition(ccpAdd(ccp(visitCardPng->getContentSize().width / 2, visitCardPng->getContentSize().height / 2), ccp(0, 0)));
        
    }
    

//
    visitCardPng->setTag(MENUGROPTAG+cardTag);
    visitCardPng->stopAllActions();
    
    
    if (!isPiecesCard)
    {
        if (visitCardPng->getChildByTag(111))
        {
            visitCardPng->removeChildByTag(111, true);
        }
        if (visitCardPng->getChildByTag(110))
        {
            visitCardPng->removeChildByTag(110, true);
        }
    }
    
    
    if (isLastEff) {
        //虚影
        if (cardPngSprite1) {
            cardPngSprite1->removeFromParent();
            cardPngSprite1 = NULL;
        }
        if (cardPngSprite2) {
            cardPngSprite2->removeFromParent();
            cardPngSprite2 = NULL;
        }
        cardPngSprite1 = CCSprite::createWithSpriteFrameName(strPng->getCString());
        cardPngSprite2 = CCSprite::createWithSpriteFrameName(strPng->getCString());
        cardPngSprite1->setPosition(visitCardPng->getPosition());
        cardPngSprite2->setPosition(visitCardPng->getPosition());
        addChild(cardPngSprite1);
        addChild(cardPngSprite2);
        
        cardPngSprite1->setOpacity(0);
        cardPngSprite1->setScale(2.95);
        cardPngSprite2->setOpacity(0);
        cardPngSprite2->setScale(2.95);
        visitCardPng->setOpacity(255*0.2);
        visitCardPng->setScale(2.95);
        
        visitCardPng->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME),
                                                  CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1),
                                                  CCDelayTime::create(SHOWCARDGAPTIME*2),
                                                  CCCallFunc::create(this, callfunc_selector(SGVisitShowCardLayer::refreshLastInfoEnd)),NULL));
        visitCardPng->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME),CCFadeTo::create(SHOWCARDGAPTIME*2, 255),NULL));
        
        cardPngSprite1->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME),
                                                  CCFadeTo::create(SHOWCARDGAPTIME, 255*0.2),
                                                  CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1),NULL));
        cardPngSprite1->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME*2),CCFadeTo::create(SHOWCARDGAPTIME*2, 255*0.6),NULL));
        
        cardPngSprite2->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME*2),
                                                  CCFadeTo::create(SHOWCARDGAPTIME, 255*0.2),
                                                  CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                                 CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                                  CCScaleTo::create(SHOWCARDGAPTIME,1),NULL));
        cardPngSprite2->runAction(CCSequence::create(CCDelayTime::create(SHOWCARDGAPTIME*3),CCFadeTo::create(SHOWCARDGAPTIME*3, 255*0.4),NULL));
        
        
    }
    else
    {
        visitCardPng->setOpacity(255*0.4);
        visitCardPng->setScale(1);
        visitCardPng->runAction(CCSequence::create(CCScaleTo::create(SHOWCARDGAPTIME,1.5),CCScaleTo::create(SHOWCARDGAPTIME,1),NULL));
        visitCardPng->runAction(CCSequence::create(CCFadeTo::create(SHOWCARDGAPTIME,255),CCFadeTo::create(SHOWCARDGAPTIME,255*0.75),NULL));
    }
    //只有单次探访，并且是武将和装备的时候，才显示超大虚化背景！
    if (_array->count() == 1 && getChildByTag(-1000) == NULL && isIllusive) {
        CCSprite *cardBgSprite = CCSprite::createWithSpriteFrameName(strPng->getCString());
        cardBgSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
        cardBgSprite->setScale(4);
        cardBgSprite->setTag(-1000);
        cardBgSprite->setOpacity(255*0.16);
        addChild(cardBgSprite);
    }
    //stars
    CCString *classstring = CCString::create(visitCard->getClassName());
    if (classstring->isEqual(CCString::create("SGPiecesCard")))
    {
     SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(visitCard->getItemId());
        if (pieces->getCallingCardId()!=0) {
            if (pieces->getPiecesType()==0) {
                SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
                setItemStars(of->getOfficerCurrStarLevel(),of->getUpgradestar(),isLastEff);
            }
        }
     
    }
    else if (classstring->isEqual(CCString::create("SGConsumableCard")) || classstring->isEqual(CCString::create("SGPropsCard")))
    {
        setItemStars(visitCard->getCurrStar());
    }
    
//    this->addChild(visitItemstar);
}
void SGVisitShowCardLayer::refreshLastInfoEnd()
{
    visitCardPng->setOpacity(255);
    visitCardPng->setEnabled(true);
    if (cardPngSprite1) {
        cardPngSprite1->removeFromParent();
        cardPngSprite1 = NULL;
    }
    if (cardPngSprite2) {
        cardPngSprite2->removeFromParent();
        cardPngSprite2 = NULL;
    }
    if (showCardTag == _array->count()-1) {
        //最后一张卡
        EFFECT_PLAY(MUSIC_55);
        showCardTag = 0;
        visitCardPng->setEnabled(true);
    }
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") < guide_tag_34) {
        visitCardPng->setEnabled(false);
    }
}


void SGVisitShowCardLayer::addKmfgRes()
{
    //修改为使用PLIST加载。
    ResourceManager* rm = ResourceManager::sharedInstance();
    CCString * plistName = NULL;

    for (int i=1; i<=16; i++)
    {
        plistName = CCString::createWithFormat("animationFile/KMFG_%d.plist",i);
        rm->bindTexture(plistName->getCString(), RES_TYPE_LAYER_UI, sg_visitShowCard);
    }
}

void SGVisitShowCardLayer::removeKmfgRes()
{
    //修改为随layer释放，留档。
//    CCTextureCache *texTureCache = CCTextureCache::sharedTextureCache();
//    CCSpriteFrameCache *frameCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
//    CCString * texName = NULL;
//    CCString * texName2 = NULL;
//    for (int i=1; i<=16; i++) {
//        texName = CCString::createWithFormat("animationFile/KMFG_%d.png",i);
//        texName2 = CCString::createWithFormat("KMFG_%d.png",i);
//        texTureCache->removeTextureForKey(texName->getCString());
//        frameCache->removeSpriteFrameByName(texName2->getCString());
//    }
}

void SGVisitShowCardLayer::onClickShareButton()
{
    
    SGShareBox *box = SGShareBox::create(this,SGShareTypeVisit);
    SGMainManager::shareMain()->showBox(box);

}

void SGVisitShowCardLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    /////kanata
    //播放场景音乐
    MUSIC_PLAY(MUSIC_SCENE);
    //RESUME_MUSIC;
    ////
//    SGVisitLayer *layer =   (SGVisitLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_visitLayer);
//    SGMainManager::shareMain()->showLayer(layer);
    main::LotteryEntryRequest *request = new main::LotteryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VISIT_ENTER, request);
    SGBaseLayer *mlayer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    mlayer->setIsCanTouch(true);
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId == guide_tag_32) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_33);
    }
}


#pragma mark ========================单击中间大图，进入详情====================================

void SGVisitShowCardLayer::showinfoLayer(CCObject * selectPng)
{
    SGButton *pngBt = (SGButton*)selectPng;
    int selectCardTag = pngBt->getTag() - MENUGROPTAG;
    
    SGBaseLayer *mlayer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    mlayer->setIsCanTouch(true);
    
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_array->objectAtIndex(selectCardTag);
    CCString *str_ = CCString::create(card->getClassName());
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)card, 10002,NULL,false);//显示装备栏
    }
    else if(str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)card, 101,false);
    }
    else //其他四大走CIB
    {
        SGMainManager::shareMain()->showCommonInfoBoxInProto(card, this);
    }
    //以下老代码勿删，需要enterType的备案。
//    else if (str_->isEqual(CCString::create("SGPiecesCard")))
//    {
//        SGMainManager::shareMain()->openChatShowLayerByType(card,5,sg_visitShowCard);
//    }
//    else if (str_->isEqual(CCString::create("SGConsumableCard")))
//    {
//        SGMainManager::shareMain()->showConsumableInfoLayer(static_cast<SGConsumableCard *>(card), false, 111);
//    }
//    else
//    {
//        SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)card, 101,false);
//    }
}



