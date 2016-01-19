//
//  SGTotalLoginRewardLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-29.
//
//

#include "SGTotalLoginRewardLayer.h"
#include "SGRewardSubLayer.h"
#include "SGSocketClient.h"
#include "RoleModule.pb.h"
#include "SGMsgId.h"
#include "SGCoinRewardBox.h"
#include "SGMainManager.h"
#include "SGPropRewardBox.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGShowRewardBox.h"
#include "SGRewardLayer.h"
#include "SGStringConfig.h"

int SUBLAYRTAG=-1;
#define SECTIONWIDTH 168
SGTotalLoginRewardLayer::SGTotalLoginRewardLayer()
:getReward(NULL),
qd_btn(NULL),
m_scrollview(NULL),
day1(0),
length(0),
leftLq(0)
{
    
}
SGTotalLoginRewardLayer::~SGTotalLoginRewardLayer(){
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GETCOINREWARD_LOGIN);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GETREWARD_LOGIN);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_totalLoginReward);
}

SGTotalLoginRewardLayer *SGTotalLoginRewardLayer::create(main::LoginAwardListResponse &response,SGBoxDelegate *dg){
    
    
    SGTotalLoginRewardLayer *layer=new SGTotalLoginRewardLayer();
    if (layer && layer->init(NULL, sg_totalLoginReward)) {
        layer->initView(response);
        layer->deletage=dg;
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
    
    
}
//void SGTotalLoginRewardLayer::onEnter(){
//    SGBaseLayer::onEnter();
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
////    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
//}
//void SGTotalLoginRewardLayer::onExit(){
//    SGBaseLayer::onExit();
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//}
//
//bool SGTotalLoginRewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    
//    return true;
//}
//void SGTotalLoginRewardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//
//}
void SGTotalLoginRewardLayer::initView(main::LoginAwardListResponse &response){
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_GETCOINREWARD_LOGIN, this, callfuncO_selector(SGTotalLoginRewardLayer::getCoinsRewardListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_GETREWARD_LOGIN, this, callfuncO_selector(SGTotalLoginRewardLayer::getRewardListener));

    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_totalLoginReward);
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_totalLoginReward, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_totalLoginReward);
    
