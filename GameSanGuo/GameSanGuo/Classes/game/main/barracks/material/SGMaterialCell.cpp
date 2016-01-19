//
//  SGMaterialCell.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#include "SGMaterialCell.h"
#include "SGShowString.h"


SGMaterialCell::SGMaterialCell():
materialBorder(NULL),
materialName(NULL),
materialNum(NULL),
materialIcon(NULL),
materialCard(NULL)
{
    
}
SGMaterialCell::~SGMaterialCell()
{
    
}

SGMaterialCell *SGMaterialCell::create(SGMaterialCard *card)
{
    SGMaterialCell *materialCell = new SGMaterialCell();
    if (materialCell && materialCell->init())
    {
        materialCell->materialCard = card;
        materialCell->initView();
        materialCell->autorelease();
        
        return materialCell;
    }
    CC_SAFE_RELEASE(materialCell);
    return NULL;
}

void SGMaterialCell::initView()
{
    
    //背景
    bgbgbg = CCSprite::createWithSpriteFrameName("item_bigbg.png");
    this->addChild(bgbgbg);
    bgbgbg->setPosition(ccpAdd(ccp(0, 0), ccp(0, 0)));
    
    //材料的名称
	char iconName[256] = "\0";
	//获取对应的iconId
	int iconId = materialCard->getHeadSuffixNum();
	//转换出对应的头像名称
	sprintf(iconName, "head%d.png", iconId);
    SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_materialLayer);
	
	materialIcon = CCSprite::createWithSpriteFrameName(iconName);
	materialIcon->setPosition(ccpAdd(ccp(-bgSize.width/2+ZoomInX(20), 2), ccp(0, 0)));
	this->addChild(materialIcon, 10);
    
    //icon的背景
    CCSprite *headBg1 = CCSprite::createWithSpriteFrameName("card_bj.png");
	this->addChild(headBg1, 9);
	headBg1->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, 0)));
    
    //边框
    char border[32] = "\0";
    int borderColor = materialCard->getMaterialBorderColor();
    if (borderColor > 1)
    {
        borderColor -= 1;
    }
    sprintf(border, "equipPinzhi_%d_0.png", borderColor);
    materialBorder = CCSprite::createWithSpriteFrameName(border);
    this->addChild(materialBorder, 1);
    materialBorder->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, 0)));
    
    //名称黑色背景
	CCSprite *blackBg = CCSprite::createWithSpriteFrameName("name_bg.png");
	this->addChild(blackBg, 2);
	blackBg->setPosition(ccpAdd(ccp(materialIcon->getPositionX() + materialIcon->getContentSize().width * 1.5,  20), ccp(0, 0)));

    //材料的名称
    materialName = SGCCLabelTTF::create(materialCard->getOfficerName()->getCString(), FONT_BOXINFO, 23);
    materialName->setAnchorPoint(ccp(0, 0.5));
    this->addChild(materialName, 3);
	materialName->setPosition(ccp(blackBg->getPositionX() - blackBg->getContentSize().width / 2.2, blackBg->getPositionY()));
    
    //数字，材料的数量
    materialNum = SGCCLabelTTF::create(CCString::createWithFormat(str_shuliang, materialCard->getMaterialCurrNum())->getCString(), FONT_BOXINFO, 23);
    materialNum->setAnchorPoint(ccp(0, 0.5));
    this->addChild(materialNum, 2);
    materialNum->setPosition(ccpAdd(materialName->getPosition(), ccp(0, -blackBg->getContentSize().height / 2 -
                                                                materialNum->getContentSize().height / 2 - 10)));
}

//向下移动，更新cell
void SGMaterialCell::updateCell(SGMaterialCard *card)
{
    
    materialCard = card;
    //数量更新
    materialNum->setString(CCString::createWithFormat(str_shuliang, materialCard->getMaterialCurrNum())->getCString());
    
    //icon
    //材料的名称
	char iconName[256] = "\0";
	//获取对应的iconId
	int iconId = materialCard->getHeadSuffixNum();
	//转换出对应的头像名称
	sprintf(iconName, "head%d.png", iconId);
    SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_materialLayer);
    materialIcon->setDisplayFrame(CCSprite::createWithSpriteFrameName(iconName)->displayFrame());
    
    //边框
    char border[32] = "\0";
    int colorIndex = materialCard->getMaterialBorderColor()>0 ? (materialCard->getMaterialBorderColor()-1):0;
    sprintf(border, "equipPinzhi_%d_0.png", colorIndex);
    materialBorder->setDisplayFrame(CCSprite::createWithSpriteFrameName(border)->displayFrame());
    
    //材料名称
    materialName->setString(materialCard->getOfficerName()->getCString());
}



























