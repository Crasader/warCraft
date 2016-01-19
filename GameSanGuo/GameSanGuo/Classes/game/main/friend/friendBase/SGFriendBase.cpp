//
//  SGFriendBase.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-7.
//
//

#include "SGFriendBase.h"
#include "SGFriendCellBase.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "FriendModule.pb.h"
#include "SGOtherPlayerInfo.h"
#include "SGFriendCountFull.h"
#include "ResourceManager.h"
#include "SGRankDetailLayer.h"
#include "SGStringConfig.h"

int SGFriendBase::addRoleId = 0;

SGFriendBase::SGFriendBase(): buttonBack(NULL) , labelCenterTip(NULL)
{
    spriteTitle = NULL;
    labelOnline = NULL;
    _touch = true;
    _selectRoleId = 0;
 
}

SGFriendBase::~SGFriendBase()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_friendLayer);
	
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
}

bool SGFriendBase::init(const char *fileName_, LayerTag tag_, bool isSpriteFrame)
{
    if(!SGBaseTableLayer::init(fileName_, tag_,isSpriteFrame))
    {
        return false;
    }        
    this->constructView();
    return true;
}

void SGFriendBase::constructView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGFriendBase::showPlayerInfoListener));
    
    /************************页面布局********************************/
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
 	
	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendLayer);

    
    //标题
    spriteTitle =  CCSprite::createWithSpriteFrameName("friend_friendRequest.png");
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -SGMainManager::shareMain()->getTotleHdHgt()+65/2)));
    this->addChild(spriteTitle);
    
    //标题旁边的提示文字
    labelOnline = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
    labelOnline->setAnchorPoint(ccp(0, 0.5));
    labelOnline->setPosition(ccpAdd(spriteTitle->getPosition(), ccp(spriteTitle->getContentSize().width*.7, 0)));
    addChild(labelOnline);
    
    //五张图片的尺寸  47 150 72 56 101
//    buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png", this, menu_selector(SGFriendBase::buttonClickGoBack),CCPointZero,false,true);
	buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGFriendBase::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    buttonBack->setPosition(ccpAdd(spriteTitle->getPosition(), ccp(-skewing(160),0)));
    addBtn(buttonBack);
    
    //tableview
    this->tableView->setFrame(CCRectMake(0,101+56, SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-SGMainManager::shareMain()->getTotleHdHgt() - SGMainManager::shareMain()->getBottomHeight()));
    
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    this->tableView->setShowSlider(false);
    this->tableView->setZOrder(1);
    this->tableViewHeight = 130;
    tableView->setDown(-18);
    
   // tableView -> setPageNEnable(true);
    
    //提示没有好友，没有黑名单等等
    labelCenterTip = SGCCLabelTTF::create(str_FriendBase_str1, FONT_BOXINFO, 30);
    labelCenterTip->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter),ccp(0, -44)));
    addChild(labelCenterTip, 1000);
    labelCenterTip->setVisible(false);
    
}

//显示没有好友
void SGFriendBase::shownoFriendTip(const char *tips)
{
    labelCenterTip->setString(tips);
    labelCenterTip->setVisible(0 == datas->count());
}

void SGFriendBase::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGFriend *friends = SGFriend::create();
    SGMainManager::shareMain()->showLayer(friends);
}

