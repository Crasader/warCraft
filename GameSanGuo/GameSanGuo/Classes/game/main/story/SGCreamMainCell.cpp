//
//  SGCreamMainCell.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-10-28.
//
//

#include "SGCreamMainCell.h"

#include "SGLayout.h"
#include "GameConfig.h"
#include "ResourceManager.h"
#include "SGCreamBattleLayer.h"
#include "SGStringConfig.h"
SGCreamMainCell::SGCreamMainCell() : spriteIsPass(NULL), spriteRank(NULL), labelName(NULL), labelDisc_1(NULL), labelDisc_2(NULL), _conTime(0), delegate(NULL), tableview(NULL)
{
    ;
}

SGCreamMainCell::~SGCreamMainCell()
{
//	ResourceManager::sharedInstance()->unBindTexture("story/story2.plist", RES_TYPE_LAYER_UI, sg_creamLayer);
//	ResourceManager::sharedInstance()->unBindTexture("story/creamBattle.plist", RES_TYPE_LAYER_UI, sg_creamLayer);
}

SGCreamMainCell* SGCreamMainCell::create(SGCreamMainCellDelegate*del,SNSTableView*tab)
{
    SGCreamMainCell *detailCell = new SGCreamMainCell();
    if(detailCell && detailCell->init())
    {
        detailCell->delegate=del;
        detailCell->tableview=tab;
        detailCell->viewDidLoad();
        detailCell->autorelease();
        return detailCell;
    }
    
    CC_SAFE_DELETE(detailCell);
    return NULL;
    
}

void SGCreamMainCell::viewDidLoad()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("story/story2.plist");
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("story/creamBattle.plist");
	
	//ResourceManager::sharedInstance()->bindTexture("story/story2.plist", RES_TYPE_LAYER_UI, sg_creamLayer);
	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_creamLayer);

    SGButton * bigBGBtn = SGButton::create("item_bigbg.png",
                                NULL,
                                this,
                                menu_selector(SGCreamMainCell::select),
                                ccp(0, 0),
                                false,
                                true);

    
    
    spriteIsPass = CCSprite::createWithSpriteFrameName("Label_win6.png");
    spriteIsPass->setPosition(ccp(-bgSize.width/2+spriteIsPass->getContentSize().width*.5f, bgSize.height*.5f-spriteIsPass->getContentSize().height*.5f));
    spriteIsPass->setVisible(false);
	
	CCSprite *kl = CCSprite::createWithSpriteFrameName("jykl.png");
	kl->setPosition(ccp(-bgSize.width/2+kl->getContentSize().width*.5f + 50, bgSize.height*.5f-kl->getContentSize().height*.5f - 12));
    
    labelName = CCLabelTTF::create("", FONT_PANGWA, FONTSIZE(17));
    labelDisc_1 = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(12));
    labelDisc_2 = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(12));
    
    spriteRank = CCSprite::createWithSpriteFrameName("Label_win5.png");
    spriteRank->setPosition(ccp(-bgSize.width/2+spriteRank->getContentSize().height/2, 0));
    spriteRank->setVisible(false);
    
    labelName->setAnchorPoint(ccp(0, 0.5f));
    labelName->setPosition(ccp(-bgSize.width/2+ZoomInX(40) + 50, 0));
    
    labelDisc_1->setAnchorPoint(ccp(1, 0.5));
    labelDisc_2->setAnchorPoint(ccp(1, 0.5));
    labelDisc_1->setPosition(ccp(bgSize.width/2-15, 15));
    labelDisc_2->setPosition(ccp(bgSize.width/2-15, -15));
    btn=bigBGBtn;
    
    this->SNSTableViewCellItem::addBtn(btn);
    this->addChild(btn);
    
	this->addChild(kl);
    this->addChild(spriteIsPass);
    this->addChild(labelName);
    this->addChild(spriteRank);
    this->addChild(labelDisc_1);
    this->addChild(labelDisc_2);
	//this->addChild(joinTime);
}

void SGCreamMainCell::select()
{
    EFFECT_PLAY(MUSIC_BTN);
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    if (delegate) {
        delegate->itemselect(tableview,ind);
    }
}


//设置信息
void SGCreamMainCell::setData(int index, SGStoryBean* bean, bool isShowNew)
{
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_creamLayer);
	
   // CCString *storyName = CCString::createWithFormat("%d.%s", bean->getStoryId(), bean->getStoryName()->getCString());
	 CCString *storyName = CCString::createWithFormat("%s", bean->getStoryName()->getCString());
    labelName->setString(storyName->getCString());
    
    _conTime = bean->getConTime();
    CCString *string = NULL;
    

	
    if(bean->getIsWin() == 1) {
        spriteIsPass->setVisible(false);
        if (bean->getRank() <= 0 ||
            bean->getRank() > 6) {
            bean->setRank(1);
        }
        const char *spname = CCString::createWithFormat("Label_win%d.png", bean->getRank())->getCString();
        spriteRank->setDisplayFrame(CCSprite::createWithSpriteFrameName(spname)->displayFrame());
        spriteRank->setVisible(true);
        
        if (bean->getExpRate() > 1.0f || bean->getCoinRate() > 1.0f) {
            labelDisc_1->setVisible(true);
            labelDisc_2->setVisible(true);
            float rate = bean->getExpRate();
            if (rate != 1.0f) {
                string = CCString::createWithFormat(str_StorySectionCell_str20, rate);
            }else {
                rate = bean->getCoinRate();
                string = CCString::createWithFormat(str_StorySectionCell_str21, rate);
            }
            labelDisc_1->setString(string->getCString());
            string = CCString::createWithFormat(str_StorySectionCell_str22, _conTime);
            labelDisc_2->setString(string->getCString());
            labelDisc_2->setInsideColor(ccRED);
            
            CCAction *action = CCSequence::create(CCDelayTime::create(60.0f),
                                                  CCCallFunc::create(this, callfunc_selector(SGCreamMainCell::resetTime)),
                                                  NULL);
            labelDisc_2->runAction(action);
        }else {
            labelDisc_1->setVisible(false);
            labelDisc_2->setVisible(false);
        }
    }else{
        labelDisc_1->setVisible(false);
        labelDisc_2->setVisible(false);
        spriteRank->setVisible(false);
        spriteIsPass->setVisible(true);
    }
	
	spriteIsPass->setVisible(isShowNew);
}



void SGCreamMainCell::resetTime()
{
    _conTime -= 1;
    if (_conTime <= 0) {
        labelDisc_1->setVisible(false);
        labelDisc_2->setVisible(false);
    }else {
        labelDisc_2->setString(CCString::createWithFormat(str_OverBattleBox_str25, _conTime)->getCString());
        CCAction *action = CCSequence::create(CCDelayTime::create(60.0f),
                                              CCCallFunc::create(this, callfunc_selector(SGCreamMainCell::resetTime)),
                                              NULL);
        labelDisc_2->runAction(action);
    }
}
