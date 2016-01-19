//
//  SGBarracksLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#include "SGBarracksLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"

#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "AppMacros.h"

using namespace cocos2d;

SGBarracksLayer *SGBarracksLayer::create()
{
    SGBarracksLayer *barracksLayer = new SGBarracksLayer();
    if (barracksLayer && barracksLayer->init(NULL, sg_barrackslayer))
    {
        barracksLayer->initView();
        barracksLayer->autorelease();
        return barracksLayer;
    }
    CC_SAFE_DELETE(barracksLayer);
    return NULL;
}
SGBarracksLayer::SGBarracksLayer()
: str_ofr(NULL)
, str_eqp(NULL)
, str_soul(NULL)
, str_prop(NULL)
, str_skill(NULL)
, str_pieces(NULL)
, str_consume(NULL)
,str_material(NULL)
, itemNum(0)
{
}
SGBarracksLayer::~SGBarracksLayer()
{
    CCLOG("SGBarracksLayer::~SGBarracksLayer()");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_barrackslayer);
}

///////////
void SGBarracksLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_barrackslayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_barrackslayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_barrackslayer);
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,2);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,3);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
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
    
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width, s.height - b - title_bg->getContentSize().height -5);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    bg->setTextureRect(r);
    
    
    int shift = 0;
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        shift = 0;
        bg->setScaleY(1136 / (bg->getContentSize().height * 1.25));
    }
    
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b + shift)));

    SGCCLabelTTF *title = SGCCLabelTTF::create(str_bag, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
    CCArray *officer = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    CCArray *equip = SGPlayerInfo::sharePlayerInfo()->getEquipCards();    
    CCArray *propCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
	CCArray *consumeCards = SGPlayerInfo::sharePlayerInfo()->getConsumableCards();
    CCArray *materialCards = SGPlayerInfo::sharePlayerInfo()->getMaterialCards();
    int generalProp = 0;
    int soulProp = SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->count();
    int equipProp = 0;
    int otherProp = 0;
    int skillProp = 0;
	int comsume = consumeCards->count();
    int material = materialCards->count();
    CCObject *obj = NULL;
    int total = 0;
    if (propCards) {
        CCARRAY_FOREACH(propCards, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card)
            {
                if (card->getType() == 1)
                {
                    generalProp += 1 ;
                }
                else if (card->getType() == 2)
                {
                    equipProp += 1 ;
                }
                else if (card->getType() == 3)
                {
                    soulProp += 1 ;
                }
                else if (card->getType() == 0)
                {
                    otherProp += 1 ;//转生道具
                }else if (card->getType() == 4 ||card->getType() == 5)
                {
                    skillProp+= 1;
                }

            }
        }
       
//        str_ofr = CCString::createWithFormat("（%d）",officer->count() + generalProp);
        str_ofr = CCString::createWithFormat("（%d）",officer->count());
        str_eqp = CCString::createWithFormat("（%d）",equip->count()  + equipProp );
        str_soul = CCString::createWithFormat("（%d）",soulProp);
        str_prop = CCString::createWithFormat("（%d）",otherProp);
        str_skill = CCString::createWithFormat("（%d）",skillProp);
		str_consume = CCString::createWithFormat("（%d）", comsume);
        //不计入背包
        

		//计算总碎片数量///////////////////////////////////////////////////////////////////////////////
		SGPlayerInfo *info = SGPlayerInfo::sharePlayerInfo();
		CCArray *temp = info->getPiecesCards();
		int sumPieces = 0;
		for (int i = 0; i < info->getPiecesCards()->count(); i++)
		{
			sumPieces += ((SGPiecesCard *)temp->objectAtIndex(i))->getPiecesCurNum();
		}
		if (info->getPiecesCommonCards()->count())
		{
			sumPieces += ((SGPiecesCard *)info->getPiecesCommonCards()->objectAtIndex(0))->getPiecesCurNum();
		}
		
		//计算总碎片数量///////////////////////////////////////////////////////////////////////////////
		str_pieces = CCString::createWithFormat("（%d）", sumPieces);// 碎片数量,  只显示，不计入背包数量
        total = officer->count() + equip->count() + propCards->count() + SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->count() + consumeCards->count();
    }
    material = 0;
    if(materialCards)
    {
        CCARRAY_FOREACH(materialCards, obj)
        {
            SGMaterialCard *card = (SGMaterialCard *)obj;
            if (card)
            {
                material += card->getMaterialCurrNum();
            }
        }
    }
    str_material = CCString::createWithFormat("（%d）", material);
    CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    cardcountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(cardcountbg,40);
    cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
    
    total = playerInfo->getGoodsNumInBag();
    SGCCLabelTTF *cardcounts = SGCCLabelTTF::create(CCString::createWithFormat(str_ExpandLayer_str3,total,playerInfo->getPlayerBagSize())->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(cardcounts,42);
    cardcounts->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));
    
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_35);
//    m_tableView = SNSTableView::create(CCRectMake(0, 0, s.width, s.height -title_bg->getContentSize().height -45 - gonggao_3_2->getContentSize().height - b), TableViewTypeVertical);
//	m_tableView->setDatasource(this);
//	m_tableView->setDelegate(this);
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height -title_bg->getContentSize().height -45 - gonggao_3_2->getContentSize().height - b));
    tableView->setShowSlider(false);
	tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - 51 - title_bg->getContentSize().height)));
    tableView->setDown(-25);
    tableView->setTag(modelTag);
    tableView->setPageNEnable(true);///1126效果优化
    tableView->setPageVertical(true);
