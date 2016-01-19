//
//  SGPiecesCell.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#include "SGPiecesCell.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"


//cell上的头像tag
#define HEAD_TAG            10
//cell上的品质tag
#define PINZHI_TAG          11

SGPiecesCell::SGPiecesCell():
totalAndCurProgress(NULL),
//commPieces(NULL),
piecesName(NULL),
delegate(NULL),
//menu(NULL),
info(NULL),
//isUseCommon(false),
//isable(false),
m_tag(sg_LayerNull)
{
	//isUseCommon = false;
	info = NULL;
	totalAndCurProgress = NULL;//碎片数量总进度与当前进度
	//commPieces = NULL;// 通用碎片数量
	piecesName = NULL;//碎片名称
}
SGPiecesCell::~SGPiecesCell()
{
	info->release();
	info = NULL;

	//ResourceManager::sharedInstance()->unBindLayerTexture(sg_piecesLayer);
}

void SGPiecesCell::onEnter()
{
	CCLayer::onEnter();
	//menu->setTouchPriority(10);
}

//创建碎片的cell
SGPiecesCell *SGPiecesCell::create(SGPiecesDelegate *_delegate, SGPiecesCard *card, bool useCommon, LayerTag tag)
{
	SGPiecesCell *piecesCell = new SGPiecesCell();
	if (piecesCell && piecesCell->init())
	{
		piecesCell->initData(card);
        piecesCell->m_tag = tag;
		//piecesCell->isUseCommon = useCommon;
		piecesCell->viewDidLoad();
		piecesCell->delegate = _delegate;
		piecesCell->autorelease();
		return piecesCell;
	}
	CC_SAFE_DELETE(piecesCell);
	return piecesCell;
}

void SGPiecesCell::initData(SGPiecesCard *card)
{
	info = card;
	info->retain();
}

void SGPiecesCell::viewDidLoad()
{
	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, m_tag, LIM_PNG_AS_PNG);

	
	char officerName[256] = "\0";
	int shift = 0;// 名称黑色背景右向偏移
	
	int iconId = -1;
	if (info->getItemType() == 0)//武将
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(info->getCallCardId());
		iconId = temp->getIconId();
	}
	else if (info->getItemType() == 1)//装备
	{
		SGEquipmentDataModel *temp =SGStaticDataManager::shareStatic()->getEquipById(info->getCallCardId());
		iconId = temp->getIconId();
	}
	
	SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_piecesLayer);
	sprintf(officerName, "head%d.png", iconId);
	
	shift = 10;
	
	//设置cell上的头像
	CCSprite *piecesHead = CCSprite::createWithSpriteFrameName(officerName);
    piecesHead->setTag(HEAD_TAG);
	piecesHead->setPosition(ccp(-bgSize.width/2+ZoomInX(20), 2));
	this->addChild(piecesHead);
	
	//品质框
    SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(info->getItemId());
    int starlvl = pieces->getStarLevel();
    if (starlvl > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
    {
        starlvl -= 1;
    }
    
	char pinzhi[256] = "\0";
	
	if (info->getItemType() == 0)
	{
		sprintf(pinzhi, "officerPinzhi_%d_0.png",starlvl);
	}
	else if (info->getItemType() == 1)
	{
		sprintf(pinzhi, "equipPinzhi_%d_0.png", starlvl);
	}
	
	CCSprite *pz = CCSprite::createWithSpriteFrameName(pinzhi);
    pz->setTag(PINZHI_TAG);
	this->addChild(pz);
	pz->setPosition(piecesHead->getPosition());
	
	
	//头像蒙版
	CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
	this->addChild(headMask);
	headMask->setPosition(piecesHead->getPosition());
	
	int piecesCommonNum = 0;
	CCArray *piecesArray = SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards();
	if (piecesArray->data && piecesArray->data->arr && piecesArray->count())
	{
		piecesCommonNum = ((SGPiecesCard *)piecesArray->objectAtIndex(0))->getPiecesCurNum();//能使用的通用碎片
	}
	
    CCSprite * bg = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    this->addChild(bg,-5);
	//名称黑色背景
	CCSprite *namebg = CCSprite::createWithSpriteFrameName("name_bg.png");
	namebg->setPosition(ccp(piecesHead->getPositionX() + piecesHead->getContentSize().width * 1.35 + shift,  20));
    this->addChild(namebg, 0);
	
	//设置碎片名称
	piecesName = SGCCLabelTTF::create(info->getPiecesName()->getCString(), FONT_BOXINFO, 25);
	piecesName->setAnchorPoint(ccp(0,0.5));
	piecesName->setPosition(ccp(headMask->getPositionX() + headMask->getContentSize().width / 2 + 10, namebg->getPositionY()));
	this->addChild(piecesName, 1);
	
	totalAndCurProgress = SGCCLabelTTF::create(CCString::createWithFormat(str_PiecesCell_str4, info->getPiecesCurNum())->getCString(), FONT_BOXINFO, 23);
    totalAndCurProgress->setAnchorPoint(ccp(0, 0.5));
//	totalAndCurProgress->setPosition(ccp(15 + prog->getPosition().x + prog->getContentSize().width, prog->getPosition().y));
    totalAndCurProgress->setPosition(ccp( piecesName->getPosition().x , piecesName->getPositionY() - 25 ));
	this->addChild(totalAndCurProgress);
//	if (info->getPiecesCurNum() >= info->getSummonNeed())//拥有的碎片数能召唤
//	{
//		totalAndCurProgress->setInsideColor(COLOR_GREEN);
//	}
//	else
//	{
//		totalAndCurProgress->setInsideColor(ccWHITE);
//	}
}

