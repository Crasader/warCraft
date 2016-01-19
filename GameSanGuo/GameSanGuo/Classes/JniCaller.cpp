//
//  JniCaller.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-7-16.
//
//

#include "JniBrigde.h"
#include "PlatformAdp.h"
#include "cocos2d.h"
#include "SGMainManager.h"
#if (PLATFORM == ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif
#include "GlobalConfig.h"
#include "ExtClass.h"

using namespace cocos2d;

//CLASS目录名
#define PACKAGE_NAME        com_xdyou_sanguo

/*
//如果google play发布，使用com/generalmobi/hero/tw/包
#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY)//正式google play
//定义包名字符串 for google play
#define PACKAGE_NAME        com_generalmobi_hero_tw

//如果等于第三方 渠道， android
#elif (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD)//正式第三方
//定义包名字符串 for third package
#define PACKAGE_NAME        com_generalmobi_herothird_tw

//uc android
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
#define PACKAGE_NAME        com_xdyou_sanguo_uc

//内网&外网测试，非正式
#elif (GAME_CHANNEL_TYPE == GCCT_TEST_ADR_EX || GAME_CHANNEL_TYPE == GCCT_TEST_ADR_IN)//测试 内网&外网
//定义包名字符串 for 测试外网
#define PACKAGE_NAME        com_generalmobi_herotest_tw

//默认使用外网测试
#else
#define PACKAGE_NAME        com_generalmobi_herotest_tw
#endif
*/


//拼出不同函数名
#define NAME3(CLASS3, CLASS4, FUNC3) Java_##CLASS3##_##CLASS4##_##FUNC3
#define NAME2(CLASS2, CLASS3, FUNC2) NAME3(CLASS2, CLASS3, FUNC2)
#define NAME(CLASS1, FUNC) NAME2(PACKAGE_NAME, CLASS1, FUNC)


