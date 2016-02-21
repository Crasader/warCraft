//
//  CGPStrengAdvLayer.cpp
//  GameSanGuo
//
//  Created by caoguoping on 15/12/10.
//
//

#include "CGPStrengAdvLayer.h"
#include "SGMainManager.h"
#include "SGStrengSoldierLayer.h"
#include "SGExpDataModel.h"
#include "SGStaticDataManager.h"


#define JUNYING_INFO_ENTER 10
#define HOME_INFO_ENTER 11
#define EMBATTLE_INFO_ENTER 12




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
    expCom = false;
    needUpdate = false;
    pieceNeedNum = 1;
    pieceHasNum = 0;
    
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
        Layer->EmbattleInfo = info;//是否从布阵界面进入
        Layer->isfromfirst = enter;
        Layer->onlyOfficer = isOnlyOfficer;
        Layer->_currentPage = currentPage;
        Layer->cardType = type;
        
        
        
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
    //fake
    mSoulNum = 123450; //假的
    strengTimes = 0;
    
    setIsCocostudio(true);
    panelUpdate  = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("cocosUi/update.ExportJson"));
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
        UIClick(mBtnUpdate[i], CGPStrengAdvLayer::btnCallUpdate)
        mBtnUpdate[i]->setTag(i);
        mBtnUpdate[i]->setBrightStyle(BRIGHT_HIGHLIGHT);    //按下的状态

        UIbtnImgOff(mBtnUpdate[i])
        mBtnUpdate[i]->setZOrder(1);
        mPanel_update[i]->setVisible(false);
        mPanel_update[i]->setEnabled(false);
        GPCCLOG("before mBtnUpdate[%d] :%p",   i, mBtnUpdate[i]);
    }
    

    
    //数值属性
    ImageView* imgAtt[E_attributeMax];
    ImageView *imgName, *imgLevel;
    
    UIGet_ImageView("Image_fontBgGong", panelAtt, imgAtt[E_attack])
    UIGet_ImageView("Image_fontBgFang", panelAtt, imgAtt[E_defense])
    UIGet_ImageView("Image_fontBgXue", panelAtt, imgAtt[E_health])
    UIGet_ImageView("Image_fontBgSu", panelAtt, imgAtt[E_speed])
    UIGet_ImageView("Image_fontBgHero", panelAtt, imgLevel)
    UIGet_ImageView("Image_nameBg", panelAtt, imgName)
    UIGet_ImageView("Image_star", imgLevel, mStar)
    
    //等级lv
    mLevelName = SGCCLabelTTF::create("L V", FONT_PANGWA, 20, ccWHITE);
    mLevelName->setPosition(ccp( -imgLevel->getContentSize().width * 0.5 + 30 , 0));
    imgLevel->addNode(mLevelName);
    
    
    //名字
    mLabelName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_PANGWA, 26, ccWHITE);
    mLabelName->setPosition(ccp(0, 0));
    imgName->addNode(mLabelName);
    
    //create
    //升级前
    currLvl = (int)_card->getCurrLevel();
    originLvl = currLvl;
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
    
    int addAtk, addDef, addMor, addSpeed;
    getAttrAdd(&addAtk, &addDef, &addMor, &addSpeed, currLvl, currLvl + 1);
    
    mLabelAttNew[E_attack] = SGCCLabelTTF::create(CCString::createWithFormat("%d",  _card->getAtk() + addAtk)->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_attack]->setPosition(ccp(70, -2));
    imgAtt[E_attack]->addNode(mLabelAttNew[E_attack]);
    
    mLabelAttNew[E_defense] = SGCCLabelTTF::create(CCString::createWithFormat("%d", _card->getDef() + addDef)->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_defense]->setPosition(ccp(70, -2));
    imgAtt[E_defense]->addNode(mLabelAttNew[E_defense]);
    
    mLabelAttNew[E_health] = SGCCLabelTTF::create(CCString::createWithFormat("%d", _card->getMorale() + addMor)->getCString() , FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_health]->setPosition(ccp(70, -2));
    imgAtt[E_health]->addNode(mLabelAttNew[E_health]);
    
    mLabelAttNew[E_speed] = SGCCLabelTTF::create(CCString::createWithFormat("%d", (int)(_card->getSpeed() + addSpeed))->getCString(), FONT_PANGWA, 20, ccWHITE);
    mLabelAttNew[E_speed]->setPosition(ccp(70, -2));
    imgAtt[E_speed]->addNode(mLabelAttNew[E_speed]);
    
    
    if (SGMainManager::shareMain()->getIsAdv())
    {
        mStar->setVisible(true);
        mLevelName->setVisible(false);
        updateAttrOne(false);
        
        
        
    }
    else
    {
        mStar->setVisible(false);
        mLevelName->setVisible(true);
        

        
    }

    //头像
    SGMainManager::shareMain()->addOfficerPng(_card->getHeadSuffixNum(), sg_generalInfoLayer);
    CCString *str = CCString::createWithFormat("officer_%d.png",_card->getHeadSuffixNum());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(-100, 300));
    panelUpdate->addNode(item, 1000);
    item->setScale(0.9);
    
    //背景色
    UIGet_ImageView("Image_peopleBgGreen", panelUpdate, m_officerBg[0])
    UIGet_ImageView("Image_peopleBgBlue", panelUpdate, m_officerBg[1])
    UIGet_ImageView("Image_peopleBgPurple", panelUpdate, m_officerBg[2]);
    UIGet_ImageView("Image_peopleBgPurple", panelUpdate, m_officerBg[3]);
    showOfficerBg(_card->getCurrStar());

    //强化界面
    ImageView*  soulBarBg;
    UIGet_ImageView("Image_loadingBg", mPanel_update[E_qianghua]  , soulBarBg)
    UIGet_LoadingBar("ProgressBar_qianghua", soulBarBg, mBarSoul)
    mBarSoul->setPercent(90);
    
    ImageView*  soulFontBg;
    UIGet_ImageView("Image_soulFontBg", mPanel_update[E_qianghua], soulFontBg)
    mLabelSoulNum = SGCCLabelTTF::create(CCString::createWithFormat("%d", mSoulNum)->getCString() , FONT_PANGWA, 20, ccWHITE);    //总经验池数目
    mPanel_update[E_qianghua]->addNode(mLabelSoulNum, 5);
    mLabelSoulNum->setPosition(ccpAdd(soulFontBg->getPosition(),  ccp(0, 0)));
    
    Button   *btnBack,   *btnOK,   *btnCancel,  *btnAddSoul;
    UIGet_Button("Button_back", mPanel_update[E_qianghua], btnBack)
    UIGet_Button("Button_OK", mPanel_update[E_qianghua], btnOK)
    UIGet_Button("Button_cancel", mPanel_update[E_qianghua], btnCancel)
    UIGet_Button("Button_add", mPanel_update[E_qianghua], btnAddSoul)
    UIClick(btnBack, CGPStrengAdvLayer::btnCallBack)
