//
//  SGShouJingZhou.cpp
//  GameSanGuo
//
//  Created by kanata on 13-11-14.
//
//

#include "SGShouJingZhou.h"
#include "SGMainManager.h"
#include "SGExpandFriend.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGTestSwitch.h"
#include "SGHelpBookInfo.h"
#include "SGShouJingZhouRank.h"
#include "SGShowJingZhouRankPlayerLayer.h"
#include "SGSjzData.h"
#include "SGBattleManager.h"
#include "SurvivalModule.pb.h"
#include "SGGuideManager.h"
#include "SGCantAdvanceBox.h"
#include "SGShouJingZhouProcessData.h"
#include "PlotModule.pb.h"
#include "SGStorySectionCell.h"
#include "SGFirstLayer.h"
#include "SGStringConfig.h"

//如此则不上榜，不管排名多少
#define RANK_GATE 0

SGShouJingZhou::SGShouJingZhou():labelcs(NULL),labelMax(NULL)
{
    
}

SGShouJingZhou::~SGShouJingZhou()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_shoujingzhou);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SURVIVAL_CHALLENGE);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CREAM);

}

SGShouJingZhou  *SGShouJingZhou::create()
{
    SGShouJingZhou *shoujingzhou = new SGShouJingZhou();
    if(shoujingzhou && shoujingzhou->init(NULL, sg_shoujingzhou))
    {
        shoujingzhou->initView();
        shoujingzhou->autorelease();
        return shoujingzhou;
    }
    
    CC_SAFE_DELETE(shoujingzhou);
    return NULL;
}

void SGShouJingZhou::initView()
{
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_SURVIVAL_CHALLENGE, this, callfuncO_selector(SGShouJingZhou::receivebossinfo));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_BUY_CREAM, this, callfuncO_selector(SGShouJingZhou::buychance));
    
    
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    float winheight=size.height;
    float winwidth=size.width;
    
    //加载图形资源
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_shoujingzhou);
    rm->bindTexture("sgshoujingzhou/sgshoujingzhou.plist", RES_TYPE_LAYER_UI, sg_shoujingzhou); //守荆州UI
    
    float headH = H_TOP;
    float bottomH = H_BOTTOM;
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_shoujingzhou);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, stdSize.height/2-(size.height-headH-bottomH)/2,bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    /////
    
    //关羽bg
    CCSprite*gybg=CCSprite::createWithSpriteFrameName("sjz_info_frame.png");
    gybg->setPosition(ccpAdd(center,ccp(0,gybg->getContentSize().height*0.55)));
//    gybg->setScaleX(1.1);
    this->addChild(gybg);
    
    float gyheight=gybg->getContentSize().height;
    float gywidth=gybg->getContentSize().width;
    //        CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("friend_myFriend.png");
    SGCCLabelTTF* label = SGCCLabelTTF::create(str_ShouJingZhou_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    label->setPosition(ccpAdd(upCenter, ccp(0, -h+65/2)));
    //modify by:zyc. merge into create.
    //label->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(label);
    
    //解析荆州详情数据
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();

    //文字部分
    int fontSize = 25;
    CCString* str = NULL;
    //当前进度
    int cur = data->getLevelCur();
    if (cur == 0)
        str = CCString::createWithFormat(str_ShouJingZhou_str2);
    else
        str = CCString::createWithFormat(str_ShouJingZhou_str3, cur+1);////
    SGCCLabelTTF*labelguan=SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, fontSize);
    
    //您的排名
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("3");
    int nJifen = dic1->valueForKey("value")->intValue();
    
    str = CCString::createWithFormat( "%d/%d", SGPlayerInfo::sharePlayerInfo()->getJunHun(), nJifen );
    SGCCLabelTTF*labelming=SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, fontSize);
    
    //今日参战
    str = CCString::createWithFormat( "%d/%d", data->getCountCur(), data->getCountMax() );
    labelcs=SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, fontSize);
    //最高记录
    str = CCString::createWithFormat( str_ShouJingZhou_str6, data->getDamageMax());
    labelMax=SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, fontSize);
    
    //labelguan->setPosition(gybg->getPosition());//
    labelguan->setPosition(ccpAdd(gybg->getPosition(), ccp(-120+labelguan->getContentSize().width/2, -fontSize +4)));
    labelMax->setPosition(ccpAdd(gybg->getPosition(), ccp(gywidth*0.3, -fontSize +4)));
    
    labelming->setPosition(ccpAdd(gybg->getPosition(), ccp(gywidth*0.3, -fontSize*2.5+4)));
    labelcs->setPosition(ccpAdd(gybg->getPosition(), ccp(-120+labelcs->getContentSize().width/2, -fontSize*2.5+4)));
    
    this->addChild(labelguan);
    this->addChild(labelming);
    this->addChild(labelcs);
    this->addChild(labelMax);
    
    //返回按钮
	SGButton *buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png",this, menu_selector(SGShouJingZhou::backHandler),CCPointZero,false,true);
