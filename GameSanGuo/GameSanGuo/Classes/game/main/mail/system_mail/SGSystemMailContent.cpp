//
//  SGSystemMailContent.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-28.
//
//

#include "SGSystemMailContent.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGOfficerDataModel.h"
#include "SGEquipmentDataModel.h"
#include "SGSoldierDataModel.h"
#include "SGPropsDataModel.h"
#include "SGSystemMailLayer.h"
#include "SGTools.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGSystemMailContent::SGSystemMailContent()
: labelSender(NULL)
, sprite_scrollview_angle(NULL)
, sprite_scrollview_bg(NULL)
, sprite_content_bg_angle(NULL)
, sprite_content_bg_bg(NULL)
, showHeadIconlayer(NULL)
, m_bIsReceived(false)
{
    labelAccessory = NULL;
    labelContent = NULL;
    labelSendTime = NULL;
    scrollAccessory = NULL;
    buttonReceiveAcc = NULL;
    m_bIsReceived = false;
    m_pSystemMail = NULL;
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MAIL_READSYSTEMMAIL,this, callfuncO_selector(SGSystemMailContent::readMailListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MAIL_GETACCOSSORY, this, callfuncO_selector(SGSystemMailContent::receiveAccessoryListener));
}

SGSystemMailContent::~SGSystemMailContent()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_MAIL_READSYSTEMMAIL);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_MAIL_GETACCOSSORY);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_systemMailContent);
}

SGSystemMailContent* SGSystemMailContent::create(SGMailSystemDataModel *systemMail)
{
    SGSystemMailContent *system = new SGSystemMailContent();
    if(system && system->init(NULL, sg_systemMailContent))
    {
        system->m_bIsReceived = systemMail->getIsReceive()==0;
        system->m_pSystemMail = systemMail;
        
        system->viewDidLoad();
        system->autorelease();
        return system;
    }
    
    CC_SAFE_DELETE(system);
    return system;
}

void SGSystemMailContent::viewDidLoad()
{
    //如果邮件现在处于未读状态
    if(!m_pSystemMail->getIsRead())
    {
        main::ShowSystemMailRequest *mail = new main::ShowSystemMailRequest();
        mail->set_mailid(m_pSystemMail->getSsid());
        SGSocketClient::sharedSocketClient()->send(MSG_MAIL_READSYSTEMMAIL, mail);
    }
    
    //-------------------------------------------------------------------------------------
    //界面布局
    //-------------------------------------------------------------------------------------
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    
    //星形背景
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);

    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30+150+72,spriteBackground->getContentSize().width, SGLayout::getSkewingY(480)-47-150-72-61-101);
    //spriteBackground->cocos2d::CCNode::setScale(r.size.width / spriteBackground->getContentSize().width, r.size.height / spriteBackground->getContentSize().height);
    spriteBackground->setTextureRect(r);
    spriteBackground->setScaleX(s.width/spriteBackground->getContentSize().width * 1.01);
    spriteBackground->setScaleX(CCDirector::sharedDirector()->getWinSize().width / spriteBackground->getContentSize().width);
    spriteBackground->setAnchorPoint(ccp(0.5, 1));
    spriteBackground->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72)));
    this->addChild(spriteBackground);
    
    //标题
    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("mail_title_1.png");
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72/2)));
    this->addChild(spriteTitle);
    
    //返回按钮
    //五张图片的尺寸   -47-150-72-56-101-30
//    SGButton *buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png", this, menu_selector(SGSystemMailContent::buttonClickGoBack),CCPointZero,false,true);
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSystemMailContent::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    buttonBack->setPosition(ccpAdd(upLeft, ccp(0, -47-150-72/2)));
    addBtn(buttonBack);
    
    //收取附件
