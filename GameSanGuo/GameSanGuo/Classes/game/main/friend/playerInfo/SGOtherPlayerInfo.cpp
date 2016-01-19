//
//  SGOtherPlayerInfo.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#include "SGOtherPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGFriend.h"
#include "SGSocketClient.h"
#include "FriendModule.pb.h"
#include "SGMainManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGStaticDataManager.h"
#include "SGSkillDataModel.h"
#include "SGGeneralsLayer.h"
#include "SGFriendBase.h"
#include "SGMainLayer.h"
#include "SGFindHelpLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGOtherPlayerInfo::SGOtherPlayerInfo()
: labelPlayerName(NULL)
, labelPlayerCountry(NULL)
, labelPlayerLevel(NULL)
, spritePlayerImage(NULL)
, labelOfficerName(NULL)
, labelOfficerLevel(NULL)
, buttonOfficerHead(NULL)
, labelMainOfficerSkill(NULL)
, labelOfficerSkill(NULL)
, spritePlayerImageBG(NULL)
, labelAttack_1(NULL)
, labelStoreUp(NULL)
, labelDefense_1(NULL)
, labelSpeed(NULL)
, labelMorale_1(NULL)
, labelBattleForce(NULL)

{
    _isFriend =true;
    _playerInfo = NULL;
    
}

SGOtherPlayerInfo::~SGOtherPlayerInfo()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_otherPlayerInfo);

    _playerInfo->release();
}

SGOtherPlayerInfo *SGOtherPlayerInfo::create(SGGamePlayerInfo *playerInfo , SGBoxDelegate *_delegate)
{
    SGOtherPlayerInfo *other = new SGOtherPlayerInfo();
    other->_playerInfo = playerInfo;
    other->_playerInfo->retain();
    if(other && other->init(_delegate, box_otherPlayerInfo, CCSizeMake(535, 760),true))
    {
    
        
        other->constructView();
        other->autorelease();
        return other;
    }
    
    CC_SAFE_DELETE(other);
    return NULL;
}

