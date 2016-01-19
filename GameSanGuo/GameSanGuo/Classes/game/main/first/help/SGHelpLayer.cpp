//
//  SGHelpLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-7.
//
//

#include "SGHelpLayer.h"
#include "SGMainManager.h"
#include "SGBarracksLayer.h"
#include "SGPlayerInfo.h"
#include "SGRegisterBox.h"
#include "SGServerListLayer.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "Itemmodule.pb.h"
#include "SGHaveCards.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGShareBox.h"
#include "SGStringConfig.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#include "AndroidSDKAdp.h"
#endif
//#import  "SdkReqCallBack.h"
#include "SGComplainLayer.h"
#include "ExtClass.h"
#include "PlatformAdp.h"
#include "GlobalConfig.h"
#include "SGFriend.h"

//MMD: SDK 台灣版需要添加fb粉絲團的鏈接。
static bool isTaiWanGo2Play = false;

//系统条目类型的区分。
enum SGSystemFuncType
{
    SGSFT_GALLERY = 0, //图鉴
    SGSFT_FRIEND, //好友（新增移入）
    SGSFT_SETTING, //设置
    SGSFT_HELP , //帮助
    SGSFT_ADVICE, //建议
    SGSFT_SHARE, //分享，社区（动态）
    SGSFT_FANS, //粉丝团（动态）
    SGSFT_RELOGIN, //切换服务器（废弃）
};


SGHelpLayer::SGHelpLayer():itemNum(0)
{
    SGMainLayer *mainLayer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }

}

SGHelpLayer::~SGHelpLayer()
{

	ResourceManager::sharedInstance()->unBindLayerTexture(sg_helpLayer);
	 
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_ALLCARDS_INFO);
}

SGHelpLayer *SGHelpLayer::create()
{
    SGHelpLayer *helpLayer = new SGHelpLayer();
    if (helpLayer && helpLayer->init(NULL, sg_helpLayer))
    {
        helpLayer->initView();
        helpLayer->autorelease();
        return helpLayer;
    }
    CC_SAFE_DELETE(helpLayer);
    return NULL;
}

void SGHelpLayer::initView()
{
    //台湾的各个版本要添加fb粉丝团的直达通道。
    isTaiWanGo2Play = (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE) ? true : false;
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_ALLCARDS_INFO,
                              this,
                              callfuncO_selector(SGHelpLayer::enterCardInfo));
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();    
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    tableViewHeight = 110;
    
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
    tableView->setSelectType(TableViewSelectTypeSingleOpposite);//
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGHelpLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));

    SGCCLabelTTF *title = SGCCLabelTTF::create(str_system, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN );
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);
}

