//
//  SGRankLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-11.
//
//

#include "SGRankLayer.h"
#include "SGMainManager.h"
#include "SimpleAudioEngine.h"
#include "SGRankCell.h"
#include "SGMenu.h"
#include "SGFirstLayer.h"
#include "RankModule.pb.h"
#include "SGRichLabel.h"
#include "SGRankDetailLayer.h"
#include "SGFriend.h"
#include "SGStringConfig.h"

SGRankLayer::SGRankLayer()
:scrollview(NULL),
ranktypetag(0),
countrytypetag(0),
tempdata(NULL),
myrank(0),
mydata(0),
layer(NULL),
menu1(NULL),
ranklabel(NULL),
shengwang(NULL),
rank(NULL),
ranknum(NULL),
name(NULL),
selectindex(0),
playerplayerid(0),
newindex(0),
maxnum(0),
isreload(true),//是否是本页下翻造成的刷新页面
count(0),
point(CCPointZero)
{
    ranktypetag=0;
    countrytypetag=100;
    tempdata=CCArray::create();
    tempdata->retain();
    selectindex=-10;
    isreload=true;
    maxnum=0;
    
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
}

SGRankLayer::~SGRankLayer()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RANK);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_rank);
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(true);
}


SGRankLayer *SGRankLayer::create(CCArray *array,int value ,int ranking,int maxitemnum)
{
    SGRankLayer *layer = new SGRankLayer();
    if (layer && layer->init(NULL,sg_rank))
    {
        layer->maxnum=maxitemnum;
        layer->initDatas(array);
        layer->initView(value,ranking);
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGRankLayer::initDatas(CCArray *array)
{
    this->datas = CCArray::create();
    this->datas->retain();
    for (int ii = 0; ii < array->count(); ii++)
    {
        CCObject *obj = array->objectAtIndex(ii);
        this->datas->addObject(obj);
    }

}


void SGRankLayer::initView(int value ,int ranking)
{
    //监听
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_RANK, this, callfuncO_selector(SGRankLayer::flushData));
    sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGRankLayer::receivedetail));

    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_rank);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_rank);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, 30, bgImg->getContentSize().width, size.height-45-bottomH);
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45)));
    this->addChild(title_bg,-1);
    
    ///////魏蜀吴等四个按钮
    const char bb[][100]={"upwar_font_sall.png","upwar_font_swei_n.png","upwar_font_sshu_n.png","upwar_font_swu_n.png"};
    int widthwidth = 0;
    
    for (int i=0;i<4;++i) {
        
        CCString*str;
        if (i==0) {
            str=CCString::create("hui.png");
        }
        else{
            str=CCString::create("an.png");
        }
        
        SGButton *btn = SGButton::create(str->getCString(), bb[i], this, menu_selector(SGRankLayer::countryTypeSelect),CCPointZero,false,true);
        float tt=  size.width-( btn->getContentSize().width*0.75*4+btn->getContentSize().width*3*0.15);
        
        btn->setPosition(ccp(btn->getContentSize().width*i*0.9+tt*0.5, size.height-45-btn->getContentSize().height*1.0));
        widthwidth+=btn->getContentSize().width;
        btn->setAnchorPoint(ccp(0, 1));
        
        
        if(i==0)
        {
           btn->setTag(100);
        }
        else if (i==1)
        {
            btn->setTag(102);
        }
        else if (i==2)
        {
            btn->setTag(101);
        }
        else
        {
            btn->setTag(103);
        }
        
        
       // btn->setTag(i+100);
        btn->setScale(0.75);

        addBtn(btn);
   
        
      point=btn->getPosition()-ccp(0,btn->getContentSize().height);
        
    }

    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(ccp(title_bg->getPositionX(),point.y), ccp(0,  0)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(ccp(title_bg->getPositionX(),point.y), ccp(-size.width/2,  0)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(ccp(title_bg->getPositionX(),point.y), ccp(size.width/2,  0)));
    this->addChild(title_bg_r,10);

    //////scrollview
    scrollview = CCScrollView::create(CCSizeMake(size.width*0.9, 112));
    layer = CCLayer::create();
    scrollview->setContainer(layer);
    scrollview->ignoreAnchorPointForPosition(false);
    scrollview->setAnchorPoint(ccp(0,1));
    
    scrollview->setPosition(ccp(size.width*0.05,size.height-45));
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(scrollview,10000);
    const char aa[][100]={str_RankLayer_str1,str_RankLayer_str2,str_RankLayer_str3,str_RankLayer_str4,str_ShouJingZhou_str1,str_RankLayer_str5};
    float mmm=title_bg->getPositionY()-title_bg->getContentSize().height;
    int width=0;
    for (int j=0;j<6;++j)
    {
       
        
        SGButton *btn = SGButton::createFromLocal("store_exchangebtnbg.png", aa[j], this, menu_selector(SGRankLayer::rankTypeSelect),CCPointZero,FONT_PANGWA,ccWHITE,25);
        btn->setPosition(ccp(0+btn->getContentSize().width*j, 108));
        width+=btn->getContentSize().width;
        btn->setAnchorPoint(ccp(0, 1));
       // btn->setTag(j);//防止和上面tag重了
        if(j==0)
        {
         btn->setFontColor(ccc3(0xff, 0xc0, 0x00));
        }
		
        
        CCMenu*menu = CCMenu::create();
		menu->setTouchPriority(128);
        menu->addChild(btn,1,j);
        menu->setPosition(CCPointZero);
        
        
        layer->addChild(menu,1,j);
        
    }
	
    
     int reallength = width;
    
    scrollview->setContentSize(CCSizeMake(reallength, 112));
    scrollview->setTouchEnabled(true);
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    
    
    
    /////自己的排名信息
    CCSprite*jibun=CCSprite::createWithSpriteFrameName("guide_board_new.png");
    jibun->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    jibun->setAnchorPoint(ccp(0, 0));
    jibun->setScaleX(size.width/jibun->getContentSize().width);
    this->addChild(jibun,1,100001);

    setselfrank(value,ranking);
   
    /////箭头们
    CCSprite*left = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    left->setAnchorPoint(ccp(1, 0.5));
    this->addChild(left);
    left->setFlipX(true);
    left->setPosition(ccpAdd(scrollview->getPosition(), ccp(0, -left->getContentSize().height*0.5)));
      
    CCSprite* rgt = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    this->addChild(rgt);
    rgt->setPosition(ccpAdd(scrollview->getPosition(), ccp(size.width*0.9, -rgt->getContentSize().height*0.5)));
    rgt->setAnchorPoint(ccp(0, 0.5));
    ////灰色背景
    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 90),size.width, scrollview->getPositionY()-title_bg_r->getPositionY()-title_bg->getContentSize().height-title_bg_r->getContentSize().height*0.5);
    bgLayer->setPosition(ccp(0, title_bg_r->getPositionY()+title_bg_r->getContentSize().height*0.5));
    this->addChild(bgLayer);


    
    
    //tableview各种设置
    tableView->setFrame(CCRectMake(0, 0, size.width, point.y-jibun->getPositionY()-jibun->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH+jibun->getContentSize().height)));
    
    
    
    

    
    
    tableViewHeight = skewing(73);
    ///////存储数值
    setTablePosition(tableView->getPosition());
    setTableViewSize(tableView->getContentSize());

}

