
#include "SGPiecesBagLayer.h"
#include "SGMainManager.h"
#include "SGEquipPiecesLayer.h"
#include "SGCardItem.h"
#include "SGPiecesBagCell.h"
#include "SGPiecesCard.h"
#include "SGGeneralInfoLayer.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGLevelUpLayer.h"
#include "SGStringConfig.h"
#include <algorithm>
#include "AppMacros.h"
#include "SGShareBox.h"
#include "SGGeneralsBagLayer.h"
#include "SGLootWiKiMainLayer.h"
#include "SGAdvancedEffectLayer.h"
#include "SGGuideManager.h"
#define EACH_PAGE_NUM 15

SGPiecesBagLayer::SGPiecesBagLayer() :
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
SGPiecesBagLayer::~SGPiecesBagLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_piecesBagLayer);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SUMMON_CARD);
    CC_SAFE_RELEASE(_allData);
}


void SGPiecesBagLayer::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(10);//设置menu的优先级
}

SGPiecesBagLayer *SGPiecesBagLayer::create()
{
    SGPiecesBagLayer *piecesLayer = new SGPiecesBagLayer();
    if (piecesLayer && piecesLayer->init(NULL, sg_piecesBagLayer))
    {
		//piecesLayer->datas = data;
        piecesLayer->initView();
		
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
    if (card1 == NULL || card2 == NULL) {
        return false;
    }
    //return card1->getCurrStar() > card2->getCurrStar();
    return card1->getPiecesCurNum() > card2->getPiecesCurNum();
}

//重新排序碎片
void SGPiecesBagLayer::resortPiecesCard()
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
 /*   SGPiecesCard * var1=NULL ;
    for(int i =0; i < tempPieces->count()-1 ; ++i)
    {
        int k = i ;
        
        for(int j = i+1 ; j < tempPieces->count() ; ++j)
        {
            SGPiecesCard * flag = dynamic_cast<SGPiecesCard * >(tempPieces->objectAtIndex(k));
            var1 = dynamic_cast<SGPiecesCard * >(tempPieces->objectAtIndex(j));
            if (var1->getCurrStar() > flag->getCurrStar()  )
            {
                k =  j ;
            }
        }
        
        if (k != i)
        {
            int temp = k;
            tempPieces->data->arr[k] = tempPieces->data->arr[i];
            tempPieces->data->arr[i] = tempPieces->data->arr[temp];
            
        }
    }
    
   */ 
   
    
    
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
    
    CCArray * officerCards = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    //flag = true 存在此武将对应的卡。
    bool flag = false;
    for (int i = 0; i < tempPieces->count(); i++)
    {
        flag = false;
        SGPiecesCard *tCard = (SGPiecesCard *)tempPieces->objectAtIndex(i);
        int officerProtoId = SGStaticDataManager::shareStatic()->getPiecesDictById(tCard->getItemId())->getOfficerProtoId();
        
        for(int k=0; k < officerCards->count() ; ++k)
        {
            SGOfficerCard * card = (SGOfficerCard *)officerCards->objectAtIndex(k) ;
            if( (card->getProtoId() == officerProtoId ) && (tCard->getItemType() == 0) )
            {
                flag = true;
                break;
            }
        }
        if (!flag )
        {
            _allData->addObject(tCard);
        }
    }
    datas->addObjectsFromArray(_allData);
}



