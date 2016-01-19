//
//  SGBattleScoreLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-20.
//
//

#include "SGBattleScoreLayer.h"
#include "SGCreditCardCell.h"
#include "SGMainManager.h"
#include "SGBattleMainLayer.h"
#include "SGStoryRewardBox.h"
#include "SGMainLayer.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PvpFight.pb.h"
#include "SGCreditCardBox.h"
#include "SimpleAudioEngine.h"
#include "ShopModule.pb.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"

/**
 * 天梯赛主界面
 * 数据模型 SGBattleData
 **/

typedef enum{
    button_credit_hot = 0,
    button_credit_card,
    button_credit_juanzhou,
    button_credit_material,
    button_credit_prop,
    button_credit_other,
}creditCardType;

SGBattleScoreLayer::SGBattleScoreLayer(void)
:_osIndex(0),// 旧的道具
_selectIndex(-1), // 选中的按钮
_selectCell(0),  // 选中的道具
entertype(0)    //返回类型值
{

}
SGBattleScoreLayer::~SGBattleScoreLayer(void)
{
    CC_SAFE_RELEASE(datas);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_creditCardLayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SEASONWAR_CDLIST);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SEASONWAR_CDCHG);
}

SGBattleScoreLayer *SGBattleScoreLayer::create(CCArray *array,int type)
{
    SGBattleScoreLayer *layer = new SGBattleScoreLayer();
    if (layer && layer->init(NULL, sg_creditCardLayer))
    {
        layer->entertype = type;
        layer->initDatas(array);
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGBattleScoreLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);
    
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);

	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);
    
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_creditCardCell);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_creditCardCell);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_creditCardCell, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_creditCardCell);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_creditCardCell);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_creditCardLayer);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_SEASONWAR_CDLIST, this, callfuncO_selector(SGBattleScoreLayer::flushData));
    notification->addObserver(MSG_SEASONWAR_CDCHG, this, callfuncO_selector(SGBattleScoreLayer::chgCreditCard));
    
    

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(SGLayout::getPoint(kBottomCenter));
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setScaleY(size.height/1028);
    this->addChild(bgImg, -5);

    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,100);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,100);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,100);

    SGCCLabelTTF *title = SGCCLabelTTF::create(str_BattleScorelayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, title_bg->getPosition().y - title_bg->getContentSize().height*.52));
    this->addChild(title);

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGBattleScoreLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width*.39, -backBtn->getContentSize().height*.7)));
    // init buttons
    this->initButtons(title->getPosition().y-40);
    
    tableView->setFrame(CCRectMake(0, 0, 590, size.height - headH - 5));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-295, 0)));
    tableViewHeight = 220;
    tableViewColumns = 2;
    tableView->setDown(-10);
}

void SGBattleScoreLayer::initButtons(float posY)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    float posYY = posY-skewing(38);
    
    CCLabelTTF *lvl = CCLabelTTF::create(str_BattleScorelayer_str2,FONT_BOXINFO, 25);
    this->addChild(lvl);
    lvl->setAnchorPoint(ccp(0, 0.5));
    lvl->setPosition(ccp(size.width*.5 - 270, posY - 19));
    lvl->setColor(ccc3(0xe7, 0xc4, 0x6a));
    
    SGCCLabelTTF *level = SGCCLabelTTF::create(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getplayerPvpLevel())->getCString(),FONT_BOXINFO, 25);

    level->setAnchorPoint(ccp(0, 0.5));
    level->setPosition(ccpAdd(lvl->getPosition(), ccp(lvl->getContentSize().width, -1)));
    this->addChild(level);
    CCLabelTTF *jifen = CCLabelTTF::create(str_BattleScorelayer_str3,FONT_BOXINFO, 25);
    this->addChild(jifen);
    jifen->setAnchorPoint(ccp(0, 0.5));
    jifen->setPosition(ccp(size.width*.5 + 65, posY - 19));
    jifen->setColor(ccc3(0xe7, 0xc4, 0x6a));
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("2");
     nJifenMax =dic1->valueForKey("value")->intValue();
    Count = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",SGPlayerInfo::sharePlayerInfo()->getplayerPvpNum(),nJifenMax)->getCString(),FONT_BOXINFO, 25);
    Count->setAnchorPoint(ccp(0, 0.5));
    Count->setPosition(ccpAdd(jifen->getPosition(), ccp(jifen->getContentSize().width, -1)));
    this->addChild(Count);
    
    // rank buttons
    const char *suffix[] = {str_hot_sale,str_kapei, str_juanzhou,str_cailiao,str_porp,str_other};
//	const char *suffix[] = {"font_hot.png","officer.png", "equip.png", "prop.png","font_wjjsck.png","other.png"};
    const int btnTag[] = {button_credit_hot,button_credit_card, button_credit_juanzhou,button_credit_material,button_credit_prop, button_credit_other};