//    this->addChild(m_tableView);
    
    EFFECT_PLAY(MUSIC_ITEM);
}

SNSTableViewCellItem* SGBarracksLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();
    
	LabelItem *item = (LabelItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
       
    if (NULL == item)
    {
		item = LabelItem::create(size,num,true,this,tableView);
    }
    switch (num)
    {
        case 0:
            item->label->setString(str_ofr->getCString());
            break;
        case 1:
            item->label->setString(str_eqp->getCString());
            break;
		case 2:
            item->label->setString(str_soul->getCString());
           	break;
        case 3:
            item->label->setString(str_pieces->getCString());//碎片
            break;
        case 4:
            item->label->setString(str_material->getCString());//材料
            break;
        case 5:
            item->label->setString(str_consume->getCString());
            break;
        case 6:
            item->label->setString(str_skill->getCString());
            break;
      
        default:
     
            break;
    }
	return item;
}

unsigned int SGBarracksLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
   
	itemNum = 8;
	return itemNum;
}

float SGBarracksLayer::tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath)
{   
	return 110;
}
///////
void SGBarracksLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_ITEM);
    SGMainManager::shareMain()->showblack();
//    int index = tableView->getItemPointerWithIndexPath(indexpath);
    int index = indexpath->getRow();
    switch (index)
    {
        case 0:
            generalHandler();
            break;
        case 1:
            equipmentHandler();
            break;
		case 2:
            soldierHandler();
            break;
        case 3:
            piecesHandler();// 碎片
            break;
        case 4:
            materialHandler();//材料
            break;
		case 5:
			consumeHandler();
			break;
        case 6:
            skillHandler();
            break;
        case 7:
            expandHandler();
            break;
            
        default:
            break;
    }

    
    
}