//	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", "返回", this, menu_selector(SGShouJingZhou::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setPosition(ccpAdd(label->getPosition(), ccp(-skewing(160), 0)));
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(buttonBack);
    
    //开始守城
    CCString*string;
    
    if(cur==0)
    {
		string=CCString::create("sjz_kssc.png");
    }
	else
    {
		string=CCString::create("sjz_jxsc.png");
    }
  
	SGButton *jxsc=SGButton::create("item_bigbg.png",string->getCString(),this, menu_selector(SGShouJingZhou::challengeprocess),CCPointZero,false,true);
	jxsc->setPosition(ccpAdd(center,ccp(0,-gyheight*0.30)));
    this->addBtn(jxsc);
    
    //军魂换礼
    SGButton *restart=SGButton::create("item_bigbg.png","sjz_jhhl.png",this, menu_selector(SGShouJingZhou::exchangeHandler),CCPointZero,false,true);
    restart->setPosition(ccpAdd(center,ccp(0,-gyheight*0.92)));
    this->addBtn(restart);
    
    //规则说明
    SGButton *gzsm=SGButton::create("sjz_info_btn.png","sjz_gzsm.png",this, menu_selector(SGShouJingZhou::ruleHandler),CCPointZero,false,true);
    gzsm->setPosition(ccpAdd(bottomCenter,ccp(-winwidth*0.24,winheight*0.22)));
    this->addBtn(gzsm);
    
    //战绩排行
    SGButton *rank=SGButton::create("sjz_info_btn.png","sjz_zjph.png",this, menu_selector(SGShouJingZhou::rankingHandler),CCPointZero,false,true);
    rank->setPosition(ccpAdd(bottomCenter,ccp(winwidth*0.24,winheight*0.22)));
    this->addBtn(rank);
    
    if (SGGuideManager::shareManager()->limitStartStep > 0) {
        setIsCanTouch(false);
    }
  
}


void SGShouJingZhou::backHandler(CCObject*obj)
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}

//开始守城
void SGShouJingZhou::challengeHandler()
{
    //获取玩家各种信息
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    if (!player->canBreakLimitById(limitShouJinZhou)) {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_ShouJingZhou_str7,player->getLimitLevelById(limitShouJinZhou))->getCString() );
        return;
    }

   ///判断VIP等级够不够
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    int curr=  data->getCountCur();
    int  max=data->getCountMax();
    
    int level = data->getLevelCur();
    
    if(curr <max)
    {
        SGBattleManager::sharedBattle()->sjzChallengeRequest();
    }
    else if (curr==max)
    {
       if(level)
       {
           SGBattleManager::sharedBattle()->sjzChallengeRequest();
       }
       else
       {
            SGMainManager::shareMain()->showMessage(str_ShouJingZhou_str8);
       }
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_ShouJingZhou_str8);
    }
    
    
}

//军魂换礼
void SGShouJingZhou::exchangeHandler()
{
    SGMainLayer* layer = SGMainManager::shareMain()->getMainLayer();
    if (layer)
        layer->sjzSoulExchangeRequest(1);
}

//TODO: 守荆州简介
void SGShouJingZhou::ruleHandler(CCObject*obj)  //暂时copy 后期再改
{
    SGHelpTitleDataModel *tit = new SGHelpTitleDataModel();
    tit->setbeg(1628);
    tit->setend(1645);
    tit->setnum(17);
    CCString *str = CCString::create(str_ShouJingZhou_str9);
    tit->settitle(str);
    
    
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
    SGHelpBookInfo *info = SGHelpBookInfo::create(tit, 3);
    SGMainManager::shareMain()->showLayer(info);
    delete tit;
}

//排行榜发送
void SGShouJingZhou::rankingHandler(CCObject*obj)
{
    //监听
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_SURVIVAL_RANK_LIST, this, callfuncO_selector(SGShouJingZhou::rankingCallback));
    //发消息
    main::SurvivalRankListRequest* req = new main::SurvivalRankListRequest();
    req->set_page(1);
    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_RANK_LIST, req);
    return;

}

