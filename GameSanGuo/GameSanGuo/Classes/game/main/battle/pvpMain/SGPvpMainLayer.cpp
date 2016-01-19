//
//  SGPvpMainLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#include "SGPvpMainLayer.h"
#include "SGMainManager.h"
#include "SGPvpMainCell.h"
#include "SGTestSwitch.h"
#include "SGBattleMainLayer.h"

#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "PvpFight.pb.h"
#include "SGBattleData.h"
#include "SGRankPlayer.h"
#include "SGBattleFieldLayer.h"
#include "ResourceManager.h"
#include "SGGuideManager.h"
#include "SGStringConfig.h"

SGPvpMainLayer::SGPvpMainLayer(void)
{
}
SGPvpMainLayer::~SGPvpMainLayer(void)
{
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVPENTER_SEASONWAR);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_pvpMainLayer);
}

SGPvpMainLayer *SGPvpMainLayer::create()
{
    SGPvpMainLayer *storyLayer = new SGPvpMainLayer();
    if (storyLayer && storyLayer->init(NULL, sg_pvpMainLayer))
    {
        storyLayer->initData();
        storyLayer->initView();
        storyLayer->autorelease();
        return storyLayer;
    }
    CC_SAFE_DELETE(storyLayer);
    return NULL;
}

void SGPvpMainLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_pvpMainLayer);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = H_TOP;
    float bottomH = H_BOTTOM;
    
    CCRect rect = CCRectMake(0, stdSize.height/2-(size.height-headH-bottomH)/2,
                             768, size.height-headH-bottomH);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_pvpMainLayer);
    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
//    bgImg->setScaleY(rect.size.height / bgImg->getContentSize().height);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("upwar_title_pvp_n.png");
//    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5f)));
//    this->addChild(title);
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_PvpMainLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
    //title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5f)));
    this->addChild(title);
    
    
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height-bottomH-headH-skewing(3)));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    tableView->setDown(-20);
    tableViewHeight = 110;
    
}

void SGPvpMainLayer::initData()
{
    CCArray *data = CCArray::create();
    data->retain();
    data->addObject(CCString::create("Hello1"));
    data->addObject(CCString::create("Hello2"));
    data->addObject(CCString::create("Hello3"));
    this->datas = data;
}

#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGPvpMainLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();
    
	SGPvpMainCell *item = (SGPvpMainCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL) {
        item = SGPvpMainCell::create(this,tableView);
    }
    
    if (index<datas->count()) {
        item->setData(index);
    }else {
        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGPvpMainLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}
///////////////////////
void SGPvpMainLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    CCLog("pvp-----------------------%d", index);
    switch (index) {
        case 0:{
            SGMainManager::shareMain()->showPvpMatchLayer();
        }break;
        case 1:
            SGMainManager::shareMain()->showMessage(str_PvpMainLayer_str2);
            //            SGMainManager::shareMain()->showBattleMatchLayer();
            break;
        case 2:
            SGMainManager::shareMain()->showMessage(str_PvpMainLayer_str2);
            break;
        default:
            break;
    }



};












void SGPvpMainLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    CCLog("pvp-----------------------%d", index);
//    switch (index) {
//        case 0:{
//            int lavel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
//            if (lavel < 10) {
//                SGMainManager::shareMain()->showMessage("需要主公等级达到10级才能参加天梯晋级战！");
//#if IF_LEVEL
//                return;
//#endif
//            }
//            
//            CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGPvpMainLayer::hideScrollItemScale));
////            CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGPvpMainLayer::gotoMatchRequest));
//            
//            CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGPvpMainLayer::showFieldLayer));
//            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*2),speedFun2,NULL));
//            
////            main::PvpEntryRequest *request = new main::PvpEntryRequest();
////            SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
//        }break;
//        case 1:
//            SGMainManager::shareMain()->showMessage("该功能暂未开放");
////            SGMainManager::shareMain()->showBattleMatchLayer();
//            break;
//        case 2:
//            SGMainManager::shareMain()->showMessage("该功能暂未开放");
//            break;
//        default:
//            break;
//    }
}


void SGPvpMainLayer::showFieldLayer()
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGBattleFieldLayer *layer = (SGBattleFieldLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_storySectionLayer);
    if (!layer) {
        layer = SGBattleFieldLayer::create();
    }
    SGMainLayer *mainLayer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setVisible(true);
        mainLayer->setIsCanTouch(true);
        mainLayer->visitshow();
    }
    
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//////1126效果优化
}
