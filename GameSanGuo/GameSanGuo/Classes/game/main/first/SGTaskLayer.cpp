//
//  SGTaskLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-18.
//
//

#include "SGTaskLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGFirstLayer.h"
#include "SGCCLabelTTF.h"
#include "SGMainLayer.h"
#include "SGChatMessageCenter.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "RewardModule.pb.h"
#include "SGGuideManager.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "DayTaskModule.pb.h"
#include "ResourceManager.h"
#include "SGShowRewardBox.h"
#include "SGMainTaskLayer.h"
#include "SGStringConfig.h"

SGTaskLayer::SGTaskLayer()
:height(0),
tag(10),
canGetRewardNum(0),
s(CCSizeZero),
scroll(NULL),
scrollViewContainer(NULL),
pxData(NULL),
arr(NULL),
font1(NULL),
dayNotice(NULL)
{
    pxData = NULL;
    dayNotice=NULL;
}
SGTaskLayer::~SGTaskLayer()
{

    CC_SAFE_RELEASE(pxData);
    SGGuideManager::shareManager()->isShowTask = false;
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_SALARYENTER);
    notification->removeObserver(this, MSG_REWARD_RECOVERENTER);
    notification->removeObserver(this, MSG_REWARD_GODENTER);

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_taskLayer);
   
}
SGTaskLayer *SGTaskLayer::create(CCArray *array)
{
    SGTaskLayer *Layer = new SGTaskLayer();
    if (Layer && Layer->init(NULL, sg_taskLayer))
    {
        Layer->arr=array;
        Layer->initView(array);
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}
void SGTaskLayer::initView(CCArray *array)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_taskLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_taskLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_taskLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/shanguang.plist", RES_TYPE_LAYER_UI, sg_taskLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_SALARYENTER,
                              this,
                              callfuncO_selector(SGTaskLayer::salaryEnterListener));
    notification->addObserver(MSG_REWARD_RECOVERENTER,
                              this,
                              callfuncO_selector(SGTaskLayer::recoverEnterListener));
    notification->addObserver(MSG_REWARD_GODENTER,
                              this,
                              callfuncO_selector(SGTaskLayer::godEnterListener));
    
  
    
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0, -47)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_taskLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, btmhgt, bg->getContentSize().width, s.height - title_bg->getContentSize().height - btmhgt);
    //bg->setScaleY(r.size.height / s.height);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));

	SGButton *mainTask = SGButton::createFromLocal("store_exchangebtnbg.png", str_RankLayer_str11, this, menu_selector(SGTaskLayer::showMainTaskLayer),CCPointZero,FONT_PANGWA,ccWHITE,22);
	mainTask->setScale(1.05);
    this->addBtn(mainTask);
    mainTask->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( mainTask->getContentSize().width*.65, -49- mainTask->getContentSize().height*.55)));

    SGButton *dayTask = SGButton::createFromLocal("store_exchangebtnbg.png", str_RankLayer_str12, this, menu_selector(SGTaskLayer::showDayTaskLayer),CCPointZero,FONT_PANGWA,ccWHITE,22);
	dayTask->setScale(1.05);
    this->addBtn(dayTask);
    dayTask->setPosition(ccpAdd(mainTask->getPosition(), ccp(mainTask->getContentSize().width*1.5, 0)));

    
    dayNotice=CCSprite::createWithSpriteFrameName("publc_notice.png");
    dayTask->addChild(dayNotice);
     dayNotice->setScale(0.7);
    dayNotice->setPosition(ccp(dayTask->getContentSize().width-dayNotice->getContentSize().width/4, dayTask->getContentSize().height-dayNotice->getContentSize().height/4));
    dayNotice->setVisible(false);
    

    
    scroll = CCScrollView::create(CCSizeMake(s.width, s.height - btmhgt*.81 - title_bg->getContentSize().height - 102));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(scroll);
    scroll->setPosition(ccp(0, btmhgt));
    scrollViewContainer = CCLayer::create();
    
    taskMenu = SGMenu::create(CCRectMake(0, btmhgt*.68, s.width, s.height - btmhgt*.68 - title_bg->getContentSize().height -35));
    //taskMenu2 = SGMenu::create(CCRectMake(0, btmhgt*.68, s.width, s.height - btmhgt*.68 - title_bg->getContentSize().height -35));
    taskMenu->setPosition(CCPointZero);
   
    scrollViewContainer->addChild(taskMenu,3,1);
    pxData = CCArray::create();
    pxData->retain();
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    
    int finish = 0;

    
    int len = array->count();
    
    
    for (int i = len-1; i>=0; i--) {
        
        SGTaskObj *taskobj = (SGTaskObj *)array->objectAtIndex(i);
        SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(taskobj->gettaskId());
    
        SGCCLabelTTF *name = SGCCLabelTTF::create(task->gettaskname()->getCString(), FONT_PANGWA, 36);
       // CCLOG("%s",task->gettaskname()->getCString());
        name->setAnchorPoint(ccp(0, 0.5));
        SGButton *item = SGButton::createFromLocal("item_bigbg.png", "", this, menu_selector(SGTaskLayer::confirmHandler));
       
        
        item->setTag(taskobj->gettaskId());
        item->setAnchorPoint(ccp(0, 0));
        //taskMenu->addChild(item);
        name->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.1, item->getContentSize().height/2)));
        item->addChild(name);
        item->setPosition(ccp(s.width/2 - item->getContentSize().width/2,(item->getContentSize().height + skewing(5))*(len-1-i)));
        
        height += item->getContentSize().height + skewing(5);
        CCSprite *yb2 = CCSprite::createWithSpriteFrameName("store_yb.png");
        yb2->setPosition(ccpAdd(name->getPosition(),ccp(item->getContentSize().width*0.6,0)));
        yb2->setAnchorPoint(ccp(0.5,0.5));
        item->addChild(yb2,2,100);
        yb2->setVisible(false);
        
        
      //  CCLOG("%d",request->gold());
        
        CCString *nn=CCString::createWithFormat("%d",taskobj->getgold());//服务器数据
        std::string ss=nn->m_sString;
        const char*aa=ss.c_str();
        CCLabelTTF*labelw=CCLabelTTF::create(aa,FONT_BOXINFO,45);
        // CCSprite *yb2 = CCSprite::createWithSpriteFrameName("store_yb.png");
        labelw->setPosition(ccpAdd(name->getPosition(),ccp(item->getContentSize().width*0.6+40,0)));
        labelw->setColor(ccYELLOW);
        labelw->setVisible(false);
        item->addChild(labelw,2,102);
        
     
        CCSprite *lingstart = CCSprite::createWithSpriteFrameName("reward_lingq_hui.png");
        lingstart->setPosition(ccpAdd(name->getPosition(),ccp(item->getContentSize().width*0.8,0)));
        lingstart->setAnchorPoint(ccp(0.5,0.5));
        item->addChild(lingstart,9,106);
        lingstart->setVisible(true);
        taskMenu->addChild(item,5);
        
        CCSprite *hehe = CCSprite::createWithSpriteFrameName("reward_font_ylq.png");
        //hehe->setPosition(ccp(50,50));
        hehe->setPosition(ccpAdd(name->getPosition(),ccp(item->getContentSize().width*0.8+lingstart->getContentSize().width/2-hehe->getContentSize().width/2,0)));
        hehe->setAnchorPoint(ccp(0.5,0.5));
        hehe->setVisible(false);
        item->addChild(hehe,3,104);
        
        CCSprite *linging2 = CCSprite::createWithSpriteFrameName("reward_lingq.png");
        linging2->setPosition(ccpAdd(name->getPosition(),ccp(item->getContentSize().width*0.8,0)));
        linging2->setAnchorPoint(ccp(0.5,0.5));
        linging2->setVisible(false);
        item->addChild(linging2,2,103);
      //
        SGButton *linging=SGButton::create("reward_lingq.png","reward_lingq.png",this,menu_selector(SGTaskLayer::linghandle));
        //lingstart->setVisible(false);
        linging->setScale(1.4);
        linging->setTag(taskobj->gettaskId());
        linging->setAnchorPoint(ccp(0.5, 0.5));
        taskMenu->addChild(linging,1,taskobj->gettaskId()+99);
        linging->setPosition(ccpAdd(item->getPosition(),ccp(item->getContentSize().width*0.8+58,44)));
     // taskMenu->addChild(linging);