//    buttonReceiveAcc = SGButton::create("public_btn_1.png", "mail_button_accessory.png", this, menu_selector(SGSystemMailContent::buttonClickAccessory),CCPointZero,false,true);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/battle_btn_sbg_n.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
    
    buttonReceiveAcc=SGButton::createFromLocal("battle_btn_sbg_n.png", str_FriendMailLayer_str13, this,
                                               menu_selector(SGSystemMailContent::buttonClickAccessory) ,CCPointZero,FONT_BOXINFO,ccWHITE,23);
    buttonReceiveAcc->setPosition(ccpAdd(bottomCenter, ccp(0, 101+56+20)));
    addBtn(buttonReceiveAcc);
    
    
    float hight=buttonBack->getPosition().y-buttonReceiveAcc->getPosition().y;
    
   
    
    float fixHeight=0;
    float fixHeight2=0;
    if (s.height==1136)
    {
        fixHeight=30;
        fixHeight2=-15;
    }
     float svHigh = skewingY(170/2);

    int count=m_pSystemMail->getAccessoryArray()->count();
    //附件标题
    CCString *labelAccessoryStr = CCString::createWithFormat(str_FriendMailLayer_str14,count);
    labelAccessory = SGCCLabelTTF::create(labelAccessoryStr->getCString(), FONT_BOXINFO, 30 ,ccWHITE);
    labelAccessory->setAnchorPoint(ccp(0, 0.5));
    labelAccessory->setPosition(ccp(50, buttonReceiveAcc->getPositionY()+skewingY(100)+10+fixHeight));
    //    labelAccessory->setOutSideColor(ccWHITE);
    //modify by:zyc. merge into create.
    //labelAccessory->setColor(ccWHITE);
    addChild(labelAccessory);
    
    
    
    
    //scrollview
    sprite_scrollview_angle = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    sprite_scrollview_bg = CCScale9Sprite::createWithSpriteFrameName("server_BlueBG.png");
//    sprite_scrollview_angle->setPreferredSize(CCSizeMake(SGLayout::getSkewing(310), svHigh));
//    sprite_scrollview_bg->setPreferredSize(CCSizeMake(SGLayout::getSkewing(310), svHigh));
    sprite_scrollview_angle->setPosition(ccpAdd(buttonReceiveAcc->getPosition(),ccp(0, sprite_scrollview_angle->getContentSize().height/2+buttonReceiveAcc->getContentSize().height/2+fixHeight)));
    sprite_scrollview_bg->setPosition(sprite_scrollview_angle->getPosition());
    addChild(sprite_scrollview_bg);
    addChild(sprite_scrollview_angle);

    
    showHeadIconlayer=SGShowHeadIconLayer::create(m_pSystemMail->getAccessoryArray(), 4,CCSizeMake(295, 85));
    addChild(showHeadIconlayer);
    
    showHeadIconlayer->setPosition(ccpAdd(buttonReceiveAcc->getPosition(), ccp(-skewing(160) + 27, sprite_scrollview_bg->getContentSize().height/2-buttonReceiveAcc->getContentSize().height+fixHeight+fixHeight2)));

    
 
    
    
