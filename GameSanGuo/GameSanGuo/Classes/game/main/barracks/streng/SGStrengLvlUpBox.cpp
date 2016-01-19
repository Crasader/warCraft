//
//  SGStrengLvlUpBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-20.
//
//

#include "SGStrengLvlUpBox.h"
#include "SGMainManager.h"
#include "SGCCLabelTTF.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGStrengLvlUpBox::SGStrengLvlUpBox()
:new_card(NULL),
old_card(NULL),
cardType(0),
isfromfirst(0),
_delegate(NULL)
{
    ;
}
SGStrengLvlUpBox::~SGStrengLvlUpBox()
{
    new_card->release();
    old_card->release();
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_strengLvlUpBox);
}
SGStrengLvlUpBox *SGStrengLvlUpBox::create(SGBoxDelegate *bdg, SGBaseMilitaryCard *aftrecard, SGBaseMilitaryCard *beforecard,int type,int from)
{
    //@无处调用
    SGStrengLvlUpBox *LvlUpBox = new SGStrengLvlUpBox();
    if (LvlUpBox && LvlUpBox->init(bdg,aftrecard,beforecard,type))
    {
        LvlUpBox->isfromfirst = from;
        LvlUpBox->autorelease();
        return LvlUpBox;
    }
    CC_SAFE_DELETE(LvlUpBox);
    return NULL;
}
bool SGStrengLvlUpBox::init(SGBoxDelegate *bdg, SGBaseMilitaryCard *aftrecard, SGBaseMilitaryCard *beforecard,int type)
{
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_box_strengLvlUpBox, LIM_PNG_AS_PNG);
 
    float wid = 535;
    float hgt = 465;
    if (!SGBaseBox::init(bdg,box_strengTag,CCSizeMake(wid, hgt)))
    {
        return false;
    }
    cardType = type;
    new_card = aftrecard;
    new_card->retain();
    old_card = beforecard;
    old_card->retain();  
    
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_StrengLvIUpBox_str1, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    this->addChild(title);
    title->setPosition(biao->getPosition());
    
    CCString* notice = CCString::createWithFormat(str_StrengLvIUpBox_str2,new_card->getOfficerName()->getCString());
    SGCCLabelTTF* lab_gx = SGCCLabelTTF::create(notice->getCString(), FONT_BOXINFO, 30);
    lab_gx->setPosition(ccpAdd(biao->getPosition(), ccp(0,-65)));
    this->addChild(lab_gx);
    
    
    SGButton *ok = SGButton::create("box_btn_newbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGStrengLvlUpBox::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -425/2+20)));

    SGCCLabelTTF *font_lvl = SGCCLabelTTF::create(str_Class, FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(font_lvl);
    //modify by:zyc . merge into create.
    //font_lvl->setColor(COLOR_YELLOW);
    font_lvl->setPosition(ccpAdd(lab_gx->getPosition(), ccp(-95, -50)));
    
    SGCCLabelTTF *lvl_jt = SGCCLabelTTF::create("→", FONT_BOXINFO, 28);
    this->addChild(lvl_jt);
    lvl_jt->setPosition(ccpAdd(font_lvl->getPosition(), ccp(139, 0)));
    
    SGCCLabelTTF *currlvl = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,old_card->getCurrLevel())->getCString(), FONT_BOXINFO, 28);
    currlvl->setAnchorPoint(ccp(1, 0.5f));
    currlvl->setPosition(ccpAdd(lvl_jt->getPosition(), ccp(-6 - currlvl->getContentSize().width/2, 0)));
    this->addChild(currlvl);
    
    SGCCLabelTTF *afterlvl = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,new_card->getCurrLevel())->getCString(), FONT_BOXINFO, 28);
    afterlvl->setAnchorPoint(ccp(0, 0.5f));
    afterlvl->setPosition(ccpAdd(lvl_jt->getPosition(), ccp(6 + currlvl->getContentSize().width/2, 0)));
    this->addChild(afterlvl);
    
    for (int i = 0; i<5; i++)
    {
        SGCCLabelTTF *jiantou = SGCCLabelTTF::create("→", FONT_BOXINFO, 28);
        this->addChild(jiantou);
        jiantou->setPosition(ccpAdd(lvl_jt->getPosition(), ccp(0, -40 - 40*i)));
        
        SGCCLabelTTF *font_atk = SGCCLabelTTF::create("", FONT_BOXINFO, 28 , COLOR_YELLOW);
        this->addChild(font_atk);
        //modify by:zyc. merge into create.
        //font_atk->setColor(COLOR_YELLOW);
        font_atk->setPosition(ccpAdd(font_lvl->getPosition(), ccp(0, -40 - 40*i)));
        
        SGCCLabelTTF *beforelabel = SGCCLabelTTF::create("", FONT_BOXINFO, 28);
        beforelabel->setAnchorPoint(ccp(1, 0.5));
        this->addChild(beforelabel);
        beforelabel->setPosition(ccpAdd(jiantou->getPosition(), ccp(-14 - jiantou->getContentSize().width/2 , 0)));
        
        SGCCLabelTTF *label = SGCCLabelTTF::create("", FONT_BOXINFO, 28);
        label->setAnchorPoint(ccp(0, 0.5));
        this->addChild(label);
        label->setPosition(ccpAdd(jiantou->getPosition(), ccp(14 + jiantou->getContentSize().width/2 , 0)));
        
        if (i == 0) {
            font_atk->setString(str_attack);
            beforelabel->setString(CCString::createWithFormat("%d",old_card->getAtk())->getCString());
            label->setString(CCString::createWithFormat("%d",new_card->getAtk())->getCString());
        }else if (i == 1) {
            font_atk->setString(str_defense);
            beforelabel->setString(CCString::createWithFormat("%d",old_card->getDef())->getCString());
            label->setString(CCString::createWithFormat("%d",new_card->getDef())->getCString());
        }else if (i == 2) {
            font_atk->setString(str_xuli);
            beforelabel->setString(CCString::createWithFormat("%d",old_card->getRound())->getCString());
            label->setString(CCString::createWithFormat("%d",new_card->getRound())->getCString());
        }else if (i == 3) {
            font_atk->setString(str_speed);
            beforelabel->setString(CCString::createWithFormat("%d",(int)old_card->getSpeed())->getCString());
            label->setString(CCString::createWithFormat("%d",(int)new_card->getSpeed())->getCString());
        }else if (i == 4) {
            font_atk->setString(str_hp);
            beforelabel->setString(CCString::createWithFormat("%d",old_card->getMorale())->getCString());
            label->setString(CCString::createWithFormat("%d",new_card->getMorale())->getCString());
        }
        
    }

    return true;
}

void SGStrengLvlUpBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    if (isfromfirst ==4) {
         SGMainManager::shareMain()->showStrengLayer(new_card,cardType,4,true);
    }
    else if (new_card->getCurrLevel() == new_card->getMaxLevel())
    {
    }
    else
    {
    SGMainManager::shareMain()->showStrengLayer(new_card,cardType,isfromfirst);
    }
    SGGuideManager::shareManager()->checkIsDone(guide_tag_17);
}