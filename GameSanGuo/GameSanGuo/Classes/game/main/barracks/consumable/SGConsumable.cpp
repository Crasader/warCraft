//
//  SGConsumable.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-13.
//
//

#include "SGConsumable.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGConsumableCell.h"
#include "SGShowRewardBox.h"
#include "SGStringConfig.h"
#include "SGCommonInfoBox.h"

#define EACH_PAGE_NUM 15
#define SECTIONINFOVIEWTAG -1000
#define SECTIONTABLEVIEWBOTTOM   8
#define SECTIONINFOFONTSIZE   12
#define SECTIONDESCBG			40

#define CHANGE_BIG_NUM	10//卖出时每次的加的数量，下同
#define CHANGE_SMALL_NUM	1


#pragma mark "------------------消耗品Layer------------------"
SGConsumable::SGConsumable():
range(NULL),
pageLabel(NULL),
selectIndex(0),
lastSelectIndex(0),
lastMoveDis(0.0),
isMovingSection(false),
sectiontableView(NULL),
m_lastShowSection(NULL),
getMoneyNum(0)
{
	_nowPage = 0;
    _totalPage = 0;
	
	lastMoveDis = 0;
	selectIndex = 0;
    sectiontableView = NULL;
    lastSelectIndex = 0;
	m_lastShowSection = NULL;
}
SGConsumable::~SGConsumable()
{
//	CC_SAFE_RELEASE_NULL(sectiontableView);
	CC_SAFE_RELEASE(_allData);
	CC_SAFE_RELEASE(datas);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_consumablelayer);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USE_CONSUMABLE);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BATCHSELL);
}

SGConsumable *SGConsumable::create()
{
	SGConsumable *consum = new SGConsumable();
	
	if (consum && consum->init(NULL, sg_consumablelayer))
	{
		consum->initView();
		consum->autorelease();
		return consum;
	}
	CC_SAFE_DELETE(consum);
	return NULL;
}
//初始化基本道具数据
void SGConsumable::initData()
{
	_allData = CCArray::create();
	_allData->retain();
	datas = CCArray::create();
	datas->retain();
	isMovingSection = false;
}

//使用消耗品的监听
void SGConsumable::consumeUseListener(CCObject *sender)
{

	std::vector<int>		itemids;	//物品id组
	std::vector<int>		nums;		//每个对应的id物品的数量
	std::vector<int>		types;		//物品对应的类型
	SGSocketRequest *useConRes = (SGSocketRequest *)sender;
	if (useConRes)
	{
		main::UseConsumableItemResponse *useRes = (main::UseConsumableItemResponse *)useConRes->m_msg;
		if (useRes)
		{
			//返回
			if (useRes->state() == 0)
			{
				CCLOG("Use Failed!");
				SG_SHOW_WINDOW(CCString::createWithFormat(str_Consumable_tips1, useRes->msg().c_str())->getCString());
				//SG_SHOW_WINDOW(useRes->msg().c_str());
			}
			else if (useRes->state() == 1)
			{
				//SG_SHOW_WINDOW("物品使用成功!");
				
				//重新刷新datas数据,否则数量剩余最后一个会出现刷新
				CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getConsumableCards();
				datas->initWithArray(temp);
				
				// 重新刷新数据后,sectiontableView会会释放,这里提前做释放操作,方便后面代码重新生成
				if (sectiontableView)
				{
					sectiontableView->removeFromParentAndCleanup(true);
					//sectiontableView->removeFromParentAndCleanup(true);
					sectiontableView = NULL;
				}
                //用完数量再次排序
                std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("consumablesort");
                CCString *type = CCString::create(str);
                SGSortBox::sortCards(datas, type->intValue(),3);
                
				this->tableView->reloadData();

				//this->tableView->reloadData(false);//重新显示
				if (useRes->itemids_size() && useRes->nums_size() && useRes->types_size())
				{
					for (int i = 0; i < useRes->itemids_size(); i++)
					{
						itemids.push_back(useRes->itemids(i));
						nums.push_back(useRes->nums(i));
						types.push_back(useRes->types(i));
					}
					//显示掉落组
					this->showOpenedConsumableCardBox(itemids, nums, types);
				}
				else//如果是消耗药水类型的，要提示回复多少，“如 使用体力药水，回复体力10点”
				{
					SG_SHOW_WINDOW(CCString::createWithFormat(str_Consumable_tips2, useRes->msg().c_str())->getCString());
				}
			}
			
		}
	}
}






