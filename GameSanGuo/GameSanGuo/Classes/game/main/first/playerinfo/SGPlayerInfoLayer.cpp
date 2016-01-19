//
//  SGPlayerInfoLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-21.
//
//

#include "SGPlayerInfoLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "ShopModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGChangeNickName.h"
#include "SGCantAdvanceBox.h"
#include "PvpFight.pb.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "SGStringConfig.h"
#include "RoleShopModule.pb.h"
#include "SGMallLayer.h"
USING_NS_CC_EXT;

SGPlayerInfoLayer::SGPlayerInfoLayer():_vipGifts(NULL),
tili(NULL),           //体力
label_junliang(NULL), //军粮
huifu(NULL),          //军粮回复
qhuifu(NULL),         //军粮全部回复
hf(NULL),             //体力回复
qhf(NULL),            //体力全部回复
menu1(NULL),
scroll(NULL)
{
    
}
SGPlayerInfoLayer::~SGPlayerInfoLayer()
{
    /*
    CCSpriteFrameCache::sharedSpriteFrameCache() -> removeSpriteFramesFromFile("public/public0.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache() -> removeSpriteFramesFromFile("public/publicOther.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache() -> removeSpriteFramesFromFile("mainLayer/mainLayerFirst.plist");
     */
    
    tili = NULL;
    label_junliang = NULL;
    huifu = NULL;
    qhuifu = NULL;
    hf = NULL;
    qhf = NULL;
    menu1 = NULL;
    scroll = NULL;
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_playerInfoLayer);
  

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->removeObserver(this, MSG_STORE_ENTER);
    notification->removeObserver(this, MSG_EXCHANGE_ENTER);
//    notification->removeObserver(this, MSG_VIPGIFT_BAGS);
    notification->removeObserver(this, MSG_SEASONWAR_CDLIST);
}
SGPlayerInfoLayer *SGPlayerInfoLayer::create()
{
    SGPlayerInfoLayer *cardsLayer = new SGPlayerInfoLayer();
    if (cardsLayer && cardsLayer->init(NULL, sg_playerInfoLayer))
    {
        cardsLayer->initView();
        cardsLayer->autorelease();
        return cardsLayer;
    }
    CC_SAFE_DELETE(cardsLayer);
    return NULL;
}
void SGPlayerInfoLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_playerInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_playerInfoLayer);
     ResourceManager::sharedInstance()->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_LAYER_UI, sg_playerInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_playerInfoLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    //整合到
//    notification->addObserver(MSG_STORE_ENTER,
//                              this,
//                              callfuncO_selector(SGPlayerInfoLayer::storeEnterListener));
    notification->addObserver(MSG_EXCHANGE_ENTER,
                              this,
                              callfuncO_selector(SGPlayerInfoLayer::exchangeEnterListener));
    notification->addObserver(MSG_SEASONWAR_CDLIST, this, callfuncO_selector(SGPlayerInfoLayer::pvpexchangeEnterListener));
//    notification->addObserver(MSG_VIPGIFT_BAGS,
//                              this,
//                              callfuncO_selector(SGPlayerInfoLayer::vipGiftBagListener));

    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
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

    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_playerInfoLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    //CCRect r = CCRectMake(0, 1136/2 - (s.height - title_bg->getContentSize().height - btmhgt*.68)/2, bg->getContentSize().width,s.height - title_bg->getContentSize().height - btmhgt*.68 );
    //bg->setTextureRect(r);
    //bg->setScaleY(r.size.height / bg->getContentSize().height);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0)); // debug by：zyc   ，盖住聊天按钮的图片。
    //bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt*.68)));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,112)));
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGPlayerInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));

