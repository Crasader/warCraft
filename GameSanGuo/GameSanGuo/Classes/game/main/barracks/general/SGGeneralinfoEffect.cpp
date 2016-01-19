//
//  SGGeneralinfoEffect.cpp
//  GameSanGuo
//
//  Created by kanata on 13-11-15.
//
//

#include "SGGeneralinfoEffect.h"
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
#include "SGStrengConfirm.h"
#include "SGStringConfig.h"


SGGeneralinfoEffect::SGGeneralinfoEffect():
_dele(NULL),
_card(NULL),
enterType(0)
{


}

SGGeneralinfoEffect::~SGGeneralinfoEffect()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_generalinfoEffect);
}

SGGeneralinfoEffect *SGGeneralinfoEffect::create(SGBoxDelegate *bdg, SGOfficerCard *card,int type)
{
    SGGeneralinfoEffect *skillInfoBox = new SGGeneralinfoEffect();
    skillInfoBox->_card = card;
//        if (skillInfoBox->_card) {
//            skillInfoBox->_card->retain();
//            skillInfoBox->_card->autorelease();
//        }
    skillInfoBox->_dele = bdg;
    
    if (skillInfoBox && skillInfoBox->init(bdg,card,type))
    {
        skillInfoBox->setTag(box_generalinfoEffect);
        skillInfoBox->autorelease();
        return skillInfoBox;
    }
    CC_SAFE_DELETE(skillInfoBox);
    return NULL;
}