//使用道具，向服务器发425
void SGConsumable::useConsumable(SGConsumableCard *card)
{
//	SGConsumableCard *card = static_cast<SGConsumableCard *>(datas->objectAtIndex(selectIndex));
	
	main::UseConsumableItemRequest *useConReq = new main::UseConsumableItemRequest();
	useConReq->set_ssid(card->getSsid());//要使用的卡牌的ssid
	useConReq->set_num(1);//使用的数量，目前没有批量使用，数量一直给1

	SGSocketClient::sharedSocketClient()->send(MSG_USE_CONSUMABLE, useConReq);
}

//卖出卡牌的listener
void SGConsumable::sellConsumableCardListener(CCObject *sender)
{
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::SellRoleItemAllResponse *sellRes = (main::SellRoleItemAllResponse *)req->m_msg;
		if (sellRes->state() == 0)
		{
			SG_SHOW_WINDOW(str_Consumable_tips3);
		}
		else if (sellRes->state() == 1)
		{
			CCLOG("Success!");
			SG_SHOW_WINDOW(CCString::createWithFormat(str_Consumable_tips4, getMoneyNum)->getCString());
			moveSectiontableViewDown(true);
			hideSectiontableView();
			
			CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getConsumableCards();
			datas->initWithArray(temp);
			
			// 重新刷新数据后,sectiontableView会会释放,这里提前做释放操作,方便后面代码重新生成
			if (sectiontableView)
			{
				sectiontableView->removeFromParentAndCleanup(true);
				//sectiontableView->removeFromParentAndCleanup(true);
				sectiontableView = NULL;
			}
			this->tableView->reloadData();
		}
	}
}


void SGConsumable::initView()
{
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_USE_CONSUMABLE, this, callfuncO_selector(SGConsumable::consumeUseListener));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_BATCHSELL, this, callfuncO_selector(SGConsumable::sellConsumableCardListener));
	
	ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
    
	this->initData();
	
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("consumablesort");
    CCString *type = CCString::create(str);
    //CCLOG("%d",type->intValue());
	//初始化数据
    
	_allData->initWithArray(SGPlayerInfo::sharePlayerInfo()->getConsumableCards());
	SGSortBox::sortCards(_allData, type->intValue(),3);

    datas->removeAllObjects();
	
	//_totalPage = ceil(_allData->count() / EACH_PAGE_NUM);
	//(_totalPage < 1) && (_totalPage = 1);
	
    int last = _allData->count();
    //last > 15 && (last = 15);
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
	
	CCLabelTTF *title = CCLabelTTF::create(str_Consumable_tips5, FONT_BOXINFO, 32);
	title->setColor(ccc3(0xe9, 0xd3, 0x7a));
	this->addChild(title, 1);
	title->setPosition(ccpAdd(titleBg->getPosition(), ccp(0, -titleBg->getContentSize().height / 2)));
	
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGConsumable::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.01);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGConsumable::buttonClickSort),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 1));
    range->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0, titleBg->getPosition().y)));
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height + 25);
//    bg->setScaleY(s.height / bg->getContentSize().height);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
	
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, b));
    
	
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h  - titleBg->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-25);
    SGSortBox::updateBtn(range,6);
    //设置左右箭头
    float posY = tableView->getContentSize().height*.5f + tableView->getPositionY();
    CCSprite *spriteL = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    float widLR = spriteL->getContentSize().width*.5f;
    float hitLR = spriteL->getContentSize().height*.5f;
    spriteL->setPosition(ccp(widLR, posY-hitLR));
    spriteL->setVisible(false);
    spriteL->setTag(111);
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
	
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    pagecountbg->setAnchorPoint(ccp(0, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
	
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
	
	
	CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
	cardcountbg->setAnchorPoint(ccp(1, 0));
	this->addChild(cardcountbg,40);
	cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
//	int total = SGPlayerInfo::sharePlayerInfo()->getTotalCardsCount();
	SGCCLabelTTF *cardcounts = SGCCLabelTTF::create(CCString::createWithFormat(str_Consumable_str2,_allData->count())->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
	this->addChild(cardcounts,42);
    //modify by:zyc. merge into create.
	//cardcounts->setColor(ccc3(0xff, 0xf0, 0x00));
	cardcounts->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));

}


