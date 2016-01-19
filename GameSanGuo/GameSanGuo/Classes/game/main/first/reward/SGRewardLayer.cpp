//
//  SGRewardLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#include "SGRewardLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGMsgId.h"
#include "RewardModule.pb.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "SGRewardItem.h"
#include "SimpleAudioEngine.h"
#include "Invite.pb.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "MonthlyCardModule.pb.h"
#include "SGStringConfig.h"
#include "SGLottery9ViewItem.h"
#include "SGMonthlySignLayer.h"

itemObj::itemObj()
:m_itemid(0),
m_state(0),
m_count(0),
m_ishave(0)
{
}
itemObj::~itemObj()
{
}
SGRewardLayer::SGRewardLayer()
:select(0),
m_tableView(NULL),
_array(NULL)
{
  
}
SGRewardLayer::~SGRewardLayer()
{
    CC_SAFE_RELEASE(_array);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_LVLUPENTER);
    notification->removeObserver(this, MSG_REWARD_SALARYENTER);
    notification->removeObserver(this, MSG_REWARD_RECOVERENTER);
    notification->removeObserver(this, MSG_REWARD_GODENTER);
    notification->removeObserver(this, MSG_REWARD_LIANDANENTER);
    notification->removeObserver(this, MSG_REWARD_REGENTER);
    notification->removeObserver(this, MSG_REWARD_INTIVER);
    notification->removeObserver(this, MSG_REWARD_BIND_INFO);
    notification->removeObserver(this, MSG_EXCHANGECODE_INFO);
    notification->removeObserver(this, MSG_REWARD_YUEKA);
    notification->removeObserver(this, MSG_REWARDLIST_LOGIN);
    notification->removeObserver(this, MSG_MONTHLY_SIGN_MAIN_INFO);
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_rewardsLayer);
    
}


SGRewardLayer *SGRewardLayer::create()
{
    SGRewardLayer *rewardLayer = new SGRewardLayer();
    if (rewardLayer && rewardLayer->init(NULL, sg_rewardsLayer))
    {
        rewardLayer->initView();
        rewardLayer->autorelease();
        return rewardLayer;
    }
    CC_SAFE_DELETE(rewardLayer);
    return NULL;
}
void SGRewardLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_rewardsLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_SALARYENTER,
                              this,
                              callfuncO_selector(SGRewardLayer::salaryEnterListener));
    notification->addObserver(MSG_REWARD_RECOVERENTER,
                              this,
                              callfuncO_selector(SGRewardLayer::recoverEnterListener));
    notification->addObserver(MSG_REWARD_GODENTER,
                              this,
                              callfuncO_selector(SGRewardLayer::godEnterListener));
    notification->addObserver(MSG_REWARD_LIANDANENTER,
                              this,
                              callfuncO_selector(SGRewardLayer::liandanEnterListener));
    notification->addObserver(MSG_REWARD_REGENTER,
                              this,
                              callfuncO_selector(SGRewardLayer::regEnterListener));
    notification->addObserver(MSG_REWARD_INTIVER,
                              this,
                              callfuncO_selector(SGRewardLayer::inviterListener));
    notification->addObserver(MSG_REWARD_BIND_INFO,
                              this,
                              callfuncO_selector(SGRewardLayer::bindListener));
    notification->addObserver(MSG_EXCHANGECODE_INFO , this,
                              callfuncO_selector(SGRewardLayer::showExchangeCodeListener));
    
    notification->addObserver(MSG_REWARD_YUEKA , this,
                              callfuncO_selector(SGRewardLayer::yueKaEnterListener));
    notification->addObserver(MSG_REWARDLIST_LOGIN, this, callfuncO_selector(SGRewardLayer::showLoginRewardListener));
    notification->addObserver(MSG_MONTHLY_SIGN_MAIN_INFO, this, callfuncO_selector(SGRewardLayer::showMonthlySignResponse));
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    _array = CCArray::create();
    _array->retain();
    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
    int len = array->count();
    bool ishave = false;
    int nCount = 0;
    for (int i = 0; i<len; i++)
    {
        SGRewardNoticeItem * typestate = (SGRewardNoticeItem *)array->objectAtIndex(i);
        if (typestate->getstateItem() !=2)
        {
            itemObj *obj = new itemObj;
            obj->setitem(typestate->gettypeItem());
            if (typestate->gettypeItem()== Reward_OtherLianDan )
            {
                obj->setishave(0);
                
                if(ishave)
                {
                    obj->setishave(1);
                }
                ishave = true;
                obj->setcount(nCount);
                nCount++;
            }
            else
            {
                obj->setcount(0);
            }
            
            
            obj->setstate(typestate->getstateItem());
            _array->addObject(obj);
            obj->autorelease();
        }        
    }

    
    tableViewHeight = 110;
    
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
    tableView->setTableType(TableViewTypeVertical);
    tableView->setPageVertical(true);
    tableView->setPageNEnable(false);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);

    
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
    
        SGCCLabelTTF *title = SGCCLabelTTF::create(str_RewardBindLayer_str15, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65*.48)));
    this->addChild(title);
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGRewardLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));
    tableView -> setPageNEnable(true);
    
    EFFECT_PLAY(MUSIC_ITEM);

    
}