//滑动时更新cell
void SGPiecesCell::updatePiecesCard(SGPiecesCard *card)
{
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_piecesLayer);
    
    char officerName[256] = "\0";
	int shift = 0;// 名称黑色背景右向偏移
	
	int iconId = -1;
	if (card->getItemType() == 0)//武将
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(card->getCallCardId());
		iconId = temp->getIconId();
	}
	else if (card->getItemType() == 1)//装备
	{
		SGEquipmentDataModel *temp =SGStaticDataManager::shareStatic()->getEquipById(card->getCallCardId());
		iconId = temp->getIconId();
	}
	
	SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_piecesLayer);
	sprintf(officerName, "head%d.png", iconId);
	
	shift = 10;
	
	//滑动过后重置cell上的头像
	CCSprite *piecesHeadTemp = CCSprite::createWithSpriteFrameName(officerName);
    static_cast<CCSprite *>(this->getChildByTag(HEAD_TAG))->setDisplayFrame(piecesHeadTemp->displayFrame());
    
	
	//滑动过后的品质框
	char pinzhi[256] = "\0";
	
	if (card->getItemType() == 0)
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(card->getCallCardId());
		int starlvl = temp->getOfficerCurrStarLevel();
		if (starlvl>1)
        {
			starlvl-=1;
		}
		sprintf(pinzhi, "officerPinzhi_%d_0.png",starlvl);
	}
	else if (card->getItemType() == 1)
	{
		SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(info->getCallCardId());
		sprintf(pinzhi, "equipPinzhi_%d_0.png", equipModel->getEquipStarPinzhi());
	}
	
    //更新的时候重围品质框
	CCSprite *pzTemp = CCSprite::createWithSpriteFrameName(pinzhi);
    static_cast<CCSprite *>(this->getChildByTag(PINZHI_TAG))->setDisplayFrame(pzTemp->displayFrame());
	
    //头像蒙版不需要更新
	
    
	int addCommonPieces = 0;//加到当前碎片数上，值由isUseCommon来决定是0，还是可使用的通用碎片数
	int piecesCommonNum = 0;
	CCArray *piecesArray = SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards();
	if (piecesArray->data && piecesArray->data->arr && piecesArray->count())
	{
		piecesCommonNum = ((SGPiecesCard *)piecesArray->objectAtIndex(0))->getPiecesCurNum();//能使用的通用碎片
	}
	
	//重置碎片名称
    piecesName->setString(card->getPiecesName()->getCString());
    
    
	//更新显示进度
    totalAndCurProgress->setString(CCString::createWithFormat(str_PiecesCell_str4, card->getPiecesCurNum())->getCString());
    
//	if (card->getPiecesCurNum() >= card->getSummonNeed())//拥有的碎片数能召唤
//	{
//		totalAndCurProgress->setInsideColor(COLOR_GREEN);
//	}
//	else
//	{
//		totalAndCurProgress->setInsideColor(ccWHITE);
//	}
}

void SGPiecesCell::showinfo()
{
    if (delegate)
    {
        delegate->showPiecesinfo(this);
    }


}