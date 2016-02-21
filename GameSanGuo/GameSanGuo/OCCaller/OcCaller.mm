//
//  OcCaller.mm
//  GameSanGuo
//
//  Created by bugcode on 14-5-12.
//
//

#include "OcCaller.h"
#include "EAGLView.h"
#include <string>
#import "ASIFormDataRequest.h"
#import "CommonCrypto/CommonDigest.h"

#if (GAME_GOCPA_STATUS == GGCS_GOCPA_ON)
#import "GocpaTracker.h"
#endif

#if (GAME_APPSFLYER_STATUS == GAFS_APPSFLYER_ON)
#include "AppsFlyerTracker.h"
#endif

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

#define   GO2PLAYSDKADDRESS     "http://payment466.generalmobi.com/qiwiwallet/"

static OcCaller *caller = NULL;

static bool _isGEIOS7 = false;

static inline void checkIfGEIOS7()
{
    static bool isReady = false;
    if (!isReady)
    {
        _isGEIOS7 = [[[UIDevice currentDevice] systemVersion] compare:@"7.0" options:NSNumericSearch] != NSOrderedAscending;
        isReady = true;
    }
}

OcCaller::OcCaller() : iap(NULL), m_SdkReqCallBack(nil)
{
	printf("OcCaller\n");
    checkIfGEIOS7();
}

OcCaller *OcCaller::getInstance()
{
	if (!caller)
	{
		caller = new OcCaller();
	}
	return caller;
}

void OcCaller::pyUserSDKCallReq()
{
	[[PYUserSDKLib sharedUserSDKLib] destroyGuest:@"1"
										ChannelId:[m_SdkReqCallBack ChannelId]
								 CallBackDelegate:m_SdkReqCallBack FinishSelector:@selector(requestFailed:) FailSelector:@selector(requestFailed:)];
}

void OcCaller::pyUserSDKChangePasswordReq(const char *userName, const char *oldPassword, const char *newPassword)
{
    [[PYUserSDKLib sharedUserSDKLib] modifyPassword:[[NSString alloc] initWithUTF8String:userName]
                                           Password:[[NSString alloc] initWithUTF8String:oldPassword]
                                             GameId:@"1"
                                          ChannelId:[m_SdkReqCallBack ChannelId]
                                       SubChannelId:[m_SdkReqCallBack SubChannelId]
                                          OsVersion:[m_SdkReqCallBack OsVersion]
                                             OsType:[m_SdkReqCallBack OsType]
										NewPassword:[[NSString alloc] initWithUTF8String:newPassword]
								   CallBackDelegate:m_SdkReqCallBack FinishSelector:@selector(passwordFinished:) FailSelector:@selector (requestFailed:)];

}

void OcCaller::pyUserSDKLoginReq(const char *userName, const char *password)
{
	[[PYUserSDKLib sharedUserSDKLib] userLogin:[[NSString alloc] initWithUTF8String:userName]
									  Password:[[NSString alloc] initWithUTF8String:password]
										GameId:@"1"
									 ChannelId:[m_SdkReqCallBack ChannelId]
								  SubChannelId:[m_SdkReqCallBack SubChannelId]
									 OsVersion:[m_SdkReqCallBack OsVersion]
										OsType:[m_SdkReqCallBack OsType]
							  CallBackDelegate:m_SdkReqCallBack FinishSelector:@selector(loginFinished:) FailSelector:@selector(requestFailed:)];

}

void OcCaller::pyUserThirdPartySDKLoginReq(const char *userName, const char *password)
{
    
    PYUserSDKLib* temp = [PYUserSDKLib sharedUserSDKLib];
    return;
//    NSString* nstr = [temp md5Encrypt:@"12345"];
	[[PYUserSDKLib sharedUserSDKLib] userThirdPartyLogin:[[NSString alloc] initWithUTF8String:userName]
									  Password:[[NSString alloc] initWithUTF8String:password]
										GameId:@"1"
									 ChannelId:[m_SdkReqCallBack ChannelId]
								  SubChannelId:[m_SdkReqCallBack SubChannelId]
									 OsVersion:[m_SdkReqCallBack OsVersion]
										OsType:[m_SdkReqCallBack OsType]
							  CallBackDelegate:m_SdkReqCallBack FinishSelector:@selector(loginFinished:) FailSelector:@selector(requestFailed:)];
    
}