void SGConsumable::buttonClickSort()
{
	//    SGSortLayer *sortLayer = SGSortLayer::create(datas, this, this, ccpAdd(range->getPosition(), ccp(-range->getContentSize().width/2, -range->getContentSize().height/2)));
	//    SGMainManager::shareMain()->addShowLayer(sortLayer);
    SGSortBox *Box = SGSortBox::create(this, this, _allData, 3);
    SGMainManager::shareMain()->showBox(Box);
}

#pragma mark "------------------------显示获得宝箱的内容_BEGIN------------------------"
//使用礼包或者宝箱
void SGConsumable::showOpenedConsumableCardBox(std::vector<int> itemids, std::vector<int> nums, std::vector<int> types)
{
	//6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14天梯积分 15消耗品
	CCArray *consumables = CCArray::create();
	consumables->retain();
	int itemid = 0;
	int itemNum = 0;
	int itemType = 0;
	for (int i = 0; i < itemids.size(); i++)
	{
		SGMailSystemAccessoryDataModel *access=new SGMailSystemAccessoryDataModel();
		itemid = itemids[i];
		itemNum = nums[i];
		itemType = types[i];
		
		access->setItemId(itemid);
		access->setType(itemType);
		access->setCount(itemNum);
		access->autorelease();
		
		consumables->addObject(access);
	}
	SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, consumables);
    SGMainManager::shareMain()->showBox(rewardBox);
//    consumables->release();
	//tableView->reloadData(false);


}

#pragma mark "------------------------显示获得宝箱的内容_END------------------------"
void SGConsumable::backHandler()
{
	EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showBarrackslayer();
}

//排序回调
void SGConsumable::sortLayerCallBack(cocos2d::CCArray *cardArray)
{
    _nowPage = 0;
    this->refreshPage(1);
    this->tableView->reloadData();
    SGSortBox::updateBtn(range,6);
}

SNSTableViewCellItem *SGConsumable::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	int index = tableView->getItemPointerWithIndexPath(indexPath);
	SGConsumableCard *temp = static_cast<SGConsumableCard *>(datas->objectAtIndex(index));
	SGConsumableCell *consumeCell = (SGConsumableCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
	//consumeCell = SGConsumableCell::create(temp, this);
    
	if (!consumeCell)
	{
		consumeCell = SGConsumableCell::create(temp, this);
	}
	else if (index < datas->count())// 更新数量显示
	{
		consumeCell->updateCell(temp);
	}
	
	return consumeCell;
}

unsigned int SGConsumable::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	return datas->count();
}
void SGConsumable::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	selectIndex = tableView->getItemPointerWithIndexPath(indexPath);
	
}

void SGConsumable::refreshPage(int type)
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


void SGConsumable::showConsumeInfoHandler(SGConsumableCell *consumeCell)
{
    if (isMovingSection || consumeCell == NULL) {
        return;
    }
	creatConsumeInfoView(consumeCell);
    return;
	
    if (sectiontableView) {
        
        if (sectiontableView->isVisible())
        {
            if (m_lastShowSection == consumeCell) {
                if (lastMoveDis>0) {
                    tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                }
                else{
                    tableView->moveCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                }
                tableView->resetDownLimit = 0;
                moveSectiontableViewDown(false);
                lastMoveDis = 0;
                lastSelectIndex = 0;
            }
            else
            {
                CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
                sectiontableView->stopAllActions();
                sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height+4));
                hideSectiontableView();
                
                if (lastMoveDis>0) {
                    tableView->setCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                }
                else{
                    tableView->setCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                }
                
                tableView->resetDownLimit = 0;
                lastMoveDis = 0;
                lastSelectIndex = 0;
                
                SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
                SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
                CCPoint selectViewCellPos = tableViewCell->getPosition();
                float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
                
                sectiontableView->setVisible(true);
                CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - consumeCell->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
                lastSelectIndex = selectIndex;
                lastMoveDis = moveDis;
                float posX = (consumeCell->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
                if(moveDis>0)
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
                    tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
                }
                else
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                    tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
                }
                tableView->resetDownLimit = sectiontableView->getContentSize().height;
                moveSectiontableViewDown(true);
                sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(consumeCell->bgSize.width/sectiontableView->getContentSize().width);
                
            }
        }
        else
        {
            SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
            SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
            CCPoint selectViewCellPos = tableViewCell->getPosition();
            float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
            
            sectiontableView->setVisible(true);
            CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - consumeCell->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
            lastSelectIndex = selectIndex;
            lastMoveDis = moveDis;
            float posX = (consumeCell->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
            if(moveDis>0)
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
                tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
            }
            else
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
            }
            tableView->resetDownLimit = sectiontableView->getContentSize().height;
            moveSectiontableViewDown(true);
            sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(consumeCell->bgSize.width/sectiontableView->getContentSize().width);
        }
        
        m_lastShowSection = consumeCell;
    }
}

