//
//  SGStoryMainCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGStoryMainCell.h"
#include "SGLayout.h"
#include "GameConfig.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGGuideManager.h"
#include "SGStaticDataManager.h"


#define STORYSTARGTAG -100
SGStoryMainCell::SGStoryMainCell():spriteIsPass(NULL)
,spriteLock(NULL)
,_lockPos(CCPointZero)
,labelName(NULL)
,labelDisc_1(NULL)
,labelDisc_2(NULL)
,rankRewardPop(NULL)
,_conTime(0)
,delegate(NULL)
,tableview(NULL)
 , m_spTouch(NULL)
{

    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storyLayer);
    
    spriteLock = CCSprite::createWithSpriteFrameName("story_lock.png");
    spriteLock->setVisible(false);
    this->addChild(spriteLock,1100);

    _conTime = 0;
}

SGStoryMainCell::~SGStoryMainCell()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_storyLayer);
}

////
SGStoryMainCell* SGStoryMainCell::create(SGStoryMainCellDelegate*del,SNSTableView*table)
{
    SGStoryMainCell *detailCell = new SGStoryMainCell();
    if(detailCell && detailCell->init())
    {
        detailCell->tableview=table;
        detailCell->delegate=del;
        detailCell->viewDidLoad();
        detailCell->autorelease();
        return detailCell;
    }
    
    CC_SAFE_DELETE(detailCell);
    return NULL;
    
}
///////

///////

void SGStoryMainCell::viewDidLoad()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sgstorysectionlayer/sgstorysectionlayer.plist");
	
//	bgbgbg = NULL;
    spriteIsPass = CCSprite::createWithSpriteFrameName("Label_win6.png");
    spriteIsPass->setPosition(ccp(-bgSize.width/2+spriteIsPass->getContentSize().width*.5f, bgSize.height*.5f-spriteIsPass->getContentSize().height*.5f));
    spriteIsPass->setVisible(false);
    
    
    /////////
    btn=SGButton::create("upwar_cell_bg.png", NULL, this, menu_selector(SGStoryMainCell::buttonClick),CCPointZero,false,true);
 //   menu1->addChild(btn);
    //this->addChild(menu1);
    /////////
    this->addChild(btn);
    addBtn(btn);
    ///////////
    labelName = CCLabelTTF::create("", FONT_PANGWA, FONTSIZE(17));
    labelDisc_1 = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(12));
    labelDisc_2 = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(12));
    
    labelName->setAnchorPoint(ccp(0, 0.5f));
    labelName->setPosition(ccp(-bgSize.width/2+ZoomInX(40), 0));
    
    labelDisc_1->setAnchorPoint(ccp(1, 0.5));
    labelDisc_2->setAnchorPoint(ccp(1, 0.5));
    labelDisc_1->setPosition(ccp(bgSize.width/2-15, 15));
    labelDisc_2->setPosition(ccp(bgSize.width/2-15, -15));
    
    this->addChild(spriteIsPass);
    this->addChild(labelName);
    this->addChild(labelDisc_1);
    this->addChild(labelDisc_2);
    
    //添加关卡星级
    while (getChildByTag(STORYSTARGTAG) != NULL) {
        getChildByTag(STORYSTARGTAG)->removeFromParent();
    }
    CCSprite *visitItemstar = NULL;
    float starPos = (int)(btn->getContentSize().width*0.3+1);
    for (int i=0; i<3 ; i++)
    {
        if (1 == i) {
            _lockPos = ccp(starPos,0);
        }
        visitItemstar = CCSprite::createWithSpriteFrameName("heixx.png");
        visitItemstar->setPosition(ccp(starPos,0));
        addChild(visitItemstar,1000,STORYSTARGTAG-1);
        starPos += visitItemstar->getContentSize().width+1;
    }
    
    rankRewardPop = CCSprite::createWithSpriteFrameName("storyRewardPop.png");
    rankRewardPop->setPosition(ccp(starPos - visitItemstar->getContentSize().width,visitItemstar->getContentSize().height*0.8));
    addChild(rankRewardPop,1000,0);
    rankRewardPop->setVisible(false);
    
    m_spTouch = CCSpriterX::create("animationFile/jiantou.scml", true , true);
    m_spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
    m_spTouch->setPosition(ccp(labelName->getPosition().x + 200 , labelName->getPosition().y ) );
    m_spTouch->setisloop(true);
    m_spTouch->setVisible(false);
    m_spTouch->play();
    addChild(m_spTouch , 10);
    
    SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
    if(playerInfo->getPlayerLevel() < playerInfo->getShowPlayerNoticeLimitLev() &&
       !playerInfo->isGuid() && !SGGuideManager::shareManager()->isGuide)
    {
        m_spTouch->setVisible(true);
    }
}