SNSTableViewCellItem *SGRewardLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGRewardItem *item = (SGRewardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    itemObj *obj = (itemObj*)_array->objectAtIndex(num);
    if (NULL == item)
    {
        item = SGRewardItem::create(obj,this,tableView);
    }    
    if (num < _array->count())
    {
        item->updateOfficerCard(obj);
    }    
    return item;
}
unsigned int  SGRewardLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return _array->count();
}
///////////
 void SGRewardLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    
    itemObj *str = (itemObj*)_array->objectAtIndex(index);
    if (str->getitem() == Reward_YueKa)
    {
        showYueKaReward();
    }
    else if (str->getitem() == Reward_LvlUpGold)
    {
//         showExchangeCode();
        showGetLvlUpGold();
    }else if (str->getitem() == Reward_Caishen)
    {
        showcaishen();
    }else if (str->getitem() == Reward_Salary)
    {
        showGetSalary();
    }else if (str->getitem() == Reward_Wine)
    {
        showWine();
    }else if (str->getitem() == Reward_Hope)
    {
        showHope();
    }else if (str->getitem() == Reward_Inviter)
    {
        showInviter();
    }else if (str->getitem() == Reward_Invitee)
    {
        showInvitee();
    }
    else if (str->getitem()== Reward_Bind)
    {
        showBind();
    }
    else if(str->getitem()== Reward_ExchangeCode)
    {
        showExchangeCode();
    }
    else if(str->getitem()== Reward_OpenServer)
    {
        showLoginReward();
    }
    else if (str->getitem() == Reward_MonthlySign)
    {
        showMonthlySignRequest();
    }
    else
    {
        select = str->getcount();
        showliandan();
    }

}
//////////
void SGRewardLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    
//    itemObj *str = (itemObj*)_array->objectAtIndex(index);
//    if (str->getitem() == 0)
//    {
//        showGetLvlUpGold();
//    }else if (str->getitem() == 1)
//    {
//        showcaishen();
//    }else if (str->getitem() == 2)
//    {
//        showGetSalary();
//    }else if (str->getitem() == 3)
//    {
//        showWine(); 
//    }else if (str->getitem() == 4)
//    {
//        showHope();
//    }else if (str->getitem() == Reward_Inviter)
//    {
//        showInviter();
//    }else if (str->getitem() == Reward_Invitee)
//    {
//        showInvitee();
//    }
//    else if (str->getitem()==7)
//    {
//        showBind();
//    }
//    else
//    { 
//        select = str->getitem()-Reward_OtherLianDan;
//        showliandan();
//    }
}

void SGRewardLayer::bindListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::RewardBindAccountInfoResponse *response = (main::RewardBindAccountInfoResponse *)sr->m_msg;
        int length = response->rewardinfo_size();
        CCArray *ary=CCArray::create();
        CCArray *ary2=CCArray::create();
       
        for(int i=0; i<length; i++)
        {
           main::RewardInfoProto ss = response->rewardinfo(i);
            int type=ss.type();
            int value=ss.value();
            int number=ss.number();
            CCString *string=CCString::createWithFormat("%d-%d-%d",type,value,number);
            ary->addObject(string);
        }
        int nLength2 = response->mailrewardinfo_size();
        for(int i=0; i<nLength2; i++)
        {
            main::RewardInfoProto ss = response->mailrewardinfo(i);
            int type=ss.type();
            int value=ss.value();
            int number=ss.number();
            CCString *string=CCString::createWithFormat("%d-%d-%d",type,value,number);
            ary2->addObject(string);
        }
        int state = response->state();
        int state2 = response->mailstate();
        std::string  mail= response->mail();
