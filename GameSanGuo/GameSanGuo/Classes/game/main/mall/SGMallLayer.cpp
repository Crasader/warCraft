#include "SGMallLayer.h"
#include "SGMallCell.h"
#include "SGFirstLayer.h"
#include "RoleModule.pb.h"
#include "SGDrawCards.h"
#include "SGDrawCards.h"
#include "SGCantAdvanceBox.h"
#include "RoleShopModule.pb.h"
#include "SpecialBattleModule.pb.h"
#include <algorithm>
#include "SGTools.h"
#include "SGBaseItemType.h"
#include "ArenaModule.pb.h"
#include "SGMainManager.h"
#include "SGShowString.h"
#include "SGButton.h"

const int EACH_PAGE_NUM = 6;
const int NOW_LAYER_ZORDER = 2000;

//初始化静态数据成员
CargoInformation * SGMallLayer::_cargoInfoBuySuccess = NULL;
SGMallCell * SGMallLayer::_buyMallCell = NULL;
bool SGMallLayer::_isBuyResponsed = true;
ShopEnterType SGMallLayer::m_enterType = SHOP_LAYER;

std::string getCurrencyNameByPayKind(int payKind)
{
    std::string ret;
    SGBaseItemType type = (SGBaseItemType)payKind;
    switch (type)
    {
        case BIT_INGOT://元宝
            ret = str_Currency_YanBao;
            break;
        case BIT_GOLD://铜钱
            ret = str_Currency_TongQian;
            break;
        case BIT_GRADE: //天梯积分
            ret = str_Currency_TianTiJiFen;
            break;
        case BIT_SOUL://军魂
            ret = str_Currency_JunHun;
            break;
        case BIT_JADE://勾玉
            ret = str_chinese_jade;
            break;
        case BIT_ARENA_SCORE:
            ret = str_arena_score;
            break;
        default:
            break;
    }
    return ret;
}







///////////////////////////////


CCSprite * SGMallUtil::createSpriteByCargoId(int itemId, int type, int num , bool isShowNum)
{
    CCSprite* ret = NULL;
    if (isShowNum)
    {
        ret = SGDrawCards::createComplexSprite(itemId, type, num, sg_mallLayer);
    }
    else
    {
        ret = SGDrawCards::createNormalSprite(itemId, type, sg_mallLayer);
    }
    return ret;
    
    
    /*
    CCSprite* sprite = SGDrawCards::drawallcards(itemId, type, sg_mallLayer);
    if (NULL == sprite )
    {
        CCLog("未找到对应物品的图片，itemid=%d，itemtype=%d",itemId,type);
        return sprite;
    }
    //数量
    if (0 < num && isShowNum )
    {
        char str[20];
        sprintf(str, "%d",num);
        SGCCLabelTTF * labelNum = SGCCLabelTTF::create(str, FONT_PANGWA, 20);
        labelNum->setCascadeColorEnabled(true);
        labelNum->setAnchorPoint(ccp(0, 0));
        labelNum->setPosition(ccp(sprite->getContentSize().width - 2 * labelNum->getContentSize().width , 0 ) );
        sprite->addChild(labelNum);
    }
    
    return sprite;
    */
}

CCSprite * SGMallUtil::createSpriteByPayType(int type)
{
    CCSprite * sprite = NULL;
    switch(type)
    {
        
        case 0 :  //元宝
            sprite = CCSprite::createWithSpriteFrameName("store_yb.png");
            break;
        case 1 : //铜钱
            sprite = CCSprite::createWithSpriteFrameName("coinicon.png");
            break;
        case 2: //天梯积分
            sprite = CCSprite::createWithSpriteFrameName("store_tianti.png");
            break;
        case 3 : //军功
            sprite = CCSprite::createWithSpriteFrameName("jungong32.png");
            break;
            
        case 11: //军魂
            sprite = CCSprite::createWithSpriteFrameName("store_junhun.png");
            break;
        case 18://勾玉
            sprite = CCSprite::createWithSpriteFrameName("gouyuIcon.png");
            break;
        case 19://征服点---竞技场币
            sprite = CCSprite::createWithSpriteFrameName("arean_coin_small.png");
            break;
        default :
            sprite = CCSprite::createWithSpriteFrameName("store_yb.png"); //暂时用元宝图片，具体策划未出。
            break;
            
            //CCAssert(false, "没有该支付类型的图片资源。");
    }
    
    return sprite;
}