extern "C"
{
    
    //com_go2play_heros_tw
    //获取facebook登陆用户的相关的userId和userName
    void NAME(GameSanGuo,fbUserIdAndUserName)(JNIEnv *env, jobject thiz, jstring userName,
                                                                  jstring userId, jstring loginKey, jboolean state)
    {
        const char *_userName = env->GetStringUTFChars(userName, NULL);
        const char *_userId = env->GetStringUTFChars(userId, NULL);
        const char *_loginKey = env->GetStringUTFChars(loginKey, NULL);
        bool _state = (state == JNI_TRUE);
        
        CCLOG("go2play -- userName: %s -- userId : %s logKey: %s", _userName, _userId, _loginKey);
        fbUserData(_userName, _userId, _loginKey, _state);
        env->ReleaseStringUTFChars(userName, _userName);
        env->ReleaseStringUTFChars(userId, _userId);
        env->ReleaseStringUTFChars(loginKey, _loginKey);
    }
    //通过java端设置facebook的session状态
    void NAME(GameSanGuo, setSessionState)(JNIEnv *env, jobject thiz, jboolean state)
    {
        bool tempState = (state == JNI_TRUE);
        CCLOG("go2play -- state: %d", (state == JNI_TRUE));
        
        fbSetSessionState(tempState);
    }
    //java端控制显示分享信息
    void NAME(GameSanGuo, fbShareTips)(JNIEnv *env, jobject thiz, jstring tips)
    {
        const char *shareTips = env->GetStringUTFChars(tips, NULL);
        CCLOG("call native method dbShareTips -- tips == %s", shareTips);
        //调用游戏内部的消息提示
        //fbShareInfo(shareTips);
        closeBoxFromJava();//提示完成后关闭弹窗
        env->ReleaseStringUTFChars(tips, shareTips);
    }
    //清除帐号信息
    void NAME(GameSanGuo, fbDoLogout)()
    {
        CCLOG("facebook logout jni calling!");
        clearAccount();
    }
    //java 调用c++本地方法，向服务器发送消息，告知facebook发布状态成功
    void NAME(GameSanGuo, sendPostSuccess)()
    {
        CCLOG("分享成功，向服务器发送消息！");
        sendPostStatusAndPicSuccess();
    }
    
    // 充值成功，通知服务器，求给予元宝。
    void NAME(GameSanGuo, dealChargeResult)(JNIEnv *env, jobject thiz, jboolean isOk, jstring bId, jint bNum)
	{
		bool _isOk = (isOk == JNI_TRUE);
		if (_isOk)
		{
			CCLOG("充值成功，向服务器发送消息！");
			const char *_bId = env->GetStringUTFChars(bId, NULL);
			const int _bNum = bNum;
            
			env->ReleaseStringUTFChars(bId, _bId);
			sendChargeSuccessBridge(_bId, _bNum);
		}
		else
		{
			CCLOG("充值失败！发生什么了！");
		}
	}
    
    // 本地方法，获取游戏客户端更新URL。
    jstring NAME(GameSanGuo, getClientUpdateUrl)(JNIEnv *env, jobject thiz)
    {
        CCLOG("JGameSanGuo_getClientUpdateUrl");
        std::string url = GlobalConfig::gi()->getClientUpdateUrl();
        return env->NewStringUTF( url.c_str() );
        //return env->NewString( (const jchar*) url.c_str(), (jsize) url.length() );
    }
    
    
    //java通过本地c++方法获取talkingdata的appid，以区分pengyou还是go2play的talkingdata统计
    jstring NAME(GameSanGuo, getTalkingDataAppId)(JNIEnv *env, jobject thiz)
    {
        std::string tdAppId = GlobalConfig::gi()->getTalkingDataAppId();
        CCLOG("getTalkingDataAppId || tdAppId = [%s]", tdAppId.c_str());
        //返回给java初始化talkingdata
        return env->NewStringUTF(tdAppId.c_str());
    }

    //java通过本地c++方法获取talkingdata的appChannelName，以区分talkingdata统计标签。
    jstring NAME(GameSanGuo, getTalkingDataAppChannel)(JNIEnv *env, jobject thiz)
    {
        std::string tdAppChannel = GlobalConfig::gi()->getTalkingDataAppChannel();
        CCLOG("getTalkingDataAppChannel || tdAppChannel = [%s]", tdAppChannel.c_str());
        //返回给java初始化talkingdata
        return env->NewStringUTF(tdAppChannel.c_str());
    }
    
    //获取渠道id， （主要用于go2play 计费， 根据渠道id判定是不是需要google wallet）
    jboolean NAME(SDKCaller, isGoolePlayChannel)(JNIEnv *env, jobject thiz)
    {
        jboolean isGoolePlay = GlobalConfig::gi()->isGoolePlayChannel();
        CCLOG("isGoolePlayChannel || channelId = [%d]", isGoolePlay);
        
        return isGoolePlay;
    }

    //在本处写入userId，userName，方便统一登陆处理。
    void NAME(SDKCaller, setUserDataNative)(JNIEnv *env, jobject thiz, jboolean isSuc, jstring uid, jstring uname, jboolean isFb)
    {
        bool cissuc = (isSuc == JNI_TRUE);
        bool cisfb = (isFb == JNI_TRUE);
        std::string cuid = JniHelper::jstring2string(uid);
        std::string cuname = JniHelper::jstring2string(uname);
        CCLOG("setUserDataNative || isSUc=%d, cuid=%s, cuname=%s, isFb=%d", cissuc, cuid.c_str(), cuname.c_str(), cisfb);

        setSdkUserData(cissuc, cuid, cuname, cisfb);
    }

    //java触发的提示文本的本地调用。
    void NAME(SDKCaller, showToastNative)(JNIEnv *env, jobject thiz, jstring text)
    {
        std::string cText = JniHelper::jstring2string(text);
        CCLOG("showToastNative || text=%s", cText.c_str());
        SGMainManager::shareMain()->showMessage(cText.c_str());
    }

    //登陆游戏。
    void NAME(SDKCaller, loginGameNative)(JNIEnv *env, jobject thiz, jboolean isSuc, jstring uid, jstring uname)
    {
        bool cissuc = (isSuc == JNI_TRUE);
        std::string cuid = JniHelper::jstring2string(uid);
        std::string cuname = JniHelper::jstring2string(uname);
        CCLOG("loginGameNative || isSUc=%d, cuid=%s, cuname=%s", cissuc, cuid.c_str(), cuname.c_str());

        loginGameNow(false, cissuc, cuid, cuname);
    }

    //快速登陆游戏。
    void NAME(SDKCaller, fastLoginGameNative)(JNIEnv *env, jobject thiz, jboolean isSuc, jstring uid, jstring uname)
    {
        bool cissuc = (isSuc == JNI_TRUE);
        std::string cuid = JniHelper::jstring2string(uid);
        std::string cuname = JniHelper::jstring2string(uname);
        CCLOG("fastLoginGameNative || isSUc=%d, cuid=%s, cuname=%s", cissuc, cuid.c_str(), cuname.c_str());

        loginGameNow(true, cissuc, cuid, cuname);
    }

    //FB登出。
    void NAME(SDKCaller, fbLogoutNative)(JNIEnv *env, jobject thiz, jboolean isSuc)
    {
        bool cissuc = (isSuc == JNI_TRUE);
        CCLOG("fbLogoutNative || isSUc=%d", cissuc);

        fbLogoutNow(cissuc);
    }
}
