//
//  SGPvpMainCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#include "SGPvpMainCell.h"
#include "SGLayout.h"
#include "GameConfig.h"
#include "ResourceManager.h"

#include "cocos-ext.h"
#include "SGStringConfig.h"
USING_NS_CC_EXT;

SGPvpMainCell::SGPvpMainCell(void)
:spriteBg(NULL),
spriteLock(NULL),
spriteTitle(NULL),
lableDesc(NULL),
lableTitle(NULL),
tableview(NULL),
_isOpen(NULL),
_index(0),
delegate(NULL),
menu(NULL)
{

}
SGPvpMainCell::~SGPvpMainCell(void)
{
 
//    ResourceManager::sharedInstance()->unBindLayerTexture(sg_pvpMainLayer);
}

SGPvpMainCell* SGPvpMainCell::create(SGPvpMainCellDelegate* del,SNSTableView* table)
{
    SGPvpMainCell *baseCell = new SGPvpMainCell();
    if (baseCell && baseCell->init()) {
        baseCell->tableview=table;
        baseCell->delegate=del;
        baseCell->viewDidLoad();
        baseCell->autorelease();
        return baseCell;
    }
    CC_SAFE_DELETE(baseCell);
    return NULL;
}

void SGPvpMainCell::viewDidLoad()
{
  
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_pvpMainLayer);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_pvpMainLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_pvpMainLayer);
	
	
    spriteBg = CCSprite::createWithSpriteFrameName("server_GreenBG.png");
    this->addChild(spriteBg, -10);
    spriteBg->setVisible(false);
    
    CCScale9Sprite *spriteBorder = CCScale9Sprite::createWithSpriteFrameName("upwar_cell_bg.png");
//    spriteBorder->setPreferredSize(CCSizeMake(cellW, cellH));
//    this->addChild(spriteBorder, -10);
    bgbgbg =NULL;
    float cellW = spriteBorder->getContentSize().width;
 //////////////
     menu=CCMenu::create();
     menu -> setPosition(CCPointZero);
    SGButton*btn=SGButton::create("upwar_cell_bg.png", NULL, this, menu_selector(SGPvpMainCell::buttonClick),CCPointZero,false,true);
//    menu->addChild(btn);
//    this->addChild(menu);
    this->addChild(btn,-5);
    addBtn(btn);
//    
    
    spriteLock = CCSprite::createWithSpriteFrameName("card_suo.png");
    spriteLock->setScale(1.2f);
    spriteLock->setPosition(ccpAdd(spriteBg->getPosition(), ccp(-cellW/2+35, 0)));
    spriteLock->setVisible(false);
    this->addChild(spriteLock);
    
//    spriteTitle = CCSprite::createWithSpriteFrameName("upwar_title_mwtt.png");
//    spriteTitle->setAnchorPoint(ccp(0, 0.5f));
//    spriteTitle->setScale(1.4f);
//    spriteTitle->setPosition(ccpAdd(spriteBg->getPosition(), ccp(-cellW/2+50, 0)));
//    this->addChild(spriteTitle);
//    spriteTitle->setVisible(false);
    
    lableDesc = SGCCLabelTTF::create(str_PvpMainCell_str1, FONT_BOXINFO, 26 , ccWHITE);
    //modify by:zyc. merge into create.
    //lableDesc->setColor(ccWHITE);
    lableDesc->setAnchorPoint(ccp(1, 0.5f));
    lableDesc->setPosition(ccpAdd(spriteBg->getPosition(), ccp(cellW/2-15, -23)));
    this->addChild(lableDesc);
    
    lableTitle = SGCCLabelTTF::create(str_PvpMainCell_str1, FONT_BOXINFO, 36 , ccYELLOW);
    //modify by:zyc. merge into create.
    //lableTitle->setColor(ccYELLOW);
    lableTitle->setAnchorPoint(ccp(0, 0.5f));
    lableTitle->setPosition(ccpAdd(spriteBg->getPosition(), ccp(-cellW/2+65, 0)));
    this->addChild(lableTitle);
}

void SGPvpMainCell::setData(int index)
{
//	ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_pvpMainLayer);
    _index = index;
    if (_index == 0) {
        _isOpen = true;
        lableDesc->setString(str_PvpMainCell_str2);
    }else {
        _isOpen = false;
        spriteBg->setDisplayFrame(CCSprite::createWithSpriteFrameName("server_BlueBG.png")->displayFrame());
        lableDesc->setString(str_PvpMainCell_str3);
    }
    lableTitle->setString(str_BattleRankLayer_str1);
//    const char *sname = "upwar_title_mwtt.png";
    if (index == 1) {
        lableTitle->setString(str_PvpMainCell_str4);
//        sname = "upwar_title_mwyp.png";
    }else if(index == 2) {
        lableTitle->setString(str_PvpMainCell_str5);
//        sname = "upwar_title_mwhc.png";
    }
//    spriteTitle->setDisplayFrame(CCSprite::createWithSpriteFrameName(sname)->displayFrame());
    
    if (!_isOpen) {
        spriteLock->setVisible(true);
    }
}





void SGPvpMainCell::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);

}


//void SGPvpMainCell::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}
//void SGPvpMainCell::onEnter()
//{
//	CCLayer::onEnter();
//    
//    menu->setTouchPriority(10);//设置menu的优先级
//    
//}
//bool SGPvpMainCell::ccTouchBegan(CCTouch *touches, CCEvent *pevent)
//{
//    return true;
//}


