//
//  SGStorySectionCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGStorySectionCell.h"

#include "SGStorySectionCell.h"
#include "SGLayout.h"
#include "GameConfig.h"
#include "SGStaticDataManager.h"
#include "SGMenu.h"
#include "ResourceManager.h"
#include "SGCreamBattleLayer.h"
#include "PlotModule.pb.h"
#include "SGShouJingZhou.h"
#include "SGStringConfig.h"
#include "SGGuideManager.h"

#define  SECTIONSTARGTAG -1009
SGStorySectionCell::SGStorySectionCell():spriteIsPass(NULL)
,spriteStar(NULL)
,labelSceneName(NULL)
,needStarInfo(NULL)
,joinTime(NULL)
,_isPassLevel(false)
,storyBean(NULL)
,delegate(NULL)
,isFromBuy(false)
,menu(NULL)
,backBtn(NULL)
,_bigBGBtn(NULL)
{
	data = CCArray::create();
	data->retain();
}

SGStorySectionCell::~SGStorySectionCell()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_storySectionLayer);
	
	
	CC_SAFE_RELEASE(data);
    data = NULL;
}

SGStorySectionCell* SGStorySectionCell::create(SGStorySectionDelegate *delegate_, SGStorySceneBean* bean, CCArray *data)
{
	SGStorySectionCell *actionCell = new SGStorySectionCell();
    actionCell->storyBean=bean;
    actionCell->delegate = delegate_;
    if (data) {
        actionCell->data->addObjectsFromArray(data);
    }
	
	if (actionCell && actionCell->init()) {
        actionCell->viewDidLoad();
		actionCell->autorelease();
		return actionCell;
	}
	CC_SAFE_DELETE(actionCell);
	return actionCell;
}

//重写对应的onEnter方法，设置menu的优先级//11.20
void SGStorySectionCell::onEnter()
{
	CCLayer::onEnter();
	menu->setTouchPriority(10);
}