void SGOtherPlayerInfo::constructView()
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint leftCenter = SGLayout::getPoint(kMiddleLeft);
    biao->setVisible(false);
    fontbg->setPreferredSize(CCSize(480, 680));
    fontbg->setPosition(ccpAdd(center, ccp(0, 25)));
    

    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);
 
    
    
    //金线
    CCSprite *sprite_line_1 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    sprite_line_1->setScaleX(480/sprite_line_1->getContentSize().width);
    sprite_line_1->setPosition(ccpAdd(biao->getPosition(), ccp(0,-123)));
    
    //金线
    CCSprite *sprite_line_2 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    sprite_line_2->setScaleX(480/sprite_line_2->getContentSize().width);
    sprite_line_2->setPosition(ccpAdd(biao->getPosition(), ccp(0,-252)));
    
    //金线
    CCSprite *sprite_line_3 =CCSprite::createWithSpriteFrameName("login_name_line.png");
    sprite_line_3->setScaleX(480/sprite_line_3->getContentSize().width);
    sprite_line_3->setPosition(ccpAdd(center, ccp(0, 0)));
    
    //金线
    CCSprite *sprite_line_4 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    sprite_line_4->setScaleX(480/sprite_line_4->getContentSize().width);
    sprite_line_4->setPosition(ccpAdd(center, ccp(0, -135)));
    
    addChild(sprite_line_1,5);
    addChild(sprite_line_2,5);
    addChild(sprite_line_3,5);
    addChild(sprite_line_4,5);

    //玩家信息
    
    
    //    labelPlayerID = CCLabelTTF::create("(角色ID)", FONT_BOXINFO, 20);
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_otherPlayerInfo);
    spritePlayerImage = CCSprite::createWithSpriteFrameName("head1001.png");
    spritePlayerImageBG = CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
    //    labelPlayerID->setAnchorPoint(ccp(0, 0.5));
    //    labelPlayerID->setPosition(ccpAdd(labelPlayerLevel->getPosition(), ccp(80, 0)));
    //    spritePlayerImage->setAnchorPoint(ccp(0, 0.5));
    spritePlayerImage->setPosition(ccpAdd(sprite_line_1->getPosition(), ccp(-217, 65)));
    spritePlayerImageBG->setPosition(ccpAdd(spritePlayerImage->getPosition(),ccp(spritePlayerImage->getContentSize().width/2, 0)));
    CCSprite *cardbg = CCSprite::createWithSpriteFrameName("card_bj.png");
    cardbg->setPosition(spritePlayerImageBG->getPosition());
    
    std::string playernicknaem=_playerInfo->getNickName()->getCString();
    int flength=playernicknaem.length();
    
    
    unsigned int flen = flength / 3 + flength % 3;
    int fontSize =27;
    if (flen>=6) {
        fontSize=22;
    }
    labelPlayerName = SGCCLabelTTF::create("", FONT_PANGWA, fontSize);
    labelPlayerName->setAnchorPoint(ccp(0, 1));
    labelPlayerName->setPosition(ccpAdd(spritePlayerImageBG->getPosition(), ccp(60, spritePlayerImageBG->getContentSize().height*.5f)));
    
    SGCCLabelTTF* spritePlayerCountry = SGCCLabelTTF::create(str_FriendCountFull_str14,FONT_BOXINFO,28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //spritePlayerCountry->setColor(COLOR_YELLOW);
    labelPlayerCountry = SGCCLabelTTF::create(str_FateInfoItem_str7, FONT_BOXINFO, 28);
    spritePlayerCountry->setPosition(ccpAdd(spritePlayerImageBG->getPosition(), ccp(270, spritePlayerImageBG->getContentSize().height*.5 -spritePlayerCountry->getContentSize().height*.5f)));
    labelPlayerCountry->setAnchorPoint(ccp(0, 0.5));
    labelPlayerCountry->setPosition(ccpAdd(spritePlayerCountry->getPosition(), ccp(spritePlayerCountry->getContentSize().width/2 +15, 0)));
    
    SGCCLabelTTF *spritePlayerLevel = SGCCLabelTTF::create(str_deji,FONT_BOXINFO,28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //spritePlayerLevel->setColor(COLOR_YELLOW);
    spritePlayerLevel->setPosition(ccp(spritePlayerCountry->getPosition().x, spritePlayerImageBG->getPosition().y));
    
    labelPlayerLevel = SGCCLabelTTF::create(str_FriendCountFull_str15, FONT_BOXINFO, 28);
    labelPlayerLevel->setAnchorPoint(ccp(0, 0.5));
    labelPlayerLevel->setPosition(ccpAdd(spritePlayerLevel->getPosition(), ccp(spritePlayerLevel->getContentSize().width/2+10, 0)));
    
    SGCCLabelTTF *spritePlayerFamily = SGCCLabelTTF::create(str_FriendCountFull_str16,FONT_BOXINFO,28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //spritePlayerFamily->setColor(COLOR_YELLOW);
    SGCCLabelTTF *labelPlayerFamily = SGCCLabelTTF::create(str_FriendCountFull_str17, FONT_BOXINFO, 28);
    spritePlayerFamily->setPosition(ccpAdd(spritePlayerLevel->getPosition(), ccp(0, -54+14)));
    labelPlayerFamily->setAnchorPoint(ccp(0, 0.5));
    labelPlayerFamily->setPosition(ccpAdd(spritePlayerFamily->getPosition(), ccp(spritePlayerFamily->getContentSize().width/2+10, 0)));

    this->addChild(labelPlayerCountry);
    this->addChild(labelPlayerLevel);
    this->addChild(labelPlayerFamily);
    this->addChild(cardbg);
    this->addChild(labelPlayerName);
    this->addChild(spritePlayerCountry);
    this->addChild(spritePlayerFamily);
    this->addChild(spritePlayerImage);
    this->addChild(spritePlayerLevel);
    this->addChild(spritePlayerImageBG);
    
    //主将信息
    SGMainManager::shareMain()->addHeadIconPlistByNum(1001,sg_otherPlayerInfo);
    buttonOfficerHead = SGButton::create("head1001.png", "jinkuang.png", this, menu_selector(SGOtherPlayerInfo::buttonClickOfficerHead),CCPointZero,false,true);
    //    buttonOfficerHead->setAnchorPoint(ccp(0, 0.5));
    buttonOfficerHead->setPosition(ccp(spritePlayerImageBG->getPosition().x, sprite_line_2->getPosition().y + 65));
    SGCCLabelTTF *labelOfficer = SGCCLabelTTF::create(str_FriendCountFull_str18, FONT_PANGWA, 30);
    labelOfficer->setPosition(ccpAdd(buttonOfficerHead->getPosition(),ccp(120, buttonOfficerHead->getContentSize().height*.5f - 14)));
    labelOfficerName = SGCCLabelTTF::create(str_No , FONT_PANGWA, 30);
    labelOfficerName->setPosition(ccp(labelOfficer->getPosition().x,buttonOfficerHead->getPosition().y-2));
    
    labelOfficerLevel = SGCCLabelTTF::create(str_FriendCountFull_str15, FONT_BOXINFO, 30);
    labelOfficerLevel->setPosition(ccpAdd(labelOfficerName->getPosition(), ccp(0,-35)));
    
    CCSprite *cardbg1 = CCSprite::createWithSpriteFrameName("card_bj.png");
    cardbg1->setPosition(buttonOfficerHead->getPosition());
    this->addChild(cardbg1);
    this->addChild(labelOfficer);
    this->addChild(labelOfficerLevel);
    this->addChild(labelOfficerName);
    this->addBtn(buttonOfficerHead);
    
    //打开全部
    SGButton *buttonOpenAll = SGButton::createFromLocal("unhot.png", str_FriendCountFull_str19, this, menu_selector(SGOtherPlayerInfo::buttonClickOpenAll),CCPointZero,FONT_PANGWA,ccWHITE);
    buttonOpenAll->setAnchorPoint(ccp(1, 0.5));
    buttonOpenAll->setPosition(ccpAdd(labelOfficerName->getPosition(), ccp(280, 0)));
    //addBtn(buttonOpenAll);
    //如果是好友，那么就可以查看所有信息，如果不是，就不行了
    if(SGFRIEND_FRIEND != SGFriend::isFriendOfPlayer(_playerInfo->getRoleId()))
    {
        buttonOpenAll->setVisible(false);
    }
    
    //主将计
    SGCCLabelTTF *spriteMainOfficerSkill = SGCCLabelTTF::create(str_zhujiangji,FONT_BOXINFO,32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //spriteMainOfficerSkill->setColor(COLOR_YELLOW);
    spriteMainOfficerSkill->setAnchorPoint(ccp(0, 0.5));
    spriteMainOfficerSkill->setPosition(ccp(buttonOfficerHead->getPosition().x - buttonOfficerHead->getContentSize().width*.5f, sprite_line_2->getPosition().y-20));
    
    labelMainOfficerSkill = SGCCLabelTTF::create(str_EmbattleLayer_str15, FONT_BOXINFO, 30, CCSizeMake(450 , 200));
    labelMainOfficerSkill->setAnchorPoint(ccp(0,1));
    labelMainOfficerSkill->setPosition(ccpAdd(spriteMainOfficerSkill->getPosition(), ccp(5, -25)));
    
    this->addChild(spriteMainOfficerSkill);
    this->addChild(labelMainOfficerSkill);
    
    //被动技    
    SGCCLabelTTF *spriteOfficerSkill = SGCCLabelTTF::create(str_FirstLayer_str21,FONT_BOXINFO,32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //spriteOfficerSkill->setColor(COLOR_YELLOW);
    spriteOfficerSkill->setAnchorPoint(ccp(0, 0.5f));
    spriteOfficerSkill->setPosition(ccp(buttonOfficerHead->getPosition().x - buttonOfficerHead->getContentSize().width*.5f, sprite_line_3->getPosition().y-20));
    spriteOfficerSkill->setAnchorPoint(ccp(0, 0.5));
    
    labelOfficerSkill = SGCCLabelTTF::create(str_GeneralInfoEffect_str2, FONT_BOXINFO, 30, CCSizeMake(450, 200));
    labelOfficerSkill->setAnchorPoint(ccp(0, 1));
    labelOfficerSkill->setPosition(ccpAdd(spriteOfficerSkill->getPosition(), ccp(5, -40)));
    
    this->addChild(spriteOfficerSkill);
    this->addChild(labelOfficerSkill);
    
    
    
    SGCCLabelTTF *atk_ = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //atk_->setColor(COLOR_YELLOW);
    atk_->setPosition(ccpAdd(spriteOfficerSkill->getPosition(), ccp(atk_->getContentSize().width*.5f - 5,-140)));
    this->addChild(atk_);
    
    SGCCLabelTTF *def_ = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //def_->setColor(COLOR_YELLOW);
    def_->setPosition(ccpAdd(atk_->getPosition(), ccp(0,-43)));
    this->addChild(def_);
    
    SGCCLabelTTF *mor_ = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //mor_->setColor(COLOR_YELLOW);
    mor_->setPosition(ccpAdd(def_->getPosition(), ccp(0,-43)));
    this->addChild(mor_);
    
    SGCCLabelTTF *xuli_ = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , COLOR_YELLOW);
    xuli_->setPosition(ccpAdd(atk_->getPosition(), ccp(230, 0)));
    this->addChild(xuli_);
    //modify by:zyc. merge into create.
    //xuli_->setColor(COLOR_YELLOW);
    
    SGCCLabelTTF *sudu_ = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 28 , COLOR_YELLOW);
    sudu_->setPosition(ccpAdd(xuli_->getPosition(), ccp(0,-43)));
    this->addChild(sudu_);
    //modify by:zyc. merge into create.
    //sudu_->setColor(COLOR_YELLOW);
    CCScale9Sprite* font_bg = CCScale9Sprite::createWithSpriteFrameName("labelbg.png");
    font_bg->setPreferredSize(CCSize(215, 24));
    font_bg->setPosition(ccp(xuli_->getPosition().x + 70, mor_->getPosition().y));
//    this->addChild(font_bg);
    SGCCLabelTTF* font_tongyu = SGCCLabelTTF::create(str_shangzheng_need_tongyuli, FONT_BOXINFO, 26 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_tongyu->setColor(COLOR_YELLOW);
    font_tongyu->setPosition(ccp(font_bg->getPosition().x - 21, font_bg->getPosition().y));
    this->addChild(font_tongyu);
    
    labelAttack_1 = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    labelAttack_1->setAnchorPoint(ccp(1, 0.5));
    labelAttack_1->setPosition(ccpAdd(atk_->getPosition(), ccp(105, 0)));
    this->addChild(labelAttack_1);
    
    labelDefense_1 = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    labelDefense_1->setAnchorPoint(ccp(1, 0.5));
    labelDefense_1->setPosition(ccpAdd(def_->getPosition(), ccp(105, 0)));
    this->addChild(labelDefense_1);
    
    labelMorale_1 = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    labelMorale_1->setAnchorPoint(ccp(1, 0.5));
    labelMorale_1->setPosition(ccpAdd(mor_->getPosition(), ccp(105,0)));
    this->addChild(labelMorale_1);
    
    labelStoreUp = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    labelStoreUp->setAnchorPoint(ccp(1, 0.5));
    labelStoreUp->setPosition(ccp(xuli_->getPosition().x + 105, labelAttack_1->getPosition().y));
    this->addChild(labelStoreUp);
    
    labelSpeed = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    labelSpeed->setAnchorPoint(ccp(1, 0.5));
    labelSpeed->setPosition(ccp(xuli_->getPosition().x + 105, labelDefense_1->getPosition().y));
    this->addChild(labelSpeed);
    
    labelBattleForce = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    labelBattleForce->setAnchorPoint(ccp(0, 0.5));
    labelBattleForce->setPosition(ccp(font_tongyu->getPosition().x + font_tongyu->getContentSize().width*.5 + 7, font_tongyu->getPosition().y));
    this->addChild(labelBattleForce);
    
    //加为好友
//    CCLabelTTF *labelTipFriend = CCLabelTTF::create("你们还不是好友", FONT_PANGWA, 30);
//    SGButton *buttonAddFriend = NULL;
//    if(SGFriend::isFriendOfPlayer(_playerInfo->getRoleId()) == SGFRIEND_FRIEND)
//    {
//        labelTipFriend->setString("你们已经是好友了");
//        buttonAddFriend = SGButton::create("box_btnbg.png", "friend_sendMail.png", this, menu_selector(SGOtherPlayerInfo::buttonClickAddFriend),CCPointZero,false,true);
//
//        _isFriend = true;
//    }else{
//        labelTipFriend->setString("你们还不是好友");
//        buttonAddFriend = SGButton::create("box_btnbg.png", "friend_friend_1.png", this, menu_selector(SGOtherPlayerInfo::buttonClickAddFriend),CCPointZero,false,true);
//
//        _isFriend = false;
//    }
//    SGFindHelpLayer *sgfhl = SGFindHelpLayer::create();
//    
//    SGButton *buttongoto = SGButton::create("box_btnbg.png",
//                                              "public_font_queding.png", sgfhl, menu_selector(SGFindHelpLayer::tableViewSelectItem),CCPointZero,false,true);
//    buttongoto -> setTag(NULL);
//    this -> addBtn(buttongoto);
//    buttongoto -> setPosition(ccpAdd(center, ccp(-145, -382)));
    
    SGButton *buttonCancel = SGButton::create("box_btnbg.png",
                                              "public_font_fanhui.png", this, menu_selector(SGOtherPlayerInfo::buttonClickCancel),CCPointZero,false,true);
//    labelTipFriend->setColor(COLOR_ORANGE);
//    labelTipFriend->setPosition(ccpAdd(center, ccp(0, -300)));
//    buttonAddFriend->setPosition(ccpAdd(center, ccp(-145, -382)));
    buttonCancel->setPosition(ccpAdd(center, ccp(0, -415)));
    
//    this->addChild(labelTipFriend);
//    this->addBtn(buttonAddFriend);
    this->addBtn(buttonCancel);
    
    this->setPlayerInfo();
    this->setLordInfo();

}

void SGOtherPlayerInfo::setLordInfo()
{
    //设置数值
    int attackTotal = 0;
    int moraleTotal = 0;
    int defenseTotal = 0;
    int speedTotal = 0;
    int storeUpTotal = 0;
    int battleTotal = 0;
    
    SGOfficerCard *lordOfficer = _playerInfo->getLordOfficer();
    if(!lordOfficer)
    {
        CCLOG("没有主将");
        return;
    }
	 SGMainManager::shareMain()->addHeadIconPlistByNum(lordOfficer->getHeadSuffixNum(),sg_otherPlayerInfo);//12.06
    
    //主将信息
    labelOfficerLevel->setString(CCString::createWithFormat(str_Format_ji,lordOfficer->getCurrLevel())->getCString());
    labelOfficerName->setString(lordOfficer->getOfficerName()->getCString());
//    const char *fileName = CCString::createWithFormat("head%d.png",lordOfficer->getItemId())->getCString();
	
	const char *fileName = CCString::createWithFormat("head%d.png",lordOfficer->getHeadSuffixNum())->getCString();//12.06
    if(NULL != CCSprite::createWithSpriteFrameName(fileName))
    {
        buttonOfficerHead->setImage(fileName);
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(lordOfficer->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        buttonOfficerHead->setFontImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString())->displayFrame());
//        buttonOfficerHead->setFontImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString())->displayFrame());
        //if (str_1->isEqual(CCString::create("SGOfficerCard")))
        {
            buttonOfficerHead->removeChildByTag(30);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(buttonOfficerHead->getContentSize().width*0.3,buttonOfficerHead->getContentSize().height - countryName->getContentSize().height/4));
            buttonOfficerHead->addChild(countryName,30,30);
        }
    }
    
    //主将计与被动技
    int lordSkillId = lordOfficer->getLordId();
    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(lordSkillId);
    if (lordSkill) {
        labelMainOfficerSkill->setString(CCString::createWithFormat(str_FriendCountFull_str20,lordSkill->getSkillName()->getCString(),lordSkill->getLevel())->getCString());
    }
    
    int generalId = lordOfficer->getOfficerSkil();
    SGSkillDataModel *generalSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(generalId);
    if (generalSkill) {
        labelOfficerSkill->setString(CCString::createWithFormat(str_FriendCountFull_str21,generalSkill->getSkillName()->getCString(),generalSkill->getSkillMaxLevel())->getCString());
    }
    
    
    labelAttack_1->setString(CCString::createWithFormat("%d",lordOfficer->getAtk())->getCString());
    labelDefense_1->setString(CCString::createWithFormat("%d",lordOfficer->getDef())->getCString());
    labelStoreUp->setString(CCString::createWithFormat("%d",lordOfficer->getRound())->getCString());
    labelSpeed->setString(CCString::createWithFormat("%d",(int)lordOfficer->getSpeed())->getCString());
    labelMorale_1->setString(CCString::createWithFormat("%d",lordOfficer->getMorale())->getCString());
    labelBattleForce->setString(CCString::createWithFormat("%d",lordOfficer->getGoverning())->getCString());
    
    //重新设置位置
//    labelAttack_2->setPosition(ccpAdd(labelAttack_1->getPosition(), ccp(labelAttack_1->getContentSize().width+10, 0)));
//    labelStoreUp_2->setPosition(ccpAdd(labelStoreUp->getPosition(), ccp(labelStoreUp->getContentSize().width, 0)));
//    labelDefense_2->setPosition(ccpAdd(labelDefense_1->getPosition(), ccp(labelDefense_1->getContentSize().width+10, 0)));
//    labelSpeed_2->setPosition(ccpAdd(labelSpeed->getPosition(), ccp(labelSpeed->getContentSize().width+10, 0)));
//    labelMorale_2->setPosition(ccpAdd(labelMorale_1->getPosition(), ccp(labelMorale_1->getContentSize().width+10, 0)));
//    labelBattleForce_2->setPosition(ccpAdd(labelBattleForce->getPosition(), ccp(labelBattleForce->getContentSize().width, 0)));
    
    
    CCArray *arrayEquip = lordOfficer->getOfficerEquips();
    if(!arrayEquip)
    {
        return;
    }
    for(int i=0;i<arrayEquip->count();i++)
    {
        SGEquipCard *card = (SGEquipCard*)arrayEquip->objectAtIndex(i);
//        attackTotal += card->getAtk();
//        moraleTotal += card->getMorale();
//        defenseTotal += card->getDef();
        storeUpTotal += card->getRound();
//        speedTotal += card->getSpeed();
        battleTotal += card->getGoverning();
    }
    
    this->addLabel(labelAttack_1->getPosition(), attackTotal);
    this->addLabel(labelDefense_1->getPosition(), defenseTotal);
    this->addLabel(labelMorale_1->getPosition(), moraleTotal);
    this->addLabel(labelStoreUp->getPosition(), storeUpTotal);
    this->addLabel(labelSpeed->getPosition(), speedTotal);
//    labelAttack_2->setString(CCString::createWithFormat("+%d",attackTotal)->getCString());
//    labelDefense_2->setString(CCString::createWithFormat("+%d",defenseTotal)->getCString());
//    labelSpeed_2->setString(CCString::createWithFormat("+%d",speedTotal)->getCString());
//    labelMorale_2->setString(CCString::createWithFormat("+%d",moraleTotal)->getCString());
//    labelBattleForce_2->setString(CCString::createWithFormat("-%d",battleTotal)->getCString());
//    labelStoreUp_2->setString(CCString::createWithFormat("+%d",storeUpTotal)->getCString());

}

void SGOtherPlayerInfo::setPlayerInfo()
{

    
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);
	ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);
	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_otherPlayerInfo);

    labelPlayerName->setString(_playerInfo->getNickName()->getCString());
    if(_playerInfo->getCountry() == 1)
    {
        labelPlayerCountry->setString(str_FateInfoItem_str7);
    }else if(_playerInfo->getCountry() == 2)
    {
        labelPlayerCountry->setString(str_FateInfoItem_str8);
    }else if(_playerInfo->getCountry() == 3)
    {
        labelPlayerCountry->setString(str_FateInfoItem_str9);
    }else
    {
        labelPlayerCountry->setString(str_FriendCountFull_str22);
    }
    CCSprite *sprite_country = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",_playerInfo->getCountry())->getCString());
    spritePlayerImageBG->setDisplayFrame(sprite_country->displayFrame());
    
    
    labelPlayerLevel->setString(CCString::createWithFormat(str_Format_ji,_playerInfo->getLevel())->getCString());
