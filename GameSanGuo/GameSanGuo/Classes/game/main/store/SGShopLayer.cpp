//
//  SGShopLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//
#include "SGShopLayer.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGPlayerInfo.h"
#include "ShopModule.pb.h"
#include "PvpFight.pb.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "YuanBaoShopModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"
#include "SGMallLayer.h"
#include "RoleShopModule.pb.h" //新商城消息
#include "GlobalConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AndroidSDKAdp.h"
#endif

//MMD: SDK 台灣go2play的gp版本需要添加到3rd版本的引流。
static bool isTaiWanGo2PlayGooglePlay = false;

//商城的类型美剧
enum SGShopType
{
    SGST_RECHARGE = 0, //充值 0
    SGST_VIP, //VIP礼包
    SGST_LUOYANG, //洛阳鬼市
    SGST_WANDERER, //云游商人 3
    SGST_LADDER, //天梯商城
    SGST_JINGZHOU, //荆州军需处
    SGST_FAIRYLAND, //幻境商城 6
    SGST_ARENA, //乱世商人
    SGST_SPECIAL, //台湾特殊需求：第三方引流。 8
};


SGShopLayer::SGShopLayer():_vipGifts(NULL)
,vipLvl(0)
{

}
SGShopLayer::~SGShopLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_shopLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->removeObserver(this, MSG_VIPGIFT_BAGS);
//    notification->removeObserver(this, MSG_STORE_ENTER);
    notification->removeObserver(this, MSG_EXCHANGE_ENTER);
    notification->removeObserver(this, MSG_SEASONWAR_CDLIST);
	//notification->removeObserver(this, MSG_ENTRY_TODAY_MALL);
    CC_SAFE_RELEASE(_vipGifts);
    

}
SGShopLayer *SGShopLayer::create()
{
    SGShopLayer *storeLayer = new SGShopLayer();
    if (storeLayer && storeLayer->init(NULL, sg_shopLayer))
    {
        storeLayer->initView();
        storeLayer->autorelease();
        return storeLayer;
    }
    CC_SAFE_DELETE(storeLayer);
    return NULL;
}
void SGShopLayer::initView()
{
    //台灣go2play的gp版本需要添加到3rd版本的引流。
    isTaiWanGo2PlayGooglePlay = (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY) ? true : false;

    
    _vipGifts = CCArray::create();
    _vipGifts->retain();
	
 
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_shopLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EXCHANGE_ENTER,
                              this,
                              callfuncO_selector(SGShopLayer::exchangeEnterListener));
    notification->addObserver(MSG_SEASONWAR_CDLIST, this, callfuncO_selector(SGShopLayer::showChgScoreLayer));
	
    tableViewHeight = skewing(55);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    nGuiShopingVipLimit = 0;//鬼市vip限制等级
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("75");
    nGuiShopingVipLimit = dic1->valueForKey("value")->intValue();
    
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
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    
    SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str6, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    this->addChild(spriteTitle);
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height- btmhgt - headhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, btmhgt)));
    tableView->setDown(-25);

}

SNSTableViewCellItem *SGShopLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGShopItem *item = (SGShopItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    CCSize size = tableView->getItemSize();
    if (NULL == item)
    {
//        item = SGShopItem::create(num);
        item = SGShopItem::create(size,num,false,this,tableView);
    }
    return item;
}
unsigned int  SGShopLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    //台灣go2play的gp版本需要添加到3rd版本的引流。
    int fix = 0;
    if (isTaiWanGo2PlayGooglePlay)
    {
        fix = 1;
    }
    
    //zyc .添加商城分类后 需要更改返回数目
    int vipLevel = SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel();
    if ( 0 < vipLevel )
    {
        //服务器推送 是否显示vip
        if ( SGPlayerInfo::sharePlayerInfo()->getShowvip()  )
        {
            if (vipLevel <nGuiShopingVipLimit )
                return 7 + fix;
            else
                return 8 + fix;
        }
    }
    return 6 + fix;
}

//////////
void SGShopLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    
    CCCallFunc *speedFun = NULL;
    CCCallFunc *speedFun2 = NULL;

    //判断是否是vip 以及vip的等级  修改num的值
    int vipLevel = SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel();
