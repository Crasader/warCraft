//
//  SGTestSwitch.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#ifndef GameSanGuo_SGTestSwitch_h
#define GameSanGuo_SGTestSwitch_h

#include "SGTestDebug.h"
/**
 * debug switch, for offline test
 */

// mytest->test Offline

// open debug model
#ifndef SG_Debug
//#define SG_Debug
#endif

#ifndef SG_LogInfo
//#define SG_LogInfo
#endif

// open color log model
#ifdef SG_Debug
    #define DDLog(format, ...)      LogInfo(LC_GREED, format, ##__VA_ARGS__);
    #define DCLog(format, ...)      LogInfo(LC_RED, format, ##__VA_ARGS__);
#else
    #ifdef SG_LogInfo
        #define DDLog(format, ...)      LogInfo(LC_GREED, format, ##__VA_ARGS__);
        #define DCLog(format, ...)      LogInfo(LC_RED, format, ##__VA_ARGS__);
    #else
        #define DDLog(format, ...)
        #define DCLog(format, ...)
    #endif
#endif

#ifdef SG_Debug
#define H_TOP      273.640015f
#define H_BOTTOM   162.0f
#else
#define H_TOP (SGMainManager::shareMain()->getTotleHdHgt())
#define H_BOTTOM (SGMainManager::shareMain()->getBottomHeight())
#endif

#endif   /* GameSanGuo_SGTestSwitch_h */