void OcCaller::registerUserSDK(const char *userName, const char *password)
{
	[[PYUserSDKLib sharedUserSDKLib] userRegister:[[NSString alloc] initWithUTF8String:userName]
										 Password:[[NSString alloc] initWithUTF8String:password]
										   GameId:@"1"
										ChannelId:[m_SdkReqCallBack ChannelId]
									 SubChannelId:[m_SdkReqCallBack SubChannelId]
										OsVersion:[m_SdkReqCallBack OsVersion]
										   OsType:[m_SdkReqCallBack OsType]
										 PhoneNum:@"123"
								 CallBackDelegate:m_SdkReqCallBack FinishSelector:@selector(registFinished:) FailSelector:@selector(requestFailed:)];

}

void OcCaller::setAccountSDK(std::string& userId, const char *userName)
{
	[m_SdkReqCallBack setAccount:[NSString stringWithFormat:@"%s",userId.c_str()]
					 accountName:[NSString stringWithUTF8String:userName]
					 accountType:kAccountRegistered];

}

void OcCaller::okLoginSDK()
{
	[[PYUserSDKLib sharedUserSDKLib] oKLogin:@"1"
								   ChannelId:[m_SdkReqCallBack ChannelId]
								SubChannelId:[m_SdkReqCallBack SubChannelId]
								   OsVersion:[m_SdkReqCallBack OsVersion]
									  OsType:[m_SdkReqCallBack OsType]
							CallBackDelegate:m_SdkReqCallBack
							  FinishSelector:@selector(fastLoginFinished:) FailSelector:@selector(requestFailed:)];

}


void OcCaller::onChargeSDK(std::string& userId, long time, int randNum, const char *ipadId, int money, int virtualMoney)
{
	[SdkReqCallBack onChargeRequst: [NSString stringWithFormat:@"User%sTimeR%ld%d", userId.c_str(),time,randNum]
							 iapId:[NSString stringWithUTF8String:ipadId]
					currencyAmount:money
					  currencyType:MONEYTTYPE
			 virtualCurrencyAmount:virtualMoney
					   paymentType: PAYMENTTYPE];

}

#pragma mark ============ SGStoreLayer App Store 计费相关_Begain==============


void OcCaller::gameIapStoreAlloc()
{
	iap = [[GameIapStore alloc] init];

}
bool OcCaller::isIapInstanceExist()
{
	if (iap)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;

}

void OcCaller::iapInstnceRelease()
{
	[iap release];

}

void OcCaller::iapBuyPayMent(int state)
{
	[iap buyPaymentQueue:state];

}

void OcCaller::iapBuyItem(const char *info)
{
	[iap buy:info];

}

#pragma mark ============ SGStoreLayer App Store 计费相关_End==============



#pragma mark ====SdkReqCallBack 相关_Begain====
void OcCaller::SDKReqAlloc()
{
	m_SdkReqCallBack = [[[SdkReqCallBack alloc] init] autorelease];
    [m_SdkReqCallBack retain];

}

id OcCaller::getSdkReqDelegate()
{
	return m_SdkReqCallBack;
}


void OcCaller::startSDK()
{
	[SdkReqCallBack onStart];

}


void OcCaller::startHasOffers()
{
#if (GAME_HASOFFER_STATUS == GHOS_HASOFFER_ON)
	[SdkReqCallBack onStartHasOffers];
#endif   
}


void OcCaller::closeFBlogin(int closeValue)
{
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    [m_SdkReqCallBack closeFBSession:closeValue];
#endif
}


#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
void OcCaller::createFBSession()
{
  [m_SdkReqCallBack creatFbSession];
}

void OcCaller::showFBlogin()
{
    [m_SdkReqCallBack startFbLogin];
}

void OcCaller::startFBShare(int shareInfo,std::string &sharePicture )
{
    [m_SdkReqCallBack startFbShare:shareInfo sharePictureInfo:[NSString stringWithFormat:@"%s",sharePicture.c_str()]];
}

#endif

void OcCaller::setSdkReqLevel(int lvl)
{
	[m_SdkReqCallBack setLevel:lvl];

}

void OcCaller::trackSDKReward(long moneyNum,const char *reason)
{
	[SdkReqCallBack onReward:moneyNum reason:[NSString stringWithUTF8String:reason]];

}

