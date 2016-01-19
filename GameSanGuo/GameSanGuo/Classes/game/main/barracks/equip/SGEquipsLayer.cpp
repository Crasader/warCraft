//
//  SGEquipsLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#include "SGEquipsLayer.h"
#include "SGCardItem.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGOfficerCard.h"
#include "SGExchangeEquipBox.h"
#include "SGGeneralEquipBox.h"
#include "SGStaticDataManager.h"
#include "SGSkillInfoBox.h"
#include "SGCantAdvanceBox.h"
#include "SimpleAudioEngine.h"
#include"SGFirstStrengBox.h"
#include "SGFirstAdvanceBox.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "DayTaskModule.pb.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGCommonInfoBox.h"
#include "RoleShopModule.pb.h"

SGEquipsLayer::SGEquipsLayer()
:officerCard(NULL),
equipType(0),
swithType(0),
entertype(0),
range(NULL),
x_(0),
canUP(NULL),
equipCards(NULL),
buttonHaveUpGerneral(NULL),
m_pDrawsLabel(NULL),
_sortData(NULL),
_allData(NULL),
pageLabel(NULL)
{
    _allData = NULL;
    _sortData = NULL;
    pageLabel = NULL;
    _nowPage = 0;
    _totalPage = 0;
    for (int i=0;i<6;i++) {
        stars[i]=0;
    }

}
SGEquipsLayer::~SGEquipsLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this,MSG_REWARD_GODENTER);

    CC_SAFE_RELEASE(_allData);
    CC_SAFE_RELEASE(_sortData);
    CC_SAFE_RELEASE(datas);
	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_equipsLayer);

}
SGEquipsLayer *SGEquipsLayer::create(SGOfficerCard *card,int equiptype,int type,int x,int currentPage,int currentRow)
{
    SGEquipsLayer *equipsLayer  = new SGEquipsLayer();
    equipsLayer->equipType = equiptype;
    equipsLayer->x_=x;
    
    
    equipsLayer->_nowPage=currentPage-1;
    if (equipsLayer && equipsLayer->init(NULL, sg_equipsLayer))
    {
        
        equipsLayer->officerCard = card;
        equipsLayer->entertype = type;
        equipsLayer->initView();        
        equipsLayer->autorelease();
        return equipsLayer;
    }
    CC_SAFE_DELETE(equipsLayer);
    return NULL;
}
void SGEquipsLayer::initDatas(int type)
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();

    notification->addObserver(MSG_REWARD_GODENTER,
                              this,
                              callfuncO_selector(SGEquipsLayer::onJiTianMsg) );

    _allData = CCArray::create();
    _allData->retain();
    
    CCArray *equips = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
    CCObject *obj = NULL;
    
    for(int i=0;i<equips->count();i++)//统计素材每个星级的数量
    {
        SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)equips->objectAtIndex(i);
//        CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
        
//        if (str_->isEqual(CCString::create("SGEquipCard")))
//        {
            SGEquipCard *card = (SGEquipCard *)baseMilitaryCard;
            
            if (card->getOfficerCardId() == 0 &&card->getIsProt() == 0)
            {
                int s = card->getCurrStar()-1;
                stars[s]+=1;
            }
