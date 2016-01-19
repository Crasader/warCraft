//
//  SGStoryMainLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGStoryMainLayer.h"
#include "SGLayout.h"
#include "SGMainManager.h"
#include "SGStoryMainCell.h"
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
#include "SGStringConfig.h"

USING_NS_CC;

SGStoryMainLayer::SGStoryMainLayer(void):selectIndex(0)
,showIndex(0)
{

}
SGStoryMainLayer::~SGStoryMainLayer(void)
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_sortLayer);

//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_SCENE_ENTER);
}

SGStoryMainLayer *SGStoryMainLayer::create(CCArray *array)
{
    SGStoryMainLayer *storyLayer = new SGStoryMainLayer();
    if (storyLayer && storyLayer->init(NULL, sg_storyLayer))
    {
        storyLayer->initDatas(array);
        storyLayer->initView();
        storyLayer->autorelease();
        return storyLayer;
    }
    CC_SAFE_DELETE(storyLayer);
    return NULL;
}

void SGStoryMainLayer::checkVisible()
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

void SGStoryMainLayer::initView()
{
    SGPlayerInfo::sharePlayerInfo()->setsaodangType(0);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();

	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storyLayer);
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_storyLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_storyLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_storyLayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, 1136/2-(size.height-headH-bottomH)/2,bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    bgImg->setVisible(false);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));

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
	this->addChild(title_bg_r,1);
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_OverBattleBox_str26, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.42)));
    this->addChild(title);
    
	SGButton *activityBtn = SGButton::create("jingying.png", NULL, this, menu_selector(SGMainLayer::creamBattleHandler), ccp(0, 0), false, true);  //挑战精英
    this->addBtn(activityBtn);
	activityBtn->setPosition(ccp(size.width-activityBtn->getContentSize().width * 0.45f, title->getPosition().y+5));
    int limittag = SGStaticDataManager::shareStatic()->getimitTagById(2,3);
    activityBtn->setTag(limittag);
    
    int modelTag = 0;
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    
    //设置最后一个引导的第二关的高亮tag
    if (guideId > guide_tag_48)
    {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_52);
    }
    else if (guideId > guide_tag_40)
    {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_45);
    }
    else if (guideId > guide_tag_18)
    {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_20);
    }
    else if (guideId > guide_tag_3)
    {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_7_3);
    }
    else if (guideId > guide_tag_0)
    {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_3);
    }
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(12)));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));

}

void SGStoryMainLayer::initDatas(CCArray *array)
{
    this->datas = CCArray::create();
    this->datas->retain();
    if (array)
    {
        for (int ii = 0; ii < array->count(); ii++)
        {
            CCObject *obj = array->objectAtIndex(ii);
            this->datas->addObject(obj);
        }
    }
}

// activity action
void SGStoryMainLayer::activityHandler()
{
    SGMainManager::shareMain()->showActivityLayer();
}


void SGStoryMainLayer::limitMenuTouch()
{
    schedule(schedule_selector(SGStoryMainLayer::resetMenuTouch) , 2);
    if (menu)
    {
        menu->setTouchEnabled(false);
        tableView->setTouchEnabled(false);
    }
}

void SGStoryMainLayer::resetMenuTouch()
{
    unschedule(schedule_selector(SGStoryMainLayer::resetMenuTouch));
    if (menu)
    {
        menu->setTouchEnabled(true);
        tableView->setTouchEnabled(true);
    }
}

#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGStoryMainLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    //DDLog("path1=================%d, %d, %d", indexPath->getColumn(), indexPath->getRow(), indexPath->getSection());
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(index);
	CCSize size = tableView->getItemSize();
   // CCLOG("%f,%f",size.width,size.height);
	SGStoryMainCell *item = (SGStoryMainCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL)
        item=SGStoryMainCell::create(this, tableView);
    
    if (index<datas->count())
    {
        item->setData(index, bean);
        item->resetBgImg(index);
        item->setVisible(bean->getVisible());
    }
    else
        item->setVisible(false);
    
    return item;
}

unsigned int SGStoryMainLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}