void OcCaller::trackSDKPurchase(const char *item,int num ,long price)
{
	[SdkReqCallBack onPurchase:[NSString stringWithUTF8String:item] itemNumber:num
		priceInVirtualCurrency:price ];

}

void OcCaller::trackSDKItemUse(const char *item,int num)
{
	[SdkReqCallBack onUse:[NSString stringWithUTF8String:item] itemNumber:num ];

}

void OcCaller::trackSDKMsnBegin(const char *storyInfo)
{
	[SdkReqCallBack onBegin:[NSString stringWithUTF8String:storyInfo]];

}

void OcCaller::trackSDKMsnCompleted(const char *storyInfo)
{
	[SdkReqCallBack onCompleted:[NSString stringWithUTF8String:storyInfo]];

}

void OcCaller::trackSDKMsnFailed(const char *storyInfo, const char *cause)
{
	[SdkReqCallBack onFailed:[NSString stringWithUTF8String:storyInfo]
				 failedCause:[NSString stringWithUTF8String:cause]];

}

void OcCaller::trackSDKTaskBegin(const char *taskInfo)
{
	[SdkReqCallBack onBegin:[NSString stringWithUTF8String:taskInfo]];

}

void OcCaller::trackSDKTaskCompleted(const char *taskInfo)
{
	[SdkReqCallBack onCompleted:[NSString stringWithUTF8String:taskInfo]];

}

void OcCaller::onChargeSuccessSDK(std::string& userId, long time, int randNum)
{
	[SdkReqCallBack onChargeSuccess: [NSString stringWithFormat:@"User%sTimeR%ld%d", userId.c_str(),time,randNum]];

}

//初始化AF
void  OcCaller::initAppsFlysWithAppid()
{
#if (GAME_APPSFLYER_STATUS == GAFS_APPSFLYER_ON)
    [AppsFlyerTracker sharedTracker].appsFlyerDevKey = @"6HRzTLQfzQHGBQ87KzZttH";
    [AppsFlyerTracker sharedTracker].appleAppID = @"892284326";
#endif
}
//启动AF，这里可以检测安装与打开
void OcCaller::startAppsFlys()
{
#if (GAME_APPSFLYER_STATUS == GAFS_APPSFLYER_ON)
    [[AppsFlyerTracker sharedTracker] trackAppLaunch];
#endif
}
//appflyer登陆统计
void OcCaller::appsFlyerLoginTrack(std::string info)
{
#if (GAME_APPSFLYER_STATUS == GAFS_APPSFLYER_ON)
    NSString *tmpInfo = [[NSString alloc] initWithUTF8String:info.c_str()];
    //appsflyer 登陆信息。info是附加字段， 可能会传某些信息
    [[AppsFlyerTracker sharedTracker] trackEvent:@"Login" withValue:tmpInfo];
    [tmpInfo release];
#endif
}


#pragma mark ====SdkReqCallBack 相关_End====



#pragma mark ==========GameConfig 相关_Begain===========
std::string OcCaller::getUdid()
{
    NSString *deviceUDID = [[PYUserSDKLib sharedUserSDKLib] getIDKC:@"1" ChannelId:[m_SdkReqCallBack ChannelId]];
    
    return std::string([deviceUDID UTF8String]);
}

void OcCaller::prohibitLockScreen()
{
	[[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];

}

NetStatus OcCaller::checkNetWorkStatus()
{
	Reachability * rea = [Reachability reachabilityWithHostName:@"www.baidu.com"];
	return (NetStatus)[rea currentReachabilityStatus];
}


const char *OcCaller::longRangePath(const char *name, std::string filePath)
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    filePath = [documentsDirectory UTF8String];
    filePath.append("/");
    
    filePath += name;
    return filePath.c_str();

}
void OcCaller::saveScreenImage(std::string fileFullPath)
{
    NSString *fullPath = [NSString stringWithUTF8String:fileFullPath.c_str()];
 	UIImage *image = [UIImage imageWithContentsOfFile:fullPath];
    UIImageWriteToSavedPhotosAlbum(image, NULL, NULL, NULL);
    

}

