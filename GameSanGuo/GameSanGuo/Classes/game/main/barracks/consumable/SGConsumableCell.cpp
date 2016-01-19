//
//  SGConsumableCell.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-15.
//
//

#include "SGConsumableCell.h"
#include "SGConsumableDataModel.h"
#include "SGStaticDataManager.h"
#include <cstring>
#include "SGStringConfig.h"

#define MENU_TAG 1024
#define BUTTON_TAG -20

#pragma mark "--------------消耗品cell--------------"
SGConsumableCell::SGConsumableCell():
consumeDel(NULL),
consumeName(NULL),
m_card(NULL),
cardNum(NULL),
consumeIcon(NULL),
border(NULL),
bgBtn(NULL),
menu(NULL),
useConsume(NULL)
{
	
}
SGConsumableCell::~SGConsumableCell()
{
	//CC_SAFE_RELEASE(m_card);
}
//创建cell，
SGConsumableCell *SGConsumableCell::create(SGConsumableCard *card, SGConsumableDelegate *del)
{
	SGConsumableCell *consumeCell = new SGConsumableCell();
	if (consumeCell && consumeCell->init())
	{
		consumeCell->consumeDel = del;
		consumeCell->initData(card);
		consumeCell->initView();
		consumeCell->autorelease();
		
		return consumeCell;
	}
	CC_SAFE_RELEASE(consumeCell);
	return NULL;
}
//初始化m_card
void SGConsumableCell::initData(SGConsumableCard *card)
{
	m_card = card;
	m_card->retain();
}

void SGConsumableCell::consumeUseHandler()
{
	if (consumeDel)
	{
		consumeDel->useConsumable(m_card);
	}
}

//可以主动使用的卡片,是不是足够使用条件
bool SGConsumableCell::isCanUseConsumable(SGConsumableCard *card)
{
	//能否主动使用
	CCString *useCondition = card->getLimitAndNum();
	//CCLOG("%s", useCondition->getCString());
	
	CCArray *limitArray = CCArray::create();
	
	//如果有条件
	if (useCondition)
	{
		limitArray = split(useCondition->getCString(), ",");
	}
	
	int conditionNum = 0;
	bool isCanUse = false;
	CCObject *obj = NULL;
	
	//拆分每个条件
	CCARRAY_FOREACH(limitArray, obj)
	{
		CCString *con = (CCString *)obj;
		std::string str(con->getCString());
		std::string idStr;//需要物品的ID
		std::string numStr;//需要物品的数量
		int index = -1;
		if ((index = str.find(";")) != std::string::npos)
		{
			idStr = str.substr(0, index);
			numStr = str.substr(index + 1);
		}
		int id = CCString::create(idStr)->intValue();
		int num = CCString::create(numStr)->intValue();
		
		//SGConsumableDataModel *conData = SGStaticDataManager::shareStatic()->getConsumableById(id);
		for (int i = 0; i < SGPlayerInfo::sharePlayerInfo()->getConsumableCards()->count(); i++)
		{
			SGConsumableCard *playerConsum = static_cast<SGConsumableCard *>(SGPlayerInfo::sharePlayerInfo()->getConsumableCards()->objectAtIndex(i));
			if (playerConsum->getItemId() == id && playerConsum->getCurrNum() >= num)//对应id的消耗品的卡牌在玩家的卡牌中含有，并且数量足够使用
			{
				conditionNum++;//条件数符合limitArray的数量，即是可以使用，否则不可以使用
				break;
			}
		}
	}
	//符合条件的数量与数组中的限制数量相一致，说明满足使用的条件
	if (conditionNum == limitArray->count() || !strcmp(useCondition->getCString(), "-1"))//如果条件足够,或者没有限制条件的
	{
		isCanUse = true;
	}
	
	return isCanUse;
}