SGCCLabelTTF * SGMallUtil::createLabelTitleByCargoId(int type , int id , int fontSize , bool isDescriptor)
{
    SGCCLabelTTF* ret = NULL;
    CCString* name = NULL;
    ccColor3B color = ccGREEN;
    int starLev = 3;
    switch (type)
    {
        case 6://武将  todo:暂时不卖
        {
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(id);
            //if (false == isDescriptor)
                name = temp->getOfficerName();
            starLev = temp->getOfficerCurrStarLevel();
        }
            break;
        case 7://装备
        {
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(id);
            if(!isDescriptor)
                name = temp->getEquipName();
            else
                name = temp->getFateDesc();
          
            starLev = temp->getEquipCurrStarLevel();
        }
            break;
        case 9://道具
        {
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(id);
            if ( !isDescriptor)
                name = temp->getPropsName();
            else
                name = temp->getPropsInfo();
            
            starLev = temp->getPropsCurrStarLevel();
            break;
        }
        case 10://碎片
        {
            SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(id);
            if (!isDescriptor)
                name = temp->getPiecesName();
            else
                name = temp->getPiecesDesc();
            
            starLev = temp->getStarLevel();
            break;
        }
        case 15://消耗品
        {
            SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(id);
            if (!isDescriptor)
                name = temp->getConsumeName();
            else
                name = temp->getConsumeDesc();
            starLev = temp->getConsumeStarLvl();
        }
            break;
        case 17://材料
        {
            SGMaterialDataModel *temp = SGStaticDataManager::shareStatic()->getMaterialById(id);
            if (!isDescriptor)
                name = temp->getMaterialName();
            else
                name = temp->getMaterialDesc();
            starLev = temp->getMaterialBorderColor();
        }
            break;
    }
    starLev = starLev<3?3:starLev;
    starLev = starLev>6?6:starLev;
    ccColor3B colorArr[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
    color = colorArr[starLev-3];
    if (!isDescriptor)
        ret = SGCCLabelTTF::create(name->getCString(), FONT_PANGWA, fontSize , color);
    else
        ret = SGCCLabelTTF::create(name->getCString(), FONT_BOXINFO, fontSize,  CCSizeMake(400,150), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    
    return ret;
}


int SGMallUtil::getNumByCargoIdAndType(int itemType, int itemId)
{
    int num = 0;
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    SGBaseItemType type = (SGBaseItemType) itemType;
    switch (type)
    {
        case BIT_INGOT ://元宝
            num = player->getPlayerGold();
            break;
        case BIT_GOLD://铜钱
            num = player->getPlayerCoins();
            break;
        case BIT_GRADE: //天梯积分
            num = player->getplayerPvpNum();
            break;
        case BIT_SOUL: //军魂
            num = player->getJunHun();
            break;
        case BIT_RECIPE://炼丹秘方
            num = player->getPlayerliandan();
            break;
        case BIT_OFFICER://武将
        {
            CCArray * officer =  player->getOfficerCards();
            for ( int i=0 ; i < officer->count() ; ++i)
            {
                if ( itemId ==( (SGOfficerCard *) (officer->objectAtIndex(i) ) )->getItemId()  )
                    num++;
            }
        }
            break;
        case BIT_EQUIP ://装备
        {
            CCArray * equip =  player->getEquipCards();
            for ( int i=0 ; i < equip->count() ; ++i)
            {
                if ( itemId ==( (SGEquipCard *) (equip->objectAtIndex(i) ) )->getItemId()  )
                    num++;
            }
        }
            break;
        
        case BIT_PROP://道具
        {
            CCArray * prop =  player->getPropCards();
            SGPropsCard * target = NULL;
            for ( int i=0 ; i < prop->count() ; ++i)
            {
                if ( itemId ==( (SGPropsCard *) (prop->objectAtIndex(i) ) )->getItemId()  )
                {
                    target =  (SGPropsCard *) (prop->objectAtIndex(i) ) ;
                    break;
                }
            }
            if (target)
                num = target->getCurNum() ;
            //未知获取数量
            else
                num = 0;
        }
            break;
        case BIT_PIECE://碎片
        {
            CCArray * suipian =  player->getPiecesCards();
            SGPiecesCard * target = NULL;
            for ( int i=0 ; i < suipian->count() ; ++i)
            {
                if ( itemId ==( (SGPiecesCard *) (suipian->objectAtIndex(i) ) )->getItemId()  )
                {
                    target =  (SGPiecesCard *) (suipian->objectAtIndex(i) );
                    break;
                }
            }
            
            if(target)
                num = target->getPiecesCurNum();
            else
                num = 0;
            
        }
            break;
        
        case BIT_CONSUMABLE://消耗品
        {
            CCArray * consumable =  player->getConsumableCards();
            SGConsumableCard * target = NULL;
            for ( int i=0 ; i < consumable->count() ; ++i)
            {
                if ( itemId ==( (SGConsumableCard *) (consumable->objectAtIndex(i) ) )->getItemId()  )
                {
                    target = (SGConsumableCard *) (consumable->objectAtIndex(i) );
                    break;
                }
            }
            if (target)
                num = target->getCurrNum();
            //未知获取数量
            else
                num = 0;
        }
            break;
        //材料
        case BIT_MATERIAL://材料
        {
            CCArray * materials =  player->getMaterialCards();
            SGMaterialCard * target = NULL;
            for ( int i=0 ; i < materials->count() ; ++i)
            {
                if ( itemId ==( (SGMaterialCard *) (materials->objectAtIndex(i) ) )->getItemId()  )
                {
                    target = (SGMaterialCard *) (materials->objectAtIndex(i) );
                    break;
                }
            }
            if (target)
                num = target->getMaterialCurrNum();
            //未知获取数量
            else
                num = 0;
        }
            break;
        case BIT_JADE://勾玉
            num = player->getPlayerJewels();
            break;
        case BIT_ARENA_SCORE://征服点
            num = player->getPlayerArenaScore();
            break;

        default:
        {
        }
           break;
    }

    return num;
}

SGMallLayer::SGMallLayer() :
_nowPage(1),
_pointLayer(NULL),
otherInfoBg(NULL),
_totalPage(0),
_currentData(NULL),
_selectIndex(0),
spriteSelectCircle(NULL),
leftArrows(NULL),
rightArrows(NULL),
goodsKindBg(NULL),
sprite_jiang(NULL),
sprite_zhuang(NULL),
sprite_bao(NULL),
refreshBtn(NULL)
{
    _mallData = NULL;
    
    _currentData = CCArray::create();
    _currentData->retain();
    
    //禁用头像 和首充 的触摸。 析构中撤销。
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    
    this->datas = CCArray::create();
    datas->retain();
    
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_CARGO_TRY_TO_BUY, this, callfuncO_selector(SGMallLayer::buyCargoOnResponse ) );
    
    notification->addObserver(MSG_CARGO_REFRESH, this, callfuncO_selector(SGMallLayer::refreshDataOnResponse ) );
}


SGMallLayer::~SGMallLayer()
{
    if (_mallData)
    {
        delete _mallData;
    }
    
    _currentData->release();
    this->datas->release();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(true);
    SGMainManager::shareMain()->setChatBtnVisiable(true);
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_mallLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
	notification->removeObserver(this, MSG_CARGO_REFRESH);
    notification->removeObserver(this, MSG_CARGO_TRY_TO_BUY);
    
    m_enterType = SHOP_LAYER;
}


SGMallLayer * SGMallLayer::create(SGMallData * mallData)
{
    //需要填充SGMallData结构。否则直接返回NULL
    if (NULL == mallData)
    {
        return NULL;
    }
    
    SGMallLayer * pMallLayer = new SGMallLayer();
    if(pMallLayer && pMallLayer->init(NULL, sg_mallLayer)  )
    {
        pMallLayer->initView(mallData);
        pMallLayer->autorelease();
    }
    else
        CC_SAFE_DELETE(pMallLayer);
    return pMallLayer;
}


void SGMallLayer::initView(SGMallData * mallData)
{
    //关闭上层触摸
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);
    SGMainManager::shareMain()->setChatBtnVisiable(false);
    
    _mallData = mallData;
    if (this->datas)
    {
        this->datas->removeAllObjects();
    }
    
    //加载图片资源。
    ResourceManager::sharedInstance()->bindTexture("sgmalllayer/sgmalllayer.plist", RES_TYPE_LAYER_UI, sg_mallLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_mallLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_mallLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_mallLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_mallLayer);

    //获取可放置当前layer上下的高度。
    float upPos = SGMainManager::shareMain()->getHeadHeight();
    float downPos = SGMainManager::shareMain()->getBottomHeight();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float centerX = winSize.width/2;
    
    //放置标题
    CCSprite * title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setPosition(ccp( centerX ,winSize.height-title_bg->getContentSize().height/2));
    title_bg->setAnchorPoint(ccp(0.5f, 0.5f));
    this->addChild(title_bg);
    SGCCLabelTTF * title_text = SGCCLabelTTF::create(_mallData->_mallTitle.c_str(), FONT_PANGWA,32 );
    title_text->setAnchorPoint(ccp(0.5f, 0.5f));
    title_text->setPosition(title_bg->getPosition());
    this->addChild(title_text);
    
    
    //返回按钮
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this,menu_selector(SGMallLayer::onBackBtnClicked),CCPointZero,FONT_PANGWA,ccWHITE,32);
    //backBtn->setScale(1.05); 调节文字的和背景图片的适配。
    backBtn->setAnchorPoint(ccp(0, 1));
    backBtn->setPosition(ccp(10 , winSize.height));
    addBtn(backBtn);
    
    //物品分类的背景
    goodsKindBg = CCSprite::createWithSpriteFrameName("mall_goodsKindBg.png");
    goodsKindBg->setAnchorPoint(ccp(0.5f, 0.5f));
    goodsKindBg->setPosition(ccp(centerX, title_bg->getPositionY() - title_bg->getContentSize().height/2 - goodsKindBg->getContentSize().height/2));
    goodsKindBg->setScaleX(CCDirector::sharedDirector()->getWinSize().width/ goodsKindBg->getContentSize().width );
    this->addChild(goodsKindBg);
    
    //标题下方的一个长杆
    CCSprite * spriteStick = CCSprite::createWithSpriteFrameName("boss_title_bar.png"); //图片在public4中。
    spriteStick->setAnchorPoint( ccp(0.5f, 0.5f) );
    spriteStick->setPosition( ccp(centerX, goodsKindBg->getPositionY() + goodsKindBg->getContentSize().height/2 ));
    this->addChild(spriteStick,goodsKindBg->getZOrder() +1 );
    
    //小女孩
    CCSprite * girl = CCSprite::createWithSpriteFrameName("mall_cartoonGirl.png");
    girl->setAnchorPoint(ccp(0, 0));
    girl->setPosition(ccp(  winSize.width - 200 ,  goodsKindBg->getPositionY() - goodsKindBg->getContentSize().height/2 + 5) );
    this->addChild(girl,NOW_LAYER_ZORDER+2);
    
    //钱包和时间的背景图片
    otherInfoBg = CCSprite::createWithSpriteFrameName("store_redbg.png");
    otherInfoBg->setScaleX(winSize.width / otherInfoBg->getContentSize().width);
    otherInfoBg->setScaleY(48 / otherInfoBg->getContentSize().height);
    otherInfoBg->setAnchorPoint(ccp(0.5f, 0.5f));
    otherInfoBg->setPosition(ccp(centerX, goodsKindBg->getPositionY() - goodsKindBg->getContentSize().height/2 - otherInfoBg->getContentSize().height/2  + 35) );
    this->addChild(otherInfoBg);
    
    //该商城支持的支付方式  和 背包中含有的数量
    this->showMoneyInNowShop();
    
    
    //显示每日刷新时间
    //char timeStr[50];
    //sprintf(timeStr, "%lld",mallData->_everydayUpdateTime);
    SGCCLabelTTF * refreshTime = SGCCLabelTTF::create( (std::string(str_MallLayer_str16) + _mallData->_everydayUpdateTime).c_str() , FONT_PANGWA,24 );
    refreshTime->setAnchorPoint(ccp(0.5f, 0.5f));
    refreshTime->setPosition(ccp(winSize.width - refreshTime->getContentSize().width/2 ,otherInfoBg->getPositionY() ) );
    this->addChild(refreshTime);
    
    
    //CCAction * arrowsAction = CC
    //左右滑动的箭头
    leftArrows = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    leftArrows->setAnchorPoint(ccp(0, 0));
    leftArrows->setPosition(ccp(0, 0.543 * winSize.height - leftArrows->getContentSize().height ));
    leftArrows->setVisible(false);
    this->addChild(leftArrows);
    
    rightArrows = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    rightArrows->setFlipX(true);
    rightArrows->setAnchorPoint(ccp(0, 0));
    rightArrows->setPosition(ccp(winSize.width - 1.35f*rightArrows->getContentSize().width,  0.543* winSize.height - rightArrows->getContentSize().height ));
    rightArrows->setVisible(false);
    this->addChild(rightArrows);
    
    
    //设置tableView属性。

    this->tableViewHeight = 295.f * winSize.height / 1120 ;
    tableView->setFrame(CCRectMake(0, 0,580 , 2 * this->tableViewHeight) );
    //tableView->setPosition(CCPoint(30, 0.5 * winSize.height - 225.0f * winSize.height / 960));
    tableView->setPosition(CCPoint( 0.5f * (winSize.width-580) , 0.5 * winSize.height - 245.0f * winSize.height / 960));
    tableView->setDown(0);