//进入章节内部加载
void SGStorySectionCell::viewDidLoad()
{
	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
	//ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
	
    
	menu=CCMenu::create();
    this->addChild(menu);
    menu->setPosition(CCPointZero);
   
    int storyId = storyBean->getStoryId();
    if (storyId < 10000)
	{
        backBtn = SGButton::create("mainBtnZhanStar_bg.png",
                                         NULL,
                                         this,
                                         menu_selector(SGStorySectionCell::entrySectionHandler),
                                         ccp(0, 0),
                                         false,
                                         true);
    
    }
    else
    {
        backBtn = SGButton::create("mainBtnZhan_bg.png",
                                   NULL,
                                   this,
                                   menu_selector(SGStorySectionCell::entrySectionHandler),
                                   ccp(0, 0),
                                   false,
                                   true);
    }
    
    backBtn->setAnchorPoint(ccp(0.5, 0.5f));
    backBtn->ignoreAnchorPointForPosition(false);
    backBtn->setPosition(ccp(bgSize.width/2-20-backBtn->getContentSize().width/2, 0));
	backBtn->setTag(-20);


    spriteIsPass = CCSprite::createWithSpriteFrameName("Label_win6.png");
    spriteIsPass->setPosition(ccp(-bgSize.width/2+spriteIsPass->getContentSize().width*.5f, bgSize.height*.5f-spriteIsPass->getContentSize().height*.5f));
    spriteIsPass->setVisible(false);
    
	//george精英副本
	if(storyBean->getStoryId() >= 20000)
	{
		CCSprite *kl = CCSprite::createWithSpriteFrameName("jykl.png");
		kl->setPosition(ccp(-bgSize.width/2+kl->getContentSize().width*.5f + 50, bgSize.height*.5f-kl->getContentSize().height*.5f - 12));
		this->addChild(kl);
	}
    bgbgbg = NULL;
    if (storyBean->getExp() == STARWARDTAG)
        labelSceneName = SGCCLabelTTF::create(str_StorySectionCell_str1, FONT_LISHU, 40 , ccc3(0xff, 0xf4,0x5c));
    else
        labelSceneName = SGCCLabelTTF::create("", FONT_BOXINFO, 32 , ccc3(0xff, 0xd7, 0x00));

	joinTime = SGCCLabelTTF::create("", FONT_BOXINFO, 22,ccWHITE);
    
    labelSceneName->setAnchorPoint(ccp(0, 0.5f));
	
	//精英副本中加骷髅后移标题位置
	if (storyBean->getStoryId() >= 20000)
	{
		labelSceneName->setPosition(ccp(-bgSize.width/2+ZoomInX(40) + 50, 0));
	}
	else
	{
		labelSceneName->setPosition(ccp(-bgSize.width/2+ZoomInX(40), 0));
    }
	

	//boss关红底
	SGButton *bigBGBtn = NULL;
	
	if (storyId < 10000)
	{
		SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataById(storyId);
		if (storyId != 1 && storyId != 5 && storyBean->getSceneId() == story->getStorySections()->count())
		{
			bigBGBtn = SGButton::create("upwar_cell_bg.png",
												  NULL,
												  this,
												  menu_selector(SGStorySectionCell::showSectionInfoHandler),
												  ccp(0, 0),
												  false,
												  true);
		}
        else if (storyBean->getExp() == STARWARDTAG)
        {
            bigBGBtn = SGButton::create("item_bigYw.png",
                                        NULL,
                                        this,
                                        menu_selector(SGStorySectionCell::showSectionInfoHandler),
                                        ccp(0, 0),
                                        false,
                                        true);
        }
		else
		{
		
			 bigBGBtn = SGButton::create("upwar_cell_bg.png",
													  NULL,
													  this,
													  menu_selector(SGStorySectionCell::showSectionInfoHandler),
													  ccp(0, 0),
													  false,
													  true);
		}
        bigBGBtn->setAnchorPoint(ccp(0.5,0.5));
        bigBGBtn->ignoreAnchorPointForPosition(false);
        bigBGBtn->setPosition(ccp(-backBtn->getContentSize().width*1.2/2, 0));
        bigBGBtn->setContentSize(CCSizeMake(bigBGBtn->getContentSize().width- backBtn->getContentSize().width*1.2, bigBGBtn->getContentSize().height));
	}
	else
	{
		bigBGBtn = SGButton::create("upwar_cell_bg.png",
									NULL,
									this,
									menu_selector(SGStorySectionCell::showSectionInfoHandler),
									ccp(0, 0),
									false,
									true);
		bigBGBtn->setAnchorPoint(ccp(0.5,0.5));
		bigBGBtn->ignoreAnchorPointForPosition(false);
        bigBGBtn->setPosition(ccp(0, 0));
        bigBGBtn->setContentSize(CCSizeMake(bigBGBtn->getContentSize().width- backBtn->getContentSize().width*1.2, bigBGBtn->getContentSize().height));

	}
    
    if (storyId < 10000) {
        //添加关卡星级
        while (getChildByTag(SECTIONSTARGTAG) != NULL) {
            getChildByTag(SECTIONSTARGTAG)->removeFromParent();
        }
        CCSprite *visitItemstar = NULL;
        CCPoint starPos = ccpAdd(ccp(-backBtn->getContentSize().width/2-3,-backBtn->getContentSize().height/2),  backBtn->getPosition());
        int starNum = storyBean->getRank();
        for (int i=0; i<starNum; i++) {
            visitItemstar = CCSprite::createWithSpriteFrameName("xx.png");
            visitItemstar->setPosition(ccpAdd(ccp(visitItemstar->getContentSize().width*0.4,visitItemstar->getContentSize().height/2), starPos) );
            addChild(visitItemstar,1000,SECTIONSTARGTAG);
            starPos.x += visitItemstar->getContentSize().width*0.82;
        }
        
        if (storyBean->getExp() == STARWARDTAG) {
            if (storyBean->getOpenState() == 0) {
                backBtn->setImageWithFrame("reward_font_ylq.png");
                backBtn->setEnabled(false);
            }
            else
            {
                backBtn->setImageWithFrame("mainBtnLing_bg.png");
            }
        }
    }
    
    this->addChild(backBtn,1);
    addBtn(backBtn);
	_bigBGBtn = bigBGBtn;
    btn=bigBGBtn;
    this->addChild(btn,-5, ITEMBIGBGTAG);
    addBtn(btn);

    
    this->addChild(labelSceneName);
    if (storyId < 10000 && storyBean->getExp() == STARWARDTAG &&(storyBean->getZhandou() - storyBean->getPower() >0)) {
        needStarInfo = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str3)->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf4, 0x5c));
        needStarInfo->setPosition(ccpAdd(labelSceneName->getPosition(), ccp(labelSceneName->getContentSize().width/2, 0)));
        this->addChild(needStarInfo);
        
        SGCCLabelTTF *needStarNum = SGCCLabelTTF::create(CCString::createWithFormat("%d",storyBean->getZhandou() - storyBean->getPower())->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf4, 0x5c));
        needStarNum->setPosition(ccp(28*4, 2));   
        needStarInfo->addChild(needStarNum);
        
        spriteStar = CCSprite::createWithSpriteFrameName("xx.png");
        spriteStar->setPosition(ccp(spriteStar->getContentSize().width*0.2 + 28*2.5,spriteStar->getContentSize().height/6+ needStarInfo->getPositionY()));
        addChild(spriteStar);
    }
    this->addChild(spriteIsPass);
	this->addChild(joinTime);
}

