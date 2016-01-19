//
//  Utils.cpp
//  WarCraftCardGame
//
//  Created by orix on 12/10/12.
//
//

#include "Utils.h"
#include "CCString.h"
#include <fstream>
#include <string>

#include <ios>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <mach/mach.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#endif

// 存储谁被类型
static SNSDeviceType zqDeviceType = SNSDeviceTypeINVALID;

void setDeviceType(float width, float height)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	float ratio = (width < height)?width / height:height / width;
	if (ratio == 768.0f / 1024.0f) {
		zqDeviceType = SNSDeviceTypeIpad;
	}
	if (ratio == 640.0f / 1136.0f) {
		zqDeviceType = SNSDeviceTypeIphone5;
	}
#endif
}

bool isiPad()
{
    return (zqDeviceType == SNSDeviceTypeIpad);
}

bool isNewPad()
{
	return (isiPad() && CC_CONTENT_SCALE_FACTOR() == 2);
}

bool isIphone5()
{
	return (zqDeviceType == SNSDeviceTypeIphone5);
}

float autoLength(float l)
{
	if ( isiPad() ) {
		l *= 2;
	}
	else if ( cocos2d::CC_CONTENT_SCALE_FACTOR() == 2 ) {
		//do nothing
	}
	else {
		//do nothing
	}
	return l;
}

/**
 * 参数: l：iphone值 r:ipad值
 */
float autoLength(float l,float r)
{
    if ( isiPad() ) {
		return r;
	}
	else if ( cocos2d::CC_CONTENT_SCALE_FACTOR() == 2 ) {
		//do nothing
	}
	else {
		//do nothing
	}
	return l;
}


/**
 *  画矩形（生成矩形纹理）
 *  size:矩形大小
 *  rect:矩形颜色
 *  edge:边线颜色
 *  position:矩形坐标（锚点为中心点）
 *  edgeW:边线宽度
 */
CCSprite* initRectSprite(CCSize size,ccColor4B rect,ccColor4B edge,CCPoint position,CCPoint _anchorPoint,int edgeW)
{
    CCSize texSize = size;
	//    int bytes = texSize.width * texSize.height * 4;
	//    void* textureData = malloc(bytes);
	//    memset(textureData, INT32_MAX, bytes);
    
    int w = size.width;
    int h = size.height;
    
    int pixels[h][w];
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            // 颜色次序AABBGGRR
            if (i < edgeW || i >= h-edgeW || j < edgeW || j >= w-edgeW) {
                pixels[i][j] = edge.a << 24 | edge.b << 16 | edge.g << 8 | edge.r;
            }else{
                pixels[i][j] = rect.a << 24 | rect.b << 16 | rect.g << 8 | rect.r;
            }
        }
    }
    
    CCTexture2D *texture = new CCTexture2D();
    texture->autorelease();
    texture->initWithData(pixels, kCCTexture2DPixelFormat_RGBA8888, texSize.width, texSize.height, texSize);
    texture->setAliasTexParameters();
    
    CCSprite* sprite = CCSprite::createWithTexture(texture);
    sprite->setAnchorPoint(_anchorPoint);
    sprite->setPosition(position);
    
    return sprite;
}

/**
 * 获取当前设备已用内存(byte)
 */
double getUsedMemory(int _type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    task_basic_info_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(),
                                         TASK_BASIC_INFO, (task_info_t)&taskInfo, &infoCount);
    if(kernReturn != KERN_SUCCESS) {
        return -1;
    }
    
    switch (_type) {
        case 0:// byte
            return taskInfo.resident_size;
            break;
        case 1:// kb
            return taskInfo.resident_size / 1024.0;
            break;
        case 2:// M
            return (taskInfo.resident_size / 1024.0) / 1024.0;
            break;
    }
    // 默认（kb）
    return taskInfo.resident_size / 1024.0;
#endif
    return 0;
}

/**
 * 获取当前设备未用内存(byte)
 */
double getNotUsedMemory(int _type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    vm_statistics_data_t vmStats;
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
	
	if (kernReturn != KERN_SUCCESS)
	{
		return -1;
	}
	
    switch (_type) {
        case 0:// byte
            return (vm_page_size * vmStats.free_count);
            break;
        case 1:// kb
            return (vm_page_size * vmStats.free_count) / 1024.0;
            break;
        case 2:// M
            return ((vm_page_size * vmStats.free_count) / 1024.0) / 1024.0;
            break;
    }
    // 默认（kb）
	return (vm_page_size * vmStats.free_count) / 1024.0;
#endif
    return 0;
}

