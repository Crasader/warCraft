//
//  SGAreanLayer.cpp
//  GameSanGuo
//
//  Created by dbq on 14-12-18.
//
//

#include "SGAreanLayer.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGDrawCards.h"
#include "ArenaModule.pb.h"
#include "SGRankDetailLayer.h"
#include "SGFriend.h"
#include "SGStringConfig.h"
#include "SGHelpBookInfo.h"
#include "SGCantAdvanceBox.h"
#include "SGMallLayer.h"
#include "SGGuideManager.h"


#define  CCC_GREY_YELLOW ccc3(204,175,157)

SGAreanLayer::SGAreanLayer():m_pMyselfInfoItem(NULL),
m_pBaseInfnBg(NULL),
m_pLabelMyName(NULL),
m_pLabelMyRank(NULL),
m_pLabelMyRankReward(NULL),
m_pZhujiangIcon(NULL),
m_pButtonLuanShi(NULL),
m_pLabelMyAreanCoinSum(NULL),
m_pLabelChallengCount(NULL),
m_pLabelLastTime(NULL),
m_pLabelDescribInfo(NULL),
m_areanLayerType(Arean_qun),
m_pBaseInfoData(NULL),
m_pLabelCdTime(NULL),
m_pButtonGetReward(NULL),
m_plabelCanReward(NULL),
m_pButtonLog(NULL),
m_pButtonzhu(NULL),
m_pLabelQun(NULL),
m_pLabelLog(NULL),
m_pLabelZhu(NULL),
m_pButtonQun(NULL),
m_pMySelfData(NULL),
m_prewardIcon(NULL),
m_pLabelCanFight(NULL),
m_pCdTime(NULL),
m_pbuttonRule(NULL)
{
}
SGAreanLayer::~SGAreanLayer()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ARENA_START);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ARENA_GET_REWARD);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ARENA_BUY_FIGHT_COUNT);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_areanLayer);
}
SGAreanLayer * SGAreanLayer::create(CCArray * arrayData,SGAreanBaseInfoData *baseInfoData, SGAreanItemInfoData * itemData,AreanLayerType type)
{
    SGAreanLayer * areanLayer = new SGAreanLayer();
    if(areanLayer && areanLayer->init(NULL, sg_areanLayer))
    {
        areanLayer->initDatas(arrayData);
        areanLayer->setBaseInfoData(baseInfoData);
        areanLayer->setMyselfData(itemData);
        areanLayer->initView();
        areanLayer->autorelease();
        return areanLayer;
    }
    CC_SAFE_DELETE(areanLayer);
    return  NULL;
}
void SGAreanLayer::initView()
{
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    
    sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGAreanLayer::lookPlayerInfoListen));
    sgnc->addObserver(MSG_ARENA_START, this, callfuncO_selector(SGAreanLayer::enterFightListen));
    sgnc->addObserver(MSG_ARENA_GET_REWARD, this, callfuncO_selector(SGAreanLayer::getArenaRewardListen));
    sgnc->addObserver(MSG_ARENA_BUY_FIGHT_COUNT, this, callfuncO_selector(SGAreanLayer::buyArenaFightCountListen));
    
    ResourceManager::sharedInstance()->bindTexture("sgAreanLayer/sgAreanLayer.plist", RES_TYPE_LAYER_UI, sg_areanLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_areanLayer);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    
    m_pBaseInfnBg = CCSprite::createWithSpriteFrameName("arrea_base_bg1.png");//基础信息背景图
    this->addChild(m_pBaseInfnBg);
    m_pBaseInfnBg->setPosition(ccp(size.width/2,size.height - m_pBaseInfnBg->getContentSize().height*0.5 - 45 -10));
    CCSize baseInfonBg = m_pBaseInfnBg->getContentSize();
    CCLayerColor * colorLayerBg = CCLayerColor::create(ccc4(142,106,84,255), size.width, baseInfonBg.height + 20 );//背景遮罩
    this->addChild(colorLayerBg);
    colorLayerBg->ignoreAnchorPointForPosition(false);
    colorLayerBg->setAnchorPoint(ccp(0.5,0.5));
    colorLayerBg->setPosition(m_pBaseInfnBg->getPosition());
    colorLayerBg->setZOrder(-1);
    
    float SizeColorLayerBg = colorLayerBg->getContentSize().height/2;
    
    CCSprite * infobg = CCSprite::createWithSpriteFrameName("arrea_base_bg.png");
    infobg->setPosition(ccpAdd(m_pBaseInfnBg->getPosition(),ccp(0,-20)));
    this->addChild(infobg);
    
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    float offset1  = title_bg_l->getContentSize().height/2;
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(ccp(colorLayerBg->getPositionX(),colorLayerBg->getPositionY()), ccp(-size.width/2,  -SizeColorLayerBg - offset1)));
    this->addChild(title_bg_l,10);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(ccp(colorLayerBg->getPositionX(),colorLayerBg->getPositionY()), ccp(0,  -SizeColorLayerBg - offset1)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(ccp(colorLayerBg->getPositionX(),colorLayerBg->getPositionY()), ccp(size.width/2,  -SizeColorLayerBg - offset1)));
    this->addChild(title_bg_r,10);
    //////////////
    
    CCSprite * areanCoin = CCSprite::createWithSpriteFrameName("arean_coin_small.png");
    areanCoin->setPosition(ccpAdd(infobg->getPosition(),ccp(-230,23)));
    this->addChild(areanCoin);
    
    m_pLabelMyAreanCoinSum = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pBaseInfoData->getAreanCoin())->getCString(), FONT_PANGWA, 20, ccYELLOW);
    m_pLabelMyAreanCoinSum->setPosition(ccpAdd(infobg->getPosition(),ccp(-180,23)));
    this->addChild(m_pLabelMyAreanCoinSum);
    
    
    m_pLabelDescribInfo = SGCCLabelTTF::create("",FONT_PANGWA, 20);
    m_pLabelDescribInfo->setPosition(ccpAdd(infobg->getPosition(),ccp(70,23)));
    this->addChild(m_pLabelDescribInfo);
    
    SGCCLabelTTF * challengCount = SGCCLabelTTF::create(str_sgareanlayer_str3, FONT_PANGWA, 20,ccGREEN);
    challengCount->setPosition(ccpAdd(infobg->getPosition(),ccp(-210,-23)));
    this->addChild( challengCount);
    
    m_pLabelChallengCount = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",m_pBaseInfoData->getChallengeCount(),m_pBaseInfoData->getMaxChallengeCount())->getCString(), FONT_PANGWA, 20,ccGREEN);
    m_pLabelChallengCount->setPosition(ccpAdd(infobg->getPosition(),ccp(-120,-23)));
    this->addChild(m_pLabelChallengCount);
    
    SGCCLabelTTF * lastTime = SGCCLabelTTF::create(str_sgareanlayer_str4, FONT_PANGWA, 20,ccGREEN);
    lastTime->setPosition(ccpAdd(infobg->getPosition(),ccp(20,-23)));
    this->addChild(lastTime);
    
    m_pLabelLastTime = SGCCLabelTTF::create(calcTime(m_pBaseInfoData->getRewardTime())->getCString(), FONT_PANGWA, 20,ccGREEN);
    m_pLabelLastTime->setPosition(ccpAdd(infobg->getPosition(),ccp(140,-23)));
    this->addChild(m_pLabelLastTime);
    
    //////////////
    
    m_pMyselfInfoItem = CCSprite::createWithSpriteFrameName("arean_log_item.png");
    m_pMyselfInfoItem->setPosition(ccp(size.width/2,bottomH + m_pMyselfInfoItem->getContentSize().height/2));
    this->addChild(m_pMyselfInfoItem);
    //引导用的tag
    m_pMyselfInfoItem->setTag(SGStaticDataManager::shareStatic()->getimitTagById(18, 19));
    
    CCMenu *menu = CCMenu::create();
    menu->setTouchPriority(1);
    m_pMyselfInfoItem->addChild(menu, 10);
    menu->setPosition(CCPointZero);
    
    float offsetDown = m_pMyselfInfoItem->getContentSize().height;
    m_pLabelMyName = SGCCLabelTTF::create(m_pMySelfData->getNickName()->getCString(),FONT_PANGWA , 24,ccYELLOW);
   
    m_pLabelMyName->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(-118,-offsetDown + 15)));
    m_pMyselfInfoItem->addChild(m_pLabelMyName);
    
    SGCCLabelTTF * myRank = SGCCLabelTTF::create(str_sgareanlayer_str5, FONT_PANGWA, 24);
    myRank->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(-110,-offsetDown - 22)));
    m_pMyselfInfoItem->addChild(myRank);
    
    m_pLabelMyRank = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pMySelfData->getRank())->getCString(),FONT_PANGWA, 24);
    m_pLabelMyRank->setPosition(ccpAdd(myRank->getPosition(),ccp(myRank->getContentSize().width/2 + m_pLabelMyRank->getContentSize().width/2,0)));
    m_pMyselfInfoItem->addChild(m_pLabelMyRank);
    
    SGCCLabelTTF * myRankReaward = SGCCLabelTTF::create(str_sgareanlayer_str6, FONT_PANGWA, 24);
    myRankReaward->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(-110,-offsetDown - 56)));
    m_pMyselfInfoItem->addChild(myRankReaward);
    
    CCSprite  * rewardIcon = CCSprite::createWithSpriteFrameName("arean_coin_small.png");
    rewardIcon->setPosition(ccpAdd(myRankReaward->getPosition(),ccp(myRankReaward->getContentSize().width/2 + rewardIcon->getContentSize().width/2,0)));
    m_pMyselfInfoItem->addChild(rewardIcon);
    
    m_pLabelMyRankReward =SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pMySelfData->getAreanCoin())->getCString(),FONT_PANGWA,24,ccYELLOW);
    m_pLabelMyRankReward->setPosition(ccpAdd(rewardIcon->getPosition(),ccp(m_pLabelMyRankReward->getContentSize().width/2  + rewardIcon->getContentSize().width/2,0)));
    m_pMyselfInfoItem->addChild(m_pLabelMyRankReward);
    
    m_pButtonLuanShi = SGButton::createFromLocal("store_exchangebtnbg.png", str_sgareanlayer_str7, this,
                                                 menu_selector(SGAreanLayer::onClickLuanShiStoreButton), CCPointZero, FONT_PANGWA,ccWHITE,26,false,true);
    m_pButtonLuanShi->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(220,0)));
    m_pButtonLuanShi->setVisible(false);
   // m_pMyselfInfoItem->addChild(m_pButtonLuanShi);
    this->addBtn(m_pButtonLuanShi);
    m_pButtonLuanShi->setEnabled(false);
    
    m_pButtonGetReward = SGButton::createFromLocal("store_exchangebtnbg.png", str_sgareanlayer_str8, this, menu_selector(SGAreanLayer::onClickGetRewardButton), CCPointZero, FONT_PANGWA,ccWHITE,26,false,true);
    m_pButtonGetReward->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(210,-185)));
    menu->addChild(m_pButtonGetReward);
    

    
    m_prewardIcon  = CCSprite::createWithSpriteFrameName("arean_coin_small.png");
    m_prewardIcon->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(170,25)));
    this->addChild(m_prewardIcon);
    m_plabelCanReward = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_pBaseInfoData->getCanReward())->getCString(),FONT_PANGWA,24,ccYELLOW);
    m_plabelCanReward->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(230,25)));
    this->addChild(m_plabelCanReward);
    
    
    m_pZhujiangIcon = SGDrawCards::createNormalButton(m_pMySelfData->getOfficeItemId(),BIT_OFFICER,sg_areanLayer,this, menu_selector(SGAreanLayer::onClickMyZhuJiangIconButton));
    m_pZhujiangIcon->setPosition(ccpAdd(ccp(30, 20),ccp(m_pZhujiangIcon->getContentSize().width / 2,m_pZhujiangIcon->getContentSize().height / 2)));