//        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
//        int nsum = array->count();
//        int state = 0;
//        for (int i = 0 ;i < nsum ;i++) {
//            SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
//            if(item->gettypeItem()==Reward_Bind)
//            {
//                state = item->getstateItem();
//                break;
//            }
//        }

        //CCString *str = (CCString *)array->objectAtIndex(8);
        SGMainManager::shareMain()->showBindLayer(ary,state,ary2,state2,mail.c_str());
        
//        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
//        CCString *str = (CCString *)array->objectAtIndex(7);
//        CCString *str1 = (CCString *)array->objectAtIndex(Reward_Invitee);
//        CCString *str2 = (CCString *)array->objectAtIndex(1);
//        CCLOG("strInt:%d",str->intValue());
//        if (str->intValue() ==1)
//        {
//            SG_SHOW_WINDOW("恭喜领取元宝50");
//            str->setString("0");
//            if ((str1->intValue()==2)&&(str2->intValue()==2)) {
//                itemObj *obj = (itemObj*)_array->objectAtIndex(Reward_Inviter);
//                obj->setstate(0);
//            }
//            else if ((str1->intValue()!=2)&&(str2->intValue()!=2))
//            {
//                itemObj *obj = (itemObj*)_array->objectAtIndex(7);
//                obj->setstate(0);
//            }
//            else
//            {
//                itemObj *obj = (itemObj*)_array->objectAtIndex(Reward_Invitee);
//                obj->setstate(0);
//            }
//        }
//        else if (str->intValue()==3)
//        {
//            SGMainManager::shareMain()->showBindLayer(1, 2, 3);
//        }
//        else if (str->intValue() ==0)
//        {
//            CCLog("0000000000");
//        }
        
        //      tableView->reloadData();
        //      tableView->setTableType(TableViewTypeVertical);
    }
    
    

}
void SGRewardLayer::showLoginReward()
{
    main::LoginAwardListRequest *request=new main::LoginAwardListRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARDLIST_LOGIN, request);
}
void SGRewardLayer::showLoginRewardListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("showRewardLoginListener error !!! ");
    }
    else{
        main::LoginAwardListResponse response =*(main::LoginAwardListResponse *)request->m_msg;
        
        int length=response.model_size();
        bool  has=false;
        int min=1000;
        
        //检查是否有可领取的或者没有登入够15天
        for (int i=length-1; i>=0; i--) {
            main::LoginAwardModel lam=response.model(i);
            if (lam.isfinish()==-1) {//-1代表已领取，0；代表可领取，其他代表还有几天可领取
                continue;
                
            }
            if (lam.isfinish()==0) {
                min=lam.isfinish();
                has=true;
                break;
            }
        }
        
        bool LoginAwardGold=false;
        LoginAwardGold=SGPlayerInfo::sharePlayerInfo()->getLoginAwardGold();
        
        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if (guideId >= guide_tag_45)
        {
           
                SGMainManager::shareMain()->showTotalRewardLayer(response,this);
                
            
            
        }
    }
    

}


void SGRewardLayer::showExchangeCode(){
    main::ExchangeCodeEntryRequest *request= new main::ExchangeCodeEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGECODE_INFO, request);
}
void SGRewardLayer::showExchangeCodeListener(CCObject *obj){
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        std::string desp;
        
        main::ExchangeCodeEntryResponse *response=( main::ExchangeCodeEntryResponse *)sr->m_msg;
        int length=response->info_size();
        for (int i=0; i<length; i++) {
            desp.append(response->info(i));
            desp.append("\n");
            //CCLog("%s",desp.c_str());
        }
        CCString *despriteion=CCString::create(desp);
        SGMainManager::shareMain()->showExchangeCodeLayer(despriteion);
    }

   

}
void SGRewardLayer::showBind()
{
    main::RewardBindAccountInfoRequest *request = new main::RewardBindAccountInfoRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_BIND_INFO, request);
}

void SGRewardLayer::showGetLvlUpGold()
{
    main::RewardLevelUpEntryRequest *request = new main::RewardLevelUpEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LVLUPENTER, request);
}