CCPoint autoPosition(CCPoint p)
{
    if ( isiPad() ) {
		p.x *= 2;
		p.y *= 2;
	}
	else if ( cocos2d::CC_CONTENT_SCALE_FACTOR() == 2 ) {
		//do nothing
	}
	else {
		//do nothing
	}
	return p;
}

CCSize autoSize(CCSize s)
{
    if ( isiPad() ) {
		s.width *= 2;
		s.height *= 2;
	}
	else if ( cocos2d::CC_CONTENT_SCALE_FACTOR() == 2 ) {
		//do nothing
	}
	else {
		//do nothing
	}
	return s;
}

CCRect autoRect(CCRect r)
{
    if ( isiPad() ) {
		r.origin.x *= 2;
		r.origin.y *= 2;
		r.size.width *=2;
		r.size.height *=2;
	}
	else if ( cocos2d::CC_CONTENT_SCALE_FACTOR() == 2 ) {
		//do nothing
	}
	else {
		//do nothing
	}
	return r;
}

const char* getMacAddress()
{
    cocos2d::CCString* strMac = new cocos2d::CCString();
    strMac->autorelease();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    char                mac_addr[60] = {0};
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        printf("Error: if_nametoindex error\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 1\n");
        return NULL;
    }
    
    if ((buf = (char*)malloc(len)) == NULL) {
        printf("Could not allocate memory. error!\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 2");
        free(buf);
        return NULL;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    
	//    sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
    //按照服务器端需求，去掉":"
    sprintf(mac_addr, "%02X%02X%02X%02X%02X%02X", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
    free(buf);
    
    strMac->m_sString = mac_addr;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //这里添加android获取mac地址方法
    //strMac->m_sString = jniGetMacAddress();
#endif
    
    return strMac->m_sString.c_str();
}

//开关或关闭ipad高清读取
static bool g_ipadReadHD = true;
void setIPadHDRead(bool open)
{
    g_ipadReadHD = open;
}

const char* getFileResource(const char* file)
{
	// 如果是ipad或者非ipad且为高清的情况下调用高清图片
	if (isiPad() || (!isiPad() && CC_CONTENT_SCALE_FACTOR() == 2)) {
        
        std::string strFile(file);
        
        cocos2d::CCString* pRet = new cocos2d::CCString();
        pRet->autorelease();
        
        // remove .xxx 后缀
        size_t startPos = strFile.find_last_of(".");
        pRet->m_sString = strFile.substr(0, startPos);
        
        // 加上hd
        if ( g_ipadReadHD ) {
            pRet->m_sString += "-hd";
        }
        //加上后缀
        pRet->m_sString += strFile.substr(startPos, strFile.length()-startPos);
        
        return (pRet->m_sString.c_str());
	}
	
    return file;
}

const char* getAndroidFileResource(const char* file, const char* dir)
{
	//sprintf(g_ResourceDir, "640x960/");
	//这里等实际到android的时候再搞吧
    cocos2d::CCString* pRet = new CCString("");
    pRet->autorelease();
    
    if ( dir && strlen(dir)>0 ) {
        pRet->m_sString += dir;
        pRet->m_sString += '/';
    }
    
    pRet->m_sString += getFileResource(file);
    
    return (pRet->m_sString.c_str());
}

CCAnimate* getAnimate(const char* fileName, int startNumber, int endNumber, float duration)
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	// 构造每一个帧的实际图像数据
	CCAnimation *animation = CCAnimation::create();
	CCString *frameName = NULL;
	CCSpriteFrame* frame = NULL;
	for (int i = startNumber; i <= endNumber; i++) {
		frameName = CCString::createWithFormat(fileName, i);
		//CCLOG("imnage %d:%s", i, frameName->getCString());
		frame = cache->spriteFrameByName(frameName->getCString());
		if (frame) {
			animation->addSpriteFrame(frame);
		}
	}
	animation->setDelayPerUnit(duration);
	return CCAnimate::create(animation);
}