//	const int btnTag[] = {button_credit_hot,button_credit_officer, button_credit_equip, button_credit_props,button_credit_skill, button_credit_other};
    for (int ii = 0; ii < 6; ii++) {
        const char *sname = CCString::createWithFormat("%s", suffix[ii])->getCString();
        SGButton *button = NULL;
        
        if (ii ==0) {
            button = SGButton::createFromLocal("hotBtnBg.png", sname, this, menu_selector(SGBattleScoreLayer::getInfoAction), ccp(0, 2), FONT_PANGWA,ccWHITE,26,false, true);
        }else
        {
            button = SGButton::createFromLocal("unhot.png", sname, this, menu_selector(SGBattleScoreLayer::getInfoAction), ccp(0, 2), FONT_PANGWA,ccWHITE,26,false, true);

        }
        float posX = size.width*.5 - 195 + ii%3*195;
        button->setPosition(ccp(posX, posYY - ii/3*73));
        button->setTag(btnTag[ii]);
        this->addBtn(button);
    }
    CCSprite *line = CCSprite::createWithSpriteFrameName("jinbian.png");
    this->addChild(line);
    line->setScaleX(size.width/640);
    line->setPosition(ccp(size.width*.5, posY - 188));
}

void SGBattleScoreLayer::setSelecIndex(int index)
{
    CCMenuItemSprite *menuItem = NULL;
    if (_selectIndex != -1) {
        _osIndex = _selectIndex;
        menuItem = (CCMenuItemSprite *)menu->getChildByTag(_selectIndex);
        if (menuItem) menuItem->unselected();
    }
    _selectIndex = index;
    menuItem = (CCMenuItemSprite *)menu->getChildByTag(_selectIndex);
    if (menuItem) menuItem->selected();
}

void SGBattleScoreLayer::initDatas(CCArray *array)
{
    this->datas = CCArray::create();
    this->datas->retain();
    for (int ii = 0; ii < array->count(); ii++) {
        CCObject *obj = array->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
}

// button actions
void SGBattleScoreLayer::sortHandler()
{
}

void SGBattleScoreLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(true);
    if (entertype == 2)
    {
        this->removeFromParentAndCleanup(true);
        SGMainManager::shareMain()->showPlayerInfo();
    }else if (entertype == 1)
    {
        this->removeFromParentAndCleanup(true);
        SGMainManager::shareMain()->showShopLayer();
    }else
    {
    this->removeFromParentAndCleanup(true);
    SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
    mainScene->addShowLayer(layer);
    }
}

#pragma mark - -------tableView delegate-------
SNSTableViewCellItem* SGBattleScoreLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	   
	SGCreditCardCell *item = (SGCreditCardCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL)
    {
        item = SGCreditCardCell::create();
    }
    
    if (index<datas->count())
    {
        SGCreditCardModel *model =  (SGCreditCardModel *)datas->objectAtIndex(index);
        item->setData(model);
    }
    else
    {
        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGBattleScoreLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGBattleScoreLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag)
{
    EFFECT_PLAY(MUSIC_BTN);
    if (btnTag) {
        return;
    }
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    _selectCell = index;
    if (index < datas->count()) {
        int score = SGPlayerInfo::sharePlayerInfo()->getplayerPvpNum();
        int level = SGPlayerInfo::sharePlayerInfo()->getplayerPvpLevel();
        SGCreditCardModel *model =  (SGCreditCardModel *)datas->objectAtIndex(index);
        if (level < model->getpvpLev()) {
            SGMainManager::shareMain()->showMessage(str_BattleScorelayer_str4);
        }else if (score < model->getCostScore()) {
            SGMainManager::shareMain()->showMessage(str_BattleScorelayer_str5);
        }else if (model->getCurrCount() >= model->getMaxCout()) {
            SGMainManager::shareMain()->showMessage(str_BattleScorelayer_str6);
        }else { // OK
            DDLog("credit---------------OK");
            _selectCell = index;
            SGCreditCostBox *basebox = SGCreditCostBox::create(this, model->getUId());
            basebox->initWithInfo(score, model->getCostScore());
            SGMainManager::shareMain()->showBox(basebox);
        }
    }
}

void SGBattleScoreLayer::getInfoAction(CCNode *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    // 1=officer, 2=equip, 3=propers, 4=other
//    if (sender->getTag() == 4) {
//        SG_SHOW_WINDOW(str_SHOWWINDOW_1001);
//    }
//    else{
    main::PvpShopListRequest *request = new main::PvpShopListRequest();
//    CCMenuItem *menu = (CCMenuItem *)sender;
    this->setSelecIndex(sender->getTag());
    request->set_type(sender->getTag());
//    switch (sender->getTag()) {
//           
//        case 1: {
//            request->set_type(1);
//        } break;
//        case button_credit_officer: {
//            request->set_type(2);
//        } break;
//        case button_credit_equip: {
//            request->set_type(3);
//        } break;
//        case button_credit_props: {
//            request->set_type(4);
//        } break;
//        case button_credit_skill: {
//            request->set_type(5);
//        } break;
//        case button_credit_other: {
//            request->set_type(6);
//        } break;
//        default:
//            break;
//    }
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_CDLIST, request);
//    }
}

