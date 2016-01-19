//
//  SGLevelUpGetGold.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#include "SGLevelUpGetGold.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "cocos-ext.h"
#include "RewardModule.pb.h"
#include "SGMenu.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"
#include "SGRewardLayer.h"
USING_NS_CC_EXT;



SGLevelUpGetGold::SGLevelUpGetGold()

{
}
SGLevelUpGetGold::~SGLevelUpGetGold()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_levelUpGetGold);
   


    _array->release();
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_REWARD_LVLUP);
}
SGLevelUpGetGold *SGLevelUpGetGold::create(CCArray *array)
{
    SGLevelUpGetGold *rewardLayer = new SGLevelUpGetGold();
    if (rewardLayer && rewardLayer->init(NULL, sg_levelUpGetGold))
    {
        rewardLayer->_array = array;
        rewardLayer->_array->retain();
        rewardLayer->initView();
        rewardLayer->autorelease();
        return rewardLayer;
    }
    CC_SAFE_DELETE(rewardLayer);
    return NULL;
}
void SGLevelUpGetGold::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_levelUpGetGold);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_levelUpGetGold);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_REWARD_LVLUP,
                              this,
                              callfuncO_selector(SGLevelUpGetGold::lvlUpEnterListener));

    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    tableViewHeight = 110;
    
    tableView->setFrame(CCRectMake(0, 0, s.width, (s.height - btmhgt - headhgt)));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, btmhgt)));
    tableView->setDown(25);
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGLevelUpGetGold::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("reward_font_getgold.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_LevelUpGetGold_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    
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
    title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt + 65*.48)));
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title->getPosition().y));
    
}

SNSTableViewCellItem *SGLevelUpGetGold::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)//hehe
{
    
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    CCString *str = NULL;
    LvlGold_Item *item = (LvlGold_Item*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
//    LvlGold_Item *item = (LvlGold_Item*)tableView->getItemByIndexPath(indexPath);

    //CCString *str = (CCString*)_array->objectAtIndex(num-1);
//    if (NULL == item)
//    {
//        if (num> 0 && num<5)
//        {
//            str=(CCString*)_array->objectAtIndex(num-1);
//           if( str->intValue()==2)
//           {
//               item = LvlGold_Item::create(this,num-1,2);
//           }
//            else
//            {
//               item = LvlGold_Item::create(this,num-1,1);
//            }
//        }
//        else if(num>5)
//        {
//           str=(CCString*)_array->objectAtIndex(num-2);
//            if( str->intValue()==2)
//            {
//                item = LvlGold_Item::create(this,num-1,2);
//            }
//            else
//            {
//                item = LvlGold_Item::create(this,num-1,1);
//            }
//        }
//        else 
//        {
//          item = LvlGold_Item::create(this,num-1,1);
//        }
//    }
    if (num> 0 && num<=10)
    {
        
        str = (CCString*)_array->objectAtIndex(num-1);
        CCLOG("<<<<%d<<%d",num-1,str->intValue() );
        if(item==NULL)
        {
            item = LvlGold_Item::create(this,num-1,str->intValue());
        }
        item->updateitem(num-1, str->intValue());
    }
    else if(num>10)
    {
        str = (CCString*)_array->objectAtIndex(num-2);
        CCLOG("<<<TTTT<%d<<%d",num-1,str->intValue() );
        if(item==NULL)
        {
            item = LvlGold_Item::create(this,num-1,str->intValue());
        }
        item->updateitem(num-1, str->intValue());
    }
    else
    {
        if(item==NULL)
        {
            item = LvlGold_Item::create(this,num-1,1);
        }
        item->updateitem(num-1, 1);
    }
    return item;
}
unsigned int  SGLevelUpGetGold::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return _array->count() + 2;
}
void SGLevelUpGetGold::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    //int index = tableView->getItemPointerWithIndexPath(indexPath);
}