//        linging->setPosition(ccpAdd(item->getPosition(),ccp(item->getContentSize().width*0.8+20,23)));
        linging->setEnabled(false);
        linging->setVisible(false);
        
        
         if (taskobj->getisAward()==0)
        {
            lingstart->setVisible(true);
            linging->setVisible(false);
            linging->setEnabled(false);
            yb2->setVisible(true);
            labelw->setVisible(true);
        }
        else if (taskobj->gettaskCount() >= task->gettaskcount()) {
            CCLOG("taskobj->gettaskCount() >= task->gettaskcount()");
            finish++;
            if(taskobj->getisAward()==1)
            {
                
            
            linging->setVisible(false);
            linging->setEnabled(false);
            lingstart->setVisible(false);
            hehe->setVisible(true);
                
                
            }
           else if (taskobj->getisAward()==2)
            {
                //linging->setVisible(true);
                linging->setVisible(false);
                linging->setEnabled(true);
                lingstart->setVisible(false);
                linging2->setVisible(true);
                yb2->setVisible(true);
                labelw->setVisible(true);
                
                canGetRewardNum++;
            }
            
        }
        
        if(guideId < guide_tag_45 && i==0){ //任务闪烁
            CCSpriterX *resultx = CCSpriterX::create("animationFile/shanguang.scml", true, true);
            resultx->setAnchorPoint(ccp(0.5, 0.5));
            resultx->setPosition(ccpAdd(item->getPosition(), ccp(0, 5)));
            resultx->setanimaID(0);
            resultx->setisloop(true);
            scrollViewContainer->addChild(resultx, 10);
            resultx->setVisible(false);
            pxData->addObject(resultx);
        }
    }

    scroll->setContainer(scrollViewContainer);
    
    scrollViewContainer->setContentSize(CCSizeMake(s.width, height));
    
    scroll->setContentSize(CCSizeMake(s.width,height ));
    
    scroll->setContentOffset(ccp(0,-(height  - (s.height - btmhgt - title_bg->getContentSize().height - 75))));
    
    
    font1 = SGCCLabelTTF::create(CCString::createWithFormat(str_RankLayer_str13,finish,array->count())->getCString(), FONT_BOXINFO, FONTSIZE(15));
   this->addChild(font1);
    font1->setAnchorPoint(ccp(0, 0.5));
   font1->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(s.width*.05, -title_bg->getContentSize().height - font1->getContentSize().height-40)));