void SGRankLayer::rankTypeSelect(CCNode*node)
{
    ////////默认为全部 按钮为灰色
    
    
    SGButton*btn0    =(SGButton*) getBtnByTag(100);
    btn0->setBackFrame(CCSprite::createWithSpriteFrameName("hui.png")->displayFrame());
    SGButton*btn1    =(SGButton*) getBtnByTag(101);
    btn1->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
    SGButton*btn2    =(SGButton*) getBtnByTag(102);
    btn2->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
    SGButton*btn3   =(SGButton*) getBtnByTag(103);
    btn3->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
    
    ///////
 
     SGButton*btn=(SGButton*)node;
    int  tag=btn->getTag();
    isreload=true;
   
    if(ranktypetag==btn->getTag())
    {
        return;
    }
    
    ranktypetag=tag;
    /////////////////////////////换排行种类了默认总排行  自己可见
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite*sp=(CCSprite*) getChildByTag(100001);
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    tableView->setFrame(CCRectMake(0, 0, size.width, point.y-sp->getPositionY()-sp->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH+sp->getContentSize().height)));
    
    jibunmieru(true);
    
    
    for (int i=0; i<6; i++)
	{
 
        CCMenu*menu=(CCMenu*)layer->getChildByTag(i);
        SGButton*bt= (SGButton*) menu->getChildByTag(i);
        bt->setFontColor(ccWHITE);
        
    }
    
    
    CCMenu*menu=(CCMenu*)layer->getChildByTag(ranktypetag);
    SGButton*btnn= (SGButton*) menu->getChildByTag(ranktypetag);
    btnn->setFontColor(ccc3(0xff, 0xc0, 0x00));
    
    
	main::RankRequest* req = new main::RankRequest();
	req->set_type(tag);
	req->set_nationid(0);
	req->set_page(1);
	SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
    

}

