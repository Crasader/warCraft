//
//  SGSoldierinfoEffect.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-17.
//
//

#include "SGSoldierinfoEffect.h"
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
//#include "SGStrengConfirm.h"
#include "SGSoldierInfoLayer.h"
#include "SGStringConfig.h"


SGSoldierinfoEffect::SGSoldierinfoEffect()
:
m_scrollview(NULL),
_dele(NULL),
_card(NULL),
enterType(0)
{


}

SGSoldierinfoEffect::~SGSoldierinfoEffect()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_soldierinfoEffect);
}

SGSoldierinfoEffect *SGSoldierinfoEffect::create(SGBoxDelegate *bdg, SGSoldierCard *card,int type)
{
    SGSoldierinfoEffect *skillInfoBox = new SGSoldierinfoEffect();
    skillInfoBox->_card = card;
//        if (skillInfoBox->_card) {
//            skillInfoBox->_card->retain();
//            skillInfoBox->_card->autorelease();
//        }
    skillInfoBox->_dele = bdg;
    
    if (skillInfoBox && skillInfoBox->init(bdg,card,type))
    {
        skillInfoBox->setTag(box_soldierInfoEffect);
        skillInfoBox->autorelease();
        return skillInfoBox;
    }
    CC_SAFE_DELETE(skillInfoBox);
    return NULL;
}




