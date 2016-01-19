//
//  SGServerListNewServerBox.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-1-25.
//
//

#ifndef __GameSanGuo__SGServerListNewServerBox__
#define __GameSanGuo__SGServerListNewServerBox__

#include <iostream>
#include "SGBaseBox.h"
#include "SGHTTPDelegate.h"

class SGServerListNewServerBox : public SGBaseBox , public SGHTTPDelegate
{

private:
    CCString *m_serverName;
    SGBoxDelegate *_target;
    short m_sServerPost;
    CCString * m_pServerIp;
    
    void buttonClickOK(SGButton *sender);
    void buttonClickCancel(SGButton* sender);
    
    
public:
    SGServerListNewServerBox();
    ~SGServerListNewServerBox();

//    bool init();
    void initView();

    static SGServerListNewServerBox *create(CCString *serverName,SGBoxDelegate *target ,CCString *serverIp ,unsigned short serverPost);

    virtual void requestFinished(int msgid, cocos2d::CCObject *data);

    
};

#endif /* defined(__GameSanGuo__SGServerListNewServerBox__) */
