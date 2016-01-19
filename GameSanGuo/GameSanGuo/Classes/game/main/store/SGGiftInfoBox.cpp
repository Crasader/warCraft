//
//  SGGiftInfoBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-14.
//
//

#include "SGGiftInfoBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SimpleAudioEngine.h"
#include "SGVipGiftLayer.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGDrawCards.h"
#include "SGStringConfig.h"
SGGiftInfoBox::SGGiftInfoBox():_array(NULL)
,_buyGift(NULL)
,_delegate(NULL)
{
}
SGGiftInfoBox::~SGGiftInfoBox()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_giftInfoBox);

}
SGGiftInfoBox *SGGiftInfoBox::create(SGBoxDelegate *delegate,SGVipGift *buyGift)
{
    SGGiftInfoBox *giftBox = new SGGiftInfoBox();
    
    if (giftBox && giftBox->init(delegate,box_sellTag, CCSizeMake(535, 530),true,false))
    {
        giftBox->_delegate = delegate;
        giftBox->initView(buyGift);
        giftBox->autorelease();
        return giftBox;
    }
    CC_SAFE_DELETE(giftBox);
    return NULL;
}

void SGGiftInfoBox::initView(SGVipGift *buyGift)
{
    _buyGift = buyGift;

    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_box_giftInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_box_giftInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sggiftinfobox/sggiftinfobox.plist", RES_TYPE_LAYER_UI, sg_box_giftInfoBox);

    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    tableViewHeight = 115;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    tableView->setFrame(CCRectMake(0, 0, 400, 350));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(s.width/2-tableView->getContentSize().width/2+20,-200)));


    _array = buyGift->getGiftList();
    
    CCSprite *infoBg = CCSprite::createWithSpriteFrameName("chongzhi.png");
    infoBg->setPosition(center);
    this->addChild(infoBg);
    
    CCSprite *storebg_k = CCSprite::createWithSpriteFrameName("store_k.png");
    storebg_k->setPosition(ccp(storebg_k->getContentSize().width*.8,infoBg->getContentSize().height-storebg_k->getContentSize().height+30));
    infoBg->addChild(storebg_k);
    CCSprite *store_k = CCSprite::createWithSpriteFrameName("store_libaoicon.png");
    store_k->setPosition(ccp(storebg_k->getContentSize().width/2,storebg_k->getContentSize().height/2));
    storebg_k->addChild(store_k);
    

    CCSprite *libaoName = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("lb%d.png",_buyGift->getGiftVipLvl())->getCString());
    libaoName->setAnchorPoint(ccp(0, 1));
    libaoName->setPosition(ccpAdd(storebg_k->getPosition(), ccp(storebg_k->getContentSize().width - 50, storebg_k->getContentSize().height/2)));
    infoBg->addChild(libaoName);
    
    CCLabelTTF *yuanjia = CCLabelTTF::create(str_SystemMailLayer_str6, FONT_BOXINFO, FONTSIZE(12));
    yuanjia->setPosition(ccpAdd(libaoName->getPosition(), ccp(60,-libaoName->getContentSize().height-6)));
    infoBg->addChild(yuanjia);
    