void SGRewardLayer::showYueKaReward()
{
    main::MonthlyCardShowRequest *request = new main::MonthlyCardShowRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_YUEKA, request);
}
void SGRewardLayer::yueKaEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::MonthlyCardShowResponse *response = (main::MonthlyCardShowResponse *)sr->m_msg;
        if (response != NULL)
        {
            int yueKaListCount = response->info_size();
            CCArray *arr = CCArray::create();
            for(int i=0;i<yueKaListCount;i++)
            {
                main::MonthlyCardShowInfo yueKainfo = response->info(i);
                arr->addObject(CCString::createWithFormat("%d - %d - %d - %d",yueKainfo.type(),yueKainfo.gold(),yueKainfo.state(),yueKainfo.invalid()));
            }
            SGMainManager::shareMain()->showYueKaRewardLayer(arr);
        }
        
    }
}

//void SGRewardLayer::lvlUpEnterListener(cocos2d::CCObject *obj)
//{
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//        if (sr)
//    {
//        main::RewardLevelUpEntryResponse *response = (main::RewardLevelUpEntryResponse *)sr->m_msg;
//        CCArray *array = CCArray::create();
//        
//        int len = response->records_size();
//        for (int i = 0; i<len; i++)
//        {
//            array->addObject(CCString::createWithFormat("%d",response->records(i)));
//        }
//        SGMainManager::shareMain()->showLvlUpGetGold(array);
//    }
//}
void SGRewardLayer::showGetSalary()
{
    main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_SALARYENTER, request);
}
void SGRewardLayer::salaryEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::RewardSalaryEntryResponse *response = (main::RewardSalaryEntryResponse *)sr->m_msg;
    int levelCoin = response->levelcoin();
	//int canGetlevelCoin = response->cangetlevelcoin();
	int levelRewardTime = response->levelrewardtime();
	int pvpLevel = response->pvplevel();
	int pvpLevelCoin = response->pvplevelcoin();
	//int canGetPvPLevelCoin = response->cangetpvplevelcoin();
	int pvpRewardTime = response->pvprewardtime();
    int minLevel = response->pveminlevel();
    int minLevelpvp = response->pvpminlevel();
    SGMainManager::shareMain()->showSalaryLayer(levelCoin,levelRewardTime,pvpLevel,pvpLevelCoin,pvpRewardTime,minLevel,minLevelpvp);
}
void SGRewardLayer::showWine()
{
    main::RewardRecoverPowerEntryRequest *request = new main::RewardRecoverPowerEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_RECOVERENTER, request);

}
void SGRewardLayer::recoverEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
         main::RewardRecoverPowerEntryResponse *response = (main::RewardRecoverPowerEntryResponse *)sr->m_msg;
        int power = response->power();
        int iscan = response->enable();
        SGMainManager::shareMain()->showDrinkWineLayer(power,iscan);
    }   
}
void SGRewardLayer::showHope()
{
    main::RewardGodPrayEntryRequest *request = new main::RewardGodPrayEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GODENTER, request);
}
void SGRewardLayer::showInviter()
{
    main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVER, request);
}
void SGRewardLayer::showInvitee()
{
  SGMainManager::shareMain()->showInviteeLayer();
}
void SGRewardLayer::godEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::RewardGodPrayEntryResponse *response = (main::RewardGodPrayEntryResponse *)sr->m_msg;
        int current = response->current();
        int max = response->max();
        int iscan = response->cando();
       /////////加入可能获得物品的信息们
        CCArray*array=CCArray::create();
        for (int i=0; i<response->godrewardinfo_size(); i++) {
            
            main::GodRewardInfo info =response->godrewardinfo(i);
            
            SGMailSystemAccessoryDataModel* data=new SGMailSystemAccessoryDataModel();
            data->setCount(info.count());
            data->setItemId(info.itemid());
            data->setType(info.type());
            
            
            array->addObject(data);
        }
        
      //////////////////////////////
        
        
        
        
        
        
        SGMainManager::shareMain()->showJitianLayer(current,max,iscan,array);
    }
}
void SGRewardLayer::showliandan()
{
    main::RewardLianDanTimeRequest *request = new main::RewardLianDanTimeRequest();
    request->set_index(select);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LIANDANENTER, request);
}
void SGRewardLayer::liandanEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::RewardLianDanTimeResponse *response = (main::RewardLianDanTimeResponse *)sr->m_msg;
        int time = response->time();
        SGMainManager::shareMain()->showLiandanLayer(time,select);
    }    
}
void SGRewardLayer::showcaishen()
{
    main::RewardRegLotteryEntryRequest *request = new main::RewardRegLotteryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_REGENTER, request);
}
void SGRewardLayer::regEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::RewardRegLotteryEntryResponse *response = (main::RewardRegLotteryEntryResponse *)sr->m_msg;
        int chance = response->leftchance();
        int time = response->lefttime();
        int cost = response->costgold();
        CCArray *bcsArr=CCArray::create();
        bcsArr->retain();
        int count=response->getgolds_size();
        for (int i=0; i<count; i++) {
            
            CCString *str=CCString::createWithFormat("%s",SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString());
          
            bcsArr->addObject(str);
            bcsArr->addObject(CCString::createWithFormat("%s",response->getgolds(i).c_str()));
        }
        
        SGMainManager::shareMain()->showRegLayer(chance, time, cost,bcsArr);
    }
}

