//
//  SGBattleRankLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-16.
//
//

#include "SGBattleRankLayer.h"
#include "SGMainManager.h"
#include "SGBattleRankCell.h"
#include "SGRankPlayer.h"
#include "SGBattleMainLayer.h"
#include "SGBattleHelpLayer.h"

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PvpFight.pb.h"
#include "SGTestSwitch.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGStaticDataManager.h"

SGBattleRankLayer::SGBattleRankLayer(void)
:_selectIndex(0), // 选中的按钮
type(0),
notice(NULL)
{
}
SGBattleRankLayer::~SGBattleRankLayer(void)
{
    CC_SAFE_RELEASE(datas);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SEASONWAR_RANK);
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_rankLayer);
}

SGBattleRankLayer *SGBattleRankLayer::create(CCArray *array,int num)
{
    SGBattleRankLayer *layer = new SGBattleRankLayer();
    if (layer && layer->init(NULL, sg_rankLayer))
    {
        layer->initDatas(array);
        layer->type = 3;
        layer->number[3]=num;
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGBattleRankLayer::initView()
{
 
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_rankLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer, LIM_PNG_AS_PNG);
    
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_rankLayer);

    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getVisitHgt();
    float head2 = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_SEASONWAR_RANK, this, callfuncO_selector(SGBattleRankLayer::flushData));
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_rankLayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, stdSize.height/2-(size.height-headH-bottomH)/2, bgImg->getContentSize().width, size.height-headH-bottomH);

    
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
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_BattleRankLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(title);
    
    
    this->initButtons(title_bg->getPosition().y-title_bg->getContentSize().height*.5-head2+headH);
    CCSprite *line = CCSprite::createWithSpriteFrameName("jinbian.png");
    line->setScaleY(0.5f);
    line->setScaleX(size.width/line->getContentSize().width);
    line->setPosition(ccp(size.width*.5f, title->getPositionY()+title->getContentSize().height*.5));
    //this->addChild(line);
    
    notice = SGCCLabelTTF::create(str_BattleRankLayer_str2, FONT_BOXINFO, 32);
    notice->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -30)));
    notice->setVisible(false);
    this->addChild(notice, 10);
    if (this->datas->count()==0) {
        notice->setVisible(true);
    }
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    
    SGCCLabelTTF *dqjf = SGCCLabelTTF::create(str_BattleRankLayer_str3, FONT_BOXINFO, 26 , ccYELLOW);
   
    dqjf->setAnchorPoint(ccp(0, 0.5f));
    dqjf->setPosition(ccpAdd(title_bg->getPosition(), ccp(-230, (headH-head2)*.5+10-title_bg->getContentSize().height*.5)));
    this->addChild(dqjf);
    
    CCString *string2 = CCString::createWithFormat("%d", player->getplayerPvpLevel());
    SGCCLabelTTF *lvalue2 = SGCCLabelTTF::create(string2->getCString(), FONT_BOXINFO, 30,ccWHITE);
    
    lvalue2->setAnchorPoint(ccp(0, 0.5f));
    lvalue2->setPosition(ccpAdd(dqjf->getPosition(), ccp(26*5, 0)));
    this->addChild(lvalue2);

    SGCCLabelTTF *junx = SGCCLabelTTF::create(str_BattleRankLayer_str4, FONT_BOXINFO, 26 , ccYELLOW);
   
    junx->setAnchorPoint(ccp(0, 0.5f));
    junx->setPosition(ccpAdd(title_bg->getPosition(), ccp(50, (headH-head2)*.5+10-title_bg->getContentSize().height*.5)));
    this->addChild(junx);
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("2");
    int nJifen =dic1->valueForKey("value")->intValue();

    string2 = CCString::createWithFormat("%d/%d", player->getPvpCredit(),nJifen);
    SGCCLabelTTF *lvalue1 = SGCCLabelTTF::create(string2->getCString(), FONT_BOXINFO, 30 , ccWHITE);
    lvalue1->setAnchorPoint(ccp(0, 0.5f));
    lvalue1->setPosition(ccpAdd(junx->getPosition(), ccp(26*5, 0)));
    this->addChild(lvalue1);
    
    SGCCLabelTTF *jx = SGCCLabelTTF::create(str_BattleRankLayer_str5, FONT_BOXINFO, 26 , ccYELLOW);
    jx->setAnchorPoint(ccp(0, 0.5f));
    jx->setPosition(ccpAdd(title_bg->getPosition(), ccp(-230, (headH-head2)*.5-30-title_bg->getContentSize().height*.5)));
    this->addChild(jx);
    
    string2 = CCString::createWithFormat("%s", (player->getMilitaryRankName())->getCString());
    SGCCLabelTTF *lvalue3 = SGCCLabelTTF::create(string2->getCString(), FONT_BOXINFO, 30 , ccWHITE);
    lvalue3->setAnchorPoint(ccp(0, 0.5f));
    lvalue3->setPosition(ccpAdd(jx->getPosition(), ccp(26*5, 0)));
    this->addChild(lvalue3);
    
    
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - head2 - 10));//headH
    //tableView->setFrame(CCRectMake(0, 0, size.width*0.5, (size.height - bottomH - head2 - 10)*0.5));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    tableViewHeight = 130;
    tableView->setDown(-2);//-2
}