//    //删除按钮
//    buttonDelete = SGButton::create("friend_redButton.png","friend_delete.png", this, menu_selector(SGSystemMailContent::buttonClickDelete),CCPointZero,false,true);
//    buttonDelete->setPosition(ccpAdd(bottomCenter, ccp(100, 101+56+40)));
//    addBtn(buttonDelete);
    
 
  /*
    scrollview = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(280), svHigh));
    CCLayer *container = CCLayer::create();
    scrollview->setContainer(container);
    
    CCArray *arraySprite = getSpriteByType();
    int spriteCount = arraySprite->count();
    int width = 54;
    for(int i=0;i<spriteCount;i++)
    {
        CCSprite *sprite = (CCSprite*)arraySprite->objectAtIndex(i);
        container->addChild(sprite);
        sprite->setPosition(ccp(width, 50));
        width += 108;
    }
    
    //如果width很短，那么我们就把它设成scrollview的宽度
    if(width < scrollview->cocos2d::CCNode::getContentSize().width)
    {
        width = scrollview->cocos2d::CCNode::getContentSize().width;
    }
    
    scrollview->setContentSize(CCSizeMake(width, svHigh));
    container->setContentSize(CCSizeMake(width, svHigh));
    
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    scrollview->setPosition(ccp(SGLayout::getSkewing(20), bottomCenter.y+240));
    addChild(scrollview);
    */
    
 
    
    //邮件内容
    sprite_content_bg_angle = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    sprite_content_bg_angle->setPreferredSize(CCSizeMake(SGLayout::getSkewing(300),hight-SGLayout::getSkewingY(85)-80));
    sprite_content_bg_bg = CCScale9Sprite::createWithSpriteFrameName("server_BlueBG.png");
    sprite_content_bg_bg->setPreferredSize(CCSizeMake(SGLayout::getSkewing(300), hight-SGLayout::getSkewingY(85)-80));
    sprite_content_bg_angle->setAnchorPoint(ccp(0.5, 1));
    sprite_content_bg_bg->setAnchorPoint(ccp(0.5, 1));
    sprite_content_bg_angle->setPosition(ccpAdd(upCenter, ccp(0, -330)));
    sprite_content_bg_bg->setPosition(sprite_content_bg_angle->getPosition());
    addChild(sprite_content_bg_bg);
    addChild(sprite_content_bg_angle);
    
    sprite_scrollview_angle->setPreferredSize(CCSize(sprite_content_bg_angle->getContentSize().width ,svHigh ) );
    sprite_scrollview_bg->setPreferredSize(CCSize( sprite_content_bg_angle->getContentSize().width ,svHigh ) );
    
    //BYMM: 设置内容文本，并提前获得将要显示内容文本的区域。
    CCSize cttSize = m_bIsReceived ? CCSizeMake(SGLayout::getSkewing(260), 240) : CCSizeMake(SGLayout::getSkewing(260), 460);
    
    labelContent = SGCCLabelTTF::create(m_pSystemMail->getcontent()->getCString(), FONT_BOXINFO, 24, cttSize); //这里原来使用的是测试文字，str_FriendMailLayer_str15。
    labelContent->setAnchorPoint(ccp(0.5, 1));
    labelContent->setPosition(ccp(center.x, sprite_content_bg_angle->getPosition().y-20));
    addChild(labelContent);
    
    //--------------------------------------------------------------------------------------------
    //发件人
    CCScale9Sprite *sprite_reciever_bg = CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png");
    sprite_reciever_bg->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72-30)));
    sprite_reciever_bg->setPreferredSize(CCSizeMake(SGLayout::getSkewing(290) , 40));
    addChild(sprite_reciever_bg);
    
    CCSprite *sprite_sender_sender = CCSprite::createWithSpriteFrameName("mail_sender.png");
    sprite_sender_sender->setAnchorPoint(ccp(0, 0.5));
    sprite_sender_sender->setPosition(ccpAdd(sprite_reciever_bg->getPosition(), ccp(-sprite_reciever_bg->getContentSize().width/2, 0)));
    addChild(sprite_sender_sender);
    
    labelSender = CCLabelTTF::create(str_FriendMailLayer_str11, FONT_BOXINFO, 24);
    labelSender->setAnchorPoint(ccp(0, 0.5));
    labelSender->setPosition(ccpAdd(sprite_sender_sender->getPosition(), ccp(sprite_sender_sender->getContentSize().width*1.5, 0)));
    labelSender->setColor(ccWHITE);
    addChild(labelSender);
    
    //发件时间
    labelSendTime = CCLabelTTF::create(SGTools::getTimeDistanceNow(m_pSystemMail->getSendTime()).c_str(), FONT_BOXINFO, 24);
    labelSendTime->setAnchorPoint(ccp(0, 0.5));
    labelSendTime->setPosition(ccpAdd(sprite_reciever_bg->getPosition(), ccp(100, 0)));
    labelSendTime->setColor(ccWHITE);
    addChild(labelSendTime);
    //发件人
    //--------------------------------------------------------------------------------------------
    
    receiveAccessory();
    
    buttonReceiveAcc->setPosition(ccpAdd(buttonReceiveAcc->getPosition(), ccp(0, -5)));
}

//收取附件，控制窗口变大变小
void SGSystemMailContent::receiveAccessory()
{
    if(m_bIsReceived)
    {
        sprite_content_bg_angle->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), skewingY(110)));
        sprite_content_bg_bg->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), skewingY(110)));
    }
    else
    {
        sprite_content_bg_angle->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 465));
        sprite_content_bg_bg->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 465));
        
        sprite_scrollview_angle->setVisible(false);
        sprite_scrollview_bg->setVisible(false);
        labelAccessory->setVisible(false);
        
        buttonReceiveAcc->setVisible(false);
        showHeadIconlayer->setVisible(false);
        
    }
    sprite_scrollview_angle->setPreferredSize(CCSize(sprite_content_bg_angle->getContentSize().width ,sprite_scrollview_angle->getContentSize().height ) );
    sprite_scrollview_bg->setPreferredSize(CCSize( sprite_content_bg_angle->getContentSize().width ,sprite_scrollview_angle->getContentSize().height ) );
}

