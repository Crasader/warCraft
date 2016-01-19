//
//  SGShareBox.cpp
//  GameSanGuo
//
//  Created by dbq on 14-6-4.
//
//

#include "SGShareBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "Invite.pb.h"
#include "CCRenderTexture.h"

#define PICTUREFAIL  "fail"
SGShareBox::~SGShareBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_shareGameBox);
}
SGShareBox * SGShareBox::create(SGBoxDelegate *bdg,ShareType shareType)
{
    SGShareBox *shareBox = new SGShareBox();
    if (shareBox && shareBox->init(bdg,shareType))
    {
        shareBox->autorelease();
        return shareBox;
    }
    CC_SAFE_RELEASE(shareBox);
    return NULL;

}
bool SGShareBox::init(SGBoxDelegate *bdg,ShareType shareType)
{
    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_shareGameBox);
    
    if (!SGBaseBox::init(bdg,box_shareGame,CCSizeMake(535, 400),true))//295
    {
        return false;
    }
    
    SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                    "public_font_qixiao.png",
                                    this,
                                    menu_selector(SGShareBox::boxCloseWithOutCallBack),
                                    ccp(0, 0),
                                    false,
                                    true);
    cancel->setScale(1);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,-150)));
    SGCCLabelTTF*label = NULL;
    
    label = SGCCLabelTTF::create(str_SGShareBox_str1, FONT_PANGWA, 30 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //label->setOutSideColor(ccc3(143,87,0));
    label->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 150)));
    this->addChild(label);
    SGCCLabelTTF*label2 = NULL;

    label2 = SGCCLabelTTF::create(str_SGShareBox_str2, FONT_PANGWA, 16 , ccWHITE , ccc3(143, 87, 0));
    label2->setVisible(false);
    //modify by:zyc . merge into create.
    //label2->setOutSideColor(ccc3(143,87,0));
    label2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 120)));
    this->addChild(label2);
    
    SGCCLabelTTF*label3 = NULL;
    
    label3 = SGCCLabelTTF::create(str_SGShareBox_str3, FONT_PANGWA, 20 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //label3->setOutSideColor(ccc3(0,0,0));
    label3->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-115, -80)));
    this->addChild(label3);
    CCSprite *yubao = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yubao);
    yubao->setPosition(ccpAdd(label3->getPosition(),ccp(label3->getContentSize().width/2+yubao->getContentSize().width/2,0)));
    yubao->setScale(0.7);
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic2 =(CCDictionary *) dic->objectForKey("25");
    int nyuanbao =dic2->valueForKey("value")->intValue();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("26");
    int ntongqian =dic1->valueForKey("value")->intValue();


    SGCCLabelTTF*label4 = NULL;
    
    label4 = SGCCLabelTTF::create(CCString::createWithFormat("x%d",nyuanbao)->getCString(), FONT_PANGWA, 20 , ccGREEN , ccBLACK);
    //modify by:zyc . merge into create.
//    label4->setOutSideColor(ccc3(0,0,0));
//    label4->setColor(ccc3(0,255,0));
    label4->setPosition(ccpAdd(yubao->getPosition(), ccp(label4->getContentSize().width/2+yubao->getContentSize().width/2, 0)));
    this->addChild(label4);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int level = playerInfo->getPlayerLevel();
    
    CCSprite *tongqian = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(tongqian);
    tongqian->setPosition(ccpAdd(label4->getPosition(),ccp(label4->getContentSize().width/2+tongqian->getContentSize().width/2,0)));
    tongqian->setScale(0.7);
    
        SGCCLabelTTF*label5 = NULL;
    
    label5 = SGCCLabelTTF::create(CCString::createWithFormat("x%d",ntongqian)->getCString(), FONT_PANGWA, 20 , ccGREEN , ccBLACK);
//modify by:zyc . merge into create.
//    label5->setOutSideColor(ccc3(0,0,0));
//    label5->setColor(ccc3(0,255,0));
    label5->setPosition(ccpAdd(tongqian->getPosition(), ccp(label5->getContentSize().width/2+tongqian->getContentSize().width/2, 0)));
    this->addChild(label5);
    
    
    CCSprite *kuang2 = CCSprite::createWithSpriteFrameName("equipPinzhi_5_3.png");
    this->addChild(kuang2,101);
    kuang2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 30)));
    
    SGButton *button3 = SGButton::create("card_bj.png",
                                         "sharefacebook-3.png",
                                         this,
                                         menu_selector(SGShareBox::onClick3),
                                         ccp(0, 0),
                                         false,
                                         true);
    //button1->setScale(0.7);
    this->addBtn(button3);
    button3->setPosition(kuang2->getPosition());
    
    CCSprite *namebg3 = CCSprite::createWithSpriteFrameName("box_name_bg.png");
    this->addChild(namebg3);
    namebg3->setPosition(ccpAdd(kuang2->getPosition(),ccp(0,-kuang2->getContentSize().height/2)));
    SGCCLabelTTF*name3 = NULL;
    
    name3 = SGCCLabelTTF::create("facebook", FONT_PANGWA, 16 , ccWHITE , ccBLACK);
    //modify by:zyc . merge into create.
    //name3->setOutSideColor(ccc3(0,0,0));
    name3->setPosition(ccpAdd(namebg3->getPosition(), ccp(0, 0)));
    this->addChild(name3);

    m_shareType = shareType;
    
    setTouchEnabled(true);
	return true;
    
}