//    this->addBtn(m_pZhujiangIcon);
    menu->addChild(m_pZhujiangIcon);
    //tableview各种设置
    tableView->setFrame(CCRectMake(0, 0, size.width, title_bg_l->getPositionY()-m_pMyselfInfoItem->getPositionY()-m_pMyselfInfoItem->getContentSize().height/2));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH + m_pMyselfInfoItem->getContentSize().height)));
    
    if (SGGuideManager::shareManager()->isGuide)
    {
        menu->setEnabled(false);
        m_pButtonGetReward->setEnabled(false);
        m_pZhujiangIcon->setEnabled(false);
    }
    
    m_pCdTime = SGCCLabelTTF::create(str_sgareanlayer_str9,FONT_PANGWA, 20);
    m_pCdTime->setPosition(ccpAdd(infobg->getPosition(),ccp(20,23)));
    this->addChild(m_pCdTime);
    
    m_pLabelCdTime = SGCCLabelTTF::create("", FONT_PANGWA, 20,ccRED);
    m_pLabelCdTime->setPosition(ccpAdd(infobg->getPosition(),ccp(130,23)));
    this->addChild(m_pLabelCdTime);
    
    m_pLabelCanFight = SGCCLabelTTF::create(str_sgareanlayer_str33, FONT_PANGWA, 20,ccGREEN);
    m_pLabelCanFight->setPosition(ccpAdd(infobg->getPosition(),ccp(20,23)));
    this->addChild(m_pLabelCanFight);
    
    m_pbuttonRule = SGButton::create("battle_rules.png","battle_rules.png" ,this,menu_selector(SGAreanLayer::onClickButtonRule));
    this->addBtn(m_pbuttonRule);
    m_pbuttonRule->setPosition(ccpAdd(infobg->getPosition(),ccp(230,0)));
    
    
    CCSprite * buttonbg1 = CCSprite::create();
    buttonbg1->setContentSize(CCSize(130,40));
    
    m_pButtonQun = SGButton::createFromSprite(buttonbg1,buttonbg1, this,menu_selector(SGAreanLayer::onClickButtonQun));
    m_pButtonQun->setPosition(ccpAdd(m_pBaseInfnBg->getPosition(), ccp(-220,60)));
    this->addBtn(m_pButtonQun);
    
    m_pLabelQun = SGCCLabelTTF::create(str_sgareanlayer_str10,FONT_PANGWA, 32);
    m_pLabelQun->setPosition(m_pButtonQun->getPosition());
    this->addChild(m_pLabelQun);
    
    CCSprite * buttonbg2 = CCSprite::create();
    buttonbg2->setContentSize(CCSize(130,40));
    m_pButtonLog = SGButton::createFromSprite(buttonbg2,buttonbg2, this,menu_selector(SGAreanLayer::onClickButtonLog));
    m_pButtonLog->setPosition(ccpAdd(m_pBaseInfnBg->getPosition(), ccp(0,60)));
    this->addBtn(m_pButtonLog);
    
    m_pLabelLog = SGCCLabelTTF::create(str_sgareanlayer_str11,FONT_PANGWA, 32);
    m_pLabelLog->setPosition(m_pButtonLog->getPosition());
    this->addChild(m_pLabelLog);
    
    CCSprite * buttonbg3 = CCSprite::create();
    buttonbg3->setContentSize(CCSize(130,40));
    m_pButtonzhu = SGButton::createFromSprite(buttonbg3,buttonbg3, this,menu_selector(SGAreanLayer::onClickButtonzhu));
    m_pButtonzhu->setPosition(ccpAdd(m_pBaseInfnBg->getPosition(), ccp(220,60)));
    this->addBtn(m_pButtonzhu);
    
    m_pLabelZhu = SGCCLabelTTF::create(str_sgareanlayer_str12,FONT_PANGWA, 32);
    m_pLabelZhu->setPosition(m_pButtonzhu->getPosition());
    this->addChild(m_pLabelZhu);

    m_pLabelQun->setInsideColor(ccWHITE);
    m_pLabelLog->setInsideColor(CCC_GREY_YELLOW);
    m_pLabelZhu->setInsideColor(CCC_GREY_YELLOW);
    
    schedule(schedule_selector(SGAreanLayer::updateCdTime), 1.0);
    schedule(schedule_selector(SGAreanLayer::updateRewardTime), 1.0);
    