ccColor3B OcCaller::colorWithHexString(const char *hexString)
{
	NSString *s = [NSString stringWithUTF8String:hexString];
    NSString *cString = [[s  stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
    if ([cString length] < 6)
        return ccWHITE;
    
    if ([cString hasPrefix:@"0X"])
        cString = [cString substringFromIndex:2];
    else if ([cString hasPrefix:@"#"])
        cString = [cString substringFromIndex:1];
    
    if ([cString length] != 6)
        return ccWHITE;
    
    // Separate into r, g, b substrings
    NSRange range;
    range.location = 0;
    range.length = 2;
    NSString *rString = [cString substringWithRange:range];
    
    range.location = 2;
    NSString *gString = [cString substringWithRange:range];
    
    range.location = 4;
    NSString *bString = [cString substringWithRange:range];
    
    unsigned int r, g, b;
    [[NSScanner scannerWithString:rString] scanHexInt:&r];
    [[NSScanner scannerWithString:gString] scanHexInt:&g];
    [[NSScanner scannerWithString:bString] scanHexInt:&b];
    //NSLog(@"r = %u, g = %u, b = %u",r, g, b);
    return ccc3(r, g, b);

	return ccc3(0, 0, 0);

}

const char *OcCaller::bundleVersion()
{
	NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey];
    return [version UTF8String];
 
	return NULL;

}

bool OcCaller::memoryInfo(vm_statistics_data_t *vmStats)
{
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)vmStats, &infoCount);
    
    return kernReturn == KERN_SUCCESS;
}


void OcCaller::logMemInfo()
{
	vm_statistics_data_t vmStats;
    
    if (memoryInfo(&vmStats)) {
        NSLog(@"free: %u\nactive: %u\ninactive: %u\nwire: %u\nzero fill: %u\nreactivations: %u\npageins: %u\npageouts: %u\nfaults: %u\ncow_faults: %u\nlookups: %u\nhits: %u",
              vmStats.free_count * vm_page_size,
              vmStats.active_count * vm_page_size,
              vmStats.inactive_count * vm_page_size,
              vmStats.wire_count * vm_page_size,
              vmStats.zero_fill_count * vm_page_size,
              vmStats.reactivations * vm_page_size,
              vmStats.pageins * vm_page_size,
              vmStats.pageouts * vm_page_size,
              vmStats.faults,
              vmStats.cow_faults,
              vmStats.lookups,
              vmStats.hits
              );
	}

}

bool OcCaller::isContainEmoji(const char *str)
{
	__block BOOL isEomji = NO;
    
    NSString *string = [NSString stringWithUTF8String:str];
    
    [string enumerateSubstringsInRange:NSMakeRange(0, [string length]) options:NSStringEnumerationByComposedCharacterSequences usingBlock:
     
     ^(NSString *substring, NSRange substringRange, NSRange enclosingRange, BOOL *stop) {
         
         
         const unichar hs = [substring characterAtIndex:0];
         
         // surrogate pair
         
         if (0xd800 <= hs && hs <= 0xdbff) {
             
             if (substring.length > 1) {
                 
                 const unichar ls = [substring characterAtIndex:1];
                 
                 const int uc = ((hs - 0xd800) * 0x400) + (ls - 0xdc00) + 0x10000;
                 
                 if (0x1d000 <= uc && uc <= 0x1f77f) {
                     
                     isEomji = YES;
                     
				 }
                 
			 }
             
		 } else if (substring.length > 1) {
             
             const unichar ls = [substring characterAtIndex:1];
             
             if (ls == 0x20e3) {
                 
                 isEomji = YES;
                 
			 }
             
             
             
		 } else {
             
             // non surrogate
             
             if (0x2100 <= hs && hs <= 0x27ff && hs != 0x263b) {
                 
                 isEomji = YES;
                 
			 } else if (0x2B05 <= hs && hs <= 0x2b07) {
                 
                 isEomji = YES;
                 
			 } else if (0x2934 <= hs && hs <= 0x2935) {
                 
                 isEomji = YES;
                 
			 } else if (0x3297 <= hs && hs <= 0x3299) {
                 
                 isEomji = YES;
                 
			 } else if (hs == 0xa9 || hs == 0xae || hs == 0x303d || hs == 0x3030 || hs == 0x2b55 || hs == 0x2b1c || hs == 0x2b1b || hs == 0x2b50|| hs == 0x231a ) {
                 
                 isEomji = YES;
                 
			 }
             
		 }
         
	 }];
    return isEomji;
	

	return false;

}