std::string SGShareBox::creatSharePicture(ShareType shareType)
{
    std::string sharePictureInfo("pengyouSharePic.png");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    switch (shareType) {
        case SGShareTypeGeneral:
        {
            CCPoint center = SGLayout::getPoint(kMiddleCenter);
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/login_ip5.plist", RES_TYPE_LAYER_UI, sg_shareGameBox);
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/logo.plist", RES_TYPE_LAYER_UI, sg_shareGameBox);
            ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_shareGameBox);

            CCSprite *welecomeBg = CCSprite::createWithSpriteFrameName("login_ip5.png");
            CCSprite *logo = CCSprite::createWithSpriteFrameName("logo.png");
            CCSprite *login_start = CCSprite::createWithSpriteFrameName("login_start.png");
            welecomeBg->addChild(logo);
            logo->setVisible(false);
            welecomeBg->addChild(login_start);
            
            welecomeBg->setScaleY(size.height / welecomeBg->getContentSize().height);
            welecomeBg->setScaleX(size.width / welecomeBg->getContentSize().width);
            
            
            logo->setPosition(ccpAdd(center, ccp(0, welecomeBg->getContentSize().height * 0.11)));
            login_start->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, screenheight * 0.15)));
 
            welecomeBg->setPosition(center);

            CCRenderTexture* sharePicture = CCRenderTexture::create(size.width, size.height,kCCTexture2DPixelFormat_RGBA8888);
            
            sharePicture->begin();
            welecomeBg->visit();
            sharePicture->end();
            
            if(!sharePicture->saveToFile(sharePictureInfo.c_str(),kCCImageFormatPNG))
            {
                sharePictureInfo = std::string(PICTUREFAIL);
            }
            CC_SAFE_DELETE(sharePicture);

        }
            break;
        case SGShareTypeVisit:
        case SGShareTypeShoujingzhou:
        case SGShareTypeGeneralFight:
        {
            
            
            CCRenderTexture* screen = CCRenderTexture::create(size.width, size.height,kCCTexture2DPixelFormat_RGBA8888);
            CCScene* temp = SGMainManager::shareMain()->getMainScene();//  CCDirector::sharedDirector()->getRunningScene();
          
            screen->begin();
            temp->visit();
            screen->end();
            
            if(!screen->saveToFile(sharePictureInfo.c_str(),kCCImageFormatPNG))
            {
                sharePictureInfo = std::string(PICTUREFAIL);
            }
            CC_SAFE_DELETE(screen);
        }
            break;
        case SGShareTypeBossBattel:
        {
            
            CCSprite *bossBg = CCSprite::create("sanguobigpic/boss_fight_bg.png");
            CCSprite *center_title = CCSprite::createWithSpriteFrameName("boss_title.png");
            bossBg->addChild(center_title);
            center_title->setPosition(ccp(bossBg->getContentSize().width/2 - center_title->getContentSize().width/2, bossBg->getContentSize().height/2));
         
            bossBg->setPosition(ccp(0, 742));
            bossBg->setAnchorPoint(ccp(0, 1));
            
            CCRenderTexture* sharePicture = CCRenderTexture::create(644,742,kCCTexture2DPixelFormat_RGBA8888);
            sharePicture->begin();
            bossBg->visit();
            sharePicture->end();
            
            if(!sharePicture->saveToFile(sharePictureInfo.c_str(),kCCImageFormatPNG))
            {
                sharePictureInfo = std::string(PICTUREFAIL);
            }
            CC_SAFE_DELETE(sharePicture);
            
    
        }
            break;

        default:
            break;
    }
    return sharePictureInfo;
}

bool SGShareBox::createShareData()
{
    this->setVisible(false);
    m_sharePictureName = creatSharePicture(m_shareType);
    this->setVisible(true);
    
    if (m_sharePictureName.compare(PICTUREFAIL) == 0)
    {
        return false;
    }
    return true;
}

void SGShareBox::onClick1()
{
    main::ShareRewardRequest *request = new main::ShareRewardRequest();
    request->set_platformid(0);
    SGSocketClient::sharedSocketClient()->send(MSG_SHARE_REWARD, request);
    CCLOG("微信朋友圈一分享成功了!");
    
}
void SGShareBox::onClick2()
{
    main::ShareRewardRequest *request = new main::ShareRewardRequest();
    request->set_platformid(1);
    SGSocketClient::sharedSocketClient()->send(MSG_SHARE_REWARD, request);
    CCLOG("twitter一分享成功了!");

}
void SGShareBox::onClick3()
{
    
    if (createShareData() )
    {
//        setIsCanTouch(false);
        
        SGMainManager::shareMain()->shareGameById(2,(int)m_shareType,m_sharePictureName);
    }

//    main::ShareRewardRequest *request = new main::ShareRewardRequest();
//    request->set_platformid(2);
//    SGSocketClient::sharedSocketClient()->send(MSG_SHARE_REWARD, request);
//    CCLOG("facebook一分享成功了!");

}
void SGShareBox::onClick4()
{
    main::ShareRewardRequest *request = new main::ShareRewardRequest();
    request->set_platformid(3);
    SGSocketClient::sharedSocketClient()->send(MSG_SHARE_REWARD, request);
    CCLOG("sina一分享成功了!");

}
void SGShareBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_34);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
    }
    SGBaseBox::boxCloseWithOutCallBack();

}


//@NOTICE: 守荆州&胜利，也添加触控，但是后续流程交由玩家处理。
void SGShareBox::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);//11.17
}

bool SGShareBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}