//        }
    }

    if (entertype == 1 ) {
        equipCards = CCArray::create();
        for (int i = 0; i <equips->count(); i++) {
            _allData->addObject(equips->objectAtIndex(i));
        }
    } else if (entertype ==0) {
        if(equipType==0){
            for (int i = 0; i <equips->count(); i++) {
                _allData->addObject(equips->objectAtIndex(i));
            }
            CCArray *cards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
        
            CCObject *obj_ = NULL;
            CCARRAY_FOREACH(cards, obj_)
            {
                SGPropsCard *card = (SGPropsCard *)obj_;
                if (card && card->getType() == 2)
                {
                    _allData->addObject(card);
                }
            }
        }else
        {
            CCARRAY_FOREACH(equips, obj)
            {
               SGEquipCard *card = (SGEquipCard*)obj;
                if (card->getItemType() == equipType)
                {
                    _allData->addObject(card);
                }
            }
        }
    }
    else if (entertype == 2)
    {
        CCARRAY_FOREACH(equips, obj)
        {
            SGEquipCard *card = (SGEquipCard*)obj;
            if (card->getMaxStar())
            {
                _allData->addObject(card);
            }
        }

    }
    else
    {
//        CCARRAY_FOREACH(equips, obj)
//        {
//            SGEquipCard *card = (SGEquipCard*)obj;
//            if (card->getItemType() == equipType)
//            {
//                _allData->addObject(card);
//            }
//        }
        for(int i = 0; i < equips->count(); i++)
        {
            SGEquipCard *card = (SGEquipCard*)equips->objectAtIndex(i);
            if (card->getItemType() == equipType)
            {
                _allData->addObject(card);
            }

        }
//        if (_allData->count() == 0)
//        {
//            SGCCLabelTTF *notice = SGCCLabelTTF::create(str_Equipslayer_str7, FONT_BOXINFO, FONTSIZE(14));
//            this->addChild(notice);
//            notice->setPosition(SGLayout::getPoint(kMiddleCenter));
//            SGButton *Btn = SGButton::create("box_btnbg.png",
//                                             "public_font_fanhui.png",
//                                             this,
//                                             menu_selector(SGEquipsLayer::backHandler),
//                                             ccp(0, 0),
//                                             false,
//                                             true);
//            this->addBtn(Btn);
//            Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 300)));
//        }
    }
    
    std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("equipsort");
    CCString *stype = CCString::create(str);
    SGSortBox::sortCards(_allData, stype->intValue(),1);
    
    datas = CCArray::create();
    datas->retain();
    
    _sortData = CCArray::create();
    _sortData->retain();
    
    //_nowPage = 1;
    _totalPage = ceil(_allData->count()/15.0) ;
    
    int size = _allData->count();
    size > 15 && (size = 15);
    for (int ii = 0; ii < size; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
}

void SGEquipsLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_equipsLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_equipsLayer);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_equipsLayer);
    SGCardItem::addCardItemRes(sg_equipsLayer);//添加item资源
 
