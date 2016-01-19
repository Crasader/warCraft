//
//  SGBattleCallFriendBox.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-5-8.
//
//

#include "SGBattleCallFriendBox.h"
#include "SGStaticDataManager.h"
#include "SGStoryRewardBox.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGBattleManager.h"

SGBattleCallFriendBox::SGBattleCallFriendBox():m_bCanUp(false)
,m_nFriendHeadId(0)
,m_nFriendLevel(0)
{
  
}

SGBattleCallFriendBox::~SGBattleCallFriendBox()
{
    CCLOG("~SGBattleCallFriendBox()");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_battleCallFriend);
    
}

SGBattleCallFriendBox* SGBattleCallFriendBox::create(SGBoxDelegate *delegate,int friendHeadId,int friendLevel,int canUp)
{
    SGBattleCallFriendBox *call = new SGBattleCallFriendBox();
    if(call && call->init(delegate, box_battleCallFriend, CCSizeMake(535, 530), false, true, CCSizeMake(535, 175)))
    {
        
        call->m_nFriendHeadId = friendHeadId;
        call->m_nFriendLevel = friendLevel;
        call->m_bCanUp = canUp;
        call->viewDidLoad();
        
        call->autorelease();
        return call;
    }
    
    CC_SAFE_DELETE(call);
    return call;
}

