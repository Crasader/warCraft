//
//  SGAnnounceMsg.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-8-5.
//
//
#include "SGPlayerInfo.h"
#include "SGAnnounceMsg.h"
#include "SGRichLabel.h"
#include "ResourceManager.h"
using namespace std;
SGAnnounceMsg *SGAnnounceMsg::create(SGAnnouncementDataModel *a)
{
    SGAnnounceMsg *msg = new SGAnnounceMsg();
    if (msg&&msg->init(NULL,sg_announcemsg)) {
        msg->intiview(a);
        msg->autorelease();
        return msg;
    }
    CC_SAFE_DELETE(msg);
    return NULL;
}
void SGAnnounceMsg::intiview(SGAnnouncementDataModel *datemodel)
{
 
    //    CCDictionary *dic = SGPlayerInfo::sharePlayerInfo()->getmsgstyleDic();
    layerhgt = 0;
    string styleString;
    string content;
    
    CCPoint pt = ccp(0, 0);
    bool isreturn = true;
    float length = 0;
    for (int i = 0; i< 1; i++)
    {
        //        contentproto *cont = (contentproto*)datemodel->getcontentArr()->objectAtIndex(i);
        //        content.assign(cont->getcontent()->getCString());
        //
        //        CCString *style = (CCString*)dic->objectForKey(cont->getstyle()->getCString());
        //        if (style == NULL) {
        //            styleString.assign("null");
        //        }else
        //        {
        //            styleString.assign(style->getCString());
        //        }
        //
        //        CCLOG("content is %s  %d",content.c_str(),content.length());
        //        //        if (content.length()>=4&&content.substr(content.length()-4,4).compare("abcd") == 0)
        //        if (content.length()>=4 &&
        //            (content.find("abcd", 0)!=-1))
        //        {
        //            if (content.find("abcd", 0)==0) {
        //                content = "";
        //            }else
        //            {
        //                content = content.substr(content.length()-4);
        //            }
        //            isreturn= true;
        //            layerhgt += 42;
        //        }else
        //        {
        //            isreturn = false;
        //        }
        //
        //        //        CCLOG("content.c_str()===%s",content.c_str());
        //        CCLabelTTF *telab = CCLabelTTF::create(content.c_str(), "Arial", 30);
        //        addChild(telab,100);
        //        telab->setColor(ccBLACK);
        //        telab->setAnchorPoint(ccp(0, 0.5f));
        //        getstyle(telab,styleString.c_str());
        //        length = telab->getContentSize().width;
        //        telab->setPosition(pt);
        
        SGButton *button = SGButton::create( "box_btnbg.png",
                                            "public_font_queding.png", this, menu_selector(SGAnnounceMsg::AnnounceButton),CCPointZero,false,true);
        //        layer->addChild(button);
        button->setPosition(pt);
        button->setTag(i);
        
        addBtn(button);
        
        
        if (isreturn) {
            pt = ccp(0, pt.y -42);
        }else
        {
            pt = ccp(pt.x + length, pt.y);
        }
        //        CCLOG("positionis  %f  %f",pt.x,pt.y);
    }
    
    
    //    CCDictionary *dic = SGPlayerInfo::sharePlayerInfo()->getmsgstyleDic();
    //    layerhgt = 0;
    //    string styleString;
    //    string content;
    //
    //    CCPoint pt = ccp(0, 0);
    //    bool isreturn;
    //    float length = 0;
    //    for (int i = 0; i<datemodel->getcontentArr()->count(); i++)
    //    {
    //        contentproto *cont = (contentproto*)datemodel->getcontentArr()->objectAtIndex(i);
    //        content.assign(cont->getcontent()->getCString());
    //
    //        CCString *style = (CCString*)dic->objectForKey(cont->getstyle()->getCString());
    //        if (style == NULL) {
    //            styleString.assign("null");
    //        }else
    //        {
    //            styleString.assign(style->getCString());
    //        }
    //
    //        CCLOG("content is %s  %d",content.c_str(),content.length());
    ////        if (content.length()>=4&&content.substr(content.length()-4,4).compare("abcd") == 0)
    //        if (content.length()>=4 &&
    //            (content.find("abcd", 0)!=-1))
    //        {
    //            if (content.find("abcd", 0)==0) {
    //               content = "";
    //            }else
    //            {
    //               content = content.substr(content.length()-4);
    //            }
    //            isreturn= true;
    //            layerhgt += 42;
    //        }else
    //        {
    //            isreturn = false;
    //        }
    //
    ////        CCLOG("content.c_str()===%s",content.c_str());
    //        CCLabelTTF *telab = CCLabelTTF::create(content.c_str(), "Arial", 30);
    //        addChild(telab,100);
    //        telab->setColor(ccBLACK);
    //        telab->setAnchorPoint(ccp(0, 0.5f));
    //        getstyle(telab,styleString.c_str());
    //        length = telab->getContentSize().width;
    //        telab->setPosition(pt);
    //        if (isreturn) {
    //            pt = ccp(0, pt.y -42);
    //        }else
    //        {
    //            pt = ccp(pt.x + length, pt.y);
    //        }
    ////        CCLOG("positionis  %f  %f",pt.x,pt.y);
    //    }
}
int SGAnnounceMsg::returnsize()
{
    return layerhgt;
}
void SGAnnounceMsg::getstyle(CCLabelTTF*lab,std::string testString)
{
    int index = 0;
    int begin = 0;
    while (index <testString.length())
    {
        if (index == testString.find(";", index))
        {
            std::string get = testString.substr(begin,index-begin);
            //            CCLOG("#####get:%s",get.c_str());
            begin = index+1;
            if (get.find(":"))
            {
                std::string styl = get.substr(0,get.find(":"));
                if (styl.compare("color") == 0) {
                    labcolor(lab, get.c_str());
                }else if (styl.compare("fontsize") == 0)
                {
                    labsize(lab, get.c_str());
                }
            }
        }
        index++;
    }
    
}
void SGAnnounceMsg::labsize(CCLabelTTF*lab,std::string a)
{
    std::string get;
    get.assign(a, 9,a.length()-9);
   // CCLOG("%s",get.c_str());
    lab->setFontSize(atoi(get.c_str()));
}
void SGAnnounceMsg::labcolor(CCLabelTTF*lab,std::string a)
{
    std::string get;
    get.assign(a, 6,a.length()-6);
   // CCLOG("%s",get.c_str());
    lab->setColor(GameConfig::colorWithHexString(get.c_str()));
}

void SGAnnounceMsg::AnnounceButton(cocos2d::CCObject *obj)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGButton *item = (SGButton*)obj;
    
    CCLOG("item  tag==%d",item->getTag());
    //    CCMenuItemLabel *button = (CCMenuItemLabel*)obj;
    //    button->getTag();
    //    CCLOG("tag=====%d",button->getTag());
    
}
