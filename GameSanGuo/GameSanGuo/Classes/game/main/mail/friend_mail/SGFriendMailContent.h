//
//  SGFriendMailContent.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-28.
//
//

#ifndef __GameSanGuo__SGFriendMailContent__
#define __GameSanGuo__SGFriendMailContent__

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGCCLabelTTF.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "MailModule.pb.h"
#include "SGMailManager.h"

USING_NS_CC_EXT;

class SGFriendMailContent : public SGBaseLayer
{
private:
    //昵称
    SGCCLabelTTF *labelNickName;
    
    //滑动控件
    CCScrollView *scrollview;
    
    //data 
    SGMailDataModel *_dataModel;
    
    //计时器
    long _timer; 
    
    int roleId; //对方id
    
    //init view
    void viewDidLoad();

    //返回
    void buttonClickGoBack();
    
    //发送消息
    void buttonClickSendMessage();
    
    //对方说的话
    CCSprite *getOppsiteTalk(const char *message);
    
    //得到一个发言气泡
    CCScale9Sprite* getBubbleOfTalk(bool isMe,const char *message);

    //发送监听
    void sendMailListener(CCObject *obj);
    
    //读邮件
    void readMailListener(CCObject *obj);
    CCSize getSizeByCharBuffer(const char *charBuffer);
public:
    SGFriendMailContent();
    
    ~SGFriendMailContent();
    
    static SGFriendMailContent *create(SGMailDataModel *dataModel);
    
    //字符是不是汉字字符
    static bool charIsChinese(char ch);
    
    //获得一个汉字的宽度
    static int getWidthOfOneChineseWord(const char* fontName=FONT_BOXINFO,int fontSize=24);
    
    //获得一个英文字母的宽度
    static int getWidthOfOneEnglishWord(char ch='m',const char* fontName=FONT_BOXINFO,int fontSize=24);
    
    //获得一个英文字母或者汉字的高度
    static int getHeightOfOneWord(const char* fontName=FONT_BOXINFO,int fontSize=24);

    //pop box call back
    virtual void showBoxCall(CCObject *obj);

    //构建CCScrollView
    void constructScrollView();

    void setDataModel(SGMailDataModel *newDataModel);

    
};



#endif /* defined(__GameSanGuo__SGFriendMailContent__) */
