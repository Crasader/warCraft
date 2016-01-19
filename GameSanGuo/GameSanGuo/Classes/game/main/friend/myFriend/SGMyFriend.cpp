//
//  SGMyFriend.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGMyFriend.h"
#include "SGMyFriendCell.h"
#include "SGMainManager.h"
#include "SGFriend.h"
#include "SGGamePlayer.h"
#include "SGPlayerInfo.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGMyFriend::SGMyFriend()
{
    _model = true;
    buttonSend = NULL;
    buttonBlack = NULL;
    _arrayFriend = NULL;
    
    _touch = true;
}

SGMyFriend::~SGMyFriend()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_myFriend);
	
    _arrayFriend->release();
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_DELETEFRIEND);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PRESENTPOW);
    
}


void SGMyFriend::setEnabletouch(bool b)
{
    int sum = datas->count();
    for(int i = 0; i<sum; i++)
    {
     SNSIndexPath* indexPath = SNSIndexPath::create(i, 0);
     SGMyFriendCell* ptabeitem = (SGMyFriendCell*)tableView->getCellByIndexPath(indexPath);
        //ptabeitem->setMenuEnable(false);
    }
    
}

SGMyFriend* SGMyFriend::create(CCArray *arrayFriend)
{
    SGMyFriend *myFriend = new SGMyFriend();
    myFriend->_arrayFriend = arrayFriend;
    myFriend->_arrayFriend->retain();
    
    if(myFriend && myFriend->init(NULL, sg_myFriend))
    {
        myFriend->constructView();
        myFriend->autorelease();
        return myFriend;
    }
    
    CC_SAFE_DELETE(myFriend);
    return NULL;
}

void SGMyFriend::constructView()
{
    this->datas = _arrayFriend;
    shownoFriendTip(str_FriendCountFull_str10);

    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_DELETEFRIEND, this, callfuncO_selector(SGMyFriend::deleteFriendListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PRESENTPOW, this, callfuncO_selector(SGMyFriend::sendTiliListener));
    
    
    //我的好友
	ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI, sg_myFriend);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_myFriend);
	ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_myFriend);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_myFriend);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_myFriend);
	 float h = SGMainManager::shareMain()->getTotleHdHgt();
	CCPoint upCenter = SGLayout::getPoint(kUpCenter);
//    CCSprite *sprite = CCSprite::createWithSpriteFrameName("friend_myFriend.png");
    SGCCLabelTTF *sprite = SGCCLabelTTF::create(str_FriendCountFull_str11, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//sprite->setColor(ccc3(0xff, 0x95, 0x0c));
//    sprite->setColor(ccc3(0xe7, 0xb6, 0x7b));
    sprite->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(sprite);
    spriteTitle->setVisible(false);

    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    CCSize s=CCDirector::sharedDirector()->getWinSize();


    
    labelOnline->setString(this->getPeopleCountOnline()->getCString());

    buttonSend = SGButton::create("chatbtn.png", "friend_deleteModel.png", this, menu_selector(SGMyFriend::buttonClickSendLetter),CCPointZero,false,true);
    buttonBlack = SGButton::create("chatbtn.png", "friend_sendModel.png", this, menu_selector(SGMyFriend::buttonClickBlack),CCPointZero,false,true);
    buttonSend->setAnchorPoint(ccp(1, 0.5));
    buttonBlack->setAnchorPoint(ccp(1, 0.5));
    buttonSend->setPosition(ccp(SGLayout::getPoint(KMiddleRight).x, buttonBack->getPosition().y));
    buttonBlack->setPosition(buttonSend->getPosition());    
    this->addBtn(buttonSend);
    this->addBtn(buttonBlack);
    this->setSendLetterIsShow(_model);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
   // CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
   
    if(SGPlayerInfo::sharePlayerInfo()->checkIsSkipDayPlaying())
    {
        SGPlayerInfo::sharePlayerInfo()->resetSendFriendTili();
    }
    else
    {
        schedule(schedule_selector(SGMyFriend::checkDuringSkipDay) , 2*60);
    }
}