// 执行粒子围绕一个rect旋转的动画
void runParticleAroundRect(CCRect rect, CCParticleSystemQuad* particle, float duration)
{
	CCAssert(NULL != particle, "Particle should not be NULL!");
	// 移动粒子到初始位置
	particle->setPosition(ccp(rect.origin.x, rect.origin.y));
	float factor = (rect.size.width > rect.size.height)?(rect.size.height / rect.size.width):(rect.size.height/rect.size.width);
	CCMoveTo* moveLeftUp = CCMoveTo::create(duration * factor, ccp(rect.origin.x, rect.origin.y + rect.size.height));
	CCMoveTo* moveMidRight = CCMoveTo::create(duration, ccp(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height));
	CCMoveTo* moveRightDown = CCMoveTo::create(duration * factor, ccp(rect.origin.x + rect.size.width, rect.origin.y));
	CCMoveTo* moveMidLeft = CCMoveTo::create(duration, ccp(rect.origin.x, rect.origin.y));
	CCAction* seq = CCSequence::create(moveLeftUp, moveMidRight, moveRightDown, moveMidLeft, NULL);
	CCRepeatForever* repeat = CCRepeatForever::create((CCActionInterval*)seq);
	
	particle->runAction(repeat);
}

CCArray* split(const char* str, const char* split)
{
	CCArray *arr = CCArray::create();
    int length = strlen(str) + 1;
	char *newStr = new char[length];
    char* temp = newStr;
    memset(newStr, '\0', length);
	sprintf(newStr, "%s", str);
	char *p = strtok(newStr,split);
	while(p!=NULL) {
		arr->addObject(CCString::create(p));
		p = strtok(NULL,split);
	}
    if (temp)
    {
        delete []temp;
    }
	return arr;
}

#define N 8
const char* tran(int num,int k)
{
    cocos2d::CCString* pRet = new CCString("");
    pRet->autorelease();
    
    int arr[N],i;
    for (i=0;i <N;i++)
    {
        arr[i]=num%k;
        num=num/k;
        if (num==0)
            break;
    }
    for (;i>=0;i--)
        switch (arr[i])
    {
        case 10: pRet->m_sString += 'A';break;
        case 11: pRet->m_sString += 'B';break;
        case 12: pRet->m_sString += 'C';break;
        case 13: pRet->m_sString += 'D';break;
        case 14: pRet->m_sString += 'E';break;
        case 15: pRet->m_sString += 'F';break;
        default: pRet->m_sString += arr[i]-48;
    }
    
    return (pRet->m_sString.c_str());
}

unsigned long ato16(const char* str)//字符串转16进制数
{
    unsigned long var=0;
    unsigned long t;
	//    int len = strlen(str);
    
    if (var > 8) //最长8位
        return -1;
    for (; *str; str++)
    {
        if (*str>='A' && *str <='F')
            t = *str-55;//a-f之间的ascii与对应数值相差55如'A'为65,65-55即为A
        else
            t = *str-48;
        var<<=4;
        var|=t;
    }
    return var;
}

const char* GetStrFromVector(std::vector<char> * _vecbuf)
{
    
    cocos2d::CCString* pRet = new CCString("");
    pRet->autorelease();
        
    int veLen = _vecbuf->size();
    if ( veLen <= 0 )
    {
        return NULL;
    }
   
    
    for (unsigned int i = 0; i < veLen; i++)
    {
        pRet->m_sString += (*_vecbuf)[i];
    }
    
    pRet->m_sString += '\0';
    
    return (pRet->m_sString.c_str());
}

#pragma mark - date & time function

//返回从1970年1月1日0时0分0秒到此时的秒数
long getTimeIntervalSince1970(long secs)
{
    time_t lt = time(&secs);
    return lt;
}

//返回从xxxx年x月x日x时x分x秒字符串
const char* getCurTimeString()
{
    time_t lt = time(NULL);
    struct tm * ptm = localtime(&lt);
    
    char stime[21] = {0};
    strftime(stime, 20, "%Y%m%dc%H%M%S", ptm);
    
    std::string pRet = stime;
    return pRet.c_str();
}

const char* getCurTimeStringTo1970(long n)
{
	struct tm * ptm = localtime(&n);
    
    char stime[21] = {0};
    strftime(stime, 20, "%Y%m%d%H%M%S", ptm);
    
    std::string pRet = stime;
    return pRet.c_str();
}

