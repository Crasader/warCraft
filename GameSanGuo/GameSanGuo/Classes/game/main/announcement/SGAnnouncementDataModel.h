//
//  SGAnnouncementDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-9.
//
//

#ifndef __GameSanGuo__SGAnnouncementDataModel__
#define __GameSanGuo__SGAnnouncementDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class contentproto :public CCObject
{
public:
    CC_SYNTHESIZE_RETAIN(CCString*, _con, content);
    CC_SYNTHESIZE_RETAIN(CCString*, _sytle, style);//key
    contentproto();
    ~contentproto();
};
class SGAnnouncementDataModel : public CCObject
{
private:
    CC_SYNTHESIZE(long, timeStamp, TimeStamp);
    CC_SYNTHESIZE(int, m_type, msgType);
    CC_SYNTHESIZE_RETAIN(CCString*, title, Title);//sfjg
    CC_SYNTHESIZE_RETAIN(CCString*, content, Content);
	CC_SYNTHESIZE(int, viewId, ViewId);
	CC_SYNTHESIZE_RETAIN(CCString*, url, Url);
	
    CCArray *contentArr;
public:
    SGAnnouncementDataModel();
    ~SGAnnouncementDataModel();
    CCArray *getcontentArr();
};

#endif /* defined(__GameSanGuo__SGAnnouncementDataModel__) */