//    tableViewColumns = 5;
    tableViewHeight = 120;
    
    this->initDatas();

    CCSize s= CCDirector::sharedDirector()->getWinSize();
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();

    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - 230)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGEquipsLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    backBtn->setScale(1.02);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGEquipsLayer::buttonClickSort),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 1));
    range->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0, titleBg->getPosition().y)));

    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - 230 - titleBg->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-25);
    tableView->setShowSlider(false);
    
    //设置左右箭头
    float posY = tableView->getContentSize().height*.5f + tableView->getPositionY();
    CCSprite *spriteL = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    float widLR = spriteL->getContentSize().width*.5f;
    float hitLR = spriteL->getContentSize().height*.5f;
    spriteL->setPosition(ccp(widLR, posY-hitLR));
    spriteL->setTag(111);
    spriteL->setVisible(false);
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
    
    //设置页数
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    pagecountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
    
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, 0, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(-pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
    
    
	//批量卖出按钮
	SGButton *sellButton = SGButton::createFromLocal("store_exchangebtnbg.png", str_sale, this, menu_selector(SGEquipsLayer::batchSell),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
	sellButton->setAnchorPoint(ccp(0, 0));
	sellButton->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
    sellButton->setVisible(false);
	this->addBtn(sellButton);
    if(equipType==0 && entertype==0)
        sellButton->setVisible(true);
	
    //前往购买按钮 小于5件装备
	SGButton *buyBtn = NULL;
    if( equipType!=11 )
    {
        buyBtn = SGButton::createFromLocal("an.png", str_Equipslayer_str13, this, menu_selector(SGEquipsLayer::buyBtnClicked),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
        buyBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 300)));
        this->addBtn(buyBtn);
    }
    //飾品
    else
    {
        buyBtn = SGButton::createFromLocal("an.png", str_Equipslayer_str14, this, menu_selector(SGEquipsLayer::gotoJiTian),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
        buyBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 300) ));
        this->addBtn(buyBtn);
    }
	
    if(_allData->count() >= 5)
        buyBtn->setVisible(false);
    buyBtn->setVisible(false);
	
    if (officerCard == NULL) {
        CCSprite *title = NULL;
       
        buttonHaveUpGerneral= SGButton::createFromLocal("public_tishi_bg.png", "", this, menu_selector(SGEquipsLayer::buttonClickCanUpStar),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
        /////////////////////
        // 您总共有       件装备符合转生要求
        SGCCLabelTTF *kengdieaa = SGCCLabelTTF::create(str_Equipslayer_str8, FONT_PANGWA, 32 , ccWHITE ,ccc3(0x92, 0x2a, 0x9d) );
        //modify by:zyc  merge into create.
        //kengdieaa->setOutSideColor((ccc3(0x92, 0x2a, 0x9d)));
        kengdieaa->setAnchorPoint(ccp(0, 0.5));
        kengdieaa->setPosition( ccpAdd(buttonHaveUpGerneral->getPosition(), ccp(buttonHaveUpGerneral->getContentSize().width*0.1, buttonHaveUpGerneral->getContentSize().height*0.5)));
        
        buttonHaveUpGerneral->addChild(kengdieaa);
        ///////////////////
        buttonHaveUpGerneral->setAnchorPoint(ccp(0.5, 1));
        buttonHaveUpGerneral->setPosition(ccpAdd(titleBg->getPosition(), ccp(0,-titleBg->getContentSize().height*1.1)));
        
        
        //显示几个可进阶武将
        m_pDrawsLabel = CCLabelTTF::create("10", "Arial", 25);
        m_pDrawsLabel->setColor(ccRED);
        m_pDrawsLabel->setPosition(ccpAdd(buttonHaveUpGerneral->getPosition(), ccp(-150, -buttonHaveUpGerneral->getContentSize().height/2)));
        this->addChild(m_pDrawsLabel, 10);
//        this->initSomeProcess();
        CCSize s= CCDirector::sharedDirector()->getWinSize();
        m_pDrawsLabel->setVisible(false);
        buttonHaveUpGerneral->setVisible(false);

        
        if (entertype ==2) {
            buttonHaveUpGerneral->setVisible(false);
            m_pDrawsLabel->setVisible(false);
            title = CCSprite::createWithSpriteFrameName("font_yzsngzb.png");
        }else  if (entertype == 1) {
            buttonHaveUpGerneral->setVisible(false);
            m_pDrawsLabel->setVisible(false);
            title = CCSprite::createWithSpriteFrameName("font_yqhngzb.png");
        }else{
            title = CCSprite::createWithSpriteFrameName("font_yydzbk.png");
        }
        title->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
        this->addChild(title);
        
        if (datas->count() == 0) {
            SGCCLabelTTF *nocard = SGCCLabelTTF::create(str_Equipslayer_str9, FONT_BOXINFO, 32);
            this->addChild(nocard,10);
            nocard->setPosition(SGLayout::getPoint(kMiddleCenter));
        }

        
    }
    else{
        CCLabelTTF *font2 = CCLabelTTF::create(str_Equipslayer_str10, FONT_BOXINFO, FONTSIZE(13));
        font2->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
        this->addChild(font2);
        
        if (datas->count() == 0) {
//            SGCCLabelTTF *nocard = SGCCLabelTTF::create("您没有此类装备，可以在闯关模式或探访中获得。", FONT_BOXINFO, 32);
//            this->addChild(nocard,10);
//            nocard->setPosition(SGLayout::getPoint(kMiddleCenter));
        }


    }
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_equipsLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height );
    //bg->setScaleY(s.height / bg->getContentSize().height);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-2, b));
    SGSortBox::updateBtn(range,2);
    bg->setVisible(false);  
    
    
    if (_allData->count()>0) {
        refreshPage(1);
    }
    
}
//批量卖出
void SGEquipsLayer::batchSell()
{
	CCLOG("Equip Batch Sell");
	SGMainManager::shareMain()->showBatchSellLayer(7);
}
SNSTableViewCellItem *SGEquipsLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    
        if (NULL == item)
        {
            item = SGCardItem::create(false,0,this,tableView);
        }
        if (num<datas->count())
        {
            
            SGBaseMilitaryCard * card = ((SGBaseMilitaryCard *)datas->objectAtIndex(num));

            
            CCString *str_=CCString::create(card->getClassName());
            
            if (str_->isEqual(CCString::create("SGEquipCard"))) {
                int usednum = 0;
                int star = card->getCurrStar()-1;
                bool po = !(((SGEquipCard *)card)->getOfficerCardId());
               // CCLOG("%d",po);
                usednum = stars[star]-(int)po;
                //            int s = card->getCurrStar()-1;
                //            int po =((SGEquipCard *)card)->getOfficerCardId();
                item ->updataCard((SGEquipCard *)datas->objectAtIndex(num),cardType_equip,true,entertype == 2,usednum>=3);
            }else
            {
                int usednum = 1;
                item ->updataCard((SGBaseMilitaryCard *)datas->objectAtIndex(num),cardType_equip,true,entertype == 2,usednum>=3);
            }
            
        }
        else
        {
            item->setVisible(false);
        }
    return item;
}
//////////
void SGEquipsLayer::itemselect(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(index);
    if (officerCard == NULL) {
        CCString *str_ = CCString::create(card->getClassName());
        if (str_->isEqual(CCString::create("SGEquipCard")))
        {
            if (entertype ==2) {
                advanceHandler((SGEquipCard *)card);
            }else  if (entertype == 1) {
                strengHandler((SGEquipCard *)card);
            }else
            {
                if(_nowPage==1) {
                    _nowPage=1000;
                    
                }
                SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)card,0,true,NULL,false, NULL,_nowPage ,index);
                
            }
        }
        else
        {
            SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)card,3);
        }
    }else
    {
        officerCard->retain();
//        SGGeneralEquipBox *swearBox = SGGeneralEquipBox::create(this, officerCard,((SGEquipCard *)card),2,entertype,x_);
//        SGMainManager::shareMain()->showBox(swearBox);
//            create(SGBaseMilitaryCard* main, SGBaseMilitaryCard* sub = NULL, SGCibOpenType otype = SGCIBOT_PROTO_ITEM, int myEnterType = -1, int otherEnterType = -1, std::string extArg = "", SGCommonInfoBoxDelegate* deg = NULL, SGBoxDelegate* boxDeg = NULL);
        SGCommonInfoBox* cib = SGCommonInfoBox::create(this, card, officerCard, SGCIBOT_MY_EQUIP_UNWEAR, entertype, x_);
        SGMainManager::shareMain()->showBox(cib);
        officerCard->autorelease();
    }

}
//////////
void SGEquipsLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
//    SGBaseMilitaryCard *card = (SGBaseMilitaryCard*)datas->objectAtIndex(index);
//    if (officerCard == NULL) {
//        CCString *str_ = CCString::create(card->getClassName());
//        if (str_->isEqual(CCString::create("SGEquipCard")))
//        {
//            if (entertype ==2) {
//                advanceHandler((SGEquipCard *)card);
//            }else  if (entertype == 1) {
//                strengHandler((SGEquipCard *)card);
//            }else
//            {
//                if(_nowPage==1) {
//                    _nowPage=1000;
//                    
//                }
//                SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)card,0,true,NULL,false, NULL,_nowPage ,index);
//
//            }
//        }
//        else
//        {
//            SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)card,3);
//        }
//    }else
//    {
//        officerCard->retain();
//            SGGeneralEquipBox *swearBox = SGGeneralEquipBox::create(this, officerCard,((SGEquipCard *)card),2,entertype);
//            SGMainManager::shareMain()->showBox(swearBox);
//        officerCard->autorelease();
//    }
}