//根据指定的时间秒数，返回时间字符串(00:00:00)
const char* getTimeString(int seconds)
{
    char fromChar[10] = {0};
    
	int hour = seconds / 3600;
	int minute = (seconds % 3600) / 60;
	int second = (int)seconds % 60;
    
    sprintf(fromChar, "%02d:%02d:%02d", hour, minute, second);
    
    cocos2d::CCString* timeStr = new cocos2d::CCString();
    timeStr->autorelease();
    timeStr->m_sString = fromChar;
    
    return (timeStr->m_sString.c_str());
}

long long getLongTimes()
{
	struct timeval tvStart;
	gettimeofday(&tvStart,NULL);
	long long tStart = (long long)1000000*tvStart.tv_sec+tvStart.tv_usec;
	return tStart;
}

long long get1970Milliscond()
{
	struct timeval tvStart;
	gettimeofday(&tvStart,NULL);
	long long tStart = (long long)1000000*tvStart.tv_sec+tvStart.tv_usec;
	return (tStart/1000);
}

int getTimeZone()
{
    struct timezone tz;
    gettimeofday(NULL, &tz);
    int zone = (-tz.tz_minuteswest)/60;
    
    return zone;
}

//根据指定的时间秒数，返回时间结构
tTimeType getTime(int seconds)
{
    tTimeType t;
	t.hour = seconds/3600;
	seconds = seconds%3600;
	t.minute = seconds/60;
	t.second = seconds%60;
	
	return t;
}

//根据指定的时间秒数，返回加速需要钻石数
int getTimeDiamond(float seconds)
{
    return (seconds / 1800 + 1);
}
int getRandom(int min, int max)
{
    int random = (int) (abs( (int) arc4random() )) % (max-min +1)+min;
    return random;
}
#pragma mark - 字符串处理函数

// 全部转小写
char* strToLower(char* str)
{
    if (NULL == str) return str;
    
    char* const p_return = str;
    const int XX = int('a') - int('A');
    
    while (*str != '\0') {
        if (*str >= 'A' && *str <= 'Z') {
            *str += XX;
        }
        
        ++str;
    }
    
    return p_return;
}

// 全部转大写
char* strToUpper(char* str)
{
    if (NULL == str) return str;
    
    char* const p_return = str;
    const int XX = int('A') - int('a');
    
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str += XX;
        }
        ++str;
    }
    
    return p_return;
}

/*
 * 块string替换函数
 * src:原始字符串
 * oldStr:需要替换的内容
 * newStr:替换之后的内容
 */
CCString* replaceString(const char* src, const char* oldStr, const char* newStr)
{
	log("%s -- %s", oldStr, newStr);
	std::string str = src;
    std::string searchString(oldStr);
    std::string replaceString(newStr);
    
    assert( searchString != replaceString );
    
    std::string::size_type pos = 0;
    while ( (pos = str.find(searchString, pos)) != std::string::npos ) {
        str.replace( pos, searchString.size(), replaceString );
        pos++;
    }
	//    int cpStrLength = str.length();
	//    char* cpStr = new char[cpStrLength+1];
	//    memset(cpStr, 0, cpStrLength+1);
	//    sprintf(cpStr, "%s", str.c_str());
    return CCString::createWithFormat("%s", str.c_str());
}

#define rubbishCount (348)

