//
//  SGPiecesLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#include "SGPiecesLayer.h"
#include "SGMainManager.h"
#include "SGEquipPiecesLayer.h"
#include "SGCardItem.h"
#include "SGPiecesCell.h"
#include "SGPiecesCard.h"
#include "SGGeneralInfoLayer.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGLevelUpLayer.h"
#include "SGStringConfig.h"
#include <algorithm>
#include "AppMacros.h"
#include "SGShareBox.h"
#include "SGCommonInfoBox.h"

#define EACH_PAGE_NUM 15

SGPiecesLayer::SGPiecesLayer() :
isUseCommon(false),
selectIndex(-1),
piecesCommonTotalNum(0),
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
SGPiecesLayer::~SGPiecesLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_piecesLayer);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SUMMON_CARD);
    CC_SAFE_RELEASE(datas);
    CC_SAFE_RELEASE(_allData);
}


void SGPiecesLayer::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(10);//设置menu的优先级
}

SGPiecesLayer *SGPiecesLayer::create()
{
    SGPiecesLayer *piecesLayer = new SGPiecesLayer();
    if (piecesLayer && piecesLayer->init(NULL, sg_piecesLayer))
    {
        piecesLayer->initView();
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
    if (card1 == NULL || card2 == NULL) {
        return false;
    }
    //return card1->getCurrStar() > card2->getCurrStar();
    return card1->getPiecesCurNum() > card2->getPiecesCurNum();
}

//重新排序碎片
void SGPiecesLayer::resortPiecesCard()
{
    
    if (datas)
    {
        datas->release();
        datas = NULL;
    }
    if (_allData)
    {
        _allData->release();
        _allData = NULL;
    }
    
    datas = CCArray::create();
    datas->retain();
    
    _allData = CCArray::create();
    _allData->retain();
    
    
    CCArray *tempPieces = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
    std::sort(tempPieces->data->arr, tempPieces->data->arr + tempPieces->data->num, SGSortAccorCanSummon);
    
    
    CCArray *canSummon = CCArray::create();
    for (int i = 0; i < tempPieces->count(); i++)
    {
        SGPiecesCard *temp = static_cast<SGPiecesCard *>(tempPieces->objectAtIndex(i));
        //如果是可召唤的,放到可召唤数组中,最后集中放到最前面
        if (temp->getSummonNeed() <= temp->getPiecesCurNum())
        {
            canSummon->addObject(temp);
        }
    }
    tempPieces->removeObjectsInArray(canSummon);
    
    for (int i = canSummon->count() - 1; i >= 0; i--)
    {
        tempPieces->insertObject(canSummon->objectAtIndex(i), 0);
    }
    
    
    for (int i = 0; i < tempPieces->count(); i++)
    {
        SGPiecesCard *tCard = (SGPiecesCard *)tempPieces->objectAtIndex(i);
        if (tCard->getItemType() == 0)//武将碎片
        {
            _allData->addObject(tCard);
        }
    }
    datas->addObjectsFromArray(_allData);
    
}



void SGPiecesLayer::initView()
{
    
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
    
	//添加监听
	SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
	notification->addObserver(MSG_SUMMON_CARD, this, callfuncO_selector(SGPiecesLayer::summonPiecesListener));
	
    
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	float h = SGMainManager::shareMain()->getHeadHeight();
    
    
	//数据
    //新规则排序
 
    this->resortPiecesCard();
    
    _totalPage = ceilf((float)_allData->count() / EACH_PAGE_NUM);
	(_totalPage < 1) && (_totalPage = 1);
    
    //标题背景
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));

	
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGPiecesLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.02);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2 + 3));
//    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55, titleBg->getPosition().y - backBtn->getContentSize().height / 2 - 5));
    
	SGCCLabelTTF *title = SGCCLabelTTF::create(str_Pieceslayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccpAdd(titleBg->getPosition(), ccp(0, -titleBg->getContentSize().height*.52)));
    this->addChild(title, 2);
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height );
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-2, b));
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - titleBg->getContentSize().height ));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-15);
	
//	CCLayerColor * color = CCLayerColor::create(ccc4(255, 0, 0, 150) , tableView->getContentSize().width , tableView->getContentSize().height);
//    color->ignoreAnchorPointForPosition(false);
//    color->setAnchorPoint(tableView->getAnchorPoint());
//    color->setPosition(color->getPosition());
//    tableView->getParent()->addChild(color);
	
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
        tableView->setHorizontal(true);
    }
    
    //设置页数
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    pagecountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
    
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);

    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(-pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
    
	if (_allData->count()>0) {
        refreshPage(1);
    }
}


void SGPiecesLayer::refreshPage(int type)
{
	//    DDLog("ccc===================%d", type);
    datas->removeAllObjects();
    int pageCount =  _allData->count() > EACH_PAGE_NUM ? EACH_PAGE_NUM : _allData->count(); // 每页显示的个数
    
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

unsigned int SGPiecesLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

SNSTableViewCellItem *SGPiecesLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(num);//获取对应cell的数据
    
	SGPiecesCell *item = (SGPiecesCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
	if (NULL == item)
	{
		item = SGPiecesCell::create(this, card, isUseCommon, sg_piecesLayer);
	}
    else
    {
        item->updatePiecesCard(card);
    }
	item->setTouchEnabled(false);//设置item不可触摸
    return item;
}

void SGPiecesLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);//选择哪一item
    if (selectIndex >= 0 && selectIndex < datas->count())
    {
        SGPiecesCard* pc = dynamic_cast<SGPiecesCard*>(datas->objectAtIndex(selectIndex));
        if (pc)
        {
            SGMainManager::shareMain()->showCommonInfoBoxInProto(pc, this);
        }
    }
}