void SGPiecesBagLayer::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_piecesBagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_piecesBagLayer);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_piecesBagLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_piecesBagLayer);
	ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_piecesBagLayer);
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_piecesBagLayer);
    
	//添加监听
	SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
	notification->addObserver(MSG_SUMMON_CARD, this, callfuncO_selector(SGPiecesBagLayer::summonPiecesListener));
	
    
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
/*
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,1);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,2);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,2);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,2);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,2);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,2);
*/	
	
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGPiecesBagLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    //backBtn->setScale(1.02);
	this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.5, titleBg->getPosition().y - backBtn->getContentSize().height / 2));
    
	CCSprite * title = CCSprite::createWithSpriteFrameName("officerBagTitle_2.png");
    title->setPosition(ccpAdd( titleBg->getPosition() , ccp(0, -titleBg->getContentSize().height*0.5)) );
    this->addChild(title, 2);
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height );
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-2, b));
    
    
	//添加内部UI
	//武将碎片button
    SGButton *officerBtn = SGButton::createFromLocal("officerBagBtnbg_2.png", str_generalsBagLayer_1, this, menu_selector(SGPiecesBagLayer::officerBtnClicked), ccp(0, 0), FONT_BOXINFO, ccWHITE ,30 );
	//装备碎片Button
	SGButton *pieceBtn = SGButton::createFromLocal("officerBagBtnbg_1.png", str_generalsBagLayer_2, this, menu_selector(SGPiecesBagLayer::pieceBtnClicked), ccp(0, 0),FONT_BOXINFO, ccYELLOW ,30);
    
    int limitTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_SUMMON_OFFICER, 4);
    pieceBtn->setTag(limitTag);
    
	this->addBtn(officerBtn);
	this->addBtn(pieceBtn);
//	officerBtn->setPosition(ccp(officerBtn->getContentSize().width , titleBg->getPositionY() - officerBtn->getContentSize().width *0.8));
//	pieceBtn->setPosition(ccp(officerBtn->getPositionX() + officerBtn->getContentSize().width, officerBtn->getPositionY()));
	//button下横线
	CCSprite *bottomBorder = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    bottomBorder->setAnchorPoint(ccp(0.5, 1));
    bottomBorder->setScaleX(15);
	bottomBorder->setScaleY(0.5);
	
	this->addChild(bottomBorder, 2);

	//通用碎片选框
	CCSprite *u1 = CCSprite::createWithSpriteFrameName("unuse_common_oieces.png");
	CCSprite *u2 = CCSprite::createWithSpriteFrameName("use_common_pieces.png");
	CCMenuItemSprite *unUsePic = CCMenuItemSprite::create(u1, u1);
	CCMenuItemSprite *usePic = CCMenuItemSprite::create(u2, u2);
	usePic->setVisible(false);
    unUsePic->setVisible(false);
	CCMenuItemToggle *toggleMenu = CCMenuItemToggle::createWithTarget(this, menu_selector(SGPiecesBagLayer::useCommonPieces), unUsePic, usePic, NULL);
	toggleMenu->setPosition(ccp(titleBg->getPositionX() + toggleMenu->getContentSize().width, titleBg->getPositionY() - toggleMenu->getContentSize().height * 2.6));
	
	CCMenu *useMenu = CCMenu::create(toggleMenu, NULL);//
	useMenu->setPosition(CCPointZero);
	this->addChild(useMenu, 9);
	
	
	//计算可用的通用碎片数量
	if (SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards()->count())
	{
		piecesCommonTotalNum = ((SGPiecesCard *)(SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards()->objectAtIndex(0)))->getPiecesCurNum();
	}
	//显示当前通用碎片数量
	commonCount = SGCCLabelTTF::create((CCString::createWithFormat(str_EquipPiecesLayer_str2, piecesCommonTotalNum))->getCString(), FONT_LISHU, 21);
	commonCount->setPosition(ccp(toggleMenu->getPositionX(), toggleMenu->getPositionY() - toggleMenu->getContentSize().height));
	this->addChild(commonCount, 2);
	commonCount->cocos2d::CCNode::setVisible(false);
	
	//设置tableview
	tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - titleBg->getContentSize().height - 90));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-15);
    
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_SUMMON_OFFICER, 6);
    tableView->setTag(tag);
	
	if(s.height == 1024)
    officerBtn->setPosition(ccp( tableView->getPositionX() + s.width/2-tableView->getContentSize().width/2 , titleBg->getPositionY() - officerBtn->getContentSize().width*0.8));
    else
        officerBtn->setPosition(ccp(officerBtn->getContentSize().width * 0.8  -2, titleBg->getPositionY() - officerBtn->getContentSize().width*0.8));
	pieceBtn->setPosition(ccp(officerBtn->getPositionX() + officerBtn->getContentSize().width, officerBtn->getPositionY()));
    bottomBorder->setPosition(ccpAdd(ccp(0, 0), ccp(0, officerBtn->getPositionY() - officerBtn->getContentSize().height / 2 )));
    
    //如果碎片可召唤 显示提示
    bool showNotice = false;
    for(int i=0 ; i<_allData->count() ; ++i)
    {
        SGPiecesCard * temp = (SGPiecesCard*)_allData->objectAtIndex(i);
        if(temp->getPiecesCurNum() >= temp->getSummonNeed())
        {
            showNotice = true;
            break;
        }
        
    }
	if(showNotice)
    {
        CCSprite * notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        notice->setPosition(ccpAdd(pieceBtn->getContentSize(), ccp(-6, -6)));
        pieceBtn->addChild(notice , 10);
    }
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