//    CCSprite *title = CCSprite::createWithSpriteFrameName("public_playerinfo_title.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_PlayerInfoLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
    scroll = CCScrollView::create(CCSizeMake(s.width, s.height - btmhgt*.68 - title_bg->getContentSize().height - 10));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(scroll);
    scroll->setPosition(ccp(0, btmhgt*.68 + 5));
    CCLayer *scrollViewContainer = CCLayer::create();
    menu1 = CCMenu::create();
    menu1->setPosition(CCPointZero);
    scrollViewContainer->addChild(menu1,10);

    int height = 0;
    CCSprite *item = NULL;
    for (int i = 0 ; i<3; i++) {
        if (i == 0) {
            item = CCSprite::createWithSpriteFrameName("playerinfo_diban1.png");
        }else if(i==1)
        {
            item = CCSprite::createWithSpriteFrameName("playerinfo_diban2.png");
        }else
        {
            item = CCSprite::createWithSpriteFrameName("playerinfo_diban3.png");
        }

            item->setAnchorPoint(ccp(0, 0));
            scrollViewContainer->addChild(item);
            
            if (i == 0) {
                item->setPosition(ccp(s.width/2 - item->getContentSize().width/2,height));
                std::string serverName = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
                CCLabelTTF *fwq = CCLabelTTF::create(serverName.c_str(), FONT_PANGWA, 26);
                scrollViewContainer->addChild(fwq);
                fwq->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.5, item->getContentSize().height*.5)));
            }else if (i == 1) {
                item->setPosition(ccp(s.width/2 - 282.5,height));
                
                CCSprite * a = CCSprite::createWithSpriteFrameName("playerinfo_diban2.png");
                a->setAnchorPoint(ccp(1, 0));
                scrollViewContainer->addChild(a);
                a->setPosition(ccp(s.width/2 + 282.5,height));
                
                CCSprite *curr_jl = CCSprite::createWithSpriteFrameName("font_dqjl.png");
                curr_jl->setAnchorPoint(ccp(0, 0.5));
                curr_jl->setPosition(ccpAdd(a->getPosition(), ccp(20 - a->getContentSize().width,175)));
                scrollViewContainer->addChild(curr_jl);
                
                //军粮
                int limitJunliang = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("13"))->objectForKey("value"))->intValue();
                label_junliang = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",playInfo->getPvpPower(),limitJunliang)->getCString(),FONT_PANGWA, 26);//5改30
                label_junliang->setPosition(ccpAdd(curr_jl->getPosition(), ccp(155, 0)));
                label_junliang->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(label_junliang);
                
                huifu = SGCCLabelTTF::create("00:00:00", FONT_PANGWA, 22);
                scrollViewContainer->addChild(huifu);
                huifu->setPosition(ccpAdd(a->getPosition(), ccp(195 - a->getContentSize().width,123)));
                
                qhuifu = SGCCLabelTTF::create("20:20:59", FONT_PANGWA, 22);
                scrollViewContainer->addChild(qhuifu);
                qhuifu->setPosition(ccpAdd(a->getPosition(), ccp(195 - a->getContentSize().width,83)));
                
                SGButton *relife =SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str2, this, menu_selector(SGPlayerInfoLayer::recoverPvpPower),CCPointZero,FONT_PANGWA,ccWHITE);
                
                relife->setPosition(ccpAdd(a->getPosition(), ccp(210 - a->getContentSize().width,40)));
                menu1->addChild(relife);
                
                CCSprite *curr_tl = CCSprite::createWithSpriteFrameName("font_currtili.png");
                scrollViewContainer->addChild(curr_tl);
                curr_tl->setAnchorPoint(ccp(0,0.5));
                curr_tl->setPosition(ccpAdd(item->getPosition(), ccp(20,175)));
                
                //体力
                int nTiliMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(playInfo->getPlayerLevel())->getPlayerPower();//获得当前等级最大体力值
                tili = CCLabelTTF::create(CCString::createWithFormat("%d/%d",playInfo->getPlayerPower(),nTiliMax)->getCString(),FONT_PANGWA, 26);
                scrollViewContainer->addChild(tili);
                tili->setAnchorPoint(ccp(0, 0.5));
                tili->setPosition(ccpAdd(curr_tl->getPosition(), ccp(120, 0)));
                
                hf = SGCCLabelTTF::create("00:09:55", FONT_PANGWA, 22);
                scrollViewContainer->addChild(hf);
                hf->setPosition(ccpAdd(item->getPosition(), ccp(195 ,123)));
                qhf = SGCCLabelTTF::create("20:20:59", FONT_PANGWA, 22);
                scrollViewContainer->addChild(qhf);
                qhf->setPosition(ccpAdd(item->getPosition(), ccp(195 ,83)));
                
                SGButton *rejl  =SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str2, this, menu_selector(SGPlayerInfoLayer::recoverPower),CCPointZero,FONT_PANGWA,ccWHITE);
                rejl->setPosition(ccpAdd(item->getPosition(), ccp(210,40)));
                menu1->addChild(rejl);
                
            }
            else
            {
                item->setPosition(ccp(s.width/2 - item->getContentSize().width/2,height));
                
                //战斗力
                SGCCLabelTTF *combatle=SGCCLabelTTF::create(CCString::createWithFormat("%d",playInfo->getcombatValue())->getCString(), FONT_PANGWA, 26,CCSizeZero ,kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                combatle->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(combatle);
                combatle->setPosition(ccpAdd(item->getPosition(), ccp(205, 59)));
                //combatle->setPosition(ccpAdd(pvpLvl->getPosition(), ccp(0,59)));
                
                //统御力 ladygaga
                
                SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playInfo->getPlayerLevel());
                SGCCLabelTTF *Governing=SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",GetCurrentTYL(),player->getPlayerGoverning() + SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering())->getCString(), FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                Governing->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(Governing);
                Governing->setPosition(ccpAdd(combatle->getPosition(), ccp(0,59)));
                
                
                // 1 大都督
                // 2 左都督
                // 3 右都督
                // 4~10 卫将军
                // 11~50 左将军
                // 51~100 右将军
                // 101~200 左校尉
                // 201~500 右校尉
                // 501~+∞平民   0也是平民
                
                int RankNum=playInfo->getMilitaryRank();
                int RankLevel=1;
                CCSprite   *militaryRank = NULL; //ranknum must >=0
                if (RankNum>=0)
                {
                    if (RankNum==1)
                    {
                        RankLevel=9;
                    }
                    else if(RankNum==2)
                    {
                        RankLevel=8;
                    }
                    else if(RankNum==3)
                    {
                        RankLevel=7;
                    }
                    else if(RankNum>501 ||  RankNum==0)
                    {
                        RankLevel=1;
                    }
                    else if(RankNum>=201)
                    {
                        RankLevel=2;
                    }else if(RankNum>=101)
                    {
                        RankLevel=3;
                    }
                    else if(RankNum>=51)
                    {
                        RankLevel=4;
                    }else if(RankNum>=11)
                    {
                        RankLevel=5;
                    }else if(RankNum>=4)
                    {
                        RankLevel=6;
                    }
                    
                    
                    CCString * jxStr=CCString::createWithFormat("ph%d.png",RankLevel);
                    militaryRank=CCSprite::createWithSpriteFrameName(jxStr->getCString());
                    scrollViewContainer->addChild(militaryRank);
                    militaryRank->setAnchorPoint(ccp(0, 0.5f));
                    //militaryRank->setPosition(ccpAdd(item->getPosition(), ccp(210, 40)));
                    militaryRank->setPosition(ccpAdd(Governing->getPosition(), ccp(-12, 59)));
                    
                }
                
                SGCCLabelTTF *rankName = SGCCLabelTTF::create(playInfo->getMilitaryRankName()->getCString(), FONT_PANGWA, 26);
                rankName->setAnchorPoint(ccp(0, 0.5));
                // scrollViewContainer->addChild(rankName);
                rankName->setPosition(ccpAdd(item->getPosition(), ccp(180, 40)));
                
                
                CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
                CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("1");
                int nJungong =dic1->valueForKey("value")->intValue();
                CCDictionary *dic2 =(CCDictionary *) dic->objectForKey("2");
                int nJifen =dic2->valueForKey("value")->intValue();
                CCDictionary * dic3 = (CCDictionary*)dic->objectForKey("3");
                int junHunUp = dic3->valueForKey("value")->intValue();
                
                //  积分
                SGCCLabelTTF *pvpNum=SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",playInfo->getplayerPvpNum(),nJifen)->getCString(), FONT_PANGWA, 26, CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                pvpNum->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(pvpNum);
                //pvpNum->setPosition(ccpAdd(rankName->getPosition(), ccp(-5, 47)));
                pvpNum->setPosition(ccpAdd(militaryRank->getPosition(), ccp(12, 59)));
                SGButton *huanli = SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str4, this, menu_selector(SGPlayerInfoLayer::pvpexchang),CCPointZero,FONT_PANGWA,ccWHITE);
                menu1->addChild(huanli);
                //huanli->setPosition(ccpAdd(pvpNum->getPosition(), ccp(300, 0)));
                huanli->setPosition(ccpAdd(pvpNum->getPosition(), ccp(255, 0)));
                
                //天梯等级
                //SGCCLabelTTF *pvpLvl = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,playInfo->getplayerPvpLevel())->getCString(), FONT_PANGWA, 26);
                SGCCLabelTTF *pvpLvl = SGCCLabelTTF::create(CCString::createWithFormat("%d",playInfo->getplayerPvpLevel())->getCString(), FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                pvpLvl->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(pvpLvl);
                pvpLvl->setPosition(ccpAdd(pvpNum->getPosition(), ccp(0, 59)));

               
                //拥有军魂
                SGCCLabelTTF *junHun = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",playInfo->getJunHun(),junHunUp)->getCString(), FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
                junHun->setAnchorPoint(ccp(0, 0.5));
                //havejung->setPosition(ccpAdd(Governing->getPosition(), ccp(0, 59)));
                junHun->setPosition(ccpAdd(pvpLvl->getPosition(), ccp(0, 59)));
                scrollViewContainer->addChild(junHun);
                
                SGButton *junHunExchange = SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str4, this, menu_selector(SGPlayerInfoLayer::junHunBtnClick),CCPointZero,FONT_PANGWA,ccWHITE);
                menu1->addChild(junHunExchange);
                junHunExchange->setPosition(ccpAdd(junHun->getPosition(), ccp(255, 0)));
                
                //拥有铜钱
                //modify by:zyc . 添加铜钱图标
                CCSprite * iconCoin = CCSprite::createWithSpriteFrameName("coinicon.png");
                iconCoin->setAnchorPoint(ccp(0, 0.5f));
                iconCoin->setPosition(ccpAdd(junHun->getPosition(), ccp(-35,60) ) );
                scrollViewContainer->addChild(iconCoin);
                //end modify.
                SGCCLabelTTF *haveCoin = SGCCLabelTTF::create(CCString::createWithFormat("%d",playInfo->getPlayerCoins())->getCString(), FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
                haveCoin->setAnchorPoint(ccp(0, 0.5));
                haveCoin->setPosition(ccpAdd(junHun->getPosition(), ccp(10, 60)));
                scrollViewContainer->addChild(haveCoin);
                
                SGButton* duihuan = SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str5, this, menu_selector(SGPlayerInfoLayer::showExchangeLayer),CCPointZero,FONT_PANGWA,ccWHITE);
                menu1->addChild(duihuan);
                duihuan->setPosition(ccp(huanli->getPosition().x,haveCoin->getPosition().y+5));
                
                //元宝
                //modify by:zyc . 添加元宝图片
                CCSprite * iconYuanBao = CCSprite::createWithSpriteFrameName("store_yb.png");
                iconYuanBao->setAnchorPoint(ccp(0,0.5f));
                iconYuanBao->setPosition(ccpAdd(iconCoin->getPosition() , ccp(0, 60) ) );
                scrollViewContainer->addChild(iconYuanBao);
                //end.
                
                havegold = SGCCLabelTTF::create(CCString::createWithFormat("%d",playInfo->getPlayerGold())->getCString(), FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                havegold->setAnchorPoint(ccp(0, 0.5));
                havegold->setPosition(ccpAdd(haveCoin->getPosition(), ccp(0, 60)));
                scrollViewContainer->addChild(havegold);
                
                SGButton* chongzhi = NULL;
                if (playInfo->getHaveCharged()) {
                    chongzhi = SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str6, this, menu_selector(SGPlayerInfoLayer::showStoreLayer),CCPointZero,FONT_PANGWA,ccWHITE);
                }
                else
                {
                    chongzhi = SGButton::create("firstCharge1.png", NULL, this, menu_selector(SGPlayerInfoLayer::showStoreLayer));
                    CCSprite * chongzhiGuan = CCSprite::createWithSpriteFrameName("firstChargeGuang.png");
                    if (chongzhiGuan) {
                        chongzhiGuan->setAnchorPoint(ccp(0.5,0.5));
                        chongzhiGuan->ignoreAnchorPointForPosition(false);
                        chongzhiGuan->setPosition(ccp(chongzhi->getContentSize().width/2,chongzhi->getContentSize().height/2));
                        chongzhiGuan->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
                        chongzhi->addChild(chongzhiGuan,-10,-10);
                    }
                    
                    chongzhi->setScale(0.8);
                }

                menu1->addChild(chongzhi);
                //chongzhi->setPosition(ccp(huanli->getPosition().x,havegold->getPosition().y+5));
                chongzhi->setPosition(ccp(huanli->getPosition().x,havegold->getPosition().y ));
                
                //角色等级部分
                SGCCLabelTTF *level = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_ji,playInfo->getPlayerLevel())->getCString(), FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                level->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(level);
                level->setPosition(ccpAdd(junHun->getPosition(), ccp(2, 195)));
                
                //经验
                int currLevelExperienceMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(playInfo->getPlayerLevel())->getPlayerExp();//当前等级最大经验
                SGCCLabelTTF *exp = SGCCLabelTTF::create(CCString::createWithFormat(str_PlayerInfoLayer_str7,playInfo->getPlayerExp(),currLevelExperienceMax)->getCString(), FONT_PANGWA, 20 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
                exp->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(exp);
                exp->setPosition(ccpAdd(level->getPosition(), ccp(100, 0)));
                
                CCSprite *jindu = CCSprite::createWithSpriteFrameName("jingyantiaobg.png");
                //太长
                jindu->setScaleX(0.9f);
                scrollViewContainer->addChild(jindu);
                //jindu->setPosition(ccpAdd(exp->getPosition(), ccp(65, -22)));
                jindu->setPosition(ccpAdd(exp->getPosition(), ccp(55, -22)));
                
                CCProgressTimer *progress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("jingyantiao1.png"));
                progress->setScaleX(0.9f);
                progress->setType(kCCProgressTimerTypeBar);
                progress->setMidpoint(ccp(0,0));
                progress->setBarChangeRate(ccp(1, 0));
                scrollViewContainer->addChild(progress);
                //progress->setPosition(ccpAdd(jindu->getPosition(), ccp(18, 0)));
                progress->setPosition(jindu->getPosition() );	
                float rate = (float)playInfo->getPlayerExp()/currLevelExperienceMax;
                progress->setPercentage(rate*100);
                
                //vip等级
                
                //            SGCCLabelTTF *viplvl = SGCCLabelTTF::create(CCString::createWithFormat("VIP%d",playInfo->getplayerVipLevel())->getCString(), FONT_PANGWA, 26);
                //            viplvl->setAnchorPoint(ccp(0, 0.5));
                //            scrollViewContainer->addChild(viplvl);
                //            viplvl->setPosition(ccpAdd(level->getPosition(), ccp(0, 50)));
                
                if (SGPlayerInfo::sharePlayerInfo()->getShowvip())
                {
                    CCSprite *vipitem = CCSprite::createWithSpriteFrameName("playerinfo_vip.png");
                    //vipitem->setPosition(ccpAdd(item->getPosition(), ccp(85,565)));
                    vipitem->setPosition(ccpAdd(level->getPosition() , ccp(-110,48) ));
                    scrollViewContainer->addChild(vipitem);
                }

                
                CCString *vip;
                CCSprite *vipSprite;
                if (playInfo->getplayerVipLevel()>0)
                {
                    vip=CCString::createWithFormat("vip%d.png",playInfo->getplayerVipLevel());
                    vipSprite=CCSprite::createWithSpriteFrameName(vip->getCString());
                    if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
                        vipSprite->setVisible(true);
                    }
                    else
                    {
                        vipSprite->setVisible(false);
                    }
                }
                else
                {
                    vipSprite=CCSprite::createWithSpriteFrameName("vip1.png");
                    vipSprite->setVisible(false);
                    
                }
                
                scrollViewContainer->addChild(vipSprite);
                vipSprite->setPosition(ccpAdd(level->getPosition(), ccp(50, 50)));
                
                SGButton* special = SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str8, this, menu_selector(SGPlayerInfoLayer::showvipinfo),CCPointZero,FONT_PANGWA,ccWHITE);
                if (SGPlayerInfo::sharePlayerInfo()->getShowvip())
                {
                    menu1->addChild(special);
                }
                
                special->setPosition(ccp(huanli->getPosition().x,vipSprite->getPosition().y-1));
                
                //国家
                SGCCLabelTTF *label = SGCCLabelTTF::create(str_FateInfoItem_str7, FONT_PANGWA, 26 , CCSizeZero , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
                if (playInfo->getPlayerCountry() == 1)
                {
                    label->setString("武士");
                }
                if (playInfo->getPlayerCountry() == 2)
                {
                    label->setString("武士");
                }
                if (playInfo->getPlayerCountry() == 3)
                {
                    label->setString("武士");
                }
                label->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(label);
                label->setPosition(ccpAdd(vipSprite->getPosition(), ccp(-50, 60)));
                //特性
                SGButton* texing = SGButton::createFromLocal("btn_fenbg.png", str_PlayerInfoLayer_str9, this, menu_selector(SGPlayerInfoLayer::showCountrysInfo),CCPointZero,FONT_PANGWA,ccWHITE);
                menu1->addChild(texing);
                texing->setPosition(ccp(huanli->getPosition().x,label->getPosition().y-2));
                //昵称 ID
                int flength=playInfo->getNickName()->length();
                
                unsigned int flen = flength / 3 + flength % 3;
                int fontSize =26;
                if (flen>7)
                {
                    fontSize=24;
                }
                SGCCLabelTTF *name = SGCCLabelTTF::create(playInfo->getNickName()->getCString(), FONT_PANGWA, fontSize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                
                name->setAnchorPoint(ccp(0, 0.5));
                scrollViewContainer->addChild(name);
                //name->setPosition(ccpAdd(label->getPosition(), ccp(-70, 57)));
                //debug by:zyc. 上下对齐
                name->setPosition(ccpAdd(label->getPosition(), ccp(0, 57)));
                
                SGCCLabelTTF *playerID = SGCCLabelTTF::create(CCString::createWithFormat(str_PlayerInfoLayer_str10,playInfo->getInvitationCode()->getCString())->getCString(), FONT_PANGWA, 26 , CCSizeZero  , kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter );
                //scrollViewContainer->addChild(playerID);
                playerID->setAnchorPoint(ccp(1, 0.5));
                playerID->setPosition(ccpAdd(name->getPosition(), ccp(443, 0)));
                
                
            }// end else.
            height += item->getContentSize().height + skewing(10);
        
    }//end for.
    
    scroll->setContainer(scrollViewContainer);
    
    scrollViewContainer->setContentSize(CCSizeMake(s.width, height));

    scroll->setContentSize(CCSizeMake(s.width,height ));
    
    scroll->setContentOffset(ccp(0, -(height+ skewing(10) -10- (s.height - btmhgt*.68 - title_bg->getContentSize().height))));
    
    
    
        
