//
//  SGSoldiersLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-9.
//
//

#include "SGSoldiersLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "ItemModule.pb.h"
#include "SGCardItem.h"
#include "SGSoldierItem.h"
#include "SGEquipCard.h"
#include "SGStaticDataManager.h"
#include "SGSkillInfoBox.h"
#include "SimpleAudioEngine.h"
#include  "SGFirstStrengBox.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "DayTaskModule.pb.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "AppMacros.h"

SGSoldiersLayer::SGSoldiersLayer()
:soldiers(NULL),
x_(0),
pageLabel(NULL)
{
    
}
SGSoldiersLayer::~SGSoldiersLayer()
{
 
    CC_SAFE_RELEASE(soldiers);
    CCLOG("~SGSoldiersLayer");
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_soldiersLayer);

}

SGSoldiersLayer* SGSoldiersLayer::create(int x)
{
    SGSoldiersLayer *soldierlayer = new SGSoldiersLayer();
    soldierlayer->x_=x;
    if (soldierlayer && soldierlayer->init(NULL, sg_soldiersLayer))
    {
        soldierlayer->initView();
        soldierlayer->autorelease();
        return soldierlayer;
    }
    CC_SAFE_DELETE(soldierlayer);
    return soldierlayer;
}
void SGSoldiersLayer::initMsg()
{

}

void SGSoldiersLayer::initView()
{
    
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_soldiersLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_soldiersLayer);
    //SGCardItem::addCardItemRes(sg_soldiersLayer);
    
    //this->initData();
       
    CCSize s = CCDirector::sharedDirector()->getWinSize();             
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();  

    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSoldiersLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.01);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));   

    CCSprite *font2 = CCSprite::createWithSpriteFrameName("font_yydsbk.png");
    font2->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(font2);
    

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_soldiersLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, b,bg->getContentSize().width,s.height -h - b - titleBg->getContentSize().height);
    bg->setTextureRect(r);

    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-3, b));
    
    
    this->setTouchEnabled(true);
    soldiers = CCArray::create();
    soldiers->retain();
    
    //下方信息提示
    int shift = 0;
    int distance = 0;
    if (CCDirector::sharedDirector()->getWinSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
    {
        shift = -40;
    }
    else if (CCDirector::sharedDirector()->getWinSize().width == iphonehdResource.size.width
             && CCDirector::sharedDirector()->getWinSize().height != 1136)
    {
        shift = -10;
        distance = 50;
    }
    

    
    SGCCLabelTTF *info1 = SGCCLabelTTF::create(str_soldier_intro1,  FONT_PANGWA, 21);
    SGCCLabelTTF *info2 = SGCCLabelTTF::create(str_soldier_intro2,  FONT_PANGWA, 21);
    SGCCLabelTTF *info3 = SGCCLabelTTF::create(str_soldier_intro3,  FONT_PANGWA, 21);
    
    info1->setAnchorPoint(ccp(0, 0.5));
    info2->setAnchorPoint(ccp(0, 0.5));
    info3->setAnchorPoint(ccp(0, 0.5));

    this->addChild(info1, 2);
    this->addChild(info2, 2);
    this->addChild(info3, 2);
    
    info1->setPosition(ccpAdd(ccp(s.width / 32, s.height / 2), ccp(0, -150 + shift)));
    info2->setPosition(ccpAdd(info1->getPosition(), ccp(0, -info1->getContentSize().height * 1.5)));
    info3->setPosition(ccpAdd(info2->getPosition(), ccp(0, -info2->getContentSize().height * 1.5)));

    
    //底部横条
    CCSprite *bottomBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(bottomBg);
    bottomBg->setAnchorPoint(ccp(0, 0));
    bottomBg->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, 166)));
    
    //横条上的label
    CCSprite *label = CCSprite::createWithSpriteFrameName("soldier_num.png");
    this->addChild(label, 3);
    label->setPosition(ccpAdd(bottomBg->getPosition(), ccp(bottomBg->getContentSize().width / 3, label->getContentSize().height / 1.4)));
    //兵魂卡数量
    
    int soulNum = 0;
    CCArray *soulCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject *obj_ = NULL;
    CCARRAY_FOREACH(soulCards, obj_)
    {
        
        SGPropsCard *card = (SGPropsCard *)obj_;
        if (card && card->getType() == 3 )
        {
            soulNum++;
        }
    }
    SGCCLabelTTF *cardNum = SGCCLabelTTF::create(CCString::createWithFormat("%d", soulNum)->getCString(), FONT_PANGWA, 28 , ccc3(228, 232, 12));
    this->addChild(cardNum,4);
    //228	232	12
    //modify by:zyc. merge into create.
    //cardNum->setColor(ccc3(228, 232, 12));
    cardNum->setPosition(ccpAdd(label->getPosition(), ccp(label->getContentSize().width / 2 + 50, 0)));
    for (int i = 0; i<3; i++)
    {
        SGSoldierItem *item = NULL;
        int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        switch (i) {
            case 0:
                item = SGSoldierItem::create(SGTeamgroup::shareTeamgroupInfo()->getsoldierblue(teamid),2);
                break;
            case 1:
                item = SGSoldierItem::create(SGTeamgroup::shareTeamgroupInfo()->getsoldierred(teamid),3);
                break;
            case 2:
                item = SGSoldierItem::create(SGTeamgroup::shareTeamgroupInfo()->getsoldiergreen(teamid),1);
                break;
                
            default:
                break;
        }
        if (i == 0)
        {
            this->addChild(item,0,MENUTAG);
        }
        else
        {
            this->addChild(item);
        }
        
        soldiers->addObject(item);
        item->setPosition(ccp(s.width/32, backBtn->getPosition().y - 160 - (i * 180) + distance));
        if(x_==3&&soulNum>=15)
            item->setNoticeFlag(true);
        
    }

    
}

