//
//  CGPVisitResult.cpp
//  GameSanGuo
//
//  Created by caoguoping on 16/1/25.
//
//

#include "CGPVisitResultLayer.h"
#include "SGMainManager.h"
//#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"

#define maskVTag 1001



CGPVisitResultLayer::CGPVisitResultLayer()
{
    mOpenDoor = nullptr;

    
}

CGPVisitResultLayer::~CGPVisitResultLayer()
{
    SGMainManager::shareMain()->getTouchGroup()->removeChild(mOpenDoor, true);
}

CGPVisitResultLayer*  CGPVisitResultLayer::create(CCArray *array, int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage)
{
    CGPVisitResultLayer *Layer = new CGPVisitResultLayer();
    if (Layer && Layer->init(array  , type,  isOnlyOfficer))
    {
        Layer->initView(array);
        Layer->autorelease();

        
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

bool CGPVisitResultLayer::init(CCArray* array, int type,bool isOnlyOfficer)
{
    if (!SGBaseLayer::init(NULL, cgp_visitResult))
    {
        return false;
    }

    return true;
    
}





void CGPVisitResultLayer::initView(CCArray*  array)
{
    
    mOfficer =  (SGBaseMilitaryCard*)array->objectAtIndex(0);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    //notification->addObserver(MSG_VISIT_PURCH,
//                              this,
//                              callfuncO_selector(CGPVisitResultLayer::buyLegListener));
    
    if(mOfficer->getItemId() < 1001 || mOfficer->getItemId() >2122)
    {
        SGMainManager::shareMain()->showFirstLayer();
         return;
    }
    SGOfficerDataModel*  officerModel = SGStaticDataManager::shareStatic()->getOfficerById(mOfficer->getItemId());
   
    
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1756-1760.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1001-1007.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1008-1013.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1014-1016.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    
    ResourceManager::sharedInstance()->bindTexture("headIcon/head1701-1800.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    ResourceManager::sharedInstance()->bindTexture("headIcon/head1001-1100.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    
    
    setIsCocostudio(true);
    mPanelVisitResult  = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("cocosUi/visit.ExportJson"));
    CCLayerColor*   blackLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), 640, 1136);
    blackLayer->setPosition(ccp(0, 0));
    mPanelVisitResult->addNode(blackLayer, -1);
    SGMainManager::shareMain()->getMainScene()->mCocosLayers[cgp_visitResult - cocostudioLayerStart] = mPanelVisitResult;

    Button*    btnBack;
    UIGet_Button("Button_back", mPanelVisitResult, btnBack)
    UIClick(btnBack, CGPVisitResultLayer::btnCallBack)

    UIGet_ImageView("Image_back", mPanelVisitResult, mImageBack)
    UIGet_ImageView("Image_attr", mPanelVisitResult, mImageAttr)
    UIGet_ImageView("Image_type", mImageAttr, mImageAtk)
    UIGet_ImageView("Image_atk", mImageAttr, mImageAtk)
    UIGet_ImageView("Image_def", mImageAttr, mImageDef)
    UIGet_ImageView("Image_hp", mImageAttr, mImageHp)
    UIGet_ImageView("Image_speed", mImageAttr, mImageSpeed)
    
    Layout* spinePanel;
    UIGet_Layout("Panel_spine", mPanelVisitResult, spinePanel)
    UIGet_ImageView("Image_30", spinePanel, mImageHeroLight)
    
    ImageView*  imgSkillBg;
    UIGet_ImageView("Image_skillBg", mPanelVisitResult, imgSkillBg)
    UIGet_Button("Button_mainSkill", imgSkillBg, btnMainSkill)
    UIGet_Button("Button_passiveSkill", imgSkillBg, btnPassiveSkill)
    UIGet_Label("Label_mainSkill", btnMainSkill, mainSkillLabel)
    UIGet_Label("Label_passiveSkill", btnPassiveSkill, passiveSkillLabel)

    
    RT_Label(officerModel->getOfficerName()->getCString(), 26, ccWHITE, mHeroName, mImageBack)
    mHeroName->setPosition(ccp(-5, 15));
    
    
    
    mLabelAtk = SGCCLabelTTF::create(CCString::createWithFormat("%d", mOfficer->getAtk())->getCString(), FONT_PANGWA, 20, ccWHITE);
    mLabelDef = SGCCLabelTTF::create(CCString::createWithFormat("%d", mOfficer->getDef() )->getCString(), FONT_PANGWA, 20, ccWHITE);
    mLabelHp = SGCCLabelTTF::create(CCString::createWithFormat("%d", mOfficer->getMorale())->getCString(), FONT_PANGWA, 20, ccWHITE);
    mLabelSpeed = SGCCLabelTTF::create(CCString::createWithFormat("%d", (int)mOfficer->getSpeed())->getCString(), FONT_PANGWA, 20, ccWHITE);
    
    mLabelAtk->setScaleX(2.0f);
    mLabelDef->setScaleX(2.0f);
    mLabelHp->setScaleX(2.0f);
    mLabelSpeed->setScaleX(2.0f);
    
    mLabelAtk->setPositionX(30);
    mLabelDef->setPositionX(30);
    mLabelHp->setPositionX(30);
    mLabelSpeed->setPositionX(30);
    
    mImageAtk->addNode(mLabelAtk);
    mImageDef->addNode(mLabelDef);
    mImageHp->addNode(mLabelHp);
    mImageSpeed->addNode(mLabelSpeed);
    
    SGCCLabelTTF  *mainSkills = SGCCLabelTTF::create("主将计:", FONT_PANGWA, 30, ccShitYellow);
    SGCCLabelTTF  *passiveSkills = SGCCLabelTTF::create("被动计:", FONT_PANGWA, 30, ccShitYellow);
    CCPoint labelPosMain = mainSkillLabel->getPosition();
    CCPoint labelPosPassive = passiveSkillLabel->getPosition();
    mainSkills->setPosition(ccpAdd(labelPosMain, ccp(-100, 0)));
    passiveSkills->setPosition(ccpAdd(labelPosPassive, ccp(-90, 0)));
    btnMainSkill->addNode(mainSkills);
    btnPassiveSkill->addNode(passiveSkills);
    
    SGCCLabelTTF *roundNum1,   *roundNum2;
    RT_Label("破釜沉舟", 26, ccGrassGreen, mLabelMainSkillName, btnMainSkill)
//    RT_Label(" ( 回合数: ", 26, ccGrassGreen, roundNum1, btnMainSkill)
//    RT_Label("5 )", 26, ccGrassGreen, mLabelMainSkillRound, btnMainSkill)
    RT_Label("造成大量伤害", 28, ccWHITE, mLabelMainSkillDesc, btnMainSkill)
    RT_Label("千钧一发 ", 26, ccGrassGreen, mLabelPassiveSkillName, btnPassiveSkill)
//    RT_Label(" ( 回合数: ", 26, ccGrassGreen, roundNum2, btnPassiveSkill)
//    RT_Label("5 )", 26, ccGrassGreen, mLabelPassiveSkillRound, btnPassiveSkill)
    RT_Label("造成大量伤害", 28, ccWHITE, mLabelPassiveSkillDesc, btnPassiveSkill)

    mLabelMainSkillName->setPosition(ccpAdd(labelPosMain, ccp(60, 0)));
    mLabelMainSkillDesc->setPosition(ccpAdd(labelPosMain, ccp(120, -40)));
//    roundNum1->setPosition(ccpAdd(labelPosMain, ccp(240, 0)));
//    mLabelMainSkillRound->setPosition(ccpAdd(labelPosMain, ccp(320, 0 )));
    
    mLabelPassiveSkillName->setPosition(ccpAdd(labelPosPassive, ccp(60, 0)));
    mLabelPassiveSkillDesc->setPosition(ccpAdd(labelPosPassive, ccp(120, -40)));
    
//    roundNum2->setPosition(ccpAdd(labelPosPassive, ccp(240, 0)));
//    mLabelPassiveSkillRound->setPosition(ccpAdd(labelPosPassive, ccp(320, 0)));

    RT_SPRITE(CCString::createWithFormat("officer_%d.png", officerModel->getIconId())->getCString(), mHeroIcon)
    mHeroIcon->setScale(0.95);
    mPanelVisitResult->addNode(mHeroIcon, 10);
    mHeroIcon->setPosition(ccp(320, 850));

    RT_spine(CCString::createWithFormat("spine/wujiang_%d_r.json", officerModel->getOfficerFileId())->getCString(),
             CCString::createWithFormat("spine/wujiang_%d.atlas", officerModel->getOfficerFileId())->getCString(),
             mSpine)
    mSpine->setAnimation(acActionNames[ACT_moveDown], true);
    mSpine->setScale(0.15);
    mImageHeroLight->addNode(mSpine);
    mImageHeroLight->setScale(1.0f);
    mSpine->setPositionY(30 - mImageHeroLight->getContentSize().height * 0.5 );

    currentAction = ACT_moveDown;
                                     
    RT_spine("effSpine/kaimen.json", "effSpine/kaimen.atlas", mOpenDoor)
    
    mOpenDoor->setScale(0.5);
    
    mOpenDoor->setAnimation("animation", false);
    CCLayerColor*  maskV = CCLayerColor::create(ccc4(0, 0, 0, 255), 640, 1136);
    maskV->setPosition(0, 0);
    
    SGMainManager::shareMain()->getTouchGroup()->addChild(maskV, 100, maskVTag);
    mOpenDoor->setPosition(ccp(320, 568));
    SGMainManager::shareMain()->getTouchGroup()->addChild(mOpenDoor, 101);
    
    CCCallFunc*  callVisit = CCCallFunc::create(this, callfunc_selector(CGPVisitResultLayer::callVisitResult));
    CCDelayTime*  delayVisit = CCDelayTime::create(4.0f);
    
    this->runAction(CCSequence::create(delayVisit, callVisit, nullptr ));

    
    CCSprite* smallIcon;
    RT_SPRITE(CCString::createWithFormat("head%d.png",  officerModel->getIconId())->getCString(), smallIcon)
    mImageAttr->addNode(smallIcon, 10);
    smallIcon->setAnchorPoint(ccp(0, 1));
    smallIcon->setPosition(ccp(-mImageAttr->getContentSize().width * 0.5 + 10 , mImageAttr->getContentSize().height * 0.5 -15));
    
   
    CCSprite*  iconFrame;
    RT_SPRITE("equipPinzhi_5_0.png", iconFrame);
    iconFrame->setAnchorPoint(ccp(0, 0));
    iconFrame->setPosition(ccp(-22, -22));
    smallIcon->addChild(iconFrame);
    
//    showStarts(mOfficer->getCurrStar() ,  10);
     showStarts(1 ,  10);
    
    
    this->setTag(cgp_visitResult);
    SGMainManager::shareMain()->getTouchGroup()->addWidget(mPanelVisitResult);
    mPanelVisitResult->setPosition(ccp(0, 0));
    
    schedule(schedule_selector(CGPVisitResultLayer::updateAnimation), 3.5f);

}