void SGStorySectionCell::setData(SGStorySceneBean* bean, CCArray *data)
{
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_storySectionLayer);
	//int joinCream = 0;//精英副本
    storyBean = bean;
    if (this->data == NULL)
    {
        data = CCArray::create();
        data->retain();
    }
    this->data->addObjectsFromArray(data);
    
	if (storyBean->getStoryId() >= 20000)
    {
		//精英副本
		int dayTimes = storyBean->getCreamDayTimes();;
		CCString *jt = CCString::createWithFormat(str_StorySectionCell_str2, dayTimes);
		joinTime->setString(jt->getCString());
		joinTime->setPosition(ccp(backBtn->getContentSize().width/2 - 30,backBtn->getContentSize().height/2 - 75));
		
	}else if(storyBean->getStoryId() > 10000&&storyBean->getStoryId() < 20000)
    {
    }
    

    int storyId = bean->getStoryId();
    if (storyId >= 10000)
        //storyId = bean->getStoryId() - 10000;
        labelSceneName->setString(bean->getSceneName()->getCString());
    else
    {
        CCString *sceneName = NULL;
        SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataById(storyId);
        if (bean->getSceneId() == story->getStorySections()->count()) {

            sceneName = CCString::createWithFormat("BOSS %s", bean->getSceneName()->getCString());
        }
        else
        {
            int storyTmp = storyId;
            switch (storyTmp)
            {
                case 2:
                    storyTmp = 4;
                    break;
                case 3:
                    storyTmp = 6;
                    break;
                case 4:
                    storyTmp = 2;
                    break;
                case 6:
                    storyTmp = 3;
                    break;
                default:
                    break;
            }
            
            sceneName = CCString::createWithFormat("%d-%d %s", storyTmp, bean->getSceneId(), bean->getSceneName()->getCString());
        }
        
        if (labelSceneName) {
            labelSceneName->removeFromParent();
            labelSceneName = NULL;
        }
        if (needStarInfo) {
            needStarInfo->removeFromParent();
            needStarInfo = NULL;
        }
        if (spriteStar) {
            spriteStar->removeFromParent();
            spriteStar = NULL;
        }
        if (storyBean->getExp() == STARWARDTAG) {
            labelSceneName = SGCCLabelTTF::create(str_StorySectionCell_str1, FONT_BOXINFO, 40 , ccc3(0xff, 0xf4, 0x5c));
            
            if (storyBean->getZhandou() - storyBean->getPower() >0) {
                needStarInfo = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str3)->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf4, 0x5c));
                needStarInfo->setPosition(ccpAdd(labelSceneName->getPosition(), ccp(labelSceneName->getContentSize().width/2, 0)));
                this->addChild(needStarInfo);
                
                SGCCLabelTTF *needStarNum = SGCCLabelTTF::create(CCString::createWithFormat("%d",storyBean->getZhandou() - storyBean->getPower())->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf4, 0x5c));
                needStarNum->setPosition(ccp(28*4, 2));
                needStarInfo->addChild(needStarNum);
                
                spriteStar = CCSprite::createWithSpriteFrameName("xx.png");
                spriteStar->setPosition(ccp(spriteStar->getContentSize().width*0.2 + 28*2.5,spriteStar->getContentSize().height/6+ needStarInfo->getPositionY()));
                addChild(spriteStar);
            }
        }
        else
            labelSceneName = SGCCLabelTTF::create(sceneName->getCString(), FONT_BOXINFO, 32 , ccc3(0xff, 0xd7, 0x00));
        labelSceneName->setAnchorPoint(ccp(0, 0.5f));
        labelSceneName->setPosition(ccp(-bgSize.width/2+ZoomInX(40), 0));
        this->addChild(labelSceneName);
    }
    
    if (storyId < 10000)
    {
        //添加关卡星级
        while (getChildByTag(SECTIONSTARGTAG) != NULL) {
            getChildByTag(SECTIONSTARGTAG)->removeFromParent();
        }
        CCSprite *visitItemstar = NULL;
        CCPoint starPos = ccpAdd(ccp(-backBtn->getContentSize().width/2,-backBtn->getContentSize().height/2),  backBtn->getPosition());
        int starNum = storyBean->getRank();
        for (int i=0; i<starNum; i++) {
            visitItemstar = CCSprite::createWithSpriteFrameName("xx.png");
            visitItemstar->setPosition(ccpAdd(ccp(visitItemstar->getContentSize().width*0.4,visitItemstar->getContentSize().height/2), starPos) );
            addChild(visitItemstar,1000,SECTIONSTARGTAG);
            starPos.x += visitItemstar->getContentSize().width*0.82;
        }
        
        SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataById(storyId);
        if (storyId != 1 && storyId != 5 && storyBean->getSceneId() == story->getStorySections()->count() &&storyBean->getExp() != STARWARDTAG)
        {
            _bigBGBtn->setImage("upwar_cell_bg.png");
            backBtn->setImageWithFrame("mainBtnZhanStar_bg.png");
        }
        else if (storyBean->getExp() == STARWARDTAG)
        {
            _bigBGBtn->setImage("item_bigYw.png");
            if (storyBean->getOpenState() == 0) {
                backBtn->setImageWithFrame("reward_font_ylq.png");
                backBtn->setEnabled(false);
            }
            else
            {
                backBtn->setImageWithFrame("mainBtnLing_bg.png");
            }
           
        }
        else
        {
            _bigBGBtn->setImage("upwar_cell_bg.png");
            backBtn->setImageWithFrame("mainBtnZhanStar_bg.png");
        }
    }
    else
    {
        _bigBGBtn->setImageWithFrame("upwar_cell_bg.png");
    }
    if (backBtn != NULL)
    {
        backBtn->removeChildByTag(SECTIONCELLEFFECTAG, true);
        CCSpriterX * temp =  (CCSpriterX *)this->getChildByTag(HAND_EFFECT);
        if(temp)
        {
            temp->stop();
            this->removeChild(temp ,true);
        }
    }
    
    if(SGMainManager::sourceToStory )
    {
        if ((SGMainManager::static_plotId == bean->getPlotId()) && backBtn)
        {
            CCSprite *spriteEffect = CCSprite::createWithSpriteFrameName("xuanwo.png");
            if (spriteEffect)
            {
                spriteEffect->setAnchorPoint(ccp(0.5,0.5));
                spriteEffect->ignoreAnchorPointForPosition(false);
                spriteEffect->setPosition(ccp(backBtn->getContentSize().width/2,backBtn->getContentSize().height/2));
                spriteEffect->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
                backBtn->addChild(spriteEffect,SECTIONCELLEFFECTAG,SECTIONCELLEFFECTAG);
            }
            CCSpriterX *m_spTouch = CCSpriterX::create("animationFile/jiantou.scml", true , true);
            m_spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
            m_spTouch->setPosition(ccpAdd(ccp( 0 , 0), backBtn->getPosition() ) );
            m_spTouch->setisloop(true);
            m_spTouch->play();
            this->addChild(m_spTouch,HAND_EFFECT , HAND_EFFECT);
            
            SGMainManager::sourceToStory = false;
        }

    }
    else if(bean->getRank() == 0 && (storyBean->getExp() != STARWARDTAG || storyBean->getOpenState() != 0))
    {
        spriteIsPass->setVisible(true);
        if (backBtn)
        {
            CCSprite *spriteEffect = CCSprite::createWithSpriteFrameName("xuanwo.png");
            if (spriteEffect)
            {
                spriteEffect->setAnchorPoint(ccp(0.5,0.5));
                spriteEffect->ignoreAnchorPointForPosition(false);
                spriteEffect->setPosition(ccp(backBtn->getContentSize().width/2,backBtn->getContentSize().height/2));
                spriteEffect->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
                backBtn->addChild(spriteEffect,SECTIONCELLEFFECTAG,SECTIONCELLEFFECTAG);
            }
            SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
            SGGuideManager * manager = SGGuideManager::shareManager();
            if(playerInfo->getPlayerLevel() < playerInfo->getShowPlayerNoticeLimitLev() && storyBean->getExp()!= STARWARDTAG && !playerInfo->isGuid() && !manager->isGuide)
            {
                CCSpriterX *m_spTouch = CCSpriterX::create("animationFile/jiantou.scml", true , true);
                m_spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
                m_spTouch->setPosition(ccpAdd(ccp( 0 , 0), backBtn->getPosition() ) );
                m_spTouch->setisloop(true);
                m_spTouch->play();
                this->addChild(m_spTouch,HAND_EFFECT , HAND_EFFECT);
            }

        }
    }
}