//    labelPlayerID->setString(CCString::createWithFormat("(角色ID%d)",_playerInfo->getRoleId())->getCString());
    
    CCSprite *sprite_player = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("countryIcon_%d.png",_playerInfo->getImage())->getCString());
    if(sprite_player)
    {
//        spritePlayerImage->cocos2d::CCNode::setScale(0.75, 0.9);
        spritePlayerImage->setDisplayFrame(sprite_player->displayFrame());
        spritePlayerImage->setPosition(ccpAdd(spritePlayerImageBG->getPosition(),ccp(3, 10)));
    }
    
}

void SGOtherPlayerInfo::buttonClickOpenAll()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGGeneralsLayer *layer = SGGeneralsLayer::create(1);
    SGMainManager::shareMain()->showLayer(layer,false);
    
    this->boxClose();
}

void SGOtherPlayerInfo::buttonClickAddFriend()
{
    if(_isFriend)
    {
        SG_SHOW_WINDOW(str_FriendCountFull_str23);
        this->boxClose();
        
    }else{
        if(SGFriend::checkFriendIsSelf(_playerInfo->getRoleId()))
        {
            SG_SHOW_WINDOW(str_FriendCountFull_str24);
            return;
        }
        
        //记录添加好友的id
        SGFriendBase::addRoleId = _playerInfo->getRoleId();
        
        main::AddFriendRequest *add = new main::AddFriendRequest();
        add->set_friendroleid(_playerInfo->getRoleId());
        
        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_ADDFRIEND, add);
        this->boxClose();
    }
}

