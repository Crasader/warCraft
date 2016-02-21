#ifndef _CGPTOOLS_H_
#define _CGPTOOLS_H_
#include "cocos2d.h"


#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

USING_NS_CC;
using namespace std;

class Tools
{
public:
	static void shakeWnd(CCNode* layout);
	static string parseInt2String(int param);
	static int parseString2Int(string& param);
	static void split(const string& src, const string& separator, vector<string>& dest);
    static int createRandom(int start , int end);    //产生一个随机数，从start 到end ,包含start, end.
    static bool isDayOrLight();      //返回白天或黑夜， true 为白天，  6:00 - 17:59为白天
    
    /** 读取本地文件，返回数据 */
    static string getFileByName(string pFileName);
    
    /** 储存内容到文件 */
    static bool saveFile(const char* pContent,string pFileName);
    
    
    
};
#endif