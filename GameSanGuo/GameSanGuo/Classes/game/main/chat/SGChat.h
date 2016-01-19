//
//  SGChat.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#ifndef __GameSanGuo__SGChat__
#define __GameSanGuo__SGChat__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos-ext.h"
#include "SGChatMessageCenter.h"
#include "SGCCLabelTTF.h"
#include "SGMenu.h"
#include "MessageModule.pb.h"
//#include "SGRichLabel.h"

USING_NS_CC_EXT;

#define SGCHATSHOWTAGSTR "<pengyou.com>"
class SGChat : public SGBaseLayer{
private:
    //禁言
    CCLabelTTF *labelNoHorn;
    //小喇叭数量
//    SGCCLabelTTF *labelHornCount;
    //发送消息按钮
    SGButton *buttonSend;

    //本地存储的聊天消息
    CCArray *_arrayChat;
    
    //滑动控件
    CCScrollView *scrollView;
    CCLayer *scrollViewContainer;
    
    void constructView();
    
    //按钮点击
    void buttonClickGoback();
    void buttonClickSend();
    
    
//    //对小喇叭的操作
//    int getSmallTrumpetCount();
//    void minusSmallTrumpetCount();
//    void setSmallSpeakerCount();

    //获取玩家等级
    bool getPlayerLevel();
    
    //检查字符串长度
    bool checkWordsLength(char *buffer);
    
    /* 计算一个字符串占的区域大小 */
    CCSize getSizeByCharBuffer(const char *charBuffer);
     //获取一个聊天气泡
    CCSprite* getBubbleByChatMessage(SGChatMessage *chatMessage);

    
public:
    
    SGChat();
    ~SGChat();
    static SGChat* create(CCArray *arrayChat);
    
    //收到消息
    void sendMessage(CCObject *chatObject);
    //组装scrollview
    void constructScrollView();
    //sgboxdelegate的方法
    virtual void showBoxCall(CCObject *obj);
    
    //查看玩家信息监听，未作，暂留
    void getPlayerInfoListener(CCObject *object);
    void showSpeakerInfo(CCObject *sender);
    virtual void setIsCanTouch(bool isTouch);
//    virtual void onEnter();
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
    void showItemClickSend(CCObject *obj);
};

#endif /* defined(__GameSanGuo__SGChat__) */
