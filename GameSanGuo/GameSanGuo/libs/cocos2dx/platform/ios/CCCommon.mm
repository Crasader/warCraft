/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCCommon.h"

#include <stdarg.h>
#include <stdio.h>
#include "GameConfig.h"
#import <UIKit/UIAlert.h>
#include "GlobalConfig.h"

NS_CC_BEGIN

//cgpADR
#define IS_CGPLOG_OUT 1

//原程序中的打印，打印太多，不用了，用自己定义的CGPLog
#define IS_CCLOG_OUT 0

void CCLog(const char * pszFormat, ...)
{
#if GCPM_INTERNAL_TEST_MODE == GAME_PUBLISH_MODE || GCPM_EXTERNAL_TEST_MODE == GAME_PUBLISH_MODE || IS_CCLOG_OUT
    printf("Cocos2d: ");
    char szBuf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");

#endif
}


//先添加的打印，以前的打印太多，看不出来了。
void GPCCLOG(const char * pszFormat, ...)
{
#if GCPM_INTERNAL_TEST_MODE == GAME_PUBLISH_MODE || GCPM_EXTERNAL_TEST_MODE == GAME_PUBLISH_MODE || IS_CGPLOG_OUT
    printf("cgpPrint: ");
    char szBuf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
    
#endif
}


// ios no MessageBox, use CCLog instead
void CCMessageBox(const char * pszMsg, const char * pszTitle)
{
    NSString * title = (pszTitle) ? [NSString stringWithUTF8String : pszTitle] : nil;
    NSString * msg = (pszMsg) ? [NSString stringWithUTF8String : pszMsg] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: title
                                                          message: msg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
}

void CCLuaLog(const char * pszFormat)
{
    puts(pszFormat);
}

NS_CC_END
