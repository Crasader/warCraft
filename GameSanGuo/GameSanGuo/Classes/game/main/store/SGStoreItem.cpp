		//
//  SGStoreItem.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#include "SGStoreItem.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGStoreItem::SGStoreItem()
:type(-1)
,rmb(NULL)
,labelyb(NULL)
,font(NULL)
,labelsh(NULL)
,wyh(NULL)
,font1(NULL)
,yb(NULL)
,yukaT1(NULL)
,yukaT2(NULL)
,yukaD1(NULL)
,yukaD2(NULL)
,yukaGold(NULL)
,dik(NULL)
,delegate(NULL)
,tableview(NULL)
,jz(NULL)
{
}
SGStoreItem::~SGStoreItem()
{
    CCLOG("~SGStoreItem");
}

void SGStoreItem::updatedata(int num, CCArray *ary)
{

    
    int nshowtype = 0 ;//标签类型
    int nChargeType = 0;//充值类型
    int nDiscount = 0; //折扣
    int nGoldNum = 0;//获得元宝数
    int nCostRMB = 0;//花费
    int ngoldPerDay = 0 ;//每天可领数
    int nleftDay = 0;//剩余可购买天数
    int nRewardDay = 0;//可领天数
    //    CCString *str = (CCString *)ary->objectAtIndex(num);
    sscanf(((CCString*)ary->objectAtIndex(num))->getCString(), "%d-%d-%d-%d-%d-%d-%d-%d", &nshowtype,&nChargeType,&nDiscount,&nGoldNum,&nCostRMB,&ngoldPerDay,&nleftDay,&nRewardDay);
    // CCLOG("@@@@@-%d-%d-%d-%d-%d",num,label,sheng,numyb,b);
    
    //根据服务器的内容画每个item
    if (nChargeType!=0) {
        
        yb->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("yueKa_%d.png",nChargeType)->getCString())->displayFrame());
        rmb->setString(CCString::createWithFormat(str_GoldShopLimitTimeSell_str9,nCostRMB)->getCString());
        labelyb->initWithString(CCString::createWithFormat("%d",nGoldNum)->getCString(), "sanguobigpic/storenum1.png", 33, 34,'0');

        if(nleftDay == 0)
        {
            yukaD2->setString(str_GoldShopLimitTimeSell_str200);
        }
        else{
            yukaD2->setString(CCString::createWithFormat(str_GoldShopLimitTimeSell_str10,nleftDay)->getCString());
        }
        yukaGold->setString(CCString::createWithFormat(str_GoldShopLimitTimeSell_str11,ngoldPerDay)->getCString());

        rmb->setPosition(ccpAdd(dik->getPosition(), ccp(dik->getContentSize().width*1.1 +35, 0)));
        labelyb->setPosition(ccpAdd(jz->getPosition(), ccp(/*-jz->getContentSize().width/4*/-20, labelyb->getContentSize().height/2)));
        yukaT1->setVisible(true);
        yukaT2->setVisible(true);
        yukaD1->setVisible(true);
        yukaD2->setVisible(true);
        yukaGold->setVisible(true);
        
        font->setVisible(false);
        labelsh->setVisible(false);
        wyh->setVisible(false);
        dik->setScaleX(1.4);
    }
    else
    {
        CCString*  str_rmb = CCString::createWithFormat(str_GoldShopLimitTimeSell_str9,nCostRMB);
        rmb->setString(str_rmb->getCString());
        rmb->setFontSize(26);
        labelyb->initWithString(CCString::createWithFormat("%d",nGoldNum)->getCString(), "sanguobigpic/storenum1.png", 33, 34,'0');
        
        if (nDiscount !=0) {
            CCString *string = CCString::createWithFormat("%d/",nDiscount);
            labelsh ->initWithString(string->getCString(), "sanguobigpic/storenum2.png", 25, 28,'/');
            font->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_sheng.png")->displayFrame());
            font->setVisible(true);
            labelsh->setVisible(true);
            wyh->setVisible(false);
        }
        else
        {
            font->setVisible(false);
            labelsh->setVisible(false);
            wyh->setVisible(true);
        }
        yb->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("big_yb.png")->getCString())->displayFrame());
        
        yukaT1->setVisible(false);
        yukaT2->setVisible(false);
        yukaD1->setVisible(false);
        yukaD2->setVisible(false);
        yukaGold->setVisible(false);
        dik->setScaleX(1);
        rmb->setPosition(ccpAdd(dik->getPosition(), ccp(dik->getContentSize().width*1.1 +15, 0)));
        labelyb->setPosition(ccpAdd(jz->getPosition(), ccp(/*-jz->getContentSize().width/4*/0, labelyb->getContentSize().height/2)));
        
    }
    switch (nshowtype) {
        case 0:
        {
            font1->setVisible(false);
        }
            break;
        case 2:
        {
            font1->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_cz.png")->displayFrame());//超值
            font1->setVisible(true);
        }
            break;
        case 1:
        {
            font1->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_rm.png")->displayFrame());//热卖
            font1->setVisible(true);
        }
            break;
        case 3:
        {
            font1->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_YueKaTj.png")->displayFrame());//推荐
            font1->setVisible(true);
        }
            break;
            
        default:
        {
            font1->setVisible(false);
        }
            break;
    }
    
}

