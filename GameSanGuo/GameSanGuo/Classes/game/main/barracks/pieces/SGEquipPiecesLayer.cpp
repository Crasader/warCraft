//
//  SGEquipPiecesLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#include "SGEquipPiecesLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGPiecesLayer.h"
#include "SGPiecesCell.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGEquipInfoLayer.h"
#include "SGStringConfig.h"
#include <algorithm>
#include "AppMacros.h"

#define EACH_PAGE_NUM 15

SGEquipPiecesLayer::SGEquipPiecesLayer():
piecesTotalCommon(0),
selectIndex(0),
card(NULL),
commonCount(NULL),
pageLabel(NULL),
_allData(NULL)
{
	_allData = NULL;
    pageLabel = NULL;
    _nowPage = 0;
    _totalPage = 0;
}
SGEquipPiecesLayer::~SGEquipPiecesLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_equipPiecesLayer);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SUMMON_CARD);
}

SGEquipPiecesLayer *SGEquipPiecesLayer::create()
{
    SGEquipPiecesLayer *piecesLayer = new SGEquipPiecesLayer();
    if (piecesLayer && piecesLayer->init(NULL, sg_equipPiecesLayer))
    {
		
        piecesLayer->initView();
		//piecesLayer->datas = data;
		//piecesLayer->datas->retain();
        piecesLayer->autorelease();
        return piecesLayer;
    }
    CC_SAFE_DELETE(piecesLayer);
    return NULL;
}

//排序回调
static int SGSortAccorCanSummon(CCObject *p1, CCObject *p2)
{
    SGPiecesCard *card1 = static_cast<SGPiecesCard *>(p1);
    SGPiecesCard *card2 = static_cast<SGPiecesCard *>(p2);
    
    return card1->getCurrStar() > card2->getCurrStar();
}
//重新排序碎片
void SGEquipPiecesLayer::resortPiecesCard()
{
    //按可召唤排序
    CCArray *canSummon = CCArray::create();
    for (int i = 0; i < datas->count(); i++)
    {
        SGPiecesCard *temp = static_cast<SGPiecesCard *>(datas->objectAtIndex(i));
        //如果是可召唤的,放到可召唤数组中,最后集中放到最前面
        if (temp->getSummonNeed() <= temp->getPiecesCurNum())
        {
            canSummon->addObject(temp);
        }
    }
    std::sort(canSummon->data->arr, canSummon->data->arr + canSummon->data->num, SGSortAccorCanSummon);
    
    
    //再将datas中的含有canSummon的对象都去除,重新从头部加入
    datas->removeObjectsInArray(canSummon);
    _allData->removeObjectsInArray(canSummon);
    
    std::sort(datas->data->arr, datas->data->arr + datas->data->num, SGSortAccorCanSummon);
    std::sort(_allData->data->arr, _allData->data->arr + _allData->data->num, SGSortAccorCanSummon);
    
    for (int i = canSummon->count() - 1; i >= 0; i--)
    {
        datas->insertObject(canSummon->objectAtIndex(i), 0);
        _allData->insertObject(canSummon->objectAtIndex(i), 0);
    }
}

void SGEquipPiecesLayer::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
	
	//添加监听
	SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
	notification->addObserver(MSG_SUMMON_CARD, this, callfuncO_selector(SGEquipPiecesLayer::summonPiecesListener));
	
	datas = CCArray::create();
	datas->retain();
	_allData = CCArray::create();
	_allData->retain();
	
	CCArray *temp = CCArray::create();
	temp = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
	
	for (int i = 0; i < temp->count(); i++)
	{
		SGPiecesCard *tCard = (SGPiecesCard *)temp->objectAtIndex(i);
//		if (tCard->getCallCardId() >= 4001)
		if (tCard->getItemType() == 1)//装备碎片
		{
			datas->addObject(tCard);
			_allData->addObject(tCard);
		}
	}
    
    
	_totalPage = ceilf((float) _allData->count() / EACH_PAGE_NUM);
	(_totalPage < 1) && (_totalPage = 1);
    float b = SGMainManager::shareMain()->getBottomHeight();
	float h = SGMainManager::shareMain()->getHeadHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    
    //按可召唤排序
    if (this->datas->count())
    {
        this->resortPiecesCard();
    }
    
    
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
	
