//
//  SGFriendManager.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-1.
//
//

#include "SGFriendManager.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGFriend.h"
#include "SGFriendBase.h"
#include "SGFirstLayer.h"

SGFriendManager::SGFriendManager()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_ADDFRIEND, this, callfuncO_selector(SGFriendManager::friendRequestListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_ADDBLACK, this, callfuncO_selector(SGFriendManager::addBlackListListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_DELETEFRIEND, this, callfuncO_selector(SGFriendManager::deleteFriendListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_AGREEFRIEND, this, callfuncO_selector(SGFriendManager::agreeFriendRequestListener));
    
}

SGFriendManager::~SGFriendManager()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_ADDFRIEND);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_ADDBLACK);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_DELETEFRIEND);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_AGREEFRIEND);
}

void SGFriendManager::deleteFriendListener(cocos2d::CCObject *object)
{
    SGSocketRequest *req = (SGSocketRequest*)object;
    if(!req)
    {
        CCLOG("deleteFriend Error");
        return;
    }
    
    main::DeleteFriendResponse *response = (main::DeleteFriendResponse*)req->m_msg;
    
    if(response->state() == 100)
    {
        //等于100 ，就是对方的操作        
        SGGamePlayer *player = SGFriend::getSGGamePlayer(response->friendproto());
        dealDeleteFriend(player);
        updateData();
    }
}

void SGFriendManager::addBlackListListener(cocos2d::CCObject *object)
{
    SGSocketRequest *req = (SGSocketRequest*)object;
    if(!req)
    {
        CCLOG("addBlaockList Error");
        return;
    }
    
    main::AddBlockResponse *response = (main::AddBlockResponse*)req->m_msg;
    if(response->state() == 100)
    {
        //等于100 ，就是对方的操作
        SGGamePlayer *player = SGFriend::getSGGamePlayer(response->friendproto());
        dealAddBlackList(player);
        updateData();
    }
}

void SGFriendManager::agreeFriendRequestListener(cocos2d::CCObject *object)
{
    SGSocketRequest *req = (SGSocketRequest*)object;
    if(!req)
    {
        CCLOG("agreeFriendRequest Error");
        return;
    }
    
    main::AgreeFriendResponse *response = (main::AgreeFriendResponse*)req->m_msg;
    if(response->state() == 100)
    {
        //等于100 ，就是对方的操作
        SGGamePlayer *player = SGFriend::getSGGamePlayer(response->friendproto());
        dealAgreeFriendRequest(player);
        updateData();
    }
}

void SGFriendManager::friendRequestListener(cocos2d::CCObject *object)
{
    SGSocketRequest *req = (SGSocketRequest*)object;
    if(!req)
    {
        CCLOG("friendRequest Error");
        return;
    }
    
    main::AddFriendResponse *response = (main::AddFriendResponse*)req->m_msg;
    if(response->state() == 100)
    {
        //等于100 ，就是对方的操作
        SGGamePlayer *player = SGFriend::getSGGamePlayer(response->friendproto());
        dealFriendRequest(player);
        updateData();
    }
}

void SGFriendManager::dealAddBlackList(SGGamePlayer *player)
{
    CCArray *arrayFriend = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    
    CCObject *object = NULL;
    CCARRAY_FOREACH(arrayFriend, object)
    {
        SGGamePlayer *friendPlayer = (SGGamePlayer*)object;
        if(friendPlayer->getRoleID() == player->getRoleID())
        {
            SGPlayerInfo::sharePlayerInfo()->getFriendList()->removeObject(player);
            break;
        }
    }
}

void SGFriendManager::dealAgreeFriendRequest(SGGamePlayer *player)
{
    SGPlayerInfo::sharePlayerInfo()->getFriendList()->addObject(player);
}

void SGFriendManager::dealDeleteFriend(SGGamePlayer *player)
{
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getFriendList();
    CCObject *object = NULL;
    CCARRAY_FOREACH(array, object)
    {
        SGGamePlayer *_player = (SGGamePlayer*)object;
        if(_player->getFriendID() == player->getFriendID())
        {
            array->removeObject(_player);
            break;
        }
    }
}

void SGFriendManager::dealFriendRequest(SGGamePlayer *player)
{
    SGPlayerInfo::sharePlayerInfo()->getApplyList()->addObject(player);
}

void SGFriendManager::updateData()
{
    //我的好友界面
    SGFriendBase *delegate = (SGFriendBase*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_myFriend);
    if(delegate)
    {
        delegate->updateData();
    }
    
    //请求好友界面
    SGFriendBase *delegate1 = (SGFriendBase*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendRequest);
    if(delegate1)
    {
        delegate1->updateData();
    }
    
    //黑名单界面
    SGFriendBase *delegate2 = (SGFriendBase*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_blackFriend);
    if(delegate2)
    {
        delegate2->updateData();
    }
    
    //刷新首页的请求个数
    SGFirstLayer *firstLayer = (SGFirstLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
     if(firstLayer)
     {
         firstLayer->setFriendCount(SGPlayerInfo::sharePlayerInfo()->getApplyList()->count());
     }
    
    //刷新好友请求个数
    SGFriend *fr = (SGFriend*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendLayer);
    if(fr)
    {
        fr->updateData();
    }
    
 
}