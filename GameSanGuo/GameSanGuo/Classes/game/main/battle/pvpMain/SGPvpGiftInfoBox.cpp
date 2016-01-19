//
//  SGPvpGiftInfoBox.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//
#include "SGGiftInfoBox.h"
#include "SGPvpGiftInfoBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SimpleAudioEngine.h"
#include "SGVipGiftLayer.h"
#include "ResourceManager.h"
#include "SGpvpEverydayLayer.h"
#include "SGSeasonListLayer.h"
#include "SGStringConfig.h"
SGPvpGiftInfoBox::SGPvpGiftInfoBox()
:_type(0),
_array(NULL),
_buyGift(NULL),
_delegate(NULL)
{
}
SGPvpGiftInfoBox::~SGPvpGiftInfoBox()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_pvpGiftInfoBox);

}
SGPvpGiftInfoBox *SGPvpGiftInfoBox::create(SGBoxDelegate *delegate,SGVipGift *buyGift,int type)
{
    SGPvpGiftInfoBox *giftBox = new SGPvpGiftInfoBox();
    
    if (giftBox && giftBox->init(delegate,box_sellTag, CCSizeMake(535, 594),true,false))
    {
        giftBox->_delegate = delegate;
        giftBox->_type = type;//1每日奖励,2赛季奖励
        giftBox->initView(buyGift);
        giftBox->autorelease();
        return giftBox;
    }
    CC_SAFE_DELETE(giftBox);
    return NULL;
}

void SGPvpGiftInfoBox::initView(SGVipGift *buyGift)
{
    _buyGift = buyGift;

    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_box_pvpGiftInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_box_pvpGiftInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sggiftinfobox/sggiftinfobox.plist", RES_TYPE_LAYER_UI, sg_box_pvpGiftInfoBox);

    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    tableViewHeight = 115;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    tableView->setFrame(CCRectMake(0, 0, 400, 350));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(s.width/2-tableView->getContentSize().width/2+20,-200)));
    
    

    //测试：不更改服务器消息
   /* SGGift * gold = new SGGift();
    gold->setItemId(10000);
    gold->setIconid(10000);
    CCString * temp = CCString::create("元宝");
    gold->setName(temp);
    gold->setNum(1000);
    gold->setType(0);
    
    
    buyGift->addGiftList(gold);
    
 */
    
    
    
    _array = buyGift->getGiftList();
    
    CCSprite *infoBg = CCSprite::createWithSpriteFrameName("chongzhi.png");
    infoBg->setPosition(center);
    this->addChild(infoBg);
    
    CCSprite *storebg_k = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");
    storebg_k->setPosition(ccp(storebg_k->getContentSize().width*.6,infoBg->getContentSize().height-storebg_k->getContentSize().height*.6+10));
    infoBg->addChild(storebg_k);
    CCSprite *store_k = NULL;
    switch(buyGift->getPvpgifttype())
    
    {
        case 1:
            store_k = CCSprite::createWithSpriteFrameName("gift_guanjun.png");
            break;
        case 2:
            store_k = CCSprite::createWithSpriteFrameName("gift_yajun.png");
            break;
        case 3:
            store_k = CCSprite::createWithSpriteFrameName("gift_jijun.png");
            break;
        case 5:
        {   if(_type ==1)
            store_k = CCSprite::createWithSpriteFrameName("5reward.png");
            else
                store_k = CCSprite::createWithSpriteFrameName("gift_4-10.png");   
        }
            break;
        case 10:
            store_k = CCSprite::createWithSpriteFrameName("10reward.png");
            break;
        case 15:
            store_k = CCSprite::createWithSpriteFrameName("15reward.png");
            break;

        default:
            store_k = CCSprite::createWithSpriteFrameName("gift_4-10.png");
            break;
            
    }
    

    //store_k = CCSprite::createWithSpriteFrameName("pvpgift.png");
    store_k->setPosition(ccp(storebg_k->getContentSize().width/2,storebg_k->getContentSize().height/2));
    storebg_k->addChild(store_k);
    
    CCLabelTTF * name = CCLabelTTF::create(CCString::createWithFormat("%s",buyGift->getGiftName()->getCString())->getCString(), FONT_PANGWA, 26);
    name->setAnchorPoint(ccp(0, 1));
    name->setColor(ccYELLOW);
    //name->setPosition(ccpAdd(storebg_k->getPosition(), ccp(storebg_k->getContentSize().width*.6-20 , storebg_k->getContentSize().height*.6/2+10)));
    name->setPosition(ccpAdd(storebg_k->getPosition(), ccp(storebg_k->getContentSize().width*.6-20 , storebg_k->getContentSize().height*.6/2+10 -33 )));
    infoBg->addChild(name);
    
/*  CCLabelTTF* info = CCLabelTTF::create(CCString::createWithFormat("%s",buyGift->getGiftInfo()->getCString())->getCString(), FONT_PANGWA, 24,CCSizeMake(24*9, 24*3),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    info->setAnchorPoint(ccp(0,1));
    info->setPosition(ccpAdd(name->getPosition(),ccp(0,-33)));
    infoBg->addChild(info);
    
*/
    
