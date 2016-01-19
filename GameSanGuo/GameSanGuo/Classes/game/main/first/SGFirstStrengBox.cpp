//
//  SGFirstStrengBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-18.
//
//

#include "SGFirstStrengBox.h"
#include "SGCCLabelTTF.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGStrengSoldierLayer.h"
#include "SGFirstLayer.h"
#include "SGMainScene.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGSoldiersLayer.h"

SGFirstStrengBox::SGFirstStrengBox()
{
    
}
SGFirstStrengBox::~SGFirstStrengBox()
{
         ResourceManager::sharedInstance()->unBindLayerTexture(sg_firstStrengBox);
}
SGFirstStrengBox *SGFirstStrengBox::create(SGBoxDelegate *bdg)
{
    SGFirstStrengBox *Box = new SGFirstStrengBox();
    if (Box && Box->init(bdg))
    {
        Box->autorelease();
        return Box;
    }
    CC_SAFE_DELETE(Box);
    return NULL;
}
bool SGFirstStrengBox::init(SGBoxDelegate *bdg)
{
    
     
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_firstStrengBox, LIM_PNG_AS_PNG);
    
    if (!SGBaseBox::init(bdg,box_firstStrengBox,CCSizeMake(600, 770),true,true))
    {
        return false;
    }
    CCPoint center = SGLayout::getPoint(kMiddleCenter);

    SGCCLabelTTF* title = SGCCLabelTTF::create(str_FirstLayer_str19, FONT_PANGWA, 40 , ccc3(143, 87, 0));
    this->addChild(title);
    title->setPosition(ccpAdd(biao->getPosition(),ccp(0,-5)));
    fontbg->setPreferredSize(CCSizeMake(600-55, 750-150));
    fontbg->setPosition(ccpAdd(center,ccp(0,0)));

    int soldiernum = 0;
    int wjjnum = 0;
    int zjjnum = 0;
    
    CCArray *officer = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    CCArray *equip = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
    CCArray *propCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject *obj = NULL;
    if (propCards) {
        CCARRAY_FOREACH(propCards, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card)
            {
                if (card->getType() == 3)
                {
                    soldiernum += 1;
                }
                else if (card->getType() == 4)//武将计
                {
                    wjjnum+= 1;
                }
                else if (card->getType() == 5)//主将计
                {
                    zjjnum+= 1;
                }
            }
        }

    }
    
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_13);
    SGButton *wjk = SGButton::createFromLocal("upwar_cell_bg.png", str_AllTaskLayer_str24, this, menu_selector(SGFirstStrengBox::strengOfficer),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    this->addBtn(wjk);
    wjk->setTag(modelTag);
    wjk->setScaleX(0.9);
    SGCCLabelTTF* label = SGCCLabelTTF::create((CCString::createWithFormat("（%d）",officer->count()))->getCString(), FONT_BOXINFO, 34);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(wjk->getPosition(), ccp(wjk->getContentSize().width*0.5+50*1.6, wjk->getContentSize().height*0.5)));
    wjk->addChild(label);
    
    SGButton *zbk = SGButton::createFromLocal("upwar_cell_bg.png", str_AllTaskLayer_str25, this, menu_selector(SGFirstStrengBox::strengEquipment),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    this->addBtn(zbk);
    zbk->setScaleX(0.9);
    SGCCLabelTTF* label2 = SGCCLabelTTF::create((CCString::createWithFormat("（%d）",equip->count()))->getCString(), FONT_BOXINFO, 34);
    label2->setAnchorPoint(ccp(0, 0.5));
    label2->setPosition(ccpAdd(zbk->getPosition(), ccp(zbk->getContentSize().width*0.5+50*1.6, zbk->getContentSize().height*0.5)));
    zbk->addChild(label2);
    
    SGButton *sbk = SGButton::createFromLocal("upwar_cell_bg.png", str_FirstLayer_str20, this, menu_selector(SGFirstStrengBox::strengSoldier),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    this->addBtn(sbk);
    sbk->setScaleX(0.9);
    SGCCLabelTTF* label3 = SGCCLabelTTF::create((CCString::createWithFormat("（%d）",soldiernum))->getCString(), FONT_BOXINFO, 34);
    label3->setAnchorPoint(ccp(0, 0.5));
    label3->setPosition(ccpAdd(sbk->getPosition(), ccp(sbk->getContentSize().width*0.5+50*1.6, sbk->getContentSize().height*0.5)));
    sbk->addChild(label3);
    
    int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitStrenghSoilder,3);
    sbk->setTag(limittag);
    
    SGButton *wjj = SGButton::createFromLocal("upwar_cell_bg.png", str_FirstLayer_str21, this, menu_selector(SGFirstStrengBox::strengSkill),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    this->addBtn(wjj);
    wjj->setScaleX(0.9);
    
    SGCCLabelTTF* label4 = SGCCLabelTTF::create((CCString::createWithFormat("（%d）",wjjnum))->getCString(), FONT_BOXINFO, 34);
    label4->setAnchorPoint(ccp(0, 0.5));
    label4->setPosition(ccpAdd(wjj->getPosition(), ccp(wjj->getContentSize().width*0.5+50*1.6, wjj->getContentSize().height*0.5)));
    wjj->addChild(label4);
    

    
    SGButton *zjj = SGButton::createFromLocal("upwar_cell_bg.png", str_FirstLayer_str22, this, menu_selector(SGFirstStrengBox::strengLord),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    this->addBtn(zjj);
    zjj->setScaleX(0.9);
    
    limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitStrenghWujianJi,3);
    zjj->setTag(limittag);
    
    SGCCLabelTTF* label5 = SGCCLabelTTF::create((CCString::createWithFormat("（%d）",zjjnum))->getCString(), FONT_BOXINFO, 34);
    label5->setAnchorPoint(ccp(0, 0.5));
    label5->setPosition(ccpAdd(zjj->getPosition(), ccp(zjj->getContentSize().width*0.5+50*1.6, zjj->getContentSize().height*0.5)));
    zjj->addChild(label5);
    
    wjk->setPosition(ccpAdd(center, ccp(0,230)));
    zbk->setPosition(ccpAdd(wjk->getPosition(), ccp(0,-wjk->getContentSize().height-10)));
    sbk->setPosition(ccpAdd(zbk->getPosition(), ccp(0, -wjk->getContentSize().height-10)));
    wjj->setPosition(ccpAdd(sbk->getPosition(), ccp(0,-wjk->getContentSize().height-10)));
    zjj->setPosition(ccpAdd(wjj->getPosition(), ccp(0, -wjk->getContentSize().height-10)));
    
    
    SGButton *backBtn = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, menu_selector(SGFirstStrengBox::boxClose),CCPointZero,false,true);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(center, ccp(0, -750/2+5 +35)));
    
    return true;
}

