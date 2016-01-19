//
//  SGSpecialBattleLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-12-12.
//
//

#include "SGSpecialBattleLayer.h"
#include "SGMainManager.h"
#include "SGDrawCards.h"
#include "SGSpecialBattleSectionLayer.h"
#include "SpecialBattleModule.pb.h"
#include "AppMacros.h"
#include "RoleShopModule.pb.h"
#include "SGMallLayer.h"

SGRuleItem::SGRuleItem():
index(0)
{
    
}
SGRuleItem::~SGRuleItem()
{
    
}
SGRuleItem *SGRuleItem::create(int index, std::string ruleInfo)
{
    SGRuleItem *rl = new SGRuleItem;
    if (rl)
    {
        rl->index = index;
        rl->ruleInfo = ruleInfo;
        rl->autorelease();
        rl->initView();
        return rl;
    }
    CC_SAFE_DELETE(rl);
    return NULL;
}

void SGRuleItem::initView()
{
    CCLayer *bg = CCLayer::create();
	this->addChild(bg, 1);
	
	//名字与排名数字
	SGCCLabelTTF *playerName = SGCCLabelTTF::create(CCString::createWithFormat("%3d.	%s", index, ruleInfo.c_str())->getCString(),
                                                    FONT_PENGYOU, 22, CCSizeMake(330, 80));
	playerName->setAnchorPoint(ccp(0, 0.5));
	bg->addChild(playerName, 2);
	playerName->setPosition(ccpAdd(playerName->getPosition(), ccp(-100, 0)));
	
	//设置item的大小
	this->setContentSize(CCSizeMake(340, 85));
}


//规则显示弹窗
SpRuleInfoBox::SpRuleInfoBox()
{
    
}
SpRuleInfoBox::~SpRuleInfoBox()
{
    
}

SpRuleInfoBox *SpRuleInfoBox::create()
{
    SpRuleInfoBox *rBox = new SpRuleInfoBox();
    if (rBox && rBox->init(NULL, box_spRuleBox, CCSize(450, 550)))
    {
        rBox->autorelease();
        rBox->initView();
        return rBox;
    }
    CC_SAFE_DELETE(rBox);
    return NULL;
}

void SpRuleInfoBox::initView()
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
	fontbg->setPreferredSize(CCSizeMake(380, 380));
	fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 25)));
	SGCCLabelTTF *tips = SGCCLabelTTF::create(str_SpecialBattleIntro, FONT_PANGWA, 32 ,ccRED);
	this->addChild(tips, 2);
	tips->setPosition(ccpAdd(biao->getPosition(), ccp(0, 0)));
	
    //可滚动规则视图
    
	CCScrollView *m_scrollview = CCScrollView::create(CCSizeMake(350, 150));
	m_scrollview->setViewSize(CCSizeMake(350, 360));
    CCLayerColor *container = CCLayerColor::create(ccc4(0, 0, 0, 0));
    container->setContentSize(CCSizeMake(350, 355));
    m_scrollview->setContainer(container);
	m_scrollview->setPosition(ccpAdd(center, ccp(-fontbg->getContentSize().width / 2, -fontbg->getContentSize().height / 2 + 30)));//设置scrollview位置
	
	m_scrollview->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(m_scrollview);
	m_scrollview->setTag(103);
	
	//测试数据
	int finalLen = 0;
	int rankSize = 10;
    SGStaticDataManager *mgr = SGStaticDataManager::shareStatic();
    
	for (int i = 0; i < mgr->getSpHelpInfoKeysNum(); i++)
	{
        
        std::string info = SGStaticDataManager::shareStatic()->getSingleSpBattleRule(i);
        if (info != "")
        {
            SGRuleItem *item = SGRuleItem::create(i, info.c_str());
            container->addChild(item);
            item->setPosition(ccpAdd(container->getPosition(), ccp(item->getContentSize().width / 2.8,
															   (item->getContentSize().height + 10) * (rankSize - i) - item->getContentSize().height - 30)));
            finalLen += (item->getContentSize().height + 10);
        }
	}
	m_scrollview->setContentOffset(ccp(0, -finalLen + m_scrollview->getContentSize().height - 10));
	m_scrollview->setContentSize(CCSizeMake(350, finalLen));
	m_scrollview->getContainer()->setContentSize(CCSizeMake(350, finalLen + 20));
    
    
	
	SGButton *okButton = SGButton::createFromLocal("box_btnbg.png", str_Ok, this, menu_selector(SpRuleInfoBox::pressOkButton),
												   ccp(0, 0), FONT_PANGWA, ccWHITE, 24);
	this->addBtn(okButton);
	okButton->setPosition(ccpAdd(center, ccp(0, -324/2+5 -70)));
}

