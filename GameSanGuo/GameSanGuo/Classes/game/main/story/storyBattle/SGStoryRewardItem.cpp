//
//  SGStoryRewardItem.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#include "SGStoryRewardItem.h"
#include "SGPlayerInfo.h"
#include "SGLayout.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"

/***********SGStoryRewardItem***********/

SGStoryRewardItem::SGStoryRewardItem(void) : m_nCoin(0), soulNum(0)
{
    _itemId = 0;
    _itemLev = 0;
    _itemType = 0;
    m_tag = sg_LayerNull;

}
SGStoryRewardItem::~SGStoryRewardItem(void)
{
	//ResourceManager::sharedInstance()->unBindLayerTexture(sg_storyLayer);
}

SGStoryRewardItem* SGStoryRewardItem::create(int soulNum, int coin, LayerTag tag)
{
    SGStoryRewardItem *item = new SGStoryRewardItem();
    item->soulNum = soulNum;
    item->m_nCoin = coin;
    item->m_tag = tag;
    if (item && item->init())
    {
        item->initView();
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

void SGStoryRewardItem::initView()
{
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_storyLayer);
    //直接使用head9999.png
    SGMainManager::shareMain() -> addHeadIconPlistByNum(9999,sg_storyLayer);
    
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, m_tag, LIM_PNG_AS_PNG);
	
    bgbgbg = NULL;
}
 void SGStoryRewardItem::setDataWithArray(CCArray * array,int ntype,int ncount)
{
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, m_tag, LIM_PNG_AS_PNG);
    this->removeAllChildren();
    if(ntype>1)
    {
        SGCCLabelTTF * labelCount  = SGCCLabelTTF::create(CCString::createWithFormat(str_OverBattleBox_str1014, ncount)->getCString(), FONT_PANGWA, 22);
        this->addChild(labelCount,100);
        labelCount->setPosition(ccp(100,80));
    }
    int sum = array->count();
    if(sum==0)
    {
    this->setContentSize(CCSize(ccp(100,340)));}
    for (int i = 0; i< sum; i++) {
        SGStoryRdItem *model = (SGStoryRdItem *)array->objectAtIndex(i);
        this->setData(model->getItemId(), model->getItemTag(), model->getItemType(), model->m_nCount,i);

        
    }
    
    
}
void SGStoryRewardItem::setData(int iId, int iTag, int iType, int num,int index)
{
    if(num == 0)
        return ;
    CCSprite *sprItemCount = NULL;
    CCSprite *spriteIcon = NULL;
    CCSprite *spriteIconBg = NULL;
    CCSprite *blackBg = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(blackBg);
    spriteIcon = CCSprite::createWithSpriteFrameName("head1004.png");
    this->addChild(spriteIcon);
    
    spriteIconBg = CCSprite::createWithSpriteFrameName("jinkuang.png");
    this->addChild(spriteIconBg);
    CCLog("data---------------%d, %d, %d", iId, iTag, iType);
    CCString *spName = NULL;
    CCString *bgName = NULL;
    CCString *advancenumber = NULL;
	bool isPieces = false;
    
    
	
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, m_tag);
	ResourceManager::sharedInstance()->bindTexture("sgshoujingzhou/sgshoujingzhou.plist", RES_TYPE_LAYER_UI, m_tag);

    // 0 武将 1 装备 2 士兵 3 道具
    spName = CCString::create("");
	
 
	//12.06
    if (iType == 0) {
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(iId);
		SGMainManager::shareMain() -> addHeadIconPlistByNum(temp->getIconId(),sg_storyLayer);
		spName = CCString::createWithFormat("head%d.png", temp->getIconId());
        int  star=temp->getOfficerCurrStarLevel();
        if ( star>1) {
            star=star-1;
        }
        bgName = CCString::createWithFormat("officerPinzhi_%d_0.png", star);
        if (temp->getAdvNum()>0) {
            advancenumber = CCString::createWithFormat("+%d",temp->getAdvNum());
        }
    }else if (iType == 1) {
        //int index = iTag - 9;
        //if (index < 0 || index > 3)  index = 0;
		SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(iId);
		SGMainManager::shareMain() -> addHeadIconPlistByNum(temp->getIconId(),sg_storyLayer);
		spName = CCString::createWithFormat("head%d.png", temp->getIconId());
        if (temp->getAdvNum()>0) {
            advancenumber = CCString::createWithFormat("+%d",temp->getAdvNum());
        }
        bgName = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
    }else if (iType == 2) { // PVE不会掉落士兵卡
        spName = CCString::createWithFormat("%d-%d.png", iId, 3);
        int contry = SGPlayerInfo::sharePlayerInfo()->getPlayerCountry();
        bgName = CCString::createWithFormat("country_%d.png", contry);
    }else if (iType == 3) {
		SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(iId);
		SGMainManager::shareMain() -> addHeadIconPlistByNum(temp->getIconId(),sg_storyLayer);
		spName = CCString::createWithFormat("head%d.png", temp->getIconId());
		
		
        int starlvl = temp->getPropsCurrStarLevel();
        if (starlvl>1 && temp->getPropsCountry() > 0) {
            starlvl-=1;
            
            bgName = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
        }
        else
        {
			if (temp->getPropsPropsType() == 3) //如果是兵魂卡
			{
				int starLvl = temp->getPropsCurrStarLevel();
				if (starLvl > 1)
					starLvl -= 1;
				bgName = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl);
			}
			else
			{
				bgName = CCString::create("jinkuang.png");
			}
        }
 
    }
    
    else if (iType == 5)//碎片
	{
		SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(iId);
        int starLv = temp->getStarLevel();
        if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
        {
            starLv -= 1;
        }
        
		if (temp->getCallingCardId() == -1)// 通用碎片//common_pieces_border
		{
			
			spName = CCString::createWithFormat("common_pieces_border.png");
		}
		else if (temp->getPiecesType() == 0)//武将碎片
		{
			isPieces = true;
			
			SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(temp->getCallingCardId());
			SGMainManager::shareMain() -> addHeadIconPlistByNum(of->getIconId(),sg_storyLayer);
			spName = CCString::createWithFormat("head%d.png", of->getIconId());
            bgName = CCString::createWithFormat("officerPinzhi_%d_0.png",starLv);
			
		}
		else if (temp->getPiecesType() == 1)//装备碎片
		{
			isPieces = true;
			SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(temp->getCallingCardId());
			SGMainManager::shareMain() -> addHeadIconPlistByNum(eq->getIconId(),sg_storyLayer);
			spName = CCString::createWithFormat("head%d.png", eq->getIconId());

			bgName = CCString::createWithFormat("equipPinzhi_%d_0.png",starLv);
		}
	}else if (iType == 6)//消耗品
	{
		SGConsumableDataModel *con = SGStaticDataManager::shareStatic()->getConsumableById(iId);
		SGMainManager::shareMain()->addHeadIconPlistByNum(con->getIconId(), sg_storyLayer);
		spName = CCString::createWithFormat("head%d.png", con->getIconId());
		int currStar = con->getConsumeStarLvl();
		if (currStar > 1)
			currStar -= 1;
		bgName = CCString::createWithFormat("equipPinzhi_%d_0.png", currStar);
		
	}
    else if (iType == 7)//材料
    {
        SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(iId);
		SGMainManager::shareMain()->addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_storyLayer);
		spName = CCString::createWithFormat("head%d.png", materialModel->getMaterialIconId());
		int currStar = materialModel->getMaterialBorderColor();
        
		if (currStar > 1)
        {
			currStar -= 1;
        }
		bgName = CCString::createWithFormat("equipPinzhi_%d_0.png", currStar);
    }
	else if (iType == 11)//军魂
	{
		spName = CCString::create("soul_icon.png");
		bgName = CCString::create("equipPinzhi_3_1.png");
	}
    else if (iType == 12)//tongqian
	{
		spName = CCString::create("head9999.png");
		bgName = CCString::create("equipPinzhi_3_1.png");
	}
    else if (iType == 18)//勾玉
    {
        spName = CCString::create("big_gouyu.png");
        bgName = CCString::create("equipPinzhi_3_0.png");
    }
	
	
    
    spriteIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(spName->getCString())->displayFrame());
	//如果为碎片，加上蒙版
		
    if (bgName) {
        spriteIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(bgName->getCString())->displayFrame());
        if (iType == 0)
        {
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(iId);
            spriteIconBg->removeChildByTag(30);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spriteIconBg->getContentSize().width*0.35,spriteIconBg->getContentSize().height - countryName->getContentSize().height));
            spriteIconBg->addChild(countryName,30,30);
        }
        else if (iType == 3)
        {
            SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(iId);
            
            if (temp->getPropsCountry() > 0 && temp->getPropsCurrStarLevel() > 0) {
                if (spriteIcon)
                {
                    spriteIconBg->removeChildByTag(30, true);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(spriteIconBg->getContentSize().width*0.35,spriteIconBg->getContentSize().height
												 - countryName->getContentSize().height));
                    spriteIconBg->addChild(countryName,30,30);
                }
            }
        }
        
    }
    
    if (advancenumber) {
        spriteIconBg->removeChildByTag(250);
        SGCCLabelTTF *ad = SGCCLabelTTF::create(advancenumber->getCString(), FONT_PANGWA, 24);
        ad->setInsideColor(ccGREEN);
        ad->setAnchorPoint(ccp(1,1));
        ad->ignoreAnchorPointForPosition(false);
        ad->setPosition(ccp(spriteIconBg->getContentSize().width*0.8,spriteIconBg->getContentSize().height - ad->getContentSize().height));
        spriteIconBg->addChild(ad,250,250);
        
    }
    
    
       int y= 0;
    if(index>4)
        y = 1;
    else if(index>9)
        y = 2;
    if(index>=5)
        index = index%5;
    if(y==0){
     this->setContentSize(CCSize(ccp(spriteIconBg->getContentSize().width*5,40+spriteIconBg->getContentSize().height*(y+1))));
    }else{
    this->setContentSize(CCSize(ccp(spriteIconBg->getContentSize().width*5,40+spriteIconBg->getContentSize().height*(y+1))));
    }
    spriteIconBg->setPosition(ccp(spriteIconBg->getContentSize().width*index*.8+70,-spriteIconBg->getContentSize().height*1*y));
    spriteIcon->setPosition(spriteIconBg->getPosition());
    blackBg->setPosition(spriteIconBg->getPosition());
    if (isPieces)
	{
		CCSprite *piecesMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
		piecesMask->setPosition(ccp(spriteIcon->getPositionX(), spriteIcon->getPositionY()));
		this->addChild(piecesMask, 90);
	}
    if(iType == 11)
    {
        SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create(str_Currency_JunHun, FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
    }
     if (iType == 12)//tongqian
	{
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create(str_Currency_TongQian, FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
		
	}
    if (iType == 0)//武将
	{
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(iId);
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create( temp->getOfficerName()->getCString(), FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
		
	}
    if (iType == 1)//装备
	{
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(iId);
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create( temp->getEquipName()->getCString(), FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
        
	}
    if (iType == 2)//士兵
	{
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(iId);
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create( temp->getEquipName()->getCString(), FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
        
	}
    if (iType == 3)
	{
       SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(iId);
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create( temp->getPropsName()->getCString(), FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
        
	}
    if (iType == 5)
	{
        SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(iId);
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount,1000);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create( temp->getPiecesName()->getCString(), FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
        
	}
    if (iType == 6)
	{
        SGConsumableDataModel *con = SGStaticDataManager::shareStatic()->getConsumableById(iId);
		SGCCLabelTTF *soulCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(), FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
		//#29FF1B
        //modify by:zyc. merge into create.
		//soulCount->setColor(ccc3(0x29, 0xff, 0x1b));
		this->addChild(soulCount);
		soulCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2 - sprItemCount->getContentSize().height / 2 - 3)));
		SGCCLabelTTF *soulName = SGCCLabelTTF::create( con->getConsumeName()->getCString(), FONT_PANGWA, 22);
		soulName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(soulName);
        
	}
    //材料
    if (iType == 7)
    {
        SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(iId);
		SGCCLabelTTF *materialCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(),
                                                       FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));

		this->addChild(materialCount);
		materialCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2
                                                                        - sprItemCount->getContentSize().height / 2 - 3)));
        
		SGCCLabelTTF *materialName = SGCCLabelTTF::create(materialModel->getMaterialName()->getCString(), FONT_PANGWA, 22);
		materialName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(materialName);

    }
    if (iType == 18)
    {
		SGCCLabelTTF *count = SGCCLabelTTF::create(CCString::createWithFormat("X %d", num)->getCString(),
                                                           FONT_PANGWA, 22 , ccc3(0x29, 0xff, 0x1b));
        
		this->addChild(count);
		count->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -30)));
		
		sprItemCount = CCSprite::createWithSpriteFrameName("box_name_bg.png");
		this->addChild(sprItemCount);
		sprItemCount->setPosition(ccpAdd(spriteIcon->getPosition(), ccp(0, -spriteIcon->getContentSize().height / 2
                                                                        - sprItemCount->getContentSize().height / 2 - 3)));
        
		SGCCLabelTTF *rewardName = SGCCLabelTTF::create(str_GouYu, FONT_PANGWA, 22);
		rewardName->setPosition(ccpAdd(sprItemCount->getPosition(), ccp(0, 0)));
		this->addChild(rewardName);
        
    }
    
}


/***********SGStoryRdItem***********/

SGStoryRdItem::SGStoryRdItem(void)
{
    _itemId = 0;
    _itemTag = 0;
    _itemType = 0;
    m_nCount = 0;
}
SGStoryRdItem::~SGStoryRdItem(void)
{
}

SGStoryRdItem* SGStoryRdItem::create(int iId, int iTag, int iType)
{
    SGStoryRdItem *item = new SGStoryRdItem();
    if (item) {
        item->initWithInfo(iId, iTag, iType);
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}

void SGStoryRdItem::initWithInfo(int iId, int iTag, int iType)
{
    _itemId = iId;
    _itemTag = iTag;
    _itemType = iType;
}
