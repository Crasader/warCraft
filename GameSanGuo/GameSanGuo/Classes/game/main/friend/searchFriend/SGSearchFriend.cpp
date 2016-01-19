//
//  SGSearchFriend.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGSearchFriend.h"
#include "SGFriend.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSearchFriendFinish.h"
#include "SGCCLabelTTF.h"
#include "ResourceManager.h"
#include "SGNikeNameBox.h"
#include "SGStringConfig.h"
SGSearchFriend::SGSearchFriend()
{
    _isFriend = true;
    editID = NULL;
    editNikeName = NULL;
    _nickName = NULL;
}

SGSearchFriend::~SGSearchFriend()
{
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_searchFriend);
    _nickName->release();
    
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_SEARCHBYID);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_SEARCHBYNAME);
    
}

SGSearchFriend* SGSearchFriend::create(bool isFriend)
{
    SGSearchFriend *search = new SGSearchFriend();
    search->_isFriend = isFriend;
    if(search && search->init(NULL, sg_searchFriend))
    {
        
        search->constructView();
        search->autorelease();
        return search;
    }
    
    CC_SAFE_DELETE(search);
    return NULL;
}

void SGSearchFriend::constructView()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_FRIEND_SEARCHBYNAME, this, callfuncO_selector(SGSearchFriend::searchFriendByNikeNameListener));
    notification->addObserver(MSG_FRIEND_SEARCHBYID, this, callfuncO_selector(SGSearchFriend::searchFriendByIdListener));
    
    _nickName = CCString::create("");
    _nickName->retain();
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
//    float h = SGMainManager::shareMain()->getTotleHdHgt();
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    //    float h = SGMainManager::shareMain()->getTotleHdHgt();
    //    float b = SGMainManager::shareMain()->getBottomHeight();
    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI, sg_searchFriend);
	ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_searchFriend);
    
	ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_searchFriend);
	ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_searchFriend);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_searchFriend);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_searchFriend);

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();

    SGCCLabelTTF *sprite = SGCCLabelTTF::create(str_SearchFriend_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//sprite->setColor(ccc3(0xff, 0x95, 0x0c));
    sprite->setPosition(ccpAdd(upCenter, ccp(0, -headhgt+65/2)));
    this->addChild(sprite);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);

    //返回按钮
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSearchFriend::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setPosition(ccpAdd(upLeft, ccp(0, -headhgt+65/2)));
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(buttonBack);
    
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
    
    /************************已屏蔽使用昵称搜索好友*************************/
    
    CCScale9Sprite *soldierframe = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    this->addChild(soldierframe,1);
    soldierframe->setPreferredSize(CCSize(598, 180));
    soldierframe->setPosition(ccpAdd(upCenter, ccp(0, -s.height*0.5)));
    
    CCScale9Sprite *spriteBackground1 = CCScale9Sprite::createWithSpriteFrameName("bingzhong_bg.png");
    this->addChild(spriteBackground1);
    spriteBackground1->setPreferredSize(CCSize(600, 180));
    spriteBackground1->setPosition(soldierframe->getPosition());
    CCScale9Sprite *btou2 = CCScale9Sprite::createWithSpriteFrameName("bantou.png");
    this->addChild(btou2,-1);
    btou2->setPreferredSize(CCSize(615, 21));
    btou2->setAnchorPoint(ccp(0.5, 1));
    btou2->setPosition(ccpAdd(soldierframe->getPosition(), ccp(15, -soldierframe->getContentSize().height*.5f)));
    
    
    CCSprite *infobg =  CCSprite::createWithSpriteFrameName("country_infobg.png");
    this->addChild(infobg);
    infobg->setPosition(ccpAdd(upCenter , ccp(0, -s.height*0.37)));
    //显示用户邀请码
    SGCCLabelTTF *labelRoleId = SGCCLabelTTF::create(CCString::createWithFormat(str_SearchFriend_str2, SGPlayerInfo::sharePlayerInfo()->getSearchfriendcode()->getCString())->getCString(), FONT_BOXINFO, 30);
    labelRoleId->setPosition(infobg->getPosition());
    addChild(labelRoleId);
   
    editNikeName = CCEditBox::create(CCSizeMake(500,42), CCScale9Sprite::createWithSpriteFrameName("friend_searchinput.png"));
    editNikeName->setAnchorPoint(ccp(0.5, 0.5));
    editNikeName->setPosition(ccpAdd(spriteBackground1->getPosition(),ccp(0, s.width*0.0)));
    this->addChild(editNikeName);
    
    SGCCLabelTTF *labelNike = SGCCLabelTTF::create(str_SearchFriend_str3, FONT_BOXINFO, 30);
    labelNike->setAnchorPoint(ccp(0, 1));
    labelNike->setPosition(ccpAdd(editNikeName->getPosition(), ccp(-editNikeName->getContentSize().width/2, s.height*0.06)));
    this->addChild(labelNike);
    //SGButton * SGButton::createFromLocal(const char *spFile, const char *font, CCObject*target, SEL_MenuHandler selector,CCPoint mov,const char *fontName,const ccColor3B &color,float fontSize,bool isFilx,bool isFrame)
    SGButton *buttonSearch1 =SGButton::createFromLocal("btn_fenbg.png", str_SearchFriend_str4, this, menu_selector(SGSearchFriend::searchFriendByID),CCPointZero,FONT_BOXINFO,ccWHITE, 32, true, true);
    buttonSearch1->setAnchorPoint(ccp(0.5, 1));
    buttonSearch1->setPosition(ccpAdd(editNikeName->getPosition(), ccp(0, -30)));
    buttonSearch1->setScale(1.2);
    this->addBtn(buttonSearch1);
}