//  this->tableViewHeight = 240 + 20*((int)winSize.height/1000 );
    //禁止上下滑动
    tableView->setVertical(false);
//    tableView->setPageVertical(false);
    
    
    
    
    tableView->setPagegate(this);
    tableView->setPageNEnable(true);
    tableView->setShowSlider(false);
    
    //标识选中的光圈
    spriteSelectCircle = CCSprite::createWithSpriteFrameName("mall_goodsKindSelected.png");
    this->addChild(spriteSelectCircle,NOW_LAYER_ZORDER);
    spriteSelectCircle->setAnchorPoint(ccp(0.5f, 0.5f) );
    spriteSelectCircle->setVisible(false);
    
    //加载将 装 宝   默认不显示，后来显示。
    sprite_jiang = SGButton::create("mall_goodsKind_jiang.png", NULL, this, menu_selector(SGMallLayer::onJiangClicked) );
    sprite_jiang->setAnchorPoint(ccp(0.5f, 0.5f));
    //sprite_jiang->setPosition(ccp(winSize.width/3 + 4 *i * sprite_jiang->getContentSize().width/3, goodsKindBg->getPositionY() ) );
    sprite_jiang->setVisible(false);
    addBtn(sprite_jiang);
    
    sprite_zhuang = SGButton::create("mall_goodsKind_zhuang.png", NULL, this, menu_selector(SGMallLayer::onZhuangClicked) );
    sprite_zhuang->setAnchorPoint(ccp(0.5f, 0.5f));
    //sprite_zhuang->setPosition(ccp(winSize.width/3 + 4 *i * sprite_zhuang->getContentSize().width/3, goodsKindBg->getPositionY() ) );
    sprite_zhuang->setVisible(false);
    addBtn(sprite_zhuang);
    
    sprite_bao = SGButton::create("mall_goodsKind_bao.png", NULL, this, menu_selector(SGMallLayer::onBaoClicked) );
    sprite_bao->setAnchorPoint(ccp(0.5f, 0.5f));
    //sprite_bao->setPosition(ccp(winSize.width/3 + 4 *i * sprite_bao->getContentSize().width/3, goodsKindBg->getPositionY() ) );
    sprite_bao->setVisible(false);
    addBtn(sprite_bao);
    
    
    //下部大背景图片
    CCSprite * bigBg = CCSprite::createWithSpriteFrameName("storyBg.png");
    CCSize bigBgSize = CCSize(684, 756);
    bigBg->setAnchorPoint(ccp(0.5f, 0));
    bigBg->setTextureRect(CCRect(0,0,bigBgSize.width,bigBgSize.height));
    float scaleTemp = winSize.height>winSize.width ? winSize.height : winSize.width ;
    bigBg->setScaleX(scaleTemp / bigBgSize.height);
    bigBg->setScaleY(scaleTemp / bigBgSize.height);
    bigBg->setPosition(ccp(winSize.width * 0.5, 112));
    this->addChild(bigBg,-100000);
    //大背景图片的贴膜
    CCSprite * upBigBg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    upBigBg->setAnchorPoint(ccp(0.5f, 0));
    upBigBg->setOpacity(180);
    upBigBg->setScaleX(scaleTemp / upBigBg->getContentSize().width);
    upBigBg->setScaleY(scaleTemp / upBigBg->getContentSize().height);
    upBigBg->setPosition(ccp(winSize.width * 0.5, bigBg->getPositionY() ));
    this->addChild(upBigBg,-50);

    
    //刷新按钮
    refreshBtn = SGButton::createFromLocal("an.png", str_MallLayer_str4, this, menu_selector(SGMallLayer::onRefreshBtnClicked),ccp(0,3),FONT_PANGWA,ccWHITE,40);
    refreshBtn->setPosition(ccp(centerX ,  downPos-50 + 20 *winSize.height/1120  + refreshBtn->getContentSize().height/2));
    addBtn(refreshBtn);
    
    //筛选天梯数据,并选择第一页数据
    partData(this->_mallData);
}


