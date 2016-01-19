//
//  SGFont.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-23.
//
//

#include "SGFont.h"
static std::string fontList[] =
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // custom ttf files are defined in Test-info.plist
    "American Typewriter",
    "Marker Felt",
    "A Damn Mess",
    "Abberancy",
    "Abduction",
    "Paint Boy",
    "Schwarzwald Regular",
    "Scissor Cuts",
#else
    "fonts/A Damn Mess.ttf",
    "fonts/Abberancy.ttf",
    "fonts/Abduction.ttf",
    "fonts/Paint Boy.ttf",
    "fonts/Schwarzwald Regular.ttf",
    "fonts/Scissor Cuts.ttf",
#endif
};


CCRenderTexture* SGFont::createStroke(cocos2d::CCLabelTTF *label, float size, cocos2d::ccColor3B color)
{
    float x=label->getTexture()->getContentSize().width+size*2;
    float y=label->getTexture()->getContentSize().height+size*2;
    CCRenderTexture *rt=CCRenderTexture::create(x, y);
    CCPoint originalPos=label->getPosition();
    ccColor3B originalColor=label->getColor();
    label->setColor(color);
    ccBlendFunc originalBlend=label->getBlendFunc();
    label->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
    CCPoint center=ccp(x/2, y/2);
    rt->begin();
    for (int i=0; i<360; i+=15) {
        float _x=center.x+sin(CC_DEGREES_TO_RADIANS(i))*size;
        float _y=center.y+cos(CC_DEGREES_TO_RADIANS(i))*size;
        
        label->setPosition(ccp(_x, _y));
        label->visit();
    }
    rt->end();
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    float rtX=originalPos.x-size;
    float rtY=originalPos.y-size;
    rt->setPosition(ccp(rtX, rtY));
    return rt;
}
CCLabelTTF* SGFont::createmyfont(const char *string, const char *fontName, float fontSize)
{
    fontName = fontList[0].c_str();
    CCLabelTTF *abc = CCLabelTTF::create(string, fontName,fontSize);
    return abc;

}