void SGSearchFriend::searchFriendByID()
{
    EFFECT_PLAY(MUSIC_BTN);
    const char *str = editNikeName->getText();
    if(NULL == str)
    {
        SGMainManager::shareMain()->showMessage(str_SearchFriend_str5);
        return;
    }
	else if (strlen(str) < 2 || SGNikeNameBox::utf8StrLength((char *)str)<1)//搜索条件字数限制
	{
		SGMainManager::shareMain()->showMessage(str_SearchFriend_str6);
        return;
	}
    
    CCLOG("before name || %s", str);

    CCLOG("after name || %s", str);

    main::SearchFriendByInviteCodeRequest *id = new main::SearchFriendByInviteCodeRequest;
    id->set_invitecode(str);
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_SEARCHBYID, id);
    editNikeName->setText("");

}

void SGSearchFriend::searchFriendByNikeName()
{
    const char *str = editNikeName->getText();
    if(NULL == str)
    {
        SGMainManager::shareMain()->showMessage(str_SearchFriend_str7);
        return;
    }
    
    _nickName->setString(str);
    editNikeName->setText("");
    
    main::SearchFriendByNameRequest *name = new main::SearchFriendByNameRequest();
    name->set_nickname(str);
    name->set_pagenum(1);
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_SEARCHBYNAME, name);
    
}

void SGSearchFriend::searchFriendByIdListener(CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest*)obj;
    if(!req)
    {
        CCLOG("searchFriendByIdListener error");
        return;
    }
    
    main::SearchFriendByInviteCodeResponse *response = (main::SearchFriendByInviteCodeResponse*)req->m_msg;
    
    main::FriendProto fr = response->friend_();
    if(0 == fr.roleid()){
        SGMainManager::shareMain()->showMessage(str_SearchFriend_str8);
        return;
    }
    
    CCArray *array = CCArray::create();
    array->addObject(SGFriend::getSGGamePlayer(fr));
    SGSearchFriendFinish *finish = SGSearchFriendFinish::create(CCString::create(""),array, _isFriend);
    SGMainManager::shareMain()->showLayer(finish);
    
}

void SGSearchFriend::searchFriendByNikeNameListener(CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest*)obj;
    if(!req)
    {
        CCLOG("searchFriendByNameListener error");
        return;
    }
    
    main::SearchFriendByNameResponse *response = (main::SearchFriendByNameResponse*)req->m_msg;
    int count = response->friendlist_size();
    if(0 == count)
    {
        SGMainManager::shareMain()->showMessage(str_SearchFriend_str8);
        return;
    }
    
    CCArray *array = CCArray::create();
    for(int i=0;i<count;i++)
    {
        main::FriendProto fr = response->friendlist(i);
        array->addObject(SGFriend::getSGGamePlayer(fr));
    }
    
    SGSearchFriendFinish *finish = SGSearchFriendFinish::create(_nickName, array, _isFriend);
    SGMainManager::shareMain()->showLayer(finish);
    
}

void SGSearchFriend::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGFriend *friends = SGFriend::create();
    SGMainManager::shareMain()->showLayer(friends);
}
