//
//  SGConsumableInfoLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-18.
//
//

#include "SGConsumableInfoLayer.h"
//
//  SGPropInfoLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-19.
//
//

#include "SGPropInfoLayer.h"
#include "SGMainManager.h"
#include "cocos-ext.h"
#include "SGSellBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "itemmodule.pb.h"
#include "SGStaticDataManager.h"
#include "SGBattleScoreLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGConsumableInfoLayer::SGConsumableInfoLayer():
enterType(0),
_card(NULL),
btnwidth(0.0),
btnwid(0.0),
_currentPage(0)
{
	
}
SGConsumableInfoLayer::~SGConsumableInfoLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_comsumableInfoLayer);
}


SGConsumableInfoLayer *SGConsumableInfoLayer::create(SGConsumableCard *card_,int type,int currentPage)
{
    SGConsumableInfoLayer *consumeInfoLayer = new SGConsumableInfoLayer();
    if (consumeInfoLayer && consumeInfoLayer->init(NULL, sg_comsumableInfoLayer))
    {
        consumeInfoLayer->_currentPage=currentPage;
        consumeInfoLayer->enterType = type;
        consumeInfoLayer->_card = card_;
        consumeInfoLayer->_card->retain();
        consumeInfoLayer->initView();
        consumeInfoLayer->autorelease();
        return consumeInfoLayer;
    }
    CC_SAFE_DELETE(consumeInfoLayer);
    return NULL;
}
void SGConsumableInfoLayer::initView()
{
	
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist",RES_TYPE_LAYER_UI ,sg_comsumableInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist",RES_TYPE_LAYER_UI ,sg_comsumableInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist",RES_TYPE_LAYER_UI ,sg_comsumableInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist",RES_TYPE_LAYER_UI ,sg_comsumableInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist",RES_TYPE_LAYER_UI ,sg_comsumableInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist",RES_TYPE_LAYER_UI ,sg_comsumableInfoLayer);
	
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();
	
    //float frameheight = skewingY(155);
    float hgt = skewingY(244);
    if (s.height == 1136) {
        //frameheight = skewing(155);
        hgt = skewing(244);
    }
    
	
    //左火
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/qhhyA.scml", true, true);
    
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect, 10, 0);
    //右火
    CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/qhhyB.scml", true, true);
    fermEffect1->setCCSpriterXFlipX(true);
    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    addChild(fermEffect1, 10, 1);
    CCLOG("type%d",_card->getItemType());
	//    if (_card->getType()) {
	//        statements
	//    }
    SGMainManager::shareMain()->addPropPng(_card->getHeadSuffixNum(),sg_comsumableInfoLayer);
    CCString *str = CCString::createWithFormat("prop%d.png",_card->getHeadSuffixNum());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    this->addChild(item,2);
    item->setAnchorPoint(ccp(0.5, 0));
	
    if (s.height == 960) {
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
		
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));
		
	}
    else if (s.height == 1136) {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*1.13 - title_bg->getContentSize().height), 768, (s.height - hgt*1.13 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
		
        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
		
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -450)));
		
		
    }else
    {
        CCSprite *a = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        this->addChild(a,-100);
        a->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - a->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width*.41, a->getContentSize().height*.02 -a->getContentSize().height*.09)));
        fermEffect1->setPosition(ccpAdd(a->getPosition(), ccp(a->getContentSize().width*.41, a->getContentSize().height*.02-a->getContentSize().height*.09)));
        
        item->setPosition(ccpAdd(a->getPosition(), ccp(0, -a->getContentSize().height*.362)));
    }
    fermEffect->play();
    fermEffect1->play();
    menu->setZOrder(7);
	
    this->setstar(_card->getCurrStar());
    this->setTitle(_card->getOfficerName()->getCString());

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back,
                                                  this, menu_selector(SGConsumableInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    
    if(enterType == 101)
    {
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_comsumableInfoLayer);

        CCSprite *bg1 = CCSprite::create("storyBg.png");
        //CCRect r = CCRectMake(0, 1136/2 - (hgt*1.13)/2, s.width, hgt*1.13);
        
        //bg1->setTextureRect(r);
        bg1->setScaleY(s.height/2 / bg1->getContentSize().height);
        bg1->setScaleX(s.width / bg1->getContentSize().width);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
    }
    else
    {
		CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
		bg1->setAnchorPoint(ccp(0.5, 0));
		bg1->setScaleX(s.width/bg1->getContentSize().width);
		bg1->setScaleY((hgt*1.13-btmheight*.68)/bg1->getContentSize().height);
		bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
		this->addChild(bg1,5);
    }
    
    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setPosition(ccp(s.width/2, hgt*1.13));
    jinbian->setScaleX(s.width/jinbian->getContentSize().width);
    this->addChild(jinbian,6);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 320));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, -frame2->getContentSize().height/2 - 30)));
	
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(320/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2l,6);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2r,6);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang2m,6);
    guang2m->setScaleX(468/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(284+10, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-284-10, 0)));
    
    CCScale9Sprite *fontbg = CCScale9Sprite::createWithSpriteFrameName("box_fontbg.png");
    fontbg->setPreferredSize(CCSizeMake(538, 205));
    this->addChild(fontbg,6);
    fontbg->setPosition(ccpAdd(frame2->getPosition(), ccp(0, 26)));

    
    SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(_card->getItemId());
    SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getConsumeDesc()->getCString(), FONT_BOXINFO, 28, CCSizeMake(504,400));
    a->setAnchorPoint(ccp(0.5f, 1));
    this->addChild(a,6);
	a->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.33)));
	
	//setCardType，“强化合成“
	SGConsumableDataModel *consumeData = SGStaticDataManager::shareStatic()->getConsumableById(_card->getItemId());
    this->setCardType(consumeData->getConsumeType());
	
}
void SGConsumableInfoLayer::setCardType(int type)
{
    SGCCLabelTTF *title = SGCCLabelTTF::create("", FONT_PANGWA, 30);
    title->setAnchorPoint(ccp(1, 0.5));
    switch (type) {
		case 0:
            title->setString(str_treasure_box);
            break;
        case 1:
            title->setString(str_The_key);
            break;
        case 2:
            title->setString(str_gift_bag);
            break;
        case 3:
            title->setString(str_tiliyaoshui);
            break;
        case 4:
            title->setString(str_junliangyaoshui);
            break;
        default:
            break;
    }
    this->addChild(title,99);
    title->setAnchorPoint(ccp(1, 0.5));
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(-skewing(10), -125)));
}




void SGConsumableInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
	switch (enterType) {
		case 0:
		{
			SGMainManager::shareMain()->showConsumeLayer();
		}
			break;
		case 12:
		{
		}
			break;
        case 10:
        {
            SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_creditCardLayer);
            SGMainManager::shareMain()->showLayer(layer);
        }
            break;
        case 25:
        {
            SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_totalLoginReward);
            SGMainManager::shareMain()->showLayer(layer);
        }
            break;
        case 26:
        {
            main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVER, request);
        }
            break;
        case CHATSHOWENTRYTAG:
        {
            SGMainManager::shareMain()->closeChatShowLayer();
        }
            break;
        case VISIT_ENTER:
        {
            SGVisitLayer *layer = (SGVisitLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_visitShowCard);
            if (layer) {
                SGMainManager::shareMain()->showLayer(layer);
            }
        }
            break;
		default:
			break;
	}
	
	
}

bool SGConsumableInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (enterType == 101) {
        backHandler();
    }
    return false;
}

void SGConsumableInfoLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGConsumableInfoLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}