void CGPVisitResultLayer::updateAnimation(float dt)
{
    currentAction = (Action_Sequnce)(currentAction + 1);
    if (currentAction > ACT_atkDown)
    {
        currentAction = ACT_wait;
    }
    
    mSpine->stopAllActions();
    mSpine->setAnimation(acActionNames[currentAction], true);
    
}



void CGPVisitResultLayer::showStarts(int starLevel, int maxStarLevel)
{
    ResourceManager::sharedInstance()->bindTexture("public/public1.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, cgp_visitResult);
    
    //放空的星星
    CCSprite *maxstar = NULL;
    for(int i=0; i < maxStarLevel ; i++)
    {
        maxstar = CCSprite::createWithSpriteFrameName("blackstar.png");
        maxstar->setAnchorPoint(ccp(0.5, 0));
        maxstar->setPosition(ccp(maxstar->getContentSize().width/2 * 0.7  + maxstar->getContentSize().width * i * 0.7 - 100, -30));
        maxstar->setScale(0.7);
        mImageBack->addNode(maxstar,1000);
        starList2[i] = maxstar;
    }
    CCSprite *currstar = NULL;
    for(int i=0; i < starLevel; i++)
    {
        currstar = CCSprite::createWithSpriteFrameName("star.png");
        currstar->setAnchorPoint(ccp(0.5, 0));
        currstar->setScale(0.7);
        currstar->setPosition(ccpAdd(starList2[i]->getPosition(), ccp(-1,-5)));
        mImageBack->addNode(currstar,1000);

    }
    
    
}



void CGPVisitResultLayer::callVisitResult()
{
    SGMainManager::shareMain()->getTouchGroup()->removeChildByTag(maskVTag, true);
}


void CGPVisitResultLayer::btnCallBack(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SGMainManager::shareMain()->showFirstLayer();
    }
    
}