bool SGStoreItem::initWithSize(int num,CCArray *ary)
{
    if (!SNSTableViewCellItem::init()) {
		return false;
	}
 	
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_storeLayer);
	ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_storeLayer);
	ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_storeLayer);

    bgbgbg = NULL;
        
    //////////
 
    SGButton*itembg=SGButton::create("store_mainbg.png", NULL, this, menu_selector(SGStoreItem::buttonClick),CCPointZero,false,true);
    this->addChild(itembg);
    addBtn(itembg);
    
    
    dik = CCSprite::createWithSpriteFrameName("store_dik1.png");
    CCSprite *kuang = CCSprite::createWithSpriteFrameName("store_k.png");
     jz = CCSprite::createWithSpriteFrameName("store_jz.png");
	
  //  this->addChild(itembg,-1);
    //    itembg->setPosition(ccpAdd(itembg->getPosition(), ccp(kuang->getContentSize().width/2, 0)));
    dik->setPosition(ccpAdd(itembg->getPosition(), ccp(65, 0)));
    this->addChild(dik);
    
    kuang->setPosition(ccpAdd(itembg->getPosition(), ccp(-itembg->getContentSize().width/2 + kuang->getContentSize().width/2, 0)));
    this->addChild(kuang);
    this->addChild(jz);
    jz->setPosition(ccpAdd(kuang->getPosition(), ccp(dik->getPosition().x/2 - dik->getContentSize().width/4 - kuang->getPosition().x/2 + kuang->getContentSize().width/4 , 0)));
    
    yb = CCSprite::createWithSpriteFrameName("big_yb.png");
    this->addChild(yb);
    yb->setPosition(kuang->getPosition());
    
   
    
    int nshowtype = 0 ;//标签类型
    int nChargeType = 0;//充值类型
    int nDiscount = 0; //折扣
    int nGoldNum = 0;//获得元宝数
    int nCostRMB = 0;//花费
    int ngoldPerDay = 0 ;//每天可领数
    int nleftDay = 0;//剩余可购买天数
    int nRewardDay = 0;//可领天数
