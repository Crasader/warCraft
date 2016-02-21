//
//  SdkController.h
//  GameSanGuo
//
//  Created by bugcode on 15-1-30.
//
//

#ifndef GameSanGuo_SdkController_h
#define GameSanGuo_SdkController_h

#include "SdkImp.h"

/*
 所有无界面外部用户sdk的调度器
 */

class SdkController
{
private:
    SdkController();
    
public:
    ~SdkController();
    
    static SdkController *gi();
    
    //获取类实现
    SdkImp *getSdkImp();
    
private:
    //类实现的实例
    SdkImp *si;
};


#endif