void OcCaller::delLog()
{
	//创建文件管理器
    NSFileManager * fileManager = [NSFileManager defaultManager];
    NSString *documentsDirectory =[NSHomeDirectory()stringByAppendingPathComponent:@"Documents"];
    NSString *fileName=@"ailog.txt";
    NSString *finalPath = [documentsDirectory stringByAppendingPathComponent:fileName];
    
	//判断文件是否存在
	if ([[NSFileManager defaultManager] fileExistsAtPath:finalPath]) {
        //        [fileManager copyItemAtPath:copypath toPath:finalPath error:nil];
        [fileManager createFileAtPath:finalPath contents:NULL attributes:NULL];
    }

}

void OcCaller::writeLogToXML(const char *logInfo)
{
	//创建文件管理器
    NSFileManager * fileManager = [NSFileManager defaultManager];
    NSString *documentsDirectory =[NSHomeDirectory()stringByAppendingPathComponent:@"Documents"];
    NSString *fileName=@"ailog.txt";
    NSString *finalPath = [documentsDirectory stringByAppendingPathComponent:fileName];
    
	//判断文件是否存在
	if (![[NSFileManager defaultManager] fileExistsAtPath:finalPath]) {
        [fileManager createFileAtPath:finalPath contents:NULL attributes:NULL];
    }
	
	FILE *pf;//定义一个文件指针   大写通常定义的变量都是指针  封装好的_t才是整数类型
	char buf[100];//定义一个缓冲区是100的变量
    
	pf = fopen([finalPath UTF8String],"a+");//以追加的方式打开文件
	
	if(pf == NULL)
	{
		perror("open");
		exit(1);
    }
	strcpy(buf,"\n");//将word 拷贝到 buf中
	fputs(logInfo,pf);//将world  追加到  pf文件中.
	fputs(buf, pf);
    
	fclose(pf);
	

}

void OcCaller::showPopWindow(const char *showInfo)
{
	CCNative::createAlert("加载失败", showInfo, NULL);
    CCNative::addAlertButton("~ ~");
    CCNative::showAlert();

}


double OcCaller::getAvaliableMem()
{
	vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(),
                                               HOST_VM_INFO,
                                               (host_info_t)&vmStats,
                                               &infoCount);
    
    if (kernReturn != KERN_SUCCESS) {
        return 0;
    }
    
    return ((vm_page_size *vmStats.free_count) / 1024.0) / 1024.0;

	return 0.0;

}

double OcCaller::usedMem()
{
	task_basic_info_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(),
                                         TASK_BASIC_INFO,
                                         (task_info_t)&taskInfo,
                                         &infoCount);
    
    if (kernReturn != KERN_SUCCESS) {
        return 0;
    }
    
    return taskInfo.resident_size / 1024.0 / 1024.0;

	return 0.0;

}

const char *OcCaller::getTime()
{
	//精确获取系统当前时间
	NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
	NSTimeInterval a=[dat timeIntervalSince1970]*1000;
	NSString *timeString = [NSString stringWithFormat:@"%f", a];
	NSArray *arry=[timeString componentsSeparatedByString:@"."];
	NSString *fir = arry[0];
	return [fir UTF8String];

	return NULL;

}

#pragma mark ==========GameConfig 相关_End===========



#pragma mark ==========PlatFormIOS 相关_Begain===========

void OcCaller::createController(SGRootViewController **root)
{
	*root = [[SGRootViewController alloc]init];
    [[EAGLView sharedEGLView]addSubview:[*root view]];
}

void OcCaller::releaseController(SGRootViewController **root)
{
	[(*root).view removeFromSuperview];
    [*root release];
}

void OcCaller::showIOSConnection(SGRootViewController **root, bool isshow)
{
	[*root showConn];
}
void OcCaller::removeIOSConnection(SGRootViewController **root)
{
	[*root hideConn];
}
//推送好友消息
void OcCaller::pushIOSFriendMessage(SGRootViewController **root, const char* name)
{
	[*root pushFriendMessage:name];
}
//推送体力回满
void OcCaller::pushIOSPowerFull(SGRootViewController **root, int timeLeft)
{
	[*root pushActivities];
}
//推送活动
void OcCaller::pushIOSActivities(SGRootViewController **root)
{
	[*root pushActivities];
}

#pragma mark ==========PlatFormIOS 相关_End===========

#pragma mark ==========RichLabel 相关_Begain===========