CCString* filterString(const char* str)
{
	const char* rubbish[rubbishCount] = {"肏","fuck","鸡巴","bitch","penis","pulic","vagi","法轮功","法轮大法","藏独","makelove","make love","操你妈","草你妈","草泥马","法克鱿","操你","操他","妈了","巴子","你老母","日你","生殖器","一夜情","阴道","产道","女干","草拟","草你","shit","shoot","hag","傻逼","傻比","傻b","撒b","煞笔","弱智","sb","干你","阴毛","阴部","阴茎","cunt","sex","fart","arse","你丫","丫挺","共产党","共匪","毛泽东","江泽民","胡锦涛","pee","妈了","王八","龟头","鸡鸡","jj","婊子","80后群","后群","单身群","交友群","日死","代办","税费代缴","做贷款","代人了仇","承接","cnmd","nima","cao","jb","byd","gcd","他妈的","md","交流群","单身群","交友群","新疆","扎针","热比亚","热比娅","游行","暴动","示威","小和尚","和尚","一三一二零零九二一三一","mlgb","迷药","圣战","69圣战","nc不死","圣战不休","丁子酷","卖枪","卖弩","网络赌博","假币","假发票","窃听器","蒙汗药","指纹套","枪支","tmd","法轮","李洪志","大纪元","新唐人","肉棍","淫靡","淫水","六四事件","迷药","迷魂药","窃听器","六合彩","买卖枪支","三唑仑","麻醉药","麻醉乙醚","群发器","帝国之梦","毛一鲜","黎阳评","出售枪支","迷药","摇头丸","天葬","军长发威","PK黑社会","恶搞晚会","枪决女犯","投毒杀人","出售假币","监听王","昏药","侦探设备","麻醉钢枪","升达","手机复制","戴海静","自杀指南","自杀手册","张小平","安定片","蒙汗药","古方迷香","失意药","迷歼药","透视眼镜","远程偷拍","自制手枪","激情小电影","黄色小电影","色情小电影","天鹅之旅","盘古乐队","高校暴乱","群体事件","大学骚乱","高校骚乱","催情药","拍肩神药","春药","窃听器材","身份证生成","枪决现场","出售手枪","麻醉枪","办理证件","办理文凭","疆独","藏独","高干子弟","高干子女","枪支弹药","血腥图片","无界","特码","成人片","成人电影","换妻","三级片","本拉登","地下先烈","领导财产公示","GPS预警器","毛主席复活","防拍器","电子狗","曝光王","隐形喷剂","催情水","变声电话","变声器","代开发票","二奶大赛","全国二奶","死亡笔记","死亡日志","色情电视","宋平顺","四种当","黄色图片","成人论坛","性爱电影","黄色电影","成人图片","色情图片","成人网站","成人小说","激情视频","激情图片","激情电影","色情网站","私服","十八禁","18禁","淫荡","成人贴图","毛片","美女视频","脱衣舞","江姐问","董存瑞问","吴琼花问","拉登熟知中国","激情自拍","淫书","激情美女","监听器","淫奸","淫乱","成人激情","同居社区","淫虫","右派","赌博专用","透视器","透视照片","拦截器","冰毒","K粉","麻古","万能钥匙","赌博粉","地磅仪","无网界","无界","性网站","军火","广安事件","炒股国歌","素女","乱伦","嫖妓","嫖鸡","小穴","淫魔","做鸡","口交","粗口歌","性虐待","女优","包娃衣","耽美","丝袜写真","维多利亚包","花花公子","大禁","美腿写真","隐私1图片","龙虎豹","啄木鸟公司","巴拉斯","玉蒲团","成人杂志","ANDREWBLAKE","藏春阁","痴汉是犯罪","美女野球拳","裸女对对碰","偷窥有罪","伪装美女","虎胆雄心","买春堂","护士点","成人配色","秘密潜入","欲望格斗","七宗罪","尾行","性感扑克","六月联盟","FeelOnline","采花堂","爱姐妹","露拉D","吸血莱恩","梦幻麻将馆","性福人生","禁忌试玩","沉默杀手","惊悚空间","美少女麻雀","杨思敏","人工少女","电车之狼","臭作","欲望之血","性感沙滩","色狼网","ILLUSION","红河谷论坛","AV麻将","恐惧杀手","波动少女","MC军团","恐怖牢笼","反雷达测速","英语枪手","假钞","电话拦截","探测狗","手机跟踪","监听宝","针孔摄像","迷魂香","中国国家领导人子女任职名单","qq幸运用户抽奖","麻醉迷幻极品","隐形镜","手机监听器","淫乱电影","两性淫乱","黄海暗杀事件","胡温怒批政法系统","出售冰毒","PE-6拦截器","汽车爆炸案现场","刘晓波","和平奖","海洛因"};
	char* p = NULL;
	CCString* temp = NULL;
	for (int i = 0; i < rubbishCount; i++) {
		p = strstr(str, rubbish[i]);
		if (p) {
			temp = replaceString(str, rubbish[i], "**");
			str = temp->getCString();
		}
	}
	if (str) {
		CCString* newString = CCString::createWithFormat("%s", str);
		return newString;
	}
	return NULL;
}