SNSTableViewCellItem * SGMallLayer::tableViewItemForCell(SNSTableView *tableView, SNSIndexPath *indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);

    //TEST:
    if (num >= datas->count())
        return NULL;
    
    CargoInformation* pCargoInfo = (CargoInformation *)datas->objectAtIndex(num);//获取对应cell的数据
    
    SGMallCell *item = (SGMallCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
	if (NULL == item)
	{
		item = SGMallCell::create(this,pCargoInfo);
	}
//	item->setTouchEnabled(false);//设置item不可触摸
    return item;
}


void SGMallLayer::partData(SGMallData *mallData)
{
    CCAssert(NULL!=mallData, "null data.");
    
    CCArray * all = mallData->_allCargo;
    CargoInformation * pCargoInfo = NULL;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    if ( ( 0== mallData->_mallId )  ) //天梯商城
    {
        int temp=-1; // 记录将装包  从哪个开始有
        int countTemp =0; //记录有几个
        for(int i=0 ; i< 3 ; ++i)
        {
            if ( mallData->_goodsKind[i] ==1)
            {
                temp = i;
                break;
            }
        }
        for (int i=0 ; i< 3  ; ++i)
        {
            
            if (0== i  && mallData->_goodsKind[i] ==1)
            {
                sprite_jiang->setPosition(ccp(winSize.width/3 + 4 *countTemp * sprite_jiang->getContentSize().width/3, goodsKindBg->getPositionY() ) );
                sprite_jiang->setVisible(true);
                countTemp ++;
            }
            if (1==i && mallData->_goodsKind[i] ==1)
            {
                sprite_zhuang->setPosition(ccp(winSize.width/3 + 4 *countTemp * sprite_zhuang->getContentSize().width/3, goodsKindBg->getPositionY() ) );
                sprite_zhuang->setVisible(true);
                countTemp ++;
            }
            if (2==i  && mallData->_goodsKind[i] ==1)
            {
                sprite_bao->setPosition(ccp(winSize.width/3 + 4 *countTemp* sprite_bao->getContentSize().width/3, goodsKindBg->getPositionY() ) );
                sprite_bao->setVisible(true);
                countTemp ++;
            }

        }
        
        //如果只有一个分类，则不显示。
        if ( countTemp  >1 )
        {
            spriteSelectCircle->setPosition( ccp(winSize.width/3 - 10 , goodsKindBg->getPositionY() + 5) );
            spriteSelectCircle->setVisible(true);
        }
        else
        {
            switch (temp)
            {
                case 0:
                    sprite_jiang->setVisible(false);
                    break;
                case 1:
                    sprite_zhuang->setVisible(false);
                    break;
                case 2:
                    sprite_bao->setVisible(false);
                    break;
            }
        }
        
        for ( int i = 0 ; i < all->count() ; ++i)
        {
            pCargoInfo = (CargoInformation * )all->objectAtIndex(i);
            if ( temp == pCargoInfo->cargoTag )
                _currentData->addObject(pCargoInfo);
        }
    }
    else//非天梯商城时  直接显示。
    {
        for ( int i = 0 ; i < all->count() ; ++i)
        {
            pCargoInfo = (CargoInformation * )all->objectAtIndex(i);
            _currentData->addObject(pCargoInfo);
        }
        
    }
    
    //加载默认情况下第一页的数据
    for ( int i = 0 ; i < _currentData->count() ; ++i)
    {
        if (i < 6 )
        {
            //_currentPageData->addObject(_currentData->objectAtIndex(i) );
            this->datas->addObject(_currentData->objectAtIndex(i) ) ;
        }
        else
            break;
    }
    
    //如果数据1页以上 则显示滑动的箭头和点点
    if (this->_currentData->count() >6)
    {
        this->showScrollGuid(true);
    }
    else
    {
        this->showScrollGuid(false);
    }
}