SNSTableViewCellItem *SGHelpLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    CCSize size = tableView->getItemSize();
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    LabelItem *item = (LabelItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if (NULL == item)
    {
        item = LabelItem::create(size,num,false,this,tableView);
    }
    
    //获得title指针，修改文本。
    SGCCLabelTTF* label = item->title;
    
    switch (num)
    {
        case SGSFT_GALLERY:
        {
            label->setString(str_HelpLayer_str9);
        }
            break;
        case SGSFT_FRIEND:
        {
            label->setString(str_FriendCountFull_str11);
            //好友需要判断红点显隐。
            SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
            CCArray *temp = playerInfo->getApplyList();
            if (temp->data && temp->data->arr && temp->count() != 0  && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev())
            {
                item->setRedSpot(true);
            }
            else
            {
                item->setRedSpot(false);
            }
        }
            break;
        case SGSFT_SETTING:
        {
            label->setString(str_HelpLayer_str1);
        }
            break;
        case SGSFT_HELP:
        {
            label->setString(str_HelpLayer_str2);
        }
            break;
        case SGSFT_ADVICE:
        {
            CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
            int count=0;
            if (temp)
            {
                for(int i=0;i<temp->count();i++)
                {
                    SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
                    
                    if(!qa->getState())
                    {
                        count ++;
                    }
                }
            }
            CCString*string=CCString::createWithFormat(str_HelpLayer_str3,count);
            label->setString(string->getCString());
        }
            break;
        case SGSFT_SHARE:
        {
            if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
            {
                label->setString(str_HelpLayer_str8);
            }
            else
            {
                if (GAME_ACCOUNT_TYPE == GCAT_PP)
                {
                    label->setString(str_HelpLayer_str4);
                }
                else if (GAME_ACCOUNT_TYPE == GCAT_TONGBUTUI)
                {
                    label->setString(str_HelpLayer_str5);
                }
                else//如果其他情况一旦显示出来
                {
                    label->setString(str_HelpLayer_str6);
                }
            }
            //label->setString("绑定帐号");
        }
            break;
        case SGSFT_FANS:
        {
            if (isTaiWanGo2Play) //台湾版本要添加fb粉丝团的直达通道。
            {
                label->setString(str_HelpLayer_str10);
            }
            else
            {
                label->setString(str_SkipUpdate);
            }
            break;
            //这以下是添加引流功能之前的老代码，保留待命，不执行。
            /*
            if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
            {
                if (GAME_ACCOUNT_TYPE == GCAT_PP)
                {
                    label->setString(str_HelpLayer_str4);
                }
                else if (GAME_ACCOUNT_TYPE == GCAT_TONGBUTUI)
                {
                    label->setString(str_HelpLayer_str5);
                }
                else//如果其他情况一旦显示出来
                {
                    label->setString(str_HelpLayer_str6);
                }
            }
            else
            {
                label->setString(str_HelpLayer_str7);
                
            }
            */
        }
            break;
        case SGSFT_RELOGIN:
        {
            label->setString(str_HelpLayer_str7);
        }
            break;
        default:
            break;
    }

    return item;
}
unsigned int  SGHelpLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	if (GAME_ACCOUNT_TYPE == GCAT_XDYOU) //目前主要是台湾使用。
	{
        if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID) //如果开启了FB分享，需要显示"分享游戏"
        {
            int fix = 0;
            if (isTaiWanGo2Play) //台湾版本要添加fb粉丝团的直达通道。
            {
                fix = 1;
            }
            itemNum = 6 + fix;
        }
        else
        {
            itemNum = 5;
        }
		
	}
	else if (GAME_ACCOUNT_TYPE == GCAT_PP ||
			 GAME_ACCOUNT_TYPE == GCAT_TONGBUTUI)
	{
        if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
        {
            itemNum = 6;
        }
        else
        {
            itemNum = 5;
        }
	}
    else //MMD: ACCOUNT uc等其他渠道在此处理，目前来看，与PP和TB的处理是一样的。
    {
        if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
        {
            itemNum = 6;
        }
        else
        {
            itemNum = 5;
        }
    }
    return itemNum;
}
void SGHelpLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{}
///////////
void SGHelpLayer::itemselect(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);

    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    
    if (true /*SGPlayerInfo::sharePlayerInfo()->getPlayerLevel() >= 10*/)
    {
        switch (index)
        {
            case SGSFT_GALLERY:
            {
                showCards();
            }
                break;
            case SGSFT_FRIEND:
            {
                showFriendFunc();
            }
                break;
            case SGSFT_SETTING:
            {
                playerSet();
            }
                break;
            case SGSFT_HELP:
            {
                help();
            }
                break;
            case SGSFT_ADVICE:
            {
                CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
                int count=temp->count();
                //                for(int i=0;i<temp->count();i++)
                //                {
                //                    SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
                //
                //                    if(!qa->getState())
                //                    {
                //                        count ++;
                //                    }
                //                }
                //
                //
                
                if(count)
                {
                    
                    advice();
                }
                
                else
                {
                    
                    SGMainManager::shareMain()->showliuyanlayer();
                }
                
                
            }
                break;
            case SGSFT_SHARE:
            {
                //                band();
                if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
                {
                    showShareBox();
                }else{
                    enterUserCommunity();
                }
            }
                break;
            case SGSFT_FANS:
            {
                if (isTaiWanGo2Play) //台湾版本要添加fb粉丝团的直达通道。
                {
                    CCLOG("open url %s", GlobalConfig::gi()->getTaiWanFbFansGroupUrl().c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    OcCaller::getInstance()->openUrl(GlobalConfig::gi()->getTaiWanFbFansGroupUrl());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    AndroidSDKAdp::getInstance()->openUrl(GlobalConfig::gi()->getTaiWanFbFansGroupUrl());
#else
                    ;
#endif
                }
                else
                {
                    //do nothing
                }
                break;
                //这以下是添加引流功能之前的老代码，保留待命，不执行。
                /*
                //                band();
                if(SGPlayerInfo::sharePlayerInfo()->getShowvip()==SHOW_SHAREBOX_ID)
                {
                    enterUserCommunity();
                }else{
                    change();
                }
                */
            }
                break;
            case SGSFT_RELOGIN:
            {
                change();
            }
                break;
            default:
                break;
        }
    }
}

