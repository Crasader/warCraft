#include "CGPTools.h"


void Tools::shakeWnd(CCNode* layout)
{
	float heightWave=40;
	CCEaseBackOut* moveByStart=CCEaseBackOut::create(CCMoveBy::create(0.075f,  ccp(0, heightWave)));
	CCEaseBackOut* moveBy2=CCEaseBackOut::create(CCMoveBy::create(0.13f, ccp(0, -heightWave*3/2)));
	CCEaseBackOut* moveBy3=CCEaseBackOut::create(CCMoveBy::create(0.099f, ccp(0, heightWave*5/6)));
	CCEaseBackOut* moveBy4=CCEaseBackOut::create(CCMoveBy::create(0.099f, ccp(0, -heightWave*1/2)));
	CCEaseBackOut* moveByEnd=CCEaseBackOut::create(CCMoveBy::create(0.066f, ccp(0, heightWave*1/6)));
	CCFiniteTimeAction* seAction=CCSequence::create(moveByStart, moveBy2, moveBy3, moveBy4, moveByEnd, nullptr);
	layout->runAction(seAction);
}


string Tools::parseInt2String(int param)
{
	char buffer[100] = "";
	sprintf(buffer, "%d", param);
	return buffer;
}

int Tools::parseString2Int(string& param)
{
	int ret = atoi(param.c_str());
	return ret;
}

void Tools::split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

int Tools::createRandom(int start, int end)
{
//    cc_timeval psv;
//    CCTime::gettimeofdayCocos2d(&psv, NULL);
//    unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec/1000;
//    srand((unsigned int)rand_seed);
    
    int randomValue = (int)(CCRANDOM_0_1() * end + start);
    return randomValue;
    
}

bool Tools::isDayOrLight()
{
    time_t lt = time(NULL);
    struct tm * ptm = localtime(&lt);
    int hours =  ptm->tm_hour;
    if (hours < 6 || hours >= 18)  //晚上
    {
        //GPCCLOG("now is night hours:%d" , hours);
        return false;
    }
    else
    {
        //GPCCLOG("now is daytime hours:%d",  hours);
        return true;
    }
}



