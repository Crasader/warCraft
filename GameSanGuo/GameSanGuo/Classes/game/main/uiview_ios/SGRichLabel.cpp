//
//  SGRichLabel.mm
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-24.
//
//

#include "SGRichLabel.h"
#include "GameConfig.h"
#include "SGLayout.h"
#include "SGTestSwitch.h"
#include "SGCCLabelTTF.h"
#include <string>
#include <vector>

#define _DEBUG_(format,...) CCLOG("-- Debug --: " format " -- %s -- %d \n",##__VA_ARGS__, __func__, __LINE__)


#if (PLATFORM == IOS)
	#include <CoreGraphics/CoreGraphics.h>
	#include <Foundation/Foundation.h>
	#include "OcCaller.h"

	static NSString* getStringByWidth(NSString *str, id font, float width);
	static int getStringIndexByWidth(NSString *str, id font, float width);
	static CGSize getStringSize(NSString *str, id font, CGSize *constrainSize);

#else
	static CCString* getStringByWidth(CCString *str, void* font, float width);
	static int getStringIndexByWidth(CCString *str, void* font, float width);
	static CCSize getStringSize(CCString *str, void* font, CCSize *constrainSize);
	CCString *substringtoindex (CCString* str,int index);
	CCString *substringWithRange (CCString* str,int index1,int length);
	CCArray *componentsSeparatedByString(CCString *str);
#endif



USING_NS_CC;
using namespace std;


std::vector<std::string>  parseUTF8(const std::string &str)
{
    int l = str.length();
    std::vector<std::string> ret;
    ret.clear();
    for(int p = 0; p < l; )
    {
        int size;
        unsigned char c = str[p];
        if(c < 0x80)
        {
            size = 1;
        }
        else if(c < 0xc2)
        {
        }
        else if(c < 0xe0)
        {
            size = 2;
        }
        else if(c < 0xf0)
        {
            size = 3;
        }
        else if(c < 0xf8)
        {
            size = 4;
        }
        else if (c < 0xfc)
        {
            size = 5;
        }
        else if (c < 0xfe)
        {
            size = 6;
        }
        std::string temp = "";
        temp = str.substr(p, size);
        ret.push_back(temp);
        p += size;
    }
    return ret;
}
std::string subUTF8(const std::string &str,int from, int to)
{
    if(from > to) return "";
    vector<std::string> test = parseUTF8(str);
    if (test.size() < to) return str;
    vector<string>::iterator iter = test.begin();
    std::string res;
    std::string result;
    for(iter=(test.begin() + from); iter != (test.begin() + to); iter++)
    {
        res += *iter;
    }
    return res;
}