bool SGSoldierinfoEffect::init(SGBoxDelegate *bdg, SGSoldierCard *card,int type)
{
//    _card = card;
//    
//    if (_card) {
//        _card->retain();
//        _card->autorelease();
//    }

        CCSize s = CCDirector::sharedDirector()->getWinSize();
        CCPoint center = SGLayout::getPoint(kMiddleCenter);
        enterType = type;
    
//        _dele->setIsCanTouch(false);

        ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_box_soldierinfoEffect);
        ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_box_soldierinfoEffect);
        ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_box_soldierinfoEffect);
    
        ResourceManager::sharedInstance()->bindTexture("ofc_frame/ofc_frame.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    
        CCSize size = CCDirector::sharedDirector()->getWinSize();
    
        CCScale9Sprite *bg = CCScale9Sprite::createWithSpriteFrameName("ofc_bkg.png");
    
    
        bg->setPreferredSize(CCSizeMake(s.width*0.99, s.height*0.76));
        bg->setPosition (ccpAdd(center,ccp(0,size.height*0.01)));
        bg->setScale(1.05);
        this->addChild(bg);
    
        //灰图
        CCLayerColor* layer = CCLayerColor::create(ccc4(0, 0, 0, 180), skewing(320)*4, skewingY(960)*3);
        layer->ignoreAnchorPointForPosition(false);
    
        layer->setPosition(center);
        this->addChild(layer, -10);
    
        //技能 主将计 被动技三张图片
        CCSprite*jn=CCSprite::createWithSpriteFrameName("ofc_title_soldierskill.png");
        jn->setPosition(ccpAdd(center,ccp(0,bg->getContentSize().height*0.5) ));
        this->addChild(jn);
        CCSprite*zjj=CCSprite::createWithSpriteFrameName("ofc_title_chengzhang.png");
        zjj->setPosition(ccpAdd(center,ccp(-bg->getContentSize().width*0.3,bg->getContentSize().height*0.37) ));
        this->addChild(zjj);
        CCSprite*wjj=CCSprite::createWithSpriteFrameName("ofc_title_tianfu.png");
        wjj->setPosition(ccpAdd(center,ccp(-bg->getContentSize().width*0.3,-bg->getContentSize().height*0.05) ));
        this->addChild(wjj);
    
        SGButton *back = SGButton::create("box_btnbg.png",
                                          "public_font_queding.png",
                                          this,
                                          menu_selector(SGSoldierinfoEffect::backhandle),
                                          ccp(0, 0),
                                          false,
                                          true);
    
    
    
    //主将计label bg
    CCScale9Sprite *labelbg1 = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png");
    labelbg1->setPreferredSize(CCSizeMake(size.width*0.8, s.height*0.15));
     //labelbg1->setPosition (ccpAdd(center,ccp(0,size.height*0.1)));
   labelbg1->setPosition (ccpAdd(center,ccp(0,size.height*0.12)));
    this->addChild(labelbg1);
     //被动技label bg
    CCScale9Sprite *labelbg2 = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png");
    labelbg2->setPreferredSize(CCSizeMake(size.width*0.8, s.height*0.15));
    labelbg2->setPosition (ccpAdd(center,ccp(0,-size.height*0.18)));
    this->addChild(labelbg2);
    
    labelbg1->setVisible(false);
    labelbg2->setVisible(false);
    
        CCMenu*menu=CCMenu::create();
        menu->addChild(back);
        menu->setPosition(CCPointZero);
        bg->addChild(menu,3);
       back->setPosition (ccpAdd(center,ccp(0,-size.height*0.45)));
           //back->setPosition (center);
    
    //主将计
        CCScale9Sprite *spriteBorder1 = CCScale9Sprite::createWithSpriteFrameName("ofc_frame.png");
    
        spriteBorder1->setPreferredSize(CCSizeMake(s.width*0.8,s.height*0.27));
        spriteBorder1->setPosition( ccpAdd(center,ccp(-spriteBorder1->getContentSize().width*0.01, spriteBorder1->getContentSize().height*0.15)));
        bg->addChild(spriteBorder1);
         // 被动技
        CCScale9Sprite *spriteBorder2 = CCScale9Sprite::createWithSpriteFrameName("ofc_frame.png");
    
        spriteBorder2->setPreferredSize(CCSizeMake(s.width*0.8,s.height*0.27));
        spriteBorder2->setPosition( ccpAdd(center,ccp(-spriteBorder1->getContentSize().width*0.01, -spriteBorder1->getContentSize().height*0.95)));
        bg->addChild(spriteBorder2);
    
    spriteBorder1->setVisible(false);
    spriteBorder2->setVisible(false);
        //主将计强化
    
        SGButton *qianhua1 = SGButton::create("hotBtnBg.png",
                                              "font_streng.png",
                                              this,
                                              menu_selector(SGSoldierinfoEffect::strengHandlerzjj),
                                              ccp(0, 0),
                                              false,
                                              true);
    
    qianhua1->setPosition( ccpAdd(labelbg1->getPosition(),ccp(labelbg1->getContentSize().width*0.3, labelbg1->getContentSize().height*0)));

    
        // 被动技强化
        SGButton *qianhua2 = SGButton::create("hotBtnBg.png",
                                              "font_streng.png",
                                              this,
                                              menu_selector(SGSoldierinfoEffect::strengHandlerwjj),
                                              ccp(0, 0),
                                              false,
                                              true);
     qianhua2->setPosition( ccpAdd(labelbg2->getPosition(),ccp(labelbg2->getContentSize().width*0.3, labelbg2->getContentSize().height*0)));
//        menu->addChild(qianhua1);
//        menu->addChild(qianhua2);
 
    
        //读取成长技
    int growid = SGStaticDataManager::shareStatic()->getSoldierGrowSkillid(_card->getCurrLevel(),_card->getItemId());
    SGSoldierSkillDataModel *growskill = SGStaticDataManager::shareStatic()->getSoldierSkillById(growid);
//    if (enterType ==16 ||enterType ==20) {
//         lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
//    }
//    else
//    {
//    lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
//    }
        SGCCLabelTTF *title = SGCCLabelTTF::create("", FONT_PANGWA, 36);
        title->setAnchorPoint(ccp(0,0.5));
        title->setPosition( ccpAdd(zjj->getPosition(),ccp(-zjj->getContentSize().width/2, -qianhua1->getContentSize().height*0.5)));
    this->addChild(title);

        if (growskill) {
             title->setString(growskill->getSkillName()->getCString());
            CCSprite* label11=SGRichLabel::getFomLabel(growskill->getSkillInfo()->getCString(), CCSizeMake(size.width*0.78, size.height*0.2),FONT_BOXINFO, 28);
            this->addChild(label11);
            label11->setPosition(ccpAdd(labelbg1->getPosition(),ccp(0, -labelbg1->getContentSize().height*0.35)));
            
            
            CCString *str=CCString::createWithFormat("Lv%d",growskill->getSkillLevel());
            SGCCLabelTTF*lv = SGCCLabelTTF::create( str->getCString(), FONT_BOXINFO, 32);
            lv->setAnchorPoint(ccp(0,0.5));
            lv->setPosition(ccpAdd(title->getPosition(),ccp(zjj->getContentSize().width*0.75+title->getContentSize().width,0 )));
            this->addChild(lv);
        }else{
            title->setString(str_wu_chengzhangji);
            qianhua1->setVisible(false);
            qianhua1->setEnabled(false);
            SGCCLabelTTF*a = SGCCLabelTTF::create(str_SoldiersInfoLayer, FONT_BOXINFO, 32);
            this->addChild(a);
           a->setPosition(ccpAdd(center,ccp(0, qianhua1->getContentSize().height*1.5)));
        }

    
    //读取天赋技
    
    
     SGCCLabelTTF *title2 = SGCCLabelTTF::create("", FONT_PANGWA, 32);
    title2->setAnchorPoint(ccp(0,0.5));
    title2->setPosition( ccpAdd(wjj->getPosition(),ccp(-wjj->getContentSize().width/2, -qianhua1->getContentSize().height*0.5)));
    this->addChild(title2);
	/////////////////////////////////////////////////////////////
    SGSoldierSkillDataModel *talentSkill= SGStaticDataManager::shareStatic()->getSoldierSkillById(_card->getsoldiertalentskill());

    if (talentSkill) {
        title2->setString(talentSkill->getSkillName()->getCString());
//        title2->setAnchorPoint(ccp(0,0.5));
//        title2->setPosition( ccpAdd(wjj->getPosition(),ccp(-wjj->getContentSize().width/2, -qianhua1->getContentSize().height*0.5)));

        CCSprite* label22=SGRichLabel::getFomLabel(talentSkill->getSkillInfo()->getCString(), CCSizeMake(size.width*0.78, size.height*0.2),FONT_BOXINFO, 28);
        
       label22->setPosition(ccpAdd(labelbg2->getPosition(),ccp(0, -labelbg2->getContentSize().height*0.35)));
//        CCString *str1=CCString::createWithFormat("Lv%s",officerSkill->getSkillClientLevel()->getCString());
//        SGCCLabelTTF*lv = SGCCLabelTTF::create( str1->getCString(), FONT_BOXINFO, 32 );
//        lv->setPosition(ccpAdd(a1->getPosition(),ccp(wjj->getContentSize().width*0.75,0 )));
//        this->addChild(lv);
//        this->addChild(a1);
        this->addChild(label22);
//        this->addChild(title2);
        
        
    }else{
        title2->setString(str_wu_tianfuji);
//        title2->setPosition( ccpAdd(wjj->getPosition(),ccp(0, -qianhua1->getContentSize().height*0.5)));
//        this->addChild(title2);
        qianhua2->setVisible(false);
        qianhua2->setEnabled(false);
        SGCCLabelTTF*a1 = SGCCLabelTTF::create(str_SoldiersInfoLayer_str1, FONT_BOXINFO, 32);
         a1->setPosition(ccpAdd(center,ccp(0, -qianhua1->getContentSize().height*2.2)));
        this->addChild(a1);
      //  a1->setPosition(ccpAdd(center, ccp(0, 100)));
    }

    //100=好友，20=商城，16=卡牌图鉴，4=找人帮忙,7=强化 ,1111聊天展示
    if(type==100 || type == 20 ||type == 16 || type == 4 ||type == 7 || type== CHATSHOWENTRYTAG )
    {
        qianhua1->setVisible(false);
        qianhua1->setEnabled(false);
        qianhua2->setVisible(false);
        qianhua2->setEnabled(false);
    }

    CCScaleTo *scale1=CCScaleTo::create(0.1,1.15);
    CCScaleTo *scale2=CCScaleTo::create(0.1,0.95);
    CCScaleTo *scale3=CCScaleTo::create(0.1,1.0);
    //CCScaleBy *scale2=CCScaleBy::create(2,0.5);
    CCSequence*action=CCSequence::create(scale1,scale2,scale3,NULL);
    this->runAction(action);
    
    
     //  CCSprite* label2=SGRichLabel::getFomLabel(str, CCSizeMake(480,354),FONT_BOXINFO, 32,ccRED);
    
       return true;
    

}