//    CCLabelTTF * info = CCLabelTTF::create("赛季结束时", FONT_PANGWA, 24);
//    info->setAnchorPoint(ccp(0, 1));
//    info->setPosition(ccpAdd(name->getPosition(), ccp(0,-info->getContentSize().height*1.3)));
//    infoBg->addChild(info);
//    
//    CCLabelTTF * info2 = CCLabelTTF::create(CCString::createWithFormat("天梯第%d可领取",buyGift->getPvpgifttype())->getCString(), FONT_PANGWA, 24);
//    info2->setAnchorPoint(ccp(0, 1));
//    info2->setPosition(ccpAdd(info->getPosition(), ccp(0,-info2->getContentSize().height)));
//    infoBg->addChild(info2);
//    
//    if (_type ==1) {
//        
//        CCString *str = CCString::createWithFormat("%d场奖励礼包",buyGift->getPvpgifttype());
//        name->setString(str->getCString());
//        str = CCString::createWithFormat("每天战斗%d场",buyGift->getPvpgifttype());
//        info->setString(str->getCString());
////        str = CCString::createWithFormat("每天战斗%d场",buyGift->getPvpgifttype());
//        info2->setString("即可获得奖励");
//        
//    }
    
    
//    SGButton *buyButton = SGButton::create("hotBtnBg.png", "buy.png", this, menu_selector(SGPvpGiftInfoBox::buyHandler),CCPointZero,false,true);
    SGButton *buyButton = SGButton::createFromLocal("hotBtnBg.png", str_Get_, this, menu_selector(SGPvpGiftInfoBox::buyHandler),ccp(0,3),FONT_PANGWA,ccWHITE,32);

    buyButton->setPosition(ccpAdd(center, ccp(-buyButton->getContentSize().width/2, -594/2+40)));
    this->addBtn(buyButton);
    
    if (_buyGift->getHaveBuy()!=1) {
        buyButton->setImage("hotBtnBgGray.png");
        buyButton->setEnabled(false);
    }
    
//    SGButton *cancelButton = SGButton::create("hotBtnBg.png", "back.png", this, menu_selector(SGPvpGiftInfoBox::boxCloseWithOutCallBack),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::createFromLocal("hotBtnBg.png", str_Back_, this, menu_selector(SGPvpGiftInfoBox::boxCloseWithOutCallBack),ccp(0,3),FONT_PANGWA,ccWHITE,32);
    cancelButton->setPosition(ccpAdd(center, ccp(cancelButton->getContentSize().width/2, -594/2+40)));
    this->addBtn(cancelButton);
    
    CCSize a = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(a.width*2, a.height*2);

#if (PLATFORM == IOS)
    char *spritData = new char[(int)size.width * (int)size.height *4];
    memset(spritData,0,(int)size.width * (int)size.height *4);
    int spriteDataSize = (int)size.width * (int)size.height*4;
    for (int j=0; j<spriteDataSize; j+=4) {
        spritData[j] = 0;
        spritData[j+1] = 0;
        spritData[j+2] = 0;
        spritData[j+3] = 255;
    }
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, (int)size.width, (int)size.height, size);
    delete [] spritData;
    pTexture->autorelease();
    
    CCSprite *finaleSprite = CCSprite::createWithTexture(pTexture);
#else
    CCLayerColor *finaleSprite = CCLayerColor::create(ccc4(0, 0, 0, 180), size.width, size.height);
#endif
    
    finaleSprite->setPosition(ccpAdd(CCPointZero, ccp(-finaleSprite->getContentSize().width*.2,-finaleSprite->getContentSize().height*.2)));
    finaleSprite->setAnchorPoint(CCPointZero);
    finaleSprite->setOpacity(198);


    this->addChild(finaleSprite,-5,-5);

}

void SGPvpGiftInfoBox::buyHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
    }
    if (_type == 1)
    {
        
        SGpvpEverydayLayer *vipGiftLayer = (SGpvpEverydayLayer *)mainScene->getChildByTag(sg_pvpeverydaylayer);
        if (vipGiftLayer) {
            SGBaseBox::boxCloseWithOutCallBack();
            vipGiftLayer->getHandle(_buyGift);
        }
    }
   else if (_type ==2)
   {
       SGSeasonListLayer *vipGiftLayer = (SGSeasonListLayer *)mainScene->getChildByTag(sg_seasonlistlayer);
       if (vipGiftLayer) {
           SGBaseBox::boxCloseWithOutCallBack();
           vipGiftLayer->getHandler(_buyGift);
       }
   }

}

void SGPvpGiftInfoBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}

SNSTableViewCellItem *SGPvpGiftInfoBox::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    Gift_Item *item = (Gift_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGGift *gift = (SGGift*)_array->objectAtIndex(indexPath->getRow());
    if (NULL == item) {
        item = Gift_Item::create(gift, sg_box_pvpGiftInfoBox);
    }
    else
    {
        item->updategift(gift,indexPath->getRow());
    }
    
	return item;
    
}
unsigned int  SGPvpGiftInfoBox::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    if (_array == NULL) {
        return 0;
    }else{
    int num = _array->count();
        return num;}
}

void SGPvpGiftInfoBox::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
}


#pragma mark - pvpGift_Item Class

pvpGift_Item::pvpGift_Item()
:_gift(NULL),
spritePlayerHead(NULL),
labelName(NULL),
num(NULL),
arrayStar(NULL),
kuang(NULL)
{
    
}

pvpGift_Item::~pvpGift_Item()
{
    arrayStar->release();
}