CCSprite* SGRichLabel::getFomLabel(const char* cstr, const CCSize &size, const char* fntName, float fontSize, ccColor3B color, ccColor3B defaultInColor,ccColor3B defaultOutColor)
{
	
	
#if (PLATFORM == IOS)
	return OcCaller::getInstance()->getIOSFomLabel(cstr, size, fntName, fontSize, color, defaultInColor, defaultOutColor);
#else
	//android code here
	string labelStr(cstr);
    string temp;
    
    CCSprite *spriteStar = CCSprite::create();
    spriteStar->setTextureRect(CCRectMake(0, 0, size.width, size.height));
    spriteStar->setOpacity(0);
    SGCCLabelTTF *label = NULL;
    
    CCSize cgSzie = CCSizeZero;
    
 	CCLabelTTF *font = CCLabelTTF::create();
	font->setFontName(fntName);
	font->setFontSize(fontSize);
    
    float width = size.width;
    float height = size.height;
    float gwidth = 0;   // global width
    float gheight = 0;  // global heigth
    
    int index1 = 0;
    int index2 = 0;
    int pCount = 0;
    
    while (index2 != string::npos)
    {
        index2 = labelStr.find("#", index1);
        if (index2 != string::npos)
        { // substr label
            pCount ++;
            temp = labelStr.substr(index1, index2-index1-1);
_DEBUG_("%s", temp.c_str());
            index1 = index2 + 1;
            
            CCString *text = CCString::create(temp);
            std::vector<string> u8Vec = parseUTF8(temp);
            
            CCSize textSize = getStringSize(text, font, &cgSzie);
            gwidth += textSize.width;
            
            if ((int)gwidth >= (int)width)
            {
                gwidth -= textSize.width; //恢复原样
_DEBUG_("%s", text->getCString());
                
                CCString *ch=substringWithRange(text,0,1);
                CCSize csize = getStringSize(ch, font, &cgSzie);
_DEBUG_("%s", ch->getCString());

                if ((gwidth + csize.width) > width)
                {
                    // 不能添加一个字符，直接换行
                    gwidth = 0;
                    gheight += csize.height;
                }
                
                int lIndex = 0, rIndex = u8Vec.size();//text->length();
				
                bool chgLine = false; //换行标志位
                while (rIndex > 0 && rIndex <= u8Vec.size() && lIndex < rIndex)
                {
                    CCString *tempstr=substringWithRange(text,lIndex, rIndex-lIndex);
                    CCSize tempSize = getStringSize(tempstr, font, &cgSzie);
                    float discWid = gwidth + tempSize.width;
                    if ((discWid < width))
                    {
                         if (pCount %2 == 0)
                        {// red label
                            label = SGCCLabelTTF::create(tempstr->getCString(), fntName, fontSize, CCSizeZero, color);
                        }
                        else
                        {
                            label = SGCCLabelTTF::create(tempstr->getCString(), fntName, fontSize, CCSizeZero, defaultInColor, defaultOutColor);
                        }
                        label->setAnchorPoint(ccp(0, 0.5f));
                        label->setPosition(ccp(gwidth, height-gheight));
                        spriteStar->addChild(label);
                        
                        // reset some data
                        lIndex = rIndex;
                        rIndex = u8Vec.size();//text->length();
                        if (chgLine)
                        {
                            gwidth = 0;
                            gheight += tempSize.height;
                            chgLine = false;
                        }
                        else
                        {
                            gwidth = tempSize.width;
                        }
                    }else
                    {
                        rIndex--;
                        chgLine = true;
                    }
                }
            }
            else
            {
                gwidth -= textSize.width;
                CCString *cString = CCString::create(temp);
                label = SGCCLabelTTF::create(cString->getCString(), fntName, fontSize);

                if (pCount %2 == 0)
                { // red label
                    label = SGCCLabelTTF::create(cString->getCString(), fntName, fontSize, CCSizeZero, color);
                }
                else
                {
                    label = SGCCLabelTTF::create(cString->getCString(), fntName, fontSize, CCSizeZero, defaultInColor, defaultOutColor);
                }
                label->setAnchorPoint(ccp(0, 0.5f));
                label->setPosition(ccp(gwidth, height-gheight));
                spriteStar->addChild(label);
                gwidth += textSize.width;
            }
        }
    }
    
    // last line string
    temp = labelStr.substr(index1, strlen(cstr));
    //通过vec的真实size获取所需要的真实长度
    std::vector<string> u8Vec = parseUTF8(temp);
    
    int index = 0;
    CCString *text = CCString::create(temp);//2.1.4
    CCSize textSize = getStringSize(text, font, &cgSzie);
    if ((int)(textSize.width+gwidth) > (int)width)
    { // last line bigger than width
        while ((index++) <= u8Vec.size())
        {
_DEBUG_("%s", text->getCString());
            CCString *tempstr=substringWithRange(text,0, index);
            CCSize tempSize = getStringSize(tempstr, font, &cgSzie);
_DEBUG_("%s", tempstr->getCString());
            if ((gwidth + tempSize.width > width))
            {
                // 回溯一个字
                index -= 1;
_DEBUG_("%s", text->getCString());
                
                tempstr=substringWithRange(text,0, index);
                tempSize = getStringSize(tempstr, font, &cgSzie);
                label = SGCCLabelTTF::create(tempstr->getCString(),fntName, fontSize, CCSizeZero, defaultInColor, defaultOutColor);//2.1.4
                label->setAnchorPoint(ccp(0, 0.5f));
                label->setPosition(ccp(gwidth, height - gheight));

                spriteStar->addChild(label);

                gwidth = 0;
                gheight += tempSize.height;
                break;
            }
        }
    }
    
    text=substringWithRange(text,index, (u8Vec.size())-index);
    CCSize tsize = getStringSize(text, font, &cgSzie);

    float lheight = (tsize.width/width+1) * tsize.height;
    
    label = SGCCLabelTTF::create(text->getCString(),fntName, fontSize, CCSizeMake(width, lheight), defaultInColor, defaultOutColor);//2.1.4
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccp(gwidth, height-gheight+tsize.height/2-lheight/2));
    spriteStar->addChild(label);
    
    return spriteStar;
#endif

}

void SGRichLabel::setString(const char *cstr, CCSprite *sprite, CCNode *parent)
{
	
#if (PLATFORM == IOS)
	OcCaller::getInstance()->setString(cstr, sprite, parent);
#else
	//android code here
	CCNode *layer = parent;//(CCLayer *)sprite->getParent();
    CCSize size = sprite->getTextureRect().size;
    SGCCLabelTTF *sgLabel = (SGCCLabelTTF *)sprite->getChildren()->objectAtIndex(0);
    CCLabelTTF *label = (CCLabelTTF *)sgLabel->getChildren()->objectAtIndex(0);
    CCSprite *newSp = getFomLabel(cstr, size, label->getFontName(), label->getFontSize());
    newSp->setPosition(sprite->getPosition());
    newSp->setTag(sprite->getTag());
    layer->removeChild(sprite, true);
    layer->addChild(newSp);
#endif
}

#if (PLATFORM == IOS)
void SGRichLabel::drawOnLabel(const char* cstr, const cocos2d::CCSize &size, const char* fntName, float fontSize)
{
    
}

