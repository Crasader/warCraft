//
//  SGPiecesCell.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#include "SGPiecesBagCell.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"


//cell上的头像tag
#define HEAD_TAG            10
//cell上的品质tag
#define PINZHI_TAG          11

SGPiecesBagCell::SGPiecesBagCell():
totalAndCurProgress(NULL),
commPieces(NULL),
piecesName(NULL),
delegate(NULL),
menu(NULL),
callBtn(NULL),
info(NULL),
isUseCommon(false),
isable(false),
m_tag(sg_LayerNull)
{
	isUseCommon = false;
	info = NULL;
	totalAndCurProgress = NULL;//碎片数量总进度与当前进度
	commPieces = NULL;// 通用碎片数量
	piecesName = NULL;//碎片名称
}
SGPiecesBagCell::~SGPiecesBagCell()
{
	info->release();
	info = NULL;

	//ResourceManager::sharedInstance()->unBindLayerTexture(sg_piecesLayer);
}

void SGPiecesBagCell::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(10);
}

//创建碎片的cell
SGPiecesBagCell *SGPiecesBagCell::create(SGPiecesBagDelegate *_delegate, SGPiecesCard *card, bool useCommon, LayerTag tag)
{
	SGPiecesBagCell *piecesCell = new SGPiecesBagCell();
	if (piecesCell && piecesCell->init())
	{
		piecesCell->initData(card);
        piecesCell->m_tag = tag;
		piecesCell->isUseCommon = useCommon;
		piecesCell->viewDidLoad();
		piecesCell->delegate = _delegate;
		piecesCell->autorelease();
		return piecesCell;
	}
	CC_SAFE_DELETE(piecesCell);
	return piecesCell;
}

void SGPiecesBagCell::initData(SGPiecesCard *card)
{
	info = card;
	info->retain();
}