void SGConsumable::moveSectiontableViewDown(bool isDown)
{
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    sectionInfoView->stopAllActions();
    
    if (isDown)
    {
        sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height));
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME,CCPointZero));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGConsumable::resetMovingSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    else
    {
        sectiontableView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME, ccp(0,sectiontableView->getContentSize().height+4)));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGConsumable::hideSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    isMovingSection = true;
}

void SGConsumable::hideSectiontableView()
{
    if (sectiontableView != NULL) {
        sectiontableView->setVisible(false);
        isMovingSection = false;
    }
}

void SGConsumable::resetMovingSectiontableView()
{
    if (sectiontableView != NULL) {
        isMovingSection = false;
    }
}


void SGConsumable::creatConsumeInfoView(SGConsumableCell *consumeCell)
{
	
	SGConsumableCard *card = (SGConsumableCard *)(datas->objectAtIndex(selectIndex));
    if (card == NULL) {
        return;
    }
    
    //复杂的弹簧框逻辑由通用信息BOX替代。
    m_lastShowSection = consumeCell;
    SGMainManager::shareMain()->showCommonInfoBoxInProto(card, this);
    return;
    
    CCSprite *sectionInfoView = NULL;
	CCSprite *consumeInfo = NULL;
//    float contentHeight = 0;
//    float fontWid = FONTSIZE(SECTIONINFOFONTSIZE);
    CCMenu *menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_consumablelayer);
	//sectiontableView = NULL;
    if (!sectiontableView)
    {

		sectionInfoView = CCSprite::createWithSpriteFrameName("bban.png");//所有任务点开后的背景
        
        sectionInfoView->setAnchorPoint(CCPointZero);
        sectionInfoView->ignoreAnchorPointForPosition(false);
        sectionInfoView->setPosition(ccp((consumeCell->bgSize.width - sectionInfoView->getContentSize().width)/2,150));
        sectionInfoView->setOpacity(255);
        sectiontableView = new NewScrollView();
        if (sectiontableView && sectiontableView->initGSScrollViewFrame(CCRectMake(0, 0, consumeCell->bgSize.width, sectionInfoView->getContentSize().height)))
        {
            sectiontableView->autorelease();
            CCPoint tableViewPos = tableView->getParent()->convertToWorldSpace(tableView->getPosition());
			sectiontableView->setParentScissorFrame(CCRectMake(tableViewPos.x, tableViewPos.y, tableView->getScissorFrame().size.width, tableView->getScissorFrame().size.height));
        }
        
        sectiontableView->setAnchorPoint(CCPointZero);
        sectiontableView->ignoreAnchorPointForPosition(false);
        sectiontableView->addChild(sectionInfoView,1,SECTIONINFOVIEWTAG);
		
		//显示消耗品描述
		consumeInfo = CCSprite::createWithSpriteFrameName("consumable_desc_bg.png");
		consumeInfo->setAnchorPoint(ccp(0, 1));
		sectionInfoView->addChild(consumeInfo, 1, SECTIONDESCBG);
		consumeInfo->setPosition(ccpAdd(sectionInfoView->getPosition(), ccp(20, 60)));
		
		//描述文字
		SGCCLabelTTF *descInfo = SGCCLabelTTF::create(card->getconsumableDesc()->getCString(), FONT_BOXINFO, 22, CCSizeMake(consumeInfo->getContentSize().width - 35, consumeInfo->getContentSize().height - 20));
//		SGCCLabelTTF *descInfo = SGCCLabelTTF::create("可打开白银宝箱，可打开白银宝箱，可打开白银宝箱，可打开白银宝箱，可打开白银宝箱，可打开白银宝箱，可打开白银宝箱，可打开白银宝箱，", FONT_BOXINFO, 22);
		descInfo->setAnchorPoint(ccp(0, 0.5));

//		descInfo->setColor(ccc3(0xf1, 0xba, 0x0b));
		
		sectionInfoView->addChild(descInfo, 1, -10);
		descInfo->setPosition(ccpAdd(consumeInfo->getPosition(), ccp(20, -100)));
		
		//卖出按钮
		SGButton *sellButton = SGButton::createFromLocal("batch_sell.png", str_Sale_, this, menu_selector(SGConsumable::sellComsuableHandler), CCPointZero, FONT_PANGWA, ccRED, 32);
		sellButton->setAnchorPoint(ccp(0, 1));
		sellButton->setScale(0.8);
		menu->addChild(sellButton);
		sellButton->setPosition(ccpAdd(consumeInfo->getPosition(), ccp(consumeInfo->getContentSize().width + sellButton->getContentSize().width / 5, 0)));
		
		//展示按钮
		SGButton *showButton = SGButton::createFromLocal("batch_sell.png", str_Show_, this, menu_selector(SGConsumable::showConsumeHandler), CCPointZero, FONT_PANGWA, ccc3(0xff, 0xc2, 00), 32);
		showButton->setAnchorPoint(ccp(0, 1));
		showButton->setScale(0.8);
		menu->addChild(showButton);
		showButton->setPosition(ccpAdd(consumeInfo->getPosition(), ccp(consumeInfo->getContentSize().width + sellButton->getContentSize().width / 5,
																	   -showButton->getContentSize().height * 1.5)));
		
		sectionInfoView->addChild(menu, 1);
		
		
		sectiontableView->setVisible(false);
        tableView->m_body->addChild(sectiontableView);
	}
	else
    {
		sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
		if (sectionInfoView)
		{
			SGCCLabelTTF *desc = (SGCCLabelTTF *)sectionInfoView->getChildByTag(-10);
			desc->setString(card->getconsumableDesc()->getCString());
		}
        //sectiontableView = NULL;
    }
	
}