void SGLevelUpGetGold::getGoldHandler(LvlGold_Item *LvlGold_Item)
{
    EFFECT_PLAY(MUSIC_34);
    lvlupItem = LvlGold_Item;
    
    main::RewardLevelUpPurchRequest *request = new main::RewardLevelUpPurchRequest();
    request->set_level(lvlupItem->lvl);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LVLUP, request);
}
void SGLevelUpGetGold::lvlUpEnterListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_levelUpGetGold);


	
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::RewardLevelUpPurchResponse *response = (main::RewardLevelUpPurchResponse *)sr->m_msg;

        if (response->result() > 0) {
            SGPlayerInfo *playerInfo =SGPlayerInfo::sharePlayerInfo();
            
            lvlupItem->btn->init("reward_font_ylq.png", "reward_font_ylq.png", lvlupItem, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);
            lvlupItem->btn->type=2;
            lvlupItem->state = 2;
            
           
//            lvlupItem->btn->setImage("reward_font_ylq.png");
//            CCSprite *font = CCSprite::createWithSpriteFrameName("reward_font_ylq.png");
//            lvlupItem->btn->setFontImage(font->displayFrame());
            
            playerInfo->setPlayerGold(response->result() + playerInfo->getPlayerGold());
//            SGMainManager::shareMain()->showMessage(CCString::createWithFormat("已获得%d元宝",response->result())->getCString());
            
            //弹出框显示奖励
            CCArray *data=CCArray::create();
            data->retain();
            SGMailSystemAccessoryDataModel *datamodel=new SGMailSystemAccessoryDataModel ;
            datamodel->setType(0);
            datamodel->setCount(response->result());
            data->addObject(datamodel);
            
            SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
            SGMainManager::shareMain()->showBox(rewardBox);
            
            SGMainManager::shareMain()->updataUserMsg();
            
            int i = 0;
            switch (lvlupItem->lvl) {
                case 5:
                    i = 0;
                    break;
                case 10:
                    i = 1;
                    break;
                case 15:
                    i = 2;
                    break;
                case 20:
                    i = 3;
                    break;
				case 25:
                    i = 4;
                    break;
				case 30:
                    i = 5;
                    break;
				case 35:
                    i = 6;
                    break;
				case 40:
                    i = 7;
                    break;
				case 45:
                    i = 8;
                    break;
				case 50:
                    i = 9;
                    break;
					
					
					
                case 1550:
                    i = 10;
                    break;
                case 1600:
                    i = 11;
                    break;
                case 1650:
                    i = 12;
                    break;
                case 1700:
                    i = 13;
                    break;
                case 1750:
                    i = 14;
                    break;
                case 1800:
                    i = 15;
                    break;
                case 1850:
                    i = 16;
                    break;
                case 1900:
                    i = 17;
                    break;
                case 1950:
                    i = 18;
                    break;
                case 2000:
                    i = 19;
                    break;
                default:
                    break;
            }
            
            CCString*str = (CCString*)_array->objectAtIndex(i);
            str->setString(CCString::createWithFormat("%d",1)->getCString());
           // CCString*temp =(CCString *)playerInfo->getrewardNotice()->objectAtIndex(1);
           // temp->setString(CCString::createWithFormat("%d",0)->getCString());
            CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
            int nsum = array->count();
            for (int i = 0 ;i < nsum ;i++) {
                SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                if(item->gettypeItem()==Reward_LvlUpGold)
                {
                    item->setstateItem(0);
                    break;
                }
            }

            CCObject *obj = NULL;
            CCARRAY_FOREACH(_array, obj)
            {
                CCString *str = (CCString *)obj;
                if (str->intValue() == 3) {
                   // CCString*temp =(CCString *)playerInfo->getrewardNotice()->objectAtIndex(1);
                    //temp->setString(CCString::createWithFormat("%d",1)->getCString());
                    
                    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getrewardNotice();
                    int nsum = array->count();
                    for (int i = 0 ;i < nsum ;i++) {
                        SGRewardNoticeItem * item =(SGRewardNoticeItem *) array->objectAtIndex(i);
                        if(item->gettypeItem()==Reward_LvlUpGold)
                        {
                            item->setstateItem(1);
                            break;
                        }
                    }

                    break;
                }
            }
        }
        
    }
}

void SGLevelUpGetGold::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}


LvlGold_Item::LvlGold_Item()
:state(0),
lvl(0),
item(NULL),
btn(NULL)
{
    
}

LvlGold_Item::~LvlGold_Item()
{

}

