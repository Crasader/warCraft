//
//  SGStoreLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//
//#import "SdkReqCallBack.h"
//#include "OcCaller.h"
#include "SGStoreLayer.h"
#include "SGMainManager.h"
#include "SGStoreItem.h"
#include "SGPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "ShopModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGExpandFriend.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "ExtClass.h"
#include "SGShowString.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#elif (PLATFORM == ANDROID)
#include "AndroidSDKAdp.h"
#endif
#include "GlobalConfig.h"

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
#include "auxiliary/UCSdk.h"
#include "auxiliary/UCSdkCallback.h"
#include "android/jni/CUCGameSdk.h"
using namespace ucgamesdk;
#endif

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
#include "KuaiYongIosImp.h"
#endif

#include "SdkController.h"


SGStoreLayer::SGStoreLayer()
:label_exp(NULL)
,label1(NULL)
,label2(NULL)
,enterType(0)
,fontvip(NULL)
,expprog(NULL)
,vipLevel(0)
,currvip(NULL)
,special(NULL)
,redbg(NULL)
,_vipGifts(NULL)
,_item(NULL)
,_strary(NULL)
, _time(0)
,ranNum(0)
{
    _vipGifts = NULL;
    _item = NULL;
}
SGStoreLayer::~SGStoreLayer()
{
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_storeLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_STORE_BUY);
    notification->removeObserver(this, MSG_EXCHANGE_ENTER);
//    notification->removeObserver(this, MSG_VIPGIFT_BAGS);

    //modified by cgp
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey("store/storenum.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTexture(texture);
    if (_vipGifts) {
        _vipGifts->release();
        _vipGifts = NULL;
    }
    if (_item) {
        _item->release();
        _item = NULL;
    }
    if (_strary) {
        _strary->release();
        _strary = NULL;
    }
	

	
#if IS_IAP_STORE

#if (PLATFORM == IOS)
	if (OcCaller::getInstance()->isIapInstanceExist())
	{
		OcCaller::getInstance()->iapInstnceRelease();
	}
#else
	//android code
#endif
	/*
    if (iap) {
        [iap release];
    }
	 */
#endif
    
}


SGStoreLayer *SGStoreLayer::create(int curr,int next,int currrmb,int maxrmb,CCArray *a,int type,CCArray *strary)
{
    SGStoreLayer *storeLayer = new SGStoreLayer();
    if (storeLayer && storeLayer->init(NULL, sg_storeLayer))
    {
        storeLayer->enterType = type;
        storeLayer->initView(curr,next,currrmb,maxrmb,a);
		storeLayer->_strary = CCArray::create();
		storeLayer->_strary ->retain();
        storeLayer->_strary->addObjectsFromArray(strary);
        storeLayer->autorelease();

        return storeLayer;
    }
    CC_SAFE_DELETE(storeLayer);
    return NULL;
}
void SGStoreLayer::reSetData(CCArray *a,CCArray *strary)
{
    if(_strary)
    {
        _strary->removeAllObjects();
        _strary->addObjectsFromArray(strary);
    }
    if(datas)
    {
        datas->removeAllObjects();
        datas->addObjectsFromArray(a);
    }
    tableView->reloadData();
}
void SGStoreLayer::initView(int curr,int next,int currrmb,int maxrmb,CCArray *ary)
{
	
#if IS_IAP_STORE
#if (PLATFORM == IOS)
	OcCaller::getInstance()->gameIapStoreAlloc();
#else
	//android code
#endif
    //iap = [[GameIapStore alloc] init];
#endif
    
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist",RES_TYPE_LAYER_UI ,sg_storeLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_storeLayer);

    if (_vipGifts)
	{
        _vipGifts->release();
        _vipGifts = NULL;
    }
    vipLevel = curr;
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_STORE_BUY,
                              this,
                              callfuncO_selector(SGStoreLayer::buyGoldListener));
    notification->addObserver(MSG_EXCHANGE_ENTER,
                              this,
                              callfuncO_selector(SGStoreLayer::exchangGoldListener));
