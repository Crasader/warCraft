//
//  SGServerItem.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#include "SGServerItem.h"
#include "SGLayout.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

//10.25//george

SGServerItem::SGServerItem()
:borderSprite(NULL),
spriteServerNew(NULL),
spriteIsLogin(NULL),
labelserverState(NULL),
labelRoleNike(NULL),
labelAr(NULL),
labelNoRoles(NULL),
labelServerName(NULL)
{
    
}

SGServerItem::~SGServerItem()
{
}


bool SGServerItem::init()
{
    if (!SNSTableViewCellItem::init())
    {
        return false;
    }
    
    //背景
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_serverListLayer);
	ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_serverListLayer);
	
    borderSprite = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    bgbgbg = borderSprite;
//    borderSprite->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 102));
    this->addChild(borderSprite);
    
    spriteServerNew = CCSprite::createWithSpriteFrameName("login_server_new.png");
    spriteIsLogin = CCSprite::createWithSpriteFrameName("server_online.png");
    labelRoleNike = SGCCLabelTTF::create(str_ServerItem_str1, FONT_BOXINFO, 24);
    labelNoRoles = SGCCLabelTTF::create(str_ServerItem_str2, FONT_BOXINFO, 24);
    labelServerName = SGCCLabelTTF::create(str_ServerItem_str3,FONT_BOXINFO, 32 , ccYELLOW);
    labelAr = SGCCLabelTTF::create("",FONT_BOXINFO, 32);
    labelserverState = SGCCLabelTTF::create("",FONT_BOXINFO, 26);
    labelServerName->setAnchorPoint(ccp(0, 0.5));
    labelAr->setAnchorPoint(ccp(1, 0.5));
    
    this->addChild(spriteServerNew);
    this->addChild(labelAr);
    this->addChild(labelserverState);
    this->addChild(spriteIsLogin);
//    this->addChild(labelRoleCountry);
    this->addChild(labelRoleNike);
    this->addChild(labelNoRoles);
//    this->addChild(labelRoleLevel);
    this->addChild(labelServerName);

//    this->ignoreAnchorPointForPosition(false);
//    this->setAnchorPoint(ccp(0, 0));
    
    labelNoRoles->setPosition(ccp(0, -20));
  //  labelRoleCountry->setPosition(ccp(SGLayout::getSkewing(-80),-20));
    labelRoleNike->setPosition(ccp(0, -20));
  //  labelRoleLevel->setPosition(ccp(SGLayout::getSkewing(100), -20));
    //modify by:zyc. merge into create.
    //labelServerName->setColor(ccYELLOW);
    labelServerName->setPosition(ccp(-50,20));
    labelAr->setPosition(ccpAdd(borderSprite->getPosition(), ccp(-65, labelServerName->getPosition().y)));
    labelserverState->setPosition(ccp(120,labelServerName->getPosition().y));
    spriteIsLogin->setAnchorPoint(ccp(1, 0.5));
    spriteIsLogin->setPosition(ccpAdd(borderSprite->getPosition(), ccp(borderSprite->getContentSize().width*0.5 - 10 , -20)));
    spriteServerNew->setPosition(ccpAdd(borderSprite->getPosition(), ccp(-borderSprite->getContentSize().width*.5 + 27 , borderSprite->getContentSize().height*.5 - 32)));
    return true;
}