void SGConsumableCell::initView()
{
	
	//放使用的button

	//消耗品的名称
	char iconName[256] = "\0";
	//获取对应的iconId
	int iconId = m_card->getHeadSuffixNum();
	//转换出对应的头像名称
	sprintf(iconName, "head%d.png", iconId);
	
	if (iconId == -1)
	{
		strcpy(iconName, "mall_gift_pack.png");
	}
	else
	{
		SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_consumablelayer);
	}
	
	consumeIcon = CCSprite::createWithSpriteFrameName(iconName);
	consumeIcon->setPosition(ccpAdd(ccp(-bgSize.width/2+ZoomInX(20), 2), ccp(0, 0)));
	this->addChild(consumeIcon, 10);

	
	
	//获取星级，得到对应的品质框
	char consumeBorder[256] = "\0";
	//星级
	int starLvl = m_card->getCurrStar();
	if (starLvl > 1)//一星与二星用同一个相同的品质框
		starLvl -= 1;
	sprintf(iconName, "equipPinzhi_%d_0.png", starLvl);
	
	border = CCSprite::createWithSpriteFrameName(iconName);
	this->addChild(border, 1);
	border->setPosition(consumeIcon->getPosition());
	

	
	//设置使用button
	useConsume = SGButton::createFromLocal("store_exchangebtnbg.png", str_use, this, menu_selector(SGConsumableCell::consumeUseHandler),
										   CCPointZero, FONT_PANGWA, ccc3(0xd9, 0xac, 0x29), 28);
	useConsume->setPosition(ccp(bgSize.width/2 - useConsume->getContentSize().width/1.5, 0));//暂时设置，按需要调整
	//useConsume->setTag(-20);
	
	bool isCanUse = this->isCanUseConsumable(m_card);