void SGBattleCallFriendBox::viewDidLoad()
{
    //弹窗显示，设置herolayer上的按钮不可触
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(false);

    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_box_battleCallFriend);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_box_battleCallFriend, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_box_battleCallFriend);

    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    //标题
    SGCCLabelTTF* spriteTitle = SGCCLabelTTF::create(str_Call_friend_battale, FONT_PANGWA, 36 , ccWHITE , ccc3(143,87,0));
    //modify by:zyc  merge into create.
    //spriteTitle->setOutSideColor(ccc3(143,87,0));
    spriteTitle->setPosition(ccpAdd(biao->getPosition(), ccp(0, 6)));
    addChild(spriteTitle);
    
    fontbg = CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    this->addChild(fontbg, 0);
    fontbg->setScaleX((this->boxSize.width-55)/fontbg->getContentSize().width);
    fontbg->setScaleY(this->boxSize.height*.42f/fontbg->getContentSize().height);
    fontbg->setPosition(ccpAdd(center, ccp(0, this->boxSize.height*.42f*.4f)));
    
    //您之前选择的友军主将
    SGCCLabelTTF *label_1 = SGCCLabelTTF::create(str_Select_friend_main_role, FONT_BOXINFO, 28 , ccGRAY);
    //modify by:zyc. merge into create.
    //label_1->setColor(ccGRAY);
    label_1->setPosition(ccpAdd(center, ccp(25, 40)));
    addChild(label_1);
	
	int iconId = 0;
	//12.05
	if (m_nFriendHeadId > 1000 && m_nFriendHeadId < 4000)
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(m_nFriendHeadId);
		iconId = temp->getIconId();
	}
	else
	{
		iconId = m_nFriendHeadId;
	}
	
	//12.05
	SGMainManager::shareMain() -> addHeadIconPlistByNum(iconId,sg_box_battleCallFriend);
    CCSprite *spriteHead = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
    
    if(!spriteHead)
    {
        spriteHead = CCSprite::createWithSpriteFrameName("head1001.png");
    }
    
    SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(m_nFriendHeadId);
    if (officerData) {
        int starlvl = officerData->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        CCString *countryStr = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
        
        //    CCString *countryStr = CCString::createWithFormat("officerPinzhi_%d.png",officerData->getOfficerVisitType());
        CCSprite *spriteHeadRect = CCSprite::createWithSpriteFrameName(countryStr->getCString());
        spriteHead->setPosition(ccpAdd(center, ccp(-160, 130)));
        CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
        this->addChild(background);
        background->setPosition(spriteHead->getPosition());
        spriteHeadRect->setPosition(spriteHead->getPosition());
        addChild(spriteHead);
        addChild(spriteHeadRect);
        
        //if (str_->isEqual(CCString::create("SGOfficerCard")))
        {
            spriteHeadRect->removeChildByTag(30);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spriteHeadRect->getContentSize().width*0.35,spriteHeadRect->getContentSize().height - countryName->getContentSize().height));
            spriteHeadRect->addChild(countryName,30,30);
            
            spriteHeadRect->removeChildByTag(250);
            if (officerData->getAdvNum()>0) {
                SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",officerData->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advancenumber->setInsideColor(ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                advancenumber->ignoreAnchorPointForPosition(false);
                advancenumber->setPosition(ccp(spriteHeadRect->getContentSize().width * 0.95,spriteHeadRect->getContentSize().height));
                spriteHeadRect->addChild(advancenumber,50,250);
            }

        }
        
        float headWid = spriteHead->getContentSize().width;
        float headHid = spriteHead->getContentSize().height;
        CCString *nameStr = CCString::createWithFormat("%s  %d级", officerData->getOfficerName()->getCString(), m_nFriendLevel);
        SGCCLabelTTF *labelNames = SGCCLabelTTF::create(nameStr->getCString(), FONT_BOXINFO, 30);
        labelNames->setAnchorPoint(ccp(0, 0.5f));
        labelNames->setPosition(ccpAdd(spriteHead->getPosition(), ccp(headWid*.7f, headHid*.25f)));
        this->addChild(labelNames);
        
        CCSprite *stars = spriteWithStars(3);
        stars->setPosition(ccpAdd(spriteHead->getPosition(), ccp(headHid*.85f, -headHid*.15f)));
        this->addChild(stars);
    }else
    {
        CCString *tip = CCString::createWithFormat(str_Format_get_wujiang_data_failure_id,m_nFriendHeadId);
        SGMainManager::shareMain()->showMessage(tip->getCString());
    }
    
    
    SGCCLabelTTF *label_2 = SGCCLabelTTF::create(str_Call_friend_tips, FONT_BOXINFO, 28, CCSizeMake(this->boxSize.width*.95f, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    label_2->setAnchorPoint(ccp(0, 0.5f));
    label_2->setPosition(ccpAdd(center, ccp(-boxSize.width*.45f, -95)));
    addChild(label_2);
    
    //援军槽
    SGCCLabelTTF *label_3 = SGCCLabelTTF::create("", FONT_BOXINFO, 28);
    CCString *label_3_str = NULL;
    if(m_bCanUp)
    {
        label_3_str = CCString::create(str_Can_call_friend);
        label_3->setInsideColor(ccGREEN);
    }else{
        label_3_str = CCString::create(str_No_Can_call_friend);
        label_3->setInsideColor(ccRED);
    }
    label_3->setString(label_3_str->getCString());
    label_3->setPosition(ccpAdd(center, ccp(0, -this->boxSize.height*.4f+55)));
    addChild(label_3);
    
    //按钮 呼叫友军 取消
    CCString *okButtonImageNameStr = NULL;
    if(m_bCanUp)
    {
        okButtonImageNameStr = CCString::create("public_btn_5.png");
    }else{
        okButtonImageNameStr = CCString::create("public_btn_4.png");
    }

    
    
    SGButton *buttonCallFriend = SGButton::create(okButtonImageNameStr->getCString(),"battle_ui_friend_call.png", this, menu_selector(SGBattleCallFriendBox::buttonClickCallFriend),CCPointZero,false,true);
    buttonCallFriend->setPosition(ccpAdd(center, ccp(-120, -this->boxSize.height*.5f+50)));
    addBtn(buttonCallFriend);
    buttonCallFriend->cocos2d::CCMenuItem::setEnabled(m_bCanUp);
    
    SGButton *buttonCancel = SGButton::create("public_btn_5.png", "public_font_qixiao.png", this, menu_selector(SGBattleCallFriendBox::boxClose),CCPointZero,false,true);
    buttonCancel->setPosition(ccpAdd(center, ccp(120, -this->boxSize.height*.5f+50)));
    addBtn(buttonCancel);
    
}
void SGBattleCallFriendBox::boxClose()
{
    SGBaseBox::boxClose();
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(true);
}

void SGBattleCallFriendBox::buttonClickCallFriend(cocos2d::CCObject *object)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    //如果已经切换到对方回合，直接关闭，不响应
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        SGBaseBox::boxClose();
        return ;
    }
    
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("Friend"), "key");
    this->data = dict;
    this->boxClose();
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(true);
}


