//    if(m_pBaseInfoData->getCanReward() == 0)
//    {
//       // m_pButtonGetReward->setImage("");
//        m_pButtonGetReward->setEnabled(false);
//        
//    }
    tableViewHeight = skewing(66);
    
    //tableview设置tag
    tableView->setTag(SGStaticDataManager::shareStatic()->getimitTagById(18, 21));
    
    
}
void SGAreanLayer::onClickGetRewardButton()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    main::ArenaGetRewardRequest  * req = new main::ArenaGetRewardRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_GET_REWARD, req);
    
}
void SGAreanLayer::buyArenaFightCountListen(CCObject * obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    main::ArenaBuyFightCountResponse *resp = (main::ArenaBuyFightCountResponse*)request->m_msg;
    if(resp)
    {
        int state = resp->state();
        if(state == 0)
        {
            SGMainManager::shareMain()->showMessage(str_sgareanlayer_str27);
            m_pLabelChallengCount->setString(CCString::createWithFormat("%d/%d",1,m_pBaseInfoData->getMaxChallengeCount())->getCString());
        }
        else if(state == 1)
        {
            SGMainManager::shareMain()->showMessage(str_sgareanlayer_str28);
        }
        
    }

}
void SGAreanLayer::getArenaRewardListen(CCObject * obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    main::ArenaGetRewardResponse *resp = (main::ArenaGetRewardResponse*)request->m_msg;
    if(resp)
    {
        if(resp->state() == 0)
        {
            SGMainManager::shareMain()->showMessage(str_sgareanlayer_str13);
            int addcount = resp->zhengfudian();
            m_pBaseInfoData->setAreanCoin(m_pBaseInfoData->getAreanCoin() + addcount);
            m_pLabelMyAreanCoinSum->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerArenaScore())->getCString());
            
            m_plabelCanReward->setString("0");
            //m_pButtonGetReward->setEnabled(false);
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_sgareanlayer_str14);
        }
    }
}
void SGAreanLayer::onClickButtonRule()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    //SGMainManager::shareMain()->showMessage("正在定制中......");
    SGHelpTitleDataModel *tit = new SGHelpTitleDataModel;
    tit->setbeg(1751);
    tit->setend(1769);
    tit->setnum(19);
    CCString *str = CCString::create(str_sgareanlayer_str32);
    tit->settitle(str);
    
    SGHelpBookInfo *info = SGHelpBookInfo::create(tit,5);
    SGMainManager::shareMain()->showLayer(info);
    delete tit;

}
void SGAreanLayer::enterFightListen(CCObject * obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    main::ArenaStartResponse *resp = (main::ArenaStartResponse*)request->m_msg;
    if(resp)
    {
        int state = resp->state();
        if(state == 0)
        {
            
        }
        else if( state == -1)
        {
            SGMainManager::shareMain()->showMessage(CCString::create(resp->msg())->getCString());
        }
        else
        {
            switch (state) {
                case 1://次数不够
                    {
                        if(SGPlayerInfo::sharePlayerInfo()->getShowvip())
                        {
                            CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
                            CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("88");
                            int VipLevel = dic1->valueForKey("value")->intValue();
                            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_sgareanlayer_str26,VipLevel)->getCString());
                        }
                        else
                        {
                            SGMainManager::shareMain()->showMessage(str_sgareanlayer_str1);

                        }
                    }
                    break;
                case 2://时间未到
                    {
                        SGMainManager::shareMain()->showMessage(str_sgareanlayer_str2);
                    }
                    break;
                case 3 :
                    {
                        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL,2015,0,0,NULL,resp->gold());
                        SGMainManager::shareMain()->showBox(cantadvanceBox);
                    }
                    break;
                case 4 :
                {
                    
                }
                    break;
                case 5 :
                {
                    
                }
                    break;


                    
                default:
                    break;
            }
        }
    }

}
void SGAreanLayer::lookPlayerInfoListen(CCObject * obj)
{
   
    SGSocketRequest *request = (SGSocketRequest*)obj;
    main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)request->m_msg;
   
       int  typetype=0;
       ///////////////////////////////////////////////////
    SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
    SGRankDetailLayer*box = NULL;
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank))
    {
        box = (SGRankDetailLayer*)(SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank));
        box->removeFromParent();
    }
    
    box=SGRankDetailLayer::create(playerInfo,3);
    SGMainManager::shareMain()->showBox(box);
    /////////////////////////存储排行类型  国家  人物id
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    player->setrankType(typetype);
    //    SGRankAllPlayer *playerpl =  (SGRankAllPlayer *)datas->objectAtIndex(selectindex);
    
   // player->setplayerID(playerplayerid);
}
void SGAreanLayer::resetMyselfInfo()
{
    m_pLabelMyRankReward->setString(CCString::createWithFormat("%d",m_pMySelfData->getAreanCoin())->getCString());
    m_pLabelMyRank->setString(CCString::createWithFormat("%d",m_pMySelfData->getRank())->getCString());
    
    if(m_pZhujiangIcon)
    {
        m_pZhujiangIcon->removeFromParent();
    }
    m_pZhujiangIcon = SGDrawCards::createNormalButton(m_pMySelfData->getOfficeItemId(),BIT_OFFICER,sg_areanLayer,this, menu_selector(SGAreanLayer::onClickLuanShiStoreButton));
    m_pZhujiangIcon->setPosition(ccpAdd(m_pMyselfInfoItem->getPosition(),ccp(-220,0)));
    this->addBtn(m_pZhujiangIcon);

};
void SGAreanLayer::resetBaseInfo()
{
    m_pLabelMyAreanCoinSum->setString(CCString::createWithFormat("%d",m_pBaseInfoData->getAreanCoin())->getCString());
    m_pLabelDescribInfo->setString("");
    m_pLabelChallengCount->setString(CCString::createWithFormat("%d/%d",m_pBaseInfoData->getChallengeCount(),m_pBaseInfoData->getMaxChallengeCount())->getCString());
    
}
void SGAreanLayer::setMyselfData(SGAreanItemInfoData * myselfData)
{
    if(myselfData)
    {
        m_pMySelfData = myselfData;
        m_pMySelfData->retain();
    }
}
void SGAreanLayer::refreshView()
{
    tableView->reloadData();
}
void SGAreanLayer::onClickButtonQun()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    CCLOG("群雄争霸");
    m_pBaseInfnBg->setDisplayFrame(CCSprite::createWithSpriteFrameName("arrea_base_bg1.png")->displayFrame());
    main::ArenaMainInfoRequest * request = new main::ArenaMainInfoRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_MAIN_INFO, request);
    m_areanLayerType = Arean_qun;
    showMyselfinfoItem();
    
    m_pLabelQun->setInsideColor(ccWHITE);
    m_pLabelLog->setInsideColor(CCC_GREY_YELLOW);
    m_pLabelZhu->setInsideColor(CCC_GREY_YELLOW);
}
void SGAreanLayer::onClickButtonzhu()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    CCLOG("诸侯割据");
    m_pBaseInfnBg->setDisplayFrame(CCSprite::createWithSpriteFrameName("arrea_base_bg3.png.png")->displayFrame());
    main::ArenaRanksRequest * request = new main::ArenaRanksRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_RANK_LIST, request);
    m_areanLayerType = Arean_zhu;
    showMyselfinfoItem();
    
    m_pLabelQun->setInsideColor(CCC_GREY_YELLOW);
    m_pLabelLog->setInsideColor(CCC_GREY_YELLOW);
    m_pLabelZhu->setInsideColor(ccWHITE);

    
}
void SGAreanLayer::onClickButtonLog()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    CCLOG("征战之路");
    m_pBaseInfnBg->setDisplayFrame(CCSprite::createWithSpriteFrameName("arrea_base_bg2.png")->displayFrame());
    main::ArenaFightReportRequest * request = new main::ArenaFightReportRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_FIGHT_REPORT, request);
    m_areanLayerType = Arean_log;
    hideMyselfInfoItem();
    
    m_pLabelQun->setInsideColor(CCC_GREY_YELLOW);
    m_pLabelLog->setInsideColor(ccWHITE);
    m_pLabelZhu->setInsideColor(CCC_GREY_YELLOW);

   
}
void SGAreanLayer::hideMyselfInfoItem()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    m_pMyselfInfoItem->setVisible(false);
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - m_pBaseInfnBg->getContentSize().height  - bottomH - m_pMyselfInfoItem->getContentSize().height*0.5));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH )));
    
    m_pButtonLuanShi->setVisible(false);
    m_pButtonLuanShi->setEnabled(false);
    
    m_pZhujiangIcon->setVisible(false);
    m_pZhujiangIcon->setEnabled(false);
    
    m_pButtonGetReward->setVisible(false);
    m_pButtonGetReward->setEnabled(false);
    m_prewardIcon->setVisible(false);
    m_plabelCanReward->setVisible(false);

}
void SGAreanLayer::showMyselfinfoItem()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    m_pMyselfInfoItem->setVisible(true);
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - m_pBaseInfnBg->getContentSize().height  - bottomH - m_pMyselfInfoItem->getContentSize().height*1.5));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH + m_pMyselfInfoItem->getContentSize().height )));
    
    m_pButtonLuanShi->setVisible(true);
    m_pButtonLuanShi->setEnabled(true);
    
    m_pZhujiangIcon->setVisible(true);
    m_pZhujiangIcon->setEnabled(true);
    
    if(m_areanLayerType == Arean_qun)
    {
        m_pButtonGetReward->setVisible(true);
        m_pButtonGetReward->setEnabled(true);
        m_pButtonLuanShi->setVisible(false);
        m_pButtonLuanShi->setEnabled(false);
        m_prewardIcon->setVisible(true);
        m_plabelCanReward->setVisible(true);
    }
    else
    {
        m_pButtonGetReward->setVisible(false);
        m_pButtonGetReward->setEnabled(false);
        m_pButtonLuanShi->setVisible(true);
        m_prewardIcon->setVisible(false);
        m_plabelCanReward->setVisible(false);
    }

}
void SGAreanLayer::updateAreanLayer()
{
     tableView->reloadData();
     m_plabelCanReward->setString(CCString::createWithFormat("%d",m_pBaseInfoData->getCanReward())->getCString());
     unschedule(schedule_selector(SGAreanLayer::updateCdTime));
     schedule(schedule_selector(SGAreanLayer::updateCdTime), 1.0);
    if(m_pBaseInfoData->getCanReward() == 0)
    {
        // m_pButtonGetReward->setImage("");
        //m_pButtonGetReward->setEnabled(false);
        
    }
    else{
        m_pButtonGetReward->setEnabled(true);
    }
}
CCString * SGAreanLayer::calcTime(int time)
{
    
    if(time >= 3600)
    {
        return CCString::createWithFormat(str_sgareanlayer_str15,time/3600);
    }
    else if(time < 3600 && time >= 60)
    {
        return CCString::createWithFormat(str_sgareanlayer_str16,time/60);
    }
    else
    {
        return CCString::createWithFormat(str_sgareanlayer_str17,time);
    }

}
void SGAreanLayer::setCdTime()
{
    int time = m_pBaseInfoData->getCdTime();
    
    m_pLabelCdTime->setString(calcTime(time)->getCString());
}
void SGAreanLayer::updateCdTime()
{
    int time = m_pBaseInfoData->getCdTime();
   
    setCdTime();
    time--;
    m_pBaseInfoData->setCdTime(time);
    m_pLabelCanFight->setVisible(false);
    m_pCdTime->setVisible(true);
    if(time <= 0)
    {
        unschedule(schedule_selector(SGAreanLayer::updateCdTime));
        m_pLabelCdTime->setString("");
        m_pLabelCanFight->setVisible(true);
        m_pCdTime->setVisible(false);
        
    }
    
}
void SGAreanLayer::setRewardTime()
{
   int time = m_pBaseInfoData->getRewardTime();
    m_pLabelLastTime->setString(calcTime(time)->getCString());
}
void SGAreanLayer::updateRewardTime()
{
    int time = m_pBaseInfoData->getRewardTime();
    
    setRewardTime();
    time--;
    m_pBaseInfoData->setRewardTime(time);
    
    if(time <= 0)
    {
        m_pBaseInfoData->setRewardTime(30*60);
        //unschedule(schedule_selector(SGAreanLayer::updateRewardTime));
    }
}

