//
//  SGRankDetailCell.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-12.
//
//

#include "SGRankDetailCell.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGShowString.h"

SGRankDetailCell::SGRankDetailCell()
:datas(NULL),
item(NULL),
kuang(NULL),
name(NULL),
level(NULL),
btn(NULL)
{
    
}
SGRankDetailCell::~SGRankDetailCell()
{
    
}



SGRankDetailCell * SGRankDetailCell::create()
{
    
    SGRankDetailCell *item = new SGRankDetailCell();
    
    if (item && item->init())
    {

        item->initView();
        item->autorelease();
        return item;
    }
    CC_SAFE_RELEASE(item);
    return NULL;
}

void SGRankDetailCell::initView()
{
    
    //头像
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_rankLayer);
    item = CCSprite::createWithSpriteFrameName("head1001.png");
    this->addChild(item);
    
    
    // 框
    kuang = CCSprite::createWithSpriteFrameName("officerPinzhi_2_0.png");
    
    kuang->setPosition(item->getPosition());
    //武将名字
    name = SGCCLabelTTF::create("fuck", FONT_BOXINFO, 32);
    name->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width*0.0, -kuang->getContentSize().height*0.6)));
    this->addChild(kuang);
    this->addChild(name);
    
    
    level = SGCCLabelTTF::create("fuck", FONT_BOXINFO, 32);
    level->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width*0.0, -kuang->getContentSize().height*0.6)));
    this->addChild(level);
  
    
    
    
    
    
    
    
    
}



void  SGRankDetailCell::setData(DPI*detail)
{
    
    
    CCString *str; //装备武将等等本体图片
    CCString *str2;//框
    CCString*str3;//武将专用标识国家字段
    CCString*str4;//武将装备名字
    CCString*str5;//武将等级串
    int xx=0;//读plist专用
    

    

    SGOfficerDataModel* obj = SGStaticDataManager::shareStatic()->getOfficerById(1002);
    int starlvl  =obj->getOfficerCurrStarLevel();
    SGOfficerCard *card = new SGOfficerCard;
    card->setHeadSuffixNum(obj->getIconId());
    str = CCString::createWithFormat("head%d.png",card->getHeadSuffixNum());
    str3 = CCString::createWithFormat("countryName%d.png",obj->getOfficerRace());
    str2=  CCString::createWithFormat("officerPinzhi_%d_0.png",2/*starlvl*/);
    str5=  CCString::createWithFormat("lv%d",55);
    str4= CCString::createWithFormat(str_lvBu_);

    btn=SGButton::create(str->getCString(), str3->getCString(), this,menu_selector(SGRankDetailCell::enter) ,CCPointZero,false,true);
    this->addBtn(btn);
//    item->setDisplayFrame(CCSprite::createWithSpriteFrameName(str->getCString())->displayFrame());
//    kuang->setDisplayFrame(CCSprite::createWithSpriteFrameName(str2->getCString())->displayFrame());
    
    name->setString(str4->getCString());
    level->setString(str5->getCString());
    
}




void SGRankDetailCell::enter()
{
}