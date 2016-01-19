//
//  PlatformAdp.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-5-13.
//
//

#include "PlatformAdp.h"


PlatformAdp::PlatformAdp()
{
	printf("PlatformAdp\n");
}

PlatformAdp::~PlatformAdp()
{
	
}

void PlatformAdp::pyUserSDKCallReq()
{

}

void PlatformAdp::pyUserSDKChangePasswordReq(const char *userName, const char *oldPassword, const char *newPassword)
{
	
}

void PlatformAdp::pyUserSDKLoginReq(const char *userName, const char *password)
{
	
}

void PlatformAdp::registerUserSDK(const char *userName, const char *password)
{
	
}

void PlatformAdp::setAccountSDK(int userId, const char *userName)
{

}

void PlatformAdp::okLoginSDK()
{
	
}


void PlatformAdp::onChargeSDK(std::string &userId, long time, int randNum, const char *ipadId, int money, int virtualMoney)
{
	
}

#pragma mark ============ SGStoreLayer App Store 计费相关_Begain==============


void PlatformAdp::gameIapStoreAlloc()
{
	
}
bool PlatformAdp::isIapInstanceExist()
{
	return false;
}

void PlatformAdp::iapInstnceRelease()
{
	
}

void PlatformAdp::iapBuyPayMent(int state)
{
	
}

void PlatformAdp::iapBuyItem(const char *info)
{
	
}

#pragma mark ============ SGStoreLayer App Store 计费相关_End==============



#pragma mark ====SdkReqCallBack 相关_Begain====
void PlatformAdp::SDKReqAlloc()
{
	
}

OBJ_TYPE PlatformAdp::getSdkReqDelegate()
{
	return NULL;
}


void PlatformAdp::startSDK()
{
	
}

void PlatformAdp::setSdkReqLevel(int lvl)
{
	
}

void PlatformAdp::trackSDKReward(long moneyNum,const char *reason)
{
	
}

void PlatformAdp::trackSDKPurchase(const char *item,int num ,long price)
{
	
}

void PlatformAdp::trackSDKItemUse(const char *item,int num)
{
	
}

void PlatformAdp::trackSDKMsnBegin(const char *storyInfo)
{
	
}

void PlatformAdp::trackSDKMsnCompleted(const char *storyInfo)
{
	
}

void PlatformAdp::trackSDKMsnFailed(const char *storyInfo, const char *cause)
{
	
}

void PlatformAdp::trackSDKTaskBegin(const char *taskInfo)
{
	
}

void PlatformAdp::trackSDKTaskCompleted(const char *taskInfo)
{
	
}

void PlatformAdp::onChargeSuccessSDK(std::string &userId, long time, int randNum)
{
	
}

#pragma mark ====SdkReqCallBack 相关_End====



#pragma mark ==========GameConfig 相关_Begain===========
std::string PlatformAdp::getUdid()
{
	return "";
}

void PlatformAdp::prohibitLockScreen()
{
	
}

NetStatus PlatformAdp::checkNetWorkStatus()
{
	return NotReach;
}


const char *PlatformAdp::longRangePath(const char *name, std::string filePath)
{
	return NULL;
}
void PlatformAdp::saveScreenImage(std::string fileFullPath)
{

}

ccColor3B PlatformAdp::colorWithHexString(const char *hexString)
{
	return ccc3(0, 0, 0);
}

const char *PlatformAdp::bundleVersion()
{
	return NULL;
}
/*
bool PlatformAdp::memoryInfo(vm_statistics_data_t *vmStats)
{
	return false;
}


void PlatformAdp::logMemInfo()
{

}
*/
bool PlatformAdp::isContainEmoji(const char *str)
{
	return false;
}


void PlatformAdp::delLog()
{
	
}

void PlatformAdp::writeLogToXML(const char *logInfo)
{
	
}

void PlatformAdp::showPopWindow(const char *showInfo)
{
	
}


double PlatformAdp::getAvaliableMem()
{
	return 0.0;
}

double PlatformAdp::usedMem()
{
	return 0.0;
}

const char *PlatformAdp::getTime()
{
	return NULL;
}

//打开指定URL。
void PlatformAdp::openUrl(std::string url)
{
    ;
}

#pragma mark ==========GameConfig 相关_End===========