CGSize getStringSize(NSString *str, id font, CGSize *constrainSize)
{
    NSArray *listItems = [str componentsSeparatedByString: @"\n"];
    CGSize dim = CGSizeZero;
    CGSize tmp;
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize->width > 0 ? constrainSize->width : 0x7fffffff;
    textRect.height = constrainSize->height > 0 ? constrainSize->height : 0x7fffffff;
    
    for (NSString *s in listItems) {
        if (_isGEIOS7)
        {
            tmp = [s boundingRectWithSize:textRect options:NSStringDrawingUsesLineFragmentOrigin attributes:@{ NSFontAttributeName:font } context:nil].size;
        }
        else
        {
            tmp = [s sizeWithFont:font constrainedToSize:textRect];
        }
        
        if (tmp.width > dim.width) {
            dim.width = tmp.width;
        }
        dim.height += tmp.height;
    }
    
    return dim;
}

//同原来的richlabel
void OcCaller::setString(const char *cstr, CCSprite *sprite, CCNode *parent)
{
	CCNode *layer = parent;//(CCLayer *)sprite->getParent();
    CCSize size = sprite->getTextureRect().size;
    SGCCLabelTTF *sgLabel = (SGCCLabelTTF *)sprite->getChildren()->objectAtIndex(0);
    CCLabelTTF *label = (CCLabelTTF *)sgLabel->getChildren()->objectAtIndex(0);
    CCSprite *newSp = getIOSFomLabel(cstr, size, label->getFontName(), label->getFontSize());
    newSp->setPosition(sprite->getPosition());
    newSp->setTag(sprite->getTag());
    layer->removeChild(sprite, true);
    layer->addChild(newSp);
}