//下拉菜单上的卖出按钮
void SGConsumable::sellComsuableHandler()
{
	CCLOG("Sell Comsumable!");
	SGConsumableCard *card = static_cast<SGConsumableCard *>(datas->objectAtIndex(selectIndex));
	SGConsumableSellBox *sellBox = SGConsumableSellBox::create(card, this);
	SGMainManager::shareMain()->showBox(sellBox);
}
#pragma mark "------------------展示------------------"
//下拉菜单上的展示按钮
void SGConsumable::showConsumeHandler()
{
	CCLOG("Show Consumable!");
	SGConsumableCard *card = static_cast<SGConsumableCard *>(datas->objectAtIndex(selectIndex));
	
	SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(card->getOfficerName(), 6, card->getCurrStar(), card->getSsid());
	
	
	//SGMainManager::shareMain()->showConsumableInfoLayer(card);
}
// 数据刷新
void SGConsumable::freshView()
{
	moveSectiontableViewDown(true);
	hideSectiontableView();
	
	// 重新刷新数据后,sectiontableView会会释放,这里提前做释放操作,方便后面代码重新生成
	if (sectiontableView)
	{
		sectiontableView->removeFromParentAndCleanup(true);
		//sectiontableView->removeFromParentAndCleanup(true);
		sectiontableView = NULL;
	}
	this->tableView->reloadData();
}



#pragma mark "------------------卖出选择框------------------"
//消耗品卖出弹窗
SGConsumableSellBox::SGConsumableSellBox():
sellNum(0),
sellNumLabel(NULL),
sellMoney(0),
moneyNumLabel(NULL),
del(NULL),
money(NULL),
m_card(NULL)
{
	m_card = NULL;
	money = NULL;
	moneyNumLabel = NULL;
	sellMoney = 0;
	sellNum = 0;
}
SGConsumableSellBox::~SGConsumableSellBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_consumableSellBox);
}
//创建box
SGConsumableSellBox *SGConsumableSellBox::create(SGConsumableCard *card, SGBoxDelegate *del)
{
	SGConsumableSellBox *sellBox = new SGConsumableSellBox();

	if (sellBox && sellBox->init(del, box_consumableSell, CCSizeMake(600, 644)))
	{
		sellBox->del = del;
		sellBox->m_card = card;
		sellBox->initView();
		sellBox->autorelease();
		
		return sellBox;
	}
	CC_SAFE_RELEASE(sellBox);
	return NULL;
}