void SGRankLayer::countryTypeSelect(CCNode*node)
{
   SGButton*btn=(SGButton*)node;
    
    
     SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
     int mycountry=playInfo->getPlayerCountry();
     CCSize size = CCDirector::sharedDirector()->getWinSize();
     CCSprite*sp=(CCSprite*) getChildByTag(100001);
     float bottomH = SGMainManager::shareMain()->getBottomHeight();
    if(mycountry==btn->getTag()-100||btn->getTag()==100)
    {
        
        
        
        
        tableView->setFrame(CCRectMake(0, 0, size.width, point.y-sp->getPositionY()-sp->getContentSize().height));
        tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH+sp->getContentSize().height)));
        
        jibunmieru(true);
    
    
    }
    else
    {
    
        sp->setVisible(false);
        tableView->setFrame(CCRectMake(0, 0, size.width, point.y-sp->getPositionY()));
        tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
        
        jibunmieru(false);
    
    
    }
    
    
    
    
    
    
    if(countrytypetag==btn->getTag())
    {
        isreload=true;
    }
   countrytypetag=btn->getTag();

    
    switch (countrytypetag) {
        case 100:
        {
            SGButton*btn0    =(SGButton*) getBtnByTag(100);
            btn0->setBackFrame(CCSprite::createWithSpriteFrameName("hui.png")->displayFrame());
            SGButton*btn1    =(SGButton*) getBtnByTag(101);
            btn1->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn2    =(SGButton*) getBtnByTag(102);
            btn2->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn3   =(SGButton*) getBtnByTag(103);
            btn3->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
        }
            break;
        case 101:
        {
            SGButton*btn0    =(SGButton*) getBtnByTag(100);
            btn0->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn1    =(SGButton*) getBtnByTag(101);
            btn1->setBackFrame(CCSprite::createWithSpriteFrameName("hui.png")->displayFrame());
            SGButton*btn2    =(SGButton*) getBtnByTag(102);
            btn2->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn3   =(SGButton*) getBtnByTag(103);
            btn3->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
        }
            break;
        case 102:
        {
            SGButton*btn0    =(SGButton*) getBtnByTag(100);
            btn0->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn1    =(SGButton*) getBtnByTag(101);
            btn1->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn2    =(SGButton*) getBtnByTag(102);
            btn2->setBackFrame(CCSprite::createWithSpriteFrameName("hui.png")->displayFrame());
            SGButton*btn3   =(SGButton*) getBtnByTag(103);
            btn3->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
        }
            break;
        case 103:
        {
            SGButton*btn0    =(SGButton*) getBtnByTag(100);
            btn0->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn1    =(SGButton*) getBtnByTag(101);
            btn1->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn2    =(SGButton*) getBtnByTag(102);
            btn2->setBackFrame(CCSprite::createWithSpriteFrameName("an.png")->displayFrame());
            SGButton*btn3   =(SGButton*) getBtnByTag(103);
            btn3->setBackFrame(CCSprite::createWithSpriteFrameName("hui.png")->displayFrame());
        }
            break;
            
        default:
            break;
    }
    
    
    
    
    
    isreload=true;
//    datas->removeAllObjects();
    main::RankRequest* req = new main::RankRequest();
    req->set_type(ranktypetag);
    req->set_nationid(countrytypetag-100);
    req->set_page(1);
    SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
    
    
}





#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGRankLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);//从0开始
	SGRankCell *item = (SGRankCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGRankAllPlayer *player =  (SGRankAllPlayer *)datas->objectAtIndex(index);
    
    
 //   playerplayerid=player->getplayerId();
    if (item == NULL) {
        item = SGRankCell::create(player->getplayerId());
    }
    
    if (index < datas->count())
       {
        item->setData(player,ranktypetag,index+1);
   
           newindex = index-1;
           
           int datasmun=datas->count();
           
           int mmm=datasmun%20;
          
                   if((index==datasmun-1)&&(mmm==0))
                   {
                 
                       int allnum=datas->count();
                       int page=allnum/20;
                       main::RankRequest* req = new main::RankRequest();
                       req->set_type(ranktypetag);
                       req->set_nationid(countrytypetag-100);
                       req->set_page(page+1);
                       SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
                       isreload=false;

                   
                   }
               
               
               
               
        
           
           
        }
    
    else
      {
        item->setVisible(false);
      }
    
    return item;
}

