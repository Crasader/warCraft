//
//  SdkController.cpp
//  GameSanGuo
//
//  Created by bugcode on 15-1-30.
//
//

#include "SdkController.h"
#include "../Classes/game/GlobalConfig.h"

#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE)
#include "Go2playIosImp.h"

#elif (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD)
#include "Go2playAdrImp.h"

#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
#include "KuaiYongIosImp.h"

#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_QIHU_ADR_THIRD)
#include "QihuSdkImp/QihuAdrImp.h"

#else

#endif

static SdkController *sc = NULL;

SdkController::SdkController()
{
//根据不同的渠道，获取不同的实现类实例，android或者ios
#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE)
    //go2play ios
    si = new Go2playIosImp();
#elif (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD)
    //go2play android
    si = new Go2playAdrImp();
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
    //kuaiyong ios
    si = new KuaiYongIosImp();
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_QIHU_ADR_THIRD)
    //360 android
    si = new QihuAdrImp();
#else
    si = new SdkImp();
#endif
}

SdkController::~SdkController()
{
    
}
//singleton
SdkController *SdkController::gi()
{
    if (!sc)
    {
        sc = new SdkController();
    }
    return sc;
}
//获取实现的类实例
SdkImp *SdkController::getSdkImp()
{
    return si;
}




