//
//  SGPropsLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#include "SGPropsLayer.h"
#include "SGCardItem.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGOfficerCard.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGPropsLayer::SGPropsLayer():
currentPage(0),
pageLabel(NULL)

{
    _totalPage = 0;
    _nowPage = 0;
    _allData = NULL;
    pageLabel = NULL;
}
SGPropsLayer::~SGPropsLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_propsLayer);
    CC_SAFE_RELEASE(_allData);
    datas->release();
}
SGPropsLayer *SGPropsLayer::create(int currentPage)
{
    SGPropsLayer *propsLayer  = new SGPropsLayer();
    if (propsLayer && propsLayer->init(NULL, sg_propsLayer))
    {
        propsLayer->_nowPage=currentPage-1;
        propsLayer->initView();
        propsLayer->autorelease();
        return propsLayer;
    }
    CC_SAFE_DELETE(propsLayer);
    return NULL;
}
void SGPropsLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist",RES_TYPE_LAYER_UI ,sg_propsLayer);
    SGCardItem::addCardItemRes(sg_propsLayer);//添加item资源

    
    tableViewHeight = 120;
    CCArray *propCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    datas = CCArray::create();
    datas->retain();
    _allData = CCArray::create();
    _allData->retain();
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(propCards, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card->getType() == 0) {
            _allData->addObject(card);
        }
    }
    int last = _allData->count();
    last > 15 && (last = 15);
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
    //_nowPage = 1;
    _totalPage = ceil(_allData->count()/15.0) ;
    
    if (datas->count() == 0) {
        SGCCLabelTTF *nocard = SGCCLabelTTF::create(str_Equipslayer_str9, FONT_BOXINFO, 32);
        this->addChild(nocard,10);
        nocard->setPosition(SGLayout::getPoint(kMiddleCenter));
    }

    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", "返回", this, menu_selector(SGPropsLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.05);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    CCSprite *font2 = CCSprite::createWithSpriteFrameName("font_yyddjk.png");
    font2->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(font2);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_propInfoLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(384- s.width/2, 1136/2 - (s.height - h - b - 80)/2, s.width, s.height - h - titleBg->getContentSize().height - b);
    //bg->cocos2d::CCNode::setScale(r.size.width / bg->getContentSize().width, r.size.height / bg->getContentSize().height);
    bg->setTextureRect(r);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, b));
    
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height-b - h - titleBg->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
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
    pagecountbg->setAnchorPoint(ccp(0, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
    
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , ccc3(0xff, 0xf0, 0x00));
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
    
    if (_allData->count()>0) {
        refreshPage(1);
    }
}
SNSTableViewCellItem *SGPropsLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (NULL == item)
    {
        item = SGCardItem::create();
    }
    if (num<datas->count())
    {
        item ->updataCard((SGPropsCard *)datas->objectAtIndex(num), cardType_prop);
    }
    else
    {
        item->setVisible(false);
    }
    
    return item;
}

//回调函数
void SGPropsLayer::sortLayerCallBack(CCArray *cardArray)
{
    this->datas = cardArray;
    this->tableView->reloadData();
}
void SGPropsLayer::refreshPage(int type)
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
void SGPropsLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGPropsCard *card = (SGPropsCard*)datas->objectAtIndex(index);
    SGMainManager::shareMain()->showPropInfoLayer(card,0,true,_nowPage);
}

void SGPropsLayer::buttonClickSort()
{
//    SGSortLayer *sortLayer = SGSortLayer::create(datas, this, this, ccpAdd(range->getPosition(), ccp(-range->getContentSize().width/2, -range->getContentSize().height/2)));
//    SGMainManager::shareMain()->addShowLayer(sortLayer);
}

void SGPropsLayer::backHandler()
{
    SGMainManager::shareMain()->showBarrackslayer();
}
