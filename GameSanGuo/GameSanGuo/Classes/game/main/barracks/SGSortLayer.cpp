//
//  SGSortLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-14.
//
//
#include "SGSortLayer.h"
#include "SGBaseMilitaryCard.h"
#include "SGOfficerCard.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
using namespace std;
static int compareStars(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getCurrStar() > ((SGBaseMilitaryCard*)p2)->getCurrStar();
}
static int compareLevel(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getCurrLevel() > ((SGBaseMilitaryCard*)p2)->getCurrLevel();
}
static int compareAtk(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getAtk() > ((SGBaseMilitaryCard*)p2)->getAtk();
}
static int compareDef(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getDef() > ((SGBaseMilitaryCard*)p2)->getDef();
}
static int compareSpeed(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getSpeed() > ((SGBaseMilitaryCard*)p2)->getSpeed();
}
static int compareMor(const CCObject* p1, const CCObject* p2)
{
    return ((SGBaseMilitaryCard*)p1)->getMorale() > ((SGBaseMilitaryCard*)p2)->getMorale();
}


SGSortLayer::SGSortLayer()
: _arrayCard(NULL)
{
    
}

SGSortLayer::~SGSortLayer()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_sortLayer);


////    _arrayCard->release();
}

SGSortLayer *SGSortLayer::create(CCArray *arrayCard ,SGSortLayerDelegate *callBack ,SGBoxDelegate *isCanTouch,CCPoint sortButtonCenter)
{
    SGSortLayer *sortLayer = new SGSortLayer();
    
    sortLayer->_arrayCard = arrayCard;
//    sortLayer->_arrayCard->retain();
    sortLayer->_delegateCallBack = callBack;
    sortLayer->_delegateCannotTouch = isCanTouch;
    sortLayer->_sortButtonCenter = sortButtonCenter;
    
    if (sortLayer && sortLayer->init(NULL, sg_sortLayer))
    {        
        sortLayer->initView();
        sortLayer->autorelease();
        return sortLayer;        
    }
    
    CC_SAFE_DELETE(sortLayer);
    return NULL;
    
}

void SGSortLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist",RES_TYPE_LAYER_UI ,sg_sortLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist",RES_TYPE_LAYER_UI ,sg_sortLayer, LIM_PNG_AS_PNG);
    
    _delegateCannotTouch->setIsCanTouch(false);
    
    //半透的背景
    CCSprite *spriteBlackBG = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    spriteBlackBG->setPosition(SGLayout::getPoint(kMiddleCenter));
    spriteBlackBG->setScaleX(SGLayout::getSkewing(320)/spriteBlackBG->getContentSize().width);
    spriteBlackBG->setScaleY(SGLayout::getSkewing(480)/spriteBlackBG->getContentSize().height);
    this->addChild(spriteBlackBG);
    
    SGButton *range = SGButton::create("redbtn_jiao.png",
                                       "public_range.png",
                                       this,
                                       menu_selector(SGSortLayer::buttonSortCancel),
                                       ccp(18, 0),
                                       true,
                                       true);
    range->setPosition(_sortButtonCenter);
    this->addBtn(range);
    
    CCSprite *spriteBG = CCSprite::createWithSpriteFrameName("barrack_sort_bg.png");
    spriteBG->setAnchorPoint(ccp(1,1));
    spriteBG->setPosition(ccpAdd(_sortButtonCenter, ccp(range->getContentSize().width/2, -range->getContentSize().height/2)));
    this->addChild(spriteBG);
    
    CCPoint pointBG = spriteBG->getPosition();
    CCSize sizeBG = spriteBG->getContentSize();

    //四条线
    CCSprite *spriteLine1 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    CCSprite *spriteLine2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    CCSprite *spriteLine3 = CCSprite::createWithSpriteFrameName("playerinfo_Lbian.png");
    CCSprite *spriteLine4 = CCSprite::createWithSpriteFrameName("playerinfo_Rbian.png");
    spriteLine1->setAnchorPoint(ccp(0.5, 1));
    spriteLine1->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, 0)));
    spriteLine1->setScaleX(sizeBG.width/spriteLine1->getContentSize().width);
    spriteLine2->setAnchorPoint(ccp(0.5, 0));
    spriteLine2->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -sizeBG.height)));
    spriteLine2->setScaleX(sizeBG.width/spriteLine2->getContentSize().width);
    spriteLine3->setAnchorPoint(ccp(0, 0.5));
    spriteLine3->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width, -sizeBG.height/2)));
    spriteLine3->setScaleY(sizeBG.height/spriteLine3->getContentSize().height);
    spriteLine4->setAnchorPoint(ccp(1, 0.5));
    spriteLine4->setPosition(ccpAdd(pointBG, ccp(0, -sizeBG.height/2)));
    spriteLine4->setScaleY(sizeBG.height/spriteLine4->getContentSize().height);
    this->addChild(spriteLine1);
    this->addChild(spriteLine2);
    this->addChild(spriteLine3);
    this->addChild(spriteLine4);
    
    //四个角
    CCSprite *spriteBG_leftAngle_1 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png");
    CCSprite *spriteBG_leftAngle_2 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png");
    CCSprite *spriteBG_rightAngle_1 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png");
    CCSprite *spriteBG_rightAngle_2 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png");
    spriteBG_leftAngle_1->setAnchorPoint(ccp(0, 0));
    spriteBG_leftAngle_1->setRotation(90);
    spriteBG_leftAngle_2->setAnchorPoint(ccp(0, 0));
    spriteBG_rightAngle_1->setAnchorPoint(ccp(0, 0));
    spriteBG_rightAngle_1->setRotation(180);
    spriteBG_rightAngle_2->setAnchorPoint(ccp(0, 0));
    spriteBG_rightAngle_2->setRotation(270);
    
    spriteBG_leftAngle_1->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width, 0)));
    spriteBG_leftAngle_2->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width, -sizeBG.height)));
    spriteBG_rightAngle_1->setPosition(pointBG);
    spriteBG_rightAngle_2->setPosition(ccpAdd(pointBG, ccp(0, -sizeBG.height)));    
    this->addChild(spriteBG_leftAngle_1);
    this->addChild(spriteBG_leftAngle_2);
    this->addChild(spriteBG_rightAngle_1);
    this->addChild(spriteBG_rightAngle_2);
    
    SGButton *buttonSort1 = SGButton::create("barrack_sort_buttonbg.png", "sort_1.png", this, menu_selector(SGSortLayer::buttonClickSort_1),CCPointZero,false,true);
    SGButton *buttonSort2 = SGButton::create("barrack_sort_buttonbg.png", "sort_4.png", this, menu_selector(SGSortLayer::buttonClickSort_2),CCPointZero,false,true);
    SGButton *buttonSort3 = SGButton::create("barrack_sort_buttonbg.png", "sort_2.png", this, menu_selector(SGSortLayer::buttonClickSort_3),CCPointZero,false,true);
    SGButton *buttonSort4 = SGButton::create("barrack_sort_buttonbg.png", "sort_3.png", this, menu_selector(SGSortLayer::buttonClickSort_4),CCPointZero,false,true);
    SGButton *buttonSort5 = SGButton::create("barrack_sort_buttonbg.png", "sort_5.png", this, menu_selector(SGSortLayer::buttonClickSort_5),CCPointZero,false,true);
    SGButton *buttonSort6 = SGButton::create("barrack_sort_buttonbg.png", "sort_6.png", this, menu_selector(SGSortLayer::buttonClickSort_6),CCPointZero,false,true);
    SGButton *buttonSort7 = SGButton::create("barrack_sort_buttonbg.png", "sort_7.png", this, menu_selector(SGSortLayer::buttonClickSort_7),CCPointZero,false,true);
    SGButton *buttonSort8 = SGButton::create("public_btn_1.png", "public_font_qixiao.png", this, menu_selector(SGSortLayer::buttonSortCancel),CCPointZero,false,true);
    buttonSort7->setVisible(false);
    
    buttonSort1->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6)));
    buttonSort2->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*2)));
    buttonSort3->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*3)));
    buttonSort4->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*4)));
    buttonSort5->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*5)));
    buttonSort6->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*6)));
    buttonSort7->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*7)));
    buttonSort8->setPosition(ccpAdd(pointBG, ccp(-sizeBG.width/2, -buttonSort1->getContentSize().height*1.6*8)));
    this->addBtn(buttonSort1);
    this->addBtn(buttonSort2);
    this->addBtn(buttonSort3);
    this->addBtn(buttonSort4);
    this->addBtn(buttonSort5);
    this->addBtn(buttonSort6);
    this->addBtn(buttonSort7);
    this->addBtn(buttonSort8);
    
}
SGSortLayer *SGSortLayer::sortCards(cocos2d::CCArray *_arrayCard, int type)
{
    int arrayCount = _arrayCard->count();
    
    if (_arrayCard &&arrayCount>0) {
        SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)_arrayCard->objectAtIndex(0);
        if(!strcmp(card->getClassName(),"SGEquipCard"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("equipsort", CCString::createWithFormat("%d",(int)type)->getCString());
        }
        if(!strcmp(card->getClassName(),"SGOfficerCard"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("officersort", CCString::createWithFormat("%d",(int)type)->getCString());
        }
		if (!strcmp(card->getClassName(), "SGConsumableCard"))
		{
			CCUserDefault::sharedUserDefault()->setStringForKey("consumablesort", CCString::createWithFormat("%d",(int)type)->getCString());
		}
    }
    
    
    for(int i=0;i<arrayCount-1;i++)
    {
        for(int m=i+1;m<arrayCount;m++)
        {            
            
            SGBaseMilitaryCard *cardSoldier = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(i);
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(m);
            
            int num1_atk = cardSoldier->getAtk();
            int num2_atk = card->getAtk();
            
            int num1_def = cardSoldier->getDef();
            int num2_def = card->getDef();
            
            int num1_round = cardSoldier->getRound();
            int num2_round = card->getRound();
            
            int num1_speed = cardSoldier->getSpeed();
            int num2_speed = card->getSpeed();
            
            int num1_morale = cardSoldier->getMorale();
            int num2_morale = card->getMorale();
            
            if(!strcmp(cardSoldier->getClassName(),"SGEquipCard"))
            {
                SGEquipCard *equip = (SGEquipCard*)cardSoldier;
                if (equip->getOfficerCardId()) {
                    break;
                }
                
            }
            if(!strcmp(cardSoldier->getClassName(),"SGOfficerCard"))
            {
                SGOfficerCard *officer = (SGOfficerCard*)cardSoldier;

                int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
                if ((SGTeamgroup::shareTeamgroupInfo()->getPositionId(id, officer))>0) {
                    break;
                }
                CCArray *array = officer->getOfficerEquips();
                if(array)
                {
                    for(int i=0;i<array->count();i++)
                    {
                        SGEquipCard *equip = (SGEquipCard*)array->objectAtIndex(i);
//                        num1_atk += equip->getAtk();
//                        num1_def += equip->getDef();
                        num1_round += equip->getRound();
//                        num1_speed += equip->getSpeed();
//                        num1_morale += equip->getMorale();
                        
                    }
                }
            }
            CCUserDefault::sharedUserDefault()->flush();
            
            if(!strcmp(card->getClassName(), "SGOfficerCard"))
            {
                SGOfficerCard *officer = (SGOfficerCard*)card;
                CCArray *array = officer->getOfficerEquips();
                if(array)
                {
                    for(int i=0;i<array->count();i++)
                    {
                        SGEquipCard *equip = (SGEquipCard*)array->objectAtIndex(i);
//                        num2_atk += equip->getAtk();
//                        num2_def += equip->getDef();
                        num2_round += equip->getRound();
//                        num2_speed += equip->getSpeed();
//                        num2_morale += equip->getMorale();
                    }
                    
                }
            }
            
            
            switch (type) {
                case 0:    //星级高
                {
                    
                    
//                    sort(_arrayCard->data->arr, _arrayCard->data->arr+_arrayCard->data->num,compareStars);
                    
                    if(cardSoldier->getCurrStar() < card->getCurrStar())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                    
                }
                    break;
                case 2: //攻击高
                {
                    
                    if(num1_atk < num2_atk)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
//                    sort(_arrayCard->data->arr, _arrayCard->data->arr+_arrayCard->data->num,compareAtk);
                    
                }
                    break;
                case 3: //防御高
                {
                    if(num1_def < num2_def)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
//                    sort(_arrayCard->data->arr, _arrayCard->data->arr+_arrayCard->data->num,compareDef);
                    
                    
                }
                    break;
                case sort_round: //蓄力快
                {
                    if(num1_round < num2_round)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
                    
                }
                    break;
                case 4: //速度高
                {
                    if(num1_speed < num2_speed)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
//                     sort(_arrayCard->data->arr, _arrayCard->data->arr+_arrayCard->data->num,compareSpeed);
                    
                    
                }
                    break;
                case 5: //血量高
                {
                    if(num1_morale < num2_morale)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
//                     sort(_arrayCard->data->arr, _arrayCard->data->arr+_arrayCard->data->num,compareMor);
                    
                    
                }
                    break;
                case 1: //等级高
                {
                    if(cardSoldier->getCurrLevel() < card->getCurrLevel())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }
//                     sort(_arrayCard->data->arr, _arrayCard->data->arr+_arrayCard->data->num,compareLevel);
                    
                    
                }
                    break;
                default:
                {
                    CCLOG("Default...");
                    break;
                }
            }
        }
    }
    return 0;
}