//设置信息
void SGStoryMainCell::setData(int index, SGStoryBean* bean)
{
	
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storyLayer);
    
    CCString *storyName = CCString::createWithFormat("%d.%s", bean->getStoryId(), bean->getStoryName()->getCString());
    labelName->setString(storyName->getCString());
    
    _conTime = bean->getConTime();
    
    
    //闯关  等级不足 则加锁。
    //debug by: zyc
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerLevel() < bean->getMinLevel() )
    {
        spriteLock->setVisible(true);
        spriteLock->setPosition(_lockPos);
    }
    else
    {
        if (spriteLock )
            spriteLock->setVisible(false);
    }

    if(bean->getIsWin() == 1)
    {
        spriteIsPass->setVisible(false);

        if (bean->getExpRate() > 1.0f || bean->getCoinRate() > 1.0f) {
            labelDisc_1->setVisible(false);
            labelDisc_2->setVisible(false);
        }
        else
        {
            labelDisc_1->setVisible(false);
            labelDisc_2->setVisible(false);
        }
    }
    else
    {
        labelDisc_1->setVisible(false);
        labelDisc_2->setVisible(false);
        spriteIsPass->setVisible(true);
    }

    
    //添加关卡星级
    while (getChildByTag(STORYSTARGTAG) != NULL)
    {
        getChildByTag(STORYSTARGTAG)->removeFromParent();
    }
    CCSprite *visitItemstar = NULL;
    float starPos = (int)(btn->getContentSize().width*0.3);
    int starNum = bean->getRankStars();
    for (int i=0; i<starNum ; i++) {
        visitItemstar = CCSprite::createWithSpriteFrameName("xx.png");
        visitItemstar->setPosition(ccp(starPos,0));
        addChild(visitItemstar,1000,STORYSTARGTAG);
        starPos += visitItemstar->getContentSize().width;
    }
    
    if (rankRewardPop != NULL) {
        if (bean->getRankReward() == 1) {
            rankRewardPop->setVisible(true);
            rankRewardPop->runAction(CCRepeatForever::create( CCSequence::create(CCScaleTo::create(0.4, 0.8),CCScaleTo::create(0.4, 1),NULL) ));
        }
        else
        {
            rankRewardPop->setVisible(false);
            rankRewardPop->stopAllActions();
        }
    }
    if(m_spTouch && spriteIsPass->isVisible())
    {
        SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
        if(playerInfo->getPlayerLevel() < playerInfo->getShowPlayerNoticeLimitLev() &&
           !playerInfo->isGuid() )
        {
            m_spTouch->setVisible(true);
        }
    }
    else if(m_spTouch)
        m_spTouch->setVisible(false);
}

void SGStoryMainCell::resetTime()
{
    _conTime -= 1;
    if (_conTime <= 0) {
        labelDisc_1->setVisible(false);
        labelDisc_2->setVisible(false);
    }else {
        labelDisc_2->setString(CCString::createWithFormat(str_OverBattleBox_str25, _conTime)->getCString());
        CCAction *action = CCSequence::create(CCDelayTime::create(60.0f),
                                              CCCallFunc::create(this, callfunc_selector(SGStoryMainCell::resetTime)),
                                              NULL);
        labelDisc_2->runAction(action);
    }
}
////////////
void SGStoryMainCell::buttonClick(CCObject*obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);
}