void SGAreanLayer::onClickLuanShiStoreButton()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_ARENA , ARENA_LAYER);
}

void  SGAreanLayer::onClickMyZhuJiangIconButton()
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
     SGMainManager::shareMain()->showEmbattleLayer(4,SGTeamgroup::shareTeamgroupInfo()->getiscurrent());
}

void SGAreanLayer::initDatas(CCArray * arrayDatas)
{
    if(datas)
    {
        datas->removeAllObjects();
    }
    else
    {
        this->datas = CCArray::create();
        this->datas->retain();
    }
    
    for (int ii = 0; ii < arrayDatas->count(); ii++)
    {
        CCObject *obj = arrayDatas->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
}
void SGAreanLayer::setBaseInfoData( SGAreanBaseInfoData * baseInfnData)
{
    m_pBaseInfoData = baseInfnData;
    m_pBaseInfoData->retain();
}
// 根据indexPath返回tableView的cell
SNSTableViewCellItem* SGAreanLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);//从0开始
    
    if(m_areanLayerType != Arean_log)
    {
        SGAreanItemInfo *item = dynamic_cast<SGAreanItemInfo *>(tableView->dequeueReusableCellItemForIndexPath(indexPath));
        SGAreanItemInfoData *itemData = dynamic_cast<SGAreanItemInfoData *>(datas->objectAtIndex(index));
        
        
        
        if (item == NULL) {
            if(itemData != NULL)
            {

                item = SGAreanItemInfo::create(itemData, m_areanLayerType);
            }
        }
        else
        {
            item->setItemInfoData(itemData);
            item->setType(m_areanLayerType);
            item->updateItem();
        }
        return item;
    }
    else
    {
        SGAreanItemFightLog *item = dynamic_cast<SGAreanItemFightLog *>(tableView->dequeueReusableCellItemForIndexPath(indexPath));
        SGAreanFightLogData *itemData = dynamic_cast<SGAreanFightLogData *>(datas->objectAtIndex(index));
        
        
        
        if (item == NULL) {
            if(itemData != NULL)
            {
                item = SGAreanItemFightLog::create(itemData);
            }
        }
        else
        {
            item->setItemFightLogData(itemData);
            item->refreshItem();
        }
        return item;
    }

    
}
// 返回tableView的cell数
unsigned int SGAreanLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return  datas->count();
}
// 每个item的点击事件
void SGAreanLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
}

