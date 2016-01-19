//
//  SGButton.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-6.
//
//

#include "SGButton.h"

//按钮放大效果的比例。
const static float BUTTON_MAGNIFY_FACTOR = 1.07;    //m cgp

SGButton *SGButton::create(const char *spfile, const char *btnfont, CCObject *target,SEL_MenuHandler selector, CCPoint mov, bool isFilx, bool isFrame, SGButtonEffect be)
{
    SGButton *btn = new SGButton();
    
    if (btn)
    {
        btn->btnEff_ = be;
        if (btn->init(spfile, btnfont, target, selector, mov, isFilx, isFrame))
        {
            btn->autorelease();
            return btn;
        }
    }
    CC_SAFE_DELETE(btn);
    return NULL;
}

SGButton *SGButton::createFromSprite(CCSprite *icon,CCSprite *icon2, CCObject *target,SEL_MenuHandler selector, CCPoint mov, bool isFilx, bool isFrame,bool iscardbutton, SGButtonEffect be)
{
    SGButton *btn = new SGButton();
    
    if (btn)
    {
        btn->btnEff_ = be;
        if (btn->init(icon,icon2, target, selector, mov, isFilx, isFrame,iscardbutton))
        {
            btn->autorelease();
            return btn;
        }
    }
    CC_SAFE_DELETE(btn);
    return NULL;
}
SGButton * SGButton::createFromLocal(const char *spFile, const char *font, CCObject*target, SEL_MenuHandler selector,CCPoint mov,const char *fontName,const ccColor3B &color,float fontSize,bool isFilx,bool isFrame, SGButtonEffect be)
{
    SGButton *btn = new SGButton();
	if (btn)
    {
        btn->btnEff_ = be;
        if (btn->thisinit(spFile, font, target, selector,mov,fontName,color,fontSize,isFilx,isFrame))
        {
            btn->autorelease();
            return btn;
        }
    }
    CC_SAFE_DELETE(btn);
    return NULL;
}
bool SGButton::init(const char *spfile, const char *btnfont, CCObject *target, SEL_MenuHandler selector, CCPoint mov, bool isFilx, bool isFrame)
{   
    if (isFrame)
    {
        normalSprite = CCSprite::createWithSpriteFrameName(spfile);
        
        selectedSprite = CCSprite::createWithSpriteFrameName(spfile);
//        selectedSprite->setColor(ccGRAY);
//        
//        if (SGBE_GRAY_MAGNIFY == btnEff_)
//        {
//            selectedSprite->setScale(BUTTON_MAGNIFY_FACTOR);
//        }
        
//        CCLOG("btnfont==%s",btnfont);
        if (btnfont)
        {
            normalFont  = CCSprite::createWithSpriteFrameName(btnfont);
            normalSprite->addChild(normalFont);
            
            CCSize btnsize = normalSprite->getContentSize();
            normalFont->setPosition(ccpAdd(ccp(btnsize.width * .5, btnsize.height *.5), mov));
            
            selectedFont  = CCSprite::createWithSpriteFrameName(btnfont);
            selectedSprite->addChild(selectedFont);
            selectedFont->setColor(ccGRAY);
            selectedFont->setPosition(ccpAdd(ccp(btnsize.width * .5, btnsize.height *.5), mov));
        }
    }
    else
    {
        normalSprite = CCSprite::create(spfile);
        selectedSprite = CCSprite::create(spfile);
//        selectedSprite->setColor(ccGRAY);
//        
//        if (SGBE_GRAY_MAGNIFY == btnEff_)
//        {
//            selectedSprite->setScale(BUTTON_MAGNIFY_FACTOR);
//        }
        
        if (btnfont)
        {
            CCSize btnsize = selectedSprite->getContentSize();
            
            normalFont  = CCSprite::create(btnfont);
            normalSprite->addChild(normalFont);
            normalFont->setPosition(ccpAdd(ccp(btnsize.width * .5, btnsize.height *.5), mov));
            
            selectedFont  = CCSprite::create(btnfont);
            selectedSprite->addChild(selectedFont);
            selectedFont->setColor(ccGRAY);
            selectedFont->setPosition(ccpAdd(ccp(btnsize.width * .5, btnsize.height *.5), mov));
        }
    }
    normalSprite->setFlipX(isFilx);
    selectedSprite->setFlipX(isFilx);
    CCSprite *disabledImage = CCSprite::createWithSpriteFrame(selectedSprite->displayFrame());
    disabledImage->setColor(ccGRAY);
    normalSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    
    if (!CCMenuItemSprite::initWithNormalSprite(normalSprite, selectedSprite, NULL, target, selector))
    {
        return false;
    }
    
    return true;
}
bool SGButton::init(CCSprite *icon,CCSprite *icon2, CCObject *target, SEL_MenuHandler selector, CCPoint mov, bool isFilx, bool isFrame,bool iscardbutton)
{
    normalSprite = icon;
    normalSprite->setCascadeColorEnabled(true);
    selectedSprite = icon2;
    selectedSprite->setCascadeColorEnabled(true);
    selectedSprite->setColor(ccGRAY);
    
    if (SGBE_GRAY_MAGNIFY == btnEff_)
    {
        selectedSprite->setScale(BUTTON_MAGNIFY_FACTOR);
    }
    
    if (iscardbutton)
    {
        CCSprite* huise = CCSprite::createWithSpriteFrameName("card_bj.png");
        selectedSprite->addChild(huise,100);
        huise->setAnchorPoint(ccp(0,0));
        huise->setPosition(selectedSprite->getPosition());
    }
    normalSprite->setFlipX(isFilx);
    selectedSprite->setFlipX(isFilx);

    normalSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    
    if (!CCMenuItemSprite::initWithNormalSprite(normalSprite, selectedSprite, NULL, target, selector))
    {
        return false;
    }
    return true;
}