void SDLOG(const char* _data)
{
	int templength = strlen(_data);
	if (templength>=16*1024)
	{
		int asd = templength/10;
		for (int i =0; i<10; i++)
		{
			char* temdata = new char[asd+1];
			memset(temdata, 0, asd+1);
			strncpy(temdata, _data+asd*i , asd);
			printf("%s",temdata);
			delete []temdata;
		}
		
	}
	else
	{
		//CCLOG("%s",_data);//cgpLogDel
	}
	
	
}

CCSprite* drawnum(char* showchar,char* numberfile)
{
	int m = 0;
	CCSprite* basespr = CCSprite::create(numberfile);
    
	CCSize allsize = basespr->getContentSize();
    
	CCSize persize = CCSizeMake(allsize.width/10, allsize.height);
	
	int _strlen = strlen(showchar);
	
	CCSprite* returnspr = CCSprite::create();
    
	returnspr->setContentSize(CCSizeMake(persize.width*_strlen, persize.height));
	
	
	for (int i =0; i<_strlen; i++)
	{
		
		//		if (showchar[i]=='-')
		//		{
		//			m = 10;
		//		}
		//		else if (showchar[i]=='+')
		//		{
		//			m = 11;
		//		}
		//		else
        if (showchar[i]=='0')
		{
			m = 9;
		}
		else
		{
			m = showchar[i]-49;
		}
        
		CCSprite* mSpr = CCSprite::create(numberfile, CCRect(persize.width*m, 0, persize.width, persize.height));
        
        int yOffest = 0;
        // 奇数位坐标偏移
        if (i%2 != 0) {
            yOffest = autoLength(3);
        }
        
		mSpr->setPosition(ccp(persize.width*(i+0.5f),persize.height*0.5f+yOffest));
		returnspr->addChild(mSpr);
	}
	
	return returnspr;
}

void writeToFile(const char* pszFileName, unsigned char* pData, unsigned long len)
{
    //创建写入文件指针
    FILE *fpWrite = fopen(pszFileName, "wb");
	if (!fpWrite)
	{
        CCLOG("文件写入失败 %s",pszFileName);
        return;
    }
	
    //将数据写入文件
    fwrite(pData, 1, len, fpWrite);
    fclose(fpWrite);
	
	
}


CCString* getLibrarypath(const char* addname)
{
	//	int addlen = 0;
	//	if (addname)
	//	{
	//		addlen = strlen(addname)+1;
	//	}
	//
	//	const char *pPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Icon.png");
	//	int len = strlen(pPath);
	//	char* path = new char[len+14-8+addlen];
	//	strncat(path, pPath, len-8);
	//	strcat(path, "Library/Caches");
	//	if (addname)
	//	{
	//		strcat(path, "/");
	//		strcat(path,addname);
	//	}
	//	CCString* Path = CCString::createWithFormat(path);
	//	delete []path;
	//
	
	
	int addlen = 0;
	if (addname)
	{
		addlen = strlen(addname)+1;
	}
	
	const char *pPath = CCFileUtils::sharedFileUtils()->getWritablePath().c_str();
	int len = strlen(pPath);
	char* path = new char[len+addlen+1];
	strcat(path, pPath);
	
	if (addname)
	{
		strcat(path,addname);
	}
	CCString* Path = CCString::createWithFormat("%s",path);
	delete []path;
	return Path;
}

//+(double) getAvailableBytes
//{
//	vm_statistics_data_t vmStats;
//	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
//	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);

//	if (kernReturn != KERN_SUCCESS)
//	{
//		return NSNotFound;
//	}

//	return (vm_page_size * vmStats.free_count);
//}
void LogSystiemTiem(char * tag )
{
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    CCLog("@@@@@@@@@@@@%s@@@@@@@@@@@%ld@@@@%ld",tag,psv.tv_sec,psv.tv_usec);

}

/*
 *获得utf-8编码的长度
 *
 */
unsigned char arr[6] = {0, 0xc0 , 0xe0 , 0xf0 , 0xf8 , 0xfc};
int getUtf8Length(const std::string & str)
{
    int num = 0;
    std::cout<<str<<std::endl;
    
    for ( int index =0 ; index < str.length() ; )
    {
        unsigned char temp = str.at(index);
        
        for (int i=5 ; i >= 0 ; --i)
        {
            if (temp >= arr[i])
            {
                index += i+1;
                break;
            }
        }
        ++num;
    }
    return num;
}