//    notification->addObserver(MSG_VIPGIFT_BAGS,
//                              this,
//                              callfuncO_selector(SGStoreLayer::vipGiftBagListener));
    
//    notification->addObserver(MSG_VIPGIFT_BAGS,
//                              this,
//                              callfuncO_selector(SGStoreLayer::vipGiftBagListener));
    tableViewHeight = skewing(55);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    datas = CCArray::create();
    datas->retain();
    datas ->addObjectsFromArray(ary);
   
    redbg = CCSprite::createWithSpriteFrameName("store_redbg.png");
    redbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt - redbg->getContentSize().height*.55)));
    redbg->setScaleX(s.width/redbg->getContentSize().width);
    
    
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
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_CantAdvance_tips35_31, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN );
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(upCenter, ccp(0, -headhgt+65/2)));
    this->addChild(title);


    if (SGPlayerInfo::sharePlayerInfo()->getShowvip())
	{
        tableView->setFrame(CCRectMake(0, 0, s.width, s.height- btmhgt - headhgt - redbg->getContentSize().height - 5));
    }
    else
    {
		tableView->setFrame(CCRectMake(0, 0, s.width, s.height- btmhgt - headhgt - 5));
    }
    
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, btmhgt)));
    
    
	CCSprite *font_vip = CCSprite::createWithSpriteFrameName("font_storevip.png");
    
	font_vip->setPosition(ccpAdd(redbg->getPosition(), ccp(-s.width*.4, 0)));

           
	CCSprite *expbg = CCSprite::createWithSpriteFrameName("store_jindutiao.png");
	expbg->setPosition(ccpAdd(redbg->getPosition(), ccp(10, 0)));
	expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("store_jindutiao_li.png"));
	expprog->setType(kCCProgressTimerTypeBar);
	expprog->setMidpoint(ccp(0,0));
	expprog->setBarChangeRate(ccp(1, 0));
	expprog->setPosition(expbg->getPosition());
    this->addChild(redbg);
    special = SGButton::create("store_btn.png", "font_tequan.png", this, menu_selector(SGStoreLayer::showGiftsLayer),CCPointZero,false,true);
    this->addBtn(special);
    special->setPosition(ccpAdd(expprog->getPosition(), ccp(253, 0)));
    this->addChild(font_vip,1);
    this->addChild(expbg);
    this->addChild(expprog);


    if (!SGPlayerInfo::sharePlayerInfo()->getShowvip())
	{
        redbg->setVisible(false);
        special->setEnabled(false);
        special->setVisible(false);
        font_vip->setVisible(false);
        expbg->setVisible(false);
        expprog->setVisible(false);
    }
    
        //您当前是vip12 享受最尊崇的服务
        if (curr == 0)
		{
            label1 = SGCCLabelTTF::create(str_StoreLayer_str1, FONT_BOXINFO, 20);
            label1->setAnchorPoint(ccp(0, 0.5));
            
            label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, 24)));
            
            CCString *str_ = CCString::createWithFormat(str_StoreLayer_str2,maxrmb-currrmb);
            label2 = SGCCLabelTTF::create(str_->getCString(), FONT_BOXINFO, 20,ccc3(0xe7, 0xc4, 0x6a));
            label2->setAnchorPoint(ccp(0, 0.5));
            label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, -26)));
            
            //label2->setInsideColor(ccc3(0xe7, 0xc4, 0x6a));
            float rate = (float)(currrmb)/maxrmb;
            expprog->setPercentage(rate*100);
            
            label_exp = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",currrmb,maxrmb)->getCString(), FONT_BOXINFO, 20);
            
            label_exp->setPosition(ccpAdd(redbg->getPosition(), ccp(redbg->getContentSize().width*.2, 24)));
            this->addChild(label1);
            this->addChild(label2);
            this->addChild(label_exp);
            if (!SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
                label1->setVisible(false);
                label2->setVisible(false);
                label_exp->setVisible(false);
            }
        }else if(curr == 12)
        {
            label1 = SGCCLabelTTF::create(str_StoreLayer_str3, FONT_BOXINFO, 20);
            label1->setAnchorPoint(ccp(0, 0.5));
            label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, 24)));
            
            
            fontvip = CCSprite::createWithSpriteFrameName("font_store_vip.png");
            
            fontvip->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width + fontvip->getContentSize().width*.5, 0)));
            
            currvip = CCLabelAtlas::create(CCString::createWithFormat("%d",curr)->getCString(), "sanguobigpic/store_shuzi_z.png", 18, 23, '0');
            currvip->setAnchorPoint(ccp(0, 0.5));
            currvip->setPosition(ccpAdd(fontvip->getPosition(), ccp(fontvip->getContentSize().width*.5, 0)));
            
            
            label2 = SGCCLabelTTF::create(str_StoreLayer_str4, FONT_BOXINFO, FONTSIZE(12) ,ccc3(0xe7, 0xc4, 0x6a) );
            
            label2->setAnchorPoint(ccp(0, 0.5));
            label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, -26)));
            //label2->setColor(ccc3(0xe7, 0xc4, 0x6a));
            