void SGStorySectionCell::showSectionInfoHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (delegate) {
        delegate->showSectionInfoHandler(this);
    }
    
}
void SGStorySectionCell::entrySectionHandler()  //cgp_pro
{
    EFFECT_PLAY(MUSIC_BTN);

	//精英副本进入战斗判定
	if (storyBean->getStoryId() < 20000)
	{
        //如果是活动副本
        if (storyBean->getStoryId() >= 10000 && storyBean->getCreamDayTimes() <= 0)
        {
            SG_SHOW_WINDOW(str_StorySectionCell_str401);
            return ;
        }
        
		if (delegate) {
			delegate->entrySectionHandler(this);
		}
	}
	else if (storyBean->getStoryId() >= 20000)
	{
		int selectIndex = 0;
		if (delegate)
		{
			selectIndex = delegate->getSelectIndex();
		}
		storyBean = (SGStorySceneBean *)data->objectAtIndex(selectIndex);
		
		if (SGPlayerInfo::sharePlayerInfo()->getPlayerPower() >= storyBean->getPower())
		{
			if (storyBean->getCreamCanJoin() == 0 && storyBean->getCreamDayTimes())
			{
				//有剩余次数并且体力足够，直接进战斗
				if (delegate) {
					delegate->entrySectionHandler(this);
				}
			}
			else if (storyBean->getCreamBuyTimes() != 0 && !storyBean->getCreamDayTimes())
			{
				//可购买挑战次数,要传入此次购买的价格，已经购买的次数
				if (!isFromBuy)// 如果是从购买弹窗中直接进入，则不再显示弹窗
				{
					SGCreamBattleBox *box = SGCreamBattleBox::create(NULL, storyBean->getBuyCompTimes() , storyBean->getBuyPrice(), storyBean->getPlotId() , true , (storyBean->getCreamBuyTimes() + storyBean->getBuyCompTimes()) );
					SGMainManager::shareMain()->showBox(box);
					this->isFromBuy = false;
				}
				
			}
			else if (storyBean->getCreamBuyTimes() == 0  && storyBean->getCreamDayTimes() == 0)
			{
				//不可购买，也不可挑战
				SGMainManager::shareMain()->showMessage(str_StorySectionCell_str4);
				return ;
			}
		}
		else
		{
			if (delegate)
				delegate->gotoPurchPower();
		}
	}
    else if (storyBean->getStoryId() >= 10000)
    {
        CCLOG("----Activity----");
    }
}