//	SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGEquipPiecesLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGEquipPiecesLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.1);
	this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, title_bg->getPosition().y - backBtn->getContentSize().height / 2 - 5));
    
	SGCCLabelTTF *title = SGCCLabelTTF::create(str_EquipPiecesLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
	
	
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_equipPiecesLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width,bg->getContentSize().height - gonggao_3_2->getContentSize().height - b);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    int shift = 0;
    if(CCDirector::sharedDirector()->getWinSize().height != 1136)
    {
        shift = -25;
        bg->cocos2d::CCNode::setScaleY(0.86);
    }
    if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height)
    {
        bg->setScaleY(0.94);
    }
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b + 30 + shift)));

    
	//添加内部UI
	//武将碎片button
	SGButton *zbPiecesBtn = SGButton::create("wujiang_pieces_btn.png", "zhunagbei_pieces_active_letter.png",
                                             this, menu_selector(SGEquipPiecesLayer::showEquipPiecesLayer), ccp(0, 0), false, true);
	//装备碎片Button
	SGButton *wjPiecesBtn = SGButton::create("zhuangbei_pieces_btn.png", "wujiang_pieces_unactive_letter.png",
                                             this, menu_selector(SGEquipPiecesLayer::showEquipPiecesLayer), ccp(0, 0), false, true);
	this->addBtn(wjPiecesBtn);
	this->addBtn(zbPiecesBtn);
	wjPiecesBtn->setPosition(ccp(wjPiecesBtn->getContentSize().width - 30, title_bg->getPositionY() - wjPiecesBtn->getContentSize().width));
	zbPiecesBtn->setPosition(ccp(wjPiecesBtn->getPositionX() + wjPiecesBtn->getContentSize().width, wjPiecesBtn->getPositionY()));
	//button下横线
	CCSprite *bottomBorder = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    bottomBorder->setAnchorPoint(ccp(0.5, 1));
    bottomBorder->setScaleX(15);
	bottomBorder->setScaleY(0.5);
	bottomBorder->setPosition(ccpAdd(ccp(0, 0), ccp(0, wjPiecesBtn->getPositionY() - wjPiecesBtn->getContentSize().height / 2)));
	this->addChild(bottomBorder, 10);
	
	//通用碎片选框
	CCSprite *u1 = CCSprite::createWithSpriteFrameName("unuse_common_oieces.png");
	CCSprite *u2 = CCSprite::createWithSpriteFrameName("use_common_pieces.png");
	CCMenuItemSprite *unUsePic = CCMenuItemSprite::create(u1, u1);
	CCMenuItemSprite *usePic = CCMenuItemSprite::create(u2, u2);
	usePic->setVisible(false);
    unUsePic->setVisible(false);
	CCMenuItemToggle *toggleMenu = CCMenuItemToggle::createWithTarget(this, menu_selector(SGEquipPiecesLayer::useCommonPieces), unUsePic, usePic, NULL);
	toggleMenu->setPosition(ccp(title_bg->getPositionX() + toggleMenu->getContentSize().width, title_bg->getPositionY() - toggleMenu->getContentSize().height * 2.6));
	
	CCMenu *useMenu = CCMenu::create(toggleMenu, NULL);//
	useMenu->setPosition(CCPointZero);
	this->addChild(useMenu, 9);
	
	//通用碎片数量
	if (SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards()->count())
	{
		piecesTotalCommon = ((SGPiecesCard *)(SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards()->objectAtIndex(0)))->getPiecesCurNum();
	}
	
	commonCount = CCLabelTTF::create((CCString::createWithFormat(str_EquipPiecesLayer_str2, piecesTotalCommon))->getCString(), FONT_LISHU, 21);
	commonCount->setPosition(ccp(toggleMenu->getPositionX(), toggleMenu->getPositionY() - toggleMenu->getContentSize().height));
	this->addChild(commonCount, 2);
	commonCount->cocos2d::CCNode::setVisible(false);
	//设置tableview
	tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - title_bg->getContentSize().height - 100));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-15);
	
	
	//设置左右箭头
    float posY = tableView->getContentSize().height*.5f + tableView->getPositionY();
    CCSprite *spriteL = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    float widLR = spriteL->getContentSize().width*.5f;
    float hitLR = spriteL->getContentSize().height*.5f;
    spriteL->setPosition(ccp(widLR, posY-hitLR));
    spriteL->setTag(111);
    spriteL->setVisible(false);
    this->addChild(spriteL);
    CCSprite *spriteR = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    spriteR->setFlipX(true);
    spriteR->setTag(112);
    spriteR->setVisible(false);
    spriteR->setPosition(ccp(s.width-widLR, posY-hitLR));
    this->addChild(spriteR);
    
    if (_totalPage > 1) {
        spriteL->setVisible(true);
        spriteR->setVisible(true);
        tableView->setPagegate(this);
        tableView->setPageNEnable(true);
        tableView->setShowSlider(false);
    }
    
    //设置页数
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    pagecountbg->setAnchorPoint(ccp(0, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
    
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf0, 0x00));
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
    CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    cardcountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(cardcountbg,40);
    cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
	
	//计算总碎片数量///////////////////////////////////////////////////////////////////////////////
	SGPlayerInfo *info = SGPlayerInfo::sharePlayerInfo();
	CCArray *t = info->getPiecesCards();
	int sumPieces = 0;
	for (int i = 0; i < info->getPiecesCards()->count(); i++)
	{
		sumPieces += ((SGPiecesCard *)t->objectAtIndex(i))->getPiecesCurNum();
	}
	if (info->getPiecesCommonCards()->count())
	{
		sumPieces += ((SGPiecesCard *)info->getPiecesCommonCards()->objectAtIndex(0))->getPiecesCurNum();
	}
	
	//计算总碎片数量///////////////////////////////////////////////////////////////////////////////
	
    SGCCLabelTTF *cardcounts = SGCCLabelTTF::create(CCString::createWithFormat(str_EquipPiecesLayer_str3,sumPieces)->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf0, 0x00));
    this->addChild(cardcounts,42);
    //modify by:zyc. merge into create.
    //cardcounts->setColor(ccc3(0xff, 0xf0, 0x00));
    cardcounts->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));
	
	if (_allData->count()>0) {
        refreshPage(1);
    }

	

}