//    CCSprite *tejia = CCSprite::create("store/font_tj.png");
    CCSprite *tejia = CCSprite::createWithSpriteFrameName("font_tj.png");
    infoBg->addChild(tejia);
    tejia->setPosition(ccpAdd(yuanjia->getPosition(), ccp(0, -40)));
    
    SGCCLabelTTF *_yuan = SGCCLabelTTF::create(CCString::createWithFormat("%d",buyGift->getGiftCost())->getCString(), FONT_BOXINFO, 24);
    SGCCLabelTTF *_te = SGCCLabelTTF::create(CCString::createWithFormat("%d",buyGift->getGiftDiscount())->getCString(), FONT_BOXINFO, 28 , ccYELLOW);
    infoBg->addChild(_yuan);
    infoBg->addChild(_te);
    _yuan->setAnchorPoint(ccp(0, 0.5));
    _te->setAnchorPoint(ccp(0, 0.5));
    _yuan->setPosition(ccpAdd(yuanjia->getPosition(), ccp(yuanjia->getContentSize().width/2 +20, 0)));
    _te->setPosition(ccpAdd(tejia->getPosition(), ccp(tejia->getContentSize().width/2 +20, 0)));
    //modify by:zyc. merge into create.
    //_te->setColor(ccYELLOW);
    
    
    SGButton *buyButton = SGButton::create("hotBtnBg.png", "buy.png", this, menu_selector(SGGiftInfoBox::buyHandler),CCPointZero,false,true);
    buyButton->setPosition(ccpAdd(center, ccp(-buyButton->getContentSize().width/2, -594/2+40)));
    this->addBtn(buyButton);
    
    SGButton *cancelButton = SGButton::create("hotBtnBg.png", "back.png", this, menu_selector(SGGiftInfoBox::boxCloseWithOutCallBack),CCPointZero,false,true);
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
    
    finaleSprite->setPosition(ccpAdd(CCPointZero, ccp(-finaleSprite->getContentSize().width*.2,-finaleSprite->getContentSize().height*.2)));
    finaleSprite->setAnchorPoint(CCPointZero);
    finaleSprite->setOpacity(198);
#else
    CCLayerColor *finaleSprite = CCLayerColor::create(ccc4(0, 0, 0, 180),a.width, a.height);
    finaleSprite->setPosition(ccpAdd(CCPointZero, ccp(0, 0)));
    finaleSprite->setAnchorPoint(CCPointZero);
    finaleSprite->setOpacity(198);
#endif

    this->addChild(finaleSprite,-5,-5);

}

void SGGiftInfoBox::buyHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGVipGiftLayer *vipGiftLayer = (SGVipGiftLayer *)mainScene->getChildByTag(sg_vipGiftLayer);
    if (vipGiftLayer) {
        SGBaseBox::boxCloseWithOutCallBack();
        vipGiftLayer->buyHandler(_buyGift);
    }
}

void SGGiftInfoBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, true);
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}

SNSTableViewCellItem *SGGiftInfoBox::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    Gift_Item *item = (Gift_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGGift *gift = (SGGift*)_array->objectAtIndex(indexPath->getRow());
    if (NULL == item) {
        item = Gift_Item::create(gift, sg_box_giftInfoBox);
    }
    else
    {
        item->updategift(gift,indexPath->getRow());
    }
    
	return item;
    
}
unsigned int  SGGiftInfoBox::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    if (_array == NULL) {
        return 0;
    }else{
    int num = _array->count();
        return num;}
}

void SGGiftInfoBox::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
}


#pragma mark - Gift_Item Class

Gift_Item::Gift_Item():_gift(NULL)
,spritePlayerHead(NULL)
,labelName(NULL)
,num(NULL)
//,arrayStar(NULL)
,kuang(NULL)
,advnacenum(NULL)
,bj(NULL)
,m_tag(sg_LayerNull)
{
    
}

Gift_Item::~Gift_Item()
{
//    arrayStar->release();
}

void Gift_Item::initView()
{
//    spritePlayerHead = SGDrawCards::drawallcards(_gift->getItemId(), _gift->getType(), m_tag);
    spritePlayerHead = SGDrawCards::createNormalSprite(_gift->getItemId(), _gift->getType(), m_tag);
    CCAssert(spritePlayerHead != NULL, "wrong gift! icon does not exit!");
    
//    CCLog("%f,%f",spritePlayerHead->getContentSize().width,spritePlayerHead->getContentSize().height);

    spritePlayerHead->setPosition(ccpAdd(ccp(0,0), ccp(-180 + spritePlayerHead->getContentSize().width/2, 0)));
    this->addChild(spritePlayerHead);
    
    CCString *str_ = CCString::createWithFormat("%s",_gift->getName()->getCString());
    labelName = CCLabelTTF::create(str_->getCString(), FONT_BOXINFO, FONTSIZE(12));
    labelName->setAnchorPoint(ccp(0, 1));
    labelName->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6, spritePlayerHead->getContentSize().height*.45)));
    this->addChild(labelName);
    
    num = CCLabelTTF::create(CCString::createWithFormat(str_SystemMailLayer_str7,_gift->getNum())->getCString(), FONT_BOXINFO, FONTSIZE(12));
    num->setAnchorPoint(ccp(0, 0));
    num->setPosition(ccpAdd(labelName->getPosition(), ccp(0, -spritePlayerHead->getContentSize().height*.9)));
    this->addChild(num);
    
    labelName->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6, spritePlayerHead->getContentSize().height*.45)));
    num->setPosition(ccpAdd(labelName->getPosition(), ccp(0, -spritePlayerHead->getContentSize().height*.9)));