//            int currLevelExperienceMax = SGStaticDataManager::shareStatic()->getVipById(next)->getneedRMB();//当前等级最大经验
            float rate = (float)(currrmb)/maxrmb;
            expprog->setPercentage(rate*100);
            
            label_exp = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",currrmb,maxrmb)->getCString(), FONT_BOXINFO, 20);
            label_exp->setPosition(ccpAdd(redbg->getPosition(), ccp(redbg->getContentSize().width*.2, 24)));
            this->addChild(label1);
            this->addChild(fontvip);
            this->addChild(currvip);
            this->addChild(label2);
            this->addChild(label_exp);

            if (!SGPlayerInfo::sharePlayerInfo()->getShowvip())
			{
                label1->setVisible(false);
                font_vip->setVisible(false);
                currvip->setVisible(false);
                label2->setVisible(false);
                label_exp->setVisible(false);
                fontvip->setVisible(false);
			}
            return;
        }
        else
		{
            
            label1 = SGCCLabelTTF::create(str_StoreLayer_str3, FONT_BOXINFO, 20);
            label1->setAnchorPoint(ccp(0, 0.5));
            label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, 24)));
            
            
            fontvip = CCSprite::createWithSpriteFrameName("font_store_vip.png");
            
            fontvip->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width + fontvip->getContentSize().width*.5, 0)));
            
            currvip = CCLabelAtlas::create(CCString::createWithFormat("%d",curr)->getCString(), "sanguobigpic/store_shuzi_z.png", 18, 23, '0');
            currvip->setAnchorPoint(ccp(0, 0.5));
            currvip->setPosition(ccpAdd(fontvip->getPosition(), ccp(fontvip->getContentSize().width*.5-2, 0)));
           
            CCString *str_ = CCString::createWithFormat(str_StoreLayer_str5,maxrmb-currrmb,next);
            label2 = SGCCLabelTTF::create(str_->getCString(), FONT_BOXINFO, FONTSIZE(12) , ccc3(0xe7, 0xc4, 0x6a) );
            
            label2->setAnchorPoint(ccp(0, 0.5));
            label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, -26)));
            //label2->setColor(ccc3(0xe7, 0xc4, 0x6a));
            
            float rate = (float)(currrmb)/maxrmb;
            expprog->setPercentage(rate*100);
            
            label_exp = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",currrmb,maxrmb)->getCString(), FONT_BOXINFO, 20);
            
            label_exp->setPosition(ccpAdd(redbg->getPosition(), ccp(redbg->getContentSize().width*.2, 24)));
            this->addChild(label1);
            this->addChild(fontvip);
            this->addChild(currvip);
            this->addChild(label2);
            this->addChild(label_exp);

            if (!SGPlayerInfo::sharePlayerInfo()->getShowvip())
			{
                label1->setVisible(false);
                fontvip->setVisible(false);
                currvip->setVisible(false);
                label2->setVisible(false);
                label_exp->setVisible(false);
			}
            
        }

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGStoreLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.65, title->getPosition().y));

}
void SGStoreLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
    tableView->setTouchEnabled(isTouch);
}