bool SGButton::thisinit(const char *spFile, const char *font, CCObject*target, SEL_MenuHandler selector,CCPoint mov,const char *fontName,const ccColor3B &color,float fontSize,bool isFilx,bool isFrame)
{
    CCSprite *normalSprite = NULL;
    CCSprite *selectedSprite = NULL;
	
    if(isFrame)
    {
        normalSprite = CCSprite::createWithSpriteFrameName(spFile);
        selectedSprite = CCSprite::createWithSpriteFrameName(spFile);
    }
        else
	{
	    normalSprite = CCSprite::create(spFile);
        selectedSprite = CCSprite::create(spFile);
	}

    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
	
    normalSprite->setFlipX(isFilx);
    selectedSprite->setFlipX(isFilx);

    selectedSprite->setColor(ccGRAY);
    
    if (SGBE_GRAY_MAGNIFY == btnEff_)
    {
        selectedSprite->setScale(BUTTON_MAGNIFY_FACTOR);
    }
    
    if (font)
    {        
        normal_font = SGCCLabelTTF::create(font, fontName, fontSize , color);
        //modify by:zyc. merge into create.
        //normal_font->setColor(color);
        normalSprite->addChild(normal_font);
        
        selected_font = SGCCLabelTTF::create(font, fontName, fontSize , ccGRAY);
        //modify by:zyc. merge into create.
        //selected_font->setColor(ccGRAY);
        selectedSprite->addChild(selected_font);
        
        CCSize btnsize = normalSprite->getContentSize();
        normal_font->setPosition(ccpAdd(ccp(btnsize.width * .5, btnsize.height *.5), mov));
        selected_font->setPosition(ccpAdd(ccp(btnsize.width * .5, btnsize.height *.5), mov));
    }
    
    CCSprite *disabledImage = CCSprite::createWithSpriteFrame(selectedSprite->displayFrame());
    disabledImage->setColor(ccGRAY);
    if (!CCMenuItemSprite::initWithNormalSprite(normalSprite, selectedSprite, NULL, target, selector))
    {
        return false;
    }
    return true;
}


void SGButton::setOutsideColor(const ccColor3B &color)
{
    normal_font->setOutSideColor(color);
    selected_font->setOutSideColor(color);
}

void SGButton::setBackFrame(CCSpriteFrame *frame)
{
    CCSprite *sprite1 = CCSprite::createWithSpriteFrame(frame);
    sprite1->setColor(ccGRAY);
    CCSprite *sprite2 = CCSprite::createWithSpriteFrame(frame);
    CCSprite *sp1 = (CCSprite*)this->getSelectedImage();
    CCSprite *sp2 = (CCSprite*)this->getNormalImage();
    sp1->setDisplayFrame(sprite1->displayFrame());
    sp2->setDisplayFrame(sprite2->displayFrame());

}

////通过图片设置背景texture（废弃），请使用楼下的setImage()或setImageWithFrame()。
//void SGButton::setImagNew(const char *fileName)
//{
//    CCSprite *sprite1 = CCSprite::create(fileName);
//    sprite1->setColor(ccGRAY);
//    CCSprite *sprite2 = CCSprite::create(fileName);
//    CCSprite *sp1 = (CCSprite*)this->getSelectedImage();
//    CCSprite *sp2 = (CCSprite*)this->getNormalImage();
//    sp1->setDisplayFrame(sprite1->displayFrame());
//    sp2->setDisplayFrame(sprite2->displayFrame());
//}