//根据不同的类型显示出不同的附件 /*垃圾代码，坐等优化*/
/*CCArray* SGSystemMailContent::getSpriteByType()
{
    CCArray *arraySprite = CCArray::create();
//    ResourceManager::sharedInstance()->bindTexture("store/store.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
    
    CCArray *array = m_pSystemMail->getAccessoryArray();
    for(int i=0;i<array->count();i++)
    {
        SGMailSystemAccessoryDataModel *access = (SGMailSystemAccessoryDataModel*)array->objectAtIndex(i);
        
        CCSprite *sprite = NULL;
        //1:将领卡牌 2:装备卡牌 3:士兵卡牌 4:道具卡牌 5:铜钱 6:金子 7:军功 8:炼丹秘方 9:小喇叭
        switch(access->getType())
        {
            case 1://将领卡牌
            {
                SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(access->getItemId());
                if(officerData)
                {
//                    sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",officerData->getOfficerId())->getCString());
                    SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_systemMailContent);
                    sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",officerData->getIconId())->getCString());//12.06
                    int starlvl = officerData->getOfficerCurrStarLevel();
                    if (starlvl>1) {
                        starlvl-=1;
                    }
                    CCSprite *sprite_rect = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",starlvl)->getCString());
//                    CCSprite *sprite_rect = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d.png",officerData->getOfficerVisitType())->getCString());
                    sprite_rect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                    sprite->addChild(sprite_rect);
                    if (sprite_rect)
                    {
                        sprite_rect->removeChildByTag(30);
                        
//                        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sgcardslayer/sgcardslayer.plist");
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(sprite_rect->getContentSize().width*0.3,sprite_rect->getContentSize().height - countryName->getContentSize().height/4));
                        sprite_rect->addChild(countryName,30,30);
                    }
                    
                }
                break;
            }
            case 2://装备卡牌
            {
//                sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",access->getItemId())->getCString());
				SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(access->getItemId());
				
                SGMainManager::shareMain()->addHeadIconPlistByNum(equip->getIconId(), sg_systemMailContent);
				sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",equip->getIconId())->getCString());//12.06
                //CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("lankuang.png");
                CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("equipPinzhi_1.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                sprite->addChild(spriteRect);
                
                break;
            }
            case 3://士兵卡牌
            {
                SGSoldierDataModel *soldier = SGStaticDataManager::shareStatic()->getSoliderById(access->getItemId());
                if(soldier)
                {
                    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_systemMailContent);
                    sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%d-1.png",access->getItemId())->getCString());
                    CCSprite *spriteRect = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",soldier->getSoldierRace())->getCString());
                    spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                    sprite->addChild(spriteRect);
                }
                
                break;
            }
            case 4://道具卡牌
            {
				//12.06
                
                
				SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(access->getItemId());
                SGMainManager::shareMain()->addHeadIconPlistByNum(prop->getIconId() , sg_systemMailContent);
                sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",prop->getIconId())->getCString());
				CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("jinkuang.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                sprite->addChild(spriteRect);
                
                break;
            }
            case 5://加铜钱
            {
                SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
                sprite = CCSprite::createWithSpriteFrameName("head9999.png");
                CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("jinkuang.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                SGCCLabelTTF *labelCount = SGCCLabelTTF::create(CCString::createWithFormat("X%d",access->getCount())->getCString(), FONT_PANGWA, FONTSIZE(8));
                sprite->addChild(labelCount, 5);
                labelCount->setPosition(ccp(sprite->getContentSize().width/2, labelCount->getContentSize().height));
                sprite->addChild(spriteRect);
                break;
            }
            case 6://金子
            {
                sprite = CCSprite::createWithSpriteFrameName("big_yb.png");
                CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("jinkuang.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                SGCCLabelTTF *labelCount = SGCCLabelTTF::create(CCString::createWithFormat("X%d",access->getCount())->getCString(), FONT_PANGWA, FONTSIZE(8));
                sprite->addChild(labelCount, 5);
                labelCount->setPosition(ccp(sprite->getContentSize().width/2, labelCount->getContentSize().height));
                sprite->addChild(spriteRect);
                break;
            }
            case 7://军功
            {
                SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
                sprite = CCSprite::createWithSpriteFrameName("head9997.png");
                CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("jinkuang.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                SGCCLabelTTF *labelCount = SGCCLabelTTF::create(CCString::createWithFormat("X%d",access->getCount())->getCString(), FONT_PANGWA, FONTSIZE(8));
                sprite->addChild(labelCount, 5);
                labelCount->setPosition(ccp(sprite->getContentSize().width/2, labelCount->getContentSize().height));
                sprite->addChild(spriteRect);
                break;
            }
            case 8://炼丹秘方
            {
                SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
                sprite = CCSprite::createWithSpriteFrameName("head9996.png");
                CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("jinkuang.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                SGCCLabelTTF *labelCount = SGCCLabelTTF::create(CCString::createWithFormat("X%d",access->getCount())->getCString(), FONT_PANGWA, FONTSIZE(8));
                sprite->addChild(labelCount, 5);
                labelCount->setPosition(ccp(sprite->getContentSize().width/2, labelCount->getContentSize().height));
                sprite->addChild(spriteRect);
                break;
            }
            case 9://小喇叭
            {
                SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_systemMailContent);
                sprite = CCSprite::createWithSpriteFrameName("head9998.png");
                CCSprite *spriteRect = CCSprite::createWithSpriteFrameName("jinkuang.png");
                spriteRect->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
                SGCCLabelTTF *labelCount = SGCCLabelTTF::create(CCString::createWithFormat("X%d",access->getCount())->getCString(), FONT_PANGWA, FONTSIZE(8));
                sprite->addChild(labelCount, 5);
                labelCount->setPosition(ccp(sprite->getContentSize().width/2, labelCount->getContentSize().height));
                sprite->addChild(spriteRect);
                break;
            }
        }
        
        arraySprite->addObject(sprite);
    }
    
    return arraySprite;
}
*/
#pragma mark - ------ 按钮点击 -----

