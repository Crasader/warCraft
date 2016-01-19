//
//  SGStrengSkillCardLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#include "SGSkillCardsLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGPropsCard.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGSkillCardsLayer::SGSkillCardsLayer():pageLabel(NULL)
{
}
SGSkillCardsLayer::~SGSkillCardsLayer()
{
 
    CCLOG("~SGSkillCardsLayer");
     	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_skillCardsLayer);

}

SGSkillCardsLayer* SGSkillCardsLayer::create(int type)
{
    SGSkillCardsLayer *Layer = new SGSkillCardsLayer();
    
    if (Layer && Layer->init(NULL, sg_skillCardsLayer))
    {
        Layer->initView(type);
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return Layer;
}

void SGSkillCardsLayer::initView(int type)
{
   
    ResourceManager::sharedInstance()->bindTexture("sgskillcardslayer/sgskillcardslayer.plist", RES_TYPE_LAYER_UI, sg_skillCardsLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_skillCardsLayer);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_skillCardsLayer);
    SGCardItem::addCardItemRes(sg_skillCardsLayer);//添加item资源
    
    CCArray *propCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    datas = CCArray::create();
    datas->retain();
    _allData = CCArray::create();
    _allData->retain();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(propCards, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card->getType() == 4 || card->getType()==5) {
            _allData->addObject(card);
        }
    }
    int last = _allData->count();
    last > 15 && (last = 15);
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
    _nowPage = 1;
    _totalPage = _allData->count()/15 + 1;
    
    if (datas->count() == 0) {
        SGCCLabelTTF *nocard = SGCCLabelTTF::create(str_Equipslayer_str9, FONT_BOXINFO, 32);
        this->addChild(nocard,10);
        nocard->setPosition(SGLayout::getPoint(kMiddleCenter));
    }

    tableViewHeight = 120;
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h )));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSkillCardsLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,true);
    backBtn->setScale(1.01);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    CCSprite *font2 = CCSprite::createWithSpriteFrameName("font_yydjnsck.png");
    font2->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(font2);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_skillCardsLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width,s.height -h - b - titleBg->getContentSize().height);
    
    bg->setTextureRect(r);
    //bg->setScaleY(r.size.height / bg->getContentSize().height);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.02);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-2, b));
    
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h  - titleBg->getContentSize().height - 70));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b + 55)));
    tableView->setDown(-25);
    
    //设置左右箭头
    float posY = tableView->getContentSize().height*.5f + tableView->getPositionY();
    CCSprite *spriteL = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    float widLR = spriteL->getContentSize().width*.5f;
    float hitLR = spriteL->getContentSize().height*.5f;
    spriteL->setPosition(ccp(widLR, posY-hitLR));
    spriteL->setVisible(false);
    spriteL->setTag(111);
    this->addChild(spriteL);
    CCSprite *spriteR = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    spriteR->setFlipX(true);
    spriteR->setTag(112);
    spriteR->setVisible(false);
    spriteR->setPosition(ccp(s.width-widLR, posY-hitLR));
    this->addChild(spriteR);
    if (_totalPage > 1) {
        spriteL->setVisible(true);
        spriteR->setVisible(true);
        tableView->setPagegate(this);
        tableView->setPageNEnable(true);
        tableView->setShowSlider(false);
    }
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    pagecountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
    
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(-pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
    
	/*
    CCSprite *cardcountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    cardcountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(cardcountbg,40);
    cardcountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
//    int total = SGPlayerInfo::sharePlayerInfo()->getTotalCardsCount();
    SGCCLabelTTF *cardcounts = SGCCLabelTTF::create(CCString::createWithFormat("素材卡:%d",_allData->count())->getCString(), FONT_BOXINFO, 28);
    this->addChild(cardcounts,42);
    cardcounts->setColor(ccc3(0xff, 0xf0, 0x00));
    cardcounts->setPosition(ccpAdd(cardcountbg->getPosition(), ccp(-cardcountbg->getContentSize().width*.5f, cardcountbg->getContentSize().height*.5f)));
	*/
//	//批量卖出按钮
//	CCMenu *menu = CCMenu::create();
//	SGButton *sellButton = SGButton::createFromLocal("store_exchangebtnbg.png", str_sale, this, menu_selector(SGSkillCardsLayer::batchSell),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
//	sellButton->setAnchorPoint(ccp(0, 0));
//	sellButton->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
////	this->addBtn(sellButton);
//	menu->addChild(sellButton);
//	
//	this->addChild(menu, 10);
//	menu->setPosition(CCPointZero);
	
}
//批量卖出
void SGSkillCardsLayer::batchSell()
{
	CCLOG("SkillCard Batch Sell");
	SGMainManager::shareMain()->showBatchSellLayer(9);
}
void SGSkillCardsLayer::refreshPage(int type)
{
    //    DDLog("ccc===================%d", type);
    datas->removeAllObjects();
    int pageCount = 15; // 每页显示的个数
    
    if (type == 1) { // to next page
        if (_nowPage < _totalPage) {
            _nowPage++;
        }else { // _nowPage >= _totalPage
            _nowPage = 1;
        }
    }else if (type == 2) { // to pre page
        if (_nowPage > 1) {
            _nowPage--;
        }else {
            _nowPage = _totalPage;
        }
    }
    
    int last = pageCount;
    if (_nowPage == _totalPage) {
        last = _allData->count()-(_nowPage-1)*pageCount;
    }
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex((_nowPage-1)*pageCount+ii);
        datas->addObject(obj);
    }
    
    CCString *str = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    if(pageLabel) pageLabel->setString(str->getCString());
    tableView->reloadData();
}
SNSTableViewCellItem *SGSkillCardsLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

        if (NULL == item)
        {
            item = SGCardItem::create();
        }
        if (num<datas->count())
        {
            item ->updataCard((SGPropsCard *)datas->objectAtIndex(num),cardType_skill);
        }
    
    return item;
}

void SGSkillCardsLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
    
    if (card == NULL)
        return;
    
    //复杂的弹簧框逻辑由通用信息BOX替代。
    SGMainManager::shareMain()->showCommonInfoBoxInProto(card, this);
    return;
//    SGMainManager::shareMain()->showSkillCardInfoLayer((SGPropsCard *)card, 0);
}

void SGSkillCardsLayer::backHandler()
{
    SGMainManager::shareMain()->showBarrackslayer();
}