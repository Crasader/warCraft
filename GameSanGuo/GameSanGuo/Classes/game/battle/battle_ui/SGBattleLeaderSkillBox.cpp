//
//  SGBattleLeaderSkillBox.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-5-8.
//
//

#include "SGBattleLeaderSkillBox.h"
#include "SGStaticDataManager.h"
#include "SGOfficerDataModel.h"
#include "SGSkillDataModel.h"
#include "SGPlayerInfo.h"
#include "SGGuideManager.h"
#include "SGBattleManager.h"
#include "SGMainManager.h"
#include "ResourceManager.h"

#include "SGTeamgroup.h"
#include "SGStringConfig.h"


SGBattleLeaderSkillBox::SGBattleLeaderSkillBox():m_bIsMe (false)
,m_bIsCanAttack (false)
,m_pLordName (NULL)
,m_nCountryId (0)
,m_nGeneralId (0)
,m_nStarLevel (0)
,m_nLordId (0)
,m_nSkillId (0)
,m_nRoundCount (0)
{

}

SGBattleLeaderSkillBox::~SGBattleLeaderSkillBox()
{
    CCLOG("~SGBattleLeaderSkillBox()");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleLeaderSkillBox);
}

bool SGBattleLeaderSkillBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return false;
}

SGBattleLeaderSkillBox* SGBattleLeaderSkillBox::create(SGBoxDelegate *delegate,bool isme,bool iscanAttack,int generalId,int lordid,int round)
{
    SGBattleLeaderSkillBox *leaderSkill = new SGBattleLeaderSkillBox();
    if(leaderSkill && leaderSkill->init(delegate, box_battleLeaderSkillBox, CCSizeMake(530, 520), false, true, CCSizeMake(530, 150)))
    {
        leaderSkill->del = delegate;
        leaderSkill->m_bIsMe = isme;
        leaderSkill->m_nGeneralId = generalId;
        leaderSkill->m_nLordId = lordid;
        leaderSkill->m_bIsCanAttack = iscanAttack;
        leaderSkill->m_nRoundCount = round;
        
        leaderSkill->viewDidLoad();
        
        leaderSkill->autorelease();
        return leaderSkill;
    }
    
    CC_SAFE_DELETE(leaderSkill);
    return NULL;
}

void SGBattleLeaderSkillBox::viewDidLoad()
{
    
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(false);

    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLeaderSkillBox, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLeaderSkillBox);

    //获取武将的静态数据
    SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(m_nGeneralId);
    m_nCountryId = officerData->getOfficerRace();
    m_nStarLevel = officerData->getOfficerCurrStarLevel();
    m_pLordName = officerData->getOfficerName();
    
    CCPoint centerPoint = SGLayout::getPoint(kMiddleCenter);
    CCPoint center = ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 50));
    
    fontbg = CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    this->addChild(fontbg, 0);
    fontbg->setScaleX((this->boxSize.width-55)/fontbg->getContentSize().width);
    fontbg->setScaleY(this->boxSize.height*.27f/fontbg->getContentSize().height);