void SGSystemMailContent::buttonClickAccessory()
{
//    SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, m_pSystemMail->getAccessoryArray());
//    SGMainManager::shareMain()->showBox(rewardBox);
//    return;
    
    if(!m_bIsReceived)
    {
        SG_SHOW_WINDOW(str_FriendMailLayer_str16);
        return;
    }
    
     EFFECT_PLAY(MUSIC_BTN);
    main::ReceiveAttachmentRequest *request = new main::ReceiveAttachmentRequest();
    request->set_mailid(m_pSystemMail->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_MAIL_GETACCOSSORY, request);
}

/*
void SGSystemMailContent::buttonClickDelete()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::DeleteSystemMailRequest *del = new main::DeleteSystemMailRequest();
    del->set_mailid(m_pSystemMail->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_MAIL_DELETESYSTEMMAIL, del);
    
    SGMainManager::shareMain()->getMainLayer()->setMailSelectId(m_pSystemMail->getSsid());
}
*/
void SGSystemMailContent::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    backToLastLayer();
}

//返回上层菜单
void SGSystemMailContent::backToLastLayer()
{
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getSystemMails();
    SGSystemMailLayer *layer = SGSystemMailLayer::create(array);
    SGMainManager::shareMain()->showLayer(layer);
}

#pragma mark - ----- 监听消息 ------

void SGSystemMailContent::readMailListener(cocos2d::CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("readMailListener !!!");
        return;
    }
    
    main::ShowSystemMailResponse *response = (main::ShowSystemMailResponse*)request->m_msg;
    if(1 == response->state())
    {
        m_pSystemMail->setIsRead(1);
    }else{
        SG_SHOW_WINDOW(str_FriendMailLayer_str17);
    }
    
}

/*void SGSystemMailContent::deleteMailListener(cocos2d::CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("deleteMailListener !!! ");
        return;
    }
    
    main::DeleteSystemMailResponse *response = (main::DeleteSystemMailResponse*)request->m_msg;
    int state = response->state();
    if(state == 1)
    {
        
        //删除本封邮件
        SGPlayerInfo::sharePlayerInfo()->getSystemMails()->removeObject(m_pSystemMail);
        
        //显示上层界面
        backToLastLayer();
    }else{

    }
    
}
*/
void SGSystemMailContent::receiveAccessoryListener(cocos2d::CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("receiveAccessoryListener !!! ");
        return;
    }
    
    main::ReceiveAttachmentResponse *response = (main::ReceiveAttachmentResponse*)request->m_msg;
    if(1 == response->state())
    {
        //收取附件
//        int attCount = response->attachmentproto_size();
//        for(int i=0;i<attCount;i++)
//        {
//            main::AttachmentProto att = response->attachmentproto(i);
//            SGMailManager::getAccessoryByProto(att);
//        }
        
        SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, m_pSystemMail->getAccessoryArray());
        SGMainManager::shareMain()->showBox(rewardBox);
        
        //设为为已收取状态
        m_pSystemMail->setIsReceive(1);
        
        //更改显示
        m_bIsReceived = !m_bIsReceived;
        showHeadIconlayer->removeFromParentAndCleanup(true);
        CCString *labelAccessoryStr = CCString::createWithFormat(str_FriendMailLayer_str18);
        labelAccessory->setString(labelAccessoryStr->getCString());
//        receiveAccessory();
        
        
    }else{
        SG_SHOW_WINDOW(str_FriendMailLayer_str19);
    }
    
}

