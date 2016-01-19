//
//  SGChatMessageCenter.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-11.
//
//

#include "SGChatMessageCenter.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "MessageModule.pb.h"
#include "SGChat.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGMainLayer.h"
#include "SGChat.h"
#include "SGStringConfig.h"
static SGChatMessageCenter *chat = NULL;
SGChatMessageCenter::SGChatMessageCenter()
{
    isGetData = false;
}

SGChatMessageCenter::~SGChatMessageCenter()
{
    _arrayChat->release();
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHAT_MESSAGE);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHAT_GETDATA);
}

//写成单例类
SGChatMessageCenter* SGChatMessageCenter::shareChatMessageCenter()
{
    if(NULL == chat)
    {
        chat = new SGChatMessageCenter();
        chat->init();
    }
    
    return chat;
}

//初始化界面
void SGChatMessageCenter::init()
{
    //聊天的内容
    _arrayChat = CCArray::create();
    _arrayChat->retain();
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CHAT_MESSAGE, this, callfuncO_selector(SGChatMessageCenter::chatMessageListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CHAT_GETDATA, this, callfuncO_selector(SGChatMessageCenter::getChatDataListener));
    
}

//网络监听
void SGChatMessageCenter::chatMessageListener(cocos2d::CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("chatMessageListener error !!! ");
        return;
    }
    
    main::SendMessageResponse *response = (main::SendMessageResponse*)request->m_msg;
	
	
	
	// 聊天里新加了一个字段表明,表示聊天消息是不是发送成功 @wangyang sendChatState
	if (!response->sendchatstate())
	{
		SGChatMessage *chat = getChatMessageByChatProto(response->chat());
		
		//如果在黑名单中，不接收这条消息
		if(roleIsInBlacklist(chat->getRoleId()))
		{
			return;
		}
		
		_arrayChat->addObject(chat);
		
		SGMainLayer *mainlayer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
		if(mainlayer)
		{
			mainlayer->sendMessage(chat);
		}
		
		SGChat *message = (SGChat*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatLayer);
		if(message)
		{
			message->sendMessage(chat);
		}
	}
	else if (response->sendchatstate() == 1)
	{
		SG_SHOW_WINDOW(str_ChatMessageCenter1);
		return;
	}
	else
	{
		SG_SHOW_WINDOW(str_ChatMessageCenter2);
		return;
	}

}

//检查这个人是不是在黑名单中
bool SGChatMessageCenter::roleIsInBlacklist(int roleId)
{
    CCArray *blackList = SGPlayerInfo::sharePlayerInfo()->getBlackList();
    int blacklistCount = blackList->count();
    
    for(int i=0;i<blacklistCount;i++)
    {
        SGGamePlayer *role = (SGGamePlayer*)blackList->objectAtIndex(i);
        if(role->getFriendID() == roleId)
        {
            return true;
        }
    }
    return false;
}

//转换数据
SGChatMessage* SGChatMessageCenter::getChatMessageByChatProto(main::ChatProto chatproto)
{
    SGChatMessage *chat = new SGChatMessage();
    chat->setChannelId(chatproto.chid());
    
    if (chatproto.content().size() > sizeof(SGCHATSHOWTAGSTR)) {
        
        std::string chatShow = chatproto.content().substr(0,sizeof(SGCHATSHOWTAGSTR)-1);
        if (strcmp(SGCHATSHOWTAGSTR, chatShow.c_str()) == 0) {
            char *showTag = new char[128];
            char *showName= new char[128];
            int showStar = 0;
            int showSSid = 0;
            int showType = 0;
            sscanf(chatproto.content().c_str(), "%s - %s - %d - %d - %d", showTag,showName,&showType,&showStar,&showSSid);
            //异常处理
            if (showStar < 1 || showStar > 10 || showSSid < 1 || showType < 1) {
                chat->setContent(CCString::create(chatproto.content()));
            }
            else
            {
                chat->setIsChatShowHaHa(true);
                chat->setChatShowyName(CCString::createWithFormat("〈%s〉",showName));
                chat->setChatShowType(showType);
                chat->setChatShowStarLevel(showStar);
                chat->setChatShowSSid(showSSid);
                
                chat->setContent(CCString::create(showName));
            }
            delete []showTag;
            delete []showName;
            
        }
        else
        {
          chat->setContent(CCString::create(chatproto.content()));
        }
    }
    else
    {
        chat->setContent(CCString::create(chatproto.content()));
    }
    
    chat->setRoleId(chatproto.roleid());
    chat->setRoleNickName(CCString::create(chatproto.rolenickname()));
    chat->setImage(chatproto.image());
    chat->setTimeStr(CCString::create(chatproto.timestr()));
    chat->setVipLevel(chatproto.viplevel());
    //FIXME: pvprankname
    chat->setMilitaryName(CCString::create(chatproto.pvprankname()));
    chat->setCombateValue(chatproto.combatvalue());
    chat->setSoliderID(chatproto.lorditemid());
    chat->autorelease();
    return chat;
    
}

//获取聊天数组
CCArray* SGChatMessageCenter::getArrayCount()
{
    return _arrayChat;
}

//打开聊天室
void SGChatMessageCenter::openChatRoom()
{
    if(isGetData)
    {
        SGMainManager::shareMain()->showChatLayer();
    }else{
        main::ShowMessageRequest *showMessage = new main::ShowMessageRequest();
        showMessage->set_chid(1);
        SGSocketClient::sharedSocketClient()->send(MSG_CHAT_GETDATA , showMessage);
        
    }
}

//监听数据
void SGChatMessageCenter::getChatDataListener(cocos2d::CCObject *object)
{
    isGetData = true;
    
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("getChatDataListener error !!! ");
        return;
    }

    main::ShowMessageResponse *response = (main::ShowMessageResponse*)request->m_msg;
    
    _arrayChat->removeAllObjects();
    
    int dataCount = response->chat_size();
    for(int i=0;i<dataCount;i++)
    {
        SGChatMessage *message = getChatMessageByChatProto(response->chat(i));
        if(roleIsInBlacklist(message->getRoleId()))
        {
            //在黑名单中
            continue;
        }
        
        _arrayChat->addObject(message);
    }
    
    SGMainManager::shareMain()->showChatLayer();
    
}