void SGConsumableSellBox::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_box_consumableSellBox);
	
	CCPoint center = SGLayout::getPoint(kMiddleCenter);
	fontbg->setPreferredSize(CCSize(480, 352));
//二个确定取消按钮
	SGButton *okButton = SGButton::create("gonggao_an.png", "public_font_queding.png", this, menu_selector(SGConsumableSellBox::sellConfirmHandler),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::create("gonggao_an.png", "public_font_quxiao.png", this, menu_selector(SGConsumableSellBox::sellCancelHandler),CCPointZero,false,true);
	this->addBtn(okButton);
	this->addBtn(cancelButton);
	okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+5 -95)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+5-95)));
	
//标题文字
	SGCCLabelTTF *title = SGCCLabelTTF::create(str_Consumable_str3, FONT_PANGWA, 36 , ccc3(180, 0, 0));
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(180, 0, 0));
	this->addChild(title, 1);
	title->setPosition(ccpAdd(biao->getPosition(), ccp(0, -30)));
	
//消耗品信息
	CCSprite *headBg = CCSprite::createWithSpriteFrameName("card_bj.png");
	this->addChild(headBg, 1);
	SGMainManager::shareMain()->addHeadIconPlistByNum(m_card->getHeadSuffixNum(), sg_consumablelayer);
	CCSprite *headIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png", m_card->getHeadSuffixNum())->getCString());
	this->addChild(headIcon, 1);
	
	int currStar = m_card->getCurrStar();
	if (currStar > 1)
		currStar -= 1;
	CCSprite *border = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png", currStar)->getCString());
	this->addChild(border, 2);
	SGCCLabelTTF *name = SGCCLabelTTF::create(m_card->getOfficerName()->getCString(), FONT_BOXINFO, 33 , ccRED);
    //modify by:zyc. merge into create.
	//name->setColor(ccRED);
	name->setAnchorPoint(ccp(0, 0.5));
	this->addChild(name, 1);
	SGCCLabelTTF *info = SGCCLabelTTF::create(str_Consumable_str4, FONT_BOXINFO, 28);
	info->setAnchorPoint(ccp(0, 0.5));
	this->addChild(info, 1);
	
	headIcon->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width / 3, fontbg->getContentSize().height / 4)));
	headBg->setPosition(ccpAdd(headIcon->getPosition(), ccp(0, 0)));
	border->setPosition(ccpAdd(headIcon->getPosition(), ccp(0, 0)));
	name->setPosition(ccpAdd(headIcon->getPosition(), ccp(headIcon->getContentSize().width, headIcon->getContentSize().height / 4)));
	info->setPosition(ccpAdd(headIcon->getPosition(), ccp(headIcon->getContentSize().width, -headIcon->getContentSize().height / 2 + info->getContentSize().height / 2)));
	
//加入数据操作区
	SGButton *ad1 = SGButton::createFromLocal("consumable_add_num.png", "+", this, menu_selector(SGConsumableSellBox::add1), ccp(0, 2), FONT_PANGWA, ccWHITE, 40);
	SGButton *ad10 = SGButton::createFromLocal("consumable_add_num.png", "+10", this, menu_selector(SGConsumableSellBox::add10), ccp(0, 2), FONT_PANGWA, ccWHITE, 20);
	SGButton *min1 = SGButton::createFromLocal("consumable_add_num.png", "-", this, menu_selector(SGConsumableSellBox::minus1), ccp(0, 2), FONT_PANGWA, ccWHITE, 40);
	SGButton *min10 = SGButton::createFromLocal("consumable_add_num.png", "-10", this, menu_selector(SGConsumableSellBox::minus10), ccp(0, 2), FONT_PANGWA, ccWHITE, 20);
	
	this->addBtn(ad1);
	this->addBtn(ad10);
	this->addBtn(min1);
	this->addBtn(min10);
	
	min10->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width / 3, -fontbg->getContentSize().height / 10)));
	min1->setPosition(ccpAdd(min10->getPosition(), ccp(min10->getContentSize().width + 10, 0)));
	ad1->setPosition(ccpAdd(min1->getPosition(), ccp(min1->getContentSize().width * 3.5, 0)));
	ad10->setPosition(ccpAdd(ad1->getPosition(), ccp(ad1->getContentSize().width + 10, 0)));
	