bool SGGeneralinfoEffect::init(SGBoxDelegate *bdg, SGOfficerCard *card,int type)
{

        CCSize s = CCDirector::sharedDirector()->getWinSize();
        CCPoint center = SGLayout::getPoint(kMiddleCenter);
        enterType = type;

    
        ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_box_generalinfoEffect);
        ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_box_generalinfoEffect);
        ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_box_generalinfoEffect);
    
        ResourceManager::sharedInstance()->bindTexture("tran/tran.plist", RES_TYPE_LAYER_UI, sg_box_generalinfoEffect);
    
        ResourceManager::sharedInstance()->bindTexture("ofc_frame_new/ofc_frame_new.plist", RES_TYPE_LAYER_UI, sg_box_generalinfoEffect);
    
        CCSize size = CCDirector::sharedDirector()->getWinSize();
    
        CCScale9Sprite *bg = CCScale9Sprite::createWithSpriteFrameName("ofc_bkg.png");
    
        bg->setPreferredSize(CCSizeMake(s.width*0.99, s.height*0.76));
        bg->setPosition (ccpAdd(center,ccp(0,size.height*0.01)));
        bg->setScale(0.95);
        this->addChild(bg);
    
        //灰图
        CCLayerColor* layer = CCLayerColor::create(ccc4(0, 0, 0, 180), skewing(320)*4, skewingY(960)*3);
        layer->ignoreAnchorPointForPosition(false);
    
        layer->setPosition(center);
        this->addChild(layer, -10);
    
        //技能 主将计 被动技三张图片
        CCSprite*jn=CCSprite::createWithSpriteFrameName("ofc_title_jn.png");
        jn->setPosition(ccpAdd(center,ccp(0,bg->getContentSize().height*0.5 - 45) ));
        this->addChild(jn);
        CCSprite*zjj=CCSprite::createWithSpriteFrameName("ofc_title_zjj.png");  //主将计
        zjj->setPosition(ccpAdd(center,ccp(-bg->getContentSize().width*0.3 + 40,bg->getContentSize().height*0.37 -18) ));
        this->addChild(zjj);
        CCSprite*wjj=CCSprite::createWithSpriteFrameName("ofc_title_wjj.png");
        wjj->setPosition(ccpAdd(center,ccp(-bg->getContentSize().width*0.3 + 40,-bg->getContentSize().height*0.05 + 20) ));
        this->addChild(wjj);
    
        SGButton *back = SGButton::create("box_btn_newbg.png",
                                          "public_font_queding.png",
                                          this,
                                          menu_selector(SGGeneralinfoEffect::backhandle),
                                          ccp(0, 0),
                                          false,
                                          true);
    
    
    
    //主将计label bg
    CCScale9Sprite *labelbg1 = CCScale9Sprite::createWithSpriteFrameName("tran.png");
    labelbg1->setPreferredSize(CCSizeMake(size.width*0.8, s.height*0.15));
    labelbg1->setPosition (ccpAdd(center,ccp(0,size.height*0.12)));
    this->addChild(labelbg1);
    CCScale9Sprite *labelbg2 = CCScale9Sprite::createWithSpriteFrameName("tran.png");
    labelbg2->setPreferredSize(CCSizeMake(size.width*0.8, s.height*0.15));
    labelbg2->setPosition (ccpAdd(center,ccp(0,-size.height*0.18)));
    this->addChild(labelbg2);
        CCMenu*menu=CCMenu::create();
        menu->addChild(back);
        menu->setPosition(CCPointZero);
        bg->addChild(menu,3);
       back->setPosition (ccpAdd(center,ccp(0,-size.height*0.45)));
        //主将计
        CCScale9Sprite *spriteBorder1 = CCScale9Sprite::createWithSpriteFrameName("ofc_frame_new.png");
    
        spriteBorder1->setPreferredSize(CCSizeMake(s.width*0.8,s.height*0.27));
        spriteBorder1->setPosition( ccpAdd(center,ccp(-spriteBorder1->getContentSize().width*0.01, spriteBorder1->getContentSize().height*0.15)));
        bg->addChild(spriteBorder1);
         // 被动技
        CCScale9Sprite *spriteBorder2 = CCScale9Sprite::createWithSpriteFrameName("ofc_frame_new.png");
    
        spriteBorder2->setPreferredSize(CCSizeMake(s.width*0.8,s.height*0.27));
        spriteBorder2->setPosition( ccpAdd(center,ccp(-spriteBorder1->getContentSize().width*0.01, -spriteBorder1->getContentSize().height*0.95)));
        bg->addChild(spriteBorder2);
        //主将计强化
    
        SGButton *qianhua1 = SGButton::create("generalInfoBtnBg.png",
                                              "font_streng.png",
                                              this,
                                              menu_selector(SGGeneralinfoEffect::strengHandlerzjj),
                                              ccp(0, 0),
                                              false,
                                              true);
       // qianhua1->setPosition( ccpAdd(center,ccp(qianhua1->getContentSize().width*1.1, qianhua1->getContentSize().height*1.5)));
    qianhua1->setPosition( ccpAdd(labelbg1->getPosition(),ccp(labelbg1->getContentSize().width*0.3, labelbg1->getContentSize().height*0)));
   // qianhua1->setPosition(center);
    
        // 被动技强化
        SGButton *qianhua2 = SGButton::create("generalInfoBtnBg.png",
                                              "font_streng.png",
                                              this,
                                              menu_selector(SGGeneralinfoEffect::strengHandlerwjj),
                                              ccp(0, 0),
                                              false,
                                              true);

     qianhua2->setPosition( ccpAdd(labelbg2->getPosition(),ccp(labelbg2->getContentSize().width*0.3, labelbg2->getContentSize().height*0)));
        menu->addChild(qianhua1);
        menu->addChild(qianhua2);
    
    if (enterType==25 || enterType ==24 || enterType == 26|| enterType == 27 || enterType == 28) {
        qianhua1->setVisible(false);
        qianhua2->setVisible(false);
        
    }
    
        //读取主将计
    SGSkillDataModel *lordSkill = NULL;
    if (enterType ==16 ||enterType ==20 || enterType ==25 ||enterType ==1111)
    {
        lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordSkill());
    }
    else
    {
        lordSkill=SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
    }
    SGCCLabelTTF *title = SGCCLabelTTF::create("", FONT_PANGWA, 36);
    

    if (lordSkill)
    {

        SGCCLabelTTF *label_title = SGCCLabelTTF::create(lordSkill->getSkillName()->getCString(), FONT_BOXINFO, 34);

        label_title->setAnchorPoint(ccp(0, 0.5));
       label_title->setPosition(ccpAdd(ccp(zjj->getPositionX()-zjj->getContentSize().width*0.5,zjj->getPositionY()),ccp(0, -qianhua1->getContentSize().height*0.5 -14)));

        CCSprite* label11=SGRichLabel::getFomLabel(lordSkill->getSkillInfo()->getCString(), CCSizeMake(size.width*0.78* 0.72, size.height*0.2),FONT_BOXINFO, 32);
        this->addChild(label11);
          label11->setPosition(ccpAdd(labelbg1->getPosition(),ccp(0, -labelbg1->getContentSize().height*0.35)));
        this->addChild(label_title);
        
        
        CCString *str=CCString::createWithFormat("Lv%d",lordSkill->getLevel());
        SGCCLabelTTF*lv = SGCCLabelTTF::create( str->getCString(), FONT_BOXINFO, 32);
        lv->setAnchorPoint(ccp(0, 0.5));
        lv->setPosition(ccpAdd(label_title->getPosition(),ccp(label_title->getContentSize().width+18,0 )));
        this->addChild(lv);
    }
    else
    {
        title->setString(str_EmbattleLayer_str15);
        qianhua1->setVisible(false);
        qianhua1->setEnabled(false);
        title->setPosition( ccpAdd(zjj->getPosition(),ccp(0, -qianhua1->getContentSize().height*0.5 -14)));
        SGCCLabelTTF*a = SGCCLabelTTF::create(str_GeneralInfoEffect_str1, FONT_BOXINFO, 32);
        this->addChild(a);
        this->addChild(title);
       a->setPosition(ccpAdd(center,ccp(0, qianhua1->getContentSize().height*1.5 -14)));
    }
   
     SGCCLabelTTF *title2 = SGCCLabelTTF::create("", FONT_PANGWA, 36);
	//某些情况，被动技读取不正确，修正

	 SGSkillDataModel *officerSkill=SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
     SGCCLabelTTF *title1 = SGCCLabelTTF::create("", FONT_PANGWA, 36);
    if (officerSkill)
    {
        title1->setString(officerSkill->getSkillName()->getCString());
        SGCCLabelTTF* a1 = SGCCLabelTTF::create(officerSkill->getSkillName()->getCString(), FONT_BOXINFO, 32);      //上面一排 裂魂之路 lv1

        a1->setAnchorPoint(ccp(0, 0.5));
        a1->setPosition(ccpAdd(ccp(wjj->getPositionX()-wjj->getContentSize().width*0.5,wjj->getPositionY()),ccp(0, -qianhua1->getContentSize().height*0.5 -10)));

        //被动计 cgpUI
        //a1->setPosition(labelbg2->getPosition());
        CCSprite* label22=SGRichLabel::getFomLabel(officerSkill->getSkillInfo()->getCString(), CCSizeMake((size.width*0.8 - 8) * 0.73, size.height*0.2),FONT_BOXINFO, 32);
        
       label22->setPosition(ccpAdd(labelbg2->getPosition(),ccp(0, -20)));
       // label22->setPosition(labelbg2->getPosition());
        CCString *str1=CCString::createWithFormat("Lv%s",officerSkill->getSkillClientLevel()->getCString());
        //=SGCCLabelTTF::create(str1, FONT_BOXINFO, 32);
        SGCCLabelTTF*lv = SGCCLabelTTF::create( str1->getCString(), FONT_BOXINFO, 32 );
        lv->setAnchorPoint(ccp(0, 0.5));
      //   lv->setPosition(ccpAdd(a1->getPosition(),ccp(wjj->getContentSize().width*0.75,0 )));
        
        
         lv->setPosition(ccpAdd(a1->getPosition(),ccp(a1->getContentSize().width+18,0 )));
        this->addChild(lv);
        this->addChild(a1);
        this->addChild(label22);
        //a->setAnchorPoint(ccp(0.5, 1));
        
    }
    else
    {
        title2->setString(str_GeneralInfoEffect_str2);
        title2->setPosition( ccpAdd(wjj->getPosition(),ccp(0, -qianhua1->getContentSize().height*0.5)));
        this->addChild(title2);
        qianhua2->setVisible(false);
        qianhua2->setEnabled(false);
        SGCCLabelTTF*a1 = SGCCLabelTTF::create(str_GeneralInfoEffect_str3, FONT_BOXINFO, 32);
         a1->setPosition(ccpAdd(center,ccp(0, -qianhua1->getContentSize().height*2.2)));
        this->addChild(a1);
      //  a1->setPosition(ccpAdd(center, ccp(0, 100)));
    }

    //100=好友，20=商城，16=卡牌图鉴，4=找人帮忙,7=强化 ,1111聊天展示
    if(type==100 || type == 20 ||type == 16 || type == 4 ||type == 7 || type== CHATSHOWENTRYTAG ||type ==20001)
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
    
    qianhua1->setVisible(false);
    qianhua1->setEnabled(false);
    qianhua2->setEnabled(false);
    qianhua2->setVisible(false);
    
       return true;
    

}



