/*******************************************************
 *
 warning！！！！！！！！！！！！！！
 此文件目前未使用。
 by zyc。
 
 
 */




//  SGActivityDetailLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-3.
//
//

#include "SGActivityDetailLayer.h"
#include "SGMainManager.h"
#include "SGActivityDetailCell.h"
#include "SGCantAdvanceBox.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGActivityDetailLayer::SGActivityDetailLayer(void) : _activityId(0)
{
    ;
}

SGActivityDetailLayer::~SGActivityDetailLayer(void)
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("public/public0.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("barrack/barrack1.plist");

	ResourceManager::sharedInstance()->unBindLayerTexture(sg_activityDetail);
}

SGActivityDetailLayer* SGActivityDetailLayer::create(int sid)
{
    SGActivityDetailLayer *actionLayer = new SGActivityDetailLayer();
    if (actionLayer && actionLayer->init(NULL, sg_activityDetail))
    {
        actionLayer->initView(sid);
        actionLayer->autorelease();
        return actionLayer;
    }
    CC_SAFE_DELETE(actionLayer);
    return NULL;
}

void SGActivityDetailLayer::initView(int sid)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
	
#if (PLATFORM == ANDROID)
    GameConfig::gcForAndroid();
#endif
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/storyBg.plist", RES_TYPE_LAYER_UI, sg_activityDetail);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_activityDetail);
    
    ResourceManager::sharedInstance()->bindTexture("updatelayer/updatelayer.plist", RES_TYPE_LAYER_UI, sg_activityDetail);
	ResourceManager::sharedInstance()->bindTexture("sgactivitymainlayer/sgactivitymainlayer.plist", RES_TYPE_LAYER_UI, sg_activityDetail);

    
    CCRect rect = CCRectMake(stdSize.width/2-size.width/2, stdSize.height/2-(size.height-headH-bottomH)/2,
                             size.width, size.height-headH-bottomH);
    
    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("storyBg.png");
    bgImg->setTextureRect(rect);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title = CCSprite::createWithSpriteFrameName("barrack_title.png");
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, size.height - headH)));
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH+title->getContentSize().height)));
    this->addChild(title, -2);
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGActivityDetailLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setAnchorPoint(ccp(0, 0.5));
	backBtn->setScale(1.1);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.60f, title->getPosition().y));
    
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
}

void SGActivityDetailLayer::backHandler(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}

#pragma mark -
#pragma mark - -----tableView actions-----
SNSTableViewCellItem* SGActivityDetailLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();
    
	SGActivityDetailCell *item = (SGActivityDetailCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL) {
        item = SGActivityDetailCell::create();
    }
    item->setData(index);
    
    return item;
}

unsigned int SGActivityDetailLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return 5;
}

void SGActivityDetailLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
    
//    bool powerEnough = false;
//    if (powerEnough) {
//        int index = tableView->getItemPointerWithIndexPath(indexPath);
//        SGMainManager::shareMain()->showFindHelpLayer(index, false);
//    }else {
//        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 16, NULL);
//        SGMainManager::shareMain()->showBox(cantadvanceBox);
//    }
}
