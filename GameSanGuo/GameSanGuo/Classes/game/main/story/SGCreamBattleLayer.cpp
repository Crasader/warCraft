//
//  SGCreamBattleLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-10-28.
//
//

#include "SGCreamBattleLayer.h"
#include "SGLayout.h"
#include "SGMainManager.h"
#include "SGCreamMainCell.h"
#include "SGCantAdvanceBox.h"
#include "SGSpotDataModel.h"
#include "SGStaticDataManager.h"
#include "SGStoryBean.h"
#include "SGStorySceneBean.h"
#include "SGMainLayer.h"
#include "SGGuideManager.h"

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PlotModule.pb.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include  "SGStringConfig.h"

USING_NS_CC;

SGCreamBattleLayer::SGCreamBattleLayer(void) : showIndex(0)
{
    selectIndex = 0;
}

SGCreamBattleLayer::~SGCreamBattleLayer(void)
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_creamLayer);
	
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CREAM_STORY);
    
    
    SGMainManager::sourceToStory = false;
}

SGCreamBattleLayer *SGCreamBattleLayer::create(CCArray *array)
{
    SGCreamBattleLayer *storyLayer = new SGCreamBattleLayer();
    if (storyLayer && storyLayer->init(NULL, sg_creamLayer))
    {
        storyLayer->initDatas(array);
        storyLayer->initView();
        storyLayer->autorelease();
        return storyLayer;
    }
    CC_SAFE_DELETE(storyLayer);
    return NULL;
}

void SGCreamBattleLayer::checkVisible()
{
    int showIndex = 12;
    
    if (showIndex == 12) {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, NULL);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }else if (showIndex == 13) {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, NULL);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
    CCLOG("ttt===================%i", menu->isTouchEnabled());
}

void SGCreamBattleLayer::initView()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    //this->setZOrder(-1);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_CREAM_STORY, this, callfuncO_selector(SGCreamBattleLayer::showSceneLayer));
	
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_creamLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_creamLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_creamLayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
//    CCRect rect = CCRectMake(0, 1136/2-(size.height-headH-bottomH)/2, bgImg->getContentSize().width, size.height-headH-bottomH);
   CCRect rect = CCRectMake(0, 0, bgImg->getContentSize().width, size.height);
    

    bgImg->setTextureRect(rect);
//    bgImg->setScaleY(size.height / bgImg->getContentSize().height);
	bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 0)));
     //bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
	//    this->addChild(titlecenter,10);
	this->addChild(titlecenter,1);//10.28//george
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
	//    this->addChild(title_bg_r,10);//10.28
	this->addChild(title_bg_r,1);
    //10.28//george
//    CCSprite *title = CCSprite::createWithSpriteFrameName("title_storyMode_n.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_OverBattleBox_str26, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(title);
    
    // activity button
	//    SGButton *activityBtn = SGButton::create("btn_hotactivity_nn.png",
	//                                         "btn_hotactivity_nn.png",
	//                                         this,
	//                                         menu_selector(SGMainLayer::activityHandler),
	//                                         ccp(0, 0),
	//                                         false,
	//                                         true);
	//10.28//george
	SGButton *activityBtn = SGButton::create("pt.png", NULL, this, menu_selector(SGMainLayer::storyHandler), ccp(0, 0), false, true);
    this->addBtn(activityBtn);
	//    activityBtn->setPosition(ccp(size.width-activityBtn->getContentSize().width*0.65f, title->getPosition().y));
	activityBtn->setPosition(ccp(size.width-activityBtn->getContentSize().width * 0.45f, title->getPosition().y+5));
	
	//george
	//    CCSprite *activityBg = CCSprite::createWithSpriteFrameName("btn_hotactivity_ns.png");
	//    activityBg->setPosition(activityBtn->getPosition());
	//    activityBg->setOpacity(0);
	//    this->addChild(activityBg, -1);
	//    CCActionInterval *action = (CCActionInterval *)CCSequence::create(CCFadeTo::create(1.25f, 125),
	//                                                                      CCFadeTo::create(1.25f, 255), NULL);
	//    activityBg->runAction(CCRepeatForever::create(action));
    
    int modelTag = 0;
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
//    if (guideId > guide_tag_27) {
//        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_48);
//    }else
    
//    if (guideId > guide_tag_3) {
//        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_27);
//    }else
    
    if (guideId > guide_tag_0) {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_3);
    }
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(12)));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
}