void SGSoldierinfoEffect::strengHandlerzjj()
{
    EFFECT_PLAY(MUSIC_BTN);
    
////            SGMainManager::shareMain()->showStrengLayer(_card, 1,false,true);
//    
//    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
//    if (lordSkill && lordSkill->getNextId()==0)
//    {
//        SG_SHOW_WINDOW("等级已达上限");
//    }
//    else if (lordSkill && lordSkill->getLevel()==_card->getLordMaxLevel())
//    {
//        SG_SHOW_WINDOW("等级已达上限，请提升武将星级");
//    }
//    else
//    {
//    this->boxClose();
//    SGStrengConfirm *effect = SGStrengConfirm::create(_dele, _card,2);
//    SGMainManager::shareMain()->showBox(effect);
//    }
}




void SGSoldierinfoEffect::strengHandlerwjj()
{
    
//   SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
//    if (officerSkill && officerSkill->getNextId()==0)
//    {
//        SG_SHOW_WINDOW("等级已达上限");
//    }
//    else if (officerSkill && officerSkill->getSkillMaxLevel()==_card->getSkillMaxLevel())
//    {
//        SG_SHOW_WINDOW("等级已达上限，请提升武将星级");
//    }
//    else
//    {
//    //SGMainManager::shareMain()->showChooseSkillLayer();
//   // this->boxClose();
//    
////    SGMainManager::shareMain()->showStrengLayer(_card, 4,false);
//
//       this->boxClose();
//    SGStrengConfirm *effect = SGStrengConfirm::create(_dele, _card,1);
//    SGMainManager::shareMain()->showBox(effect);
//    }

}

void SGSoldierinfoEffect::backhandle(CCObject*obj)
{
    
    
    SGSoldierInfoLayer *firstLayer = (SGSoldierInfoLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_soldierInfoLayer);
    
    if (firstLayer) {
        firstLayer->setIsCanTouch(true);
    }
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    
    this->boxClose();
    
    
}

