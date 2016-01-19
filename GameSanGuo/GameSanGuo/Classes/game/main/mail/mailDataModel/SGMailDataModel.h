//
//  SGMailDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#ifndef __GameSanGuo__SGMailDataModel__
#define __GameSanGuo__SGMailDataModel__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SGMailDataModel :public CCObject
{
    //数据库Id，客户端不用关心
    CC_SYNTHESIZE(int, mailSsid, MailSsid);
    
    //邮件接受者RoleId
    CC_SYNTHESIZE(int, mailOneSelfId, MailOneSelfId);
    
    //邮件接收者昵称
    CC_SYNTHESIZE_RETAIN(CCString*, mailOneselfNickname, MailOneselfNickname);
    
    //发件人roleId
    CC_SYNTHESIZE(int, mailOtherId, MailOtherId);
    
    //发件人昵称
    CC_SYNTHESIZE_RETAIN(CCString*, mailOtherNickname, MailOtherNickname);
    
    //邮件类型 
    CC_SYNTHESIZE(int, mailType, MailType);
    
    //是否已读
    CC_SYNTHESIZE(bool, mailIdRead, MailIsRead);
    
    //itemid
    CC_SYNTHESIZE(int, m_mailItemId, MailItemId);

    //邮件内容，里面存放的是多个邮件消息体对象
    CCArray *mailContent;
//    CC_SYNTHESIZE_RETAIN(CCArray*, mailContent, MailContent);
    
public:
    
    CCArray *getMailContent();
    
    SGMailDataModel();
    
    ~SGMailDataModel();

};

class SGMailDetailDataModel :public CCObject
{
    
    //邮件体ID
    CC_SYNTHESIZE(int, mailDeId, MailDeId);
    
    //收件人roleId
    CC_SYNTHESIZE(int, mailDeReceiverId,MailDeReceiverId);
    
    //收件人昵称
    CC_SYNTHESIZE_RETAIN(CCString*,mailDeReceiverNickName , MailDeReceiverNickName);
    
    //发件人roleId
    CC_SYNTHESIZE(int, mailDeSenderId, MailDeSenderId);
    
    //发件人昵称
    CC_SYNTHESIZE(CCString*, mailDeSenderNickName, MailDeSenderNickName);
    
    //真的邮件体内容
    CC_SYNTHESIZE_RETAIN(CCString*, mailDeContent, MailDeContent);
    
    //发件时间
    CC_SYNTHESIZE(long long,mailDeSendData, MailDeSendDate);
    
public:
    
    SGMailDetailDataModel();
    
    ~SGMailDetailDataModel();
    
};



#endif /* defined(__GameSanGuo__SGMailDataModel__) */
