//
//  SGStrengConfirm.cpp
//  GameSanGuo
//
//  Created by kanata on 13-11-15.
//
//

#include "SGStrengConfirm.h"
#include "SGBaseLayer.h"
#include "ResourceManager.h"
#include "SGMainScene.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGFirstLayer.h"
#include "SGGeneralInfoLayer.h"
#include "SGCantAdvanceBox.h"
#include "SGStaticDataManager.h"
#include "SGSkillDataModel.h"
#include "SGRichLabel.h"
#include "SGStringConfig.h"


SGStrengConfirm::SGStrengConfirm():
_dele(NULL),
_card(NULL),
enterType(0)
{


}

SGStrengConfirm::~SGStrengConfirm()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_strengBox);
}

SGStrengConfirm *SGStrengConfirm::create(SGBoxDelegate *bdg, SGOfficerCard *card,int type)
{
    SGStrengConfirm *skillInfoBox = new SGStrengConfirm();
    skillInfoBox->_card = card;
    skillInfoBox->_dele = bdg;
    
    if (skillInfoBox && skillInfoBox->init(bdg,card,type))
    {
        skillInfoBox->autorelease();
        return skillInfoBox;
    }
    CC_SAFE_DELETE(skillInfoBox);
    return NULL;
}




bool SGStrengConfirm::init(SGBoxDelegate *bdg, SGOfficerCard *card,int type)
{
        CCPoint center = SGLayout::getPoint(kMiddleCenter);
        enterType = type;

    if (!SGBaseBox::init(bdg,box_confirm,CCSizeMake(600, 800),false,true))//!SGBaseBox::init(bdg,box_strengTag,CCSizeMake(535, 635),false,true,CCSizeMake(535, 125)))
    {
        return false;
    }
    
        ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_box_strengBox);