void SGEquipPiecesLayer::refreshPage(int type)
{
	//    DDLog("ccc===================%d", type);
    datas->removeAllObjects();
    int pageCount = _allData->count() > EACH_PAGE_NUM ? EACH_PAGE_NUM : _allData->count(); // 每页显示的个数
    
    if (type == 1) { // to next page
        if (_nowPage < _totalPage) {
            _nowPage++;
        }else { // _nowPage >= _totalPage
            _nowPage = 1;
        }
    }else if (type == 2) { // to pre page
        if (_nowPage > 1) {
            _nowPage--;
        }else {
            _nowPage = _totalPage;
        }
    }
    
    int last = pageCount;
    if (_nowPage == _totalPage) {
        last = _allData->count()-(_nowPage-1)*pageCount;
    }
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex((_nowPage-1)*pageCount+ii);
        datas->addObject(obj);
    }
    
    CCString *str = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    if(pageLabel) pageLabel->setString(str->getCString());
    tableView->reloadData();
}


SNSTableViewCellItem *SGEquipPiecesLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	SGPiecesCard *info = (SGPiecesCard *)datas->objectAtIndex(num);
    SGPiecesCell *item = (SGPiecesCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
	
	if (NULL == item)
	{
		item = SGPiecesCell::create(this, info, isUseCommon, sg_equipPiecesLayer);
	}
    else
    {
        item->updatePiecesCard(info);
    }
	item->setTouchEnabled(false);
    return item;
}
unsigned int SGEquipPiecesLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGEquipPiecesLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	    EFFECT_PLAY(MUSIC_BTN);
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);//选择哪一item
//    SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(selectIndex);//获取对应cell的数据
//    SGMainManager::shareMain()->showPiecesInfoLayer(card,1, false);
}