void SGMallLayer::refreshPage(int type)
{
    _totalPage = ceilf((float)_currentData->count() / 6 );
    if (_totalPage <= 1)
    {
        return;
    }
    
    CCAssert(this->datas, "datas is null.");
    datas->removeAllObjects();
    
    if (type == 1)
    { // to next page
        if (_nowPage < _totalPage)
        {
            _nowPage++;
        }
        else
        { // _nowPage >= _totalPage
            _nowPage = 1;
        }
        
    }
    else if (type == 2)
    { // to pre page
        if (_nowPage > 1)
        {
            _nowPage--;
        }else
        {
            _nowPage = _totalPage;
        }
    }
    
    
    _pointLayer->setLight(_nowPage);

    
    int pageCount = _nowPage < _totalPage ? EACH_PAGE_NUM : (_currentData->count() - (_nowPage - 1) * EACH_PAGE_NUM );
    int startItem = (_nowPage-1) * EACH_PAGE_NUM ;
    for ( int i =  startItem; i < startItem + pageCount ; ++i)
    {
        this->datas->addObject(_currentData->objectAtIndex(i) );
    }
    

    tableView->reloadData();
}



void SGMallLayer::onBackBtnClicked(CCObject * pSend)
{
    CCLog("back btn clicked.");
    
    //this->removeFromParentAndCleanup(true);

    SGMainManager * sm = SGMainManager::shareMain();
    switch (m_enterType)
    {
        case PALYER_INFO_LAYER:
            sm->showPlayerInfo();
            break;
            
        case PVP_HOME_LAYER:
            sm->gotoMatchRequest();
            break;
            
        case SHOU_JING_ZHOU_LAYER:
            sm->showshoujingzhou();
            break;
            
        case SPECIAL_BATTLE_LAYER://参见firstlayer 中 func ：specialBattleHandler（）
        {
            SGPlayerInfo *player=SGPlayerInfo::sharePlayerInfo();
            if (!player->canBreakLimitById(limitSpecailBattle))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_FirstLayer_str23,
                                                                                   player->getLimitLevelById(limitSpecailBattle))->getCString() );
                return;
            }
            
            main::SpecialBattleStoryListRequest *storyList = new main::SpecialBattleStoryListRequest;
            SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_CHAP, storyList);
        }
            break;
            
            //todo:*********竞技场返回
            
        case ARENA_LAYER:
        {
            main::ArenaMainInfoRequest * request = new main::ArenaMainInfoRequest();
            
            SGSocketClient::sharedSocketClient()->send(MSG_ARENA_MAIN_INFO, request);
        }
            break;
            
            //不明入口和商店列表入口均返回商店列表
        case SHOP_LAYER:
            sm->showShopLayer();
        default:
            sm->showShopLayer();
            break;
    }
    m_enterType = SHOP_LAYER;
}


