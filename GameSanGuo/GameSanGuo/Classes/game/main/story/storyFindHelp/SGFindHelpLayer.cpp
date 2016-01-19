	//
//  SGFindHelpLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGFindHelpLayer.h"
#include "SGMainManager.h"
#include "SGFindHelpCell.h"
#include "SGFHelpSortLayer.h"
#include "SGStorySectionCell.h"
#include "SGStoryBattleLayer.h"
#include "SGBattleManager.h"
#include "SGStaticDataManager.h"
#include "SGPveHelper.h"
#include "SGQFightBox.h"
#include "SGStoryReward.h"
#include "SGStoryRewardBox.h"
#include "SGStoryRewardItem.h"
#include "SGAIManager.h"
#include "SGMainLayer.h"

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PlotModule.pb.h"
#include "SGGeneralInfoLayer.h"

#include "SGFriend.h"
#include "SGOtherPlayerInfo.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

USING_NS_CC;

SGFindHelpLayer::SGFindHelpLayer(void): _isTouch(true)
,roleID(0)
, _plotId(1)
, _isStoryModel(false)
, _selectIndex(0)
, _selectRoleId(0)
,sortBtn(NULL)
,storyBean(NULL)
,isBattle_Rend(false)
{

}
SGFindHelpLayer::~SGFindHelpLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_findHelplayer);
    SGNotificationCenter::sharedNotificationCenter() -> removeObserver(this,MSG_FRIEND_PLAYERINFO);
}

SGFindHelpLayer *SGFindHelpLayer::create(int plotId, bool model)
{
    
    SGFindHelpLayer *helpLayer = new SGFindHelpLayer();
    if (helpLayer && helpLayer->init(NULL, sg_findHelplayer))
    {
        helpLayer-> _plotId = plotId;
        helpLayer->initView(model);
        helpLayer->autorelease();
        return helpLayer;
    }
    CC_SAFE_DELETE(helpLayer);
    
    return NULL;
}

void SGFindHelpLayer::initView(bool model)
{
    
    _isStoryModel = model;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGFindHelpLayer::showPlayerInfoListener));

    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_findHelplayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgfindhelplayer/sgfindhelplayer.plist", RES_TYPE_LAYER_UI, sg_findHelplayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_findHelplayer);
	
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_findHelplayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, 1136/2-(size.height-headH-bottomH)/2, bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -1);
    bgImg->setVisible(false);
    
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

    SGCCLabelTTF *title = SGCCLabelTTF::create(str_OverBattleBox_str24, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);

    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, size.height - headH)));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5f)));
    this->addChild(title);
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGFindHelpLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.65f, title->getPosition().y));
    this->addBtn(backBtn);
    
    sortBtn = SGButton::create("chatbtn.png", "btn_story_range.png", this,
                                         menu_selector(SGFindHelpLayer::sortHandler), ccp(0, 0), false, true);
    sortBtn->setPosition(ccp(size.width-sortBtn->getContentSize().width*0.65f, title->getPosition().y));
    
    //援军界面的tableview的tag == 506
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_6);
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(12)));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    SGAIManager::shareManager()->isPVE = true;
}

void SGFindHelpLayer::sortHandler(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    SGFHelpSortLayer *sortLayer = SGFHelpSortLayer::create(datas, this, this, sortBtn->getPosition());
    SGMainManager::shareMain()->addShowLayer(sortLayer, false);
    
}

void SGFindHelpLayer::backHandler(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_isStoryModel) {
        SGMainManager::shareMain()->m_trackIsPve = true;
        SGMainManager::shareMain()->showStorySectionLayer(storyBean);
    }else {
        SGMainManager::shareMain()->m_trackIsPve = false;
        SGMainManager::shareMain()->showActivityLayer();
    }
}

void SGFindHelpLayer::initDatas(CCArray *data)
{
    SGStoryBean *bean = NULL;
    CCArray *array = NULL;
    if (_isStoryModel == true) {
        array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
    }else {
        array = SGPlayerInfo::sharePlayerInfo()->getActivityData();
        
    }
    if (array == NULL) { //教学中断
        bean = SGStaticDataManager::shareStatic()->getStoryDataById(1);
    }else {
        array->retain();
        for (int ii = 0; ii < array->count(); ii++) {
            SGStoryBean *story = (SGStoryBean *)array->objectAtIndex(ii);
            if (story->getSceneBeanById(_plotId)) {
                bean = story;
                break;
            }
        }
    }
    
    storyBean = bean;
    
    if (this->datas) {
        this->datas->release();
        this->datas = NULL;
    }
    this->datas = CCArray::create();
    this->datas->retain();
    
    for (int ii = 0; ii < data->count(); ii++) {
        CCObject *obj = data->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
}

#pragma mark -
#pragma mark - -----SGBaseSortLayer Delegate-----
void SGFindHelpLayer::sortLayerCallBack(CCArray *data)
{
    this->datas = data;
    this->tableView->reloadData();
}

#pragma mark -
#pragma mark - -----tableView actions-----

SGFindHelpCell* SGFindHelpLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)   //cgp_pro
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	SGFindHelpCell *item = (SGFindHelpCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL)
    {

        item = SGFindHelpCell::create(this);

    }
    if (index < datas->count())
    {
        item->setData((SGPveHelper *)datas->objectAtIndex(index));
    }
    else
    {
        item->setVisible(false);
    }
    item->setIsCanTouch(_isTouch);
    return item;
    
    
}

