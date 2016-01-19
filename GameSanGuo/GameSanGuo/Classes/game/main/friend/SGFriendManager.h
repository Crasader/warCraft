//
//  SGFriendManager.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-1.
//
//


/*************************类的功能***********************************
 
 这个类实例化一个常驻内存的对象，用于作为好友功能的处理在游戏中打开好友界
 面的时间不是很多，但是需要处理的信息不少，比如被加好友， 被删好友这种被
 动的操作，就需要有一个后台的响应模块，这个类就作为这样一个 后台的响应模
 块，来处理 好友中的后台收到的各种信息
 
 ************************************************************/



#ifndef __GameSanGuo__SGFriendManager__
#define __GameSanGuo__SGFriendManager__

#include <iostream>
#include "SGGamePlayer.h"
#include "SGPlayerInfo.h"
#include "FriendModule.pb.h"

class SGFriendManager : public CCObject
{
private:
    
    
public:
    SGFriendManager();
    
    ~SGFriendManager();
    //好友请求监听
    void friendRequestListener(CCObject *object);
    
    //同意好友请求监听
    void agreeFriendRequestListener(CCObject *object);
    
    //删除好友监听
    void deleteFriendListener(CCObject *object);
    
    //添加黑名单监听
    void addBlackListListener(CCObject *object);
    
    //处理好友请求
    void dealFriendRequest(SGGamePlayer *player);
    
    //处理同意好友请求
    void dealAgreeFriendRequest(SGGamePlayer *player);
    
    //处理删除好友
    void dealDeleteFriend(SGGamePlayer *player);
    
    //处理被添加黑名单
    void dealAddBlackList(SGGamePlayer *player);
    
    //刷新数据
    void updateData();
    
    
};



#endif /* defined(__GameSanGuo__SGFriendManager__) */
