//
//  SGActivityMainLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGActivityMainLayer.h"
#include "SGMainManager.h"
#include "SGActivityMainCell.h"
#include "SGCantAdvanceBox.h"

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "ActivityModule.pb.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGGuideManager.h"
#include  "SGStringConfig.h"
#include <algorithm>

SGActivityMainLayer::SGActivityMainLayer(void) : selectIndex(0)
{
    ;
}

SGActivityMainLayer::~SGActivityMainLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_activityLayer);
   //CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("public/public0.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("story/story2.plist");
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_ACT_SCENE);
}




SGActivityMainLayer* SGActivityMainLayer::create(CCArray *array)
{
    SGActivityMainLayer *actionLayer = new SGActivityMainLayer();
    if (actionLayer && actionLayer->init(NULL, sg_activityLayer))
    {
        actionLayer->autorelease();
        actionLayer->initDatas(array);
        actionLayer->initView();
        return actionLayer;
    }
    CC_SAFE_DELETE(actionLayer);
    return NULL;
}

void SGActivityMainLayer::initView()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_STORY_ACT_SCENE, this, callfuncO_selector(SGActivityMainLayer::sceneHandler));
    
    
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_activityLayer);//
	ResourceManager::sharedInstance()->bindTexture("sgactivitymainlayer/sgactivitymainlayer.plist", RES_TYPE_LAYER_UI, sg_activityLayer);

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_activityLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_activityLayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bgImg->getContentSize().width, bgImg->getContentSize().height-headH);
    bgImg->setTextureRect(r);
    
    if (CCDirector::sharedDirector()->getWinSize().height != 1136)
    {
        bgImg->setScaleY(0.8);
    }

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
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("title_activity_n.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ActivityMainCell_str10, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGActivityMainLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGActivityMainLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.1);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.65f, title->getPosition().y));
    
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(3)));
    tableView->setDown(-20);
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    tableView -> setPageNEnable(false);
    
    if (SGGuideManager::shareManager()->limitStartStep > 0) {
        setIsCanTouch(false);
    }
	
}
static bool sortRuler(CCObject* obj1,CCObject * obj2)
{
    SGStoryBean * bean1 = (SGStoryBean*)obj1;
    SGStoryBean * bean2 = (SGStoryBean*)obj2;
    return bean1->getActivityState()> bean2->getActivityState();
}
void SGActivityMainLayer::initDatas(CCArray *array)
{
    if(array->count()>1)
    {
        std::sort(array->data->arr,array->data->arr+array->data->num,sortRuler);
    }
    this->datas = CCArray::create();
    this->datas->retain();
    if (array != NULL) {
        for (int ii = 0; ii < array->count(); ii++) {
            CCObject *obj = array->objectAtIndex(ii);
            this->datas->addObject(obj);
        }
    }
	

	
}

void SGActivityMainLayer::backHandler(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}

#pragma mark -
#pragma mark - -----tableView actions-----
SNSTableViewCellItem* SGActivityMainLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    
	SGActivityMainCell *item = (SGActivityMainCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL) {
        item = SGActivityMainCell::create(this,tableView);
    }
    if (index<datas->count()) {
        SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(index);
        item->setData(bean);
        item->setVisible(bean->getVisible());
    }else {
        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGActivityMainLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

long getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec;
}
///////
 void SGActivityMainLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    selectIndex = index;
    if (index < datas->count()) {
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//        int count = player->getOfficerCards()->count() + player->getEquipCards()->count() + player->getPropCards()->count();
        int count = player->getGoodsNumInBag();
        if (count >= player->getPlayerBagSize())
        {
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }
        else
        {
            SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(index);
			
			long activityStartTime = bean->getActivityStartTime();
			long activityEndTime = bean->getActivityEndTime();
			
            if (bean->getActivityState() == 1 && activityStartTime <= 0 &&  activityEndTime > 0) {//时间应该在对应的活动时间内
                if(bean->getChallengeCount()>0){
                    SGPlayerInfo::sharePlayerInfo()->setPlotId(10000);
                    main::ActivityListRequest *request = new main::ActivityListRequest();
                    request->set_storyid(bean->getStoryId());
                    SGSocketClient::sharedSocketClient()->send(MSG_STORY_ACT_SCENE, request);
                }
                else{
                    SGMainManager::shareMain()->showMessage(str_ActivityMainCell_str111);
                }
            }
            else
            {
				SGMainManager::shareMain()->showMessage(str_ActivityMainCell_str11);
                //CCLOG("Activity is not Open");
            }
           
        }
    }



}
////////

void SGActivityMainLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{

//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    selectIndex = index;
//    if (index < datas->count()) {
//        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//        int count = player->getOfficerCards()->count() + player->getEquipCards()->count() + player->getPropCards()->count();
//        if (count > player->getPlayerBagSize())
//        {
//            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
//            SGMainManager::shareMain()->showBox(cantadvanceBox);
//        }
//        else
//        {
//            SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(index);
//			
//			long activityStartTime = bean->getActivityStartTime();
//			long activityEndTime = bean->getActivityEndTime();
//			long currentTime = getCurrentTime();
//			
//			
//            if (bean->getActivityState() == 1 && currentTime >= activityStartTime && currentTime <= activityEndTime) {//时间应该在对应的活动时间内
//                SGPlayerInfo::sharePlayerInfo()->setPlotId(10000);
//                main::ActivityListRequest *request = new main::ActivityListRequest();
//                request->set_storyid(bean->getStoryId());
//                SGSocketClient::sharedSocketClient()->send(MSG_STORY_ACT_SCENE, request);
//            }
//            else
//            {
//                CCLOG("Activity is not Open");
//            }
//        }
//    }
}

static bool sortRuler_(CCObject* obj1,CCObject * obj2)//
{
    SGStorySceneBean * bean1 = (SGStorySceneBean*)obj1;
    SGStorySceneBean * bean2 = (SGStorySceneBean*)obj2;
    return bean1->getPlotId()> bean2->getPlotId();
}

#pragma mark ==============这里设置活动副本次数==================

void SGActivityMainLayer::sceneHandler(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::ActivityListResponse *response = (main::ActivityListResponse *)req->m_msg;
    CCLog("scene=================%d", response->plotmodel_size());
    if (response) {
        SGStoryBean *story = (SGStoryBean *)datas->objectAtIndex(selectIndex);
        int size = response->plotmodel_size();
        if (size > 0) {
            for (int ii = 0; ii < size; ii++) {
                main::PlotModel model = response->plotmodel(ii);
                SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                if (scene == NULL) {
                    continue;
                }else {
                    scene->setVisible(true);
                    scene->setRank(model.rank()); 
                    scene->setOpenState(model.openstate());
                    scene->setPower(model.power());
                    scene->setZhandou(model.zhandou());
                    scene->setExp(model.exp());
                    scene->setGold(model.gold());
                    
                    //加入对应每个场景的可挑战次数
                    story->setChallengeCount(model.joincount());
                    scene->setCreamDayTimes(model.joincount());

                    int itemNum = model.itemids_size();
                    scene->itemIds.clear();
                    for (int jj =0;jj<itemNum; jj++) {
                        scene->itemIds.push_back(model.itemids(jj));
                    }
                }
            }
        }
        
        if(story->getStorySections()->count()>1)
        {
            std::sort(story->getStorySections()->data->arr,story->getStorySections()->data->arr+story->getStorySections()->data->num,sortRuler_);
        }
        SGMainManager::shareMain()->m_trackIsPve = false;
        SGMainManager::shareMain()->showStorySectionLayer(story);
    }
}