unsigned int SGRankLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGRankLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGRankAllPlayer *player =  (SGRankAllPlayer *)datas->objectAtIndex(index);
    
    
    playerplayerid=player->getplayerId();
    //selectindex = tableView->getItemPointerWithIndexPath(indexPath);
    EFFECT_PLAY(MUSIC_BTN);
    
    
    SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
    playInfo->setindex(index);

}



//追加刷新本页面
void SGRankLayer::flushData(CCObject* obj)
{
  
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    main::RankResponse * rsp = (main::RankResponse*) sr->m_msg;
    if (rsp)
    {
        maxnum = rsp->rankinfos_size();
//        maxnum =rsp->size(); //what happened?
        //自己的信息
        main::RankProto myrankinfo=rsp->rankinfo();
        int myrank=myrankinfo.value();
        int rankstate=myrankinfo.ranking();

        refreshrank(myrank,rankstate);
        CCArray*array=CCArray::create();
       if(isreload) //不是向下滑动页面造成的
       {
           datas->removeAllObjects();
       }
        
    
         count=rsp->rankinfos_size();
        for (int i = 0; i < rsp->rankinfos_size(); i++)
        {
            
            main::RankProto rankinfo= rsp->rankinfos(i);
            SGRankAllPlayer*player=new SGRankAllPlayer();
            player->setplayerId(rankinfo.roleid());
            std::string str(rankinfo.nickname());
            CCString* ns=CCString::createWithFormat("%s",str.c_str());
            player->setName(ns);
            player->setHeadId(rankinfo.nation());
            
            player->setRank(rankinfo.value());
            player->setServerNum(rankinfo.areaid());
            std::string str2(rankinfo.areaname());
            CCString* ns2=CCString::createWithFormat("%s",str2.c_str());
            player->setServerName(ns2);

            player->setIsIn(rankinfo.ranking()); ///是否进榜
            datas->addObject(player);

        }
        datas->retain();
        if(isreload)
        tableView->reloadData(true);
        tableView->reloadData(false);
        
    }
}

void SGRankLayer::receivePlayerDetail(CCObject* obj)
{
    
   // SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SURVIVAL_RANK_LIST);
    
    
    
}