void SGPiecesBagLayer::refreshPage(int type)
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

unsigned int SGPiecesBagLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

SNSTableViewCellItem *SGPiecesBagLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(num);//获取对应cell的数据
    
	SGPiecesBagCell *item = (SGPiecesBagCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
	if (NULL == item)
	{
		item = SGPiecesBagCell::create(this, card, isUseCommon, sg_piecesBagLayer);
	}
    else
    {
        item->updatePiecesCard(card);
    }
	//item->setTouchEnabled(false);//设置item不可触摸
    return item;
}
void SGPiecesBagLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);//选择哪一item
}

//召唤监听
void SGPiecesBagLayer::summonPiecesListener(CCObject *sender)
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
//				SGGeneralInfoLayer *Layer = SGGeneralInfoLayer::create(SGPlayerInfo::sharePlayerInfo()->getOfficerCard(response->ssid()),11000);
//				SGMainManager::shareMain()->showLayer(Layer,false);
                
                
                SGMainManager::shareMain()->setWillAdvancedItemId(officerCard->getItemId());
                SGMainManager::shareMain()->setAfterAdvancedItemId(officerCard->getItemId());
                SGMainManager::shareMain()->setAfterAdvancedSsid(officerCard->getSsid());

                //新动画
                SGAdvancedEffectLayer *eff = SGAdvancedEffectLayer::create(BIT_OFFICER,
                                                                           SGMainManager::shareMain()->getWillAdvancedItemId(),
                                                                           SGMainManager::shareMain()->getAfterAdvancedItemId() , 2);
                
                SGMainManager::shareMain()->getMainScene()->addShowLayer(eff);
				
				
				// 判定通用碎片数组是否有值
//				CCArray *piecesArray = SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards();
//				if (piecesArray->data && piecesArray->count() && piecesArray->data->arr)
//				{
//					piecesCommonTotalNum = ((SGPiecesCard *)(piecesArray->objectAtIndex(0)))->getPiecesCurNum();
//				}
//				else
//				{
//					piecesCommonTotalNum = 0;
//				}
				
				//commonCount->setString((CCString::createWithFormat(str_EquipPiecesLayer_str2, piecesCommonTotalNum))->getCString());
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
void SGPiecesBagLayer::summonPieces(SGPiecesBagCell *piecesItem)
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

void SGPiecesBagLayer::useCommonPieces()
{
	this->isUseCommon = !isUseCommon;
	CCLog("Officer isUseCommon == %d", isUseCommon);
	tableView->reloadData();
}

void SGPiecesBagLayer::backHandler()
{
	EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showBarrackslayer();
}


void SGPiecesBagLayer::officerBtnClicked()
{
    SGMainManager::shareMain()->showGeneralsBagLayer(0);
}

void SGPiecesBagLayer::pieceBtnClicked()
{
	return ;
}


// void SGPiecesBagLayer::showPiecesinfo(SGPiecesBagCell *piecesItem)
//{
//    SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(selectIndex);//获取对应cell的数据
//    SGMainManager::shareMain()->showPiecesInfoLayer(card,1000, false);
//
//}

void SGPiecesBagLayer::pieceClickCallback(SGPiecesBagCell *piecesItem)
{
    SGPiecesCard *card = (SGPiecesCard *)datas->objectAtIndex(selectIndex);//获取对应cell的数据
    if(card->getSummonNeed() <= card->getPiecesCurNum())
        this->summonPieces(NULL);
    else
    {
        SGLootWiKiMainLayer * lootWiKiMainLayer = SGLootWiKiMainLayer::create( card->getItemId());
        SGMainManager::shareMain()->showBox(lootWiKiMainLayer);
    }
}

