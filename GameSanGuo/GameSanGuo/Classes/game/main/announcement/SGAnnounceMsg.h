//
//  SGAnnounceMsg.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-8-5.
//
//

#ifndef __GameSanGuo__SGAnnounceMsg__
#define __GameSanGuo__SGAnnounceMsg__

#include "cocos2d.h"
#include "SGBaseLayer.h"
#include "SGAnnouncementDataModel.h"
USING_NS_CC;
class SGAnnounceMsg:public SGBaseLayer
{
private:
    int layerhgt;
private:
    void getstyle(CCLabelTTF*lab,std::string a);
    void labsize(CCLabelTTF*lab,std::string a);
    void labcolor(CCLabelTTF*lab,std::string a);
public:
    static SGAnnounceMsg*create(SGAnnouncementDataModel *a);
    void intiview(SGAnnouncementDataModel *a);
    int returnsize();
    void AnnounceButton(cocos2d::CCObject *obj);
};

#endif /* defined(__GameSanGuo__SGAnnounceMsg__) */