void SGFriendBase::addFriendListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest*)obj;
    if(!req)
    {
        CCLOG("AddFriend Error");
        return;
    }
    
    main::AddFriendResponse *response = (main::AddFriendResponse*)req->m_msg;
    
    //0; 异常失败
    //1; 自己的好友列表满了
    //2; 对方的好友满了
    //3; 已经在你的好友列表中
    //4; 已经在你的黑名单中,解除黑名单并申请
    //5; 已经在他的黑名单中
    //6; 已经申请过了,在他的申请列表中
    //7; 正常添加
    
    switch(response->state())
    {
        case 0:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str2);
            break;
        }
        case 1:
        {
            //弹窗提示
            SGFriendCountFull *full = SGFriendCountFull::create(this);
            SGMainManager::shareMain()->showBox(full);

            break;
        }
        case 2:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str3);
            break;
        }
        case 3:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str4);
            break;
        }
        case 5:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str5);
            break;
        }
        case 6:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str6);
            break;
        }
        case 4:
        {
            SG_SHOW_WINDOW(str_FriendBase_str7);
            
            //删除黑名单的数据，并申请
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getBlackList();
            CCObject *object = NULL;
            CCARRAY_FOREACH(array, object)
            {
                SGGamePlayer *player = (SGGamePlayer*)object;
                if(player->getFriendID() == SGFriendBase::addRoleId)
                {
                    array->removeObject(player);
                }
            }
            
            break;
        }
        case 7:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str8);
            break;
        }
    }
    
    
     
}

void SGFriendBase::showPlayerInfoListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest*)obj;
    if(!req)
    {
        CCLOG("Show Player Information ");
        return;
    }
    
    
    main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)req->m_msg;
    SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
  
//    
//    SGOtherPlayerInfo *other = SGOtherPlayerInfo::create(playerInfo,this);
//    SGMainManager::shareMain()->showBox(other);
    
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank));
    {
        SGMainManager::shareMain()->getMainScene()->removeChildByTag(box_rank);
    }
    SGRankDetailLayer*box=SGRankDetailLayer::create(playerInfo,1);
    SGMainManager::shareMain()->showBox(box);
    /////////////////////////存储排行类型  国家  人物id
//    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//    player->setrankType(typetype);
//    player->setplayerNation(nation);
//    player->setplayerID(playid);
//    
    /////////////////////////

}

void SGFriendBase::agreeFriendListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("agreeFriendListener");
        return;
    }
    
    main::AgreeFriendResponse *agree = (main::AgreeFriendResponse*)request->m_msg;
    
    //0; 异常失败
    //1; 不在自己的申请列表里
    //2; 自己的好友列表满了
    //3; 对方的好友满了
    //4; 已经在对方申请列表中，清除对方申请并添加好友
    //5; 正常添加
    
    switch(agree->state())
    {
        case 0:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str9);
            break;
        }
        case 1:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str10);
            break;
        }
        case 2:
        {
            SGFriendCountFull *full = SGFriendCountFull::create(this);
            SGMainManager::shareMain()->showBox(full);
            break;
        }
        case 3:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str3);
            break;
        }
        case 4:
        case 5:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str11);
            this->refreshData(_selectRoleId);
            break;
        }
    }
    shownoFriendTip(str_FriendCountFull_str3);
}

void SGFriendBase::refuseAddFriendListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("refuseAddFriendListener");
        return;
    }
    
    main::UnagreeFriendResponse *response = (main::UnagreeFriendResponse*)request->m_msg;
    if(0 == response->state())
    {
        SG_SHOW_WINDOW(str_FriendBase_str12);
    }
    else if(1 == response->state())
    {
        SG_SHOW_WINDOW(str_FriendBase_str13);
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getApplyList();
        SGGamePlayer *player = NULL;
        for (int ii = 0; ii < array->count(); ii++) {
            player = (SGGamePlayer *)array->objectAtIndex(ii);
            if (player->getFriendID() == _selectRoleId) break;
        }
        if (player != NULL) {
            SGPlayerInfo::sharePlayerInfo()->getApplyList()->removeObject(player);
            this->datas = SGPlayerInfo::sharePlayerInfo()->getApplyList();
        }
        this->tableView->reloadData();
    }
    shownoFriendTip(str_FriendCountFull_str3);
}

void SGFriendBase::refreshData(int roleId)
{
}
void SGFriendBase::setIsCanTouch(bool isTouch)
{
    _touch = isTouch;
    
    SGBaseTableLayer::setIsCanTouch(isTouch);
    tableView->reloadData();
     shownoFriendTip(str_FriendCountFull_str3);
}