//    if ( vipLevel <= 8 ) //是vip
//    {
//        if (index > 1)
//            index += 1;
//    }
    if ( 0 < vipLevel )
    {
        if ( SGPlayerInfo::sharePlayerInfo()->getShowvip()  )
        {
            if (vipLevel <nGuiShopingVipLimit ) //没有洛阳鬼市
            {
                if (index > 1)
                    index += 1;
            }
        }
        else
        {
            if (index > 0)
            {
                index += 2;//
            }
        }
    }
    else
    {
        if ( index >0 )
            index += 2;
    }

    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    switch (index)
    {
        case SGST_RECHARGE://储值
            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::showStoreLayer));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
            
		case SGST_VIP://vip礼包
                speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
                speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::showGiftsLayer));
                this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
                break;
            
        case SGST_LUOYANG://洛阳鬼市
            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::onLuoYangGuiShiClicked));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
            
        case SGST_WANDERER: //云游商人
            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove ));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::onYunYouShangRenClicked ));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
        
        case SGST_LADDER: //天梯商城
            if(!player->canBreakLimitById(limitPvpBattle))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str21,player->getLimitLevelById(limitPvpBattle))->getCString() );
                return;
            }
            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::onPvpClicked ));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
            
        case SGST_JINGZHOU: //荆州
            if(!player->canBreakLimitById(limitShouJinZhou))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str22,player->getLimitLevelById(limitShouJinZhou))->getCString() );
                return;
            }
            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::onJinZhouClicked ));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
            
        case SGST_FAIRYLAND: //幻境商城
            if(!player->canBreakLimitById(limitSpecailBattle))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str24,player->getLimitLevelById(limitSpecailBattle))->getCString() );
                return;
            }
            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::onFairylandClicked));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
            
        case SGST_ARENA: //乱世商人（竞技场）
            if(!player->canBreakLimitById(limitArena))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str25,player->getLimitLevelById(limitArena))->getCString() );
                return;
            }

            speedFun = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::hideScrollItemSingleMove));
            speedFun2 = CCCallFunc::create(this,  callfunc_selector(SGShopLayer::onArenaClicked));
            this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*tableView->getRowCount()),speedFun2,NULL));
            break;
            
        case SGST_SPECIAL: //台湾GP版到3RD版的引流，SDK特殊需求。
        {
            if (isTaiWanGo2PlayGooglePlay)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                AndroidSDKAdp::getInstance()->openUrl(GlobalConfig::gi()->getTaiWanGpTo3rdLeadingUrl());
#endif
            }
        }
            break;
        default:
            
          break;
    }    

}


//////////
void SGShopLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
  
}

void SGShopLayer::showStoreLayer()
{
    main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);

}
void SGShopLayer::showGiftsLayer()
{
    //表明是从商城vip礼包进入  点击返回按钮时使用
    //by:zyc.
    SGMainManager::shareMain()->setFromShopLayer(true);
    
    
    main::ShopVipGiftBagEntryRequest *request = new main::ShopVipGiftBagEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VIPGIFT_BAGS, request);
}
void SGShopLayer::showExchangeLayer()
{
    main::ShopExchangeCoinEntryRequest *request = new main::ShopExchangeCoinEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_ENTER, request);
}




void SGShopLayer::exchangeEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopExchangeCoinEntryResponse *response = (main::ShopExchangeCoinEntryResponse *)sr->m_msg;
    
    if (sr)
    {
        int vipLvl = response->viplevel();
        int useChance = response->usedchance();
        int max = response->maxchance();
        int gold = response->gold();
        int coin = response->coin();
        SGMainManager::shareMain()->showExchangLayer(vipLvl,max,useChance,gold,coin);
    }
}
void SGShopLayer::showChgScoreLayer(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpShopListResponse *response = (main::PvpShopListResponse *)req->m_msg;
    if (response) {
        CCArray *datas = CCArray::create();
        int size = response->items_size();
        
//        DDLog("credit main-------------------%d", size);
        for (int ii = 0; ii < size; ii++) {
            main::PvpShopItem item = response->items(ii);
            int var1 = item.uid();
            int var2 = item.pvplevelmin();
            int var3 = item.costscore();
            int var4 = item.currentcount();
            int var5 = item.maxcount();
            //int itemId = item.itemid();
            
            SGCreditCardModel *model = SGCreditCardModel::create(var1, var2, var3, var4, var5);
            // 按服务器的15个type走
            model->setModelType(item.itemtype());
            model->setDisCount(item.discountcostscore());
            model->setitemId(item.itemid());
            datas->addObject(model);
        }
        SGMainManager::shareMain()->showChgCreditCardLayer(datas,0,1);
    }
}

void SGShopLayer::vipGiftBagListener(cocos2d::CCObject *obj)
{
    ;
}


void SGShopLayer::addGifts(SGVipGift *gift)
{
    if (_vipGifts == NULL) {
        _vipGifts = CCArray::create();
        _vipGifts->retain();
    }
    _vipGifts->addObject(gift);
}


//-------------------by:zyc

void SGShopLayer::onLuoYangGuiShiClicked()
{
    this->sendShopMsg(SHOPID_LUO_YANG);
}

void SGShopLayer::onYunYouShangRenClicked()
{
    this->sendShopMsg(SHOPID_YUN_YOU);
}

void SGShopLayer::onPvpClicked()
{
    this->sendShopMsg(SHOPID_PVP);
}

void SGShopLayer::onJinZhouClicked()
{
    this->sendShopMsg(SHOPID_JING_ZHOU);
}

//幻境商城
void SGShopLayer::onFairylandClicked()
{
    this->sendShopMsg(SHOPID_SPECIAL_BATTLE);
}

//乱世商人（竞技场）
void SGShopLayer::onArenaClicked()
{
    this->sendShopMsg(SHOPID_ARENA);
}

void SGShopLayer::sendShopMsg(ShopGUID shopId)
{
    SGMainManager::shareMain()->sendEnterMallLayerRequest(shopId , SHOP_LAYER);
}
//--------------------end