//    UIClick(btnOK, CGPStrengAdvLayer::btnCallStrengOk)
//    UIClick(btnCancel, CGPStrengAdvLayer::btnCallStrengCancel)
    UIClick(btnOK, CGPStrengAdvLayer::btnCallBack)
    UIClick(btnCancel, CGPStrengAdvLayer::btnCallBack)
    UIClick(btnAddSoul, CGPStrengAdvLayer::btnCallAddExp)
    
    
    strengLimitLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    int maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(cardType, currLvl + 1)->getExpValue() ;
    float per = (float)_card->getCurrExp() * 100/ maxExp;
    mBarSoul->setPercent(per);
    mLabelSoulBar = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", _card->getCurrExp(), maxExp)->getCString(), FONT_PANGWA, 20, ccWHITE);
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
    UIClick(btnJjBack, CGPStrengAdvLayer::btnCallBack)
    UIClick(btnJjJinjie, CGPStrengAdvLayer::btnCallAdvOk)
       // UIClick(btnJjJinjie, CGPStrengAdvLayer::btnCallBack)
    
    
    //此界面只接受 武将转生

    SGPiecesDataModel * piece = SGStaticDataManager::shareStatic()->getPiecesDictByOfficerProtoId(_card->getProtoId());
    if(piece)
    {
        int itemId = piece->getPiecesId();
        CCArray * array = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
        SGPiecesCard * temp;
        for(int i = 0 ; i < array->count() ;  ++i)
        {
            temp = (SGPiecesCard*)array->objectAtIndex(i);
            if(itemId == temp->getItemId())
            {
                pieceHasNum = temp->getPiecesCurNum();
                break;
            }
        }
    }
    
    int officerItemId = _card->getItemId();
    SGOfficerDataModel * model = SGStaticDataManager::shareStatic()->getOfficerById(officerItemId);
    pieceNeedNum = SGStaticDataManager::shareStatic()->getAdvOfficerNeedPieceNum(model->getOfficerCurrStarLevel(), model->getAdvNum());
    if(pieceNeedNum <= pieceHasNum)
    {
        m_canAdcance = true;
    }

    
    
    
    ImageView  *barJijieBg ;
    UIGet_ImageView("Image_loadingBg", mPanel_update[E_jinjie], barJijieBg)
    UIGet_LoadingBar("ProgressBar_jinjie", barJijieBg, mBarFragments)
    int perFrag = pieceHasNum <= pieceNeedNum ? pieceHasNum * 100 / pieceNeedNum  : 100;
    mBarFragments->setPercent(perFrag);
    
    mLabelFragments = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", pieceHasNum , pieceNeedNum)->getCString(), FONT_PANGWA, 20, ccWHITE);
    mBarFragments->addNode(mLabelFragments);
    
    ImageView*  imgJinjieFontBg;
    UIGet_ImageView("Image_23", mPanel_update[E_jinjie], imgJinjieFontBg)
    SGCCLabelTTF* labelJinjie0 = SGCCLabelTTF::create("进阶需求:", FONT_PANGWA, 30, ccDarkOrange);
    labelJinjie0->setPosition(ccpAdd(imgJinjieFontBg->getPosition(),  ccp(0, 45)));
    mPanel_update[E_jinjie]->addNode(labelJinjie0);
    SGCCLabelTTF* labelJinjie1 = SGCCLabelTTF::create("收集相应的武将碎片进阶武将", FONT_PANGWA, 26,  CCSize(260,0), ccWHITE);
    labelJinjie1->setPosition(ccpAdd(imgJinjieFontBg->getPosition(),  ccp(0, -5)));
    mPanel_update[E_jinjie]->addNode(labelJinjie1);
    
    
    
    //从何来
    if (SGMainManager::shareMain()->getIsAdv())
    {
        mNowUpdateTag = E_jinjie;
        mBtnUpdate[mNowUpdateTag]->setBrightStyle(BRIGHT_NORMAL);
        UIbtnImgOn(mBtnUpdate[mNowUpdateTag])
        mBtnUpdate[mNowUpdateTag]->setZOrder(4);
        mPanel_update[mNowUpdateTag]->setVisible(true);
        mPanel_update[mNowUpdateTag]->setEnabled(true);
        mBtnUpdate[mNowUpdateTag]->setTouchEnabled(false);
        
        mLabelLevel->setString(CCString::createWithFormat("%d",  _card->getCurrStar())->getCString());
        mLabelLevelNew->setString(CCString::createWithFormat("%d",  _card->getCurrStar() + 1)->getCString());
        
    }
    else
    {
        mNowUpdateTag = E_qianghua;
        mBtnUpdate[mNowUpdateTag]->setBrightStyle(BRIGHT_NORMAL);
        UIbtnImgOn(mBtnUpdate[mNowUpdateTag])
        mBtnUpdate[mNowUpdateTag]->setZOrder(4);
        mPanel_update[mNowUpdateTag]->setVisible(true);
        mPanel_update[mNowUpdateTag]->setEnabled(true);
        mBtnUpdate[mNowUpdateTag]->setTouchEnabled(false);
        
        mLabelLevel->setString(CCString::createWithFormat("%d",  _card->getCurrLevel())->getCString());
        mLabelLevelNew->setString(CCString::createWithFormat("%d",  _card->getCurrLevel()  + 1)->getCString());
    }

    

}

