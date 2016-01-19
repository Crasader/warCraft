//
//  SGSearchFriendFinish.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#include "SGSearchFriendFinish.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGSearchFriend.h"
#include "ResourceManager.h"

SGSearchFriendFinish::SGSearchFriendFinish():buttonAddBlack(NULL), buttonAddFriend(NULL)
{
    _arraySearch = NULL;
    _isFriend = true;
    _pageCount = 2;
    _canSearch = true;
    _nickName = NULL;
}

SGSearchFriendFinish::~SGSearchFriendFinish()
{

    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_searchFriendFinish);
    
    _arraySearch->release();
    _nickName->release();
    
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_SEARCHBYNAME);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_ADDFRIEND);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this,
                                                                     MSG_FRIEND_ADDBLACK);
    
}

SGSearchFriendFinish* SGSearchFriendFinish::create(CCString *nickName,CCArray *arraySearch, bool isFriend)
{
    SGSearchFriendFinish *search = new SGSearchFriendFinish();
    search->_isFriend = isFriend;
    search->_arraySearch = arraySearch;
    search->_arraySearch->retain();
    search->_nickName = nickName;
    search->_nickName->retain();
    
    if(search && search->init(NULL, sg_searchFriendFinish))
    {
        
        search->constructView();
        search->autorelease();
        return search;
    }
    
    CC_SAFE_DELETE(search);
    return NULL;
}

bool SGSearchFriendFinish::init(const char *fileName_, LayerTag tag_, bool isSpriteFrame)
{
    if(!SGFriendBase::init(fileName_, tag_, isSpriteFrame))
    {
        return false;
    }
    
    return true;
}


void SGSearchFriendFinish::constructView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_SEARCHBYNAME, this, callfuncO_selector(SGSearchFriendFinish::searchFriendByPageListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_ADDFRIEND, this, callfuncO_selector(SGSearchFriendFinish::addFriendListener));

    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI, sg_searchFriendFinish);
    

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
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
    
    this->datas = _arraySearch;
    
    labelOnline->setString(CCString::createWithFormat(" %d个",_arraySearch->count())->getCString());
    
    buttonAddFriend = SGButton::create("chatbtn.png", "friend_model_friend.png", this, menu_selector(SGSearchFriendFinish::buttonClickAddFriend),CCPointZero,false,true);
    buttonAddBlack = SGButton::create("chatbtn.png", "friend_model_black.png", this, menu_selector(SGSearchFriendFinish::buttonClickAddBlack),CCPointZero,false,true);
    buttonAddFriend->setAnchorPoint(ccp(1, 0.5));
    buttonAddBlack->setAnchorPoint(ccp(1, 0.5));
    buttonAddFriend->setPosition(ccp(SGLayout::getPoint(KMiddleRight).x, buttonBack->getPosition().y));
    buttonAddBlack->setPosition(buttonAddFriend->getPosition());
    this->addBtn(buttonAddFriend);
    this->addBtn(buttonAddBlack);
    labelOnline->setVisible(false);
    buttonAddBlack->setVisible(_isFriend);
    buttonAddFriend->setVisible(!_isFriend);
    
    //我的好友
    CCSprite *sprite = CCSprite::createWithSpriteFrameName("friend_searchResult_title.png");
    spriteTitle->setDisplayFrame(sprite->displayFrame());
    
}

SNSTableViewCellItem* SGSearchFriendFinish::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGSearchFriendCell *cell = (SGSearchFriendCell*)this->tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if(NULL == cell)
    {
        cell = SGSearchFriendCell::create((SGGamePlayer*)_arraySearch->objectAtIndex(indexPath->getRow()),_isFriend,this);
    }else{
        cell->update((SGGamePlayer*)_arraySearch->objectAtIndex(indexPath->getRow()),_isFriend);
    }
    
    //继续加载
    if(indexPath->getRow() == datas->count()-1 && this->getIsCanPage())
    {
        main::SearchFriendByNameRequest *search = new main::SearchFriendByNameRequest();
        search->set_nickname(_nickName->getCString());
        search->set_pagenum(_pageCount);
        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_SEARCHBYNAME, search);
        _pageCount ++;
    }
    
    return cell;
}

//网络监听
void SGSearchFriendFinish::searchFriendByPageListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("searchFriendByPageListener error");
        return;
    }
    
    main::SearchFriendByNameResponse *response = (main::SearchFriendByNameResponse*)request->m_msg;
    
    int friendCount = response->friendlist_size();
    for(int i=0;i<friendCount;i++)
    {
        _arraySearch->addObject(SGFriend::getSGGamePlayer(response->friendlist(i)));
    }
    
    this->tableView->reloadData();
    
}

//加黑模式
void SGSearchFriendFinish::buttonClickAddBlack()
{
    EFFECT_PLAY(MUSIC_BTN);
    buttonAddFriend->setVisible(true);
    buttonAddBlack->setVisible(false);
    _isFriend = false;
    this->tableView->reloadData();
    
}

//加友模式
void SGSearchFriendFinish::buttonClickAddFriend()
{
    EFFECT_PLAY(MUSIC_BTN);
    buttonAddFriend->setVisible(false);
    buttonAddBlack->setVisible(true);
    _isFriend = true;
    this->tableView->reloadData();
}

//回调函数
void SGSearchFriendFinish::cellCallBack(SGGamePlayer *player , bool type)
{
    
    SGGamePlayer *newPlayer = new SGGamePlayer();
    newPlayer->setFriendID(player->getRoleID());
    newPlayer->setRoleID(SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
    newPlayer->setFriendName(player->getFriendName());
    newPlayer->setImage(player->getImage());
    newPlayer->setIsOnline(player->getIsOnline());
    newPlayer->setLevel(player->getLevel());
    newPlayer->setTime(player->getTime());
    newPlayer->setTypeID(player->getTypeID());
    newPlayer->autorelease();
    
    if(type)
    {
        SGPlayerInfo::sharePlayerInfo()->getFriendList()->addObject(newPlayer);
    }else
    {
        SGPlayerInfo::sharePlayerInfo()->getBlackList()->addObject(newPlayer);
    }
    
    this->tableView->reloadData();
    
}

//是否可以翻页
bool SGSearchFriendFinish::getIsCanPage()
{
    if(datas->count() < 25*_pageCount)
    {
        _canSearch = false;
    }else{
        _canSearch = true;
    }
    
    return _canSearch;
    
}

void SGSearchFriendFinish::addFriendListener(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest*)sender;
    if(!req)
    {
        CCLOG("AddFriend Error");
        return;
    }
    
    //0; 异常失败
    //1; 自己的好友列表满了
    //2; 对方的好友满了
    //3; 已经在你的好友列表中
    //4; 已经在你的黑名单中,解除黑名单并申请
    //5; 已经在他的黑名单中
    //6; 已经申请过了,在他的申请列表中
    //7; 正常添加
    
    main::AddFriendResponse *response = (main::AddFriendResponse*)req->m_msg;
    if (response->state()==7) {
        SGSearchFriend *search = SGSearchFriend::create(true);
        SGMainManager::shareMain()->showLayer(search);
    }
}







