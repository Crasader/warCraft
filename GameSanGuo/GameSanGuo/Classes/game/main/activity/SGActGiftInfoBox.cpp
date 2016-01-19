//
//  SGActGiftInfoBox.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//
#include "SGGiftInfoBox.h"
#include "SGActGiftInfoBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SimpleAudioEngine.h"
#include "SGVipGiftLayer.h"
#include "ResourceManager.h"
#include "SGpvpEverydayLayer.h"
#include "SGSeasonListLayer.h"
#include "SGActivityLayer.h"
#include "SGStringConfig.h"
SGActGiftInfoBox::SGActGiftInfoBox():
_type(0),
_array(NULL),
_buyGift(NULL),
_delegate(NULL)
{
}
SGActGiftInfoBox::~SGActGiftInfoBox()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_actGiftInfoBox);

}
SGActGiftInfoBox *SGActGiftInfoBox::create(SGBoxDelegate *delegate,SGVipGift *buyGift,int type,CCString *name , CCString*info)
{
    SGActGiftInfoBox *giftBox = new SGActGiftInfoBox();
    
    if (giftBox && giftBox->init(delegate,box_sellTag, CCSizeMake(535, 594),true,false))
    {
        giftBox->_delegate = delegate;
        giftBox->_type = type;//1每日奖励,2赛季奖励
        giftBox->initView(buyGift,name,info);
        giftBox->autorelease();
        return giftBox;
    }
    CC_SAFE_DELETE(giftBox);
    return NULL;
}

void SGActGiftInfoBox::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(-1);
    tableView->setTouchPriority(-256);
}

