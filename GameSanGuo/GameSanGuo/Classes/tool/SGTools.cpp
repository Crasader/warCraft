//
//  SGTools.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-8.
//
//

#include "SGTools.h"
#include "SGStringConfig.h"
std::string SGTools::getTimeStringByTimeStamp(long long timeStamp)
{
    
    //        time_t rawtime;
    //
    //       time ( &rawtime );
    //       printf("%ld\n", &rawtime);
    //       printf ( "The current local time is: %s", ctime (&rawtime) );
    //
    //       char * time = ctime(&rawtime);
    //ctime(&rawtime) : time_t/timestampe -> "Www Mmm dd hh:mm:ss yyyy" format
    //        cout<< time << endl;
    //       printf("%s", asctime(gmtime(&rawtime)));
    // asctime(gmtime(&rawtime)) = ctime(&rawtime);
    //here gmtime(&rawtime) : time_t(timpstampe) -> struct tm
    //here asctime(gmtime) : struct tm -> "Www Mmm dd hh:mm:ss yyyy" format
    
    //  int	tm_sec;		/* seconds after the minute [0-60] */
    //	int	tm_min;		/* minutes after the hour [0-59] */
    //	int	tm_hour;	/* hours since midnight [0-23] */
    //	int	tm_mday;	/* day of the month [1-31] */
    //	int	tm_mon;		/* months since January [0-11] */
    //	int	tm_year;	/* years since 1900 */
    //	int	tm_wday;	/* days since Sunday [0-6] */
    //	int	tm_yday;	/* days since January 1 [0-365] */
    //	int	tm_isdst;	/* Daylight Savings Time flag */
    //	long	tm_gmtoff;	/* offset from CUT in seconds */
    //	char	*tm_zone;	/* timezone abbreviation */
    
    //    cout<<(ptm->tm_year + 1900)<<"year "<<(ptm->tm_mon + 1)<<"month "<<(ptm->tm_mday)<<"day "<<(ptm->tm_hour)<<":"<<(ptm->tm_min)<<":"<<(ptm->tm_sec)<<endl;
    
    
    time_t rawtime = (time_t)timeStamp;
    struct tm * ptm;
    ptm = localtime(&rawtime);
    
    int tm_min = ptm->tm_min;
    int tm_hour = ptm->tm_hour;
    int tm_mday = ptm->tm_mday;
    int tm_month = ptm->tm_mon+1;
    int tm_year = ptm->tm_year+1900;
    
    string timeStr;
    
    //拼接时间字符串
    char buffer[1024];
    sprintf(buffer, "%d-%d-%d %d:%d",tm_year,tm_month,tm_mday,tm_hour,tm_min);
    timeStr.append(buffer);
    
    return timeStr;
}

std::string SGTools::getTimeDistanceNow(long long timeStamp)
{
    time_t rawtime;
    time(&rawtime);  //结果是时间戳
    long long nowTime = rawtime;
    long long distance = nowTime - timeStamp/1000;
    CCLog("本地时间：%lld  服务器时间：%lld",nowTime,timeStamp);
    
    std::string backString;
    
    //1分钟
    if(distance/60 != 0)
    {
        //1小时
        if(distance/60/60 != 0)
        {
            //1天
            if(distance/60/60/24 != 0)
            {
                //1个月
                if(distance/60/60/24/30 != 0)
                {
                    if(distance/60/60/24/30/12 != 0)
                    {
                        backString.append(str_Tools_str1);
                    }else{
                        char buff[64];
                        sprintf(buff, str_Tools_str2,distance/60/60/24/30);
                        backString.append(buff);
                    }
                }else{
                    char buff[64];
                    sprintf(buff, str_Tools_str3,distance/60/60/24);
                    backString.append(buff);

                }
            }else
            {
                char buff[64];
                sprintf(buff, str_Tools_str4,distance/60/60);
                backString.append(buff);
            }
        }else{
            char buff[64];
            sprintf(buff,str_Tools_str5,distance/60);
            backString.append(buff);
        }
        
    }else{
        backString.append(str_Tools_str6);
    }
    
    return backString;
}

bool SGTools::charIsChinese(char ch)
{
    if(ch < 0)  //ascll码小于0，判断为中文，不过可能不太靠谱
    {
        return true;
    }
    
    return false;
}

//获得一个汉字的宽度
float SGTools::getWidthOfOneChineseWord(const char* fontName,int fontSize)
{
    CCLabelTTF *label = CCLabelTTF::create(str_Chat_str4, fontName, fontSize);
    return label->getContentSize().width;
}

