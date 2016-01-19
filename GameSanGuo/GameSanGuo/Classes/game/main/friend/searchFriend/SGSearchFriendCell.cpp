//
//  SGSearchFriendCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#include "SGSearchFriendCell.h"
#include "SGPlayerInfo.h"
#include "FriendModule.pb.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSearchFriend.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGSearchFriendCell::SGSearchFriendCell():_isFriend(false) , buttonControl(NULL)
{
    
}

SGSearchFriendCell::~SGSearchFriendCell()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_ADDBLACK);
}

SGSearchFriendCell* SGSearchFriendCell::create(SGGamePlayer *player , bool isFriend , SGFriendBaseDelegate *friendDelegate)
{
    SGSearchFriendCell *cell = new SGSearchFriendCell();
    cell->_player = player;
    cell->_isFriend = isFriend;
    cell->_friendDelegate = friendDelegate;
    if(cell && cell->viewDidLoad())
    {
     
        cell->constructView();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;    
}

void SGSearchFriendCell::constructView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_ADDBLACK, this, callfuncO_selector(SGSearchFriendCell::addBlackListener));
 
    // buttonControl = SGButton::create("public_btn_1.png",NULL,this, menu_selector(SGSearchFriendCell::buttonClickControl),CCPointZero,false,true);
    ///
    buttonControl = SGButton::create("public_btn_1.png","friend_friend_1.png",this, menu_selector(SGSearchFriendCell::buttonClickControl),CCPointZero,false,true);
    buttonControl->setFontImageScale(0.9);
    /////
  //SGButton*buttonControl1 = SGButton::create("public_btn_1.png","friend_friend_1.png",this, NULL,CCPointZero,false,true);
    /////
    buttonControl->setFontImage(CCSprite::createWithSpriteFrameName(this->getButtonImageName()->getCString())->displayFrame());
    buttonControl->setPosition(ccp(200, 0));
    //buttonControl1->setPosition(buttonControl->getPosition());
    menu->addChild(buttonControl);
    //menu->addChild(buttonControl1,1);
    update(_player, _isFriend);
}

//获取应该显示的button图片
CCString* SGSearchFriendCell::getButtonImageName()
{
    CCString *string = CCString::create("");
    if(_isFriend)
    {
        bool haveHe = false;
        CCArray* arrayFriend = SGPlayerInfo::sharePlayerInfo()->getFriendList();
        for(int i=0;i<arrayFriend->count();i++)
        {
            SGGamePlayer *player = (SGGamePlayer*)arrayFriend->objectAtIndex(i);
            if(_player->getRoleID() == player->getFriendID())
            {
                //在好友列表里面
                string->setString("friend_friend_2.png");
                buttonControl->setEnabled(false);
                haveHe = true;
                break;
            }
       }
        
        if(!haveHe)
        {
            //没在好友列表里面
            string->setString("friend_friend_1.png");
            buttonControl->setEnabled(true);
        }
        
    }else{
        
        bool haveHe = false;

        CCArray *arrayBlack = SGPlayerInfo::sharePlayerInfo()->getBlackList();
        
        for(int i=0;i<arrayBlack->count();i++)
        {
            SGGamePlayer *player = (SGGamePlayer*)arrayBlack->objectAtIndex(i);
           // CCLog("%d",player->getFriendID());
            if(player->getFriendID() == _player->getRoleID())
            {
                //在黑名单中
                string->setString("friend_black_2.png");
                buttonControl->setEnabled(false);
                haveHe = true;
                break;
            }
        }
        
        if(!haveHe)
        {
            //没在黑名单中
            string->setString("friend_black_1.png");
            buttonControl->setEnabled(true);
        }
    }
    
    return string;
}

//cell刷新
void SGSearchFriendCell::update(SGGamePlayer *player , bool isFriend)
{
    this->_player = player;
    this->_isFriend = isFriend;
    this->updateCellBySGamePlayer(_player);
    buttonControl->setFontImage(CCSprite::createWithSpriteFrameName(this->getButtonImageName()->getCString())->displayFrame());
}

void SGSearchFriendCell::buttonClickControl()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    if(_isFriend)
    {
        if(SGFriend::checkFriendIsSelf(_player->getRoleID()))
        {
            SG_SHOW_WINDOW(str_FriendCountFull_str24);
            return;
        }
        
        //记录添加好友id
        SGFriendBase::addRoleId = _player->getRoleID();
        
        main::AddFriendRequest *add = new main::AddFriendRequest();
        add->set_friendroleid(_player->getRoleID());
        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_ADDFRIEND, add);
        
    }else{
        
        if(SGFriend::checkFriendIsSelf(_player->getRoleID()))
        {
            SG_SHOW_WINDOW(str_SearchFriend_str9);
            return;
        }
     
        main::AddBlockRequest *addblack = new main::AddBlockRequest();
        addblack->set_friendroleid(_player->getRoleID());
        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_ADDBLACK, addblack);
        
    }
}

//添加黑名单监听
void SGSearchFriendCell::addBlackListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("addBlackListener");
        return;
    }
    
    main::AddBlockResponse *response = (main::AddBlockResponse*)request->m_msg;
    //0; 异常失败
    //1; 已经在你或者他的好友列表中，解除双方好友关系。添加黑名单
    //2; 在我的申请列表中，删除他的申请请求，添加进我的黑名单
    //3; 在他的申请列表中，删除我的申请请求，添加进我的黑名单
    //4; 正常添加
    
    switch(response->state())
    {
        case 0:
        {
            SG_SHOW_WINDOW(str_ChatWithGMInputBox_str5);
            break;
        }
            
        case 1:
        {
            //如果好友列表中有，删除
            CCObject *object = NULL;
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getFriendList();
            CCARRAY_FOREACH(array, object)
            {
                SGGamePlayer *player = (SGGamePlayer*)object;
                if(_player->getRoleID() == player->getFriendID())
                {
                    _friendDelegate->cellCallBack(_player, false);
                    array->removeObject(player);
                    
                }
            }
            break;
        }
        case 2:
        {
            //如果好友请求列表中有，删除
            CCObject *object = NULL;
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getApplyList();
            CCARRAY_FOREACH(array, object)
            {
                SGGamePlayer *player = (SGGamePlayer*)object;
                if(_player->getRoleID() == player->getFriendID())
                {
                    _friendDelegate->cellCallBack(_player, false);
                    array->removeObject(player);
                }
            }

            break;
        }
        case 3:
        case 4:
        {
            
            SG_SHOW_WINDOW(str_FriendBase_str11);
            _friendDelegate->cellCallBack(_player, false);
            
            SGSearchFriend *search = SGSearchFriend::create(false);
            SGMainManager::shareMain()->showLayer(search);
            break;
        }
    }
}