void SGActGiftInfoBox::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void SGActGiftInfoBox::initView(SGVipGift *buyGift,CCString *names, CCString*infos)
{
    _buyGift = buyGift;

    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_box_actGiftInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_box_actGiftInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sggiftinfobox/sggiftinfobox.plist", RES_TYPE_LAYER_UI, sg_box_actGiftInfoBox);

    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    tableViewHeight = 115;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    tableView->setFrame(CCRectMake(0, 0, 400, 350));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(s.width/2-tableView->getContentSize().width/2+20,-200)));

    _array = buyGift->getGiftList();
    
    CCSprite *infoBg = CCSprite::createWithSpriteFrameName("chongzhi.png");
    infoBg->setPosition(center);
    this->addChild(infoBg);
    
    CCSprite *storebg_k = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");
    storebg_k->setPosition(ccp(storebg_k->getContentSize().width*.6,infoBg->getContentSize().height-storebg_k->getContentSize().height*.6+10));
    infoBg->addChild(storebg_k);
    CCSprite *store_k = CCSprite::createWithSpriteFrameName("pvpgift.png");
    store_k->setPosition(ccp(storebg_k->getContentSize().width/2,storebg_k->getContentSize().height/2));
    storebg_k->addChild(store_k);
    
    CCLabelTTF * name = CCLabelTTF::create(CCString::createWithFormat("%s",names->getCString())->getCString(), FONT_PANGWA, 26);
    name->setAnchorPoint(ccp(0, 1));
    name->setColor(ccYELLOW);
    name->setPosition(ccpAdd(storebg_k->getPosition(), ccp(storebg_k->getContentSize().width*.6-20 , storebg_k->getContentSize().height*.6/2+10)));
    infoBg->addChild(name);
    
    
    CCLabelTTF * info = CCLabelTTF::create(CCString::createWithFormat("%s",infos->getCString())->getCString(), FONT_PANGWA, 24,CCSizeMake(225,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    info->setAnchorPoint(ccp(0, 1));
    info->setPosition(ccpAdd(name->getPosition(), ccp(0,-35)));
    infoBg->addChild(info);
    

    SGButton *buyButton = SGButton::createFromLocal("hotBtnBg.png", str_Get_, this, menu_selector(SGActGiftInfoBox::buyHandler),ccp(0,3),FONT_PANGWA,ccWHITE,32);

    buyButton->setPosition(ccpAdd(center, ccp(-buyButton->getContentSize().width/2, -594/2+40)));
    this->addBtn(buyButton);

    if (_buyGift->getHaveBuy()!=1) {
        buyButton->setImage("hotBtnBgGray.png");
        buyButton->setEnabled(false);
    }
    

    
    if (_buyGift->getHaveBuy() == FIRSTCHARGESHOWTAG) {
        SGButton *cancelButton = SGButton::createFromLocal("hotBtnBg.png", str_Ok_, this, menu_selector(SGActGiftInfoBox::boxCloseWithOutCallBack),ccp(0,3),FONT_PANGWA,ccWHITE,32);
        cancelButton->setPosition(ccpAdd(center, ccp(0, -594/2+40)));
        this->addBtn(cancelButton);
        buyButton->removeFromParentAndCleanup(true);
    }
    else
    {
        SGButton *cancelButton = SGButton::createFromLocal("hotBtnBg.png", str_Back_, this, menu_selector(SGActGiftInfoBox::boxCloseWithOutCallBack),ccp(0,3),FONT_PANGWA,ccWHITE,32);
        cancelButton->setPosition(ccpAdd(center, ccp(cancelButton->getContentSize().width/2, -594/2+40)));
        this->addBtn(cancelButton);
    }

    
    
    
    CCSize a = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(a.width*2, a.height*2);
    char *spritData = new char[(int)size.width * (int)size.height *4];
    memset(spritData,0,(int)size.width * (int)size.height *4);
    int spriteDataSize = (int)size.width * (int)size.height*4;
    for (int j=0; j<spriteDataSize; j+=4)
    {
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
    finaleSprite->setPosition(ccpAdd(CCPointZero, ccp(-finaleSprite->getContentSize().width*.2,-finaleSprite->getContentSize().height*.2)));
    finaleSprite->setAnchorPoint(CCPointZero);
    finaleSprite->setOpacity(198);
    this->addChild(finaleSprite,-5,-5);
    
    CCSprite *boxmenuitem = CCSprite::createWithTexture(pTexture);
    boxmenuitem->setPosition(ccpAdd(CCPointZero, ccp(-boxmenuitem->getContentSize().width*.2,-boxmenuitem->getContentSize().height*.2)));
    boxmenuitem->setAnchorPoint(CCPointZero);

    CCMenuItem *menuitem = CCMenuItemSprite::create(boxmenuitem, boxmenuitem);
    menuitem->ignoreAnchorPointForPosition(false);
    menuitem->setAnchorPoint(ccp(0,0));
    menuitem->setPosition(ccp(0,0));
    menuitem->setOpacity(0);
    menu->addChild(menuitem);


}

void SGActGiftInfoBox::buyHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, true);
    }
    
        SGActivityLayer *vipGiftLayer = (SGActivityLayer *)mainScene->getChildByTag(sg_luaactivity);
        if (vipGiftLayer)
        {
            SGBaseBox::boxCloseWithOutCallBack();
            vipGiftLayer->getHandle(_buyGift);
        }
}

void SGActGiftInfoBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, true);
    }
    SGBaseLayer *baselayer=(SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (baselayer) {
        baselayer->setIsCanTouch(true);
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}

SNSTableViewCellItem *SGActGiftInfoBox::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    Gift_Item *item = (Gift_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGGift *gift = (SGGift*)_array->objectAtIndex(indexPath->getRow());
    if (NULL == item) {
        item = Gift_Item::create(gift, sg_box_actGiftInfoBox);
    }
    else
    {
        item->updategift(gift,indexPath->getRow());
    }
    
	return item;
    
}
unsigned int  SGActGiftInfoBox::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    if (_array == NULL) {
        return 0;
    }else{
    int num = _array->count();
        return num;}
}

void SGActGiftInfoBox::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
}