////    if (_gift->getStar() == 0) {
//        labelName->setPosition(ccpAdd(labelName->getPosition(), ccp(0, -18)));
//        num->setPosition(ccpAdd(num->getPosition(), ccp(0, 18)));
////    }
    
    
//    arrayStar = CCArray::create();
//    arrayStar->retain();
//    this->showPlayerStar();
}

//设置星星数量
void Gift_Item::showPlayerStar()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("barrack/barrack1.plist");
    
//    for(int i=0;i<arrayStar->count();i++)
//    {
//        CCSprite *sprite = (CCSprite*)arrayStar->objectAtIndex(i);
//        sprite->removeFromParentAndCleanup(true);
//    }
    
//    for(int i=0; i < _gift->getStar();i++)
//    {
//        CCSprite *sprite = CCSprite::createWithSpriteFrameName("xx.png");
//        sprite->setAnchorPoint(ccp(0, 0.5));
//        sprite->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6 + sprite->getContentSize().width*i,0)));
//        arrayStar->addObject(sprite);
//        this->addChild(sprite);
//    }
    
}

Gift_Item* Gift_Item::create(SGGift *gift, LayerTag tag)
{
	Gift_Item *instance = new Gift_Item();
    instance->_gift = gift;
	if (instance && instance->init()) {
        
        instance->initView();
        instance->m_tag = tag; 
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

//刷新
void Gift_Item::updategift(SGGift *gift,int index)
{    
    //0:金子 1:铜钱  3:军功 4:小喇叭 5:炼丹秘方 6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌  10:碎片 11军魂 12体力 13军粮 14天梯积分 15消耗品
    _gift = gift;
    if (spritePlayerHead) {
        spritePlayerHead->removeFromParentAndCleanup(true);
        spritePlayerHead =NULL;
    }
    
//    spritePlayerHead = SGDrawCards::drawallcards(_gift->getItemId(), _gift->getType(), m_tag);
    spritePlayerHead = SGDrawCards::createNormalSprite(_gift->getItemId(), _gift->getType(), m_tag);
    CCAssert(spritePlayerHead != NULL, "wrong gift! icon does not exit!");
     
    spritePlayerHead->setPosition(ccpAdd(ccp(0,0), ccp(-180 + spritePlayerHead->getContentSize().width/2, 0)));
    this->addChild(spritePlayerHead);
    
     num->setString(CCString::createWithFormat(str_SystemMailLayer_str7,gift->getNum())->getCString());
    labelName->setString(CCString::createWithFormat("%s",gift->getName()->getCString())->getCString());
    labelName->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6, spritePlayerHead->getContentSize().height*.45)));
    num->setPosition(ccpAdd(labelName->getPosition(), ccp(0, -spritePlayerHead->getContentSize().height*.9)));
//    if (gift->getStar() == 0) {//刷新时字会重叠
//        labelName->setPosition(ccpAdd(labelName->getPosition(), ccp(0, -18)));
//        num->setPosition(ccpAdd(num->getPosition(), ccp(0, 18)));
//    }
//    if (gift->getStar()>0) {
//        labelName->setPosition(ccpAdd(spritePlayerHead->getPosition(), ccp(spritePlayerHead->getContentSize().width*.6, spritePlayerHead->getContentSize().height*.45)));
//        num->setPosition(ccpAdd(labelName->getPosition(), ccp(0, -spritePlayerHead->getContentSize().height*.9)));
//    }
//    showPlayerStar();
    
}