///////////////////////////////// class SGShopItem


SGShopItem::SGShopItem():delegate(NULL)
,tableview(NULL)
,menu(NULL)
{
}
SGShopItem::~SGShopItem()
{
    
    CCLOG("~SGShopItem");
}
bool SGShopItem::initWithSize(int num)
{
    if (!SNSTableViewCellItem::init()) {
		return false;
	}

    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_shopLayer);

    bgbgbg = NULL;    
    
    ////////// store_mainbg.png
    SGButton*btn=SGButton::create("upwar_cell_bg.png", NULL, this, menu_selector(SGShopItem::buttonClick),CCPointZero,false,true);
    this->addChild(btn);
    addBtn(btn);
    //////////
  
//    CCSprite *title = NULL;
    SGCCLabelTTF *label = NULL;
    int nGuiShopingVipLimit = 0;//鬼市vip限制等级
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("75");
    nGuiShopingVipLimit = dic1->valueForKey("value")->intValue();
    

    
    //判断是否是vip 以及vip的等级  修改num的值
    int vipLevel = SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel();
//    if ( vipLevel <= 8 ) //是vip
//    {
//        if (num > 1)
//            num += 1;
//    }
    
    if ( 0 < vipLevel )
    {
        if ( SGPlayerInfo::sharePlayerInfo()->getShowvip()  )
        {
            if (vipLevel <nGuiShopingVipLimit ) //没有洛阳鬼市
            {
                if (num > 1)
                    num += 1;
            }
        }
        else
        {
            if (num > 0)
            {
                num += 2;
            }
        }        
    }
    else
    {
        if ( num > 0)
            num += 2 ;
    }
    
    switch (num)
    {//vip
        case SGST_RECHARGE:
        {
//            title = CCSprite::createWithSpriteFrameName("font_getgold.png");
            label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str7, FONT_PANGWA, FONTSIZE(18));
            if (!SGPlayerInfo::sharePlayerInfo()->getHaveCharged()) {
                
                CCSprite *chongzhi = CCSprite::createWithSpriteFrameName("firstCharge2.png");
                CCSprite * chongzhiGuan = CCSprite::createWithSpriteFrameName("firstChargeGuang.png");
                if (chongzhiGuan) {
                    chongzhiGuan->setAnchorPoint(ccp(0.5,0.5));
                    chongzhiGuan->ignoreAnchorPointForPosition(false);
                    chongzhiGuan->setPosition(ccp(chongzhi->getContentSize().width/2,chongzhi->getContentSize().height/2));
                    chongzhiGuan->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
                    chongzhi->addChild(chongzhiGuan,-10,-10);
                }
                chongzhi->setPosition(ccp(btn->getContentSize().width/2 - chongzhi->getContentSize().width*0.32,-chongzhi->getContentSize().height*0.1));
                addChild(chongzhi);
            }
            
        }
            break;
		case SGST_VIP:
		{
            //新版商店 此处为vip礼包，一直显示，点击需要判断是否是vip
            //if (SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() )
           // {
                label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str8, FONT_PANGWA, FONTSIZE(18));
           // }
          
		}
			break;
        case SGST_LUOYANG://洛阳鬼市
        {
            //if (SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() )
            //{
                label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str18,FONT_PANGWA,FONTSIZE(18) );
           // }
        }
            break;
            
        case SGST_WANDERER:  //云游商人
            label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str20, FONT_PANGWA, FONTSIZE(18));
            break;
        case SGST_LADDER://天梯商城,一直显示，点击需要判断等级是否够天梯
            label = SGCCLabelTTF::create(str_BattleScorelayer_str1, FONT_PANGWA, FONTSIZE(18));
            break;
        case SGST_JINGZHOU://荆州商城
            label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str19, FONT_PANGWA, FONTSIZE(18));
            break;
        case SGST_FAIRYLAND: //幻境商城
        {
            label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str26, FONT_PANGWA, FONTSIZE(18));
        }
            break;
        case SGST_ARENA: //乱世商人（竞技场）
        {
            label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str27, FONT_PANGWA, FONTSIZE(18));
        }
            break;
        case SGST_SPECIAL: //台湾GP版到3RD版的引流，SDK特殊需求。
        {
            if (isTaiWanGo2PlayGooglePlay)
            {
                label = SGCCLabelTTF::create(str_GoldShopLimitTimeSell_str23, FONT_PANGWA, FONTSIZE(18));
            }
        }
            break;
        default:
            break;

    }
    if (!label ) //就是不按顺序来，中间vip需要判断才显示
        CCAssert(label !=NULL, "label can not null");
    this->addChild(label);
    return true;
}
SGShopItem* SGShopItem::create(CCSize size,int num,bool isshow ,SGShopItemItemDelegate*del,SNSTableView*table)
{
	SGShopItem *instance = new SGShopItem();
	if (instance && instance->initWithSize(num)) {
        instance->delegate=del;
        instance->tableview=table;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}


void SGShopItem::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);
    
}

//////////////////////////////////      class SGShopItem end.