void SGBattleScoreLayer::getInfoByIndex(int index)
{
    // 1=officer, 2=equip, 3=propers, 4=other
    main::PvpShopListRequest *request = new main::PvpShopListRequest();
    request->set_type(index);
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_CDLIST, request);
}

SNSTableView* SGBattleScoreLayer::getTableView()
{
    return tableView;
}

void SGBattleScoreLayer::flushData(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpShopListResponse *response = (main::PvpShopListResponse *)req->m_msg;
    if (response) {

        CCArray *rArr = this->datas; // going to release
        CCArray *_datas = CCArray::create();
        _datas->retain();
        
        int size = response->items_size();
        DDLog("credit main-------------------%d", size);
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
            
            _datas->addObject(model);
        }
        this->datas = _datas;
        tableView->reloadData();
        
        // release old datas
        rArr->removeAllObjects();
        rArr->release();
        rArr = NULL;
    }
}

void SGBattleScoreLayer::chgCreditCard(CCObject *sender)
{
    DDLog("credit main-------------------chgCreditCard");
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpShopPurchResponse *response = (main::PvpShopPurchResponse *)req->m_msg;
    if (response && response->result() == 1) {
        SGCreditCardModel *model = (SGCreditCardModel *)this->datas->objectAtIndex(_selectCell);
//        CCLOG("%d",model->getItemType());
        int icount = 1; // 道具卡 1个，消耗类道具 N个
        int iType = model->getModelType();
        const char *iName = NULL;
        SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
//        if (iType == 1) { // 道具卡
//            SGBaseMilitaryCard *card = NULL;
//            if (response->officercard().itemid() > 0) {
//                card = createOfficerCardWithInfo(response->officercard());
//                playInfo->addOfficerCard((SGOfficerCard *)card);
//            }else if (response->equipmentcard().itemid() > 0) {
//                card = createEquipCardWithInfo(response->equipmentcard());
//                playInfo->addEquipCard((SGEquipCard *)card);
//            }else if (response->propscard().itemid() > 0) {
//                card = createPropCardWithInfo(response->propscard());
//                playInfo->addPropCard((SGPropsCard *)card);
//            }
//            if (card) {
//                 iName = card->getOfficerName()->getCString();
//            }
//        }
        switch (iType) {
            case 0:
            {
                iName = str_yuanbao;

            }
                break;
            case 1:
            {
                iName = str_Tong_money;
                
            }
                break;
            case 3:
            {
                iName = str_jungong;
                
            }
                break;
            case 4:
            {
                iName = str_Small_laba;
                
            }
                break;
            case 5:
            {
                iName = str_liandianmifang;
                
            }
                break;
            case 6:
            {
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(model->getitemId());
                iName = temp->getOfficerName()->getCString();
                
            }
                break;
            case 7:
            {
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(model->getitemId());
                iName = temp->getEquipName()->getCString();
                
            }
                break;
            case 9:
            {
                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(model->getitemId());
                iName = temp->getPropsName()->getCString();
                
            }
                break;
            case 10:
            {
                SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(model->getitemId());
                iName = temp->getPiecesName()->getCString();
                
            }
                break;
            case 15:
            {
                SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(model->getitemId());
                iName = temp->getConsumeName()->getCString();
                
            }
                break;
                
            default:
                break;
        }
//    else {
//            icount = model->getitemId();
//            int pCount = 0;
//            if (iType == 2) {
//                iName = "铜钱";
//                pCount = playInfo->getPlayerCoins() + icount;
//                playInfo->setPlayerCoins(pCount);
//            } else if (iType == 3) {
//                iName = "军功";
//                pCount = playInfo->getplayerAward() + icount;
//                playInfo->setplayerAward(pCount);
//            } else if (iType == 4) {
//                iName = "小喇叭";
//                pCount = playInfo->getSpeakerCount() + icount;
//                playInfo->setSpeakerCount(pCount);
//            } else if (iType == 5) {
//                iName = "炼丹秘方";
//                pCount = playInfo->getPlayerliandan() + icount;
//                playInfo->setPlayerliandan(pCount);
//            } else if (iType == 6) {
//                iName = "元宝";
//                pCount = playInfo->getPlayerGold() + icount;
//                playInfo->setPlayerGold(pCount);
//            }
//        }
        
        CCString *message = CCString::createWithFormat(str_BattleScorelayer_str7, iName, icount);
        SGMainManager::shareMain()->showMessage(message->getCString());
        
        playInfo->setplayerPvpNum(playInfo->getplayerPvpNum() - model->getDisCount());
    
        //刷新数据
        int count = model->getCurrCount();
        model->setCurrCount(count+1);
        Count->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getplayerPvpNum())->getCString());

        tableView->reloadData();
    }
}