void SGMyFriend::checkDuringSkipDay()
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    if(playerInfo->checkIsSkipDayPlaying())
    {
        playerInfo->resetSendFriendTili();
        this->datas = playerInfo->getFriendList();
        _arrayFriend->release();
        _arrayFriend = playerInfo->getFriendList();
        _arrayFriend->retain();
        
        tableView->reloadData();
        unschedule(schedule_selector(SGMyFriend::checkDuringSkipDay));
    }
}

void SGMyFriend::deleteFriendListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("deleteFriendListener");
        return;
    }
    
    main::DeleteFriendResponse *response = (main::DeleteFriendResponse*)request->m_msg;
    if(0 == response->state())
    {
        SG_SHOW_WINDOW(str_FriendCountFull_str12);
    }else if(1 == response->state()){
        
        this->refreshData(_selectRoleId);
        SG_SHOW_WINDOW(str_FriendCountFull_str13);
    }
}
void SGMyFriend::sendTiliListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        return;
    }
    main::PowerGiftResponse *response = (main::PowerGiftResponse*)request->m_msg;
    //do nothing
}

//发信模式 删除模式
void SGMyFriend::setSendLetterIsShow(bool isShow)
{
    buttonBlack->setVisible(!isShow);   
    buttonSend->setVisible(isShow);
  
}

//获取在线人数及总人数
CCString* SGMyFriend::getPeopleCountOnline()
{
    /*
    int arrayCount = _arrayFriend->count();
    int onlineCount = 0;
    for(int i=0;i<arrayCount;i++)
    {
        SGGamePlayer *player = (SGGamePlayer*)_arrayFriend->objectAtIndex(i);
        if(player->getIsOnline())
        {
            onlineCount ++;
        }
    }
     
     据说这里不是显示的在线人数，而是显示的总人数与最大可容纳人数悲剧了
     留着在线人数的计算，没准什么时候会用的
     
     */
    
    int friendSize = SGPlayerInfo::sharePlayerInfo()->getFriendSize();
    CCString *strPeopleCount = CCString::createWithFormat("(%d/%d人)",_arrayFriend->count(),friendSize);
    
    return strPeopleCount;
}

SNSTableViewCellItem* SGMyFriend::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGMyFriendCell *cell = (SGMyFriendCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if(NULL == cell)
    {
        cell = SGMyFriendCell::create(_model,(SGGamePlayer*)this->datas->objectAtIndex(indexPath->getRow()),this);
    }else{
        cell->upDate(_model , (SGGamePlayer*)this->datas->objectAtIndex(indexPath->getRow()));
    }
    cell->setIsCanTouch(_touch);
    cell->setParentLayer(this);
    return cell;
}

void SGMyFriend::buttonClickBlack()
{
    EFFECT_PLAY(MUSIC_BTN);
    _model = true;
    this->datas = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    this->tableView->reloadData();
    this->setSendLetterIsShow(_model);
}

void SGMyFriend::buttonClickSendLetter()
{
    EFFECT_PLAY(MUSIC_BTN);
    _model = false;
    this->datas = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    this->tableView->reloadData();
    this->setSendLetterIsShow(_model);

}

void SGMyFriend::refreshData(int roleId)
{
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    SGGamePlayer *player = NULL;
    for (int ii = 0; ii < array->count(); ii++) {
        player = (SGGamePlayer *)array->objectAtIndex(ii);
        if (player->getFriendID() == roleId) break;
    }
    if (player != NULL) {
        SGPlayerInfo::sharePlayerInfo()->getFriendList()->removeObject(player);
        this->datas = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    }
    this->tableView->reloadData();
    labelOnline->setString(this->getPeopleCountOnline()->getCString());
    shownoFriendTip(str_FriendCountFull_str10);
}

void SGMyFriend::cellCallBack(SGGamePlayer *player , bool type)
{
    SGPlayerInfo::sharePlayerInfo()->getFriendList()->removeObject(player);
    this->datas = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    this->tableView->reloadData();
    labelOnline->setString(this->getPeopleCountOnline()->getCString());
    shownoFriendTip(str_FriendCountFull_str10);
}

void SGMyFriend::updateData()
{
    tableView->reloadData();
    shownoFriendTip(str_FriendCountFull_str10);
}



