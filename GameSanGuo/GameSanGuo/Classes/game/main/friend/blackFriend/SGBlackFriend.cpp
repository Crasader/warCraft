//
//  SGBlackFriend.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGBlackFriend.h"
#include "SGMainManager.h"
#include "SGBlackFriendCell.h"
#include "SGFriend.h"
#include "SGSearchFriend.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGBlackFriend::SGBlackFriend()
{
    _arrayBlackFriend = NULL;
    buttonSearch = NULL;
}

SGBlackFriend::~SGBlackFriend()
{
   
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_blackFriend);
    
    _arrayBlackFriend->release();
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_DELETEBLACK);
}

SGBlackFriend* SGBlackFriend::create(CCArray *arrayBlackFriend)
{
    SGBlackFriend *myFriend = new SGBlackFriend();
    myFriend->_arrayBlackFriend = arrayBlackFriend;
    myFriend->_arrayBlackFriend->retain();
    
    if(myFriend && myFriend->init(NULL, sg_blackFriend))
    {
        myFriend->constructView();
        myFriend->autorelease();
        return myFriend;
    }
    
    CC_SAFE_DELETE(myFriend);
    return NULL;
}

void SGBlackFriend::constructView()
{
    this->datas = _arrayBlackFriend;
    shownoFriendTip(str_BlackFriend_str1);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_blackFriend);

    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_blackFriend);
    
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_blackFriend);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_blackFriend);

    
    
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_DELETEBLACK, this, callfuncO_selector(SGBlackFriend::deleteBlackListener));
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    float h = SGMainManager::shareMain()->getTotleHdHgt();

    //我的好友
//    CCSprite *sprite = CCSprite::createWithSpriteFrameName("friend_black_title.png");
//    spriteTitle->setDisplayFrame(sprite->displayFrame());
    spriteTitle->setVisible(false);
    SGCCLabelTTF *sprite = SGCCLabelTTF::create(str_BlackFriend_str2, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//sprite->setColor(ccc3(0xff, 0x95, 0x0c));
    sprite->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(sprite);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    
    //搜索加黑
    buttonSearch = SGButton::create("chatbtn.png", "friend_search_black.png", this, menu_selector(SGBlackFriend::buttonClickSearchBlack),CCPointZero,false,true);
    buttonSearch->setAnchorPoint(ccp(1, 0.5));
    buttonSearch->setPosition(ccp(SGLayout::getPoint(KMiddleRight).x,buttonBack->getPosition().y));
    addBtn(buttonSearch);
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *titlecenter =  CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r =  CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    
}

//获取在线人数及总人数
CCString* SGBlackFriend::getPeopleCountOnline()
{
    CCString *strPeopleCount = CCString::createWithFormat(str_BlackFriend_str3,_arrayBlackFriend->count(),_arrayBlackFriend->count());
    return strPeopleCount;
}

SNSTableViewCellItem* SGBlackFriend::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGBlackFriendCell *cell = (SGBlackFriendCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if(NULL == cell)
    {
        cell = SGBlackFriendCell::create((SGGamePlayer*)_arrayBlackFriend->objectAtIndex(indexPath->getRow()),this);
    }
    cell->setParentLayer(this);
    return cell;
}

//搜索加黑
void SGBlackFriend::buttonClickSearchBlack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGSearchFriend *search = SGSearchFriend::create(false);
    SGMainManager::shareMain()->showLayer(search);
}

void SGBlackFriend::cellCallBack(SGGamePlayer *player , bool type)
{
    SGPlayerInfo::sharePlayerInfo()->getBlackList()->removeObject(player);
    this->datas = SGPlayerInfo::sharePlayerInfo()->getBlackList();
    this->tableView->reloadData();
    shownoFriendTip(str_BlackFriend_str1);
    
}

void SGBlackFriend::refreshData(int roleId)
{
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getBlackList();
    SGGamePlayer *player = NULL;
    for (int ii = 0; ii < array->count(); ii++) {
        player = (SGGamePlayer *)array->objectAtIndex(ii);
        if (player->getFriendID() == roleId) break;
    }
    if (player != NULL) {
        SGPlayerInfo::sharePlayerInfo()->getBlackList()->removeObject(player);
        this->datas = SGPlayerInfo::sharePlayerInfo()->getBlackList();
    }
    this->tableView->reloadData();
    shownoFriendTip(str_BlackFriend_str1);
}

void SGBlackFriend::deleteBlackListener(cocos2d::CCObject *object)
{
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("deleteBlackListener");
        return;
    }
    
    main::DeleteBlockResponse *response = (main::DeleteBlockResponse*)request->m_msg;
    if(response->state() == 0)
    {
        SG_SHOW_WINDOW(str_BlackFriend_str4);
    }else if(response->state() == 1)
    {
        this->refreshData(_selectRoleId);
    }
}

void SGBlackFriend::updateData()
{
    tableView->reloadData();
    shownoFriendTip(str_BlackFriend_str1);
}