//    CCSprite *protait = CCSprite::create("mainLayer/font_portait.png");
//    scrollViewContainer->addChild(protait);
//    protait->setAnchorPoint(ccp(0, 0.5));
//    protait->setPosition(ccp(layer, country->getPosition().y));
//    
//    SGButton* change = SGButton::create("mainLayer/btn_lanbg.png",
//                                         "mainLayer/font_genghuan.png",
//                                         this,
//                                         NULL,
//                                         ccp(0, 0),
//                                         true,
//                                         false);
//    menu1->addChild(change);
//    change->setAnchorPoint(ccp(1, 0.5));
//    change->setPosition(ccp(chongzhi->getPosition().x, country->getPosition().y));
    
    
//    SGButton* xiugai = SGButton::create("mainLayer/btn_lanbg.png",
//                                        "mainLayer/mainlayer_xiugai.png",
//                                        this,
//                                        menu_selector(SGPlayerInfoLayer::changNickname),
//                                        ccp(0, 0),
//                                        true,
//                                        false);
//    menu1->addChild(xiugai);
//    xiugai->setAnchorPoint(ccp(1, 0.5));
//    xiugai->setPosition(ccp(chongzhi->getPosition().x, name->getPosition().y));
    if (SGMainManager::shareMain()->getMainLayer())
    {
        SGMainManager::shareMain()->getMainLayer()->updateLayerData();
    }
    
}
void SGPlayerInfoLayer::showStoreLayer()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (SGMainManager::shareMain()->getMainLayer()) {
        SGMainManager::shareMain()->getMainLayer()->requestStoreLayer();
    }
}