void SGRewardLayer::inviterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::InviteDetailResponse *response = (main::InviteDetailResponse *)sr->m_msg;
        if (response != NULL)
        {
            CCString* inviteCode = CCString::create(response->invitecode());//发送邀请的邀请码
            int inviteNum = response->invitenum();//已邀请
            int inviteMax = response->invitemax();   //最都邀请
            
            int rewardsListCount = response->rewards_size();//奖励列表
            CCArray *arr = CCArray::create();
            for(int i=0;i<rewardsListCount;i++)
            {
                main::InviteRewardMessage rewardinfo = response->rewards(i);
                arr->addObject(CCString::createWithFormat("%d - %d - %d - %d - %d",rewardinfo.num(),rewardinfo.itemtype(),
                                                          rewardinfo.itemid(), rewardinfo.itemnum(), rewardinfo.state()));
            }
            SGMainManager::shareMain()->showInviterLayer(inviteCode,inviteNum,inviteMax,arr);
        }

    }
}

void SGRewardLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}

void SGRewardLayer::hideInviteeItem()
{

}

void SGRewardLayer::hideBindItem()
{


}

void SGRewardLayer::showMonthlySignRequest()
{
    //MMDBG: 偽造數據
    /*
    SGSocketRequest* sreq = new SGSocketRequest();
    
    main::SigninDetailResponse* sdresp = new main::SigninDetailResponse();
    sdresp->set_month(12);
    sdresp->set_day(4);
    main::SigninCell* sc = NULL;
    
    for (int k = 0; k < 24; k++)
    {
        sc = sdresp->add_cells();
        main::ItemDataProto* idp = sc->mutable_itemdata();
        idp->set_itemtype(6);
        idp->set_itemid(1021);
        idp->set_itemnum(1234);
        idp->set_itemlv(20);

        int md = k % 4;
        main::SignState ss;
        if (md == 0)
            ss = main::NORMAL_HAS;
        else if (md == 1)
            ss = main::NORMAL_NO;
        else if (md == 2)
            ss = main::NORMAL_CAN;
        else if (md == 3)
            ss = main::VIP_HALF;
        
        sc->set_state(ss);
        sc->set_showstring("V10 双倍");
        sc->set_niubility(1);
        sc->set_itemdesc("19/#79/#年，那是一個春天啊！");
        sc->set_vipdesc("有一位传说的/#VIP1小平同志/#，在中國的南海邊画了一个圈");
    }
    
    sreq->m_msg =sdresp;
    showMonthlySignResponse(sreq);
    
    CC_SAFE_DELETE(sreq);
    return;
     */
    
    main::SigninDetailRequest *request = new main::SigninDetailRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_MONTHLY_SIGN_MAIN_INFO, request);
}

void SGRewardLayer::showMonthlySignResponse(CCObject* obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SigninDetailResponse *resp = dynamic_cast<main::SigninDetailResponse *>(sr->m_msg);
        if (resp)
        {
            main::SigninDetailResponse* sdr = new main::SigninDetailResponse();
            sdr->CopyFrom(*resp);
            
            SGMonthlySignLayer* sgmsl = SGMonthlySignLayer::create(sdr);
            SGMainManager::shareMain()->getMainScene()->addShowLayer(sgmsl);
        }
    }
}