//排行榜回调
void SGShouJingZhou::rankingCallback(CCObject* obj)
{
    //移除
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->removeObserver(this, MSG_SURVIVAL_RANK_LIST);
    //处理
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SurvivalRankListResponse *rsp = (main::SurvivalRankListResponse *) sr->m_msg;
        if (rsp)
        {
            if (rsp->rankplayers_size() > 0)
            {
                /////////////
                SGSjzData* sjzData = SGPlayerInfo::sharePlayerInfo()->getSjzData();
                sjzData->setDailyRank(rsp->nowrank());
                sjzData->setDailyIngot(rsp->rewardcoins());
                
                
                ////////////
                CCArray* list = CCArray::create();
                main::SurvivalRankPlayerMessage temp;
                SGShowJingZhouRankPlayerLayer* data;
                int number = rsp->rankplayers_size();
                for (int k = 0; k < rsp->rankplayers_size(); k++)
                {
                    temp = rsp->rankplayers(k);
                    data = SGShowJingZhouRankPlayerLayer::create(temp.playerid(), temp.nickname().c_str(), temp.rankcount(), temp.survivallevel(), temp.maxdamage(), temp.iconid(), temp.acquiregold());
                    list->addObject(data);
                }
                SGMainManager::shareMain()->showsjzrank(list, rsp->pageitemsnum(), rsp->totalitemsnum(),
														rsp->nowrank(), rsp->rewardcoins(), rsp->rewardnum(), rsp->residuetime());
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_ShouJingZhou_str10);
            }
        }
    }
}

void SGShouJingZhou::sendVipCostChanceRequest(int nowchance)
{
    SGBattleManager::sharedBattle()->sjzChallengeRequest();
}


void SGShouJingZhou::challengeprocess()
{
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    int cur = data->getLevelCur();
    if(cur==0)
    {
        main::SurvivalChallengeRequest* req = new main::SurvivalChallengeRequest();

        SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_CHALLENGE, req);
    }
    else
    {
        main::SurvivalStartRequest* req = new main::SurvivalStartRequest();
        req->set_isjump(0);
        SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_START, req);
    
    }
    
}

void SGShouJingZhou::receivebossinfo(CCObject*obj)
{
  //  SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SURVIVAL_CHALLENGE);
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SurvivalChallengeResponse *rsp = (main::SurvivalChallengeResponse *) sr->m_msg;
        if (rsp)
        {
            int state=rsp->state();
            if(state==0)
            {
                SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
                SGCreamBattleBox *box = SGCreamBattleBox::create(NULL, data->getVIPCountCur(), data->getVIPGold(), -1,
																 false,data->getVIPCountMax(),rsp->willaddcount(), rsp->needrmb());
                SGMainManager::shareMain()->showBox(box);
            }
            
            else if (state==1)
            {
                CCArray*array=CCArray::create();
                //////加上第0关

                for(int i=rsp->survivalbossdumpinfo_size()-1;i>=0;i--)
                {
                    main::SurvivalBossDumpInfo   bossinfo =rsp->survivalbossdumpinfo(i);
                    SGShouJingZhouProcessData* data=new SGShouJingZhouProcessData();
                    data->setlevel(bossinfo.index());
                    data->setjunhun(bossinfo.damage());
                    data->setgold(bossinfo.coins());
                    array->addObject(data);
                }
                
                SGShouJingZhouProcessData* firstdata=new SGShouJingZhouProcessData();
                firstdata->setlevel(1);
                firstdata->setjunhun(0);
                firstdata->setgold(0);
                
                array->addObject(firstdata);

                SGMainManager::shareMain()->showjingzhouprocesslayer(array);
                
            }
            else if (state==2)
            {
                ///弹框
				SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();//获取价格
                bool havachance=true;
                if(data->getVIPCountCur()==data->getVIPCountMax())
                {
                    havachance=false;
                }

                SGCreamBattleBox *box = SGCreamBattleBox::create(NULL, data->getVIPCountCur(), data->getVIPGold(), -1,
																 havachance,data->getVIPCountMax() - data->getVIPCountCur());
                SGMainManager::shareMain()->showBox(box);
            }
            else if (state==3)
            {
				SGMainManager::shareMain()->showMessage(str_ShouJingZhou_str11);
            }
            
        }
    }

  
}

void SGShouJingZhou::buychance(CCObject *sender)
{
    
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CREAM);
    
    
    SGSocketRequest *request = (SGSocketRequest *)sender;
    if (request)
    {
        main::BossPlotBuyCountResponse *response = (main::BossPlotBuyCountResponse *)request->m_msg;
        //成功
        
        if (response->state() == 1)
		{
            //发送获取详情
            SGMainManager::shareMain()->sendGetSjzInfoRequest();
			SG_SHOW_WINDOW(str_ShouJingZhou_str12);
			
		}
		else if (response->state() == 2)
		{
			SG_SHOW_WINDOW(str_ShouJingZhou_str13);
		}
		else if (response->state() == 3)
		{
			SG_SHOW_WINDOW(str_ShouJingZhou_str14);
		}

    }

}

void SGShouJingZhou::refreshView()
{
    SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
    
    if(labelMax)
    {
        CCString * str = CCString::createWithFormat( str_ShouJingZhou_str6, data->getDamageMax() );
        labelMax->setString(str->getCString());
    }
    
    if (labelcs)
    {
        CCString*  str = CCString::createWithFormat( "%d/%d", data->getCountCur(), data->getCountMax() );
        labelcs->setString(str->getCString());
    }
}