void SGBattleRankLayer::findrankhandler()
{
    CCLog("dddddddddddd");
}

void SGBattleRankLayer::initButtons(float posY)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getVisitHgt();
    float head2 = SGMainManager::shareMain()->getTotleHdHgt();
    SGButton *helpBtn = SGButton::create("chatbtn.png",
                                         "upwar_button_help.png",
                                         this,
                                         menu_selector(SGBattleRankLayer::helpHandler),
                                         ccp(0, 0),
                                         false,
                                         true);
//    this->addBtn(helpBtn);
    helpBtn->setPosition(ccp(size.width-helpBtn->getContentSize().width*0.65f, posY));
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGBattleRankLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.65f, posY-headH+head2));
    
    // rank buttons
    float posYY = posY-15;
    const char *suffix[] = {"kua", "ben", "wei", "shu", "wu"};
    const int btnTag[] = {rank_button_kua, rank_button_ben, rank_button_wei, rank_button_shu, rank_button_wu};
    for (int ii = 0; ii < 5; ii++) {
        const char *sname = CCString::createWithFormat("upwar_font_s%s_n.png", suffix[ii])->getCString();
        SGButton *button = SGButton::create("store_exchangebtnbg.png", sname, this, menu_selector(SGBattleRankLayer::getInfoAction), ccp(0, 0), false, true);
        float posX = (ii-2)*(button->getContentSize().width+5);
        button->setPosition(ccpAdd(ccp(size.width*.5f, 0), ccp(posX, posYY)));
        button->setTag(btnTag[ii]);
        this->addBtn(button);
    }
}

void SGBattleRankLayer::setSelecIndex(int index)
{
    CCMenuItemSprite *menuItem = NULL;
    if (_selectIndex != 0) {
        menuItem = (CCMenuItemSprite *)menu->getChildByTag(_selectIndex);
        if (menuItem) menuItem->unselected();
    }
    _selectIndex = index;
    menuItem = (CCMenuItemSprite *)menu->getChildByTag(_selectIndex);
    if (menuItem) menuItem->selected();
}

void SGBattleRankLayer::initDatas(CCArray *array)
{
    this->datas = CCArray::create();
    this->datas->retain();
    for (int ii = 0; ii < array->count(); ii++) {
        CCObject *obj = array->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
}

// button actions
void SGBattleRankLayer::helpHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBattleHelpBox *box = SGBattleHelpBox::create(this);
    SGMainManager::shareMain()->showBox(box);
}

void SGBattleRankLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->removeFromParentAndCleanup(true);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainlayer = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
    mainlayer->visitshow();
    mainlayer->setPortraitMenuCanTouch(true);
    SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
    mainScene->addShowLayer(layer);
}

#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGBattleRankLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);//从0开始
	CCSize size = tableView->getItemSize();
    
	SGBattleRankCell *item = (SGBattleRankCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
//        tableView->moveToIndexPath()
    if (item == NULL) {
        item = SGBattleRankCell::create();
    }
    item->resetBgImg(index);
    
    if (index<datas->count()) {
        SGRankPlayer *player =  (SGRankPlayer *)datas->objectAtIndex(index);
        item->setData(player);
        if ((index == datas->count()-1)&&(index<number[type]-1)) {
            newindex = index-1;
            main::PvpTopRequest *request = new main::PvpTopRequest();
            request->set_page(datas->count()/25+1);
            request->set_type(type);
            SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_RANK, request);
        }
    }else {
        //        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGBattleRankLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGBattleRankLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    // get player info and show on layer
}
float SGBattleRankLayer::tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    if (size.height==1136) {
        return 140;
    }
    else if(size.height==960){
        return 140;
    }
    else
        return 160;
	
}

void SGBattleRankLayer::getInfoAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    DDLog("rank--------------getInfoAction");
    // 0=shu, 1=wei, 2=wu, 3=kuafu, 4=benfu
    main::PvpTopRequest *request = new main::PvpTopRequest();
    request->set_page(1);
    request->set_type(0);
    
    CCMenuItem *menu = (CCMenuItem *)sender;
    this->setSelecIndex(menu->getTag());
    switch (menu->getTag()) {
        case rank_button_kua: {
            request->set_type(3);
            CCLOG("rank--------------rank_button_kua");
        } break;
        case rank_button_ben: {
            request->set_type(4);
            CCLOG("rank--------------rank_button_ben");
        } break;
        case rank_button_wei: {
            request->set_type(1);
            CCLOG("rank--------------rank_button_wei");
        } break;
        case rank_button_shu: {
            request->set_type(0);
            CCLOG("rank--------------rank_button_shu");
        } break;
        case rank_button_wu: {
            request->set_type(2);
            CCLOG("rank--------------rank_button_wu");
        } break;
        default:
            break;
    }
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_RANK, request);
}

