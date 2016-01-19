//
//  SGFriendRequest.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGFriendRequest.h"
#include "SGMainManager.h"
#include "SGFriendRequestCell.h"
#include "SGFriend.h"
#include "SGPlayerInfo.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGFriendRequest::SGFriendRequest()
{
    _arrayFriendRequest = NULL;
}

SGFriendRequest::~SGFriendRequest()
{
    _arrayFriendRequest->release();
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_friendRequest);
	 
	
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_AGREEFRIEND);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_UNAGREEADDFRIEND);
}


SGFriendRequest* SGFriendRequest::create(CCArray *arrayFriendRequest)
{
    SGFriendRequest *myFriend = new SGFriendRequest();
    myFriend->_arrayFriendRequest = arrayFriendRequest;
    myFriend->_arrayFriendRequest->retain();
    
    if(myFriend && myFriend->init(NULL, sg_friendRequest))
    {
        
        myFriend->constructView();
        myFriend->autorelease();
        return myFriend;
    }
    
    CC_SAFE_DELETE(myFriend);
    return NULL;
}

void SGFriendRequest::constructView()
{
    this->datas = _arrayFriendRequest;
    shownoFriendTip(str_FriendCountFull_str3);
    
    // 好友申请列表
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_AGREEFRIEND, this, callfuncO_selector(SGFriendBase::agreeFriendListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_UNAGREEADDFRIEND, this, callfuncO_selector(SGFriendBase::refuseAddFriendListener));
    

	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendRequest);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_friendRequest);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_friendRequest);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    float h = SGMainManager::shareMain()->getTotleHdHgt();

    SGCCLabelTTF *sprite = SGCCLabelTTF::create(str_FriendCountFull_str4, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//sprite->setColor(ccc3(0xff, 0x95, 0x0c));
    sprite->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(sprite);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    //我的好友
//    CCSprite *sprite =  CCSprite::createWithSpriteFrameName("friend_friendRequest.png");
//    spriteTitle->setDisplayFrame(sprite->displayFrame());
    spriteTitle->setVisible(false);
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *titlecenter =  CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
}

SNSTableViewCellItem* SGFriendRequest::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
    SGFriendRequestCell *cell = (SGFriendRequestCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if(NULL == cell)
    {
        cell = SGFriendRequestCell::create((SGGamePlayer*)_arrayFriendRequest->objectAtIndex(indexPath->getRow()),this,this);
    }else{
        cell->upDate((SGGamePlayer*)_arrayFriendRequest->objectAtIndex(indexPath->getRow()));
    }
        cell->setIsCanTouch(_touch);
    cell->setParentLayer(this);
    return cell;
}

//cell回调  刷新界面
void SGFriendRequest::cellCallBack(SGGamePlayer *player , bool type)
{
    if(type)
    {
        SGPlayerInfo::sharePlayerInfo()->getFriendList()->addObject(player);
    }
    
    SGPlayerInfo::sharePlayerInfo()->getApplyList()->removeObject(player);
    this->datas = SGPlayerInfo::sharePlayerInfo()->getApplyList();
    this->tableView->reloadData();
    shownoFriendTip(str_FriendCountFull_str5);
}

void SGFriendRequest::refreshData(int roleId)
{
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getApplyList();
    SGGamePlayer *player = NULL;
    for (int ii = 0; ii < array->count(); ii++) {
        player = (SGGamePlayer *)array->objectAtIndex(ii);
        if (player->getFriendID() == roleId) break;
    }
    if (player != NULL) {
        SGPlayerInfo::sharePlayerInfo()->getFriendList()->addObject(player);
        SGPlayerInfo::sharePlayerInfo()->getApplyList()->removeObject(player);
        this->datas = SGPlayerInfo::sharePlayerInfo()->getApplyList();
    }
    this->tableView->reloadData();
}

void SGFriendRequest::updateData()
{
    tableView->reloadData();
    shownoFriendTip(str_FriendCountFull_str6);

}