void CGPStrengAdvLayer::showOfficerBg(int quality)
{
    //如果有意外情况 品质不属于 绿 蓝 紫 橙
    if(quality >6 || quality<3)
    {
        quality = 3;
    }
    for(int i =0 ; i < 4; i++)
    {
        m_officerBg[i]->setVisible(false);
    }
    m_officerBg[quality - 3]->setVisible(true);
}

void CGPStrengAdvLayer::updateAttrOne(bool isLevelUp)
{
    int addAtk, addDef, addMor, addSpeed;

    if (isLevelUp)
    {
        getAttrAdd(&addAtk, &addDef, &addMor, &addSpeed, originLvl, currLvl + 1);
        mLabelLevelNew->setString(CCString::createWithFormat("%d", currLvl + 1)->getCString());
        mLabelAttNew[E_attack]->setString(CCString::createWithFormat("%d", _card->getAtk() + addAtk)->getCString());
        mLabelAttNew[E_defense]->setString(CCString::createWithFormat("%d", _card->getDef() +addDef )->getCString());
        mLabelAttNew[E_health]->setString(CCString::createWithFormat("%d", _card->getMorale() + addMor)->getCString());
        mLabelAttNew[E_speed]->setString(CCString::createWithFormat("%d", (int)_card->getSpeed() + addSpeed)->getCString());
    }
    else
    {
        mLabelLevelNew->setString(CCString::createWithFormat("%d",  _card->getCurrStar() + 1)->getCString());
        mLabelAttNew[E_attack]->setString(CCString::createWithFormat("%d", int(_card->getAtk() * 1.12))->getCString());
        mLabelAttNew[E_defense]->setString(CCString::createWithFormat("%d", int(_card->getDef() * 1.13) )->getCString());
        mLabelAttNew[E_health]->setString(CCString::createWithFormat("%d", int(_card->getMorale()* 1.14))->getCString());
        mLabelAttNew[E_speed]->setString(CCString::createWithFormat("%d", (int)(_card->getSpeed() * 1.13))->getCString());
    }
   
    



}