CCSprite*  OcCaller::getIOSFomLabel(const char* cstr, const CCSize &size, const char* fntName, float fontSize,ccColor3B color, ccColor3B defaultInColor,ccColor3B defaultOutColor)
{
	string labelStr(cstr);
    vector<string> strs;
    string temp;
    
    CCSprite *spriteStar = CCSprite::create();;
    spriteStar->setTextureRect(CCRectMake(0, 0, size.width, size.height));
    spriteStar->setOpacity(0);
    SGCCLabelTTF *label = NULL;
    
    
    CGSize cgSzie = CGSizeZero;
    
    
    NSString *fontName = [NSString stringWithUTF8String:fntName];
    fontName = [[fontName lastPathComponent] stringByDeletingPathExtension];
    id font = [UIFont fontWithName:fontName size:fontSize];
    
    float width = size.width;
    float height = size.height;
    float gwidth = 0;   // global width
    float gheight = 0;  // global heigth
    
    int index1 = 0;
    int index2 = 0;
    int pCount = 0;
    
    while (index2 != string::npos) {
        index2 = labelStr.find("#", index1);
        if (index2 != string::npos) { // substr label
            pCount ++;
            temp = labelStr.substr(index1, index2-index1-1);
            index1 = index2 + 1;
            strs.push_back(temp);
            
            NSString *text = [NSString stringWithUTF8String:temp.c_str()];
            CGSize textSize = getStringSize(text, font, &cgSzie);
            gwidth += textSize.width;
            
            if (gwidth >= width) {
                gwidth -= textSize.width; //恢复原样
                
                NSString *ch = [text substringWithRange:NSMakeRange(0, 1)];
                CGSize csize = getStringSize(ch, font, &cgSzie);
                if ((gwidth + csize.width) > width) { // 不能添加一个字符，直接换行
                    gwidth = 0;
                    gheight += csize.height;
                }
                
                int lIndex = 0, rIndex = text.length;
                bool chgLine = false; //换行标志位
                while (rIndex > 0 && rIndex <= text.length && lIndex < rIndex) {
                    NSString *tempstr = [text substringWithRange:NSMakeRange(lIndex, rIndex-lIndex)];
                    CGSize tempSize = getStringSize(tempstr, font, &cgSzie);
                    
                    float discWid = gwidth + tempSize.width;
                    if ((discWid < width)) {
                        if (pCount %2 == 0) {// red label
                            label = SGCCLabelTTF::create([tempstr UTF8String], [fontName UTF8String], fontSize, CCSizeZero, color);
                        }
                        else
                        {
                            label = SGCCLabelTTF::create([tempstr UTF8String], [fontName UTF8String], fontSize, CCSizeZero, defaultInColor, defaultOutColor);
                        }
                        label->setAnchorPoint(ccp(0, 0.5f));
                        label->setPosition(ccp(gwidth, height-gheight));
                        spriteStar->addChild(label);
                        
                        // reset some data
                        lIndex = rIndex;
                        rIndex = text.length;
                        if (chgLine) {
                            gwidth = 0;
                            gheight += tempSize.height;
                            chgLine = false;
                        }else {
                            gwidth = tempSize.width;
                        }
                    }else {
                        rIndex--;
                        chgLine = true;
                    }
                }
            } else {
                gwidth -= textSize.width;
                
                if (pCount %2 == 0)
				{ // red label
                    label = SGCCLabelTTF::create(temp.c_str(), [fontName UTF8String], fontSize, CCSizeZero, color);
                }
                else
                {
                    label = SGCCLabelTTF::create(temp.c_str(), [fontName UTF8String], fontSize, CCSizeZero, defaultInColor, defaultOutColor);
                }
                
                label->setAnchorPoint(ccp(0, 0.5f));
                label->setPosition(ccp(gwidth, height-gheight));
                spriteStar->addChild(label);
                gwidth += textSize.width;
            }
        }
    }
    
    // last line string
    temp = labelStr.substr(index1, strlen(cstr));
    strs.push_back(temp);
    
    int index = 0;
    NSString *text = [NSString stringWithUTF8String:temp.c_str()];
    CGSize textSize = getStringSize(text, font, &cgSzie);
    if ((textSize.width+gwidth) > width) { // last line bigger than width
        while ((index++) <= text.length) {
            NSString *tempstr = [text substringWithRange:NSMakeRange(0, index)];
            CGSize tempSize = getStringSize(tempstr, font, &cgSzie);
			
            if ((gwidth + tempSize.width > width)) {
                // 回溯一个字
                index -= 1;
                tempstr = [text substringWithRange:NSMakeRange(0, index)];
                tempSize = getStringSize(tempstr, font, &cgSzie);
				
                label = SGCCLabelTTF::create([tempstr UTF8String], [fontName UTF8String], fontSize, CCSizeZero, defaultInColor, defaultOutColor);
                label->setAnchorPoint(ccp(0, 0.5f));
                label->setPosition(ccp(gwidth, height-gheight));
//                label->setInsideColor(defaultInColor);
//                label->setOutSideColor(defaultOutColor);
                spriteStar->addChild(label);
				
                gwidth = 0;
                gheight += tempSize.height;
                break;
            }
        }
    }
    
    text = [text substringWithRange:NSMakeRange(index, text.length-index)];
    CGSize tsize = getStringSize(text, font, &cgSzie);
    
    const char *lstr = [text UTF8String];
    float lheight = (tsize.width/width+1) * tsize.height;
    label = SGCCLabelTTF::create(lstr, [fontName UTF8String], fontSize, CCSizeMake(width, lheight), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, defaultInColor, defaultOutColor);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccp(gwidth, height-gheight+tsize.height/2-lheight/2));
//    label->setInsideColor(defaultInColor);
//    label->setOutSideColor(defaultOutColor);
    spriteStar->addChild(label);
    
    return spriteStar;
}





#pragma mark ==========RichLabel 相关_End===========


#pragma mark ==========Go2Play SDK 相关_Begin===========



