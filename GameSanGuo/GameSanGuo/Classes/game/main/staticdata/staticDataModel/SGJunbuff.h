//
//  SGJunbuff.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-12.
//
//

#ifndef __GameSanGuo__SGJunbuff__
#define __GameSanGuo__SGJunbuff__
#include "cocos2d.h"


//#define  LIANDANSTARTINDEX 9
//#define  LAST4PLOTID  12
//#define  SUPERGUIDPLOTID  16
//#define  MAXPLOTID  "maxPlotId"
USING_NS_CC;
//#include "native/CCAlertViewDelegate.h"
//#include "native/CCNative.h"
//USING_NS_CC_EXT;

class SGJunbuff : public CCObject
{
private:
    CC_SYNTHESIZE(int, buffid, Buffid);
    CC_SYNTHESIZE_RETAIN(CCString*, junName, JunName);
    CC_SYNTHESIZE_RETAIN(CCString*, condition, JunCondition);
    CC_SYNTHESIZE_RETAIN(CCString*, buff, Junbuff);
    
//进度条部分
    CC_SYNTHESIZE(int, m_score,Score);
    CC_SYNTHESIZE(int, m_min, Min);
    CC_SYNTHESIZE(int, m_max,Max);
//    CC_SYNTHESIZE(int, m_id,JinduId);
    
    
//    static SGJunbuff* shareBuffInfo(void);
    SGJunbuff();
    ~SGJunbuff();
};


// 一些关于战斗结束的全局变量

#endif /* defined(__GameSanGuo__SGJunbuff__) */