void SGFirstStrengBox::strengLord()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showGeneralsLayer(4);
}
void SGFirstStrengBox::strengEquipment()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showEquipsLayer(NULL,0,1);    
}
void SGFirstStrengBox::strengSoldier()
{
    EFFECT_PLAY(MUSIC_BTN);
//    this->boxClose();
//    SGMainManager::shareMain()->showSoldierslayer(1);
    this->boxClose();
    SGSoldiersLayer *layer = SGSoldiersLayer::create(6);
//    SGStrengSoldierLayer *layer = SGStrengSoldierLayer::create();
    SGMainManager::shareMain()->showLayer(layer);
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
}
void SGFirstStrengBox::strengOfficer()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showGeneralsLayer(2);
}
void SGFirstStrengBox::strengSkill()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
//    SGMainManager::shareMain()->showChooseSkillLayer();
    SGMainManager::shareMain()->showGeneralsLayer(5);
}
void SGFirstStrengBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
//    if (deletage)
//    {
//        deletage->setIsCanTouch(true);
//        deletage->showBoxCall(data);
//    }
//    SGMainManager::shareMain()->showFirstLayer();
//    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
//    if (mainLayer) {
//        mainLayer->setIsCanTouch(true);
//    }
    SGMainScene* mainScene=SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(true);
    SGFirstLayer *firstLayer = (SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer);
    if (!firstLayer)
    {
        firstLayer = SGFirstLayer::create(NULL, sg_firstLayer);
    }
    
//    SGLandingReward *baseLayer = (SGLandingReward*)mainScene->getChildByTag(sg_landingRewardLayer);
//    
//    if (baseLayer)
//    {
        firstLayer->setIsCanTouch(true);
    //}
    
    mainScene->addShowLayer(firstLayer);
    this->removeFromParentAndCleanup(true);
    
//    
//    CCCallFunc *speedFun3 = CCCallFunc::create(this, callfunc_selector(SGFirstLayer::showMenu));
//    deletage -> showBoxCall(speedFun3);
}