void SGEquipsLayer::refreshPage(int type)
{
//    DDLog("ccc===================%d", type);
    if(0 ==_allData->count())
        return ;
    
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

//排序回调函数
void SGEquipsLayer::sortLayerCallBack(CCArray *cardArray)
{
    _allData = cardArray;
    _nowPage = 0;
    this->refreshPage(1);
    
    
//    this->datas = cardArray;
//    this->tableView->reloadData();
    SGSortBox::updateBtn(range,2);
}

void SGEquipsLayer::strengHandler(SGEquipCard *card)
{
    
    int type = SGStaticDataManager::shareStatic()->getStrengEquipType(card->getItemId());
    
//    SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(card->getItemId());
    
//    if (equip->getEquipExpType()->isEqual(CCString::create("A"))) {
//        type = 0;
//    }
//    else if (equip->getEquipExpType()->isEqual(CCString::create("B"))) {
//        type = 1;
//    }
//    else if (equip->getEquipExpType()->isEqual(CCString::create("C"))) {
//        type = 2;
//    }
//    else if (equip->getEquipExpType()->isEqual(CCString::create("D"))) {
//        type = 3;
//    }
    
    SGExpDataModel *exp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type,card->getCurrLevel());
    
    //if (card->getCurrExp() == exp->getExpValue() && card->getMaxExp() == -1)
    if (card->getCurrExp() == card->getMaxExp() && card->getMaxExp() >= exp->getExpValue())
    {
        SGMainManager::shareMain()->showMessage(str_Equipslayer_str3);
        
    }else{
        if (_nowPage==1) {
            _nowPage=1000;
        }
        SGMainManager::shareMain()->showStrengLayer(card, 2,true,false,_nowPage);
    }
}
void SGEquipsLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
//entype 0 正常进入  15 布阵进入   2 首页进入
    if (swithType == 1) {
//        datas = equipCards;
//        tableView->reloadData();
        CCNode *node1 = this->getChildByTag(110);
        CCNode *node2 = this->getChildByTag(111);
        CCNode *node3 = this->getChildByTag(112);
        node1->setVisible(true);
        node2->setVisible(true);
        node3->setVisible(true);
        pageLabel->setVisible(true);
        
        datas->removeAllObjects();
        for (int ii = 0; ii < _sortData->count(); ii++) {
            CCObject *obj = _sortData->objectAtIndex(ii);
            datas->addObject(obj);
        }
        tableView->setPageNEnable(true);
        tableView->reloadData();
        
        m_pDrawsLabel->setVisible(true);
        buttonHaveUpGerneral->setVisible(true);
        swithType = 0;
    }else{
    if (officerCard == NULL)
    {
        if (entertype == 1) {
            if(x_==5)
            {
                main::DayTaskListRequest *request = new main::DayTaskListRequest();
                SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_LIST, request);

            }
           // SGMainManager::shareMain()->showFirstLayer();
            
            else
            {
            SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
            SGMainManager::shareMain()->showBox(strengBox);
            }
        
        }
        
        else if (entertype == 2 )
        {
            SGFirstAdvanceBox *strengBox = SGFirstAdvanceBox::create(this);
            SGMainManager::shareMain()->showBox(strengBox);
            
//            SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
//            SGMainManager::shareMain()->showBox(strengBox);
        }
        else{
        SGMainManager::shareMain()->showBarrackslayer();
        }
    }
    else{
        if(x_==11||x_==12)
        SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,entertype,NULL,false,NULL,x_);
        else
            SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,entertype);
    }
    }