void SGAreanLayer::setIsCanTouch(bool isTouch)
{
     m_pZhujiangIcon->setEnabled(isTouch);
    m_pButtonLuanShi->setEnabled(isTouch);

    m_pButtonQun->setEnabled(isTouch);//群雄逐鹿按鈕
    m_pButtonLog->setEnabled(isTouch);//征战之路按钮
    m_pButtonzhu->setEnabled(isTouch);//诸侯割据按钮

    m_pButtonGetReward->setEnabled(isTouch);
    m_pbuttonRule->setEnabled(isTouch);
    tableView->setIsTouchEnabled(isTouch);

}


SGAreanBaseInfoData::SGAreanBaseInfoData():m_areanCoin(0),
m_maxChallengeCount(0),
m_challengeCount(0),
m_describInfo(NULL),
m_rewardTime(0),
m_cdTime(0)
{
}
SGAreanBaseInfoData::~SGAreanBaseInfoData()
{
}
SGAreanBaseInfoData * SGAreanBaseInfoData::creat(int arenaCoin , int challengeCount, int maxCount, CCString * des, int rewardTime, int cdTime, int  canReward)
{
    SGAreanBaseInfoData * baseInfo = new SGAreanBaseInfoData();
    if(baseInfo)
    {
        baseInfo->setAreanCoin(arenaCoin);
        baseInfo->setChallengeCount(challengeCount);
        baseInfo->setMaxChallengeCount(maxCount);
        baseInfo->setDescribInfo(des);
        baseInfo->setRewardTime(rewardTime);
        baseInfo->setCdTime(cdTime);
        baseInfo->setCanReward(canReward);
        baseInfo->autorelease();
        
        return baseInfo;
    }
    CC_SAFE_DELETE(baseInfo);
    return NULL;
}