void SGPlayerInfoLayer::showExchangeLayer()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::ShopExchangeCoinEntryRequest *request = new main::ShopExchangeCoinEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_ENTER, request);
}
void SGPlayerInfoLayer::exchangeEnterListener(cocos2d::CCObject *obj)
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

void SGPlayerInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
//    SGMainManager::shareMain()->showLayer(SGMainManager::shareMain()->getNowShowLayer());
//    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_playerInfoLayer, true);
}
void SGPlayerInfoLayer::test()
{
    CCLOG("<<<<<<");
}
void SGPlayerInfoLayer::showCountrysInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showCountriesLayer();
}

void SGPlayerInfoLayer::updateInfo(const char *tiliLeftTime,const char *tiliTime,const char *junliangLeftTime,const char*junliangTime)
{
    int pvePower = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();   //体力
    int pvpPower = SGPlayerInfo::sharePlayerInfo()->getPvpPower();      //军粮
    int pvePowerMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())->getPlayerPower();    //最大体力值

    char tiliBuf[16];
    sprintf(tiliBuf, "%d/%d",pvePower,pvePowerMax);
    
    if (tili)
    {
        tili->setString(tiliBuf);
    }
    char junliangBuf[16];
    int limitJunliang = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("13"))->objectForKey("value"))->intValue();
    sprintf(junliangBuf, "%d/%d",pvpPower,limitJunliang);//5改30
    if (label_junliang && huifu && qhuifu && hf && qhf)
    {
        label_junliang->setString(junliangBuf);
        
//        huifu->setVisible(pvpPower != limitJunliang);//5改30
//        qhuifu->setVisible(pvpPower != limitJunliang);
//        hf->setVisible(pvePower != pvePowerMax);
//        qhf->setVisible(pvePower != pvePowerMax);
        
        if (pvpPower >= limitJunliang)
        {
            huifu->setString("00:00:00");
            qhuifu->setString("00:00:00");
        }
        else
        {
            huifu->setString(junliangLeftTime);
            qhuifu->setString(junliangTime);
        }
        
        if (pvePower >= pvePowerMax)
        {
            hf->setString("00:00:00");
            qhf->setString("00:00:00");
        }
        else
        {
            hf->setString(tiliLeftTime);
            qhf->setString(tiliTime);
        }
        
//        huifu->setString(junliangLeftTime);
//        qhuifu->setString(junliangTime);
//        hf->setString(tiliLeftTime);
//        qhf->setString(tiliTime);
    }
    

}
void SGPlayerInfoLayer::changNickname()
{
    SGChangeNickName *nikeNameBox = SGChangeNickName::create(this, 0);
    SGMainManager::shareMain()->showBox(nikeNameBox);
}

