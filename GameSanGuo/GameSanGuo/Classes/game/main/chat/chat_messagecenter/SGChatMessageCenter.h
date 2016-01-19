//
//  SGChatMessageCenter.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-11.
//
//

#ifndef __GameSanGuo__SGChatMessageCenter__
#define __GameSanGuo__SGChatMessageCenter__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "MessageModule.pb.h"
#include "SGChatMessage.h"
#include "CCNotificationCenter.h"

USING_NS_CC;
USING_NS_CC_EXT;


class SGChatMessageCenter : public CCObject
{
    
private:
    CCArray *_arrayChat;
    
    CC_SYNTHESIZE(bool, isGetData, IsGetData);
    
    void init();

    void chatMessageListener(CCObject *object);
    
    void getChatDataListener(CCObject *object);
    
public:
    SGChatMessageCenter();
    
    ~SGChatMessageCenter();
    
    static SGChatMessageCenter *shareChatMessageCenter();
    
    SGChatMessage* getChatMessageByChatProto(main::ChatProto chatproto);
    
    CCArray *getArrayCount();
    
    void openChatRoom();
    
    bool roleIsInBlacklist(int roleId);

    
    
};


#endif /* defined(__GameSanGuo__SGChatMessageCenter__) */
