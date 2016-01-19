//
//  SGExpandBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-22.
//
//

#include "SGExpandBox.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"

#include "SGSocketClient.h"
#include "ShopModule.pb.h"
#include "SGMsgId.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGExpandBox::SGExpandBox(void)
{
}
SGExpandBox::~SGExpandBox(void)
{
	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_expandBox);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_STORE_ENTER);
}

SGExpandBox *SGExpandBox::create(SGBoxDelegate *bdg)
{
    SGExpandBox *noExpBox = new SGExpandBox();
    if (noExpBox && noExpBox->init(bdg))
    {
        noExpBox->autorelease();
        return noExpBox;
    }
    CC_SAFE_DELETE(noExpBox);
    return NULL;
}
bool SGExpandBox::init(SGBoxDelegate *bdg)
{
	ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_box_expandBox, LIM_PNG_AS_PNG);
    
    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    float wid = 535;
    float hgt = 354;
    
    if (!SGBaseBox::init(bdg,box_expandBox,CCSizeMake(wid, hgt)))
    {
        return false;
    }
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_STORE_ENTER,
                              this,
                              callfuncO_selector(SGExpandBox::storeEnterListener));
    SGButton *ok =  SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGExpandBox::confirmHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);

    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.25, -hgt*.35)));
    
    
    SGButton *cancel = SGButton::create("box_btn_newbgHui.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGExpandBox::closeExpandBox),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.25, -hgt*.35)));
     
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("font_notice.png");
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(biao->getPosition());
    this->addChild(title);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();    
    CCString *str_gold = CCString::createWithFormat(str_CantAdvance_tips29,playerInfo->getPlayerGold());
    CCLabelTTF* a = CCLabelTTF::create(str_gold->getCString(), FONT_BOXINFO, 30);
    this->addChild(a);
    a->setPosition(SGLayout::getPoint(kMiddleCenter));    
  
    return true;
}
void SGExpandBox::storeEnterListener(CCObject *obj)
{
    SGBaseBox::boxClose();
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopChargeEntryResponse *response = (main::ShopChargeEntryResponse *)sr->m_msg;
    if (sr) {
        main::VIPInfo vip = response->vipinfo();
        int leng = response->chargeitems_size();
        CCArray *arrary = CCArray::create();
        CCArray *strary = CCArray::create();
        for (int i = 0;i<leng;i++) {
            main::ChargeItem item = response->chargeitems(i);
           CCString *str = CCString::createWithFormat("%d-%d-%d-%d-%d-%d-%d-%d",item.showtype(),item.chargetype(),item.discount(),item.gold(),item.rmb(),item.goldperday(),item.leftday(),item.rewardday());
            arrary->addObject(str);
            CCString *str2 = CCString::createWithFormat("%s",item.code().c_str());
            strary->addObject(str2);
            
        }
        
        
        SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,7,strary);
//        SGMainManager::shareMain()->showStoreLayer(response->currentviplevel(), response->nextviplevel(), response->updateneedrmb(), 7);
        
    }
   
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
    this->boxClose();
    
}

void SGExpandBox::closeExpandBox()
{
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
    this->boxClose();
}

void SGExpandBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (SGMainManager::shareMain()->getMainLayer()) {
        SGMainManager::shareMain()->getMainLayer()->requestStoreLayer(this);
    }
   
}