void SGPiecesBagCell::viewDidLoad()
{
	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, m_tag, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	//  创建放召唤button的menu
	menu = CCMenu::create();
	this->addChild(menu);
	menu->setPosition(CCPointZero);
	

	
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
	char pinzhi[256] = "\0";
	
	if (info->getItemType() == 0)
	{
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(info->getCallCardId());
		int starlvl = temp->getOfficerCurrStarLevel();
		if (starlvl>1) {
			starlvl-=1;
		}
		sprintf(pinzhi, "officerPinzhi_%d_0.png",starlvl);
	}
	else if (info->getItemType() == 1)
	{
		SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(info->getCallCardId());
		sprintf(pinzhi, "equipPinzhi_%d_0.png", equipModel->getEquipStarPinzhi());
	}
	
	CCSprite *pz = CCSprite::createWithSpriteFrameName(pinzhi);
    pz->setTag(PINZHI_TAG);
	this->addChild(pz);
	pz->setPosition(piecesHead->getPosition());
	
	
	//头像蒙版
	CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
	this->addChild(headMask);
	headMask->setPosition(piecesHead->getPosition());
	

	int addCommonPieces = 0;//加到当前碎片数上，值由isUseCommon来决定是0，还是可使用的通用碎片数
	int piecesCommonNum = 0;
	CCArray *piecesArray = SGPlayerInfo::sharePlayerInfo()->getPiecesCommonCards();
	if (piecesArray->data && piecesArray->data->arr && piecesArray->count())
	{
		piecesCommonNum = ((SGPiecesCard *)piecesArray->objectAtIndex(0))->getPiecesCurNum();//能使用的通用碎片
	}
	
	
    isable= info->getPiecesCurNum() + addCommonPieces >= info->getSummonNeed();
    

//	//设置召唤button
////	if (info->getPiecesCurNum() + addCommonPieces >= info->getSummonNeed())//拥有的碎片数能召唤
////	{
//    callBtn = SGButton::create("zhaohuan_active_btn.png", NULL, this, menu_selector(SGPiecesBagCell::summon), ccp(0, 0), false, true);//彩色召唤显示
////	}
//    callBtn->setPosition(ccp(bgSize.width/2-callBtn->getContentSize().width/2, 0));//暂时设置，按需要调整
//	callBtn->setTag(21);
//	this->addBtn(callBtn);
//    this->addChild(callBtn,-5);


    //重设背景
	SGButton *bgBtn = SGButton::create("item_bigbg.png",
								NULL,
								this,
								menu_selector(SGPiecesBagCell::pieceClicked),
								ccp(0, 0),
								false,
								true);
	bgBtn->setAnchorPoint(ccp(0.5,0.5));
	bgBtn->ignoreAnchorPointForPosition(false);
	bgBtn->setPosition(ccp(0, 0));
	//bgBtn->setContentSize(CCSizeMake(bgBtn->getContentSize().width- callBtn->getContentSize().width*1.2, bgBtn->getContentSize().height));

	//名称黑色背景
	CCSprite *namebg = CCSprite::createWithSpriteFrameName("name_bg.png");
	namebg->setPosition(ccp(piecesHead->getPositionX() + piecesHead->getContentSize().width * 1.35 + shift,  20));
    this->addChild(namebg, 0);
	
	//设置碎片名称
	piecesName = SGCCLabelTTF::create(info->getPiecesName()->getCString(), FONT_BOXINFO, 25);
	piecesName->setAnchorPoint(ccp(0,0.5));
	piecesName->setPosition(ccp(headMask->getPositionX() + headMask->getContentSize().width / 2 + 10, namebg->getPositionY()));
	this->addChild(piecesName, 1);
	
    //加上3颗星星
    CCSprite * star[3];
    for(int i =0 ; i<3; ++i)
    {
        star[i] = CCSprite::createWithSpriteFrameName("xx.png");
        star[i]->setPosition(ccp(piecesName->getPosition().x + i*40 + 15 , piecesName->getPosition().y - 30 ));
        this->addChild(star[i]);
    }
    
    //进度条背景
    m_pieceRateBg = CCScale9Sprite::createWithSpriteFrameName("jingyantiaobg.png");
    m_pieceRateBg->setPreferredSize(CCSize(250 , m_pieceRateBg->getOriginalSize().height));
//    m_pieceRateBg->setScaleX(250.0/m_pieceRateBg->getContentSize().width);
//    m_pieceRateBg->setScaleY(1.2);
    m_pieceRateBg->setAnchorPoint(ccp(0 , 0.5));
    m_pieceRateBg->setPosition(ccp( star[2]->getPositionX() + star[2]->getContentSize().width +10 , star[2]->getPositionY() ));
    this->addChild(m_pieceRateBg);
    
    
    m_pieceProgress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("pieceRate.png"));
    m_pieceProgress->setScaleX(250.0/ m_pieceProgress->getContentSize().width);
    m_pieceRateBg->setScaleY(m_pieceRateBg->getContentSize().height*m_pieceRateBg->getScaleY() / m_pieceProgress->getContentSize().height);
    m_pieceProgress->setType(kCCProgressTimerTypeBar);
    m_pieceProgress->setMidpoint(ccp(0,0));
    m_pieceProgress->setBarChangeRate(ccp(1, 0));
    m_pieceProgress->setPercentage(100.0 * info->getPiecesCurNum()/info->getSummonNeed());
    m_pieceProgress->setAnchorPoint(m_pieceRateBg->getAnchorPoint());
    m_pieceProgress->setPosition(ccp(m_pieceRateBg->getPositionX()  , m_pieceRateBg->getPositionY() ) );
    this->addChild(m_pieceProgress);
    
    //进度
	totalAndCurProgress = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d", info->getPiecesCurNum(), info->getSummonNeed())->getCString(), FONT_BOXINFO, 23);
	totalAndCurProgress->setPosition(ccp( m_pieceRateBg->getPositionX() + m_pieceRateBg->getContentSize().width*0.5 , m_pieceRateBg->getPositionY()));
	this->addChild(totalAndCurProgress);
    
    //设置召唤button
    callBtn = SGButton::create("zhaohuan_active_btn.png", NULL, this, menu_selector(SGPiecesBagCell::summon), ccp(0, 0), false, true);//彩色召
    callBtn->setPosition(ccp(m_pieceRateBg->getPositionX() + m_pieceRateBg->getContentSize().width*m_pieceRateBg->getScaleX() , m_pieceRateBg->getPositionY() + 5));//暂时设置，按需要调整
	this->addBtn(callBtn);
    this->addChild(callBtn ,100);
    
    
	if (info->getPiecesCurNum() >= info->getSummonNeed())//拥有的碎片数能召唤
	{
        m_pieceRateBg->setVisible(false);
        m_pieceProgress->setVisible(false);
        totalAndCurProgress->setVisible(false);
        
        callBtn->setVisible(true);
        callBtn->setEnabled(true);
	}
	else
	{
        m_pieceRateBg->setVisible(true);
        m_pieceProgress->setVisible(true);
        totalAndCurProgress->setVisible(true);
        
        callBtn->setVisible(false);
        callBtn->setEnabled(false);
	}
	
	//menu->addChild(callBtn);
	
	//显示通用碎片
	commPieces = SGCCLabelTTF::create(CCString::createWithFormat(str_PiecesCell_str2,
                                                               info->getCanUseCommon())->getCString(), FONT_BOXINFO, 23);
	commPieces->setAnchorPoint(ccp(0, 0.5));
	commPieces->setPosition(ccp(headMask->getPositionX() + commPieces->getContentSize().width / 3.5,
                                piecesName->getPositionY() - 38));
	this->addChild(commPieces);
    commPieces->setVisible(false);
    
    /////////////////
    this->addChild(bgBtn,-5);

    addBtn(bgBtn);
}

//滑动时更新cell
void SGPiecesBagCell::updatePiecesCard(SGPiecesCard *card)
{
    char officerName[256] = "\0";
	int shift = 0;// 名称黑色背景右向偏移
	
    if(card->getPiecesCurNum() >= card->getSummonNeed())
    {
        callBtn->setVisible(true);
        callBtn->setEnabled(true);
    }
    else
    {
        callBtn->setVisible(false);
        callBtn->setEnabled(false);
        m_pieceRateBg->setVisible(true);
        m_pieceProgress->setVisible(true);
        totalAndCurProgress->setVisible(true);
    }
    
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
	
	//重置碎片名称
    piecesName->setString(card->getPiecesName()->getCString());
    
    
	//更新显示进度
    totalAndCurProgress->setString(CCString::createWithFormat("%d/%d", card->getPiecesCurNum(), card->getSummonNeed())->getCString());
    
	if (card->getPiecesCurNum() >= card->getSummonNeed())//拥有的碎片数能召唤
	{
		totalAndCurProgress->setInsideColor(COLOR_GREEN);
	}
	else
	{
		totalAndCurProgress->setInsideColor(ccWHITE);
	}
}


void SGPiecesBagCell::summon()
{
    
    if(isable)
    {
    	if (delegate)
        {
            delegate->summonPieces(this);
        }
        CCLOG("Cell Summonning......");
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_PiecesCell_str3);
    }
    

}

void SGPiecesBagCell::pieceClicked()
{
    if (delegate)
    {
        delegate->pieceClickCallback(this);
    }
}