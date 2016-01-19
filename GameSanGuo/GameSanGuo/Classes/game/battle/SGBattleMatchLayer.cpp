//
//  SGBattleMatching.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//

#include "SGBattleMatchLayer.h"
#include "SGProtocolToMsg.h"
#include "SGNotificationCenter.h"
#include "FightBaseModule.pb.h"
#include "SGBattleManager.h"
#include "SGMainManager.h"
#include "SGStringConfig.h"


SGBattleMatchLayer::SGBattleMatchLayer():sp(NULL)
,spa(NULL)
,actNum(0)
{
    
}

SGBattleMatchLayer::~SGBattleMatchLayer()
{
    CCLOG("~SGBattleMatchLayer()");
}

SGBattleMatchLayer *SGBattleMatchLayer::create(const char *fileName, LayerTag tag_)
{
    SGBattleMatchLayer *bm = new SGBattleMatchLayer();
    if (bm && bm->init(fileName, tag_))
    {
        bm->autorelease();
        return bm;
    }
    CC_SAFE_DELETE(bm);
    return NULL;
}
void SGBattleMatchLayer::initMsg()
{
    CCLabelTTF* label1 = CCLabelTTF::create(str_Match, "", FONTSIZE(40));
    CCMenuItemLabel* label1Item = CCMenuItemLabel::create(label1, this, menu_selector(SGBattleMatchLayer::requestMatch));

    CCLabelTTF* label2 = CCLabelTTF::create("PVE", "", FONTSIZE(40));
    CCMenuItemLabel* label2Item = CCMenuItemLabel::create(label2, this, menu_selector(SGBattleMatchLayer::requestPve));
    
    CCLabelTTF* label3 = CCLabelTTF::create(str_Test, "", FONTSIZE(40));
    CCMenuItemLabel* label3Item = CCMenuItemLabel::create(label3, this, menu_selector(SGBattleMatchLayer::showTest));
        
    CCMenuItemLabel* label1Item1 = CCMenuItemLabel::create(CCLabelTTF::create(str_Action_plus, "", FONTSIZE(14)), this, menu_selector(SGBattleMatchLayer::actJia));
    CCMenuItemLabel* label1Item2 = CCMenuItemLabel::create(CCLabelTTF::create(str_Action_sub, "", FONTSIZE(14)), this, menu_selector(SGBattleMatchLayer::actJian));
    
    
    CCMenu* menu = CCMenu::create(label1Item,label2Item, label3Item, label1Item1, label1Item2,NULL);

    this->addChild(menu);
    menu->setPosition(CCPointZero);
    
    label1Item->setPosition(ccp(screenwidth * 0.5, label1Item->getContentSize().height));
    label2Item->setPosition(ccpAdd(label1Item->getPosition(), ccp(0, 100)));
    label1Item1->setPosition(ccp(label1Item1->getContentSize().width, label1Item1->getContentSize().height));
    label1Item2->setPosition(ccp(label1Item2->getContentSize().width, label1Item2->getContentSize().height * 2));
    label3Item->setPosition(ccp(screenwidth * 0.5, screenheight * 0.3));
}

void SGBattleMatchLayer::actJia()
{
//    actNum++;
//    if (actNum > 8) {
//        actNum = 8;
//    }
//    sp->setanimaID(actNum);
//    sp->setisloop(true);
//    spa->setanimaID(actNum);
//    spa->setisloop(true);
}

void SGBattleMatchLayer::actJian()
{
//    actNum--;
//    if (actNum < 0) {
//        actNum = 0;
//    }
//    sp->setanimaID(actNum);
//    sp->setisloop(true);
//    spa->setanimaID(actNum);
//    spa->setisloop(true);
}

void SGBattleMatchLayer::requestMatch()
{
    SGBattleManager::sharedBattle()->matchRequest();
}

void SGBattleMatchLayer::requestPve()
{

}

void SGBattleMatchLayer::showTest()
{
//    SGHeroTestLayer *heroLayer = SGHeroTestLayer::create(ccc4(200, 200, 200, 255), screenwidth, screenheight);
//    SGMainManager::shareMain()->showTestLayer(heroLayer);
}