void SGOtherPlayerInfo::buttonClickCancel()
{
  
    
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}

void SGOtherPlayerInfo::buttonClickOfficerHead()
{
    EFFECT_PLAY(MUSIC_BTN);
    //不同的界面会都会跳转到那个武将界面，根据不同的情况，跳转不同的地方
    int friendType = SGFriend::isFriendOfPlayer(_playerInfo->getRoleId());
    int generalLayerType = 0;
    if(SGFRIEND_FRIEND == friendType)
    {
        generalLayerType = 100;
    }else if(SGFRIEND_BLACK == friendType)
    {
        generalLayerType = 5;
    }else if(SGFRIEND_REQUEST == friendType)
    {
        generalLayerType = 6;
    }else if(SGFRIEND_STRANGER == friendType)
    {
        generalLayerType = 4;
    }
    
    SGOfficerCard *officer = _playerInfo->getLordOfficer();
    
    SGGeneralInfoLayer *info = SGGeneralInfoLayer::create(officer, generalLayerType);
    SGMainManager::shareMain()->showLayer(info,false);
    this->boxClose();
}

void SGOtherPlayerInfo::boxClose()
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);

    }
    SGBaseBox::boxCloseWithOutCallBack();
}
void SGOtherPlayerInfo::addLabel(cocos2d::CCPoint ponit, int m_value)
{
    if (m_value >0) {
        SGCCLabelTTF *a = SGCCLabelTTF::create(CCString::createWithFormat("+%d",m_value)->getCString(), FONT_BOXINFO, 28 , COLOR_GREEN);
        this->addChild(a);
        a->setAnchorPoint(ccp(0, 0.5));
        //modify by:zyc. merge into create.
        //a->setColor(COLOR_GREEN);
        a->setPosition(ponit);
    }
    
}