//天梯商城特有
void SGMallLayer::onGoodsKindChanged(int kind)
{
    _currentData->removeAllObjects();
    datas->removeAllObjects();
    _nowPage = 1;
    
    tableView->setTouchEnabled(true);
    //tableView->reloadData();
    
    //更新当前分类的内容
    CCArray * all = _mallData->_allCargo;
    CargoInformation * pCargoInfo = NULL;
    
    for ( int i = 0 ; i < all->count() ; ++i)
    {
        pCargoInfo = (CargoInformation * )all->objectAtIndex(i);
        if ( kind == pCargoInfo->cargoTag )
            _currentData->addObject(pCargoInfo);
    }
    
    //显示第一页内容
    for ( int i = 0 ; i < _currentData->count() ; ++i)
    {
        if (i < 6 )
        {
            //_currentPageData->addObject(_currentData->objectAtIndex(i) );
            this->datas->addObject(_currentData->objectAtIndex(i) ) ;
        }
        else
            break;
    }
    tableView->reloadData();
    
    //如果数据1页以上 则显示滑动的箭头和点点
    if (this->_currentData->count() >6)
    {
        this->showScrollGuid(true);
    }
    else
    {
        this->showScrollGuid(false);
    }

    
}


void SGMallLayer::onJiangClicked(cocos2d::CCObject * pSend)
{
    spriteSelectCircle->setPosition( ( (CCNode *)pSend )->getPosition()  + ccp( -10, 5));
    onGoodsKindChanged(0);
}