//
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_StrengConfiirm_str1,FONT_XINGKAI,34);
//    title->setColor(ccc3(0xff,0xc8, 0x07));
    title->setPosition(ccpAdd(center,ccp(0,title->getContentSize().height*8.5)));
    this->addChild(title);
    if (enterType ==2) {
        title->setString(str_StrengConfiirm_str2);
    }
    SGMainManager::shareMain()->addHeadIconPlistByNum(card->getHeadSuffixNum(), sg_generalInfoLayer);
    SGMainManager::shareMain()->addOfficerPng(card->getHeadSuffixNum(), sg_generalInfoLayer);
    CCString* str = CCString::createWithFormat("officer_%d.png",card->getHeadSuffixNum());
    
    CCSprite* bigbg = CCSprite::createWithSpriteFrameName("pad武将底图.png");
    this->addChild(bigbg,0);
    bigbg->setScale(500/bigbg->getContentSize().width);
    bigbg->setPosition(ccpAdd(title->getPosition(), ccp(0, -bigbg->getContentSize().height*.38)));


    CCSprite *officer = CCSprite::createWithSpriteFrameName(str->getCString());
    officer->setAnchorPoint(ccp(0.5,0));
    officer->setPosition(ccpAdd(title->getPosition(),ccp(0,-50-officer->getContentSize().height)));
    officer->setScale(.9);
    this->addChild(officer);
    
    CCSprite* tiao = CCSprite::createWithSpriteFrameName("streng_tiao.png");
    tiao->setPosition(ccpAdd(center,ccp(0,0)));
    this->addChild(tiao,1);
    
    CCString *strengnamebefore = CCString::create("");
    CCString *strengnameafter = CCString::create("");
    if (enterType==2) {
        SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
        if (lordSkill) {
           strengnamebefore->setString(lordSkill->getSkillName()->getCString());
            SGSkillDataModel *lordSkilla = SGStaticDataManager::shareStatic()->getLordSkillById(lordSkill->getNextId());
            if (lordSkilla) {
                strengnameafter->setString(lordSkilla->getSkillName()->getCString());
            }
            
        }

        
    }
    else
    {
        SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
        if (officerSkill) {
        strengnamebefore->setString(officerSkill->getSkillName()->getCString());
        SGSkillDataModel *officerSkilla = SGStaticDataManager::shareStatic()->getGeneralSkillById(officerSkill->getNextId());
            if (officerSkilla) {
                strengnameafter->setString(officerSkilla->getSkillName()->getCString());
            }
        
        }
    }
    SGCCLabelTTF *jibefore = SGCCLabelTTF::create(strengnamebefore->getCString(), FONT_XINGKAI,34 , ccc3(0x54, 0x00, 0x19) , ccWHITE);
    this->addChild(jibefore,1);
    jibefore->setPosition(ccpAdd(center,ccp(-160,0)));
    
    
    SGCCLabelTTF *lvbefore = SGCCLabelTTF::create("", FONT_XINGKAI,28);
    SGCCLabelTTF *lvafter = SGCCLabelTTF::create("", FONT_XINGKAI,28);
    CCString *strlv1 = CCString::create("");
    CCString *strlv2 = CCString::create("");
    
    if (enterType==2) {
        SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
        if (lordSkill) {
            strlv1 = CCString::createWithFormat("Lv%d",lordSkill->getLevel());
            SGSkillDataModel *lordSkilla = SGStaticDataManager::shareStatic()->getLordSkillById(lordSkill->getNextId());
            if (lordSkilla) {
                strlv2 = CCString::createWithFormat("Lv%d",lordSkilla->getLevel());
            }
            
        }
        
        lvbefore ->setString(strlv1->getCString());
        lvafter ->setString(strlv2->getCString());

    }
    else
    {
        SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
        if (officerSkill) {
           strlv1 =CCString::createWithFormat("Lv%s",officerSkill->getSkillClientLevel()->getCString());
           SGSkillDataModel *officerSkilla = SGStaticDataManager::shareStatic()->getGeneralSkillById(officerSkill->getNextId());
            if (officerSkilla) {
                strlv2 =CCString::createWithFormat("Lv%s",officerSkilla->getSkillClientLevel()->getCString());
            }
           
        }
        lvbefore ->setString(strlv1->getCString());
        lvafter ->setString(strlv2->getCString());
    
    }

    this->addChild(lvbefore,1);
    lvbefore->setPosition(ccpAdd(jibefore->getPosition(),ccp(jibefore->getContentSize().width*.5+2+lvbefore->getContentSize().width*.5,0)));
    
    SGCCLabelTTF *jiafter = SGCCLabelTTF::create(strengnameafter->getCString(), FONT_XINGKAI,34 , ccc3(0x09, 0xc7, 0x00) , ccWHITE);
    this->addChild(jiafter,1);
    jiafter->setPosition(ccpAdd(center,ccp(100,0)));
    
    this->addChild(lvafter,1);
    lvafter->setPosition(ccpAdd(jiafter->getPosition(),ccp(jiafter->getContentSize().width*.5+2+lvafter->getContentSize().width*.5,0)));
   
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);

    CCSprite *infobg1 = CCSprite::createWithSpriteFrameName("tipsinside.png");
    infobg1->setAnchorPoint(ccp(0.5,1));
    infobg1->setScale(.5);
    infobg1->setScaleY(.55);
    infobg1->setPosition(ccpAdd(center,ccp(-140, -tiao->getContentSize().height*.4)));
    this->addChild(infobg1,0);
    
    CCSprite *infobg2 = CCSprite::createWithSpriteFrameName("tipsinside.png");
    infobg2->setAnchorPoint(ccp(0.5,1));
    infobg2->setScale(.5);
    infobg2->setScaleY(.55);
    infobg2->setPosition(ccpAdd(center,ccp(140, -tiao->getContentSize().height*.4)));
    this->addChild(infobg2,0);
   
    CCString *infostr = CCString::create("");
    CCString *infostr2 = CCString::create("");
    
    if (enterType==2) {
        SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
        if (lordSkill) {
            infostr ->setString(lordSkill->getSkillInfo()->getCString());
            SGSkillDataModel *lordSkilla = SGStaticDataManager::shareStatic()->getLordSkillById(lordSkill->getNextId());
            if (lordSkilla) {
               infostr2->setString(lordSkilla->getSkillInfo()->getCString());
            }
            
        }
        else
        {
            infostr->setString(str_EmbattleLayer_str15);
             infostr2->setString(str_EmbattleLayer_str15);
        }
    
    }
    else
    {
        SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
        if (officerSkill) {
            infostr ->setString(officerSkill->getSkillInfo()->getCString());
             SGSkillDataModel *officerSkilla = SGStaticDataManager::shareStatic()->getGeneralSkillById(officerSkill->getNextId());
            if(officerSkilla)
            infostr2->setString(officerSkilla->getSkillInfo()->getCString());

                  }
        else
        {
            infostr->setString(str_StrengConfiirm_str3);
            infostr2->setString(str_StrengConfiirm_str3);
        }

        
    }

    
    CCLabelTTF *info1=CCLabelTTF::create(infostr->getCString(),FONT_BOXINFO, 28, CCSizeMake(200,300), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    info1->setColor(ccc3(0x35,0x14,0x03));
    info1->setPosition(ccpAdd(infobg1->getPosition(),ccp(0,-15)));
    info1->setAnchorPoint(ccp(0.5,1));
    this->addChild(info1,10);
    
    CCLabelTTF *info2=CCLabelTTF::create(infostr2->getCString(),FONT_BOXINFO, 28, CCSizeMake(200,300), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    info2->setColor(ccc3(0x0f,0x79,0x01));
    info2->setPosition(ccpAdd(infobg2->getPosition(),ccp(0,-15)));
    info2->setAnchorPoint(ccp(0.5,1));
    this->addChild(info2,10);

    
    
    
    
    CCSprite *jt = CCSprite::createWithSpriteFrameName("streng_jt.png");
    jt->setPosition(ccpAdd(center,ccp(0,-140)));
    this->addChild(jt);
    
    SGButton *cancel = SGButton::create("box_btnbg.png", "public_font_quxiao.png", this, menu_selector(SGStrengConfirm::backhandle),CCPointZero,false,true);
    cancel->setPosition(ccpAdd(center,ccp(140,-340)));
    this->addBtn(cancel);
    
    SGButton *streng = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGStrengConfirm::strenghandel),CCPointZero,false,true);
    streng->setPosition(ccpAdd(center,ccp(-140,-340)));
    this->addBtn(streng);