SGStorySceneBean *SGStorySectionCell::getGift()
{
    return storyBean;
	
}

//void SGStorySectionCell::setIsCanTouch(bool isTouch)
//{
//    ((CCMenu*)this->getChildByTag(1000))->setEnabled(isTouch);
//}

//买精英本次数弹窗

SGCreamBattleBox::SGCreamBattleBox():buyCompTimes(0)
, price(0)
, storyId(0)
, _havechance(false)
, canBuyTimes(0)
, willAddCount(0)
, nextVip(0)
{
	
}
SGCreamBattleBox::~SGCreamBattleBox()
{
	
}

SGCreamBattleBox *SGCreamBattleBox::create(SGBoxDelegate *del, int buyCompTimes, int price, int storyId,bool havechance,int canbuytimes,int willaddcount, int nVip)
{
	SGCreamBattleBox *cbb = new SGCreamBattleBox();
	
	if (cbb->init(del, box_creamBattleBox, CCSizeMake(535, 324)))
	{
		cbb->buyCompTimes = buyCompTimes;
		cbb->price = price;
		cbb->storyId = storyId;
        cbb->canBuyTimes=canbuytimes;
        cbb->_havechance=havechance;
        cbb->willAddCount=willaddcount;
		cbb->nextVip = nVip;
		cbb->autorelease();
		cbb->initView();
		return cbb;
	}
	CC_SAFE_DELETE(cbb);
	return NULL;
}