//    fontbg->setPreferredSize(CCSizeMake(this->boxSize.width-55, this->boxSize.height*.27f));
    fontbg->setPosition(ccpAdd(center, ccp(0, this->boxSize.height*.27f*.5f)));
    
    //标题
    SGCCLabelTTF *spriteTitle = NULL;
    if(m_bIsMe)
    {
        spriteTitle = SGCCLabelTTF::create(str_Mine_zhujiangji, FONT_PANGWA, 36 , ccWHITE , ccc3(143, 87, 0));
        
//        spriteTitle = CCSprite::createWithSpriteFrameName("敌方主将计");
    }else{
//        spriteTitle = CCSprite::createWithSpriteFrameName("敌方主将计");
         spriteTitle = SGCCLabelTTF::create(str_Enemy_zhujiangji, FONT_PANGWA, 36 , ccWHITE , ccc3(143, 87, 0));
    }
    //modify by:zyc  merge into create.
    //spriteTitle->setOutSideColor(ccc3(143,87,0));
    spriteTitle->setPosition(ccpAdd(biao->getPosition(), ccp(0, 0)));
    addChild(spriteTitle);
    
    //头像
    SGMainManager::shareMain() -> addHeadIconPlistByNum(officerData->getIconId(),sg_battleLeaderSkillBox);
    CCSprite *spriteHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString());
    spriteHead->setPosition(ccpAdd(fontbg->getPosition(), ccp(-150, 0)));
    
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background);
    background->setPosition(spriteHead->getPosition());
    
    addChild(spriteHead);
    
    //头像框
    int starlvl = officerData->getOfficerCurrStarLevel();
    if (starlvl>1) {
        starlvl-=1;
    }
    CCSprite *spriteHeadRect = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
   
    spriteHeadRect->setPosition(spriteHead->getPosition());
    addChild(spriteHeadRect);
    //if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        spriteHeadRect->removeChildByTag(30);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(spriteHeadRect->getContentSize().width*0.35,spriteHeadRect->getContentSize().height - countryName->getContentSize().height));
        spriteHeadRect->addChild(countryName,30,30);
        
        spriteHeadRect->removeChildByTag(250);
        if (officerData->getAdvNum()>0) {
            SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",officerData->getAdvNum())->getCString(), FONT_PANGWA, 24);
            advancenumber->setInsideColor(ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->ignoreAnchorPointForPosition(false);
            advancenumber->setPosition(ccp(spriteHeadRect->getContentSize().width*.95,spriteHeadRect->getContentSize().height));
            spriteHeadRect->addChild(advancenumber,50,250);
        }

    }
    
    //主将姓名
    SGCCLabelTTF *labelGeneralName = SGCCLabelTTF::create(m_pLordName->getCString(), FONT_BOXINFO, 32);
    labelGeneralName->setAnchorPoint(ccp(0,0.5));
    labelGeneralName->setPosition(ccpAdd(fontbg->getPosition(), ccp(-90, 30)));//-55
    addChild(labelGeneralName);
    
    //星星
    for(int i=0;i<3;i++)//統一改為三個星星
    {
        CCSprite *spriteStar = CCSprite::createWithSpriteFrameName("xx.png");
        spriteStar->setPosition(ccpAdd(fontbg->getPosition(), ccp(-70+spriteStar->getContentSize().width*1.1*i, -20)));
        addChild(spriteStar);
    }
    
    //主将计
    
    
    //等级
    SGSkillDataModel *skillData = SGStaticDataManager::shareStatic()->getLordSkillById(m_nLordId);

    CCString *lordSkillNameStr = NULL;
    CCString *lordSkillInfoStr = NULL;
    if(skillData)
    {
        lordSkillNameStr = CCString::create(skillData->getSkillName()->getCString());
        lordSkillInfoStr = CCString::create(skillData->getSkillInfo()->getCString());
    }else{
        lordSkillNameStr = CCString::create(str_No);
        lordSkillInfoStr = CCString::create("");
    }
    
    //主将计
    SGCCLabelTTF *spriteLeaderSkill = SGCCLabelTTF::create(str_Zhujiangji, FONT_BOXINFO, 32 , ccYELLOW);
    //modify by:zyc. merge into create.
    //spriteLeaderSkill->setColor(ccYELLOW);
    spriteLeaderSkill->setPosition(ccpAdd(center, ccp(-boxSize.width*.5f+95, -40)));
    addChild(spriteLeaderSkill);
    
    //主将计
    SGCCLabelTTF *labelLordSkill = SGCCLabelTTF::create(lordSkillNameStr->getCString(), FONT_BOXINFO, 32);
    labelLordSkill->setAnchorPoint(ccp(0,0.5));
    labelLordSkill->setPosition(ccpAdd(spriteLeaderSkill->getPosition(), ccp(spriteLeaderSkill->getContentSize().width*.5f-10, 0)));
    addChild(labelLordSkill);
    
    
    SGCCLabelTTF *labelGeneralLevel = NULL;
    if (skillData)
    {
        labelGeneralLevel = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,skillData->getLevel())->getCString(), FONT_BOXINFO, 32);
    }
    else
    {
        labelGeneralLevel = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,0)->getCString(), FONT_BOXINFO, 32);
    }
    
    labelGeneralLevel->setPosition(ccpAdd(labelLordSkill->getPosition(), ccp(labelLordSkill->getContentSize().width+80, 2)));//80
    addChild(labelGeneralLevel);
     
    //主将计描述
    SGCCLabelTTF *labelLeaderSkillDiscribe = SGCCLabelTTF::create(lordSkillInfoStr->getCString(), FONT_BOXINFO, 28, CCSizeMake(480, 200), kCCTextAlignmentLeft); //str_LiangFeng
    labelLeaderSkillDiscribe->setPosition(ccpAdd(center, ccp(0, -boxSize.height*.5f+85)));