//    CCArray *loginArr=new CCArray();
    CCDictionary *loginArr=new CCDictionary();
    loginArr->retain();
    day1= response.longinday();
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    
    
    //背景图
    CCSprite *bg=CCSprite::createWithSpriteFrameName("reward_login_bj.png");
    this->addChild(bg);
    bg->setScaleX(winSize.width/320);
    bg->setScaleY(winSize.height/480);
    
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 0)));
  
    //上面的条
    this->setTouchEnabled(true);
    CCSprite *leftDay=CCSprite::createWithSpriteFrameName("reward_login_tiao.png");
    this->addChild(leftDay);
    leftDay->setAnchorPoint(ccp(0.5, 0));
    leftDay->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(leftDay->getContentSize().width/2, -winSize.height/14)));

    
    
    ccColor3B color;
    color.r=79;
    color.g=36;
    color.b=19;
    SGCCLabelTTF *leftLabel=SGCCLabelTTF::create(str_RewardYueKaLayer_str8,FONT_PANGWA, 31 , color , ccYELLOW);
    this->addChild(leftLabel);
    leftLabel ->setAnchorPoint(ccp(0.5, 0));
    leftLabel->setPosition(ccpAdd(leftDay->getPosition(), ccp(-leftDay->getContentSize().width/4, leftDay->getContentSize().height/3-10)));
   
    
    
    SGCCLabelTTF *leftLabel1=SGCCLabelTTF::create(str_RewardRegLayer_str8,FONT_PANGWA, 31 , color , ccYELLOW );
    this->addChild(leftLabel1);
    leftLabel1 ->setAnchorPoint(ccp(0.5, 0));
    leftLabel1->setPosition(ccpAdd(leftDay->getPosition(), ccp(leftDay->getContentSize().width/5+10, leftDay->getContentSize().height/3-10)))  ;
  
    CCSprite *daySprite=CCSprite::createWithSpriteFrameName("mainLayer_button_bg.png");
    this->addChild(daySprite);
    daySprite ->setAnchorPoint(ccp(0.5, 0));
    daySprite->setPosition(ccpAdd(leftDay->getPosition(), ccp(leftDay->getContentSize().width/12, -10)));
    
    CCString *dayNum=CCString::createWithFormat("%d",day1);
    SGCCLabelTTF *leftLabel2=SGCCLabelTTF::create(dayNum->getCString(),FONT_PANGWA, 60 ,ccWHITE);
    this->addChild(leftLabel2);
    leftLabel2 ->setAnchorPoint(ccp(0.5, 0));
    leftLabel2->setPosition(ccpAdd(daySprite->getPosition(), ccp(0, 10)));
    
    //上下左右的框
    CCSprite *left_up_kuang=CCSprite::createWithSpriteFrameName("reward_login_kuang2.png");
    this->addChild(left_up_kuang);
    left_up_kuang->setAnchorPoint(ccp(0.5, 0));
    
    
    CCSprite *right_up_kuang=CCSprite::createWithSpriteFrameName("reward_login_kuang2.png");
    this->addChild(right_up_kuang);
    right_up_kuang->setFlipX(true);
    right_up_kuang->setAnchorPoint(ccp(0.5, 0));
    
    
    CCSprite *right_down_kuang=CCSprite::createWithSpriteFrameName("reward_login_kuang2.png");
    this->addChild(right_down_kuang);
    right_down_kuang->setFlipY(true);
    right_down_kuang->setAnchorPoint(ccp(0.5, 0));
    
    
    CCSprite *left_down_kuang=CCSprite::createWithSpriteFrameName("reward_login_kuang2.png");
    this->addChild(left_down_kuang);
    left_down_kuang->setFlipY(true);
    left_down_kuang->setFlipX(true);
    left_down_kuang->setAnchorPoint(ccp(0.5, 0));
    
    
    
    SGCCLabelTTF *remindLabel=SGCCLabelTTF::create(str_RewardYueKaLayer_str9,FONT_PANGWA, 42 ,ccYELLOW , ccBLACK);
    this->addChild(remindLabel);
    remindLabel->setAnchorPoint(ccp(0.5, 0));
    
    
    this->getReward=false;
    
    qd_btn=SGButton::create("box_btnbg.png","reward_login_queding.png", this, menu_selector(SGTotalLoginRewardLayer::getCoinRewardHandlel));
    this->addBtn(qd_btn);
    qd_btn->setAnchorPoint(ccp(0.5, 0));
    
    qd_btn->setZOrder(10);
    
    //CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCLayer *container = CCLayer::create();
    container->setPosition(ccp(0, 0));
    
   
    if (winSize.height == 960) {
        int up_height=-SGLayout::getPoint(kMiddleCenter).y+daySprite->getPosition().y-left_up_kuang->getContentSize().height;
        left_up_kuang->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-left_up_kuang->getContentSize().width/2, up_height)));
        m_scrollview = CCScrollView::create(CCSizeMake(540, 715));
         container->setContentSize(CCSizeMake(540, 715));
    }else if (winSize.height == 1136) {
        left_up_kuang->setScaleY(1.4);
        right_up_kuang->setScaleY(1.4);
        right_down_kuang->setScaleY(1.4);
        left_down_kuang->setScaleY(1.4);
        
        int up_height=-SGLayout::getPoint(kMiddleCenter).y+daySprite->getPosition().y-left_up_kuang->getContentSize().height;
        left_up_kuang->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-left_up_kuang->getContentSize().width/2, up_height-145)));
        m_scrollview = CCScrollView::create(CCSizeMake(540, 860));
         container->setContentSize(CCSizeMake(540, 860));
    }else
    {
        left_up_kuang->setScaleY(1.1);
        right_up_kuang->setScaleY(1.1);
        right_down_kuang->setScaleY(1.1);
        left_down_kuang->setScaleY(1.1);
        int up_height=-SGLayout::getPoint(kMiddleCenter).y+daySprite->getPosition().y-left_up_kuang->getContentSize().height;
        left_up_kuang->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-left_up_kuang->getContentSize().width/2, up_height-40)));
        m_scrollview = CCScrollView::create(CCSizeMake(540, 745));
         container->setContentSize(CCSizeMake(540, 745));
    }
    
    
    right_up_kuang->setPosition(ccpAdd(left_up_kuang->getPosition(), ccp(left_up_kuang->getContentSize().width, 0)));
    right_down_kuang->setPosition(ccpAdd(left_up_kuang->getPosition() , ccp(0, -left_up_kuang->getContentSize().height)));
    left_down_kuang->setPosition(ccpAdd(right_down_kuang->getPosition(), ccp(right_down_kuang->getContentSize().width, 0)));
    
    remindLabel->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,100)));
    qd_btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 20)));
    
    m_scrollview->setContainer(container);
    m_scrollview->setPosition(ccpAdd(left_up_kuang->getPosition(), ccp(-left_up_kuang->getContentSize().width/2+20, -left_up_kuang->getContentSize().height+15)));
    m_scrollview->setDirection(kCCScrollViewDirectionVertical);
    addChild(m_scrollview);
    m_scrollview->getContainer()->removeAllChildrenWithCleanup(true);
    
    
    int positionY =-SECTIONWIDTH ;
    int height=418;
    int min=100;
    leftLq=0;
    