void SGCreamBattleBox::initView()
{
    SGCCLabelTTF *tips;
    CCSize tipLabelSize = CCSizeMake(32*13, 220);
    if(storyId==-1) //守荆州相关。
    {
//        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
        if(!_havechance)
        {
            if (this->willAddCount == 0) //当前没有，也不能提升获得。
            {
                tips = SGCCLabelTTF::create(str_shoujingzhou_no_more_time, FONT_PANGWA, 28, tipLabelSize);
            }
            else //全部用盡
            {
                tips = SGCCLabelTTF::create(CCString::createWithFormat(str_shoujingzhou_please_buy_vip, nextVip, willAddCount)->getCString(),FONT_PANGWA, 28, tipLabelSize);
            }
            
            /*
            //old bgn
			int currVipLvl = player->getplayerVipLevel();
			
			if (currVipLvl < 3)
			{
				// 您今日的守荆州次数已用完。累计储值100元即可获得vip等级3，拥有购买守荆州次数的机会。

                int shoujingCount = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("10"))->objectForKey("value"))->intValue();
				tips = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str5,shoujingCount)->getCString(), FONT_PANGWA, 28, tipLabelSize);
			}
			else if (currVipLvl >= 3 && currVipLvl <= 11)
			{
				// 您今日的守荆州次数已用完。 您当前的vip等级为N级，今日剩余可购买守荆州的次数为0。 vip等级N+1级，拥有更多购买机会。
				tips = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str6, currVipLvl, nextVip)->getCString(),FONT_PANGWA, 28, tipLabelSize);
			}
			else// vip == 12 的情况
			{
				// 您今日的守荆州次数已用完。您当前的vip等级为12级，今日剩余可购买守荆州的次数为0。
				tips = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str7,
																	   currVipLvl)->getCString(), FONT_PANGWA, 28, tipLabelSize);
			}
            //old end
            */
            
        }
        else
        {
			//拥有购买次数的時候創建。
			tips = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str8,
																   canBuyTimes,price)->getCString(), FONT_PANGWA, 27, tipLabelSize);
        }
    }
    else //這裡是精英副本相关！
    {
		tips = SGCCLabelTTF::create(CCString::createWithFormat(str_StorySectionCell_str9, buyCompTimes,
															   canBuyTimes,price)->getCString(), FONT_PANGWA, 28, tipLabelSize);
	}
    tips->setAnchorPoint(ccp(.5, 1));
    tips->setPosition(ccpAdd(biao->getPosition(), ccp(0, -50)));
	
	this->addChild(tips);
    
    
    
    if(storyId==-1)
    {
        if(!_havechance)
        {
			SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGCreamBattleBox::closebox), CCPointZero, false, true);
			this->addBtn(okButton);
			CCPoint center = SGLayout::getPoint(kMiddleCenter);
			okButton->setPosition(ccpAdd(center, ccp(0, -324/2+40)));
        }
        else
        {
            SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this,
													menu_selector(SGCreamBattleBox::pressOKButton), CCPointZero, false, true);
            SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this,
													  menu_selector(SGCreamBattleBox::pressCancelButton), CCPointZero, false, true);
            
            this->addBtn(okButton);
            this->addBtn(cancelButton);
            CCPoint center = SGLayout::getPoint(kMiddleCenter);
            okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
            cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+40)));
        
        }
    }
    else
    {
    
        SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this,
											  menu_selector(SGCreamBattleBox::pressOKButton), CCPointZero, false, true);
        SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this,
												  menu_selector(SGCreamBattleBox::pressCancelButton), CCPointZero, false, true);
        
        this->addBtn(okButton);
        this->addBtn(cancelButton);
        CCPoint center = SGLayout::getPoint(kMiddleCenter);
        okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
        cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+40)));
    }
	

}
void SGCreamBattleBox::boxClose()
{
	SGBaseBox::boxClose();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,true);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }

}

void SGCreamBattleBox::pressOKButton()
{
	CCLOG("Press OK Button");
    
	main::BossPlotBuyCountRequest *buyCream = new main::BossPlotBuyCountRequest();
	buyCream->set_bossplotid(storyId);
	//buyCream->set_price(price);
	SGSocketClient::sharedSocketClient()->send(MSG_BUY_CREAM, buyCream);
 
	this->boxClose();
    if(storyId==-1)//守荆州
    {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        SGShouJingZhou *layer = ( SGShouJingZhou*)mainScene->getChildByTag(sg_shoujingzhou);
        if (mainLayer) {
            mainLayer->setIsCanTouch(true,false);
            
        }
        if(layer)
        {
            layer->setIsCanTouch(true);
        }
    }
}

void SGCreamBattleBox::pressCancelButton()
{
	CCLOG("Press Cancel Button");
	this->boxClose();
	
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
	SGMainManager::shareMain()->closeBox();
}


void SGCreamBattleBox::closebox()
{
    this->boxClose();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
	SGMainManager::shareMain()->closeBox();
}





