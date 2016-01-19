//
//  SGCCLabelTTF.h
//  Test
//
//  Created by 江南岸 on 13-4-9.
//
//

/*
 *  边框文字，会显示出不同颜色的边框
 */


#ifndef __Test__SGCCLabelTTF__
#define __Test__SGCCLabelTTF__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

//BYMM: 由于大量使用了SGLabelTTF，历史原因，还是使用双Label来替代，不然改动量会比较庞大。
//BYMM：再次修改为单Label。

//@必读：这个控件继承自CCLayerColor，默认的属性是m_bIgnoreAnchorPointForPosition(false),锚点是(0.5,0.5)，但是！这个控件里设置pos和anchor的函数：setPosition()和setAnchorPoint()，都是针对内部的m_mainLabel进行设置，并非对其自身，这样会显得比较混乱。使用这个类的同志请注意这个问题。一句话谨记，你对位置pos和锚点anchor的改变，都是对其内部的CCLabelTTF的改变。
class SGCCLabelTTF : public CCLayerColor
{
private:
//    //四个方向的标签
//    CCLabelTTF *m_upLabel;      //上
//    CCLabelTTF *m_downLabel;    //下
//    CCLabelTTF *m_leftLabel;    //左
//    CCLabelTTF *m_rightLabel;   //右
    
    //@deprecated 描边文本。
    CCLabelTTF *m_outsideLabel;
    //主体文本
    CCLabelTTF *m_mainLabel;
    //描边尺寸
    float _offset;
    //动画时间0
    float m_time;
    //动画时间1
    float m_time1;
    //内颜色
    ccColor3B m_colorInside;
    //外颜色
    ccColor3B m_colorOutSide;
    
    //即init方法。
    void viewDidLoad(const char *str, const char *fileFont, int fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment, const cocos2d::ccColor3B &inColor, const cocos2d::ccColor3B &outColor, const float outSize, bool isOutline);
    
public:
    SGCCLabelTTF();
    
    ~SGCCLabelTTF();
    
//    //简略版本
//    static SGCCLabelTTF *create(const char *str,const char*fileFont,int fontSize);
    
//    //str=文本，fileFont=字体，fontSize=字号，dimensions=文本显示空间，hAlignment=水平对齐方式，vAlignment=垂直对齐方式。  inColor=文本实体颜色，outColor=文本描边颜色，outSize=文本描边尺寸。
//    static SGCCLabelTTF *create(const char *str,const char*fileFont,int fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment = kCCTextAlignmentLeft, CCVerticalTextAlignment vAlignment = kCCVerticalTextAlignmentTop);
    
    
    /*********
    
        @NOTICE: 请务必使用下方这两个create来创建SGLabelTTF，尽最大可能在create时就确定文本内容/对齐方式/文本颜色/描边颜色/描边尺寸。若非别无他法，尽量不要在初始化之后再去使用setString()/setColor()/setInsideColor()/setOutsideColor()等设置参数的函数，会比较耗费。
        (1)括号内为参数的默认值。不限区域/水平居左/垂直居上/白色文本/黑色2px描边。
        (2)如果无需限定文本显示区域，则dimensions参数填CCSizeZero即可。
    
     *********/
    
    //必填：str=文本，fileFont=字体，fontSize=字号。
    //选填：dimensions=文本显示空间（自由），hAlignment=水平对齐方式（居左），vAlignment=垂直对齐方式（居上），inColor=文本实体颜色（白色）。
    //选填：outColor=文本描边颜色（黑色），outSize=文本描边尺寸（2px)，isOutline=是否描边（是）。
    //@NOTICE: 可以不在这create里设置颜色，使用setColor()或setInsideColor()可以设置实体颜色，使用setOutsideColor()可以设置描边颜色。
    static SGCCLabelTTF *create(const char *str,const char*fileFont,int fontSize, const cocos2d::CCSize &dimensions = CCSizeZero, CCTextAlignment hAlignment = kCCTextAlignmentLeft,CCVerticalTextAlignment vAlignment = kCCVerticalTextAlignmentTop, const cocos2d::ccColor3B &inColor = ccWHITE, const cocos2d::ccColor3B &outColor = ccBLACK, const float outSize = 2.0, bool isOutline = true);
    
    //这是一个只有区域/颜色/描边为参数的create函数，默认黑色2px描边/水平居左/垂直居上。如果不够使用，请移步上面的create版本，麻烦但是靠谱。
    static SGCCLabelTTF* create(const char *str,const char*fileFont,int fontSize, const cocos2d::CCSize &dimensions, const cocos2d::ccColor3B &inColor, const cocos2d::ccColor3B &outColor = ccBLACK, const float outSize = 2.0, bool isOutline = true);
    
    //这是一个只有颜色/描边为参数的create函数，默认不限制显示区域/黑色2px描边/水平居左/垂直居上。如果不够使用，请移步上面的create版本，麻烦但是靠谱。
    static SGCCLabelTTF* create(const char *str,const char*fileFont,int fontSize, const cocos2d::ccColor3B &inColor, const cocos2d::ccColor3B &outColor = ccBLACK, const float outSize = 2.0, bool isOutline = true);
    
    //设置位置
    virtual void setPosition(const CCPoint& pos);
    
    //设置文本
    virtual void setString(const char *label);
    
    //返回文本
    const char* getString();
    
    //设置描边颜色
    void setOutSideColor(const ccColor3B &outSideColor);
    
    //设置实体颜色
    void setInsideColor(const ccColor3B &insideColor);
    
    //设置锚点
    virtual void setAnchorPoint(const CCPoint& anchor);
    
    //设置旋转
    virtual void setRotation(float fRotation);

    /*
    //@deprecated
    //设置偏移值
    void setOffset(float offset);
    */
    
    //获得contentSize
    virtual const CCSize &getContentSize();
    
    //获取位置
    virtual const CCPoint& getPosition();
    
    //设置flipx
    void setFlipX(bool b);
    
    //设置flipy
    void setFlipY(bool b);
    
private:
    //设置颜色
    virtual void setColor(const ccColor3B& color);
public:
    
    void runactionWithScale(float duration, float s);
    void runactionWithFlicker();
    void runactionWithFlicker1();
    CCAction* fadin(float time);
    CCAction* fadout(float time);
    void callactionback(CCNode *node,int change);
    void labelstopact();
    void setop(int a);
    void runBlink(float gapTime);
    void runAllBlink(float gapTime);
    inline CCLabelTTF * getMainLabel(){ return m_mainLabel; }
//BYMM: 这三个方法封起来，不允许外部使用。在使用本控件的时候，必须在create时候就确定对齐方式和展示区域。
private:
    //设置对齐方式
    void setHorizontalAlignment(CCTextAlignment alignment);
    void setVerticalAlignment(CCVerticalTextAlignment vAlignment);
    //设置显示区域
    void setDimensions(const CCSize &dim);
};



#endif /* defined(__Test__SGCCLabelTTF__) */