//    CCString *str = (CCString *)ary->objectAtIndex(num);
    sscanf(((CCString*)ary->objectAtIndex(num))->getCString(), "%d-%d-%d-%d-%d-%d-%d-%d", &nshowtype,&nChargeType,&nDiscount,&nGoldNum,&nCostRMB,&ngoldPerDay,&nleftDay,&nRewardDay);
   // CCLOG("@@@@@-%d-%d-%d-%d-%d",num,label,sheng,numyb,b);
    font1 = CCSprite::create();//超值条
    font1->setPosition(ccpAdd(kuang->getPosition(), ccp(-kuang->getContentSize().width*.25, kuang->getContentSize().height*.15)));
    this->addChild(font1);
    
    CCString* str_rmb = CCString::createWithFormat(str_GoldShopLimitTimeSell_str9,nCostRMB);
    rmb = CCLabelTTF::create(str_rmb->getCString(), FONT_PANGWA,26);
    rmb->setAnchorPoint(ccp(1, 0.5));
    this->addChild(rmb);
    rmb->setPosition(ccpAdd(dik->getPosition(), ccp(dik->getContentSize().width*1.1 +15, 0)));
    
    labelyb = CCLabelAtlas::create(CCString::createWithFormat("%d",nGoldNum)->getCString(), "sanguobigpic/storenum1.png", 33, 34,'0');
    labelyb->setAnchorPoint(ccp(.5,.5));
    labelyb->setPosition(ccpAdd(jz->getPosition(), ccp(/*-jz->getContentSize().width/4*/0, labelyb->getContentSize().height/2)));
    this->addChild(labelyb);
    
    font = CCSprite::create();
    font->setAnchorPoint(ccp(0, 0.5));
    font->setPosition(ccpAdd(dik->getPosition(), ccp(-dik->getContentSize().width*.5,0)));
    this->addChild(font);
    
    CCString *string = CCString::createWithFormat("%d/",nDiscount);
    labelsh = CCLabelAtlas::create(string->getCString(), "sanguobigpic/storenum2.png", 25, 28,'/');
    labelsh->setAnchorPoint(ccp(.5,.5));
    labelsh->setPosition(ccpAdd(dik->getPosition(), ccp(/*-jz->getContentSize().width/4*/28, /*labelsh->getContentSize().height/5*/0)));
    this->addChild(labelsh);
    
    wyh = CCLabelTTF::create(str_GoldShopLimitTimeSell_str12, FONT_PANGWA,26);
    wyh->setColor(ccWHITE);
    this->addChild(wyh,1);
    wyh->setPosition(dik->getPosition());
    
    yukaT1 = CCLabelTTF::create(str_GoldShopLimitTimeSell_str13, FONT_PANGWA,20);
    yukaT1->setPosition(ccpAdd(dik->getPosition(), ccp(-yukaT1->getContentSize().width/2 -10, yukaT1->getContentSize().height/2)));
    
    yukaT2 = CCLabelTTF::create(str_GoldShopLimitTimeSell_str14, FONT_PANGWA,20);
    yukaT2->setPosition(ccpAdd(yukaT1->getPosition(), ccp(-yukaT2->getContentSize().width/2, -yukaT2->getContentSize().height)));
    
    yukaGold = CCLabelTTF::create(str_GoldShopLimitTimeSell_str15, FONT_PANGWA,20);
    yukaGold->setColor(ccGREEN);
    yukaGold->setAnchorPoint(ccp(0, 0.5));
    yukaGold->setPosition(ccpAdd(yukaT2->getPosition(), ccp(yukaT2->getContentSize().width/2, 0)));
    if(nRewardDay ==-1)
    {
        yukaD1 = CCLabelTTF::create(str_GoldShopLimitTimeSell_str108, FONT_PANGWA,20);
    }else
    {
        yukaD1 = CCLabelTTF::create(CCString::createWithFormat(str_GoldShopLimitTimeSell_str109,nRewardDay)->getCString(), FONT_PANGWA,20);
    }
    yukaD1->setColor(ccYELLOW);
    yukaD1->setAnchorPoint(ccp(0, 0.5));
    yukaD1->setPosition(ccpAdd(yukaGold->getPosition(), ccp(yukaGold->getContentSize().width, 0)));
    
    yukaD2 = CCLabelTTF::create(str_GoldShopLimitTimeSell_str17, FONT_PANGWA,20);
    yukaD2->setColor(ccYELLOW);
    yukaD2->setAnchorPoint(ccp(1, 0.5));
    yukaD2->setPosition(ccpAdd(dik->getPosition(), ccp(dik->getContentSize().width*1.1+10,-dik->getContentSize().height/2 - yukaD2->getContentSize().height*0.8)));
    labelyb->setPosition(ccpAdd(jz->getPosition(), ccp(/*-jz->getContentSize().width/4*/0, labelyb->getContentSize().height/2)));
    addChild(yukaT1,1);
    addChild(yukaT2,1);
    addChild(yukaD1,1);
    addChild(yukaD2,1);
    addChild(yukaGold,1);
    
    if (nChargeType != 0) {
        
       
        yb->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("yueKa_%d.png",nChargeType)->getCString())->displayFrame());
        if(nleftDay == 0)
        {
            yukaD2->setString(str_GoldShopLimitTimeSell_str200);
        }
        else{
            yukaD2->setString(CCString::createWithFormat(str_GoldShopLimitTimeSell_str10,nleftDay)->getCString());
        }
        yukaGold->setString(CCString::createWithFormat(str_GoldShopLimitTimeSell_str11,ngoldPerDay)->getCString());
        
        rmb->setPosition(ccpAdd(dik->getPosition(), ccp(dik->getContentSize().width*1.1 +35, 0)));
        labelyb->setPosition(ccpAdd(jz->getPosition(), ccp(/*-jz->getContentSize().width/4*/-20, labelyb->getContentSize().height/2)));
        yukaT1->setVisible(true);
        yukaT2->setVisible(true);
        yukaD1->setVisible(true);
        yukaD2->setVisible(true);
        yukaGold->setVisible(true);
        
       font->setVisible(false);
       labelsh->setVisible(false);
       wyh->setVisible(false);
       dik->setScaleX(1.4);
    }
    else
    {
        if (nDiscount !=0) {
            font->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_sheng.png")->displayFrame());
            font->setVisible(true);
            labelsh->setVisible(true);
            wyh->setVisible(false);
            
        }
        else
        {
            font->setVisible(false);
            labelsh->setVisible(false);
            wyh->setVisible(true);
        }
        
        yukaT1->setVisible(false);
        yukaT2->setVisible(false);
        yukaD1->setVisible(false);
        yukaD2->setVisible(false);
        yukaGold->setVisible(false);
        dik->setScaleX(1);
    }
    
     //根据服务器的内容画每个item
   
        switch (nshowtype) {
            case 0:
            {
               font1->setVisible(false);
            }
                break;
            case 2:
            {
                font1->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_cz.png")->displayFrame());//超值
                font1->setVisible(true);
            }
                break;
            case 1:
            {
                font1->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_rm.png")->displayFrame());//热卖
                font1->setVisible(true);
            }
                break;
            case 3:
            {
                font1->setDisplayFrame(CCSprite::createWithSpriteFrameName("font_YueKaTj.png")->displayFrame());//推荐
                font1->setVisible(true);
            }
                break;
                
            default:
            {
                font1->setVisible(false);
            }
                break;
        }
    
  
    return true;
}


SGStoreItem* SGStoreItem::create(int num,CCArray *ary,bool isshow ,SGStoreItemDelegate*del,SNSTableView*table)
{
	SGStoreItem *instance = new SGStoreItem();
	if (instance && instance->initWithSize(num,ary)) {
        instance->delegate=del;
        instance->tableview=table;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
    
}


///////

void SGStoreItem::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);

}