void SGCreamBattleLayer::initDatas(CCArray *array)
{
    this->datas = CCArray::create();
    this->datas->retain();
    if (array) {
        for (int ii = 0; ii < array->count(); ii++) {
            CCObject *obj = array->objectAtIndex(ii);
            this->datas->addObject(obj);
        }
    }
}


#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGCreamBattleLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    DDLog("path1=================%d, %d, %d", indexPath->getColumn(), indexPath->getRow(), indexPath->getSection());
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    
	SGCreamMainCell *item = (SGCreamMainCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL) {
        item = SGCreamMainCell::create(this,tableView);
    }
    
    if (index<datas->count()) {
        SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(index);
        item->setData(index, bean, (index == 0));
        item->resetBgImg(index);
        item->setVisible(bean->getVisible());

    }else {
        item->setVisible(false);
    }
    
    return item;
}



unsigned int SGCreamBattleLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}
///////////

void SGCreamBattleLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
	
	int index = tableView->getItemPointerWithIndexPath(indexpath);
    selectIndex = index;
    if (index < datas->count()) {
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//        int count = player->getOfficerCards()->count() + player->getEquipCards()->count() + player->getPropCards()->count();
        int count = player->getGoodsNumInBag();
        if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
        {
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }
        else
        {
            SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(index);
            //if (bean->getCreamCanJoin()) {
            SGPlayerInfo::sharePlayerInfo()->setPlotId(20000);//
            main::BossPlotListRequest *request = new main::BossPlotListRequest();
            request->set_storyid(bean->getStoryId());
            SGSocketClient::sharedSocketClient()->send(MSG_CREAM_STORY, request);
            //}
            //else
            //{
            //  CCLOG("Activity is not Open");
            //}
        }
    }


}
///////////
void SGCreamBattleLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{

}

void SGCreamBattleLayer::showStoryScene()
{
    SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(showIndex);
    SGPlayerInfo::sharePlayerInfo()->setPlotId(0);
    main::BossPlotListRequest *request = new main::BossPlotListRequest();
    request->set_storyid(bean->getStoryId());
    //request->set_poltaskid(bean->getStoryId());
	
    SGSocketClient::sharedSocketClient()->send(MSG_CREAM_STORY, request);
}

void SGCreamBattleLayer::showSceneLayer(CCObject *sender)
{
    //int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    //if (guideId <= guide_tag_10) return;
	
	
	SGSocketRequest *req = (SGSocketRequest *)sender;
    main::BossPlotListResponse *response = (main::BossPlotListResponse *)req->m_msg;
    CCLOG("scene=================%d", response->bossplotmodel_size());
    if (response) {
        SGStoryBean *story = (SGStoryBean *)datas->objectAtIndex(selectIndex);
        int size = response->bossplotmodel_size();
        if (size > 0) {
            for (int ii = 0; ii < size; ii++) {
                main::BossPlotModel model = response->bossplotmodel(ii);
                CCLOG("scene ID=================%d", model.plotid());
                SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                if (scene == NULL) {
                    continue;
                }else {
                    scene->setVisible(true);
                    //scene->setRank(model.ra);
					scene->setPower(model.power());
					scene->setGold(model.gold());
					scene->setExp(model.exp());
					scene->setZhandou(model.zhandou());
					scene->setRank(model.rank());
					scene->setCreamCanJoin(model.canjoin());
					
					scene->setCreamDayTimes(model.canjoincount());
					scene->setCreamBuyTimes(model.canbuycount());
					scene->setBuyPrice(model.price());
					scene->setBuyCompTimes(model.buycount());// 已经购买次数
					scene->setDropInfo(model.exp(), model.gold());
                    
                    //精英闯关 存储在playerinfo中
                    SGPlayerInfo::sharePlayerInfo()->setCreamData(model.plotid(), model.canjoincount(), model.canjoin());
                    
					int itemNum = model.itemids_size();
					scene->itemIds.clear();
					for (int jj =0;jj<itemNum; jj++) {
						scene->itemIds.push_back(model.itemids(jj));
					}
					
                }
                SGPlayerInfo::sharePlayerInfo()->setMaxBossPlotId(model.plotid());
            }
        }
        SGMainManager::shareMain()->m_trackIsPve = false;
        SGMainManager::shareMain()->showStorySectionLayer(story);
    }

}