void SpRuleInfoBox::pressOkButton(CCObject *obj)
{
    CCLOG("press ok button");
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
	SGMainManager::shareMain()->closeBox();
}



SGSpecialBattleLayer::SGSpecialBattleLayer():
chapDatas(NULL),
selectIndex(0)
{
    
}

SGSpecialBattleLayer::~SGSpecialBattleLayer()
{
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SPECIAL_BATTLE_SEC);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_specialBattleLayer);
    CC_SAFE_DELETE(chapDatas);
}


SGSpecialBattleLayer *SGSpecialBattleLayer::create(cocos2d::CCArray *specialBattleData)
{
    SGSpecialBattleLayer *specialLayer = new SGSpecialBattleLayer();
    
    if (specialLayer && specialLayer->init(NULL, sg_specialBattleLayer))
    {
        specialLayer->autorelease();
        if (specialBattleData)
        {
            specialLayer->chapDatas = CCArray::create();
            specialLayer->chapDatas->retain();
            specialLayer->chapDatas->initWithArray(specialBattleData);
        }
        specialLayer->initView();
        
        return specialLayer;
    }
    
    CC_SAFE_DELETE(specialLayer);
    return NULL;
}

//平铺二个背景
void SGSpecialBattleLayer::makeDoubleBg(int shift, int num, int tag)
{
    float x,y;
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    x = size.width;
    y = size.height+origin.y;
    
    CCSprite *background=CCSprite::createWithSpriteFrameName("sp_battle_space_bg.png");
    background->setScaleX(1.001);
    //iphone 5
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        background->setScaleY(1.22);
    }
    //ipad
    else if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height)
    {
        background->setScaleY(1.09);
        background->setScaleX(CCDirector::sharedDirector()->getWinSize().width / background->getContentSize().width);
    }
    else if (CCDirector::sharedDirector()->getWinSize().height == ipadhdResource.size.height)
    {
        background->setScaleY(1.2);
        background->setScaleX(CCDirector::sharedDirector()->getWinSize().width / background->getContentSize().width);
    }
    background->setAnchorPoint(CCPoint(0,0.5));
    background->setPosition(CCPoint(background->getContentSize().width*num+shift, y/2 + 28));
    this->addChild(background);
    background->setTag(tag);
    background->setZOrder(-1);
}

//无限移动二个背景
void SGSpecialBattleLayer::moveDoubleBg(float dt)
{
    float x,y;
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    x = size.width;
    y = size.height+origin.y;
    
    CCNode *bg1 = this->getChildByTag(99);
    CCSprite *sp1 = (CCSprite*)bg1;
    
    CCNode *bg2 = this->getChildByTag(98);
    CCSprite *sp2 = (CCSprite*)bg2;
    sp1->setPosition(CCPoint(sp1->getPositionX() - 1,sp1->getPositionY()));
    sp2->setPosition(CCPoint(sp2->getPositionX() - 1,sp2->getPositionY()));
    //移出区域就重置
    if(sp1->getPositionX() <= -sp1->getContentSize().width)
    {
        sp1->setPosition(CCPoint(sp1->getContentSize().width,y / 2 + 28));
    }
    //移出区域重置图片位置
    if(sp2->getPositionX() <= -sp2->getContentSize().width)
    {
        sp2->setPosition(CCPoint(sp2->getContentSize().width,y / 2 + 28));
    }

}