void LvlGold_Item::setView(int num,int station)
{
     ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_levelUpGetGold);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    
    SGCCLabelTTF *font = SGCCLabelTTF::create("", FONT_PANGWA, 30);
    this->addChild(font,0,99);
    font->setPosition(ccpAdd(font->getPosition(), ccp(0, -20)));
    
    item = CCSprite::createWithSpriteFrameName("item_bg.png");
    this->addChild(item,-1);
    bgbgbg = NULL;
    SGCCLabelTTF *lab_up = SGCCLabelTTF::create(str_LevelUpGetGold_str2,FONT_BOXINFO,28 , COLOR_YELLOW);
    this->addChild(lab_up,0,10);
    //modify by:zyc. merge into create.
    //lab_up->setColor(COLOR_YELLOW);
    lab_up->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width *.4, 0)));

    SGCCLabelTTF *lvl = SGCCLabelTTF::create("",FONT_PANGWA,28 , COLOR_ORANGE);
    this->addChild(lvl,0,11);
    //modify by:zyc. merge into create.
    //lvl->setColor(COLOR_ORANGE);
    lvl->setPosition(ccpAdd(lab_up->getPosition(), ccp(60, 0)));
    
    SGCCLabelTTF *font_lvl = SGCCLabelTTF::create(str_LevelUpGetGold_str3,FONT_PANGWA,28 , COLOR_ORANGE);
    this->addChild(font_lvl,0,12);
    //modify by:zyc. merge into create.
    //font_lvl->setColor(COLOR_ORANGE);
    font_lvl->setPosition(ccpAdd(lvl->getPosition(), ccp(50, 0)));
    
    CCSprite *song = CCSprite::createWithSpriteFrameName("reward_font_song.png");
    this->addChild(song,0,13);
    song->setPosition(ccpAdd(item->getPosition(), ccp(60, 0)));
    
    CCSprite *yb = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yb,0,14);
 

    CCSprite *font_v = CCSprite::createWithSpriteFrameName("reward_V.png");
    this->addChild(font_v,0,16);
    font_v->setPosition(ccpAdd(song->getPosition(), ccp(-60, 5)));
    font_v->setVisible(false);
    
    CCLabelAtlas* currvip = CCLabelAtlas::create("1", "sanguobigpic/store_shuzi_z.png", 18, 23, '0');
    currvip->setAnchorPoint(ccp(0, 0.5));
    currvip->setPosition(ccpAdd(song->getPosition(), ccp(-43, 1.5)));
    this->addChild(currvip,0,17);
    currvip->setVisible(false);
    
    SGCCLabelTTF *font_num = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    this->addChild(font_num,0,15);
    font_num->setAnchorPoint(ccp(0, 0.5));
    font_num->setPosition(ccpAdd(yb->getPosition(), ccp(yb->getContentSize().width/2+3, 0)));
    
   
    CCLOG("------><%d<<%d",num,station );
    if(station==1)
    {
        btn = SGButton::create("reward_lingq_hui.png", NULL, this, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);
        btn->type=1;
    }
    else if (station==2)
    {
        btn = SGButton::create("reward_font_ylq.png", NULL, this, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);
        btn->type=2;
    }
    else 
    {
        btn = SGButton::create("reward_lingq.png", NULL, this, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);//hehe
        btn->type=3;
    }
    
    SGMenu *menu1 = SGMenu::create(CCRectMake(0, btmhgt, s.width, s.height - btmhgt - headhgt));
    menu1->setTag(111);
    menu1->setPosition(CCPointZero);
    this->addChild(menu1,5);
    menu1->addChild(btn);
    btn->setPosition(ccpAdd(item->getPosition(), ccp(item->getContentSize().width*.4, 0)));
    
    updateitem(num, station);
}
void LvlGold_Item::getGoldHandler(cocos2d::CCNode *sender)
{
    
    if (state == 2)
    {
        SGMainManager::shareMain()->showMessage(str_LevelUpGetGold_str4);
        EFFECT_PLAY(MUSIC_BTN);
    }else if (state ==1)
    {
        SGMainManager::shareMain()->showMessage(str_LevelUpGetGold_str5);
        EFFECT_PLAY(MUSIC_BTN);
    }else if(delegate) {
        delegate->getGoldHandler(this);
    }
}

