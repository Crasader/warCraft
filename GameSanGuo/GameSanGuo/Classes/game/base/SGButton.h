//
//  SGButton.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-6.
//
//

#ifndef __GameSanGuo__SGButton__
#define __GameSanGuo__SGButton__

#include "cocos2d.h"
#include "SGCCLabelTTF.h"
#include "SGLayout.h"
USING_NS_CC;

//按钮按了之后的效果。
enum SGButtonEffect
{
    SGBE_GRAY = 0, //变灰（默认）
//    SGBE_MAGNIFY, //备用
    SGBE_GRAY_MAGNIFY, //变灰并且放大
};

class SGButton : public CCMenuItemSprite
{
public:
    SGButton() : normalSprite(NULL), selectedSprite(NULL), normalFont(NULL), selectedFont(NULL), normal_font(NULL), selected_font(NULL), type(0), btnEff_(SGBE_GRAY)
    {};
    
private:

    CCSprite *normalSprite;
    CCSprite *selectedSprite;
    CCSprite* normalFont;
    CCSprite *selectedFont;
    SGCCLabelTTF *normal_font;
    SGCCLabelTTF *selected_font;
    
    SGButtonEffect btnEff_;
public:
    int type;
    /*
     *  @spfile : 按钮的背景图片；
    ＊ @btnfont :按扭上的文字图片(可以为空)；
    ＊ @target ： 按钮的响应目标
    ＊ @selector 按钮的响应事件
     *  @mov 按钮文字的偏移量；
    ＊ @isFilx 按钮是否反转；
    ＊ @isFrame 是否为spriteframe;
     */
    
//    //通过图片设置背景texture（废弃），请使用楼下的setImage()或setImageWithFrame()。
//    void setImagNew(const char *fileName);
    
    //通过图片设置背景texture
    void setImage(const char *fileName);

     void setImageWithFrame(const char *frameName);
    
    //设置前景texture
    void setFontImage(CCSpriteFrame *frame);
    
    //通过frame设置背后的texture
    void setBackFrame(CCSpriteFrame *frame);
    const char * getFont();
    void setFont(const char *font);
    
    void setFontColor(const ccColor3B &color);
    void setOutsideColor(const ccColor3B &color);
    static SGButton *create(const char *spfile, const char *btnfont, CCObject *target, SEL_MenuHandler selector, CCPoint mov = CCPointZero, bool isFilx=false, bool isFrame = true, SGButtonEffect be = SGBE_GRAY_MAGNIFY);
    static SGButton *createFromSprite(CCSprite *icon,CCSprite *icon2, CCObject *target, SEL_MenuHandler selector, CCPoint mov = CCPointZero, bool isFilx=false, bool isFrame = true,bool iscardbutton=false, SGButtonEffect be = SGBE_GRAY);
    static SGButton *createFromLocal(const char *spFile, const char *font, CCObject*target, SEL_MenuHandler selector,CCPoint mov = CCPointZero,const char *fontName = FONT_BOXINFO,const ccColor3B &color = ccRED,float fontSize= 26,bool isFilx=false,bool isFrame=true, SGButtonEffect be = SGBE_GRAY);
    bool init(const char *spfile, const char *btnfont, CCObject *target, SEL_MenuHandler selector, CCPoint mov = CCPointZero, bool isFilx=false, bool isFrame = true);
     bool init(CCSprite *icon, CCSprite *icon2,CCObject *target, SEL_MenuHandler selector, CCPoint mov = CCPointZero, bool isFilx=false, bool isFrame = true,bool iscardbutton=true);//两个sprite不能少,一个是normal,一个是selected
    bool thisinit(const char *spFile, const char *font, CCObject*target, SEL_MenuHandler selector,CCPoint mov = CCPointZero,const char *fontName = FONT_BOXINFO,const ccColor3B &color = ccRED,float fontSize= 26,bool isFilx=false,bool isFrame=false);
    void buttonOutColor();
    void buttonRegainColor();
    void setFontImageScale(float scale);
    void setFontImageColor(const ccColor3B& color);
    
    //修改回调，多形态按钮使用。BYMM
    void setCallBack(CCObject *rec, SEL_MenuHandler selector);
    virtual void selected();
    virtual void unselected();
    
    //返回默认文本
    SGCCLabelTTF* getNormalLabel();
    
    //返回选中文本
    SGCCLabelTTF* getSelectedLabel();
};
#endif /* defined(__GameSanGuo__SGButton__) */