void SGBattleRankLayer::reAskData()
{
//    SGSocketRequest *req = (SGSocketRequest *)sender;
//    main::PvpTopResponse *response = (main::PvpTopResponse *)req->m_msg;
//    if (response) {
//        int size = response->players_size();
//        DDLog("rank rank-------------------%d", size);
//        for (int ii = 0; ii < size; ii++) {
//            main::TopPlayerMessage player = response->players(ii);
//            // create(const char *name, int hid, int rank, int levl, int ftime, float rwin);
//            const char* var1 = player.name().c_str();
//            int var2 = player.headid();
//            int var3 = player.toppos();
//            int var4 = player.toplevel();
//            int var5 = player.fighttimes();
//            float var6 = player.winratio();
//            SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6);
//            _datas->addObject(ranker);
//        }
//
//    
}

//void SGBattleRankLayer::flushData(CCObject *sender)
//{
//    CCArray *rArr = this->datas; // going to release
//    
//    CCArray *_datas = CCArray::create();
//    _datas->retain();
//    SGSocketRequest *req = (SGSocketRequest *)sender;
//    main::PvpTopResponse *response = (main::PvpTopResponse *)req->m_msg;
//    if (response) {
//        int size = response->players_size();
//        DDLog("rank rank-------------------%d", size);
//        for (int ii = 0; ii < size; ii++) {
//            main::TopPlayerMessage player = response->players(ii);
//            // create(const char *name, int hid, int rank, int levl, int ftime, float rwin);
//            const char* var1 = player.name().c_str();
//            int var2 = player.headid();
//            int var3 = player.toppos();
//            int var4 = player.toplevel();
//            int var5 = player.fighttimes();
//            float var6 = player.winratio();
//            SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6);
//            _datas->addObject(ranker);
//        }
//        // 测试代码
////        if (false && size == 0) {
////            for (int ii = 0; ii < 100; ii++) {
////                int rand = arc4random() % 100;
////                const char* var1 = CCString::createWithFormat("NAME%d", rand)->getCString();
////                int var2 = rand%3+1;
////                int var3 = ii+1;
////                int var4 = 1;
////                int var5 = 10;
////                float var6 = 0.3f;
////                SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6);
////                _datas->addObject(ranker);
////            }
////        }
//        this->datas = _datas;
//        tableView->reloadData();
//        if (this->datas->count()==0) {
//            notice->setVisible(true);
//        }else {
//            notice->setVisible(false);
//        }
//        
//        // release old datas
//        rArr->removeAllObjects();
//        rArr->release();
//        rArr = NULL;
//    }
//}
//
void SGBattleRankLayer::flushData(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpTopResponse *response = (main::PvpTopResponse *)req->m_msg;
    if (response)
    {
        if (response->type()==type)
        {
            CCLog("sssssssssss");
            int size = response->players_size();
            
            DDLog("rank rank-------------------%d", size);
            for (int ii = 0; ii < size; ii++) {
                main::TopPlayerMessage player = response->players(ii);
                // create(const char *name, int hid, int rank, int levl, int ftime, float rwin);
                const char* var1 = player.name().c_str();
                int var2 = player.headid();
                int var3 = player.toppos();
                int var4 = player.toplevel();
                int var5 = player.fighttimes();
                float var6 = player.winratio();
               
                SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6,0,NULL);//最后是服务器id和名字
                datas->addObject(ranker);
                
            }
            tableView->reloadData(false);

        }
        else
        {
            CCArray *rArr = this->datas; // going to release
            CCArray *_datas = CCArray::create();
            _datas->retain();
            int size = response->players_size();
            type = response->type();
            number[type]=response->allcount();
            DDLog("rank rank-------------------%d", size);
            for (int ii = 0; ii < size; ii++) {
                main::TopPlayerMessage player = response->players(ii);
                // create(const char *name, int hid, int rank, int levl, int ftime, float rwin);
                const char* var1 = player.name().c_str();
                int var2 = player.headid();
                int var3 = player.toppos();
                int var4 = player.toplevel();
                int var5 = player.fighttimes();
                float var6 = player.winratio();
                
                SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6,0,NULL);//最后是服务器id和名字
                _datas->addObject(ranker);
            }
            this->datas = _datas;
            tableView->reloadData();
            if (this->datas->count()==0) {
                notice->setVisible(true);
            }else {
                notice->setVisible(false);
            }
            
            // release old datas
            rArr->removeAllObjects();
            rArr->release();
            rArr = NULL;


        }
        
    }
    
}