void SGSpecialBattleLayer::enterSpBattleSection(CCObject *obj)
{
    
    if(SGMainManager::shareMain()->getMainScene()->getChildByTag(box_spRuleBox))
    {
        return ;
    }
    
    SGButton *button = static_cast<SGButton *>(obj);
    CCLOG("enter %d chap", button->getTag());
    selectIndex = button->getTag();
    
    
    //获取点击的章的storyId
    int storyId = static_cast<SpecialBattleData *>(chapDatas->objectAtIndex(selectIndex))->storyId;
    //全局记下storyid
    SGMainManager::shareMain()->setSpBattleChapId(storyId);
    
    main::SpecialBattlePlotListRequest *plotDetail = new main::SpecialBattlePlotListRequest;
    plotDetail->set_storyid(storyId);
    
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_SEC, plotDetail);
}




//创建一个对应的简单的button，代表具体的章
CCLayerColor *SGSpecialBattleLayer::createChapButton(int buttonTag, SpecialBattleData *data)
{
    
    CCLayerColor *layerColor = CCLayerColor::create(ccc4(0, 255, 0, 0), 160, 160);
    layerColor->setTag(111);
    layerColor->setAnchorPoint(ccp(0.5, 0.5));
    
    if (data)
    {
    
        CCSprite *headIcon = SGDrawCards::createPureSprite(data->itemId, data->itemType, sg_specialBattleLayer);
        CCSprite *borderIcon = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");
        
        headIcon->addChild(borderIcon, 1);
        borderIcon->setPosition(ccpAdd(ccp(headIcon->getContentSize().width / 2, headIcon->getContentSize().height / 2), ccp(0, 0)));
        
        CCSprite *headIcon1 = SGDrawCards::createPureSprite(data->itemId, data->itemType, sg_specialBattleLayer);
        CCSprite *borderIcon1 = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");
        
        headIcon1->addChild(borderIcon1, 1);
        borderIcon1->setPosition(ccpAdd(ccp(headIcon1->getContentSize().width / 2, headIcon1->getContentSize().height / 2), ccp(0, 0)));
        
        
        //章节的按钮
        SGButton *chapButton = SGButton::createFromSprite(headIcon, headIcon1, this, menu_selector(SGSpecialBattleLayer::enterSpBattleSection));
        chapButton->setTag(buttonTag);
        
        //章节的名称 //250	197	68
        SGCCLabelTTF *chapNameTTF = SGCCLabelTTF::create(data->chapName.c_str(), FONT_PANGWA, 24);
        chapButton->addChild(chapNameTTF, 2);
        chapNameTTF->setInsideColor(ccc3(250, 197, 68));
        chapNameTTF->setPosition(ccpAdd(ccp(chapNameTTF->getContentSize().width / 2, chapNameTTF->getContentSize().height / 3.5), ccp(0, 0)));
        
        //如果没有开启，蒙灰
        if (!data->isOpen)
        {
            CCSprite *greyMask = CCSprite::createWithSpriteFrameName("sp_battle_entry_mask.png");
            greyMask->setOpacity(150);
            chapButton->addChild(greyMask, 10);
            greyMask->setPosition(ccpAdd(ccp(greyMask->getContentSize().width / 2 - 5, greyMask->getContentSize().height / 2 - 5), ccp(0, 0)));
            //未开启，不可用
            chapButton->setEnabled(false);
        }
        
        CCMenu *menu = CCMenu::create();
        menu->setTouchPriority(10);
        menu->setPosition(CCPointZero);
        menu->addChild(chapButton);
        
        layerColor->addChild(menu);
        chapButton->setPosition(ccpAdd(ccp(0, 0), ccp(layerColor->getContentSize().width / 2, layerColor->getContentSize().height / 1.5)));
        
        
        //显示描述的背景
        CCSprite *descBg = CCSprite::createWithSpriteFrameName("sp_battle_infobg.png");
        layerColor->addChild(descBg, 1);
        descBg->setPosition(ccpAdd(chapButton->getPosition(), ccp(0, -descBg->getContentSize().height / 2 - chapButton->getContentSize().height / 1.7)));
        
        //下方对应的描述
        
        std::string desc;
        if (data->isOpen)
        {
            desc = data->chapOpenDesc;
            //如果是开启的，加上光效
            ResourceManager::sharedInstance()->bindTexture("animationFile/ms_niubility.plist", RES_TYPE_LAYER_UI, sg_specialBattleLayer);
            CCSpriterX *newRewardEffect = CCSpriterX::create("animationFile/ms_niubility.scml", true, true);
            layerColor->addChild(newRewardEffect, -1);
            newRewardEffect->setanimaID(0);
            newRewardEffect->setisloop(true);
            newRewardEffect->setScale(1.15);
            newRewardEffect->setPosition(ccpAdd(chapButton->getPosition(), ccp(0, 0)));
            newRewardEffect->play();
        }
        else
        {
            desc = data->chapClosedDesc;
        }
        
        SGCCLabelTTF *chapDescTTF = SGCCLabelTTF::create(desc.c_str(), FONT_PANGWA, 21);
        descBg->addChild(chapDescTTF, 1);
        chapDescTTF->setPosition(ccpAdd(ccp(descBg->getContentSize().width / 2, descBg->getContentSize().height / 2), ccp(0, 0)));
        
        //如果开启状态，显示剩余次数
        if (data->isOpen)
        {
            SGCCLabelTTF *residueTimeTTF = SGCCLabelTTF::create(CCString::createWithFormat(str_SpecailBattleResidueCoint, data->freePlayTimes)->getCString(), FONT_PANGWA, 20);
            residueTimeTTF->setInsideColor(ccGREEN);
            layerColor->addChild(residueTimeTTF, 1);
            residueTimeTTF->setPosition(ccpAdd(descBg->getPosition(), ccp(0, -residueTimeTTF->getContentSize().height * 1.2)));
        }
    }
    else
    {
        //没有的，直接问号显示
        SGButton *chapButton = SGButton::create("sp_battle_unlock.png", "equipPinzhi_5_0.png", this, NULL);
        chapButton->setEnabled(false);
        layerColor->addChild(chapButton);
        chapButton->setPosition(ccpAdd(ccp(0, 0), ccp(layerColor->getContentSize().width / 2, layerColor->getContentSize().height / 1.5)));
    }
    
    return layerColor;
}