void SGMallLayer::onZhuangClicked(cocos2d::CCObject * pSend)
{
    spriteSelectCircle->setPosition( ( (CCNode *)pSend )->getPosition() + ccp( -10, 5) );
    onGoodsKindChanged(1);
}

void SGMallLayer::onBaoClicked(cocos2d::CCObject * pSend)
{
    spriteSelectCircle->setPosition( ( (CCNode *)pSend )->getPosition() + ccp( -10, 5) );
    onGoodsKindChanged(2);
    
}


void SGMallLayer::onRefreshBtnClicked(cocos2d::CCObject * pSend)
{
    CCLog("refresh btn clicked.");
    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 201,_mallData->_refreshNeedPayKind , _mallData->_refreshNeedMoney  ,this) ;
    SGMainManager::shareMain()->showBox(cantadvanceBox);
}

void SGMallLayer::refreshDataOnResponse(cocos2d::CCObject * pSend)
{
    CCLog("刷新物品响应。");
    
    SGMainManager* pMainManager = SGMainManager::shareMain();
    
    SGSocketRequest *request = (SGSocketRequest *)pSend ;
    main::ShopCargoFreshResponse *response = (main::ShopCargoFreshResponse *)request->m_msg;
    if (response)
    {
        switch ( response->state() )
        {
            case 0: //failed
            {
                pMainManager->showMessage(str_MallLayer_str5);
            }
                
                break;
        
            case 1: //success
            {
                //拥有金钱数量减少
                int refreshNeedPayKindTemp = _mallData->_refreshNeedPayKind;
                std::map<int ,SGCCLabelTTF * >::iterator iter = _moneyIconAndNum.find(refreshNeedPayKindTemp); //刷新
                if (iter != _moneyIconAndNum.end() )
                {
                    iter->second->setString(CCString::createWithFormat("%d", atoi(iter->second->getString()) - _mallData->_refreshNeedMoney)->getCString() );
                }
                                            
                
                CC_SAFE_DELETE(_mallData) ; //删除之前new的对象。
                _mallData = NULL;
                
                _mallData = new SGMallData(); //重新new一个。
                
                const main::ShopCargoListResponse  &resList = response->newinfo();
                
                _mallData->_mallId = resList.shopid();
                _mallData->_mallTitle = resList.shopname();
                _mallData->_everydayUpdateTime = resList.nextfreshtime();
                _mallData->_refreshNeedMoney = resList.freshneedgold();
                _mallData->_refreshNeedPayKind = resList.freshgoldtype();
                
                int count = resList.cargoinfo_size(); //数量
                if ( 0 == count )
                {
                    //todo: 当服务器发来0个物品
                    
                }
                else
                {
                    CargoInformation * pCargoInfo = NULL;
                    for ( int i=0 ; i< count ; ++i)
                    {
                        pCargoInfo = new CargoInformation();
                        pCargoInfo->cargoId = resList.cargoinfo(i).cargoid();
                        pCargoInfo->payKind = resList.cargoinfo(i).pricetype();
                        _mallData->_vecPayKind.push_back(pCargoInfo->payKind); //所有都加入vec中，使用时用 unique函数去除相同的。
                        
                        pCargoInfo->oldPrice = resList.cargoinfo(i).price();
                        pCargoInfo->currentPrice = resList.cargoinfo(i).curprice();
                        pCargoInfo->itemKind = resList.cargoinfo(i).itemtype();
                        pCargoInfo->itemId = resList.cargoinfo(i).itemid();
                        pCargoInfo->itemCount = resList.cargoinfo(i).itemcount();
                        pCargoInfo->cargoTag = resList.cargoinfo(i).cargotag();
                        if ( 0 == _mallData->_mallId)
                        {
                            _mallData->_goodsKind[pCargoInfo->cargoTag] = 1;
                        }
                        pCargoInfo->cargoState = resList.cargoinfo(i).cargostate();
                        
                        
                        _mallData->_allCargo->addObject(pCargoInfo);
                    }
                    
                    //重新填充数据。后刷新页面
                    _currentData->removeAllObjects();
                    datas->removeAllObjects();
                    this->partData(_mallData);
                    _nowPage = 1 ;
                    
                    this->tableView->reloadData();
                    
                   
                }
            }
                break;
            case 2:
            {
                pMainManager->showMessage( CCString::createWithFormat(str_MallLayer_str10,getCurrencyNameByPayKind(_mallData->_refreshNeedPayKind ).c_str() )->getCString() );
            }
                break;
            case 3:
            {
                pMainManager->showMessage(str_MallLayer_str17);
            }
                break;
            case 4:
            {
                pMainManager->showMessage(str_MallLayer_str18);

            }
                break;
            default:
            {
                
            }
                break;
        }
        
    }
    else
    {
        CCLog("刷新无结果。");
    }
    
    
}