//中间显示数量的框
	sellNumLabel = CCLabelTTF::create(CCString::createWithFormat("%d", sellNum)->getCString(), FONT_BOXINFO, 40);
	sellNumLabel->setColor(ccBLACK);
	this->addChild(sellNumLabel, 1);
	sellNumLabel->setPosition(ccpAdd(min1->getPosition(), ccp(min1->getContentSize().width * 1.5, 0)));
	
//底部金钱信息
	CCLabelTTF *moneyInfo = CCLabelTTF::create(str_Consumable_str5, FONT_PANGWA, 32);
	moneyInfo->setColor(ccBLACK);
	this->addChild(moneyInfo, 1);
	moneyInfo->setPosition(ccpAdd(min10->getPosition(), ccp(min10->getContentSize().width, -ad10->getContentSize().height * 1.4)));
	CCSprite *coin = CCSprite::createWithSpriteFrameName("coinicon.png")	;
	coin->setAnchorPoint(ccp(0, 0.5));
	this->addChild(coin, 1);
	
	coin->setPosition(ccpAdd(moneyInfo->getPosition(), ccp(moneyInfo->getContentSize().width / 2 + 10, 0)));
	
	moneyNumLabel = SGCCLabelTTF::create(CCString::createWithFormat(": %d", sellMoney)->getCString(), FONT_BOXINFO, 36 , ccc3(0x5d, 0x0f, 0x00));
	moneyNumLabel->setAnchorPoint(ccp(0, 0.5));
    //modify by:zyc. merge into create.
	//moneyNumLabel->setColor(ccc3(0x5d, 0x0f, 0x00));
	this->addChild(moneyNumLabel, 1);
	moneyNumLabel->setPosition(ccpAdd(coin->getPosition(), ccp(coin->getContentSize().width * 1.2, 0)));
	
}
void SGConsumableSellBox::add1()
{
	sellNum += CHANGE_SMALL_NUM;
	//如果超过最大数量
	
	if (sellNum > m_card->getCurrNum())
	{
		sellNum = m_card->getCurrNum();
	}
	sellNumLabel->setString(CCString::createWithFormat("%d", sellNum)->getCString());
	moneyNumLabel->setString(CCString::createWithFormat(": %d", sellMoney = (sellNum * m_card->getSell()))->getCString());
}
void SGConsumableSellBox::add10()
{
	sellNum += CHANGE_BIG_NUM;
	//如果超过最大数量
	if (sellNum > m_card->getCurrNum())
	{
		sellNum = m_card->getCurrNum();
	}
	sellNumLabel->setString(CCString::createWithFormat("%d", sellNum)->getCString());
	moneyNumLabel->setString(CCString::createWithFormat(": %d", sellMoney = (sellNum * m_card->getSell()))->getCString());
}
void SGConsumableSellBox::minus1()
{
	sellNum -= CHANGE_SMALL_NUM;
	//如果小于0
	if (sellNum < 0)
	{
		sellNum = 0;
	}
	sellNumLabel->setString(CCString::createWithFormat("%d", sellNum)->getCString());
	moneyNumLabel->setString(CCString::createWithFormat(": %d", sellMoney = (sellNum * m_card->getSell()))->getCString());
}
void SGConsumableSellBox::minus10()
{
	sellNum -= CHANGE_BIG_NUM;
	//如果小于0
	if (sellNum < 0)
	{
		sellNum = 0;
	}
	sellNumLabel->setString(CCString::createWithFormat("%d", sellNum)->getCString());
	moneyNumLabel->setString(CCString::createWithFormat(": %d", sellMoney = (sellNum * m_card->getSell()))->getCString());
}