void SGSoldiersLayer::initData()
{
    
}



void SGSoldiersLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SGSoldiersLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	
	CCPoint point = convertTouchToNodeSpace(pTouch);
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        SGSoldierItem *item = (SGSoldierItem *)obj;
        
        CCRect faceRect = CCRectMake(item->getPosition().x - item->getContentSize().width * .5 - 50,
                                     item->getPosition().y - item->getContentSize().height * .5,
                                     item->getContentSize().width+50,
                                     item->getContentSize().height);
        if (faceRect.containsPoint(point))
        {
			CCLayerColor *bl = CCLayerColor::create(ccc4(0, 0, 0, 150), 100	,100);
			bl->setPosition(item->getPosition());
			bl->setAnchorPoint(ccp(0.5, 0.5));
			bl->ignoreAnchorPointForPosition(false);
			this->addChild(bl, 10, 321);
        }
    }
    return true;
}
void SGSoldiersLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = convertTouchToNodeSpace(pTouch);
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
		
        SGSoldierItem *item = (SGSoldierItem *)obj;
        this->removeChildByTag(321);
        CCRect faceRect = CCRectMake(item->getPosition().x - item->getContentSize().width * .5 - 50,
                                     item->getPosition().y - item->getContentSize().height * .5,
                                     item->getContentSize().width+50,
                                     item->getContentSize().height);
        if (faceRect.containsPoint(point))
        {
            CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
            CCObject *obj1 = NULL;
            CCARRAY_FOREACH(allSoldiers, obj1)
            {
                SGSoldierCard *card = (SGSoldierCard *)obj1;
                if (card->getItemId() == item->getCardId())
                {
                        SGMainManager::shareMain()->showSoldierInfoLayer(card,0);
                        break;
 
                }
            }
            break;
        }
    }
}
void SGSoldiersLayer::backHandler()
{
//    SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
//    SGMainManager::shareMain()->showBox(strengBox);
//    SGMainManager::shareMain()->showFirstLayer();//hehehe
    
    if(x_==5)
    {
        main::DayTaskListRequest *request = new main::DayTaskListRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_LIST, request);

    }
    else if (x_==3)//从布阵进入
    {
        int currteamtype = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        SGMainManager::shareMain()->showEmbattleLayer(currteamtype);
    }
    else if (x_ == 6)
    {
        SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
        SGMainManager::shareMain()->showBox(strengBox);
        //SGMainManager::shareMain()->showFirstLayer();//hehehe
    }
    else
    {
        SGMainManager::shareMain()->showBarrackslayer();
    }
}
void SGSoldiersLayer::strengHandler(SGSoldierCard *card)
{
//    SGSoldierDataModel *equip = SGStaticDataManager::shareStatic()->getSoliderById(card->getItemId());
    
    int type = SGStaticDataManager::shareStatic()->getStrengSoldierType(card->getItemId());
    
//    if (equip->getSoldierExpType()->isEqual(CCString::create("A"))) {
//        type = 0;
//    }
//    else if (equip->getSoldierExpType()->isEqual(CCString::create("B"))) {
//        type = 1;
//    }
//    else if (equip->getSoldierExpType()->isEqual(CCString::create("C"))) {
//        type = 2;
//    }
//    else if (equip->getSoldierExpType()->isEqual(CCString::create("D"))) {
//        type = 3;
//    }
    SGExpDataModel *exp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type,card->getCurrLevel());
    
    if (card->getCurrExp() == exp->getExpValue() && card->getMaxExp() == -1)
    {
        SGMainManager::shareMain()->showMessage(str_qianghua_tips1);}
    else
    {
        SGMainManager::shareMain()->showStrengLayer(card,3,true);
    }
}

void SGSoldiersLayer::setIsCanTouch(bool isTouch)
{
    isCanTouch = isTouch;
    menu->setTouchEnabled(isTouch);
    CCObject *obj;
    SGSoldierItem *item = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
       item = (SGSoldierItem *)obj;
       ((CCMenu *) item->getChildByTag(MENUTAG))->setTouchEnabled(isTouch);
    }
}