void SGRankLayer::setselfrank(int value ,int ranking)
{
    CCSprite*sp=(CCSprite*) getChildByTag(100001);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_rank);
    
    CCSprite*icon=CCSprite::createWithSpriteFrameName(CCString::createWithFormat( "countryIcon_%d.png",playerInfo->getPlayerCountry())->getCString());
    this->addChild(icon,2,1000);
    icon->setPosition(ccpAdd(sp->getPosition(), ccp(icon->getContentSize().width*1.1,sp->getContentSize().height*0.5)));
    //spr->setPosition(ccpAdd(sp->getPosition(), ccp(itemHeadPic->getContentSize().width*1.0,-sp->getContentSize().height*0.5)));
    CCSprite*kuang=CCSprite::createWithSpriteFrameName(CCString::createWithFormat( "country_%d.png",playerInfo->getPlayerCountry())->getCString());
    kuang->setPosition(ccpAdd(icon->getPosition(), ccp(-2, -5))   );
    this->addChild(kuang,2,1001);
    
    SGButton *btn = SGButton::createFromLocal("unhot.png", str_BossBattleRankListLayer_str4, this, menu_selector(SGRankLayer::gotoEmbattleLayer),CCPointZero,FONT_BOXINFO,ccWHITE,32);
    btn->setAnchorPoint(ccp(1, 0.5));
    btn->setPosition(ccp(size.width*0.95, kuang->getPositionY()-kuang->getContentSize().height*0.2));
    btn->setFontColor(ccc3(0xf3,0x89,00));
    addBtn(btn);
    btn->setTag(1002);
    
    
     name = SGCCLabelTTF::create(playerInfo->getNickName()->getCString(), FONT_BOXINFO, 32);
     name->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width*0.7, kuang->getContentSize().height*0.3)));
     name->setAnchorPoint(ccp(0, 0.5));
     this->addChild(name,20,1003);
    

    
    CCString*sss;
    if(ranking==0)
    {
        sss= CCString::create(str_RankLayer_str6);
    
    }
    else if (ranking>0)
    {
        sss=CCString::createWithFormat("%d",ranking);
    }
    else
    {
      sss=CCString::create(str_BossBattleRankListLayer_str7);
    }
    ranknum = SGCCLabelTTF::create(sss->getCString(), FONT_BOXINFO, 32 , ccc3(0x1e, 0xff, 0x00));
    ranknum->setAnchorPoint(ccp(0, 0.5));
    ranknum->setPosition(ccpAdd(ccp(btn->getPosition().x-btn->getContentSize().width*0.5,name->getPosition().y), ccp(0, 0)));
    this->addChild(ranknum,21,1004);
    //modify by:zyc. merge into create.
    //ranknum->setColor(ccc3(0x1e, 0xff, 0x00));
    
    
    
    
    rank = SGCCLabelTTF::create(str_RankLayer_str7, FONT_BOXINFO, 32 , ccc3(0xff, 0xc0, 0x00));
    rank->setAnchorPoint(ccp(1, 0.5));
   // rank->setPosition(ccpAdd(ranknum->getPosition(), ccp(-ranknum->getContentSize().width, 0)));
    rank->setPosition(ccpAdd(ccp(btn->getPosition().x-btn->getContentSize().width*0.5,name->getPosition().y), ccp(0, 0)));
    this->addChild(rank,20,1005);
    //modify by:zyc. merge into create.
    //rank->setColor(ccc3(0xff, 0xc0, 0x00));
    

    
    
    CCString*ranktypestring;
    if(ranktypetag==0)
    {
        ranktypestring=CCString::create(str_RankLayer_str8);
    
    }
    else if (ranktypetag==1)
    {
    ranktypestring=CCString::create(str_RankLayer_str8);
    }
    else if (ranktypetag==2)
    {
    ranktypestring=CCString::create(str_RankLayer_str9);
    }
    else if (ranktypetag==3)
    {
    ranktypestring=CCString::create(str_Class);
    }
    else if (ranktypetag==4)
    {
    ranktypestring=CCString::create(str_BossBattleRankListLayer_str6);
    }
    else
    {
     ranktypestring=CCString::create(str_RankLayer_str10);
    }
    
    
    
    shengwang = SGCCLabelTTF::create(ranktypestring->getCString(), FONT_BOXINFO, 32);
    shengwang->setPosition(ccpAdd(name->getPosition(), ccp(kuang->getContentSize().width*0.0, -kuang->getContentSize().height*0.5)));
    this->addChild(shengwang,20,1006);
    shengwang->setAnchorPoint(ccp(0, 0.5));
    
    CCString*mmm;
    if(ranking==0)
    {
        mmm= CCString::create(str_BossBattleRankListLayer_str7);
        
    }
    else if (ranking>0)
    {
        mmm=CCString::createWithFormat("%d",value);
    }
    else
    {
        mmm=CCString::create(str_BossBattleRankListLayer_str7);
    }

    ranklabel = SGCCLabelTTF::create(mmm->getCString(), FONT_BOXINFO, 32);
    ranklabel->setAnchorPoint(ccp(0, 0.5));
    ranklabel->setPosition(ccpAdd(shengwang->getPosition(), ccp(shengwang->getContentSize().width+10, 0)));
    this->addChild(ranklabel,20,1007);


}






