//
//  SGTestScene.cpp
//  GameSanGuo
//
//  Created by caoguoping on 15-07-31.
//
//

#include "SGTestScene.h"
#include "cocos-ext.h"
#include "GlobalConfig.h"
#include "CCSpriterX.h"
#include "ShiBing.h"



void SGTestScene::spineTest()
{
    
    CCSprite*  bg = CCSprite::create("spine/bg.png");
    //bg->setAnchorPoint(ccp(0,0));
    bg->setPosition(ccp(510, 115));
    this->addChild(bg);
 
#define ATLAS_NAME  "spine/wujiang_104.atlas"
#define SCALE   0.15f
#define TIMESCALE 0.7
    
    
    /*********红绿蓝测试************/
    
#define JSON_NAME_R   "spine/wujiang_104_r.json"
#define JSON_NAME_B   "spine/wujiang_104_b.json"
#define JSON_NAME_Y   "spine/wujiang_104_y.json"

    
    CCSkeletonAnimation*  spineBoy = CCSkeletonAnimation::createWithFile(JSON_NAME_R, ATLAS_NAME, SCALE);
    spineBoy->setAnimation(acActionNames[ACT_atkDown], true, 0);
    spineBoy->setPosition(ccp(200, 200));
    spineBoy->timeScale = TIMESCALE;
    this->addChild(spineBoy);
    
    
    CCSkeletonAnimation*  spineBoy2 = CCSkeletonAnimation::createWithFile(JSON_NAME_B, ATLAS_NAME, SCALE);
    spineBoy2->setAnimation(acActionNames[ACT_atkUp], true, 0);
    spineBoy2->setPosition(ccp(430, 190));
    spineBoy2->timeScale = TIMESCALE;
    this->addChild(spineBoy2);
    
    CCSkeletonAnimation*  spineBoy3 = CCSkeletonAnimation::createWithFile(JSON_NAME_Y, ATLAS_NAME, SCALE);
    spineBoy3->setAnimation(acActionNames[ACT_atkUp], true, 0);
    spineBoy3->setPosition(ccp(200, 600));
    spineBoy3->timeScale = TIMESCALE;
    this->addChild(spineBoy3);
    
    CCSkeletonAnimation*  spineBoy4 = CCSkeletonAnimation::createWithFile(JSON_NAME_R, ATLAS_NAME, SCALE);
    spineBoy4->setAnimation(acActionNames[ACT_atkUp], true, 0);
    spineBoy4->setPosition(ccp(430, 700));
    spineBoy4->timeScale = TIMESCALE;
    this->addChild(spineBoy4);
    
    CCSkeletonAnimation*  spineBoy5 = CCSkeletonAnimation::createWithFile(JSON_NAME_B, ATLAS_NAME, SCALE);
    spineBoy5->setAnimation(acActionNames[ACT_atkDown], true, 0);
    spineBoy5->setPosition(ccp(200, 900));
    spineBoy5->timeScale = TIMESCALE;
    this->addChild(spineBoy5);
    
    CCSkeletonAnimation*  spineBoy6 = CCSkeletonAnimation::createWithFile(JSON_NAME_Y, ATLAS_NAME, SCALE);
    spineBoy6->setAnimation(acActionNames[ACT_atkDown], true, 0);
    spineBoy6->setPosition(ccp(400, 900));
    spineBoy6->timeScale = TIMESCALE;
    this->addChild(spineBoy6);
}


void SGTestScene::effectTest()
{
        CCSkeletonAnimation*  remove = CCSkeletonAnimation::createWithFile("effSpine/xuli.json", "effSpine/xuli.atlas", 1);
        remove->setAnimation("animation2", true, 0);
        remove->setPosition(ccp(320, 640));
        this->addChild(remove,1000);

    //    CCSpriterX*   Effect004 = CCSpriterX::create("effects/004.scml", true, true);
    //    Effect004->setanimaID(0);
    //    Effect004->setisloop(true);
    //    Effect004->play();
    //    Effect004->setPosition(ccp(320, 640));
    //    this->addChild(Effect004);
    

}

void SGTestScene::gotoButtonOk(CCObject* pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        scaleSum += 0.1;
        GPCCLOG("haha, clicked ButtonOk %f", scaleSum);

        myButton->setScale(scaleSum);
    }

    
}

void SGTestScene::btnUpdate(cocos2d::CCObject *pSender, TouchEventType type)
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


void SGTestScene::cocostudioTest()
{

    Layout*  panelUpdate = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("cocosUi/update_1.ExportJson"));
    m_pUIGroup->addWidget(panelUpdate);
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
        UIClick(mBtnUpdate[i], SGTestScene::btnUpdate)
        mBtnUpdate[i]->setTag(i);
        mBtnUpdate[i]->setBrightStyle(BRIGHT_HIGHLIGHT);    //按下的状态
        UIbtnImgOff(mBtnUpdate[i])
        mBtnUpdate[i]->setZOrder(1);
        mPanel_update[i]->setVisible(false);
        mPanel_update[i]->setEnabled(false);
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
    
    for (int i = E_attack; i < E_attributeMax; i ++) {
        mLabelAtt[i] = SGCCLabelTTF::create("34", FONT_PANGWA, 20, ccWHITE);
        mLabelAtt[i]->setPosition(ccp(-40, -2));
        imgAtt[i]->addNode(mLabelAtt[i]);
        
        mLabelAttNew[i] = SGCCLabelTTF::create("45", FONT_PANGWA, 20, ccGrassGreen);
        mLabelAttNew[i]->setPosition(ccp(70, -2));
        imgAtt[i]->addNode(mLabelAttNew[i]);
    }
    
    mLabelLevel = SGCCLabelTTF::create("lv10", FONT_PANGWA, 20, ccWHITE);
    mLabelLevel->setPosition(ccp(-40, -2));
    imgLevel->addNode(mLabelLevel);
    
    mLabelLevelNew = SGCCLabelTTF::create("lv11", FONT_PANGWA, 20, ccShitYellow);
    mLabelLevelNew->setPosition(ccp(70, -2));
    imgLevel->addNode(mLabelLevelNew);
    
    mLabelName = SGCCLabelTTF::create("福岛正则", FONT_PANGWA, 26, ccWHITE);
    mLabelName->setPosition(ccp(0, 0));
    imgName->addNode(mLabelName);

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

void SGTestScene::onEnter()
{
    CCScene::onEnter();
    if (!init())
    {
        return;
    }

    scaleSum = 1;
    m_pUIGroup = TouchGroup::create();
   // m_pUIGroup->scheduleUpdate();
    this->addChild(m_pUIGroup);
    //spineTest();
    //effectTest();
    cocostudioTest();
}

void  SGTestScene::changeAction(CCObject*  obj, Action_Sequnce actSeq)
{
    CCSkeletonAnimation* spBoy = (CCSkeletonAnimation*)obj;
    spBoy->setAnimation(acActionNames[actSeq], true);
}