//获得一个英文字母的宽度
float SGTools::getWidthOfOneEnglishWord(char ch,const char* fontName,int fontSize)
{
	//adpate for android
	if (ch < 0 || ch > 127)
	{
		ch = 'A';
	}
    CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat("%c", ch)->getCString(), fontName, fontSize);
    return label->getContentSize().width;
}

float SGTools::getHeightOfOneWord(const char* fontName ,int fontSize )
{
    CCLabelTTF *label = CCLabelTTF::create(str_Chat_str4, fontName, fontSize);
   // CCLOG("%f", label->getContentSize().height); cgpLogDel
    return label->getContentSize().height;
}

TargetDevice SGTools::getDeviceNow()
{
    CCEGLView *glView = CCDirector::sharedDirector()->getOpenGLView();
    CCSize viewSize = glView->getFrameSize();
    CCLOG("CCEGLView width=%f,heigth=%f",viewSize.width,viewSize.height);
    
    if((viewSize.width == 320 && viewSize.height == 480))
    {//低清iphone
        
        return kDeviceIphone;
    }else if(viewSize.width == 640 && viewSize.height == 960)
    {//高清iphone iphone4 iphone4s
        
        return kDeviceIphone4;
    }else if(viewSize.width == 640 && viewSize.height == 1136)
    {//高清iphone iphone5
        
        return kDeviceIphone5;
    }else if(viewSize.width == 768 && viewSize.height == 1024)
    {//低清ipad ipadmini
        
        return kDeviceIpad;
    }else if(viewSize.width == 1536 && viewSize.height == 2048)
    {//高清ipad
        
        return KDeviceIpadRetina;
    }
    
    return kDeviceNull;
}

static ccColor3B color[6] = {ccc3(255, 255, 255) , ccc3(255, 255, 255) , ccc3(31 , 255 , 0) , ccc3(0, 127, 255) ,ccc3(231,0,255) , ccc3(255, 168, 0)};

ccColor3B SGTools::getColorByQuality(int quality)
{
    if (quality <3 || quality>6)
        return color[0];
    else
        return color[quality - 1];
}



PointLayer::PointLayer(int arrSize):
_count(0),
_width(0),
_light(NULL),
_arrayNormal(NULL)
{
    _arrayNormal = CCArray::createWithCapacity(arrSize);
    _arrayNormal->retain();
    this->ignoreAnchorPointForPosition(false);
}

PointLayer::~PointLayer()
{
    _arrayNormal->release();
}


PointLayer * PointLayer::create(int count, float width, const char *fileNormal, const char *fileLight)
{
    PointLayer * ret = new PointLayer(count);
    if ( ret && ret->init() )
    {
        ret->inital(count, width,fileNormal,fileLight);
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void PointLayer::inital(int count,float width ,const char * fileNormal,const char * fileLight)
{
    _count = count;
    _width = width;
    CCAssert(fileNormal != NULL, "can not null");
    CCAssert(fileLight != NULL, "can not null");
    
    //点间距
    const float dotDist = 40;
    
    _light = CCSprite::createWithSpriteFrameName(fileLight);
    this->addChild(_light,5);
    
    CCSprite * temp  = NULL;
    for(int i=0 ; i<count ; i++)
    {
        temp= CCSprite::createWithSpriteFrameName(fileNormal);
        _arrayNormal->addObject(temp);
        
        temp->setPosition(ccp(  40 * i, 0.5 * temp->getContentSize().height ) );
        this->addChild(temp);
    }
    
    this->setAnchorPoint(ccp(0.5,0.5));
    this->setContentSize(CCSizeMake(dotDist * (count - 1), _light->getContentSize().height));
    this->setLight(1);
}


void PointLayer::setLight(int order)
{
    _light->setPosition(  ( (CCSprite*)(_arrayNormal->objectAtIndex(order-1)) )->getPosition() );
}

//往buffer里写入倒计时，形如01:02:03。
void SGTools::calClockString(int leftTime,char buffer[])
{
    int time_hour;
    int time_mimute;
    int time_secend;
    
    if (leftTime <= 0)
    {
        time_hour = 0;
        time_mimute = 0;
        time_secend = 0;
    }
    else
    {
        time_hour = leftTime/3600;
        time_mimute = leftTime%3600/60;
        time_secend = leftTime%3600%60;
    }
    
    sprintf(buffer, "%02d:%02d:%02d", time_hour, time_mimute, time_secend);
}