//召唤监听
void SGEquipPiecesLayer::summonPiecesListener(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::SummonCardResponse *response = (main::SummonCardResponse *)request->m_msg;
	
    if (request)
	{
		if (response->state() == 1)//召唤成功
		{
			CCLOG("Summon Success");
			SGMainManager::shareMain()->showMessage(str_EquipPiecesLayer_str4);
			SGEquipmentDataModel *equipData = SGStaticDataManager::shareStatic()->getEquipById(card->getCallCardId());
			SGEquipCard *equipCard = new SGEquipCard;
			
			//填充装备卡数据/////////////////////////////////////////////////////////////////////
			
			equipCard->setProtoId(equipData->getEquipProtoId());
            equipCard->setCurrStar(equipData->getEquipCurrStarLevel());
            equipCard->setCurrLevel(1);            
            equipCard->setOfficerCardId(0);
            equipCard->setItemType(equipData->getEquipType());
            equipCard->setIsProt(0);
            equipCard->setOfficerName(CCString::create(equipData->getEquipName()->getCString()));
            equipCard->setMorale(equipData->getEquipHP());
            equipCard->setAtk(equipData->getEquipmentBaseAtk());
            equipCard->setDef(equipData->getEquipBaseDef());
            equipCard->setRound(equipData->getEquipRound());
            equipCard->setSpeed(equipData->getEquipBaseSpeed());
            equipCard->setMaxLevel(equipData->getEquipMaxLevel());
            equipCard->setGoverning(equipData->getEquipGoverning());
            //转生等级
            equipCard->setUpgradelevel(equipData->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(equipData->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(equipData->getUpgradestar());
            //装备原型ID
            equipCard->setProtoId(equipData->getEquipProtoId());
			equipCard->setItemId(equipData->getEquipmentId());
			//填充装备卡数据/////////////////////////////////////////////////////////////////////
            
			equipCard->setSsid(response->ssid());
            
			//将对应的卡牌传入
			SGEquipInfoLayer *Layer = SGEquipInfoLayer::create(SGPlayerInfo::sharePlayerInfo()->getEquipCard(response->ssid()),11000);
			SGMainManager::shareMain()->showLayer(Layer,false);
			
            this->resortPiecesCard();
			tableView->reloadData();//重新载入cell
			
			CCArray *piecesArray = SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards();
			if (piecesArray->data && piecesArray->count() && piecesArray->data->arr)
			{
				piecesTotalCommon = ((SGPiecesCard *)(piecesArray->objectAtIndex(0)))->getPiecesCurNum();
			}
			else
			{
				piecesTotalCommon = 0;
			}
			commonCount->setString((CCString::createWithFormat(str_EquipPiecesLayer_str2, piecesTotalCommon))->getCString());
		}
		else
		{
			SGMainManager::shareMain()->showMessage(str_EquipPiecesLayer_str5);
		}
	}
	
}


void SGEquipPiecesLayer::summonPieces(SGPiecesCell *piecesItem)
{
	CCLOG("+++++++++++++++++++++++++In EquipPiecesLayer!+++++++++++++++++++++++++");
	
	int summonSuccess = false;
	
	//	CCLOG("+++++++++++++++++++++++++In PiecesLayer!+++++++++++++++++++++++++ selectIndex == %d", selectIndex);
	if (selectIndex < datas->count())//选择在区域内
	{
		card = (SGPiecesCard *)datas->objectAtIndex(selectIndex);//根据选择拿到对应的碎片
		//碎片当前持有量
		int piecesCurr = card->getPiecesCurNum();
		//碎片召唤需要的数量
		int needNum = card->getSummonNeed();
		//碎片可使用的通用碎片数量
		int commonNum = card->getCanUseCommon();
		//所有通用碎片
		int totalCommon = piecesTotalCommon;
		
		//如果本身的碎片数量足够
		if (piecesCurr >= needNum)
		{
			piecesCurr -= needNum;//召唤后，减掉对应的数量
			summonSuccess = true;
			//			CCLOG("Current Pieces Enough!");
		}
		else if (isUseCommon && piecesTotalCommon >= commonNum && piecesCurr + commonNum >= needNum)//如果本身碎片不够，且通用碎片可用，那么使用通用碎片
		{
			int temp = needNum - piecesCurr;//计算需要的通用碎片数量
			piecesCurr = 0;// 本身碎片全都使用
			totalCommon -= temp;//减掉用去的通用碎片的数量
			
			summonSuccess = true;
			//			CCLOG("Use Common Pieces!");
		}
		else
		{
			CCLOG(" #Debug# See This Statement, BUG IS CAME OUT ! #Debug#");//进来就是出错了
		}
		
		CCLOG("piecesCurr == %d, needNum == %d, commonNum == %d, totalCommon == %d", piecesCurr, needNum, commonNum, totalCommon);
	}
	
	if (summonSuccess == true)//碎片召唤成功
	{
		main::SummonCardRequest *summsonRequest = new main::SummonCardRequest();
		summsonRequest->set_ssid(card->getSsid());//发送ssid
		
		SGSocketClient::sharedSocketClient()->send(MSG_SUMMON_CARD, summsonRequest);
	}
	
}


void SGEquipPiecesLayer::showEquipPiecesLayer(bool isBeforeDel)
{
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	SGPiecesLayer *layer = (SGPiecesLayer *)mainScene->getChildByTag(sg_piecesLayer);
    if (!layer) {
        layer = SGPiecesLayer::create();
    }
	mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	mainScene->addShowLayer(layer);
	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

void SGEquipPiecesLayer::useCommonPieces()
{
	this->isUseCommon = !isUseCommon;
	CCLog("Equip isUseCommon == %d", isUseCommon);
	tableView->reloadData();
}

void SGEquipPiecesLayer::backHandler()
{
	EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showBarrackslayer();
}



void SGEquipPiecesLayer::showPiecesinfo(SGPiecesCell *piecesItem)
{
    SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(selectIndex);//获取对应cell的数据
    SGMainManager::shareMain()->showPiecesInfoLayer(card,0, false);
    
}