void SGGeneralinfoEffect::strengHandlerzjj()
{
    EFFECT_PLAY(MUSIC_BTN);
    
//            SGMainManager::shareMain()->showStrengLayer(_card, 1,false,true);
    
    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
    if (lordSkill && lordSkill->getNextId()==0)
    {
        SG_SHOW_WINDOW(str_GeneralInfoEffect_str4);
    }
    else if (lordSkill && lordSkill->getLevel()==_card->getLordMaxLevel())
    {
        SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
    }
    else
    {
    this->boxClose();
    SGStrengConfirm *effect = SGStrengConfirm::create(_dele, _card,2);
    SGMainManager::shareMain()->showBox(effect);
    }
}




void SGGeneralinfoEffect::strengHandlerwjj()
{
    
   SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
    if (officerSkill && officerSkill->getNextId()==0)
    {
        SG_SHOW_WINDOW(str_GeneralInfoEffect_str4);
    }
    else if (officerSkill && officerSkill->getSkillMaxLevel()==_card->getSkillMaxLevel())
    {
        SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
    }
    else
    {
    //SGMainManager::shareMain()->showChooseSkillLayer();
   // this->boxClose();
    
//    SGMainManager::shareMain()->showStrengLayer(_card, 4,false);

       this->boxClose();
    SGStrengConfirm *effect = SGStrengConfirm::create(_dele, _card,1);
    SGMainManager::shareMain()->showBox(effect);
    }

}

void SGGeneralinfoEffect::backhandle(CCObject*obj)
{
    
    
    SGGeneralInfoLayer *firstLayer = (SGGeneralInfoLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_generalInfoLayer);
    
    if (firstLayer) {
        firstLayer->setIsCanTouch(true);
    }
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    if(enterType==25)
    {
        mainLayer->setIsCanTouch(false);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    
    this->boxClose();
    
    
}