unsigned int SGFindHelpLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}
///////////////////////////////////////////////////////////
void SGFindHelpLayer::itemselect(SNSIndexPath *indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    if (isBattle_Rend)
    {
        return;
    }
    isBattle_Rend = true;
    
    this -> setIsCanTouch(false);
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    if (index < datas->count())
    {
        _selectIndex = index;
		
		//初始化找朋友帮忙的数据
		SGPveHelper *roler = (SGPveHelper *)datas->objectAtIndex(index);
		roleID = roler->getRoleId();
		HelpFriendRoleName = roler->getRoleName()->getCString();
		HelpFriendRoleId=roler->getRoleId();
		HelpFriendItemId = roler->getItemId();
		HelpFriendItemLevel = roler->getItemLevel();
		isInMyFriendList = roler->getInMyFriendList();
		
        this->gotoFightingLayer(index);
 
    }
    else
    {
        // error catch
    }

}
/////////////////////////////////////////////////////
void SGFindHelpLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    this -> setIsCanTouch(false);
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    if (index < datas->count()) {
//        _selectIndex = index;
//        SGStorySceneBean *bean = (SGStorySceneBean *)storyBean->getSceneBeanById(_plotId);
//        
//        SGPveHelper *roler = (SGPveHelper *)datas->objectAtIndex(index);
//        roleID = roler->getRoleId();
//        roleName = roler->getRoleName()->getCString();
//        RoleId=roler->getRoleId();
//        
//        int vipLv = SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel();
//        SGPlayerInfo::sharePlayerInfo()->setVictory(bean->getRank());
//        if (bean->getRank() == 0) { // 第一次打，直接战斗
//            this->gotoFightingLayer(index);
//        }
//        else {
//            if (bean->getRank()==3 || (vipLv>=4 && bean->getRank()==2) ) { // 完胜，提示快速战斗
//                
//                SGQFightBox *box = SGQFightBox::create(this, index);
//                box->setDatas(_plotId, roler->getRoleId(), storyBean->getStoryId(),bean->getRank()==3 || (vipLv>=4 && bean->getRank()==2));
//                SGMainManager::shareMain()->showBox(box);
//            }else {
//                this->gotoFightingLayer(index);
//            }
//        }
//
//    }else {
//        // error catch
//    }
}



void SGFindHelpLayer::gotoFightingLayer(int index)
{
    SGPveHelper *roler = (SGPveHelper *)datas->objectAtIndex(index);
    //CCLog("%d,",roler->getRoleId());
   // CCLog("%d",_plotId);
   // CCLog("%d",storyBean->getStoryId());
    
    SGBattleManager::sharedBattle()->pveStart(storyBean->getStoryId(), _plotId, roler->getRoleId());
}

int SGFindHelpLayer::getRoleId()
{
    SGPveHelper *roler = (SGPveHelper *)datas->objectAtIndex(_selectIndex);
    DDLog("cc------------------%d", _selectIndex);
    roleID = roler->getRoleId();
    HelpFriendRoleName = roler->getRoleName()->getCString();
    
    return roleID;
}

void SGFindHelpLayer::setIsCanTouch(bool isTouch)
{
    _isTouch = isTouch;
    this -> setTouchEnabled(isTouch);
    SGBaseTableLayer::setIsCanTouch(isTouch);
 
    isAchieve_tableViewSelectItem = isTouch;
    
    int num = tableView->getRowCount();
    SGFindHelpCell *item = NULL;
    SNSIndexPath *indexpath = NULL;
    for (int i=0; i<num; i++) {
        indexpath = SNSIndexPath::create(i, 0, 1);
        item = (SGFindHelpCell*)tableView->dequeueReusableCellItemForIndexPath(indexpath);
        if (item) {
            item->setIsCanTouch(isTouch);
        }
    }

}


void SGFindHelpLayer::showPlayerInfoListener(cocos2d::CCObject *obj)
{
   
    SGSocketRequest *req = (SGSocketRequest*)obj;
    if(!req)
    {
        CCLOG("Show Player Information ");
        return;
    }
    
    
    main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)req->m_msg;
    SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
    //CCLOG("%s",playerInfo->getNickName()->getCString());
    
    int friendType = SGFriend::isFriendOfPlayer(playerInfo->getRoleId());
    int generalLayerType = 0;
    if(SGFRIEND_FRIEND == friendType)
    {
        generalLayerType = 100;
    }else if(SGFRIEND_BLACK == friendType)
    {
        generalLayerType = 5;
    }else if(SGFRIEND_REQUEST == friendType)
    {
        generalLayerType = 6;
    }else if(SGFRIEND_STRANGER == friendType)
    {
        generalLayerType = 4;
    }
    
    SGOfficerCard *officer = playerInfo->getLordOfficer();
    //改为使用CIB。
    SGMainManager::shareMain()->showCommonInfoBoxInProto(officer, this, -1, false, CIBET_FIND_HELP);
    
    //留档勿删。
//    officer->retain();
//    SGGeneralInfoLayer *info = SGGeneralInfoLayer::create(officer, generalLayerType,NULL, NULL,false, NULL,true);
//    info->setStoryID(storyBean->getStoryId());
//    info->setPlotID(_plotId);
//    info->setRoldID(roleInfo->role().roleid());
//    SGMainManager::shareMain()->showLayer(info,false);
//    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
//
//    info -> setTouchEnabled(true);
    
}
void SGFindHelpLayer::setRoleID(int roleId){
    roleID=HelpFriendRoleId;
    
}