void SGSortLayer::sortCard(sortType type)
{
    /*
     sort_star,//星级高
     sort_ap,//攻击高
     sort_def,//防御高
     sort_round,//蓄力快
     sort_speed,//速度高
     sort_morale,//血量高
     sort_newgot//新获得
     */
    
    int arrayCount = _arrayCard->count();
    if(0 == arrayCount)
    {
        CCLOG("数组为空");
        return;
    }

    for(int i=0;i<arrayCount-1;i++)
    {
        for(int m=i+1;m<arrayCount;m++)
        {
            
            SGBaseMilitaryCard *cardSoldier = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(i);
            SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)_arrayCard->objectAtIndex(m);
            
            int num1_atk = cardSoldier->getAtk();
            int num2_atk = card->getAtk();
            
            int num1_def = cardSoldier->getDef();
            int num2_def = card->getDef();

            int num1_round = cardSoldier->getRound();
            int num2_round = card->getRound();
            
            int num1_speed = cardSoldier->getSpeed();
            int num2_speed = card->getSpeed();

            int num1_morale = cardSoldier->getMorale();
            int num2_morale = card->getMorale();
            
            if(!strcmp(cardSoldier->getClassName(),"SGEquipCard"))
            {
               CCUserDefault::sharedUserDefault()->setStringForKey("equipsort", CCString::createWithFormat("%d",(int)type)->getCString());
                SGEquipCard *equip = (SGEquipCard*)cardSoldier;
                if (equip->getOfficerCardId()) {
                    break;
                }

            }
            
            if(!strcmp(cardSoldier->getClassName(),"SGOfficerCard"))
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("officersort", CCString::createWithFormat("%d",(int)type)->getCString());
                
                SGOfficerCard *officer = (SGOfficerCard*)cardSoldier;

                int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
                if ((SGTeamgroup::shareTeamgroupInfo()->getPositionId(id,officer))>0) {
                    break;
                }
                CCArray *array = officer->getOfficerEquips();                
                if(array)
                {
                    for(int i=0;i<array->count();i++)
                    {
                        SGEquipCard *equip = (SGEquipCard*)array->objectAtIndex(i);
//                        num1_atk += equip->getAtk();
//                        num1_def += equip->getDef();
                        num1_round += equip->getRound();
//                        num1_speed += equip->getSpeed();
//                        num1_morale += equip->getMorale();
                        
                    }
                }
            }
            CCUserDefault::sharedUserDefault()->flush();
            if(!strcmp(card->getClassName(), "SGOfficerCard"))
            {
                SGOfficerCard *officer = (SGOfficerCard*)card;
                CCArray *array = officer->getOfficerEquips();
                if(array)
                {
                    for(int i=0;i<array->count();i++)
                    {
                        SGEquipCard *equip = (SGEquipCard*)array->objectAtIndex(i);
//                        num2_atk += equip->getAtk();
//                        num2_def += equip->getDef();
                        num2_round += equip->getRound();
//                        num2_speed += equip->getSpeed();
//                        num2_morale += equip->getMorale();
                    }

                }
            }
            
            
            switch (type) {
                case sort_star:    //星级高
                {
                    if(cardSoldier->getCurrStar() < card->getCurrStar())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }

                    break;
                }
                    
                case sort_ap: //攻击高
                {

                    if(num1_atk < num2_atk)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);                    
                    }
                    
                    break;
                }
                case sort_def: //防御高
                {
                    if(num1_def < num2_def)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }

                    break;
                }
                case sort_round: //蓄力快
                {
                    if(num1_round < num2_round)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }

                    break;
                }
                case sort_speed: //速度高
                {
                    if(num1_speed < num2_speed)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }

                    break;
                }
                case sort_morale: //血量高
                {
                    if(num1_morale < num2_morale)
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }

                    break;
                }
                case sort_level: //等级高
                {
                    if(cardSoldier->getCurrLevel() < card->getCurrLevel())
                    {
                        _arrayCard->exchangeObjectAtIndex(i, m);
                    }

                    break;
                }
                default:
                {
                    CCLOG("Default...");
                    break;
                }
            }
        }
    }
    
    //回调
    
}



void SGSortLayer::buttonClickSort_1()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,0);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonClickSort_2()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,1);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonClickSort_3()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,2);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonClickSort_4()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,3);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonClickSort_5()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,4);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonClickSort_6()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,5);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonClickSort_7()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->sortCards(_arrayCard,6);
    _delegateCallBack->sortLayerCallBack(_arrayCard);
    _delegateCannotTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGSortLayer::buttonSortCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCLOG("取消");
    this->removeFromParentAndCleanup(true);
    _delegateCannotTouch->setIsCanTouch(true);

}

void SGSortLayer::sortTypeHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->removeFromParentAndCleanup(true);
    _delegateCannotTouch->setIsCanTouch(true);
}