//召唤监听
void SGPiecesLayer::summonPiecesListener(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::SummonCardResponse *response = (main::SummonCardResponse *)request->m_msg;
	
    if (request)
	{
		if (response->state() == 1)//召唤成功
		{
			CCLOG("Summon Success");
			SGMainManager::shareMain()->showMessage(str_EquipPiecesLayer_str4);

			SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(card->getCallCardId());
			
			if (officerData)
			{
				SGOfficerCard *officerCard = new SGOfficerCard;

				//填充武将卡数据/////////////////////////////////////////////////////////////////////
				officerCard->setOfficerName(officerData->getOfficerName());
				officerCard->setMaxLevel(officerData->getOfficerMaxLevel());
				officerCard->setCurrStar(officerData->getOfficerCurrStarLevel());
				officerCard->setRace(officerData->getOfficerRace());
				officerCard->setItemId(officerData->getOfficerId());
				//转生最大星级
				officerCard->setUpgradestar(officerData->getUpgradestar());
                //武将计最大等级
                officerCard->setSkillMaxLevel(officerData->getSkillMaxLevel());
                //主将计最大等级
                officerCard->setLordMaxLevel(officerData->getLordMaxLevel());
				//性别和缘分
				officerCard->setGender((OfficerGender) officerData->getOfficerGender());
				officerCard->setFateList(officerData->getOfficerFateList());
               //武将原型ID
				officerCard->setProtoId(officerData->getOfficerProtoId());
                //转生次数
                officerCard->setAdNum(officerData->getAdvNum());
				//速度修正系数
				officerCard->setSpeedFactor(officerData->getOfficerSpeedFactor());
				//转生等级
				officerCard->setUpgradelevel(officerData->getUpgradelevel());
				if (officerData->getOfficerGeneralsSkill()&&!officerData->getOfficerGeneralsSkill()->isEqual(CCString::create("")))
				{
					officerCard->setOfficerSkil(SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(officerData->getOfficerGeneralsSkill()->getCString()));
				}
				if (officerData->getOfficerLordSkill() &&!officerData->getOfficerLordSkill()->isEqual(CCString::create("")))
				{
					int lordskill = SGStaticDataManager::shareStatic()->getAnyoneIdByBeanId(officerData->getOfficerLordSkill()->getCString());
					officerCard->setLordSkill(lordskill);
                    officerCard->setLordId(lordskill);
				}
				officerCard->setAtk(officerData->getOfficerBaseAtk());
				officerCard->setDef(officerData->getOfficerBaseDef());
				officerCard->setRound(officerData->getOfficerRound());
				officerCard->setSpeed(officerData->getOfficerBaseSpeed());
				officerCard->setMorale(officerData->getOfficerMorale());
				officerCard->setGoverning(officerData->getOfficerGoverning());
				officerCard->setSsid(response->ssid());
				officerCard->setHeadSuffixNum(officerData->getIconId());//12.05
				
				officerCard->autorelease();
				//填充武将卡数据/////////////////////////////////////////////////////////////////////
				
                this->resortPiecesCard();
                
				tableView->reloadData();//重新载入cell
                
				//将对应的卡牌传入
				SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create(SGPlayerInfo::sharePlayerInfo()->getOfficerCard(response->ssid()),11000);
				SGMainManager::shareMain()->showLayer(Layer,false);
				
				
				// 判定通用碎片数组是否有值
				CCArray *piecesArray = SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards();
				if (piecesArray->data && piecesArray->count() && piecesArray->data->arr)
				{
					piecesCommonTotalNum = ((SGPiecesCard *)(piecesArray->objectAtIndex(0)))->getPiecesCurNum();
				}
				else
				{
					piecesCommonTotalNum = 0;
				}
				
				commonCount->setString((CCString::createWithFormat(str_EquipPiecesLayer_str2, piecesCommonTotalNum))->getCString());
			}
			else
			{
				SGMainManager::shareMain()->showMessage(str_EquipPiecesLayer_str5);
			}
		}
		else
		{
			SGMainManager::shareMain()->showMessage(str_EquipPiecesLayer_str5);
		}
	}
	
}

//召唤武将回调
void SGPiecesLayer::summonPieces(SGPiecesCell *piecesItem)
{
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
		int totalCommon = piecesCommonTotalNum;
		
		//如果本身的碎片数量足够
		if (piecesCurr >= needNum)
		{
			piecesCurr -= needNum;//召唤后，减掉对应的数量
			summonSuccess = true;
//			CCLOG("Current Pieces Enough!");
		}
		else if (isUseCommon && piecesCommonTotalNum >= commonNum && piecesCurr + commonNum >= needNum)//如果本身碎片不够，且通用碎片可用，那么使用通用碎片
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

//void SGPiecesLayer::showPiecesLayer(bool isBeforeDel)
//{
//	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
//	SGEquipPiecesLayer *layer = (SGEquipPiecesLayer *)mainScene->getChildByTag(sg_equipPiecesLayer);
//	
//    if (layer) {
//        layer->removeFromParent();
//    }
//    layer = SGEquipPiecesLayer::create();
//	mainLayer->showblack();
//    mainLayer->setPortraitMenuCanTouch(false);
//	mainScene->addShowLayer(layer);
//	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
//}

void SGPiecesLayer::useCommonPieces()
{
	this->isUseCommon = !isUseCommon;
	CCLog("Officer isUseCommon == %d", isUseCommon);
	tableView->reloadData();
}

#include "SGAdvancedEffectLayer.h"
void SGPiecesLayer::backHandler()
{
	EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showBarrackslayer();
}

 void SGPiecesLayer::showPiecesinfo(SGPiecesCell *piecesItem)
{
    SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(selectIndex);//获取对应cell的数据
    SGMainManager::shareMain()->showPiecesInfoLayer(card,0, false);
}