//    

    createMainTaskLayer();
    font1->setVisible(false);
    
    if (canGetRewardNum>0) {
        dayNotice->setVisible(true);
    }
}

void SGTaskLayer::confirmHandler(CCNode *sender)
{
    /* 
     1		进行一次闯关战斗
     2		进行一次精英闯关
     3		开启一次宝箱
     4	    进行一次铜雀夺宝
     5		进行一次天梯对战
     6		进行一次副本战斗
     7		强化一名武将
     8		强化一件装备
     9		强化一种士兵
     10		探访一次强力武将
     11		参与祭天
     12		领取当日俸禄
     13		和大家打个招呼
     14		进行守荆州战斗
     */
    
    EFFECT_PLAY(MUSIC_BTN);
    for (int ii = 0; ii < pxData->count(); ii++) {
        CCSpriterX *resultx = (CCSpriterX *)pxData->objectAtIndex(ii);
        resultx->stop();
        resultx->removeFromParentAndCleanup(true);
    }
    bool selectTask = true;
    tag = sender->getTag();
    switch (tag) {
       
        case 1:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->storyHandler();
            
        }
            break;
        case 2:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->creamBattleHandler();
        }
            break;
        case 3:
        case 4:
        {
             SGMainManager::shareMain()->getMainLayer()->entryTongQueShow();
        }
            break;
        case 5:
        {
            SGMainManager::shareMain()->showPvpMatchLayer();
            
        }
            
        
            break;
        case 6:
        {
            SGMainLayer *mainLayer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainLayer->activityHandler();
        }
            break;
        case 7:
            SGMainManager::shareMain()->showGeneralsLayer(2,true,5);
            break;
        case 8:
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL,0,1,true,5);
        }
            break;
        case 9:
            SGMainManager::shareMain()->showSoldierslayer(true,5);
            
            break;
        case 10:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->setPortraitMenuCanTouch(false);
            mainlayer->visithide();
            mainlayer->visitHandler();
        }
            
            break;
            
        case 11:
            showHope();
            break;
        case 12:
            showGetSalary();
            break;
        case 13:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->setPortraitMenuCanTouch(false);
            SGChatMessageCenter::shareChatMessageCenter()->openChatRoom();
        }
            break;
            
         ////守荆州每日
        case 14:
        {
            //发送获取详情
            SGMainManager::shareMain()->sendGetSjzInfoRequest();
        }
        default:
        {
            selectTask = false;
        }
            break;
    }
    if (selectTask) {
        SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(tag + 1);
        if (task) {
            SGMainManager::shareMain()->trackTaskBegin(task->gettaskname());
        }
    }
    
}


void SGTaskLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
//    SGMainManager::shareMain()->showFirstLayer();
}
void SGTaskLayer::showGetSalary()
{
    main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_SALARYENTER, request);
}
void SGTaskLayer::salaryEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::RewardSalaryEntryResponse *response = (main::RewardSalaryEntryResponse *)sr->m_msg;
    int levelCoin = response->levelcoin();
	//int canGetlevelCoin = response->cangetlevelcoin();
	int levelRewardTime = response->levelrewardtime();
	int pvpLevel = response->pvplevel();
	int pvpLevelCoin = response->pvplevelcoin();
	//int canGetPvPLevelCoin = response->cangetpvplevelcoin();
	int pvpRewardTime = response->pvprewardtime();
    int minLevel = response->pveminlevel();
    int minLevelpvp = response->pvpminlevel();
    SGMainManager::shareMain()->showSalaryLayer(levelCoin,levelRewardTime,pvpLevel,pvpLevelCoin,pvpRewardTime,minLevel,minLevelpvp);}
void SGTaskLayer::showWine()
{
    main::RewardRecoverPowerEntryRequest *request = new main::RewardRecoverPowerEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_RECOVERENTER, request);
    
}
void SGTaskLayer::recoverEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::RewardRecoverPowerEntryResponse *response = (main::RewardRecoverPowerEntryResponse *)sr->m_msg;
        int power = response->power();
        SGMainManager::shareMain()->showDrinkWineLayer(power,0);
    }
}
void SGTaskLayer::showHope()
{
    main::RewardGodPrayEntryRequest *request = new main::RewardGodPrayEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GODENTER, request);
}
void SGTaskLayer::godEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::RewardGodPrayEntryResponse *response = (main::RewardGodPrayEntryResponse *)sr->m_msg;
        int current = response->current();
        int max = response->max();
        int iscan = response->cando();
        /////////加入可能获得物品的信息们
        CCArray*array=CCArray::create();
        for (int i=0; i<response->godrewardinfo_size(); i++) {
            
            main::GodRewardInfo info =response->godrewardinfo(i);
            
            SGMailSystemAccessoryDataModel* data=new SGMailSystemAccessoryDataModel();
            data->setCount(info.count());
            data->setItemId(info.itemid());
            data->setType(info.type());
            
            
            array->addObject(data);
        }
        
        
        
        
        
        
        SGMainManager::shareMain()->showJitianLayer(current,max,iscan,array);
    
    }
}

void SGTaskLayer::showTaskBlank()
{
    for (int ii = 0; ii < pxData->count(); ii++) {
        CCSpriterX *resultx = (CCSpriterX *)pxData->objectAtIndex(ii);
        resultx->setVisible(true);
        resultx->play();
    }
}

void SGTaskLayer::showMenuItemMultiMove(CCMenu *menu)
{
    EFFECT_PLAY(MUSIC_ITEM);
    
    if (menu == NULL) {
        return;
    }
//    bool isSingleRow = true;
//    CCAction *action = NULL;
    SGButton *item = NULL;
//    SGButton *ling=NULL;
    int rowCount = menu -> getChildrenCount();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    CCPoint pos2 = CCPointZero;
    
    for (int ii = 0; ii < rowCount; ii++) {
        item = (SGButton *)menu->getChildByTag(ii);
       // ling = (SGButton *)menu->getChildByTag(ii+100);
        if (item != NULL)
        {
            pos = item->getPosition();
           
            item->stopAllActions();
            
            
//新效果
             item->setPosition(ccpAdd(pos, ccp(size.width, 0)));
            
            CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-size.width*1.05, 0));
            CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(size.width*0.1, 0));
            CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-size.width*0.05, 0));
            
            CCAction *action = CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGTaskLayer::hide)),CCDelayTime::create(0.15f*(ii/10.0f+1)),moveby1,moveby2,moveby3,CCCallFuncN::create(this, callfuncN_selector(SGTaskLayer::show)),NULL);
            item->runAction(action);
          
        }
    }
}




