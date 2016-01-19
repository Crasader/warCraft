//
//  SGFirstAdvanceBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-18.
//
//

#include "SGFirstAdvanceBox.h"
#include "SGCCLabelTTF.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGFirstAdvanceBox::SGFirstAdvanceBox()
{
    
}
SGFirstAdvanceBox::~SGFirstAdvanceBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_firstAdvanceBox);
   
}
SGFirstAdvanceBox *SGFirstAdvanceBox::create(SGBoxDelegate *bdg)
{
    SGFirstAdvanceBox *Box = new SGFirstAdvanceBox();
    if (Box && Box->init(bdg))
    {
        Box->autorelease();
        return Box;
    }
    CC_SAFE_DELETE(Box);
    return NULL;
}
bool SGFirstAdvanceBox::init(SGBoxDelegate *bdg)
{
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_firstAdvanceBox, LIM_PNG_AS_PNG);
    
    if (!SGBaseBox::init(bdg,box_firstAdvanceTag,CCSizeMake(600, 450),true,true))//,CCSizeMake(535, 125)
    {
        return false;
    }
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    fontbg->setPreferredSize(CCSizeMake(600-55,450-150));
    fontbg->setPosition(ccpAdd(center,ccp(0,0)));
    CCSprite *title = CCSprite::createWithSpriteFrameName("mainlayer_font_xzjj.png");
    this->addChild(title);
    title->setPosition(biao->getPosition());
    
    CCLabelTTF *label = CCLabelTTF::create(str_AllTaskLayer_str22, FONT_BOXINFO, 26);
    CCLabelTTF *label1 = CCLabelTTF::create(str_AllTaskLayer_str23, FONT_BOXINFO, 26);
    this->addChild(label);
    this->addChild(label1);
    label->setColor(ccc3(0x92,0x34,0x00));
    label1->setColor(ccc3(0x92,0x34,0x00));
//     label->setColor(ccWHITE);
//     label1->setColor(ccWHITE);
    label1->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccpAdd(center, ccp(0, -170)));
    label1->setPosition(ccpAdd(label->getPosition(), ccp(-label->getContentSize().width*.5f, -label->getContentSize().height*1.0)));
    
    SGButton *wjk = SGButton::createFromLocal("upwar_cell_bg.png", str_AllTaskLayer_str24, this, menu_selector(SGFirstAdvanceBox::advanceOfficer),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_21);
    wjk->setTag(modelTag);
    this->addBtn(wjk);
    wjk->setScaleX(0.9);
    SGButton *zbk = SGButton::createFromLocal("upwar_cell_bg.png", str_AllTaskLayer_str25, this, menu_selector(SGFirstAdvanceBox::advanceEquipment),ccp(0,3),FONT_PANGWA,ccWHITE,50,false,true);
    this->addBtn(zbk);
    zbk->setScaleX(0.9);
    wjk->setPosition(ccpAdd(center, ccp(0,wjk->getContentSize().height/2+20)));
    zbk->setPosition(ccpAdd(center, ccp(0,-wjk->getContentSize().height/2-20)));

    SGButton *backBtn = SGButton::create("box_btnbg.png", "public_font_quxiao.png", this, menu_selector(SGFirstAdvanceBox::boxClose),CCPointZero,false,true);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(center, ccp(0, -450/2+5 -40)));
    
    return true;
}
void SGFirstAdvanceBox::advanceEquipment()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showEquipsLayer(NULL,0,2);
}
void SGFirstAdvanceBox::advanceOfficer()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showGeneralsLayer(3);
}
void SGFirstAdvanceBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
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
    
    
//    if (deletage)
//    {
//        deletage->setIsCanTouch(true);
//        deletage->showBoxCall(data);
//    }
//    this->removeFromParentAndCleanup(true);
}