//
//  SGMailDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#include "SGMailDataModel.h"

#pragma mark SGMailDataModel

SGMailDataModel::SGMailDataModel()
{

    mailIdRead = false;
    mailOneSelfId = 0;
    mailOneselfNickname = NULL;
    mailOtherId = 0;
    mailOtherNickname = NULL;
    mailSsid = 0;
    mailType = 0;
    m_mailItemId = 1;
    mailContent = CCArray::create();
    mailContent->retain();
}

SGMailDataModel::~SGMailDataModel()
{
    CC_SAFE_RELEASE(mailContent);
    CC_SAFE_RELEASE(mailOneselfNickname);
    CC_SAFE_RELEASE(mailOtherNickname);
}

CCArray *SGMailDataModel::getMailContent()
{
    return mailContent;
}

#pragma mark SGMailDetailDataModel

SGMailDetailDataModel::SGMailDetailDataModel()
{
    mailDeContent = NULL;
    mailDeId = 0;
    mailDeReceiverId = 0;
    mailDeReceiverNickName = NULL;
    mailDeSendData = 0;
    mailDeSenderId = 0;
    mailDeSenderNickName = NULL;
}

SGMailDetailDataModel::~SGMailDetailDataModel()
{
//    CC_SAFE_RELEASE(mailDeContent);
//    CC_SAFE_RELEASE(mailDeReceiverNickName);
//    CC_SAFE_RELEASE(mailDeSenderNickName);
}