void SGRankLayer::refreshrank(int value ,int ranking)
{
    
    CCString*sss;
    if(ranking==0)
    {
        sss= CCString::create(str_RankLayer_str6);
        
    }
    else if (ranking>0)
    {
        sss=CCString::createWithFormat("%d",ranking);
    }
    else
    {
        sss=CCString::create(str_BossBattleRankListLayer_str7);
    }
    
    
   
    
//    ranknum = SGCCLabelTTF::create(sss->getCString(), FONT_BOXINFO, 32);
//    ranknum->setAnchorPoint(ccp(1, 0.5));
//    ranknum->setPosition(ccpAdd(ccp(btn->getPosition().x+btn->getContentSize().width*0.15,name->getPosition().y), ccp(0, 0)));
//    this->addChild(ranknum,20);
//    ranknum->setColor(ccc3(0x1e, 0xff, 0x00));
    
    ranknum->setString(sss->getCString());
    
    
    CCString*ranktypestring;
    if(ranktypetag==0)
    {
        ranktypestring=CCString::create(str_RankLayer_str8);
        
    }
    else if (ranktypetag==1)
    {
        ranktypestring=CCString::create(str_RankLayer_str8);
    }
    else if (ranktypetag==2)
    {
        ranktypestring=CCString::create(str_RankLayer_str9);
    }
    else if (ranktypetag==3)
    {
        ranktypestring=CCString::create(str_Class);
    }
    else if (ranktypetag==4)
    {
        ranktypestring=CCString::create(str_BossBattleRankListLayer_str6);
    }
    else
    {
        ranktypestring=CCString::create(str_RankLayer_str10);
    }
    
    shengwang->setString(ranktypestring->getCString());
    

    
    CCString*mmm;
    if(ranking==0)
    {
        mmm= CCString::create(str_BossBattleRankListLayer_str7);
        
    }
    else if (ranking>0)
    {
        mmm=CCString::createWithFormat("%d",value);
    }
    else
    {
        mmm=CCString::create(str_BossBattleRankListLayer_str7);
    }
    
    ranklabel->setString(mmm->getCString());
    ranklabel->setPosition(ccpAdd(shengwang->getPosition(), ccp(shengwang->getContentSize().width+10, 0)));
  


}






void SGRankLayer::receivedetail(CCObject*obj)
{
    
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
    SGSocketRequest *request = (SGSocketRequest*)obj;
    main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)request->m_msg;
    //    if (roleInfo->role().roleid()) {
    //
    int typetype=-10;
    if(ranktypetag==0)
    {
//        info->set_type(1);
        typetype=0;
    }
    else
    {
//        info->set_type(0);
        typetype=1;
    }
    
    
    
    
   // datas->retain();
    
    
    int mmm=playerplayerid;
    
    ///////////////////////////////////////////////////
    SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
    SGRankDetailLayer*box = NULL;
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank))
    {
        box = (SGRankDetailLayer*)(SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank));
        box->removeFromParent();
    }

    box=SGRankDetailLayer::create(playerInfo,2);
    SGMainManager::shareMain()->showBox(box);
    /////////////////////////存储排行类型  国家  人物id
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    player->setrankType(typetype);

    player->setplayerID(playerplayerid);
    
    

    
}





void  SGRankLayer::jibunmieru(bool mierukana)
{
     CCSprite*sp=(CCSprite*) getChildByTag(100001);
     CCSprite*sp1=(CCSprite*) getChildByTag(1000);
     CCSprite*sp2=(CCSprite*) getChildByTag(1001);
     SGButton*sp3=(SGButton*) getBtnByTag(1002);
    
    
    
     CCSprite*sp4=(CCSprite*) getChildByTag(1003);
     CCSprite*sp5=(CCSprite*) getChildByTag(1004);
     CCSprite*sp6=(CCSprite*) getChildByTag(1005);
     CCSprite*sp7=(CCSprite*) getChildByTag(1006);
     CCSprite*sp8=(CCSprite*) getChildByTag(1007);
    
    
    
    
    
     sp->setVisible(mierukana);
     sp1->setVisible(mierukana);
     sp2->setVisible(mierukana);
    
    
     sp3->setVisible(mierukana);
     sp3->setEnabled(mierukana);
     sp4->setVisible(mierukana);
     sp5->setVisible(mierukana);
     sp6->setVisible(mierukana);
     sp7->setVisible(mierukana);
     sp8->setVisible(mierukana);
    
    
    
    
    
    


}


void SGRankLayer::gotoEmbattleLayer()
{
    SGMainManager::shareMain()->showEmbattleLayer(3,SGTeamgroup::shareTeamgroupInfo()->getiscurrent(),false
                                                  );
}

void SGRankLayer::setIsCanTouch(bool isTouch)
{
    SGBaseTableLayer::setIsCanTouch(isTouch);
    for (int i=0; i<6; i++)
	{
        CCMenu*menu=(CCMenu*)layer->getChildByTag(i);
        menu->setEnabled(isTouch);
        SGButton*bt= (SGButton*) menu->getChildByTag(i);
        bt->setEnabled(isTouch);
    }
    scrollview->setTouchEnabled(isTouch);
}