void SGConsumableSellBox::sellCancelHandler()
{
	CCLOG("Sell Cancel!");
//	if (del)
//	{
//		static_cast<SGConsumable *>(del)->freshView();
//	}
	
	this->boxCloseWithOutCallBack();

	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
}

void SGConsumableSellBox::sellConfirmHandler()
{
	CCLOG("Sell Confirm!");
	if (sellNum == 0)
	{
		SG_SHOW_WINDOW(str_Consumable_str6);
		return ;
	}
	
	this->boxClose();
	SGConsumableSellConfirmBox *confirmBox = SGConsumableSellConfirmBox::create(del, sellNum, sellMoney, m_card->getOfficerName(), m_card->getSsid());
	SGMainManager::shareMain()->showBox(confirmBox);
}


#pragma mark "------------------二次确认框------------------"
//二次确认框
SGConsumableSellConfirmBox::SGConsumableSellConfirmBox():
del(NULL),
num(0),
money(0),
name(NULL),
cardId(0)
{
	num = 0;
	money = 0;
	name = NULL;
	del = NULL;
}
SGConsumableSellConfirmBox::~SGConsumableSellConfirmBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_consumableSellConfirmBox);
}


SGConsumableSellConfirmBox *SGConsumableSellConfirmBox::create(SGBoxDelegate *del, int sellNum, int sellMoney, CCString *name, int cardId)
{
	SGConsumableSellConfirmBox *confirmBox = new SGConsumableSellConfirmBox();
	if (confirmBox && confirmBox->init(del, box_sellconfirm, CCSizeMake(550, 390)))
	{
		confirmBox->num = sellNum;
		confirmBox->money = sellMoney;
		confirmBox->name = name;
		confirmBox->cardId = cardId;
		confirmBox->del = del;
		confirmBox->initView();
		confirmBox->autorelease();
		return confirmBox;
	}
	CC_SAFE_RELEASE(confirmBox);
	return NULL;
}

void SGConsumableSellConfirmBox::initView()
{
	
	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_box_consumableSellConfirmBox);
	
	CCPoint center = SGLayout::getPoint(kMiddleCenter);
	fontbg->setPreferredSize(CCSize(450, 162));
	fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 30)));
	SGButton *okButton = SGButton::create("gonggao_an.png", "public_font_queding.png", this, menu_selector(SGConsumableSellConfirmBox::sellConfirmHandler),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::create("gonggao_an.png", "public_font_quxiao.png", this, menu_selector(SGConsumableSellConfirmBox::sellCancelHandler),CCPointZero,false,true);
	this->addBtn(okButton);
	this->addBtn(cancelButton);
	okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+35)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+35)));
	
	SGCCLabelTTF *title = SGCCLabelTTF::create(str_Consumable_str7, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
	//title->setOutSideColor(ccc3(143,87,0));
	this->addChild(title, 1);
	title->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width / 30, fontbg->getContentSize().height / 1.5)));
	
	
	SGCCLabelTTF *tips = SGCCLabelTTF::create(CCString::createWithFormat(str_Consumable_str8, num, name->getCString(), money)->getCString(),
											  FONT_PANGWA, 30, CCSizeMake(25*16, 200));
    tips->setAnchorPoint(ccp(.5, 1));
	//tips->setColor(ccRED);
    tips->setPosition(ccpAdd(biao->getPosition(), ccp(10, -60)));
	this->addChild(tips, 1);
}

//二次确认，直接卖出，监听加在SGConsumable里
void SGConsumableSellConfirmBox::sellConfirmHandler()
{
	//发消息
	main::SellRoleItemAllRequest *sellConsume = new main::SellRoleItemAllRequest();
	sellConsume->add_itemid(cardId);//要卖出卡牌的id
	sellConsume->set_num(num);//数量
	sellConsume->set_type(15);//卖出卡牌的类型
	
	SGSocketClient::sharedSocketClient()->send(MSG_BATCHSELL, sellConsume);
	static_cast<SGConsumable *>(del)->setMoneyNum(money);
	
	this->boxCloseWithOutCallBack();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
}

//弹窗关闭
void SGConsumableSellConfirmBox::sellCancelHandler()
{
	if (del)
	{
		static_cast<SGConsumable *>(del)->freshView();
	}
	this->boxCloseWithOutCallBack();
	
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
}





