//    this->setstar(_card->getCurrStar(), _card->getUpgradestar());
    this->setstar(3 , 3);
    return true;
    

}

void SGStrengConfirm::strenghandel(CCObject*obj)
{
    if (enterType==2) {
        strengHandlerzjj();
    }
    else
    {
        strengHandlerwjj();
    }

}

void SGStrengConfirm::strengHandlerzjj()
{
    EFFECT_PLAY(MUSIC_BTN);
//    if (enterType == 1)
//    {
//        if (_card->getLordSkill()) {
    
            
            SGMainManager::shareMain()->showStrengLayer(_card, 1,false,true);
            this->boxClose();
//        }
//    }else
//    {
//        this->boxClose();
//        
//        SGMainManager::shareMain()->showStrengLayer(_card, 4,false);
//    }
}




void SGStrengConfirm::strengHandlerwjj()
{
    
    
    
    //SGMainManager::shareMain()->showChooseSkillLayer();
   // this->boxClose();
    
    SGMainManager::shareMain()->showStrengLayer(_card, 4,false);

       this->boxClose();

}

void SGStrengConfirm::backhandle(CCObject*obj)
{
    
    
    SGGeneralInfoLayer *firstLayer = (SGGeneralInfoLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_generalInfoLayer);
    
    if (firstLayer) {
        firstLayer->setIsCanTouch(true);
    }
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
    
    
    this->boxClose();
    
    
}

void SGStrengConfirm::setstar(int star,int max)
{
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    
        //放空的星星
        CCSprite *maxstar = NULL;
        for(int i=0; i < max; i++)
        {
            maxstar = CCSprite::createWithSpriteFrameName("blackstar.png");
            maxstar->setAnchorPoint(ccp(0.5, 0.5));
            maxstar->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(maxstar->getContentSize().width/2 + 14 + maxstar->getContentSize().width * i-255, +310)));
            this->addChild(maxstar,10,i);
            //            currstar->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(currstar->getContentSize().width/2 + 10 + currstar->getContentSize().width * i, -currstar->getContentSize().height/2 - title_bg->getContentSize().height - 5)));
            if (i<star) {
                maxstar->setDisplayFrame(CCSprite::createWithSpriteFrameName("star.png")->displayFrame());
                maxstar->setPosition(ccpAdd(maxstar->getPosition(),ccp(0,-1)));
            }
        }
    
        
        
        
//        //放满留6个星星，控制显隐
//        ResourceManager::sharedInstance()->bindTexture("animationFile/staranimation.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
//		ResourceManager::sharedInstance()->bindTexture("animationFile/bsxx.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
//		ResourceManager::sharedInstance()->bindTexture("animationFile/btnhuabiananim.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
//		ResourceManager::sharedInstance()->bindTexture("animationFile/wjzs.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
//        CCSprite *currstar = NULL;
//        for(int i=0; i < HELLO_MAX_STAR; i++)
//        {
//            currstar = CCSprite::createWithSpriteFrameName("star.png");
//            currstar->setAnchorPoint(ccp(0.5, 0));
//            //            currstar->setPosition(ccp(currstar->getContentSize().width/2 + 5 + currstar->getContentSize().width * i, +5));
//            
//            
//            CCSpriterX* fermEffect = CCSpriterX::create("animationFile/staranimation.scml", true, true);
//            fermEffect->setanimaID(0);
//            fermEffect->setisloop(true);
//            currstar->addChild(fermEffect);
//            fermEffect->setPosition(ccp(currstar->getContentSize().width*.5, currstar->getContentSize().height*.5 +1));
//            fermEffect->play();
//           
//        }
    
        
    
    
    
}