//this->removeFromParent();
}

void SGEquipsLayer::buttonClickSort()
{
//    SGSortLayer *sortLayer = SGSortLayer::create(datas, this, this, ccpAdd(range->getPosition(), ccp(-range->getContentSize().width/2, -range->getContentSize().height/2)));
//    SGMainManager::shareMain()->addShowLayer(sortLayer);
    SGSortBox *Box = SGSortBox::create(this, this, _allData, 1);
    SGMainManager::shareMain()->showBox(Box);
}
void SGEquipsLayer::buttonClickCanUpStar()
{
    EFFECT_PLAY(MUSIC_BTN);
    _sortData->removeAllObjects();
    for (int ii = 0; ii<datas->count(); ii++) {
        CCObject *obj = datas->objectAtIndex(ii);
        _sortData->addObject(obj);
    }
    
    datas->removeAllObjects();
    for (int ii = 0; ii<canUP->count(); ii++) {
        CCObject *obj = canUP->objectAtIndex(ii);
        datas->addObject(obj);
    }
    
    // 翻页效果设置为false
    CCNode *node1 = this->getChildByTag(110);
    CCNode *node2 = this->getChildByTag(111);
    CCNode *node3 = this->getChildByTag(112);
    node1->setVisible(false);
    node2->setVisible(false);
    node3->setVisible(false);
    pageLabel->setVisible(false);
    tableView->setPageNEnable(false);
    
    swithType = 1;
    m_pDrawsLabel->setVisible(false);
    buttonHaveUpGerneral->setVisible(false);
//    datas = canUP;
    tableView->reloadData();
}
//处理一些初始化逻辑
void SGEquipsLayer::initSomeProcess()
{
//    CCSize s= CCDirector::sharedDirector()->getWinSize();
//    float h = SGMainManager::shareMain()->getHeadHeight();
//    float b = SGMainManager::shareMain()->getBottomHeight();
//    //判断是否有武将需要进阶
//    int upCount = this->getCanUpStarGeneralCount();
//    if(upCount)
//    {
//        tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - 120));
//        tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
//        CCString *upCountString = CCString::createWithFormat("%d",upCount);
//        m_pDrawsLabel->setString(upCountString->getCString());
//    }else{
//        m_pDrawsLabel->setVisible(false);
//        buttonHaveUpGerneral->setVisible(false);
//    }
}