CGSize getStringSize(NSString *str, id font, CGSize *constrainSize)
{
    NSArray *listItems = [str componentsSeparatedByString: @"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize->width > 0 ? constrainSize->width : 0x7fffffff;
    textRect.height = constrainSize->height > 0 ? constrainSize->height : 0x7fffffff;
    
    for (NSString *s in listItems) {
        CGSize tmp = [s sizeWithFont:font constrainedToSize:textRect];
        
        if (tmp.width > dim.width) {
            dim.width = tmp.width;
        }
        dim.height += tmp.height;
    }
    
    return dim;
}

NSString* getStringByWidth(NSString *str, id font, float width)
{
    CGSize cgSzie = CGSizeZero;
    CGSize tempSize = getStringSize(str, font, &cgSzie);
    if (width >= tempSize.width) {
        return str;
    }
    
    int index = width/tempSize.width * str.length;
    if (index == 0) return nil;
    
    NSString *nstr = [str substringToIndex:index];
    tempSize = getStringSize(nstr, font, &cgSzie);
    if (width > tempSize.width) {
        nstr = [str substringToIndex:index-1];
    }
    return nstr;
}

int getStringIndexByWidth(NSString *str, id font, float width)
{
    CGSize cgSzie = CGSizeZero;
    CGSize tempSize = getStringSize(str, font, &cgSzie);
    if (width >= tempSize.width)
    {
        return str.length;
    }
    
    int index = width/tempSize.width * str.length;
    if (index == 0) return index;
    NSString *nstr = [str substringToIndex:index];
    
    tempSize = getStringSize(nstr, font, &cgSzie);
    if (width > tempSize.width) {
        index -= 1;
    }
    return index;
}
#else
//android code

CCSize getStringSize(CCString *str, void* font, CCSize *constrainSize)
{
	CCLabelTTF *fontSize = (CCLabelTTF * )font;//«ø◊™
	CCArray *listItems=componentsSeparatedByString(str);
#if (0)
    for (int i = 0; i < listItems->count(); i++)
    {
        _DEBUG_("%d -- %s", i, static_cast<CCString *>(listItems->objectAtIndex(i))->getCString());
    }
#endif
    
	CCSize dim = CCSizeZero;
	CCSize textRect = CCSizeZero;
	textRect.width = constrainSize->width > 0 ? constrainSize->width : 0x7fffffff;
	textRect.height = constrainSize->height > 0 ? constrainSize->height : 0x7fffffff;
    
    const char *tmp = NULL;
    CCSize tmpSize;
    CCLabelTTF *tmpTTF = NULL;

    for(int i=0;i<(listItems->count());i++)
    {
        tmp = static_cast<CCString *>(listItems->objectAtIndex(i))->getCString();
        tmpTTF = CCLabelTTF::create(tmp, fontSize->getFontName(), fontSize->getFontSize());

        tmpSize = tmpTTF->getContentSize();
		
		if ((int)tmpSize.width > (int)dim.width)
		{
			dim.width = tmpSize.width;
		}
		dim.height += tmpSize.height;
        
    }
    _DEBUG_("dim  [%f][%f]", dim.width, dim.height);
	return dim;
}

CCString* getStringByWidth(CCString *str, void* font, float width)
{
	CCSize cgSzie = CCSizeZero;
	CCSize tempSize = getStringSize(str, font, &cgSzie);
	if (width >= tempSize.width)
	{
		return str;
	}
    
	int index = width/tempSize.width * str->length();
	if (index == 0) return NULL;
	CCString *nstr=substringtoindex(str,index);
	tempSize = getStringSize(nstr, font, &cgSzie);
	if (width > tempSize.width)
	{
		nstr=substringtoindex(str,index-1);
	}
	return nstr;
    
}

int getStringIndexByWidth(CCString *str, void* font, float width)
{
	CCSize cgSzie = CCSizeZero;
	CCSize tempSize = getStringSize(str, font, &cgSzie);
	if (width >= tempSize.width) {
		return str->length();
	}
    
	int index = width/tempSize.width * (str->length());
	if (index == 0) return index;
	CCString *nstr = substringtoindex(str,index);
	tempSize = getStringSize(nstr, font, &cgSzie);
	if (width > tempSize.width) {
		index -= 1;
	}
	return index;
    
}
CCString *substringtoindex (CCString* str,int index)
{
	std::string s1=str->m_sString;
	string s2(s1,0,index);
	CCString* ns=CCString::createWithFormat("%s",s2.c_str());
	return ns;
}

CCString *substringWithRange(CCString* str,int index1,int length)
{
_DEBUG_("%s", str->getCString());
    std::string res = subUTF8(str->m_sString, index1, index1 + length);
	
	CCString* ns=CCString::create(res);
_DEBUG_("%s", ns->getCString());
	
    return ns;
}

CCArray *componentsSeparatedByString(CCString *str)
{
_DEBUG_("%s", str->getCString());
	CCArray *n=CCArray::create();
	const char *z = "\n";
	char *p=(char *)str->getCString();
	p = strtok(p,z);
	while(p)
	{
		CCString* ns=CCString::createWithFormat("%s",p);
		n->addObject(ns);
		p=strtok(NULL,z);
	}
#if (0)
    for (int i = 0; i < n->count(); i++)
    {
        CCString *tmp = static_cast<CCString *>(n->objectAtIndex(i));
        _DEBUG_("%d -- %s", i, tmp->getCString());
    }
#endif
	return n;
}


#endif