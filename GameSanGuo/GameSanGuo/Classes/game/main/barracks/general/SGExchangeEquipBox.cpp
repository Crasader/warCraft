//
//  SGExchangeEquipBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-9.
//
//

#include "SGExchangeEquipBox.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "itemmodule.pb.h"
#include "SGMsgId.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGExchangeEquipBox::SGExchangeEquipBox()
:officer(NULL),
equip(NULL),
before(NULL),
_dele(NULL),
enterType(0)
{
    deletage = NULL;
}
SGExchangeEquipBox::~SGExchangeEquipBox()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_exchangeEquipBox);

//    CC_SAFE_RELEASE(equip);
//    CC_SAFE_RELEASE(officer);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EQUIP_WEAR);
    
 

}
SGExchangeEquipBox *SGExchangeEquipBox::create(SGBoxDelegate *bdg, SGOfficerCard *card,SGEquipCard *card_,int enterType)
{
    SGExchangeEquipBox *instanceBox = new SGExchangeEquipBox();
    if (instanceBox && instanceBox->init(bdg,card,card_))
    {
        instanceBox->_dele = bdg;
        instanceBox->enterType = enterType;
        instanceBox->autorelease();
        return instanceBox;
    }
    CC_SAFE_DELETE(instanceBox);
    return NULL;
}
bool SGExchangeEquipBox::init(SGBoxDelegate *bdg, SGOfficerCard *card,SGEquipCard *card_)
{     
	ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_box_exchangeEquipBox);
	
    float wid = 535;
    float hgt = 324;
    
    if (!SGBaseBox::init(bdg,box_exchangeEquipTag,CCSizeMake(wid, hgt)))
    {
        return false;
    }
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EQUIP_WEAR,
                              this,
                              callfuncO_selector(SGExchangeEquipBox::wearEquipListener));

    officer = card;
    equip = card_;
    
    CCSprite *title = CCSprite::createWithSpriteFrameName("font_qingqr.png");
    title->setPosition(biao->getPosition());
    this->addChild(title);
    equip->getOfficerCardId();
    CCArray *officerCards = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    CCObject *obj = NULL;   
    CCARRAY_FOREACH(officerCards, obj)
    {
        SGOfficerCard *temp = (SGOfficerCard *)obj;
        if (temp->getSsid() == equip->getOfficerCardId()) {
            before = temp;
        }
    }    
    CCString *aaa= CCString::createWithFormat("%s",before->getOfficerName()->getCString());   
    
    SGCCLabelTTF* b = SGCCLabelTTF::create(str_SGExchangeEquipBox_str1, FONT_PANGWA, 32);
    this->addChild(b);
    b->setPosition(ccpAdd(biao->getPosition(), ccp(0, -115)));
    
    SGCCLabelTTF* a = SGCCLabelTTF::create(str_SGExchangeEquipBox_str2, FONT_PANGWA, 32);
    this->addChild(a);
    a->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + a->getContentSize().width/2, 50)));
    
   // a->setPosition(ccpAdd(biao->getPosition(), ccp(-215, -90)));
    SGCCLabelTTF *c = SGCCLabelTTF::create(str_SGExchangeEquipBox_str3, FONT_PANGWA, 32);

   this->addChild(c);
   c->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + c->getContentSize().width/2, -50)));
    
    SGCCLabelTTF* name = SGCCLabelTTF::create(aaa->getCString(), FONT_PANGWA, 32 , ccRED);
    //modify by:zyc. merge into create.
    //name->setColor(ccRED);
    CCMenuItemLabel* nameBtn = CCMenuItemLabel::create(name, this, NULL);
    this->addChild(nameBtn);
    nameBtn->setPosition(ccpAdd(a->getPosition(), ccp(a->getContentSize().width/2 + nameBtn->getContentSize().width/2,0)));
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(str_SGExchangeEquipBox_str4, FONT_PANGWA, 32);
    label1->setPosition(ccpAdd(nameBtn->getPosition(), ccp(nameBtn->getContentSize().width/2+label1->getContentSize().width/2, 0)));
    this->addChild(label1);
    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGExchangeEquipBox::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
        
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGExchangeEquipBox::boxCloseWithOutCallBack),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-145, -324/2+5 -40)));
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(145, -324/2+5-40)));

return true;
}
void SGExchangeEquipBox::showOfficerInfo()
{
    this->boxCloseWithOutCallBack();

    SGMainManager::shareMain()->showGeneralInfoLayer(officer,equip->getItemType());
    this->removeFromParentAndCleanup(true);
}
void SGExchangeEquipBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::WearEquipmentRequest *request = new main::WearEquipmentRequest();
    request->set_officerid(officer->getSsid());
    request->set_equipmentid(equip->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_WEAR, request);
}

void SGExchangeEquipBox::wearEquipListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::WearEquipmentResponse *response = (main::WearEquipmentResponse *)sr->m_msg;
    if (sr)
    {
        if (response->state() == 1)
        {
			//调用顺序不能随意更改，不然就crash！！！！！！！！！！！！！！！！！！
			
            equip->setOfficerCardId(officer->getSsid());
            SGMainManager::shareMain()->showEffect(equip, 1);
            officer->addEquip(equip);
			CCString *str = CCString::createWithFormat(str_SGExchangeEquipBox_str5,officer->getOfficerName()->getCString(),equip->getOfficerName()->getCString());
			
			before->removeEquip(equip);
            int type = enterType;
            
            if(type==11||type==12)
            {
                SGMainManager::shareMain()->showGeneralInfoLayer(officer,3,NULL,false,NULL,type);
            }else{
                 SGMainManager::shareMain()->showGeneralInfoLayer(officer,0);
            }
           
			
            //SGMainManager::shareMain()->showGeneralInfoLayer(officer,0);
           
			
        
            SGMainManager::shareMain()->showMessage(str->getCString());
           
			//在SGBaseBox的boxCloseWithOutCallBack已经调用removeFromParentAndCleanup了，big hole
            //this->removeFromParentAndCleanup(true);
            this->boxCloseWithOutCallBack2();
        }
    }
}
void SGExchangeEquipBox::boxCloseWithOutCallBack2()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true,false);
    
   
    this->removeFromParentAndCleanup(true);
}
void SGExchangeEquipBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true,false);
    
    if(deletage){
        deletage->setIsCanTouch(true);
    }
   this->removeFromParentAndCleanup(true);
}