void  CGPStrengAdvLayer::getAttrAdd(int *atk, int *def, int *mor, int *speed, int curLvl, int nextLvl)
{
    CCDictionary *dicc = SGCardAttribute::getValue(curLvl, _card->getItemId());
    CCDictionary *dicc1 = SGCardAttribute::getValue(nextLvl, _card->getItemId());
    
    *atk = ((CCString *)dicc1->objectForKey("atk"))->intValue() - ((CCString *)dicc->objectForKey("atk"))->intValue();
    *def = ((CCString *)dicc1->objectForKey("def"))->intValue() - ((CCString *)dicc->objectForKey("def"))->intValue();
    *mor = ((CCString *)dicc1->objectForKey("mor"))->intValue() - ((CCString *)dicc->objectForKey("mor"))->intValue();
    *speed = ((CCString *)dicc1->objectForKey("speed"))->floatValue() - ((CCString *)dicc->objectForKey("speed"))->floatValue();
}


void CGPStrengAdvLayer::updateStrengthExp(float dt)
{
    if(needUpdate)
    {
        updateAttrOne(true);
        needUpdate = false;
    }
    
    expCom = true;
    if (mSoulNum <= 0)
    {
        return;
    }
    
    int maxExp;
    int everyExp;
    
    strengLimitLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    maxExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(cardType, currLvl + 1)->getExpValue() ;
    everyExp = 0.1 * maxExp;
    mSoulNum = mSoulNum - everyExp;
    if (mSoulNum <= 0)
    {
        _card->setCurrExp(_card->getCurrExp() + everyExp + mSoulNum);  //剩多少加多少
        mSoulNum = 0;
    }
    _card->setCurrExp(_card->getCurrExp() + everyExp);
    
    mLabelSoulNum->setString(CCString::createWithFormat("%d", mSoulNum)->getCString());
    
    strengTimes ++;
    

    float per = _card->getCurrExp() * 100 / maxExp;
    mBarSoul->setPercent(per);
    mLabelSoulBar->setString(CCString::createWithFormat("%d/%d", _card->getCurrExp(), maxExp)->getCString());

    if (strengTimes >= 10)
    {
        strengTimes = 0;
        currLvl ++;
        _card->setCurrExp(0);
        needUpdate = true;
    }
   
}

void  CGPStrengAdvLayer::btnCallAdvOk(CCObject* pSender, TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
    {
        if (pieceHasNum < pieceNeedNum)
        {
             SGMainManager::shareMain()->showMessage("进阶所需武将数量不够!");
        }
        else
        {
            _card->setCurrStar(_card->getCurrStar() + 1);
            pieceHasNum -= pieceNeedNum;
            
            SGMainManager::shareMain()->setWillAdvancedItemId(_card->getItemId());
           // SGMainManager::shareMain()->setAfterAdvancedItemId(afterCard->getItemId());
            SGMainManager::shareMain()->setAfterAdvancedSsid(_card->getSsid());
            
            main::OfficerUpgradeRequest *request = new main::OfficerUpgradeRequest();
            request->set_officerid(_card->getSsid());
            SGSocketClient::sharedSocketClient()->send(MSG_BARRACKS_ADVANCE, request);
            
            
            btnCallBack(this, TOUCH_EVENT_ENDED);
        }
        
        
       
    }
}

void  CGPStrengAdvLayer::btnCallStrengOk(CCObject* pSender, TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
    {
        
    }
}