/*
	//能否主动使用
	CCString *useCondition = m_card->getLimitAndNum();
	//CCLOG("%s", useCondition->getCString());
	
	CCArray *limitArray = CCArray::create();
	
	//如果有条件
	if (useCondition)
	{
		limitArray = split(useCondition->getCString(), ",");
	}
	
	int conditionNum = 0;
	bool isCanUse = false;
	CCObject *obj = NULL;
	
	//拆分每个条件
	CCARRAY_FOREACH(limitArray, obj)
	{
		CCString *con = (CCString *)obj;
		std::string str(con->getCString());
		std::string idStr;//需要物品的ID
		std::string numStr;//需要物品的数量
		int index = -1;
		if ((index = str.find(";")) != std::string::npos)
		{
			idStr = str.substr(0, index);
			numStr = str.substr(index + 1);
		}
		int id = CCString::create(idStr)->intValue();
		int num = CCString::create(numStr)->intValue();
		
		//SGConsumableDataModel *conData = SGStaticDataManager::shareStatic()->getConsumableById(id);
		for (int i = 0; i < SGPlayerInfo::sharePlayerInfo()->getConsumableCards()->count(); i++)
		{
			SGConsumableCard *playerConsum = static_cast<SGConsumableCard *>(SGPlayerInfo::sharePlayerInfo()->getConsumableCards()->objectAtIndex(i));
			if (playerConsum->getItemId() == id && playerConsum->getCurrNum() >= num)//对应id的消耗品的卡牌在玩家的卡牌中含有，并且数量足够使用
			{
				conditionNum++;//条件数符合limitArray的数量，即是可以使用，否则不可以使用
				break;
			}
		}
	}
	//符合条件的数量与数组中的限制数量相一致，说明满足使用的条件
	if (conditionNum == limitArray->count() || !strcmp(useCondition->getCString(), "-1"))//如果条件足够,或者没有限制条件的
	{
		isCanUse = true;
	}
*/
	
	//重设背景
	bgBtn = SGButton::create("item_bigbg.png",
									   NULL,
									   this,
									   menu_selector(SGConsumableCell::showConsumableInfo),
									   ccp(0, 0),
									   false,
									   true);
	bgBtn->setAnchorPoint(ccp(0.5,0.5));
	bgBtn->ignoreAnchorPointForPosition(false);

	bgBtn->setPosition(ccp(-useConsume->getContentSize().width*1.2/2, 0));
	bgBtn->setContentSize(CCSizeMake(bgBtn->getContentSize().width- useConsume->getContentSize().width*1.2, bgBtn->getContentSize().height));

	
	if (m_card->getCanUseDriving() == 1 && isCanUse)
	{
		//加入sgbutton至menu中
		menu = CCMenu::create();
		this->addChild(menu);
		menu->setTag(MENU_TAG);
		menu->setPosition(CCPointZero);
		menu->addChild(useConsume, 1, BUTTON_TAG);
	}//是否满足使用条件,如果不满足使用条件就蒙灰
	else if (m_card->getCanUseDriving() && !isCanUse)
	{
		menu = CCMenu::create();
		this->addChild(menu);
		menu->setTag(MENU_TAG);
		menu->setPosition(CCPointZero);
		useConsume->setImage("barrack_anniu_hui.png");
		useConsume->setFontColor(ccc3(80, 80, 80));
		useConsume->setCallBack(this, menu_selector(SGConsumableCell::consumeUnuseHandler));
		//加入sgbutton至menu中
		menu->addChild(useConsume, 1, BUTTON_TAG);
	}
	else//不能主动使用，如钥匙
	{
		bgBtn->setPosition(ccp(-useConsume->getContentSize().width * 0.1, 0));
		bgBtn->setContentSize(CCSizeMake(bgBtn->getContentSize().width + useConsume->getContentSize().width, bgBtn->getContentSize().height));
	}
	

	
	this->addChild(bgBtn, -5);
	this->addBtn(bgBtn);
	
	
	CCSprite *headBg1 = CCSprite::createWithSpriteFrameName("card_bj.png");
	this->addChild(headBg1);
	headBg1->setPosition(ccpAdd(consumeIcon->getPosition(), ccp(0, 0)));
	
	//名称黑色背景
	CCSprite *blackBg = CCSprite::createWithSpriteFrameName("name_bg.png");
	this->addChild(blackBg, 2);
	blackBg->setPosition(ccpAdd(ccp(consumeIcon->getPositionX() + consumeIcon->getContentSize().width * 1.5,  20), ccp(0, 0)));
	
	//显示卡牌的数量

	cardNum = SGCCLabelTTF::create(CCString::createWithFormat(str_shuliang, m_card->getCurrNum())->getCString(), FONT_BOXINFO, 23);
	this->addChild(cardNum, 10);
	cardNum->setPosition(ccpAdd(blackBg->getPosition(), ccp(blackBg->getContentSize().width,
															blackBg->getPositionY() - blackBg->getContentSize().height * 1.5)));
	
		
	//设置碎片名称
	consumeName = SGCCLabelTTF::create(m_card->getOfficerName()->getCString(), FONT_BOXINFO, 23);
	consumeName->setAnchorPoint(ccp(0,0.5));
	consumeName->setPosition(ccp(blackBg->getPositionX() - blackBg->getContentSize().width / 2.2, blackBg->getPositionY()));
	this->addChild(consumeName, 10);

	//需求更改，有星星的地方 要=3颗星星，不需要黑色的。2014-10-15
	//设置星级的星星
    int starCount = 3;
	//for (int i = 0; i < m_card->getCurrStar(); i++)
    for (int i = 0; i < starCount; i++)
	{
		CCSprite *star = CCSprite::createWithSpriteFrameName("xx.png");
		this->addChild(star, 1, i);
		star->setPosition(ccpAdd(consumeIcon->getPosition(), ccp(consumeIcon->getContentSize().width / 1.2 + star->getContentSize().width * i,
																 blackBg->getPositionY() - blackBg->getContentSize().height)));
	}
	
	//头像的背景,一共三层,防止半透影响美观........
	CCSprite *headBg = CCSprite::createWithSpriteFrameName("card_bj.png");
	this->addChild(headBg, 3);
	headBg->setPosition(ccpAdd(consumeIcon->getPosition(), ccp(0, 0)));
	CCSprite *headBg2 = CCSprite::createWithSpriteFrameName("card_bj.png");
	this->addChild(headBg2, 4);
	headBg2->setPosition(ccpAdd(consumeIcon->getPosition(), ccp(0, 0)));
	
}

