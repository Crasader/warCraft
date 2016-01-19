//
//  SGActivityDetailCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-3.
//
//

#include "SGActivityDetailCell.h"
#include "GameConfig.h"
#include "ResourceManager.h"

//waring 将bindTexture和unbind转移到创建cell的layer。
SGActivityDetailCell::SGActivityDetailCell() : spriteIsPass(NULL), spriteActName(NULL), labelDesc(NULL), _isPassLevel(false)
{
    ;
}

SGActivityDetailCell::~SGActivityDetailCell()
{
    ;
}

SGActivityDetailCell* SGActivityDetailCell::create()
{
    SGActivityDetailCell *actionCell = new SGActivityDetailCell();
    if(actionCell && actionCell->init())
    {
        actionCell->viewDidLoad();
        actionCell->autorelease();
        return actionCell;
    }
    
    CC_SAFE_DELETE(actionCell);
    return actionCell;
    
}

void SGActivityDetailCell::viewDidLoad()
{
    spriteIsPass = CCSprite::createWithSpriteFrameName("new.png");
    spriteActName = CCSprite::create();
    labelDesc = CCLabelTTF::create("", FONT_XINGKAI, FONTSIZE(12));
    
    spriteIsPass->setAnchorPoint(ccp(0, 0.5f));
    spriteIsPass->setPosition(ccp(-bgSize.width/2+ZoomInX(10), bgSize.height/2-ZoomInX(10)));
    bgbgbg = NULL;
    this->addChild(spriteIsPass);
    this->addChild(spriteActName);
    this->addChild(labelDesc);
}

void SGActivityDetailCell::setData(int index)
{

    CCString *sName = CCString::createWithFormat("mainLayer_word%d_everydayThing.png", 3);
    spriteActName->setDisplayFrame(CCSprite::createWithSpriteFrameName(sName->getCString())->displayFrame());
    
    labelDesc->setString("new description!!!");
    
    if(_isPassLevel)
    {
        spriteIsPass->setDisplayFrame(CCSprite::createWithSpriteFrameName("gg.png")->displayFrame());
    }else{
        spriteIsPass->setDisplayFrame(CCSprite::createWithSpriteFrameName("new.png")->displayFrame());
    }
}
