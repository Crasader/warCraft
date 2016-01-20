//
//  CGPStrengAdvLayer.cpp
//  GameSanGuo
//
//  Created by caoguoping on 15/12/10.
//
//

#include "CGPStrengAdvLayer.h"
#include "SGMainManager.h"


CGPStrengAdvLayer::CGPStrengAdvLayer()
{
    panelUpdate = NULL;
    mBarSoul = NULL;
    mBarFragments = NULL;
    for (int i = 0 ; i < E_updateBtnMax; i ++)
    {
        mBtnUpdate[i] = NULL;
    }
    for (int i = 0; i < E_updateBtnMax; i ++)
    {
        mPanel_update[i] = NULL;
    }
    
}

CGPStrengAdvLayer::~CGPStrengAdvLayer()
{

}

CGPStrengAdvLayer*  CGPStrengAdvLayer::create(SGBaseMilitaryCard *card, int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage)
{
    CGPStrengAdvLayer *Layer = new CGPStrengAdvLayer();
    if (Layer && Layer->init(card  , type,  isOnlyOfficer))
    {
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

bool CGPStrengAdvLayer::init(SGBaseMilitaryCard *card, int type,bool isOnlyOfficer)
{
    if (!SGBaseLayer::init(NULL, cgp_strengAdvLayer))
    {
        return false;
    }
 //   SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    //notification->addObserver(MSG_BARRACKS_STRENG,   this, callfuncO_selector(SGStrengLayer::strengOfficerListener));

    _card = card;
    
    //SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_strengLayer, LIM_PNG_AS_PNG);
    
    _card->retain();
    return true;
    
}


void CGPStrengAdvLayer::initView()
{
    setIsCocostudio(true);
    panelUpdate  = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("cocosUi/update_1.ExportJson"));
    SGMainManager::shareMain()->getMainScene()->mCocosLayers[cgp_strengAdvLayer - cocostudioLayerStart] = panelUpdate;

    
    this->setTag(cgp_strengAdvLayer);
    SGMainManager::shareMain()->getTouchGroup()->addWidget(panelUpdate);
       
    panelUpdate->setPosition(ccp(0, 120));
    Layout*  panelbg  = NULL;
    UIGet_Layout("Panel_bg", panelUpdate, panelbg)
    
    //标签页通用
    UIGet_Layout("Panel_jinjie", panelUpdate, mPanel_update[E_jinjie])
    UIGet_Layout("Panel_qianghua", panelUpdate, mPanel_update[E_qianghua])

    Layout*  panelAtt;
    UIGet_Layout("Panel_att", panelUpdate, panelAtt)
    UIGet_Button("Button_qianghua", panelbg, mBtnUpdate[E_qianghua])
    UIGet_Button("Button_jinjie", panelbg, mBtnUpdate[E_jinjie])

    for (int i = E_qianghua; i < E_updateBtnMax; i ++)
    {
        UIClick(mBtnUpdate[i], CGPStrengAdvLayer::btnUpdate)
        mBtnUpdate[i]->setTag(i);
        mBtnUpdate[i]->setBrightStyle(BRIGHT_HIGHLIGHT);    //按下的状态

        UIbtnImgOff(mBtnUpdate[i])
        mBtnUpdate[i]->setZOrder(1);
        mPanel_update[i]->setVisible(false);
        mPanel_update[i]->setEnabled(false);
        GPCCLOG("before mBtnUpdate[%d] :%p",   i, mBtnUpdate[i]);
    }
    
    //默认第一个标签页生效
    mNowUpdateTag = E_qianghua;
    mBtnUpdate[mNowUpdateTag]->setBrightStyle(BRIGHT_NORMAL);
    UIbtnImgOn(mBtnUpdate[mNowUpdateTag])
    mBtnUpdate[mNowUpdateTag]->setZOrder(4);
    mPanel_update[mNowUpdateTag]->setVisible(true);
    mPanel_update[mNowUpdateTag]->setEnabled(true);
    mBtnUpdate[mNowUpdateTag]->setTouchEnabled(false);
    
    //数值属性
    ImageView* imgAtt[E_attributeMax];
    ImageView *imgName, *imgLevel;
    UIGet_ImageView("Image_fontBgGong", panelAtt, imgAtt[E_attack])
    UIGet_ImageView("Image_fontBgFang", panelAtt, imgAtt[E_defense])
    UIGet_ImageView("Image_fontBgXue", panelAtt, imgAtt[E_health])
    UIGet_ImageView("Image_fontBgSu", panelAtt, imgAtt[E_speed])
    UIGet_ImageView("Image_fontBgHero", panelAtt, imgLevel)
    UIGet_ImageView("Image_nameBg", panelAtt, imgName)
    
    
    mLabelName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_PANGWA, 26, ccWHITE);
    mLabelName->setPosition(ccp(0, 0));
    imgName->addNode(mLabelName);
    
    currLvl = (int)_card->getCurrLevel();
    mLabelLevel = SGCCLabelTTF::create(CCString::createWithFormat("%d", currLvl)->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelLevel->setPosition(ccp(-40, -2));
    imgLevel->addNode(mLabelLevel);
    mLabelAtt[E_attack] = SGCCLabelTTF::create(CCString::createWithFormat("%d", _card->getAtk())->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAtt[E_attack]->setPosition(ccp(-40, -2));
    imgAtt[E_attack]->addNode(mLabelAtt[E_attack]);
    
    mLabelAtt[E_defense] = SGCCLabelTTF::create(CCString::createWithFormat("%d", _card->getDef())->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAtt[E_defense]->setPosition(ccp(-40, -2));
    imgAtt[E_defense]->addNode(mLabelAtt[E_defense]);
    
    mLabelAtt[E_health] = SGCCLabelTTF::create(CCString::createWithFormat("%d", _card->getMorale())->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAtt[E_health]->setPosition(ccp(-40, -2));
    imgAtt[E_health]->addNode(mLabelAtt[E_health]);
    
    mLabelAtt[E_speed] = SGCCLabelTTF::create(CCString::createWithFormat("%d", (int)_card->getSpeed())->getCString(), FONT_PANGWA, 20, ccWHITE);
    mLabelAtt[E_speed]->setPosition(ccp(-40, -2));
    imgAtt[E_speed]->addNode(mLabelAtt[E_speed]);
    
    
    //升级后
    mLabelLevelNew = SGCCLabelTTF::create(CCString::createWithFormat("%d", currLvl + 1)->getCString(), FONT_PANGWA, 20, ccShitYellow);
    mLabelLevelNew->setPosition(ccp(70, -2));
    imgLevel->addNode(mLabelLevelNew);
    
    CCDictionary *dicc = SGCardAttribute::getValue(currLvl + 1, _card->getItemId());

    mLabelAttNew[E_attack] = SGCCLabelTTF::create(CCString::createWithFormat("%d", ((CCString *)dicc->objectForKey("atk"))->intValue())->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_attack]->setPosition(ccp(70, -2));
    imgAtt[E_attack]->addNode(mLabelAttNew[E_attack]);
    
    mLabelAttNew[E_defense] = SGCCLabelTTF::create(CCString::createWithFormat("%d", ((CCString *)dicc->objectForKey("def"))->intValue())->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_defense]->setPosition(ccp(70, -2));
    imgAtt[E_defense]->addNode(mLabelAttNew[E_defense]);
    
    mLabelAttNew[E_health] = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_health]->setPosition(ccp(70, -2));
    imgAtt[E_health]->addNode(mLabelAttNew[E_health]);
    
    mLabelAttNew[E_speed] = SGCCLabelTTF::create(CCString::createWithFormat("%d", (int)((CCString *)dicc->objectForKey("speed"))->intValue())->getCString(), FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_speed]->setPosition(ccp(70, -2));
    imgAtt[E_speed]->addNode(mLabelAttNew[E_speed]);


    
    //强化界面
    ImageView*  soulBarBg;
    UIGet_ImageView("Image_loadingBg", mPanel_update[E_qianghua]  , soulBarBg)
    UIGet_LoadingBar("ProgressBar_qianghua", soulBarBg, mBarSoul)
    mBarSoul->setPercent(90);
    
    ImageView*  soulFontBg;
    UIGet_ImageView("Image_soulFontBg", mPanel_update[E_qianghua], soulFontBg)
    mLabelSoulNum = SGCCLabelTTF::create("230000", FONT_PANGWA, 20, ccWHITE);
    mPanel_update[E_qianghua]->addNode(mLabelSoulNum, 5);
    mLabelSoulNum->setPosition(ccpAdd(soulFontBg->getPosition(),  ccp(0, 0)));
    
    Button   *btnBack,   *btnOK,   *btnCancel,  *btnAddSoul;
    UIGet_Button("Button_back", mPanel_update[E_qianghua], btnBack)
    UIGet_Button("Button_OK", mPanel_update[E_qianghua], btnOK)
    UIGet_Button("Button_cancel", mPanel_update[E_qianghua], btnCancel)
    UIGet_Button("Button_add", mPanel_update[E_qianghua], btnAddSoul)
    
    mLabelSoulBar = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", 9, 10)->getCString(), FONT_PANGWA, 20, ccWHITE);
    mBarSoul->addNode(mLabelSoulBar);
    
    ImageView*  imgQianghuaFontBg;
    UIGet_ImageView("Image_qianghuaFontbg", mPanel_update[E_qianghua], imgQianghuaFontBg)
    SGCCLabelTTF* labelQianghua0 = SGCCLabelTTF::create("强化需求:", FONT_PANGWA, 30, ccDarkOrange);
    labelQianghua0->setPosition(ccpAdd(imgQianghuaFontBg->getPosition(),  ccp(0, 45)));
    mPanel_update[E_qianghua]->addNode(labelQianghua0);
    SGCCLabelTTF* labelQianghua1 = SGCCLabelTTF::create("收集足够的灵魂石来强化武将", FONT_PANGWA, 26,  CCSize(260,0), ccWHITE);
    labelQianghua1->setPosition(ccpAdd(imgQianghuaFontBg->getPosition(),  ccp(0, -5)));
    mPanel_update[E_qianghua]->addNode(labelQianghua1);
    
    //进阶界面
    Button *btnJjBack,  *btnJjJinjie;
    UIGet_Button("Button_back", mPanel_update[E_jinjie], btnJjBack)
    UIGet_Button("Button_jinjie", mPanel_update[E_jinjie], btnJjJinjie)
    
    ImageView  *barJijieBg ;
    UIGet_ImageView("Image_loadingBg", mPanel_update[E_jinjie], barJijieBg)
    UIGet_LoadingBar("ProgressBar_jinjie", barJijieBg, mBarFragments)
    mBarFragments->setPercent(60);
    
    mLabelFragments = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", 6, 10)->getCString(), FONT_PANGWA, 20, ccWHITE);
    mBarFragments->addNode(mLabelFragments);
    
    ImageView*  imgJinjieFontBg;
    UIGet_ImageView("Image_23", mPanel_update[E_jinjie], imgJinjieFontBg)
    SGCCLabelTTF* labelJinjie0 = SGCCLabelTTF::create("进阶需求:", FONT_PANGWA, 30, ccDarkOrange);
    labelJinjie0->setPosition(ccpAdd(imgJinjieFontBg->getPosition(),  ccp(0, 45)));
    mPanel_update[E_jinjie]->addNode(labelJinjie0);
    SGCCLabelTTF* labelJinjie1 = SGCCLabelTTF::create("收集相应的武将碎片进阶武将", FONT_PANGWA, 26,  CCSize(260,0), ccWHITE);
    labelJinjie1->setPosition(ccpAdd(imgJinjieFontBg->getPosition(),  ccp(0, -5)));
    mPanel_update[E_jinjie]->addNode(labelJinjie1);

}


void CGPStrengAdvLayer::btnUpdate(cocos2d::CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        Button*  btnNow = static_cast<Button*>(pSender);
        E_updateTag  btnTag = E_updateTag(btnNow->getTag());
        if (btnTag == mNowUpdateTag)
        {
            return;
        }
        
        for (int i = E_qianghua; i < E_updateBtnMax; i ++)
        {
            GPCCLOG("click mBtnUpdate[%d] :%p",   i, mBtnUpdate[i]);

            mBtnUpdate[i]->setBrightStyle(BRIGHT_HIGHLIGHT);    //按下的状态
            UIbtnImgOff(mBtnUpdate[i])
            mBtnUpdate[i]->setZOrder(1);
            mPanel_update[i]->setVisible(false);
            mPanel_update[i]->setEnabled(false);
            mBtnUpdate[mNowUpdateTag]->setTouchEnabled(true);
        }
        
        mNowUpdateTag = btnTag;
        mBtnUpdate[mNowUpdateTag]->setBrightStyle(BRIGHT_NORMAL);
        mPanel_update[mNowUpdateTag]->setVisible(true);
        mPanel_update[mNowUpdateTag]->setEnabled(true);
        mBtnUpdate[mNowUpdateTag]->setZOrder(4);
        UIbtnImgOn(mBtnUpdate[mNowUpdateTag])
        mBtnUpdate[mNowUpdateTag]->setTouchEnabled(false);

    }
    
}