void SGConsumableCell::updateCell(SGConsumableCard *card)
{
	m_card = card;
	cardNum->setString(CCString::createWithFormat(str_shuliang, m_card->getCurrNum())->getCString());
	consumeName->setString(card->getOfficerName()->getCString());
	
	//消耗品的名称
	char iconName[256] = "\0";
	//获取对应的iconId
	int iconId = card->getHeadSuffixNum();
	//转换出对应的头像名称
	sprintf(iconName, "head%d.png", iconId);
	
	if (iconId == -1)
	{
		strcpy(iconName, "mall_gift_pack.png");
	}
	else
	{
		SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_consumablelayer);
	}
	
	consumeIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(iconName)->displayFrame());
	//星级
	int starLvl = m_card->getCurrStar();
	if (starLvl > 1)//一星与二星用同一个相同的品质框
		starLvl -= 1;
	sprintf(iconName, "equipPinzhi_%d_0.png", starLvl);
	
	border->setDisplayFrame(CCSprite::createWithSpriteFrameName(iconName)->displayFrame());
	
    //需求更改，有星星的地方 要=3颗星星，不需要黑色的。
	//设置星级的星
    int starCount = 3;
	//for (int i = 0; i < card->getCurrStar(); i++)
    for (int i = 0; i < starCount; i++)
	{
		this->removeChildByTag(i);
	}
	//for (int i = 0; i < card->getCurrStar(); i++)
    for (int i = 0; i < starCount; i++)
	{
		CCSprite *star = CCSprite::createWithSpriteFrameName("xx.png");
		this->addChild(star, 1, i);
		star->setPosition(ccpAdd(consumeIcon->getPosition(), ccp(consumeIcon->getContentSize().width / 1.2 + star->getContentSize().width * i,
																- 20)));
	}
	bool isCanUse = this->isCanUseConsumable(card);
	
	if (menu && useConsume)
	{
		menu->removeChildByTag(BUTTON_TAG);
		this->removeChildByTag(MENU_TAG, true);
		menu = NULL;
		useConsume = NULL;
	}
	
	useConsume = SGButton::createFromLocal("store_exchangebtnbg.png", str_use, this, menu_selector(SGConsumableCell::consumeUseHandler),
										   CCPointZero, FONT_PANGWA, ccc3(0xd9, 0xac, 0x29), 28);
	useConsume->setPosition(ccp(bgSize.width/2 - useConsume->getContentSize().width/1.5, 0));//暂时设置，按需要调整

	if (card->getCanUseDriving() == 1 && isCanUse)
	{
		//加入sgbutton至menu中
		menu = CCMenu::create();
		this->addChild(menu);
		menu->setTag(MENU_TAG);
		menu->setPosition(CCPointZero);
		menu->addChild(useConsume, 1);
	}//是否满足使用条件,如果不满足使用条件就蒙灰
	else if (card->getCanUseDriving() && !isCanUse)
	{
		menu = CCMenu::create();
		this->addChild(menu);
		menu->setTag(MENU_TAG);
		menu->setPosition(CCPointZero);
		useConsume->setImage("barrack_anniu_hui.png");
		useConsume->setFontColor(ccc3(80, 80, 80));
		useConsume->setCallBack(this, menu_selector(SGConsumableCell::consumeUnuseHandler));
//		//加入sgbutton至menu中
		menu->addChild(useConsume, 1);
	}
	else//不能主动使用，如钥匙
	{
		bgBtn->setPosition(ccp(0, 0));
		bgBtn->setContentSize(CCSizeMake(440 + useConsume->getContentSize().width, bgBtn->getContentSize().height));
		
		CCLOG("==================bgBtn [%f, %f]======================", bgBtn->getContentSize().width, bgBtn->getContentSize().height);
	}
	
	
	
}

void SGConsumableCell::consumeUnuseHandler()
{
	
	CCString *useCondition = m_card->getLimitAndNum();
	CCArray *limitArray = CCArray::create();
	
	//如果有条件
	if (useCondition)
	{
		limitArray = split(useCondition->getCString(), ",");
	}
	
	if (limitArray->data->arr)
	{
		std::string tips(str_need);
		//拆分每个条件
		for (int i = 0; i < limitArray->count(); i++)
		{
			CCString *con = (CCString *)limitArray->objectAtIndex(i);
			std::string str(con->getCString());
			std::string idStr;//需要物品的ID
			std::string numStr;//需要物品的数量
			int index = -1;
			if ((index = str.find(";")) != std::string::npos)
			{
				idStr = str.substr(0, index);
				numStr = str.substr(index + 1);
			}
			int id = CCString::create(idStr)->intValue();
			//int num = CCString::create(numStr)->intValue();
			
			SGConsumableDataModel *consumeData = SGStaticDataManager::shareStatic()->getConsumableById(id);
			std::string name(consumeData->getConsumeName()->getCString());
			name.append("X");
			tips.append(name);
			tips.append(numStr);
			if (i != limitArray->count() - 1)//最后一个不加逗号
				tips.append(",");
		}
		SG_SHOW_WINDOW(tips.c_str());
	}
	else
	{
		SG_SHOW_WINDOW(str_prop_not_enough);
	}
	
	
}

//单击对应的item弹出对应的下拉菜单
void SGConsumableCell::showConsumableInfo()
{
	if (consumeDel)
	{
		consumeDel->showConsumeInfoHandler(this);
	}
}