LvlGold_Item* LvlGold_Item::create(SGLevelUpDelegate *dlg,int num,int station)
{
	LvlGold_Item *instance = new LvlGold_Item();
	if (instance&&instance->init()) {
        instance->lvl = (num + 1)*5;
        instance->delegate = dlg;
        instance->state = station;
        instance->setView(num,station);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}

void LvlGold_Item::updateitem(int num, int station)
{
   ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_levelUpGetGold); 
    state = station;
    
    SGCCLabelTTF *font = (SGCCLabelTTF *)this->getChildByTag(99);
    SGCCLabelTTF *lab_up = (SGCCLabelTTF *)this->getChildByTag(10);
    SGCCLabelTTF *levl = (SGCCLabelTTF *)this->getChildByTag(11);
    SGCCLabelTTF *font_lvl = (SGCCLabelTTF *)this->getChildByTag(12);
    CCSprite *song = (CCSprite *)this->getChildByTag(13);
    CCSprite *yb = (CCSprite *)this->getChildByTag(14);
    SGCCLabelTTF *font_num =  (SGCCLabelTTF *)this->getChildByTag(15);
    CCSprite *font_v = (CCSprite *)this->getChildByTag(16);
    CCLabelAtlas* currvip = (CCLabelAtlas *)this->getChildByTag(17);
    yb->setPosition(ccpAdd(song->getPosition(), ccp(-60, 0)));
    
    if (num>10) {
        lvl = (num-10)*50 +1500;
        lab_up->setString(str_LevelUpGetGold_str6);
        levl->setPosition(ccpAdd(lab_up->getPosition(), ccp(70, 0)));
        font_lvl->setPosition(ccpAdd(levl->getPosition(), ccp(60, 0)));
    }else
    {
        lvl = (num + 1)*5;
        lab_up->setString(str_LevelUpGetGold_str2);
        levl->setPosition(ccpAdd(lab_up->getPosition(), ccp(60, 0)));
        font_lvl->setPosition(ccpAdd(levl->getPosition(), ccp(50, 0)));
    }
    levl->setString(CCString::createWithFormat("%d",lvl)->getCString());
    font_v->setVisible(false);
    currvip->setVisible(false);
    font->setString("");
    item->setVisible(true);
    btn->setVisible(true);
    lab_up->setVisible(true);
    levl->setVisible(true);
    font_lvl->setVisible(true);
    song->setVisible(true);
    yb->setVisible(true);
    font_num->setVisible(true);
    if (num == -1) {
        btn->setVisible(false);
        font->setString(str_LevelUpGetGold_str7);
        item->setVisible(false);
        btn->setVisible(false);
        lab_up->setVisible(false);
        levl->setVisible(false);
        font_lvl->setVisible(false);
        song->setVisible(false);
        yb->setVisible(false);
        font_num->setVisible(false);
        return;
    }
    
    if (num == 10) {
        btn->setVisible(false);
        font->setString(str_LevelUpGetGold_str8);
        item->setVisible(false);
        lab_up->setVisible(false);
        levl->setVisible(false);
        font_lvl->setVisible(false);
        song->setVisible(false);
        yb->setVisible(false);
        font_num->setVisible(false);
        return;
    }
    
	/*
	 put(5, 50);
	 put(10, 60);
	 put(15, 100);
	 put(20, 300);
	 put(25, 500);
	 put(30, 1000);
	 put(35, 3000);
	 put(40, 5000);
	 put(45, 8000);
	 put(50, 10000);
	 */
	
    if (num == 0) {
        font_num->setString("50");
    }else if (num == 1) {
        font_num->setString("60");
    }else if (num == 2) {
        font_num->setString("100");
    }else if (num == 3) {
        font_num->setString("300");
    }else if (num == 4){
		font_num->setString("500");
	}else if (num == 5){
		font_num->setString("1000");
	}else if (num == 6){
		font_num->setString("3000");
	}else if (num == 7){
		font_num->setString("5000");
	}else if (num == 8){
		font_num->setString("8000");
	}else if (num == 9){
		font_num->setString("10000");
	}
	else if (num == 11) {
        font_num->setString("30");
    }else if (num == 12) {
        font_num->setString("50");
    }else {
        font_num->setString(CCString::createWithFormat("%d",50 +25*(num-12))->getCString());
    }    
    if (num == 1 ||num == 9)
    {
        if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
            font_v->setVisible(true);
            currvip->setVisible(true);
            yb->setPosition(ccpAdd(song->getPosition(), ccp(-60, 0)));

        }
        
          font_v->setVisible(false);
          currvip->setVisible(false);

        if (num == 1) {
            currvip->setString("1");
        }
        if (num == 3) {
            currvip->setString("2");
        }
        
            }
    if (num != -1 || num != 10)
    {//1未到级别2已经领过了
        if(station!=(btn->type))
     {
        if(station==1)
        {            
            btn->init("reward_lingq_hui.png", "reward_lingq_hui.png", this, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);
            btn->type=1;
            
            
        }
        else if (station==2)
        {
            btn->init("reward_font_ylq.png", "reward_font_ylq.png", this, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);
            btn->type=2;
        }
        else
        {
            btn->init("reward_lingq.png", "reward_lingq.png", this, menu_selector(LvlGold_Item::getGoldHandler),CCPointZero,false,true);
            btn->type=3;
        }
     }

//        if (station == 1)
//        {            
//            btn->setImage("reward_lingq_hui.png");
//            CCSprite *a = CCSprite::createWithSpriteFrameName("reward_lingq_hui.png");
//            btn->setFontImage(a->displayFrame());
//        }
//        else if(station == 2)
//        {
//            btn->setImage("reward_font_ylq.png");
//            CCSprite *a = CCSprite::createWithSpriteFrameName("reward_font_ylq.png");
//            btn->setFontImage(a->displayFrame());
//        }
//        else
//        {
//            btn->setImage("reward_lingq.png");
//            CCSprite *a = CCSprite::createWithSpriteFrameName("reward_lingq.png");
//            btn->setFontImage(a->displayFrame());
//        }
    }
}