void SGServerItem::updataData(CCObject *obj)
{
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        
        int serverIsNew = ((CCString*)dict->objectForKey("serverIsNew"))->intValue();

        if (serverIsNew) {
            spriteServerNew->setVisible(true);
        }else
        {
            spriteServerNew->setVisible(false);
        }
        CCString *serverName = (CCString*)dict->objectForKey("serverName");
        //MM: 显示为“1服”，“9服”等，原为使用serverId，现改为serverShowId。让serverId成为服务器唯一ID。
        int serverIdNow = ((CCString*)dict->objectForKey("serverShowId"))->intValue();
        
        labelServerName->setString(serverName->getCString());
        labelAr->setString(CCString::createWithFormat("%d服",serverIdNow)->getCString());
        short serverState = ((CCString*)dict->objectForKey("serverState"))->intValue();
        // 1.维护 2.通畅 3.火爆
        ccColor3B color;
        if(serverState==1){
            
            color.r=0x8e;
            color.g=0x8e;
            color.b=0x8e;
            
            labelserverState->setString(str_ServerItem_str4);
            labelAr->setInsideColor(color);
            labelServerName->setInsideColor(color);
            labelserverState->setInsideColor(color);
        }else if(serverState==2){
            color.r=0x00;
            color.g=0xec;
            color.b=0x00;
            labelserverState->setString(str_ServerItem_str5);
            labelAr->setInsideColor(color);
            labelServerName->setInsideColor(color);
            labelserverState->setInsideColor(color);
        }else if(serverState==3){
            color.r=0xff;
            color.g=0x00;
            color.b=0x00;
            labelserverState->setString(str_ServerItem_str6);
            labelAr->setInsideColor(color);
            labelServerName->setInsideColor(color);
            labelserverState->setInsideColor(color);
        }
        else
        {
            color.r=0x00;
            color.g=0xec;
            color.b=0x00;
            labelserverState->setString(str_ServerItem_str7);
            labelAr->setInsideColor(color);
            labelServerName->setInsideColor(color);
            labelserverState->setInsideColor(color);
        }

        //是否登陆
        int serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
        spriteIsLogin->setVisible(serverId==serverIdNow);
        
        if (serverId == serverIdNow) {
            borderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("item_bigYw.png")->displayFrame());
        }
        
        short ishasRole = ((CCString*)dict->objectForKey("isHasRole"))->intValue();        
        
        //有角色
        if(ishasRole==1){
            labelNoRoles->setVisible(false);
            
            CCString *nike = (CCString*)dict->objectForKey("roleNikeName");
            
            
            CCString *roleLevel = (CCString*)dict->objectForKey("roleLevel");
       
         //   labelRoleLevel->setString(str->getCString());
            
            short country = ((CCString*)dict->objectForKey("countryId"))->intValue();
            //1.蜀 2.魏 3.吴
//            if(country==1){
//               labelRoleNike->setString(CCString::createWithFormat(str_ServerItem_str8,nike->getCString(),roleLevel->getCString())->getCString());
////                labelRoleCountry->setString("蜀国");
//            }else if(country==2){
//                labelRoleNike->setString(CCString::createWithFormat(str_ServerItem_str9,nike->getCString(),roleLevel->getCString())->getCString());
////                labelRoleCountry->setString("魏国");
//            }else if(country==3){
//                labelRoleNike->setString(CCString::createWithFormat(str_ServerItem_str10,nike->getCString(),roleLevel->getCString())->getCString());
////                labelRoleCountry->setString("吴国");
//                
//            }
            if(country==1){
                labelRoleNike->setString(CCString::createWithFormat("武士  %s  %s级", nike->getCString(),roleLevel->getCString())->getCString());
                //                labelRoleCountry->setString("蜀国");
            }else if(country==2){
                labelRoleNike->setString(CCString::createWithFormat("武士  %s  %s级",nike->getCString(),roleLevel->getCString())->getCString());
                //                labelRoleCountry->setString("魏国");
            }else if(country==3){
                labelRoleNike->setString(CCString::createWithFormat("武士  %s  %s级",nike->getCString(),roleLevel->getCString())->getCString());
                //                labelRoleCountry->setString("吴国");
                
            }
            
            
            
            labelNoRoles->setVisible(false);
            
        }else{
//            labelRoleCountry->setString("");
//            labelRoleLevel->setString("");
            labelRoleNike->setString("");
            labelNoRoles->setVisible(true);
        }
    }
}