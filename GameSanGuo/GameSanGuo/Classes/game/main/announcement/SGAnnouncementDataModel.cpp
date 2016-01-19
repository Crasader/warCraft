//
//  SGAnnouncementDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-9.
//
//

#include "SGAnnouncementDataModel.h"
contentproto::contentproto():
_sytle(NULL),
_con(NULL)
{
    _sytle = NULL;
    _con = NULL;
}
contentproto::~contentproto()
{
    CC_SAFE_RELEASE(_sytle);
    CC_SAFE_RELEASE(_con);
}
SGAnnouncementDataModel::SGAnnouncementDataModel():
timeStamp(0),
m_type(0),
title(NULL),
content(NULL),
viewId(0),
url(NULL),
contentArr(NULL)
{
    m_type = 0;
    timeStamp = 0;
    title = NULL;
    content = NULL;
    url=NULL;
    contentArr = CCArray::create();
    contentArr->retain();
}

SGAnnouncementDataModel::~SGAnnouncementDataModel()
{
    CC_SAFE_RELEASE(title);
    CC_SAFE_RELEASE(content);
    CC_SAFE_RELEASE(contentArr);
    CC_SAFE_DELETE(url);
}
CCArray *SGAnnouncementDataModel::getcontentArr()
{
    return contentArr;
}