void SGSpecialBattleLayer::initView()
{
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/sp_battle_space_bg.plist", RES_TYPE_LAYER_UI, sg_specialBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_specialBattleLayer);
    
    //上半部分的花边
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 0));
    gonggao_bg_3_l->setFlipY(true);
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-65)));
    this->addChild(gonggao_bg_3_l,13);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(gonggao_bg_3_l->getPosition(), ccp(gonggao_bg_3_l->getContentSize().width + titlecenter->getContentSize().width / 2,7)));
    this->addChild(titlecenter,11);
    titlecenter->setScaleX(4);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setFlipY(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 0));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-65)));
    this->addChild(gonggao_bg_3_r,13);
    
    //下半部分的花边
    CCSprite *gonggao_bg_3_bl = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_bl->setAnchorPoint(ccp(0, 0));
    gonggao_bg_3_bl->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(0,100)));
    this->addChild(gonggao_bg_3_bl,13);
    
    CCSprite *titleBottomcenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titleBottomcenter->setAnchorPoint(ccp(0.5, 0));
    titleBottomcenter->setPosition(ccpAdd(gonggao_bg_3_bl->getPosition(), ccp(gonggao_bg_3_bl->getContentSize().width + titleBottomcenter->getContentSize().width / 2,0)));
    this->addChild(titleBottomcenter,11);
    titleBottomcenter->setScaleX(4);
    
    CCSprite *gonggao_bg_3_br = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_br->setFlipX(true);
    gonggao_bg_3_br->setAnchorPoint(ccp(1, 0));
    gonggao_bg_3_br->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0,100)));
    this->addChild(gonggao_bg_3_br,13);
    
    
    makeDoubleBg(0, 0, 98);
    CCNode *bg1 = this->getChildByTag(98);
    CCSprite *sp1 = (CCSprite*)bg1;
    makeDoubleBg(sp1->getContentSize().width, 0, 99);
    
    
    //中间的星座图案
    CCSprite *middlePic = CCSprite::createWithSpriteFrameName("sp_battle_starline.png");
    this->addChild(middlePic, 13);
    
    float scale = CCDirector::sharedDirector()->getWinSize().width / 640.0f;
    middlePic->setPosition(ccpAdd(sp1->getPosition(), ccp((sp1->getContentSize().width * scale) / 2, 0)));
    
    //商城按钮
    SGButton *mallBtn = SGButton::create("sp_battle_mall.png", NULL, this, menu_selector(SGSpecialBattleLayer::mallEntry));
    this->addBtn(mallBtn);
    mallBtn->setPosition(ccpAdd(sp1->getPosition(), ccp(mallBtn->getContentSize().width, -sp1->getContentSize().height / 2 + mallBtn->getContentSize().height * 0.8)));
    
    //预加载一个底色的图，为了在点击的时候显示
    CCSprite *mallBg = CCSprite::createWithSpriteFrameName("sp_battle_mall_bg.png");
    this->addChild(mallBg, mallBtn->getZOrder() - 1, 100);
    mallBg->setPosition(ccpAdd(mallBtn->getPosition(), ccp(0, 0)));
    mallBg->setVisible(false);

    //规则说明按钮
    SGButton *tips = SGButton::create("battle_rules.png", NULL, this, menu_selector(SGSpecialBattleLayer::tipsInfo));
    this->addBtn(tips);
    tips->setPosition(ccpAdd(gonggao_bg_3_r->getPosition(), ccp(-tips->getContentSize().width * 0.8, -tips->getContentSize().height * 1.6)));
    
    //北斗的位置
    CCPoint starPosIphone4[10] = {ccp(70, 690), ccp(280, 720), ccp(350, 540), ccp(90, 440), ccp(140, 270), ccp(290, 140), ccp(460, 100)};
    
    for (int i = 0; i < 7; i++)
    {
        CCLayerColor *color = NULL;
        if (i < chapDatas->count())
        {
            color = this->createChapButton(i, static_cast<specialBattleData *>(chapDatas->objectAtIndex(i)));
        }
        else
        {
            color = this->createChapButton(i, NULL);
        }
        
        this->addChild(color, 20, i);
        if (CCDirector::sharedDirector()->getWinSize().height == 1136)
        {
            color->setPosition(ccpAdd(starPosIphone4[i], ccp(0, 100)));
        }
        else if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height)
        {
            color->setPosition(ccpAdd(starPosIphone4[i], ccp(60, 40)));
        }
        else
        {
            color->setPosition(starPosIphone4[i]);
        }
    }
    
    schedule(schedule_selector(SGSpecialBattleLayer::moveDoubleBg), 0.1);
    
    //随机闪烁星星
    flashStarOnSpace();
}



