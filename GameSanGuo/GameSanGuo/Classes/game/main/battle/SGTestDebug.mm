//
//  SGTestDebug.mm
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-11.
//
//

#include "SGTestDebug.h"
#include "cocos2d.h"
#include <string>

USING_NS_CC;

void show()
{
    const int cc = 10 + 6 * 2;
    const char arr[cc] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                            'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int mm = 0; mm < 100; mm++) {
        // get 6 char to string like FF00EE
        std::string str = "#";
        for (int ii = 0; ii < 6; ii++) {
            int rand = arc4random() % cc;
            str += arr[rand];
        }
        LogInfo("%s=================LogInfo", str.c_str());
    }
}

void LogInfo(const char * color, const char * pszFormat, ...)
{
    printf("Cocos2d:");
    
    // start color log
    printf(LC_ESS); printf("%s", color); printf(" ");
    
    char szBuf[kMaxLogLen];
    
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    
    //reset color log
    printf(LC_ESS); printf(LC_ESC);
    
    printf("\n");
}