//    LoginRewardObj *object =(LoginRewardObj *)loginArr->objectAtIndex(loginArr->count()-1);
//    int whitchDay=object->getCurrentDay();
   
    int  length=response.model_size();
    for(int i =length-1; i >=0; i--)
    {
        CCLayer *layer = (CCLayer*)m_scrollview->getContainer();
        
        CCArray *dayReward=CCArray::create();
        dayReward->retain();
        main::LoginAwardModel model=response.model(i);
        
        int count=model.loginawarditemmodel_size();
        int CurrentDay=model.day();
        int isfinish=model.isfinish();
        
        
        if (isfinish==0 ) {
            leftLq++;
            min=i;
        }else
        {
            min =i;
        }
        for (int kk=0 ; kk<count; kk++) {
            main::LoginAwardItemModel itemModel=model.loginawarditemmodel(kk);
            
            LoginRewardObj *obj=new LoginRewardObj();
            
            obj->setItemId(itemModel.itemid());
            obj->setItemType(itemModel.itemtype());
            obj->setRewardCount(itemModel.count());
            
            dayReward->addObject(obj);
            obj->autorelease();
        }
        
        SGRewardSubLayer *subLayer=SGRewardSubLayer::create(CurrentDay-day1,CurrentDay,isfinish,dayReward,ccp(0, height), CCSizeMake(539, SECTIONWIDTH));
        
        layer->addChild(subLayer,1,i);
        subLayer->setPosition(ccp(0, positionY+SECTIONWIDTH));
        
        positionY+=SECTIONWIDTH+10;

        
//        LoginRewardObj *obj=(LoginRewardObj *)loginArr->objectAtIndex(i);
//        
//
//        if (obj->getIsFinish()<min && obj->getIsFinish()!=-1) {
//            min=i;
//        }
//        if (whitchDay==obj->getCurrentDay()) {
//            dayReward->addObject(obj);
//            
//            if (i==0) {
//                
//            }
//            continue;
//        }
//        else
//        {
//            SGRewardSubLayer *subLayer=SGRewardSubLayer::create(dayReward,ccp(0, height), CCSizeMake(539, 308));
//            
//            layer->addChild(subLayer,1,i);
//            subLayer->setPosition(ccp(0, positionY+308));
//            
//            positionY+=318;
//            whitchDay=obj->getCurrentDay();
//            dayReward=CCArray::create();
//            dayReward->retain();
//            dayReward->addObject(obj);
//            if (i==0) {
//                SGRewardSubLayer *subLayer=SGRewardSubLayer::create(dayReward,ccp(0, height), CCSizeMake(539, 308));
//                
//                layer->addChild(subLayer,1,i);
//                subLayer->setPosition(ccp(0, positionY+308));
//                
//                positionY+=318;
//                
//            }
//        }
        
        
    }
    
    SGScrollViewSubview *layer1 =SGScrollViewSubview::create(ccp(0, height),  CCSizeMake(539, SECTIONWIDTH));
    
    container->addChild(layer1,1);
    layer1->setPosition(ccp(0, positionY+SECTIONWIDTH));
    positionY=positionY+SECTIONWIDTH;
    
    
    m_scrollview->setContentSize(CCSizeMake(m_scrollview->cocos2d::CCNode::getContentSize().width, positionY));
    m_scrollview->getContainer()->setContentSize(CCSizeMake(m_scrollview->cocos2d::CCNode::getContentSize().width, positionY));
    
    if (winSize.height == 960) {
        m_scrollview->setContentOffset(ccp(0, -positionY+(SECTIONWIDTH +10)*(min+4)+5));

    }else if (winSize.height == 1136) {
        m_scrollview->setContentOffset(ccp(0, -positionY+(SECTIONWIDTH +10)*(min+4)+155));

    }else
    {
        m_scrollview->setContentOffset(ccp(0, -positionY+(SECTIONWIDTH +10)*(min+4)+35));

    }
    
 
}

void SGTotalLoginRewardLayer::getCoinRewardHandlel()
{
    if (leftLq>0) {
        SGMainManager::shareMain()->showMessage(str_RewardYueKaLayer_str10);
        return;
    }
    EFFECT_PLAY(MUSIC_34);
    
    main::RewardLoginAwardGoldRequest *request=new main::RewardLoginAwardGoldRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_GETCOINREWARD_LOGIN, request);
    //this->setTouchEnabled(false);
    //this->m_scrollview->setTouchEnabled(false);
    //this->qd_btn->setEnabled(false);
    
}