void SGPlayerInfoLayer::recoverPower()
{
    EFFECT_PLAY(MUSIC_BTN);
    int pvePower = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();   //体力
    int pvePowerMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())->getPlayerPower();    //最大体力值
    if (pvePower < pvePowerMax) {
        main::ShopPurchPvEPowerEntryRequest *request = new main::ShopPurchPvEPowerEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_PURCHPVEPOWE_RENTER, request);
    }else
    {
        SGMainManager::shareMain()->showMessage(str_PlayerInfoLayer_str11);
    }
}

void SGPlayerInfoLayer::recoverPvpPower()
{
    EFFECT_PLAY(MUSIC_BTN);
    int pvpPower = SGPlayerInfo::sharePlayerInfo()->getPvpPower();
    int limitJunliang = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("13"))->objectForKey("value"))->intValue();
    if (pvpPower < limitJunliang) {//5改30
        main::ShopPurchPvPPowerEntryRequest *request = new main::ShopPurchPvPPowerEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_PURCHPVPPOWE_RENTER, request);
    }else
    {
        SGMainManager::shareMain()->showMessage(str_PlayerInfoLayer_str12);
    }
}
void SGPlayerInfoLayer::pvpexchang()
{
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_PVP , PALYER_INFO_LAYER);
}
void SGPlayerInfoLayer::pvpexchangeEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::PvpShopListResponse *response = (main::PvpShopListResponse *)req->m_msg;
    if (response) {
        CCArray *datas = CCArray::create();
        datas->retain();
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
        SGMainManager::shareMain()->showChgCreditCardLayer(datas,0,2);
    }

}
void SGPlayerInfoLayer::showvipinfo()
{
    EFFECT_PLAY(MUSIC_BTN);
//    SGMainManager::shareMain()->showVipInfoLayer(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel(),1);
    main::ShopVipGiftBagEntryRequest *request = new main::ShopVipGiftBagEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VIPGIFT_BAGS, request);
    
}
void SGPlayerInfoLayer::vipGiftBagListener(cocos2d::CCObject *obj)
{
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopVipGiftBagEntryResponse *response = (main::ShopVipGiftBagEntryResponse *)sr->m_msg;
    int vipLvl = 0;
    if (_vipGifts) {
        _vipGifts->release();
        _vipGifts = NULL;
    }
    _vipGifts = CCArray::create();
    if (sr)
    {
        vipLvl = response->viplevel();
        int curr = response->currentrmb();
        int max = response->maxrmb();
        int len = response->list_size();
        for (int i = 0 ; i <len; i++)
        {
            SGVipGift *vipGift = new SGVipGift();
            main::VipGiftBag gifts = response->list(i);
            vipGift->setGiftCost(gifts.cost());
            vipGift->setGiftID(gifts.ssid());
            vipGift->setGiftName(CCString::create(gifts.name()));
            vipGift->setGiftInfo(CCString::create(gifts.info()));
            vipGift->setGiftDiscount(gifts.discount());
            vipGift->setGiftVipLvl(gifts.viplevel());
//            vipGift->setGiftAward(gifts.giftaward());
//            vipGift->setGiftSpeak(gifts.speaker());
            vipGift->setHaveBuy(gifts.isbuy());
            
//            if (gifts.giftcoin())
//            {
//                SGGift *gift = new SGGift;
//                gift->setItemId(9999);
//                gift->setIconid(9999);
//                gift->setNum(gifts.giftcoin());
//                gift->setStar(0);
//                gift->setName(CCString::create(str_Tong_money));
//                vipGift->addGiftList(gift);
//                gift->autorelease();
//            }
//            if (gifts.liandan())
//            {
//                SGGift *gift = new SGGift;
//                gift->setItemId(9996);
//                gift->setIconid(9996);
//                gift->setNum(gifts.liandan());
//                gift->setStar(0);
//                gift->setName(CCString::create(str_liandianmifang));
//                vipGift->addGiftList(gift);
//                gift->autorelease();
//            }

            int lenth = gifts.list_size();
            for (int j = 0; j <lenth; j ++)
            {
                SGGift *gift = new SGGift;
                main::GiftItem GiftItem = gifts.list(j);
                gift->setItemId(GiftItem.itemid());
                gift->setNum(GiftItem.num());
                gift->setType(GiftItem.itemtype());
                int enumType = (SGBaseItemType)(GiftItem.itemtype() );
                switch (enumType)
                {
                    case BIT_INGOT:
                    {
                        gift->setName(CCString::create(str_yuanbao));
                        gift->setItemId(10000);
                    }
                        break;
                    case BIT_GOLD:
                    {
                        gift->setName(CCString::create(str_Tong_money) );
                        gift->setItemId(9999);
                    }
                        break;
                    case BIT_GRADE:
                    {
                        gift->setName(CCString::create(str_Currency_TianTiJiFen));
                        gift->setItemId(9995);
                    }
                        break;
                    case BIT_RECIPE:
                    {
                        gift->setName(CCString::create(str_Small_laba));
                        gift->setItemId(9996);
                    }
                        break;
                    case BIT_OFFICER:
                    {
                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(GiftItem.itemid());
                        gift->setName(temp->getOfficerName());
                        gift->setItemId(temp->getOfficerId());
                    }
                        
                        break;
                    case BIT_EQUIP:
                    {
                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(GiftItem.itemid());
                        gift->setName(temp->getEquipName());
                        gift->setItemId(temp->getEquipmentId());
                    }
                        
                        break;
                    case BIT_PROP:
                    {
                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(GiftItem.itemid());
                        gift->setName(temp->getPropsName());
                        gift->setItemId(temp->getPropsId());
                        
                    }
                        break;
                    case BIT_PIECE:
                    {
                        SGPiecesDataModel * temp = SGStaticDataManager::shareStatic()->getPiecesDictById(GiftItem.itemid() );
                        gift->setName(temp->getPiecesName());
                        gift->setItemId(temp->getPiecesId());
                    }
                        break;
                    case BIT_SOUL:
                    {
                        gift->setName(CCString::create(str_Currency_JunHun));
                        gift->setItemId(9994);
                    }
                        break;
                    case BIT_VIGOUR:
                    {
                        gift->setName(CCString::create(str_Format_tili__));
                        gift->setItemId(9993);
                    }
                        break;
                    case BIT_GRAIN:
                    {
                        gift->setName(CCString::create(str_Format_junliang__));
                        gift->setItemId(9992);
                    }
                        break;
                    case BIT_CONSUMABLE:
                    {
                        SGConsumableDataModel * temp = SGStaticDataManager::shareStatic()->getConsumableById(GiftItem.itemid());
                        gift->setName(temp->getConsumeName());
                        gift->setItemId(temp->getConsumId());
                    }
                        break;
                    case BIT_MEDAL:
                    {
                        gift->setName(CCString::create(str_Format_xunzhang__));
                        gift->setItemId(9991);
                    }
                        break;
                    case BIT_MATERIAL:
                    {
                        SGMaterialDataModel * temp = SGStaticDataManager::shareStatic()->getMaterialById(GiftItem.itemid());
                        gift->setName(temp->getMaterialName());
                        gift->setItemId(temp->getMaterialId());
                    }
                        break;
                    default:
                        CCAssert(1==0, "error type.");
                        break;
                    
                }

                vipGift->addGiftList(gift);
                gift->autorelease();
            }
            _vipGifts->addObject(vipGift);
            vipGift->autorelease();
        }
        SGMainManager::shareMain()->showGiftsLayer(vipLvl,_vipGifts,curr,max);
    }



}
int  SGPlayerInfoLayer::GetCurrentTYL(){
//    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();
    int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
    CCObject *obj = NULL;
    int mor = 0;
    int speed = 0;
    int  totleGovern = 0;
    CCARRAY_FOREACH(array, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        mor += card->getMorale();
        speed += card->getSpeed();
       totleGovern += card->getGoverning();
        if (card->getOfficerEquips()) {
            for (int i = 0; i <card->getOfficerEquips()->count(); i++) {
                SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
//                mor += equip->getMorale();
//                speed += equip->getSpeed();
                if ((card->getGoverning()+equip->getGoverning())<0) {
                    totleGovern -= card->getGoverning();
                }
                else{
                    totleGovern += equip->getGoverning();
                }
            }
        }
        
    }
    return totleGovern;
}


void SGPlayerInfoLayer::setIsCanTouch(bool isTouch)
{
    isCanTouch = isTouch;
    menu->setTouchEnabled(isTouch);
    menu1->setTouchEnabled(isTouch);
    scroll->setTouchEnabled(isTouch);
}

void SGPlayerInfoLayer::junHunBtnClick()
{
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_JING_ZHOU , PALYER_INFO_LAYER);
}