//通过图片设置背景texture
void SGButton::setImage(const char *fileName)
{
    CCSprite *sprite1 = CCSprite::createWithSpriteFrameName(fileName);
    sprite1->setColor(ccGRAY);
    CCSprite *sprite2 = CCSprite::createWithSpriteFrameName(fileName);
    CCSprite *sp1 = (CCSprite*)this->getSelectedImage();
    CCSprite *sp2 = (CCSprite*)this->getNormalImage();
    sp1->setDisplayFrame(sprite1->displayFrame());
    sp2->setDisplayFrame(sprite2->displayFrame());
}

void SGButton::setImageWithFrame(const char *frameName)
{
    CCSprite *sprite1 = CCSprite::createWithSpriteFrameName(frameName);
    sprite1->setColor(ccGRAY);
    CCSprite *sprite2 = CCSprite::createWithSpriteFrameName(frameName);
    CCSprite *sp1 = (CCSprite*)this->getSelectedImage();
    CCSprite *sp2 = (CCSprite*)this->getNormalImage();
    sp1->setPosition(ccp(getContentSize().width/2 - sprite1->getContentSize().width/2, getContentSize().height/2 - sprite1->getContentSize().height/2));
    sp2->setPosition(sp1->getPosition());
    sp1->setDisplayFrame(sprite1->displayFrame());
    sp2->setDisplayFrame(sprite2->displayFrame());
}

//ÊòæÁ§∫ÊåâÈíÆÊó?
void SGButton::setFontImage(cocos2d::CCSpriteFrame *frame)
{
    if(normalFont && selectedFont)
    {
        normalFont->setDisplayFrame(frame);
        selectedFont->setDisplayFrame(frame);
    }
}
void SGButton::setFontImageScale(float scale)
{
    if(normalFont && selectedFont)
    {
        normalFont->setScale(scale);
        selectedFont->setScale(scale);
    }
}

const char * SGButton::getFont()
{
    return normal_font->getString();
}

void SGButton::setFont(const char *font)
{
        normal_font->setString(font);
        selected_font->setString(font);
}

void SGButton::setFontColor(const ccColor3B &color)
{
    normal_font->setInsideColor(color);
    selected_font->setInsideColor(color);
}
void SGButton::setFontImageColor(const ccColor3B& color)
{
    normalFont->setColor(color);
}

void SGButton::buttonOutColor()
{
    if (normalSprite) {
        normalSprite->outColorDispose();
        //‰∏çÁü•ÂéüÂõ†ÁöÑÂÅèÁß? ÂèØËÉΩÊòØÂàùÂßã‰ΩçÁΩÆÈóÆÈ¢òÂØºËá¥ÁöÑ
        normalSprite->setPosition(ccp(normalSprite->getContentSize().width * 0.5, normalSprite->getContentSize().height * 0.5));
    }
    if (normalFont) {
        normalFont->outColorDispose();
    }
}

void SGButton::buttonRegainColor()
{
    if (normalSprite) {
        normalSprite->initWithTexture(normalSprite->getTexture());
        //‰∏çÁü•ÂéüÂõ†ÁöÑÂÅèÁß?
        normalSprite->setPosition(ccp(normalSprite->getContentSize().width * 0.5, normalSprite->getContentSize().height * 0.5));
    }
    if (normalFont) {
        normalFont->initWithTexture(normalFont->getTexture());
    }
}

//修改回调，多形态按钮使用。BYMM
void SGButton::setCallBack(CCObject *rec, SEL_MenuHandler selector)
{
    CCMenuItemSprite::setTarget(rec, selector);
}
void SGButton::selected()
{
    CCMenuItemSprite::selected();

    
    if (normal_font)
    {
        normal_font->setVisible(false);
    }
    if (selected_font)
    {
        selected_font->setVisible(true);
    }
}

void SGButton::unselected()
{
    CCMenuItemSprite::unselected();
    if (normal_font)
    {
        normal_font->setVisible(true);
    }
    if (selected_font)
    {
        selected_font->setVisible(false);
    }
}

//返回默认文本
SGCCLabelTTF* SGButton::getNormalLabel()
{
    return normal_font;
}

//返回选中文本
SGCCLabelTTF* SGButton::getSelectedLabel()
{
    return selected_font;
}