void SGMallLayer::buyCargoOnResponse(cocos2d::CCObject * pSend)
{
    SGSocketRequest *request = (SGSocketRequest *)pSend ;
    main::CargoBuyResponse *response = (main::CargoBuyResponse *)request->m_msg;
    if (response)
    {
        //参见定义处的注释。
        _isBuyResponsed = true;
        
        SGMainManager* pMainManager = SGMainManager::shareMain();
        
        //当前购买物品的支付方式
        std::map<int ,SGCCLabelTTF * >::iterator iter = _moneyIconAndNum.find(_cargoInfoBuySuccess->payKind);
        CCAssert(iter != _moneyIconAndNum.end(), "当前购买物品的支付方式有误，消息");
        
        std::string payKindStr = getCurrencyNameByPayKind(iter->first);
        
        switch (response->state() )
        {
            case 0: //状态失败
            {
                pMainManager->showMessage(str_MallLayer_str5);
            }
                break;
            case  1://成功 刷新数据和页面
            {
                
                //购买物品成功 ，修改状态为已经售出，
                if (SGMallLayer::_cargoInfoBuySuccess)
                    SGMallLayer::_cargoInfoBuySuccess->cargoState = 1;
                
                //购买后的物品变黑
                _buyMallCell->setGray(true);
                
                
                //拥有金钱数量减少
                
                if (iter != _moneyIconAndNum.end() )
                {
                    int nowMoney = atoi(iter->second->getString());
                    iter->second->setString(CCString::createWithFormat("%d",nowMoney - _cargoInfoBuySuccess->currentPrice) ->getCString() );
                }
                
                pMainManager->showMessage(str_MallLayer_str6);
            }
                break;
            case 2://金钱不足
            {
                pMainManager->showMessage(CCString::createWithFormat(str_MallLayer_str7,payKindStr.c_str() )->getCString() );
            }
                break;
            case 3://没有剩余物品
            {
                pMainManager->showMessage(str_MallLayer_str8);
            }
                break;
            case 4://不能购买该物品
            {
                pMainManager->showMessage(str_MallLayer_str9);
            }
                break;
        }
        
    }
    else
    {
        CCLog("server no reply on tyring to buy.");
    }
}


void SGMallLayer::showMoneyInNowShop()
{
    std::vector<int>::iterator endIter;
    std::sort(_mallData->_vecPayKind.begin(),_mallData->_vecPayKind.end() );
    endIter = std::unique(_mallData->_vecPayKind.begin(), _mallData->_vecPayKind.end() );
    int order = 0;
    for ( std::vector<int>::iterator i = _mallData->_vecPayKind.begin() ; i != endIter ; ++i )
    {
        CCSprite * spritePayKind = SGMallUtil::createSpriteByPayType( *i );
        spritePayKind->setPosition(ccp(order*170 + 0.6*spritePayKind->getContentSize().width, otherInfoBg->getPositionY() ) );
        this->addChild(spritePayKind);
        
        // 获取背包中该种支付方式的数量
        int num = SGMallUtil::getNumByCargoIdAndType(*i);

        char str[50];
        sprintf(str, "%d",num);
        SGCCLabelTTF * money = SGCCLabelTTF::create(str, FONT_XINGKAI, 24);
        money->setPosition(ccp(spritePayKind->getPositionX() + spritePayKind->getContentSize().width/2 + money->getContentSize().width/2, spritePayKind->getPositionY()));
        this->addChild(money);
        _moneyIconAndNum.insert(std::pair<int, SGCCLabelTTF* >(*i,money) );
        
        order++;
    }
    
    
}

bool SGMallLayer::sendRefreshMsg()
{
    if ( SGMallUtil::getNumByCargoIdAndType(_mallData->_refreshNeedPayKind) <  _mallData->_refreshNeedMoney)
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MallLayer_str10,getCurrencyNameByPayKind(_mallData->_refreshNeedPayKind ).c_str() )->getCString());
        return false;
    }
    else
    {
        main::ShopCargoFreshRequest *request = new main::ShopCargoFreshRequest;
        request->set_shopid(this->_mallData->_mallId );
        SGSocketClient::sharedSocketClient()->send(MSG_CARGO_REFRESH, request);
        
        return true;
    }
}



void SGMallLayer::showScrollGuid(bool bShow)
{
    //如果数据1页以上 则显示滑动的箭头
    leftArrows->setVisible(bShow);
    rightArrows->setVisible(bShow);
    
    //水平滑动
    tableView->setHorizontal(bShow);


   if (bShow)
   {
       //切换显示数据时加上点点
       CCSize winSize = CCDirector::sharedDirector()->getWinSize();
       if (_pointLayer )
       {
           _pointLayer->removeFromParent();
        }
       _pointLayer = PointLayer::create(ceilf((float)_currentData->count() / 6 ), winSize.width, "anDian.png", "liangDian.png");
       //_pointLayer->setAnchorPoint(ccp(0.5f, 0.5f));
       _pointLayer->setPosition(ccp(0.5 * CCDirector::sharedDirector()->getWinSize().width , 265.0f * CCDirector::sharedDirector()->getWinSize().height/1120 ) );
       this->addChild(_pointLayer);
   }
   else
    {
       if (_pointLayer )
        {
            _pointLayer->removeFromParent();
            _pointLayer = NULL;
        }
    }

}