//////////
void SGHelpLayer::showShareBox()
{
    SGShareBox *box = SGShareBox::create(this,SGShareTypeGeneral);
    SGMainManager::shareMain()->showBox(box);

}
void SGHelpLayer::showCards()
{
    main::CardStatesRequest *request = new main::CardStatesRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_ALLCARDS_INFO, request);
}
void SGHelpLayer::playerSet()
{
    SGMainManager::shareMain()->showSetLayer();
}
void SGHelpLayer::help()

{
    
  //  SGMainManager::shareMain()->showMessage("暂未开放");
    //暂时注释
    
    SGMainManager::shareMain()->showHelpBook();
}
void SGHelpLayer::invite()
{
    SGMainManager::shareMain()->showInviteNumLayer();
}
void SGHelpLayer::advice()
{
  //  SGMainManager::shareMain()->showMessage("暂未开放");
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();

    SGComplainLayer *layer = SGComplainLayer::create(playerInfo->getQAContent());
    SGMainManager::shareMain()->showLayer(layer);

}
void SGHelpLayer::enterCardInfo(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        CCArray *array = CCArray::create();
        main::CardStatesResponse *response = (main::CardStatesResponse *)sr->m_msg;
        int len = response->cards_size(); 
        for (int i = 0; i<len; i++)
        {
           main::CardStateMessage proto = response->cards(i);
            SGHaveCards *card = new SGHaveCards;
            card->setcardItem(proto.itemid());
            card->setcardState(proto.state());
            array->addObject(card);
            card->autorelease();
        }
        SGMainManager::shareMain()->showAllCards(array);
    }

}
void SGHelpLayer::band()
{
    SGRegisterBox *registerBox = SGRegisterBox::create(this,0);
    SGMainManager::shareMain()->showBox(registerBox);

}
//进入用户的社区中心
void SGHelpLayer::enterUserCommunity()
{
	ExtClassOfSDK::sharedSDKInstance()->enterCommunity();
}

void SGHelpLayer::change()
{
    std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
    int chanleId = CCUserDefault::sharedUserDefault()->getIntegerForKey("channelId");

    SGHttpClient::shareHttpClient()->getServerList(this, userId,chanleId);
//    int fightId = 1001;
//    short ipPort = 22335;
//    CCString *ipAddr = CCString::create("192.168.1.101");
//    SGMainManager::shareMain()->setServerInfo(ipAddr, ipPort);
//    
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("fightId", fightId);
//    
//    std::string unn = CCUserDefault::sharedUserDefault()->getStringForKey("username");
//    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
//    short sid = (short)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverId");
//    
//    SGHttpClient::shareHttpClient()->login(this, unn.c_str() , pw.c_str(), sid);
//
//    return;
//    
//    std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
//    SGHttpClient::shareHttpClient()->getServerList(this, un.c_str());

//    SGSocketClient::sharedSocketClient()->closeConn();
//    SGSocketClient::sharedSocketClient()->stopConn();
    
//    SGServerListLayer *serverListLayer = SGServerListLayer::create((CCArray *) SGPlayerInfo::sharePlayerInfo()->getServerList(),SGServerListLayerComeFromFirst);
//    SGMainManager::shareMain()->showLayer(serverListLayer);
}

void SGHelpLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
  SGMainManager::shareMain()->showFirstLayer();
}

