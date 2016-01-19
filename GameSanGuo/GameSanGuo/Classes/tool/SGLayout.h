//
//  SGLayout.h
//  TestSG
//
//  Created by gengshengcai on 12-12-1.
//
//

#ifndef __TestSG__SGLayout__
#define __TestSG__SGLayout__

#include "cocos2d.h"
#include "GameConfig.h"
#include "PlatformAdp.h"

using namespace cocos2d;
#define FONTSIZE(size) (GameConfig::getFontSize(size))

#define IS_SIMPLE_VER   0       //是否是简体版本


#if (IS_SIMPLE_VER == 0) //繁体开始

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//modified by caoguoping
#define FONT_LISHU          "FZPWK--GBK1-0.ttf"
#define FONT_PANGWA         "FZPWK--GBK1-0.ttf"    //字体方正胖娃GBK
#define FONT_XINGKAI        "FZPWK--GBK1-0.ttf"
#define FONT_BOXINFO        "FZLSK--GBK1-0.ttf"    //字体方正隶书GBK
#define FONT_FZXK           "FZLSK--GBK1-0.ttf"
#define FONT_PENGYOU        "FZHTK--GBK1-0.ttf"    //字体方正黑体GBK


#else //for Android and others
#define FONT_LISHU          "font/FZPWK--GBK1-0.ttf"
#define FONT_PANGWA         "font/FZPWK--GBK1-0.ttf"    //字体方正胖娃GBK
#define FONT_XINGKAI        "font/FZPWK--GBK1-0.ttf"
#define FONT_BOXINFO        "font/FZLSK--GBK1-0.ttf"    //字体方正隶书GBK
#define FONT_FZXK           "font/FZLSK--GBK1-0.ttf"
#define FONT_PENGYOU        "font/FZHTK--GBK1-0.ttf"    //字体方正黑体GBK
#endif //繁体结束

#else //简体开始

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define FONT_LISHU          "FZPWJW--GB1-0.ttf"
#define FONT_PANGWA         "FZPWJW--GB1-0.ttf"     //字体胖娃 简体
#define FONT_XINGKAI        "FZPWJW--GB1-0.ttf"
#define FONT_BOXINFO        "FZLSJW--GB1-0.ttf"     //字体隶书 简体
#define FONT_FZXK           "FZLSJW--GB1-0.ttf"
#define FONT_PENGYOU        "FZHTJW--GB1-0.ttf"     //黑体     简体
#else //for Android and others
#define FONT_LISHU          "font/FZPWJW--GB1-0.ttf"
#define FONT_PANGWA         "font/FZPWJW--GB1-0.ttf"     //字体胖娃 简体
#define FONT_XINGKAI        "font/FZPWJW--GB1-0.ttf"
#define FONT_BOXINFO        "font/FZLSJW--GB1-0.ttf"     //字体隶书 简体
#define FONT_FZXK           "font/FZLSJW--GB1-0.ttf"
#define FONT_PENGYOU        "font/FZHTJW--GB1-0.ttf"     //黑体     简体
#endif

#endif //简体结束




#define COLOR_GRAY   ccc3(0xc4,0xc4,0xc4)
#define COLOR_ORANGE ccc3(0xff,0x72,0x00)
#define COLOR_YELLOW ccc3(0xff, 0xf0, 0x00)
#define COLOR_GREEN ccc3(0x0c, 0xff, 0x00)
#define COLOR_BLUE ccc3(0x26, 0xd9, 0xff)
#define COLOR_RED ccc3(0xf9, 0x00, 0x00)
#define skewing(x) (SGLayout::getSkewing(x))
#define skewingY(y) (SGLayout::getSkewingY(y))
#define stdSize  (CCSizeMake(768.0f, 1136.0f))  // standard ipad size, background size
#define COLOR_TAN ccc3(143,87,0) //棕黄色  常用作标题处。
#define COLOR_UNKNOW_TAN ccc3(0xff , 0x95 , 0x0c) //未知的棕黄色  。。。
#define COLOR_LIGHT_YELLOW ccc3(251 , 188 , 95) //用途：弹框标题颜色
enum sgLayoutType
{
    kUpLeft,
    kUpCenter,
    kUpRight,
    kMiddleLeft,
    kMiddleCenter,
    KMiddleRight,
    kBottomLeft,
    kBottomCenter,
    kBottomRight
};
class SGLayout
{
public:
    static CCPoint getPoint(sgLayoutType layout);
    static float getSkewing(float x);
    static float getSkewingY(float y);
    static float getProportion();
    static float getProportionX();

};
#endif /* defined(__TestSG__SGLayout__) */