unsigned int SGBarracksLayer::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
	return 1;
}
void SGBarracksLayer::teamformationHandler()
{   
    SGMainManager::shareMain()->showEmbattleLayer(0);
}
void SGBarracksLayer::generalHandler()
{
    SGMainManager::shareMain()->showGeneralsBagLayer(0);
}
void SGBarracksLayer::soldierHandler()
{
    SGMainManager::shareMain()->showSoldierslayer();
}
void SGBarracksLayer::equipmentHandler()
{
    SGMainManager::shareMain()->showEquipsLayer(NULL,0,0);
}
//碎片/////////////////////////////////////////
void SGBarracksLayer::piecesHandler()
{
    CCLOG("Pieces");
	SGMainManager::shareMain()->showPiecesLayer();
}
void SGBarracksLayer::propHandler()
{
    SGMainManager::shareMain()->showPropsLayer();
}
void SGBarracksLayer::consumeHandler()
{
    CCLOG("Consume");
	SGMainManager::shareMain()->showConsumeLayer();
}
//材料
void SGBarracksLayer::materialHandler()
{
    CCLOG("Material Cell");
    SGMainManager::shareMain()->showMaterialLayer();
}

void SGBarracksLayer::skillHandler()
{
    SGMainManager::shareMain()->showSkillCardsLayer(0);
}
void SGBarracksLayer::expandHandler()
{
    SGMainManager::shareMain()->showExpandLayer();
}

//void SGBarracksLayer::setIsCanTouch(bool isTouch)
//{
//    SGBaseLayer::setIsCanTouch(isTouch);
//    m_tableView->setTouchEnabled(isTouch);
//    
//    LabelItem *item = NULL;
//    for (int i=0; i<itemNum; i++)
//    {
//        item = (LabelItem*)m_tableView->getItemByIndexPath(SNSIndexPath::create(i, 1));
//        if (item)
//        {
////            item->menu->setEnabled(isTouch);
//        }
//    }
//    
//}

bool LabelItem::initWithSize(CCSize size,int num,bool isshow)
{
    if (!SNSTableViewCellItem::init())
    {
		return false;
	}
    
    //////////
    //触摸优先级，这里为了兼容滑动区触摸，暂时设置menu优先级为-1
//    menu=CCMenu::create();
//    menu->setTouchPriority(-1);
//    menu -> setPosition(CCPointZero);
    
    SGButton*btn=SGButton::create("upwar_cell_bg.png", NULL, this, menu_selector(LabelItem::buttonClick),CCPointZero,false,true);
    this->addChild(btn);
    addBtn(btn);
    btn->setPosition(CCPointZero);
    
//    menu->addChild(btn);
//    this->addChild(menu);
    
    //通用红点
    redSpot_ = CCSprite::createWithSpriteFrameName("publc_notice.png");
    redSpot_->setPosition(ccp( (btn->getContentSize().width - redSpot_->getContentSize().width) * 0.5 - 5, 0));
    redSpot_->setVisible(false);
	this->addChild(redSpot_, 5);;
    
    
    title = SGCCLabelTTF::create("", FONT_PANGWA, 36 , ccWHITE);
    this->addChild(title);
    if (isshow)
    {
        switch (num)
        {
            case 0:
                title->setString(str_wujiang);
                break;
            case 1:
                title->setString(str_zhuangbei);
                break;
            case 2:
                title->setString(str_soldiers);
                break;
            case 3:
                title->setString(str_suipian);
                break;
            case 4:
                title->setString(str_material_title);
                break;
            case 5:
                title->setString(str_porp);
                break;
            case 6:
                title->setString(str_jinengshucai);
                break;
            case 7:
                title->setString(str_ExpandLayer_str2);
                break;
            default:
                break;
        }
    }
    label = SGCCLabelTTF::create("", FONT_BOXINFO, 24);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width*.5, 0)));
    this->addChild(label);    
	return true;
}
LabelItem* LabelItem::create(CCSize size,int num,bool isshow,LabelItemDelegate*del,SNSTableView*table)
{
	LabelItem *instance = new LabelItem();
	if (instance && instance->initWithSize(size,num,isshow)) {
        instance->tableview=table;
        instance->delegate=del;
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}
///////////////

void LabelItem::buttonClick(CCObject* obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);

}

//设置红点的显隐。
void LabelItem::setRedSpot(bool bl)
{
    if (redSpot_)
    {
        redSpot_->setVisible(bl);
    }
}