//    labelLeaderSkillDiscribe->setString(lordSkillInfoStr->getCString());
    addChild(labelLeaderSkillDiscribe);
    
    //确定按钮
    SGButton *buttonOK = SGButton::create("public_btn_5.png", "public_font_queding.png", this, menu_selector(SGBattleLeaderSkillBox::buttonClickOK),CCPointZero,false,true);
    buttonOK->setPosition(ccpAdd(centerPoint, ccp(0, -290)));
    addBtn(buttonOK);
    
    //发动主将计
    CCString *skillButtonStr = NULL;
    if(m_bIsCanAttack)
    {
        skillButtonStr = CCString::create("public_btn_5.png");
    }else{
        skillButtonStr = CCString::create("public_btn_4.png");
    }
    
    SGButton *buttonSend = SGButton::create(skillButtonStr->getCString(), "battle_ui_word1.png", this,menu_selector(SGBattleLeaderSkillBox::buttonClickSend),CCPointZero,false,true);
    buttonSend->setPosition(ccpAdd(centerPoint, ccp(-100, -190)));
    addBtn(buttonSend);
    buttonSend->setTag(101);
    
    //取消
    SGButton *buttonCancel = SGButton::create("public_btn_5.png", "public_font_qixiao.png", this, menu_selector(SGBattleLeaderSkillBox::buttonClickOK),CCPointZero,false,true);
    buttonCancel->setPosition(ccpAdd(centerPoint, ccp(100, -190)));
    addBtn(buttonCancel);
    
    //两条线
    CCSprite *spriteLine_1 = CCSprite::createWithSpriteFrameName("gonggao_2.png");
    CCSprite *spriteLine_2 = CCSprite::createWithSpriteFrameName("gonggao_2.png");
    spriteLine_1->setScaleX(411/spriteLine_1->getContentSize().width);
    spriteLine_2->setScaleX(411/spriteLine_2->getContentSize().width);
    spriteLine_1->setPosition(ccpAdd(centerPoint, ccp(0, -30)));
    spriteLine_2->setPosition(ccpAdd(centerPoint, ccp(0, -160)));
//    addChild(spriteLine_1);
//    addChild(spriteLine_2);

    if(m_bIsMe)
    {
        buttonOK->setVisible(false);
        //判断当前是否可以发动攻击，显示提示文字
        SGCCLabelTTF *label = SGCCLabelTTF::create("", FONT_BOXINFO, 32);
        CCString *labelStr = NULL;
        if(m_bIsCanAttack)
        {
            labelStr = CCString::create(str_Now_can_start_zhujiangji);
            label->setString(labelStr->getCString());
            label->setInsideColor(ccGREEN);
        }else{
            labelStr = CCString::createWithFormat(str_Format_some_round_can_start_zhujiangji,m_nRoundCount);
            label->setString(labelStr->getCString());
            label->setInsideColor(ccRED);
            buttonSend->setEnabled(false);
        }
        label->setPosition(ccpAdd(center, ccp(0, -boxSize.height*.4f+35)));
        addChild(label);
        
        
    }else{
        buttonSend->setVisible(false);
        buttonCancel->setVisible(false);
        
    }
    
    
}

void SGBattleLeaderSkillBox::buttonClickOK(cocos2d::CCObject *object)
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(true);
}

void SGBattleLeaderSkillBox::buttonClickSend(cocos2d::CCObject *object)
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("LeaderSkill"), "key");
    this->data = dict;
    this->boxClose();
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(true);

    
    SGGuideManager::shareManager()->getFightGuideLayer()->closeTips();
    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle()) {
        SGGuideManager::shareManager()->startFightGuide(9);
    }
    else{
		CCLOG("9th guide [%s-%d]", __FUNCTION__, __LINE__);

        SGGuideManager::shareManager()->startAutoGuide(9);
    }
    
}

void SGBattleLeaderSkillBox::useLeaderSkill()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("LeaderSkill"), "key");
    this->data = dict;
    this->boxClose();
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(true);
    
    
    SGGuideManager::shareManager()->getFightGuideLayer()->closeTips();
    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
    {
        SGGuideManager::shareManager()->startFightGuide(9);
    }
    else
    {
        CCLOG("9th guide [%s-%d]", __FUNCTION__, __LINE__);
        
        SGGuideManager::shareManager()->startAutoGuide(9);
    }
    
}



