void SGTotalLoginRewardLayer::getCoinsRewardListener(CCObject *obj){
    SGSocketRequest *socketRequest=(SGSocketRequest *)obj;
    
    main::RewardLoginAwardGoldResponse *response=(main::RewardLoginAwardGoldResponse *)socketRequest->m_msg;
    if (response && response->state() ) {
        SGCoinRewardBox *propBox=SGCoinRewardBox::create(this, response->coins());
        SGMainManager::shareMain()->showBox(propBox);

        this->qd_btn->setEnabled(false);
        
        
    }else
    {
        if (SGMainManager::shareMain()->getMainLayer()->openByActivity) {
            SGMainManager::shareMain()->getMainLayer()->openByActivity = false;
            SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
           
            
            this->removeFromParentAndCleanup(true);
            SGMainManager::shareMain()->showFirstLayer();
        }
        
        SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
        int nsum = array->count();
        for (int i = 0 ;i < nsum ;i++) {
            SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
            if(item->gettypeItem()==Reward_OpenServer)
            {
                item->setstateItem(0);
                break;
            }
        }
        SGMainManager::shareMain()->showRewardsLayer();

        CCLog("getCoinsRewardListener Error !!!!");
    }

}


void SGTotalLoginRewardLayer::getRewardListener(CCObject *obj){
    
    EFFECT_PLAY(MUSIC_34);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_totalLoginReward);
    SGRewardSubLayer *subLayer=(SGRewardSubLayer *)this->m_scrollview->getContainer()->getChildByTag(SUBLAYRTAG);
    subLayer-> lq_btn->setVisible(false);
    CCSprite *liqu_bottom=CCSprite::createWithSpriteFrameName("reward_login_yilingqu.png");
    liqu_bottom->setPosition(ccp(270+80, 0));
    liqu_bottom->setAnchorPoint(ccp(0, 0));
    subLayer->addChild(liqu_bottom,3);
    
    
    SGSocketRequest *request=(SGSocketRequest *)obj;
    main::RewardLoginAwardResponse *rs=(main::RewardLoginAwardResponse *)request->m_msg;
    
    
    if (rs && rs->state()) {
        
//        
//        this->setTouchEnabled(false);
//        this->m_scrollview->setTouchEnabled(false);
//        this->qd_btn->setEnabled(false);
        this->leftLq--;
        
        
        CCArray *consumables = CCArray::create();
        consumables->retain();
        int count=rs->loginawarditeminfo_size();
        
        
        for (int i = 0; i < count; i++)
        {
            SGMailSystemAccessoryDataModel *access=new SGMailSystemAccessoryDataModel();
            main::LoginAwardItemInfo info=rs->loginawarditeminfo(i);
            
            access->setItemId(info.itemid());
            access->setType(info.itemtype());
            access->setCount(info.count());
            access->autorelease();
            
            consumables->addObject(access);
        }
        SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, consumables);
        SGMainManager::shareMain()->showBox(rewardBox);
        
    }else
    {
        CCLOG("getRewardListener 出错");
    }
    
    
}
void SGTotalLoginRewardLayer::showBoxCall(cocos2d::CCObject *obj){
    //CCLog("执行了这个操作了，哈航");
    //CCLayer *layer = (CCLayer*)m_scrollview->getContainer();
//    for (int i=0; i<length; i++) {
//        SGRewardSubLayer *subLayer=(SGRewardSubLayer *)layer->getChildByTag(i);
////        subLayer->menu->setEnabled(true);
//    }
    this->setTouchEnabled(true);
    this->m_scrollview->setTouchEnabled(true);
    this->qd_btn->setEnabled(true);
    if (!obj) {
        return;
        
    }
    SGMainScene *mainScene=SGMainManager::shareMain()->getMainScene();
    SGFirstLayer *baseLayer = (SGFirstLayer*)mainScene->getChildByTag(sg_firstLayer);
    if (baseLayer) {
        baseLayer->setIsCanTouch(true);
    }
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
//    SGBaseLayer *baseLayer1 = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
//    if (baseLayer1) {
//        baseLayer1->setIsCanTouch(true);
//        
//    }
//    
     SGMainManager::shareMain()->showFirstLayer();
    this->removeFromParentAndCleanup(true);
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId >= guide_tag_45) {
                     //打开公告界面
            main::AnnouncementRequest *request = new main::AnnouncementRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_ANNOUCEMENT_MESSAGE, request);
        }
    
}