void SGHelpLayer::requestFinished(int msgid, cocos2d::CCObject *data)
{
    if(msgid == MSg_HTTP_SERVERLIST)
    {
//        SGServerListLayer *serverListLayer = SGServerListLayer::create((CCArray *)data,this,SGServerListLayerComeFromGame);
//        SGMainManager::shareMain()->showLayer(serverListLayer);
        SGServerListLayer *box = SGServerListLayer::create(this, (CCArray *)data, SGServerListLayerComeFromFirst);
        SGMainManager::shareMain()->showBox(box);
        
    }else if (msgid == MSG_HTTP_LOGIN){
        
        const char *serverIp = SGMainManager::shareMain()->getServerIp()->getCString();
        short serverPost = SGMainManager::shareMain()->getServerPost();
        CCLog("login------------------%d, %s", serverPost, serverIp);
        SGSocketClient::sharedSocketClient()->startConn(serverIp, serverPost);
    }

}
void SGHelpLayer::showBoxCall(CCObject *obj)
{
    //不知道为什么需要加一个，返回的已经有了呀
    SGMainLayer *mainLayer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *key =  (CCString*)dict->objectForKey("key");
        
        if(key->isEqual(CCString::create("serverList")))
        {
            CCString *serverName = (CCString*)dict->objectForKey("serverName");
            CCString *serverIp = ((CCString*)dict->objectForKey("serverIp"));
            CCString *serverId = (CCString*)dict->objectForKey("serverId");
            CCString *serverPost = (CCString*)dict->objectForKey("serverPost");
            
            CCLOG("选中服务器名字：  %s",serverName->getCString());
            CCLOG("选中服务器Ip：   %s",serverIp->getCString());
            CCLOG("选中服务器Post： %s",serverPost->getCString());
            CCLOG("选中服务器Id：   %s",serverId->getCString());
            
            
            CCUserDefault::sharedUserDefault()->setStringForKey("serverip", serverIp->getCString());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverpost", (short)serverPost->intValue());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverid", (short)serverId->intValue());
            CCUserDefault::sharedUserDefault()->setStringForKey("servername", serverName->getCString());
            CCUserDefault::sharedUserDefault()->flush();
            
            
            std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
            
            SGMainManager::shareMain()->setServerInfo(serverIp , (short)serverPost->intValue());
            //SGHttpClient::shareHttpClient()->login(this, accountId, sid);
			
			//OC调用改到外围
//#if (PLATFORM == IOS)
            SGMainManager::shareMain()->dealIosLogin(un,pw);
//			//OcCaller::getInstance()->pyUserSDKLoginReq(un.c_str(), pw.c_str());
//#else
//			//android code
//			SdkManager::shareSdkManager()->userLogin(un.c_str(), pw.c_str());
//#endif
			/*
            id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
            [[PYUserSDKLib sharedUserSDKLib] userLogin:[[NSString alloc] initWithUTF8String:un.c_str()]
                                              Password:[[NSString alloc] initWithUTF8String:pw.c_str()]
                                              GameId:@"1"
                                              ChannelId:[sdkReqCallBack ChannelId]
                                              SubChannelId:[sdkReqCallBack SubChannelId]
                                              OsVersion:[sdkReqCallBack OsVersion]
                                              OsType:[sdkReqCallBack OsType]
                                      CallBackDelegate:sdkReqCallBack FinishSelector:@selector(loginFinished:) FailSelector:@selector(requestFailed:)];
			 */
            
        }
    }
}
void SGHelpLayer::requestFailed()
{
    CCLOG("error .... ");
}

void SGHelpLayer::setIsCanTouch(bool isTouch)
{
    SGBaseTableLayer::setIsCanTouch(isTouch);
    tableView->setIsTouchEnabled(isTouch);
 
    LabelItem *item = NULL;
    for (int i=0; i<itemNum; i++)
    {
        item = (LabelItem*)tableView->getItemByIndexPath(SNSIndexPath::create(i, 1));
        if (item && item->menu)
        {
            item->menu->setEnabled(isTouch);
        }
    }
    

}


//进入好友功能。
void SGHelpLayer::showFriendFunc()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGFriend *friends = SGFriend::create();
    SGMainManager::shareMain()->showLayer(friends);
    SNSTableView::showScrollItemMultiMoveFromRight(friends->getSNSTavleView());//1126效果优化
}