SNSTableViewCellItem *SGStoreLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGStoreItem *item = (SGStoreItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if (NULL == item)
    {
        item = SGStoreItem::create(num,datas,false,this,tableView);
    }
    else
    {
       item ->updatedata(num,datas);
    }
    return item;
}
unsigned int  SGStoreLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
   // CCLOG("%d",datas->count());
    return datas->count();
}

//月卡即时买完之后,需要刷新
void SGStoreLayer::freshMonthCard(int days)
{
	int b = 0;//rmb数
    int label = 0;//热卖,超值标志
    int numyb = 0;//元宝数
    int sheng = 0;//省百分比
    sscanf(((CCString*)datas->objectAtIndex(0))->getCString(), "%d-%d-%d-%d", &label,&sheng,&numyb,&b);
	
	CCString *contents = CCString::createWithFormat("%d-%d-%d-%d", days, sheng, numyb, b);
	
	datas->removeObjectAtIndex(0);
	datas->insertObject(contents, 0);
	
	tableView->reloadData();
}

void SGStoreLayer::itemselect(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_34);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    //int rmb = 0;
    //CCLOG("%s",((CCString *)_strary->objectAtIndex(index))->getCString());//cgpLogDel
    
    userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    _time = tv.tv_sec;
    ranNum = getRandom(1,100000);
   
    int nshowtype = 0 ;//标签类型
    int nChargeType = 0;//充值类型
    int nDiscount = 0; //折扣
    int nGoldNum = 0;//获得元宝数
    int nCostRMB = 0;//花费
    int ngoldPerDay = 0 ;//每天可领数
    int nleftDay = 0;//剩余可购买天数
    int nRewardDay = 0;//可领天数
    //    CCString *str = (CCString *)ary->objectAtIndex(num);
    sscanf(((CCString*)datas->objectAtIndex(index))->getCString(), "%d-%d-%d-%d-%d-%d-%d-%d", &nshowtype,&nChargeType,&nDiscount,&nGoldNum,&nCostRMB,&ngoldPerDay,&nleftDay,&nRewardDay);
    
    //月卡购买测试 2012.2.12
    if (nChargeType != 0) {
        if (nleftDay > 0) {
            SGMainManager::shareMain()->showMessage(str_StoreLayer_str6);
            return;
        }
    }
    
    //MM: 这一段注掉的代码，反映了之前TB和PP的充值逻辑，备案，九成可能用不到了。
    /*
    //月卡购买测试 2012.2.12
	//如果是朋游SDK
	if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
	{
#if (PLATFORM == IOS)
//		OcCaller::getInstance()->onChargeSDK(userId, _time, ranNum, ((CCString *)_strary->objectAtIndex(index))->getCString(),nCostRMB, nGoldNum);
#else
//        AndroidSDKAdp::getInstance()->onChargeSDK(userId, _time, ranNum, ((CCString *)_strary->objectAtIndex(index))->getCString(), nCostRMB, nGoldNum);
#endif
	}
	else//如果是其他渠道SDK
	{
		//SDK
		//####################################################################################################
		//订单号
		int serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
		int roleId = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
    

#if (PLATFORM == IOS)
		const char *t = OcCaller::getInstance()->getTime();
#else
		//android code
		const char *t = "0";
#endif
		//支付描述中的各个字段
		std::string payDescTemp;
		if (GAME_ACCOUNT_TYPE == GCAT_PP)
		{
			payDescTemp.append(CCString::createWithFormat("%d元宝", nGoldNum)->getCString());
		}
		else
		{
			payDescTemp.append(((CCString *)_strary->objectAtIndex(index))->getCString());
			payDescTemp.append(",");
			payDescTemp.append(BUNDLE_VERSION);//客户端版本号
			payDescTemp.append(",");
			payDescTemp.append(t);//时间

			payDescTemp.append(",");
			payDescTemp.append(CCString::createWithFormat("%d", serverId)->getCString());//区ID
			payDescTemp.append(",");
			payDescTemp.append(CCString::createWithFormat("%d", roleId)->getCString());//角色ID
			payDescTemp.append(",");
			payDescTemp.append(CCString::createWithFormat("%s", userId.c_str())->getCString());//用户ID
		}
		//std::string payDescTemp(((CCString *)_strary->objectAtIndex(index))->getCString());//商品ID
		
		const char *orderNumber = CCString::createWithFormat("%s%ld%d", userId.c_str(), _time, ranNum)->getCString();
		//公共接口  计费
        
# if IS_IAP_STORE
            ExtClassOfSDK::sharedSDKInstance()->userPay(orderNumber, static_cast<float>(nCostRMB), payDescTemp.c_str());
#endif
		//
		//##################################################################################################
	}
    */
    
    //MMD: SDKIF 所有充值相关在此添加。
    if (IS_IAP_STORE == 1) //默认开启所有付费功能。
    {
#if (PLATFORM == IOS)
        //MM: IOS充值BGN<<
#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE) //台湾go2play ios
        SGMainManager::shareMain()->sendCreatInvoice(nCostRMB,((CCString *)_strary->objectAtIndex(index))->getCString());
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD) //快用 ios
        //TODO: 快用支付。
        SGMainManager::shareMain()->showMessage("计费暂未开放噢~");
        return;