//星空上加入随机闪烁的星星
void SGSpecialBattleLayer::flashStarOnSpace()
{
    
    ResourceManager::sharedInstance()->bindTexture("animationFile/flashStar.plist", RES_TYPE_LAYER_UI, sg_specialBattleLayer);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    for (int i = 0; i < 3; i++)
    {
        //生成随机位置
        int posX = CCRANDOM_0_1() * (winSize.width - 30) + 15;
        int posY = CCRANDOM_0_1() * (winSize.width - 200) + 160;
        
        
        CCSpriterX *star = CCSpriterX::create("animationFile/lanxing.scml", true, true);
        this->addChild(star, 11);
        star->setanimaID(0);
        star->setisloop(true);
        star->setPosition(ccpAdd(ccp(posX, posY), ccp(0, 0)));
        star->play();
    }
}


//规则说明窗口
void SGSpecialBattleLayer::tipsInfo(CCObject *obj)
{
    CCLOG("pop Window");
    SpRuleInfoBox *rBox = SpRuleInfoBox::create();
    SGMainManager::shareMain()->showBox(rBox);
}


void SGSpecialBattleLayer::mallEntry(CCObject *obj)
{
    CCLOG("Mall Entry");
    //如果点击的了商城的按钮，直接显示底色 100即是对应的商城的底色背景
    static_cast<CCSprite *>(this->getChildByTag(100))->setVisible(true);
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_SPECIAL_BATTLE , SPECIAL_BATTLE_LAYER);
}




