////////////
 void SGStoryMainLayer::itemselect(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    

    
    
    
    
    //index;
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    GPCCLOG("\nindex, %d,   datasCount, %d", index,  datas->count());
    
//    if (datas->count() -index < 7 )
//    {
//        SGMainManager::shareMain()->showMessage("1 到 6 关正在调试新手引导，请选择其他关卡!");
//        return;
//    }
    
    
    if (index < datas->count())
    {
        
        
        
        //*****************闯关等级限制
        int minLevel = ( (SGStoryBean * )(datas->objectAtIndex(index) ) )->getMinLevel();
        if (SGPlayerInfo::sharePlayerInfo()->getPlayerLevel() < minLevel ) {
            //CCLOG("等级不够");
            char str[100];
            sprintf(str,str_StoryMainLayer_str1,minLevel);
            SGMainManager::shareMain()->showMessage(str);
            return;
        }
        
        selectIndex = index;
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
        int count = player->getGoodsNumInBag();
        if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
        {
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 12, count);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }
        else
        {
            int count = datas->count();
            
            if (count >= 5 && index == (count-5))
            {
                int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
                if (guideId <=guide_tag_46)
                {
                    SGGuideManager::shareManager()->checkIsDone(guide_tag_46);
                    return;
                }
            }
            showIndex = index;
            /////kanata  MT效果
            CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGStoryMainLayer::hideScrollItemSingleMove));
            CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGStoryMainLayer::showStoryScene));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*2),speedFun2,NULL));

        }
        
        limitMenuTouch();
    }

}
/////////////
void SGStoryMainLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    ;
}

void SGStoryMainLayer::showStoryScene()
{
    SGStoryBean *bean =  (SGStoryBean*)datas->objectAtIndex(showIndex);
    SGPlayerInfo::sharePlayerInfo()->setPlotId(0);
    main::PlotListRequest *request = new main::PlotListRequest();
    int storyId = bean->getStoryId();
    switch (storyId)
    {
        case 2:
            storyId = 4;
            break;
        case 3:
            storyId = 6;
            break;
        case 4:
            storyId = 2;
            break;
        case 6:
            storyId = 3;
            break;
        default:
            break;
    }

    request->set_storyid(storyId);
    request->set_poltaskid(storyId);
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
}

void SGStoryMainLayer::showSceneLayer(CCObject *sender)
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId <= guide_tag_10 || guideId == guide_tag_30)
    {
       return;
    }
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PlotListResponse *response = (main::PlotListResponse *)req->m_msg;
    if (response) {
        int storyId = response->storyid();
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
        SGStoryBean *story = NULL;
        if (array == NULL) { //战斗中断
            story = SGStaticDataManager::shareStatic()->getStoryDataById(1);
        }else {
            for (int ii = 0; ii < array->count(); ii++) {
                story = (SGStoryBean *)array->objectAtIndex(ii);
                if (storyId == story->getStoryId()) {
                    break;
                }
            }
        }
        if (story == NULL) {
            return;
        }
        story->setStoryStars(response->star());
        story->setMaxStar(response->maxstar());
        story->setCanReward(response->canreward());
        story->setRewardId(response->rewardid());
        if (response->state() == 0) {
            //第一次进入章节
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
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
            }
        }
        else if (response->state() == 1)
        {
            // 已进入过章节
            int size = response->plotmodel_size();
            if (size > 0) {
                int maxSceneId = 0;
                for (int ii = 0; ii < size; ii++) {
                    main::PlotModel model = response->plotmodel(ii);
                    
                    CCLOG("scene ID=================%d", model.plotid());
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL) {
                        continue;
                    }else {
                        if (maxSceneId < model.plotid()) {
                            maxSceneId = model.plotid();
                        }
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }

            }
        }
        else
        {
            return;
        }
        
        if (response->plotaskid() == 0 || story->getStoryId() == 1
            || story->getStoryId() == 2)
        {
            SGMainManager::shareMain()->m_trackIsPve = true;
            SGMainManager::shareMain()->showStorySectionLayer(story);
            

            
        }else
        { // 开启新关卡对话
            //对应类型中的章id，要进行对话引导
            if (SGPlayerInfo::sharePlayerInfo()->isGuideStory(4, story->getStoryId()))
            {
                //触发引导
                SGGuideManager::shareManager()->setLimitGuideTag(SGPlayerInfo::sharePlayerInfo()->checkStartLimitGuide(4,story->getStoryId()));
                SGGuideManager::shareManager()->startLimitGuide();
            }
            else
            {
                SGStorySceneBean *scene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(0);
                SGGuideManager::shareManager()->showStorySpeaker(scene->getPlotId(), this, story);//序章问题
            }
        }
    }
}