//        KuaiYongIosImp* kyImp = (KuaiYongIosImp*) SdkController::gi()->getSdkImp();
//        kyImp->showPaymentView();
#else //大陆xdyou ios 包含（GCCT_CHINA_XDYOU_IOS_APPLE_APPSTORE和各种测试模式，如果需要在测试模式下联调充值，则修改这里与上面一样即可。）
        OcCaller::getInstance()->iapBuyItem(((CCString *)_strary->objectAtIndex(index))->getCString());
#endif
        //MM: IOS充值END>>
        
#elif (PLATFORM == ANDROID)
        //MM: ADR充值BGN<<
        int serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
        std::string accountId = CCUserDefault::sharedUserDefault()->getStringForKey("userId", "XDYOU_UFO"); //貌似说是可能收到纯数字的acountId，现默认用ufo代替。
        SGPlayerInfo* sgpi = SGPlayerInfo::sharePlayerInfo();
        int roleId = sgpi->getPlayerRoleId();
        const char* roleName = sgpi->getPlayerName()->getCString();
        int roleGrade = sgpi->getPlayerLevel();
        
        char pszRoleId[10];
        sprintf(pszRoleId, "%d", roleId);
        char pszRoleGrade[10];
        sprintf(pszRoleGrade, "%d", roleGrade);
        CCLOG("PAYMENT || channelType=%d, nCost=%d, roleId=%d, roleName=%s, serverId=%d", GAME_CHANNEL_TYPE, nCostRMB, roleId, roleName, serverId);
        //记得届时在后台配置callback地址，并且修改支付金额。
#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD) //台湾go2play adr 3rd/gp
        AndroidSDKAdp::getInstance()->charge(((CCString *)_strary->objectAtIndex(index))->m_sString, nCostRMB, serverId, accountId, roleId);
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD) //大陆uc adr
        //加入充值4元素，由uc服务器回调给game服务器。规则：serverId,accountId,roleId,chargeId
        char customInfo[200];
        sprintf(customInfo, "%d,%s,%d,%s", serverId, accountId.c_str(), roleId, ((CCString *)_strary->objectAtIndex(index))->m_sString.c_str());
        CUCGameSdk::pay(false, nCostRMB, UCSdk::s_serverid, pszRoleId, roleName, pszRoleGrade, customInfo, "", "", UCSdkCallback::pay_callback);
#endif
        //MM: ADR充值END>>
#endif
    }
    else //未开启付费的提示
    {
        SGMainManager::shareMain()->showMessage(str_StoreLayer_str7);
    }
}

/////////
void SGStoreLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{

    
}

void SGStoreLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    //0 正常进入 1探访进入 2金子换铜钱进入 3购买礼包 4扩建军营 5拜财神 6故事模式 7扩建好友
    switch (enterType) {
        case 0:
            SGMainManager::shareMain()->showShopLayer();
            break;
        case 1:
            showLayerByTag(sg_visitLayer);
            break;
        case 2:
            showExchangeLayer();
            break;
        case 3:
            showGiftsLayer();
            break;
        case 4:
            SGMainManager::shareMain()->showExpandLayer();
            break;
        case 5:
            showLayerByTag(sg_regLotteryLayer);
            break;
        case 6:
            showLayerByTag(sg_storySectionLayer);
            break;
        case 7:{
            SGExpandFriend *expand = SGExpandFriend::create();
            SGMainManager::shareMain()->showLayer(expand);
        }break;
        default:
            break;
    }
    
}
void SGStoreLayer::showGiftsLayer()
{
    
    main::ShopVipGiftBagEntryRequest *request = new main::ShopVipGiftBagEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VIPGIFT_BAGS, request);
}
void SGStoreLayer::vipGiftBagListener(cocos2d::CCObject *obj)
{

}

void SGStoreLayer::showExchangeLayer()
{
    main::ShopExchangeCoinEntryRequest *request = new main::ShopExchangeCoinEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_ENTER, request);
}
void SGStoreLayer::exchangGoldListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopExchangeCoinEntryResponse *response = (main::ShopExchangeCoinEntryResponse *)sr->m_msg;
    
    if (sr)
    {
        int vipLvl = response->viplevel();
        int useChance = response->usedchance();
        int max = response->maxchance();
        int gold =response->gold();
        int coin = response->coin();
        SGMainManager::shareMain()->showExchangLayer(vipLvl,max,useChance,gold,coin);
    }
    
}
void SGStoreLayer::buyGoldListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_storeLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopChargeResponse *response = (main::ShopChargeResponse *)sr->m_msg;
	
	//朋游的SDK
	
	/*
    if (sr && iap && GAME_ACCOUNT_TYPE == GCAT_XDYOU) {
        
        [iap buyPaymentQueue:response->state()];
        
        [SdkReqCallBack onChargeSuccess: [NSString stringWithFormat:@"User%dTimeR%ld%d", userId,_time,ranNum]];
		
		//
		
    }
	 */
	
	bool iapIsExist = false;
#if (PLATFORM == IOS)
	OcCaller *caller = OcCaller::getInstance();
	iapIsExist = caller->isIapInstanceExist();
#endif
	if (sr &&  iapIsExist && GAME_ACCOUNT_TYPE == GCAT_XDYOU) //xdyou账号体系下。
	{
		
#if (PLATFORM == IOS)
        
		caller->iapBuyPayMent(response->state());
		//caller->onChargeSuccessSDK(userId, _time, ranNum);
#else
//        AndroidSDKAdp::getInstance()->onChargeSuccessSDK(userId, _time, ranNum);
#endif
	}
	else if (sr && GAME_ACCOUNT_TYPE != GCAT_XDYOU) //非xdyou账号体系下。
	{
		// 处理SDK购买成功
		if (response->state())
		{
			SGMainManager::shareMain()->showMessage(str_StoreLayer_str8);
		}
		else if (!response->state())
		{
			SGMainManager::shareMain()->showMessage(str_StoreLayer_str9);
		}
		
	}
	
	if (response->state() == 1) {
        SGPlayerInfo::sharePlayerInfo()->setHaveCharged(1);
        SGMainManager::shareMain()->getMainLayer()->hideFirstCharge();
    }
	if (response->state() == 1)
	{
		//成功或者失败都设置月卡的天数
		//this->freshMonthCard(response->monthlycardday());
        main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
	}

}

void SGStoreLayer::showLayerByTag(LayerTag layerTag)
{
    
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(layerTag);
    if(NULL == layer)
    {
        return;
    }
    
    SGMainManager::shareMain()->showLayer(layer);
    
    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_storeLayer, true);
    
}