//计算当前有多少武将可以进阶
int SGEquipsLayer::getCanUpStarGeneralCount()
{
    int upStarCount = 0;
    canUP = CCArray::create();
    canUP->retain();
  
    for(int i=0;i<_allData->count();i++)
    {
        SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)_allData->objectAtIndex(i);
        CCString *str_ = CCString::create(baseMilitaryCard->getClassName());
        if (str_->isEqual(CCString::create("SGEquipCard")))
        {
            int s = baseMilitaryCard->getCurrStar()-1;
            int po =((SGEquipCard *)baseMilitaryCard)->getOfficerCardId();
            if(baseMilitaryCard->getCurrLevel() == baseMilitaryCard->getUpgradelevel() && baseMilitaryCard->getMaxStar() && (stars[s]>3 || (stars[s]==3 && po!=0)))
      
            {
                canUP->addObject(baseMilitaryCard);
                upStarCount ++;
            }
        }
    }
    
    return upStarCount;
}

void SGEquipsLayer::advanceHandler(SGEquipCard *card)
{
//    if (card->getMaxStar() == 0)
//    {
//        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, card,18,NULL);
//        SGMainManager::shareMain()->showBox(cantadvanceBox);
//    }
//    int s = card->getCurrStar()-1;
//    if (card->getCurrLevel() <card->getUpgradelevel())
//    {
//        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, card,103,NULL);
//        SGMainManager::shareMain()->showBox(cantadvanceBox);
//    }
//    else if (stars[s]<=3)
//    {
//        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, card,104,NULL);
//        SGMainManager::shareMain()->showBox(cantadvanceBox);
//        
//    }
//
//    else    {
        SGMainManager::shareMain()->showAdvanceLayer(card,2,true);
//    }
    
}

void SGEquipsLayer::buyBtnClicked()
{
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_YUN_YOU , SHOP_LAYER);
}

void SGEquipsLayer::gotoJiTian()
{
    main::RewardGodPrayEntryRequest *request = new main::RewardGodPrayEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GODENTER, request);
}

void SGEquipsLayer::onJiTianMsg(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::RewardGodPrayEntryResponse *response = (main::RewardGodPrayEntryResponse *)sr->m_msg;
        int current = response->current();
        int max = response->max();
        int iscan = response->cando();
        /////////加入可能获得物品的信息们
        CCArray*array=CCArray::create();
        for (int i=0; i<response->godrewardinfo_size(); i++) {
            
            main::GodRewardInfo info =response->godrewardinfo(i);
            
            SGMailSystemAccessoryDataModel* data=new SGMailSystemAccessoryDataModel();
            data->setCount(info.count());
            data->setItemId(info.itemid());
            data->setType(info.type());
            
            
            array->addObject(data);
        }
        
        //////////////////////////////
        
        
        
        
        
        
        SGMainManager::shareMain()->showJitianLayer(current,max,iscan,array);
    }
}