void OcCaller::sendCreatInvoice(const char *usrName,float payAmount,int playerId)
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int min = tm->tm_min;
    int sec = tm->tm_sec;
    
    
    NSString *pay_type = [NSString stringWithFormat:@"%s",GO2PLAYPAYTYPE];
    NSString *user = [NSString stringWithFormat:@"%s",usrName];
    NSString *amount = [NSString stringWithFormat:@"%.2f",payAmount];
    NSString *ccy = [NSString stringWithFormat:@"%s",GO2PLAYCCY];
    NSString *comment = [NSString stringWithFormat:@"%s",GO2PLAYCOMMENT];
    NSString *lifetime = [NSString stringWithFormat:@"%4d-%02d-%02dT%02d:%02d:%02d",year + 1,month,day,hour,min,sec];
    NSString *client_user_id= [NSString stringWithFormat:@"%d",playerId];
    NSString *gameId = [NSString stringWithFormat:@"%d",GO2PLAYGAMEID];
    NSString *timeStamp = [NSString stringWithFormat:@"%4d-%02d-%02d%02d:%02d:%02d",year,month,day,hour,min,sec];
    NSString *country = [NSString stringWithFormat:@"%s",GO2PLAYCOUNTRY];
    
    NSString *md5Str = [NSString stringWithFormat:@"%s%@%@%@",GO2PLAYKEY, client_user_id,amount, timeStamp];
    const char *cStr = [md5Str UTF8String];
    unsigned char result[32];
    CC_MD5(cStr, strlen(cStr), result);
    
    NSString *auth =  [[NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                        result[0], result[1], result[2], result[3],
                        result[4], result[5], result[6], result[7],
                        result[8], result[9], result[10], result[11],
                        result[12], result[13], result[14], result[15]
                        ] lowercaseString];
    
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"%s%s",GO2PLAYSDKADDRESS,"recharge"]];
    ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:url];
    [request setRequestMethod:@"POST"];
    
    [request setPostValue:pay_type forKey:@"pay_type"];
    [request setPostValue:user forKey:@"user"];
    [request setPostValue:amount forKey:@"amount"];
    [request setPostValue:ccy forKey:@"ccy"];
    [request setPostValue:comment forKey:@"comment"];
    
    [request setPostValue:lifetime forKey:@"lifetime"];
    [request setPostValue:client_user_id forKey:@"client_user_id"];
    [request setPostValue:gameId forKey:@"game_id"];
    [request setPostValue:timeStamp forKey:@"timeStamp"];
    [request setPostValue:country forKey:@"country"];
    [request setPostValue:auth forKey:@"auth"];
    
    [request setDidFinishSelector:@selector(creatInvoiceFinished:)];
    [request setDidFailSelector:@selector(invoiceFail:)];
    [request setTag:1];
    [request setDelegate:m_SdkReqCallBack];
    [request startAsynchronous];
}



void OcCaller::sendInvoiceStatus(int billId)
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int min = tm->tm_min;
    int sec = tm->tm_sec;
    
    NSString *bill_id = [NSString stringWithFormat:@"%d",billId];
    NSString *gameId = [NSString stringWithFormat:@"%d",GO2PLAYGAMEID];
    NSString *status = @"paid";
    NSString *timeStamp = [NSString stringWithFormat:@"%4d-%02d-%02d%02d:%02d:%02d",year,month,day,hour,min,sec];
    
    NSString *md5Str = [NSString stringWithFormat:@"%s%@%@",GO2PLAYKEY, bill_id, timeStamp];
    const char *cStr = [md5Str UTF8String];
    unsigned char result[32];
    CC_MD5(cStr, strlen(cStr), result);
    
    NSString *auth =  [[NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
             result[0], result[1], result[2], result[3],
             result[4], result[5], result[6], result[7],
             result[8], result[9], result[10], result[11],
             result[12], result[13], result[14], result[15]
             ] lowercaseString];
    
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"%s%s",GO2PLAYSDKADDRESS,"receiveStatus"]];
    ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:url];
    [request setRequestMethod:@"POST"];
    [request setPostValue:bill_id forKey:@"bill_id"];
    [request setPostValue:gameId forKey:@"game_id"];
    [request setPostValue:status forKey:@"status"];
    [request setPostValue:auth forKey:@"auth"];
    [request setPostValue:timeStamp forKey:@"timeStamp"];
    
    [request setDidFinishSelector:@selector(receiveInvoiceFinished:)];
    [request setDidFailSelector:@selector(invoiceFail:)];
    [request setTag:2];
    [request setDelegate:m_SdkReqCallBack];
    [request startAsynchronous];
}

#pragma mark ==========Go2Play SDK 相关_End===========

//打开指定URL。
void OcCaller::openUrl(std::string url)
{
    NSString *nsUrl = [NSString stringWithUTF8String:url.c_str()];
    //打开链接
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:nsUrl]];
}


void OcCaller::addGoCPASDK()
{
#if (GAME_GOCPA_STATUS == GGCS_GOCPA_ON)
    GocpaTracker *tracker = [[GocpaTracker alloc]initWithAppId:@"9d2d5abb56f74e6c984aa48c9626e34e" advertiserId:@"442" referral:false];
    [tracker reportDevice];
#endif
}






