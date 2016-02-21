#include "CGPTools.h"

#include "CCFileUtils.h"


void Tools::shakeWnd(CCNode* layout)
{
	CCEaseBackOut* moveByStart = CCEaseBackOut::create(CCMoveBy::create(0.05f,  ccp(0, 10)));
	CCEaseBackOut* moveBy2 = CCEaseBackOut::create(CCMoveBy::create(0.05f, ccp(0, -10)));
	CCEaseBackOut* moveBy3 = CCEaseBackOut::create(CCMoveBy::create(0.05f, ccp(0, 10)));
	CCEaseBackOut* moveByEnd = CCEaseBackOut::create(CCMoveBy::create(0.02f, ccp(0, 0)));
	CCFiniteTimeAction* seAction = CCSequence::create(moveByStart, moveBy2, moveBy3, moveBy2, moveBy3,moveBy2, moveBy3, moveByEnd, nullptr);
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


string Tools::getFileByName(string pFileName){
    //第一先获取文件的路径
    string path = CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;
    CCLOG("path = %s",path.c_str());
    
    //创建一个文件指针
    FILE* file = fopen(path.c_str(), "r");
    
    if (file) {
        char* buf;  //要获取的字符串
        int len;    //获取的长度
        /*获取长度*/
        fseek(file, 0, SEEK_END);   //移到尾部
        len = ftell(file);          //提取长度
        rewind(file);               //回归原位
        CCLOG("count the file content len = %d",len);
        //分配buf空间
        buf = (char*)malloc(sizeof(char) * len + 1);
        if (!buf) {
            CCLOG("malloc space is not enough.");
            return NULL;
        }
        
        //读取文件
        //读取进的buf，单位大小，长度，文件指针
        int rLen = fread(buf, sizeof(char), len, file);
        buf[rLen] = '\0';
        CCLOG("has read Length = %d",rLen);
        CCLOG("has read content = %s",buf);
        
        string result = buf;
        fclose(file);
        free(buf);
        return result;
    }
    else
        CCLOG("open file error.");
    
    return NULL;
}

bool Tools::saveFile(const char *pContent, string pFileName)
{
    //第一获取储存的文件路径
    string path = CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;
    //GPCCLOG("wanna save file path = %s",path.c_str());
    
    //创建一个文件指针
    //路径、模式
    FILE* file = fopen(path.c_str(), "a+");   //"w"
    if (file) {
        fputs(pContent, file);
        fclose(file);
    }
    else
        GPCCLOG("save file error.");
    
    return false;
}




