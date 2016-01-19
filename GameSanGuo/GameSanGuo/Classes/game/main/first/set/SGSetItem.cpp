//
//  SGSetItem.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-11.
//
//

#include "SGSetItem.h"
#include "cocos-ext.h"
#include "SGLayout.h"
#include "GameMusicmanage.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
USING_NS_CC_EXT;
SGSetItem::SGSetItem()
:on(NULL),
off(NULL),
menu1(NULL)
{
}
SGSetItem::~SGSetItem()
{
    CCLOG("~SGStoreItem");
}
bool SGSetItem::initWithSize(int num)
{
	ResourceManager::sharedInstance()->bindTexture("sgsetlayer/sgsetlayer.plist", RES_TYPE_LAYER_UI, sg_setLayer);
 	
    menu1 = CCMenu::create();
    menu1->setPosition(CCPointZero);
    this->addChild(menu1);

    if (!SNSTableViewCellItem::init()) {
		return false;
	}
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *item = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    bgbgbg = NULL;

    this->addChild(item,-1);
//    if(num ==2)
//    {
//        on = SGButton::create("help/blueBtn.png", "help/font_show.png", this,menu_selector(SGSetItem::turnOnHandler),CCPointZero,false,false);
//        off = SGButton::create("friend/friend_grayBtn.png", "help/font_hide.png", this,menu_selector(SGSetItem::turnOffHandler),CCPointZero,false,false);
//    }
//    else{
        on = SGButton::create("blueBtn.png", "font_on.png", this,menu_selector(SGSetItem::turnOnHandler),CCPointZero,false,true);
        off = SGButton::create("friend_grayBtn.png", "font_off.png", this,menu_selector(SGSetItem::turnOffHandler),CCPointZero,false,true);
//    }
    
    
    
    on->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.12, 0)));
    
    menu1->addChild(on,1,num);
    off->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.3, 0)));
    menu1->addChild(off,1,num);
    on->setTag(num);
    off->setTag(100+num);
    
    SGCCLabelTTF *label = SGCCLabelTTF::create("", FONT_PANGWA, FONTSIZE(18));
    this->addChild(label);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.4, 0)));
    
    switch (num) {
        case 0:
        {
            label->setString(str_SetItem_str1);
//            CCSprite *tujian = CCSprite::create("help/font_yinyue.png");
//            tujian->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.35, 0)));
//            tujian->setAnchorPoint(ccp(0, 0.5));
//            this->addChild(tujian);
            std::string str;
            str = CCUserDefault::sharedUserDefault()->getStringForKey("isplaymusic");
            int isplay = CCString::create(str)->intValue();
            if (isplay != 0) {
                off->setImage("blueBtn.png");
                on->setImage("friend_grayBtn.png");
            }
            
            }
            break;
        case 1:
        {
            label->setString(str_SetItem_str2);
            std::string str;
            str = CCUserDefault::sharedUserDefault()->getStringForKey("isplayeffect");
            int isplay = CCString::create(str)->intValue();
            if (isplay != 0) {
                off->setImage("blueBtn.png");
                on->setImage("friend_grayBtn.png");
            }
//            CCSprite *shezhi = CCSprite::create("help/font_yinxiao.png");
//            shezhi->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.35, 0)));
//            shezhi->setAnchorPoint(ccp(0, 0.5));
//            this->addChild(shezhi);
            
        }
            break;
        case 2:
        {
            label->setString(str_SetItem_str3);
//            CCSprite *bangzhu = CCSprite::create("help/font_wangge.png");
//            bangzhu->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.35, 0)));
//            bangzhu->setAnchorPoint(ccp(0, 0.5));
//            this->addChild(bangzhu);                       
        }
            break;
        case 3:
        {
            label->setString(str_SetItem_str3);
//            CCSprite *fankui = CCSprite::create("help/font_tuisong.png");
//            fankui->setPosition(ccpAdd(item->getPosition(), ccp(- item->getContentSize().width*.35, 0)));
//            fankui->setAnchorPoint(ccp(0, 0.5));
//            this->addChild(fankui);
                   }
            break;
        default:
            break;
            
    }    
     return true;
}
SGSetItem* SGSetItem::create(int num)
{
	SGSetItem *instance = new SGSetItem();
	if (instance && instance->initWithSize(num)) {
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}
void SGSetItem::turnOnHandler(cocos2d::CCNode *sender)
{
    EFFECT_PLAY(MUSIC_BTN);

    int type = sender->getTag();
    SGButton *a = (SGButton *)menu1->getChildByTag(type);
    a->setImage("blueBtn.png");
    SGButton *b = (SGButton *)menu1->getChildByTag(type + 100);
    b->setImage("friend_grayBtn.png");
    if (type == 0) {
//        GameSoundControl::shareGameSound()->resumeBGM();
        CCLOG("音乐");
        SET_MUSIC_ON(true);
        MUSIC_PLAY(MUSIC_SCENE);
        CCUserDefault::sharedUserDefault()->setStringForKey("isplaymusic", CCString::createWithFormat("%d",0)->getCString());
        
    }else if(type == 1){
        CCLOG("音效");
        SET_EFFECT_ON(true);
        CCUserDefault::sharedUserDefault()->setStringForKey("isplayeffect", CCString::createWithFormat("%d",0)->getCString());
    }else if(type == 2){
        CCLOG("网格");
    }else if(type == 3){
        CCLOG("推送");
    }
    CCUserDefault::sharedUserDefault()->flush();
}
void SGSetItem::turnOffHandler(cocos2d::CCNode *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    int type = sender->getTag();
    //ResourceManager::sharedInstance()->bindTexture("help/help1.plist", RES_TYPE_LAYER_UI, sg_setLayer);
    
    
    
    SGButton *a = (SGButton *)menu1->getChildByTag(type);
    a->setImage("blueBtn.png");
    SGButton *b = (SGButton *)menu1->getChildByTag(type - 100);
    b->setImage("friend_grayBtn.png");
    if (type == 100) {
//        GameSoundControl::shareGameSound()->pauseBGM();
        CCLOG("100");
        SET_MUSIC_ON(false);
        CCUserDefault::sharedUserDefault()->setStringForKey("isplaymusic", CCString::createWithFormat("%d",1)->getCString());
    }else if(type == 101){
        CCLOG("101");
        SET_EFFECT_ON(false);
        CCUserDefault::sharedUserDefault()->setStringForKey("isplayeffect", CCString::createWithFormat("%d",1)->getCString());
        
    }else if(type == 102){
        CCLOG("102");
    }else if(type == 103){
        CCLOG("103");
    }
    CCUserDefault::sharedUserDefault()->flush();

}