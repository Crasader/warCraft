//
//  SGAutoEquipBox.cpp
//  GameSanGuo
//
#include "SGGeneralEquipBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ArmyModule.pb.h"
#include "SGSocketClient.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGExchangeEquipBox.h"
#include "SGMainLayer.h"
#include "SGAutoEquipBox.h"
#include "AutoEquip.pb.h"
#include "SGGeneralInfoLayer.h"
#include "SGMainScene.h"
#include "SGGeneralInfoLayer.h"
#include "SGRichLabel.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGAutoEquipBox::SGAutoEquipBox()
:officerCard(NULL),
equipCard(NULL),
state(0),
dlg(NULL),
enterType(0),
autotype(0)
{
    ;
}

SGAutoEquipBox::~SGAutoEquipBox()
{
    ;
}

SGAutoEquipBox *SGAutoEquipBox::create(SGBoxDelegate *delegate, SGOfficerCard *card,int x)
{
    SGAutoEquipBox *instaceBox = new SGAutoEquipBox();
  
    instaceBox->dlg = delegate;
    
    instaceBox->officerCard = card;
 
    if (instaceBox && instaceBox->init(delegate, box_autoEquip, CCSizeMake(539,354),true))
    {
        instaceBox->initView(x);
        instaceBox->autorelease();
        return instaceBox;
    }
    CC_SAFE_RELEASE(instaceBox);
    return NULL;
}

void SGAutoEquipBox::initView(int x)
{
    autotype=x;
    if(autotype==2)
    {
    
   
    
    
    SGButton *upBtn = SGButton::create("box_btn_newbg.png",
                                       "public_font_queding.png",
                                       this,
                                       menu_selector(SGAutoEquipBox::boxClosesendmsg),
                                       ccp(0, 0),
                                       false,
                                       true);
    
    this->addBtn(upBtn);
    upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-150, -138)));
        
    int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitEquipGuide,4);
    upBtn->setTag(limittag);
        
        const char* str=str_AutoEquipBox_str2;
        CCSprite* label2=SGRichLabel::getFomLabel(str, CCSizeMake(450,354),FONT_BOXINFO, 32,ccRED);
//        CCSize size=CCDirector::sharedDirector()->getWinSize();
        label2->setPosition(ccpAdd(biao->getPosition(),ccp(0,-270)));
        this->addChild(label2);
    SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGAutoEquipBox::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(145, -138)));
    }
    else if (autotype==1)
    {
        
       
        SGButton *upBtn = SGButton::create("box_btn_newbg.png",
                                           "public_font_queding.png",
                                           this,
                                           menu_selector(SGAutoEquipBox::boxClosesendmsg),
                                           ccp(0, 0),
                                           false,
                                           true);
        
        this->addBtn(upBtn);
        upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-150, -138)));
        
        const char* str=str_AutoEquipBox_str1;
        CCSprite* label2=SGRichLabel::getFomLabel(str, CCSizeMake(450,340),FONT_BOXINFO, 32,ccRED);
//        CCSize size=CCDirector::sharedDirector()->getWinSize();
        label2->setPosition(ccpAdd(biao->getPosition(),ccp(0,-270)));
        //label2->setPosition(ccp(320,350));
        this->addChild(label2);
        
//        SGCCLabelTTF* label1 = SGCCLabelTTF::create("要强", FONT_BOXINFO, 32);

        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGAutoEquipBox::boxClose),
                                            ccp(0, 0),
                                            false,
                                            true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(145, -138)));
        
        
    }

}


void SGAutoEquipBox::boxClose(CCObject*obj)
{
   
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }

 SGBaseBox::boxClose();
}


void SGAutoEquipBox::boxClosesendmsg(CCObject*obj)
{
   if(autotype==2)
   {
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
  //send message
    main::AutoEquipRequest*request=new main::AutoEquipRequest();

    int i= officerCard->getSsid();

  
  request->set_officerid(i);
   SGSocketClient::sharedSocketClient()->send(MSG_AUTO_EQUIP, request);
     SGBaseBox::boxClose();
   }

    
    else if (autotype==1)
    {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        if (mainLayer) {
            mainLayer->setIsCanTouch(true);
        }
        //send message
        main::AutoAllEquipRequest*request=new main::AutoAllEquipRequest();

        SGSocketClient::sharedSocketClient()->send(MSG_AUTO_ALLEQUIP, request);
		
		//刷新阵容数据
		//CCDictionary *dict = CCDictionary::create();
		//dict->setObject(CCString::createWithFormat("%d", 123), "type");
		//data = dict;
        SGBaseBox::boxClose();
        SG_SHOW_WINDOW(str_AutoEquipBox_str3);
        
   }
}