void  CGPStrengAdvLayer::btnCallStrengCancel(CCObject* pSender, TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
    {
        
    }
}

void  CGPStrengAdvLayer::btnCallAddExp(CCObject* pSender, TouchEventType type)
{

    if(type == TOUCH_EVENT_ENDED)
    {
        if (!expCom)   //单次加
        {
            updateStrengthExp(0);
        }
        expCom = false;
        unschedule(schedule_selector(CGPStrengAdvLayer::updateStrengthExp));
        
    }
    else if(type == TOUCH_EVENT_BEGAN)
    {
        schedule(schedule_selector(CGPStrengAdvLayer::updateStrengthExp), 0.1, kCCRepeatForever, 0.5);
    }
}


void  CGPStrengAdvLayer::btnCallBack(CCObject* pSender, TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
    {
        EFFECT_PLAY(MUSIC_BTN);
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(true);
        
        //如果从布阵界面进入
        if (EmbattleInfo && EmbattleInfo->isFromEmbattle)
        {
            CCLog("Back To Embattle");
            //返回布阵页面
            _card->retain();
            SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, (SGOfficerCard *)_card, EmbattleInfo->ty, EmbattleInfo->em);
            SGMainManager::shareMain()->showBox(embattleBox);
        }
        else if (isfromfirst==4)
            //if (isfromfirst==4)
        {
            SGMainManager::shareMain()->showGeneralsLayer(4,true,0,_currentPage);
        }
        else if(isfromfirst==5)
        {
            SGMainManager::shareMain()->showGeneralsLayer(5,true,0,_currentPage);
        }
        else if (isfromfirst==1)
        {
            if (cardType == 1)
            {
                SGMainManager::shareMain()->showGeneralsLayer(2,true,0,_currentPage);
            }
            else if (cardType == 2)
            {
                SGMainManager::shareMain()->showEquipsLayer(NULL,0,1,true,0 ,_currentPage,0);
            }
            else if (cardType == 3)
            {
                SGStrengSoldierLayer *layer = SGStrengSoldierLayer::create();
                SGMainManager::shareMain()->showLayer(layer);
            }
            else
            {
                SGMainManager::shareMain()->showGeneralsLayer(5,true,0,_currentPage);
            }
        }
        else if (isfromfirst == JUNYING_INFO_ENTER)
        {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card,0,NULL,true,NULL,false,NULL,1,0);
        }
        else if (isfromfirst == HOME_INFO_ENTER)   //主页进入
        {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card, 3,NULL,true,NULL,11);
        }
        else if (isfromfirst == EMBATTLE_INFO_ENTER)   //布阵进入
        {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card, 22,NULL,true,NULL,12);
        }
        else if(isfromfirst==3)
        {
            //int id =  (SGEquipCard*)_card->getOfficerCardId();
            SGEquipCard * equip =(SGEquipCard*)_card;
            int id = equip->getOfficerCardId();
            
            SGMainManager::shareMain()->showGeneralInfoLayer(SGPlayerInfo::sharePlayerInfo()->getOfficerCard(id), 3,NULL,true,NULL,12);
            
        }
        else
        {
            if (cardType == 1)
            {
                SGMainManager::shareMain()->showGeneralsLayer(0,true,0,_currentPage);
            }
            else if (cardType == 2)
            {
                SGMainManager::shareMain()->showEquipsLayer(NULL,0,0,true,0 ,_currentPage,0);
            }else if (cardType == 3){
                SGMainManager::shareMain()->showSoldierslayer(true,3);
            }else
            {
                SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard*)_card, 0);
            }
        }

    }

}


void CGPStrengAdvLayer::btnCallUpdate(cocos2d::CCObject *pSender, TouchEventType type)
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
        
        if (btnTag == E_jinjie)
        {
            mLabelLevel->setString(CCString::createWithFormat("%d",  _card->getCurrStar())->getCString());
            mLabelLevelNew->setString(CCString::createWithFormat("%d",  _card->getCurrStar() + 1)->getCString());
            mStar->setVisible(true);
            mLevelName->setVisible(false);
            updateAttrOne(false);
        }
        else
        {
            mLabelLevel->setString(CCString::createWithFormat("%d",  _card->getCurrLevel())->getCString());
            mLabelLevelNew->setString(CCString::createWithFormat("%d",  _card->getCurrLevel()  + 1)->getCString());
            mStar->setVisible(false);
            mLevelName->setVisible(true);
            updateAttrOne(true);
        }
        

    }
    
}