void SGTaskLayer::linghandle(CCObject*obj)
{
    
    EFFECT_PLAY(MUSIC_34);
    SGMenu*menu=(SGMenu*)(scrollViewContainer->getChildByTag(1));
    SGButton*item2=(SGButton*)obj;
    int m=item2->getTag()-99;
    SGButton*item=(SGButton*)(menu->getChildByTag(m));
    SGButton*linging=(SGButton*)(menu->getChildByTag(m+99));
    CCSprite* yb2=(CCSprite*)(item->getChildByTag(100));
    CCLabelTTF* labelw=(CCLabelTTF*)(item->getChildByTag(102));
    CCSprite* linging2=(CCSprite*)(item->getChildByTag(103));
    CCSprite* hehe=(CCSprite*)(item->getChildByTag(104));
     CCSprite* lingstart=(CCSprite*)(item->getChildByTag(106));
    CCLOG("teswr");
    yb2->setVisible(false);
    labelw->setVisible(false);
    hehe->setVisible(true);
    linging2->setVisible(false);
    linging->setVisible(false);
    linging->setEnabled(false);
    lingstart->setVisible(false);
    main::DayTaskGetRewardRequest *request = new main::DayTaskGetRewardRequest();
    request->set_taskid(m);
    
    //领取奖励框
    //弹出框显示奖励
    CCArray *data=CCArray::create();
    data->retain();
    SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
    datamodel->setType(0);
    datamodel->setCount(5);
    data->addObject(datamodel);
    
    SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
    SGMainManager::shareMain()->showBox(rewardBox);
    
    taskMenu->setTouchEnabled(false);
    scroll->setTouchEnabled(false);
   
    
    SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_STATUS, request);
    
    SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(m + 1);
    if (task) {
        SGMainManager::shareMain()->trackTaskCompleted(task->gettaskname());
    }
    
    canGetRewardNum--;
    if (canGetRewardNum==0) {
        dayNotice->setVisible(false);
    }
}

 void SGTaskLayer::hide(CCObject*obj)
{
    SGMenu*menu=(SGMenu*)(scrollViewContainer->getChildByTag(1));
     SGButton*item=(SGButton*)obj;
    int n=item->getTag();
      SGButton*ling=(SGButton*)(menu->getChildByTag(n+99));
    ling->setVisible(false);
    
}

void SGTaskLayer::show(CCObject*obj)
{
     SGMenu*menu=(SGMenu*)(scrollViewContainer->getChildByTag(1));
    SGButton*item=(SGButton*)obj;
    int n=item->getTag();
//    SGButton *ling=NULL;
//    ling = (SGButton *)menu->getChildByTag(n+100);
    SGButton*ling=(SGButton*)(menu->getChildByTag(n+99));
    ling->setVisible(true);
}

void SGTaskLayer::showBoxCall(CCObject *obj){
    taskMenu->setTouchEnabled(true);
    scroll->setTouchEnabled(true);
}

void SGTaskLayer::showMainTaskLayer(){
    if (isShowMainTaskLayer) {
        return;
    }
    setMainTaskVisible(true);
    mainTaskLayer->showRunAction();
     
}
void SGTaskLayer::showDayTaskLayer(){
    if (!isShowMainTaskLayer) {
        return;
    }
    setMainTaskVisible(false);
    this -> showMenuItemMultiMove(taskMenu);
    
}
void SGTaskLayer::createMainTaskLayer(){
    
    int index=0;
    CCArray *data=CCArray::create();
    data->retain();
    while (index<5) {
        CCObject *obj=new CCObject();
          index++;
        data->addObject(obj);
    }
    mainTaskLayer=SGMainTaskLayer::create(data);
    this->addChild(mainTaskLayer);
    scroll->setVisible(false);
    isShowMainTaskLayer=true;
    
  
//    SNSTableView::showScrollItemMultiMoveFromRight(mainTaskLayer->getSNSTavleView());
    
}

void SGTaskLayer::showFirstTask(){
      mainTaskLayer->showIsFinishTask();
}
void SGTaskLayer::setMainTaskVisible(bool isVisible){
    scroll->setVisible(!isVisible);
    font1->setVisible(!isVisible);
    mainTaskLayer->setVisible(isVisible);
    isShowMainTaskLayer=isVisible;

}
