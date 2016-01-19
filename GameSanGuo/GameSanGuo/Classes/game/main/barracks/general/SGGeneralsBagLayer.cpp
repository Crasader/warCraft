
#include "SGGeneralsBagLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCardItem.h"
#include "SGSortBox.h"
#include "SGStaticDataManager.h"
#include "SGSkillInfoBox.h"
#include "SGCantAdvanceBox.h"
#include "SimpleAudioEngine.h"
#include "SGFirstStrengBox.h"
#include "SGGuideManager.h"
#include "SGMainScene.h"
#include "SGBaseLayer.h"
#include"SGFirstAdvanceBox.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "DayTaskModule.pb.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGStringConfig.h"
#include "Utils.h"
#include "SGPiecesBagLayer.h"

SGGeneralsBagLayer::SGGeneralsBagLayer()
:swichType(0),
_currentRow(0),
m_pDrawsLabel(NULL),
pageLabel(NULL),
range(NULL),
canUP(NULL),
_allData(NULL),
_sortData(NULL),
selectId(0),
enterType(0),
x_(0)
{
    _totalPage = 0;
    _nowPage = 0;
    _allData = NULL;
    _sortData = NULL;
    pageLabel = NULL;
   
}
SGGeneralsBagLayer::~SGGeneralsBagLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    CC_SAFE_RELEASE(_sortData);
    CC_SAFE_RELEASE(canUP);
    CC_SAFE_RELEASE(_allData);
    CC_SAFE_RELEASE(datas);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_generalsBagLayer);
}

/////////
SGGeneralsBagLayer* SGGeneralsBagLayer::create(int type,int x,int currentPage,int currentRow)
{
    
    SGGeneralsBagLayer *generalLayer = new SGGeneralsBagLayer();
    
    generalLayer->enterType = type;
    generalLayer->x_=x;
    generalLayer->_nowPage=currentPage-1;
    generalLayer->_currentRow   =currentRow;
    
    if (generalLayer && generalLayer->init(NULL, sg_generalsBagLayer))
    {
        generalLayer->initView();
        generalLayer->autorelease();
        return generalLayer;
    }
    CC_SAFE_DELETE(generalLayer);
    return generalLayer;
}

void SGGeneralsBagLayer::initMsg()
{
    SGCardItem::addCardItemRes(sg_generalsLayer);//添加item资源
//    tableViewColumns = 5;
    
    tableViewHeight = 120;
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();


    _allData = CCArray::create();
     _allData->retain();
    CCArray *array =CCArray::create();
    array->retain();
    array =SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    ;
    for (int i=0;i<6;i++) {
        stars[i]=0;
    }
    for(int i=0;i<array->count();i++)//统计素材每个星级的数量
    {
        SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)array->objectAtIndex(i);
        SGOfficerCard *card = (SGOfficerCard *)baseMilitaryCard;
        int po1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,card);
        int po2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,card);
        int po3 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,card);
        if ( po1 == 0 && po2 ==0 && po3 == 0 && card->getIsProt() == 0)
        {
            int s = card->getCurrStar()-1;
            stars[s]+=1;
        }

    }

   
    for(int i=0;i<array->count();i++)
    {
        SGOfficerCard *card = (SGOfficerCard *)array->objectAtIndex(i);
        int usednum = 0;
        
        if (card->getMaxStar())
        {
            if (card->getCurrStar()>=1)
            {
                
                CCArray* array =SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
                for(int i=0;i<array->count();i++)
                {
                    SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)array->objectAtIndex(i);
                    SGOfficerCard *card1 = (SGOfficerCard *)baseMilitaryCard;
                    int po1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,card1);
                    int po2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,card1);
                    int po3 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,card1);
                    if ( po1 == 0 && po2 ==0 && po3 == 0 && card1->getIsProt() == 0 && card1->getSsid()!=card->getSsid() && card1->getProtoId() ==((SGOfficerCard*)card)->getProtoId() && card->getCurrStar() == card1->getCurrStar())
                    {
                        usednum+=1;
                        if(usednum>3)
                            break;
                    }
                }
                
                
            }
            else
            {
                int star = card->getCurrStar()-1;
                bool po = !(SGTeamgroup::shareTeamgroupInfo()->isOnEmbattle((SGOfficerCard*)card));
                usednum = stars[star]-(int)po;
                
            }
            
            card->setAdvanceStuffNum(usednum);
        }
        _allData->addObject(array->objectAtIndex(i));
    }
   //    CCArray *offices = SGPlayerInfo::sharePlayerInfo()->getPropCards();
//    CCObject *obj_ = NULL;
//    CCARRAY_FOREACH(offices, obj_)
//    {
//        SGPropsCard *card = (SGPropsCard *)obj_;
//        if (card && card->getType() == 1)
//        {
//            _allData->addObject(card);
//        }
//    }
    
    datas = CCArray::create();
    datas->retain();

    _sortData = CCArray::create();
    _sortData->retain();
    
    _totalPage = ceil(_allData->count()/10.0);
    
}

void SGGeneralsBagLayer::refreshPage(int type)
{
//    DDLog("ccc===================%d", type);
    if (swichType != 1)
    {
        datas->removeAllObjects();
    }
    
    int pageCount = 10; // 每页显示的个数
    
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
    if (swichType != 1)
    {
        for (int ii = 0; ii < last; ii++) {
            CCObject *obj = _allData->objectAtIndex((_nowPage-1)*pageCount+ii);
            datas->addObject(obj);
        }
    }


    CCString *str = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    if(pageLabel)
        pageLabel->setString(str->getCString());
   
    tableView->reloadData();
}

void SGGeneralsBagLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);
    
    
    //临时借用图片。
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);
	ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);
	ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_generalsBagLayer);

    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("officersort");
    CCString *type = CCString::create(str);
    
    // 初始化数据
    SGSortBox::sortCards(_allData, type->intValue(),1);
    
    datas->removeAllObjects();

    int last = _allData->count();
    last > 15 && (last = 15);
    for (int ii = 0; ii < last; ii++)
    {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGGeneralsBagLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.01);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGGeneralsBagLayer::sortHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 1));
    range->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0, titleBg->getPosition().y)));

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height );

    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);

    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-3, b));
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_14);
    if (guideId > guide_tag_14) {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_22);
    }
	
	SGButton *officerBtn = SGButton::createFromLocal("officerBagBtnbg_1.png", str_generalsBagLayer_1, this, menu_selector(SGGeneralsBagLayer::officerBtnClicked), ccp(0, 0), FONT_BOXINFO, ccYELLOW ,30 );
	//装备碎片Button
	SGButton *pieceBtn = SGButton::createFromLocal("officerBagBtnbg_2.png", str_generalsBagLayer_2, this, menu_selector(SGGeneralsBagLayer::pieceBtnClicked), ccp(0, 0),FONT_BOXINFO, ccWHITE ,30);
    
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_SUMMON_OFFICER, 4);
    pieceBtn->setTag(tag);
    
	this->addBtn(officerBtn);
	this->addBtn(pieceBtn);
	
	
	//button下横线
	CCSprite *bottomBorder = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    bottomBorder->setAnchorPoint(ccp(0.5, 1));
    bottomBorder->setScaleX(15);
	bottomBorder->setScaleY(0.5);
	
	this->addChild(bottomBorder, 2);
	
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h - titleBg->getContentSize().height - 90));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-25);
    
    //对齐
//    officerBtn->setPosition(ccp(officerBtn->getContentSize().width , titleBg->getPositionY() - officerBtn->getContentSize().width*0.8));
//	pieceBtn->setPosition(ccp(officerBtn->getPositionX() + officerBtn->getContentSize().width, officerBtn->getPositionY()));
    //pad
    if(s.height == 1024)
        officerBtn->setPosition(ccp( tableView->getPositionX() + s.width/2-tableView->getContentSize().width/2 + 3, titleBg->getPositionY() - officerBtn->getContentSize().width*0.8));
    else
        officerBtn->setPosition(ccp(officerBtn->getContentSize().width * 0.8 , titleBg->getPositionY() - officerBtn->getContentSize().width*0.8));
	pieceBtn->setPosition(ccp(officerBtn->getPositionX() + officerBtn->getContentSize().width, officerBtn->getPositionY()));
    bottomBorder->setPosition(ccpAdd(ccp(0, 0), ccp(0, officerBtn->getPositionY() - officerBtn->getContentSize().height / 2 )));
    
    //如果有可召唤的武将 显示红点提示
    if(this->officerCanSummon())
    {
        CCSprite * notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        notice->setPosition(ccpAdd(pieceBtn->getContentSize(), ccp(-6, -6)));
        pieceBtn->addChild(notice , 10);
    }
    
    
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

    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(-pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));

    CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject *obj = NULL;
    int nPropsNum = 0;
    CCARRAY_FOREACH(array_pro, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card  && card->getType() == 1 && card->getIsProt() == 0)
        {
            nPropsNum++;
        }
    }
    SGCCLabelTTF *labelMaterialTips = SGCCLabelTTF::create(CCString::createWithFormat(str_generalsBagLayer_3,nPropsNum)->getCString(),FONT_PANGWA,22,ccWHITE);
    this->addChild(labelMaterialTips);
    labelMaterialTips->setPosition(ccpAdd(pieceBtn->getPosition(),ccp(pieceBtn->getContentSize().width+labelMaterialTips->getContentSize().width*0.3,0)));

    
/*
	//批量卖出按钮
	SGButton *sellButton = SGButton::createFromLocal("store_exchangebtnbg.png", str_sale, this, menu_selector(SGGeneralsBagLayer::batchSell),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
	sellButton->setAnchorPoint(ccp(0, 0));
	sellButton->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0, b)));
	this->addBtn(sellButton);
*/
    SGSortBox::updateBtn(range,1);
    
    CCSprite *title = NULL;
    title = CCSprite::createWithSpriteFrameName("officerBagTitle_1.png");
    title->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(title);
    
    if (_allData->count()>0)
        refreshPage(1);
    
}

void SGGeneralsBagLayer::buttonClickCanUpStar()
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
    
    swichType = 1;
    if(m_pDrawsLabel)
    m_pDrawsLabel->setVisible(false);
    

    tableView->reloadData();
    //原来在reloadData之前，可能会造成列表不可滑动
    tableView->setPageNEnable(false);


//    CCLOG("可进阶武将");
}

//void SGGeneralsBagLayer::batchSell()
//{
//	CCLOG("Officer Batch Sell");
//	SGMainManager::shareMain()->showBatchSellLayer(6);
//}

SNSTableViewCellItem *SGGeneralsBagLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath); 
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath); 
    
        if (NULL == item)
                item = SGCardItem::create(false,0,this,tableView,6);
        if (num<datas->count())
        {
            int usednum = 0;
            SGBaseMilitaryCard * card =(SGBaseMilitaryCard *)datas->objectAtIndex(num);
            CCString *str_ = CCString::create(card->getClassName());
            if (str_->isEqual(CCString::create("SGOfficerCard")))
            {
                usednum = ((SGOfficerCard*)card)->getAdvanceStuffNum();
            }
            
            item ->updataCard((SGBaseMilitaryCard *)datas->objectAtIndex(num), cardType_officer,true,enterType == 3,usednum>=3, enterType);
        }

     return item;
}
///////

void SGGeneralsBagLayer::itemselectByIndex(int index)
{
    SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
    CCString *str_ = CCString::create(card->getClassName());
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
            //这里也开启滑动功能，传入背包武将。
            CCArray* arr = CCArray::create();
            arr->retain();
            arr=_allData;//SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
            SGGeneralInfoLayer::setMyOfficerList(arr);
            if(_nowPage==1)
                _nowPage=1000;
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)card,enterType,NULL,true,NULL,10,NULL,_nowPage,index);
    }
    else
    {
        SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)card,2);
    }

}

void SGGeneralsBagLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int index = tableView->getItemPointerWithIndexPath(indexpath);
    itemselectByIndex(index);
}
///////
void SGGeneralsBagLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
}

//排序回调
void SGGeneralsBagLayer::sortLayerCallBack(cocos2d::CCArray *cardArray)
{
//    this->datas = cardArray;
    _nowPage = 0;
    this->refreshPage(1);
    this->tableView->reloadData();
    SGSortBox::updateBtn(range,1);
}

//处理一些初始化逻辑
void SGGeneralsBagLayer::initSomeProcess()
{

}

//计算当前有多少武将可以进阶
int SGGeneralsBagLayer::getCanUpStarGeneralCount()
{
    int upStarCount = 0;
    canUP = CCArray::create();
    canUP->retain();
    //int stars[5]={0,0,0,0,0};
  
    for(int i=0;i<_allData->count();i++)
    {
        SGOfficerCard * temp = dynamic_cast<SGOfficerCard *>( _allData->objectAtIndex(i));
        
        if (temp)
        {
            if(temp->getCurrLevel() >= temp->getUpgradelevel() && temp->getMaxStar() && temp->getAdvanceStuffNum()>=3)//看转生素材是否够
            {
                canUP->addObject(temp);
                upStarCount ++;
            }        
        }
    }
    
    return upStarCount;
}

void SGGeneralsBagLayer::openStrengView(int index)
{
    if (index >= datas->count()) return;
    SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
    this->strengHandler((SGOfficerCard *)card);
}

void SGGeneralsBagLayer::openAdvanceView(int index)
{
    if (index >= datas->count()) return;
    SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
    this->advanceHandler((SGOfficerCard *)card);
}

void SGGeneralsBagLayer::strengHandler(SGOfficerCard *card)
{
    //由等级来限制，不再使用经验，（当前卡卡片的等级不能高于主公人物的等级）
    if (card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
    {
        SGSkillInfoBox *cantstrengBox = SGSkillInfoBox::create(this, card,2);
        SGMainManager::shareMain()->showBox(cantstrengBox);
    }
    else
    {
        if (_nowPage==1) {
            _nowPage=1000;
        }
        SGMainManager::shareMain()->showStrengLayer(card,1,true,false,_nowPage);
    }
}
void SGGeneralsBagLayer::advanceHandler(SGOfficerCard *card)
{
    int s = card->getCurrStar()-1;
    if (card->getCurrLevel() <card->getUpgradelevel())
    {
        SGMainManager::shareMain()->showAdvanceLayer(card,1,true);
    }
    else if (stars[s]<3)
    {
        SGMainManager::shareMain()->showAdvanceLayer(card,1,true);

    }
    else
        SGMainManager::shareMain()->showAdvanceLayer(card,1,true);
}

void SGGeneralsBagLayer::sortHandler()
{
    if (swichType == 1)
    {
        SGSortBox *Box = SGSortBox::create(this, this, datas, 1);
        
        SGMainManager::shareMain()->showBox(Box);
    }
    else
    {
        SGSortBox *Box = SGSortBox::create(this, this, _allData, 1);
        
        SGMainManager::shareMain()->showBox(Box);
    }
}

void SGGeneralsBagLayer::backHandler()
{
    // entertype 2 从首页进入
    if (swichType == 1)
    {
        CCNode *node1 = this->getChildByTag(110);
        CCNode *node2 = this->getChildByTag(111);
        CCNode *node3 = this->getChildByTag(112);
        node1->setVisible(true);
        node2->setVisible(true);
        node3->setVisible(true);
        pageLabel->setVisible(true);
        
        datas->removeAllObjects();
        for (int ii = 0; ii < _sortData->count(); ii++)
        {
            CCObject *obj = _sortData->objectAtIndex(ii);
            datas->addObject(obj);
        }
        tableView->setPageNEnable(true);
        tableView->reloadData();
        if(m_pDrawsLabel)
            m_pDrawsLabel->setVisible(true);
        swichType = 0;
    }
    else
    {
        if (enterType ==1)
        {
            //从我的好友里面来的，现在返回我的好友
            
            //从内存中找到这个layer
            //使用的前提是在显示 SGGeneralInfoLayer 时不能把上一个layer释放
            SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_myFriend);
            if(NULL == layer)
            {
                return;
            }
            
            SGMainManager::shareMain()->showLayer(layer);
            SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_generalInfoLayer, true);

        }
        else if (enterType == 2 ||enterType ==4 ||enterType ==5)
        {
            if(x_==5)
            {
                main::DayTaskListRequest *request = new main::DayTaskListRequest();
                SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_LIST, request);
                
            }
            
            else
            {
                SGMainManager::shareMain()->showFirstLayer();
                //       SGFirstLayer *firstLayer = (SGFirstLayer *)mainScene->getChildByTag(sg_firstL             ayer);
                SGFirstStrengBox *strengBox = SGFirstStrengBox::create(this);
                SGMainManager::shareMain()->showBox(strengBox);
            }
        }
        else if (enterType == 3)///SGFirstAdvanceBox现已没用
        {
            //		SGFirstAdvanceBox *strengBox = SGFirstAdvanceBox::create(this);
            //		SGMainManager::shareMain()->showBox(strengBox);
            SGMainManager::shareMain()->showFirstLayer();
        }
        else if (enterType == 10)///firstlayer进入
        {
            SGMainManager::shareMain()->showFirstLayer();
        }
        else
        {
            SGMainManager::shareMain()->showBarrackslayer();
        }
    }
}

void SGGeneralsBagLayer::officerBtnClicked()
{
	return ;
}

void SGGeneralsBagLayer::pieceBtnClicked()
{
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	SGPiecesBagLayer *layer = (SGPiecesBagLayer *)mainScene->getChildByTag(sg_piecesBagLayer);
    if (!layer) {
        layer = SGPiecesBagLayer::create();
    }
	mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	mainScene->addShowLayer(layer);
	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

bool SGGeneralsBagLayer::officerCanSummon()
{
    bool temp = false;
    SGPlayerInfo*playerInfo = SGPlayerInfo::sharePlayerInfo();
    if(playerInfo->getPlayerLevel() > playerInfo->getShowPlayerNoticeLimitLev())
    {
        CCArray * array = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
        for(int i=0; i<array->count() ; ++i)
        {
            SGPiecesCard * card = (SGPiecesCard*)array->objectAtIndex(i);
            SGPiecesDataModel * model = SGStaticDataManager::shareStatic()->getPiecesDictById(card->getItemId());
            bool hasCurOfficer = playerInfo->alreadyHasTheOfficer( model->getOfficerProtoId() );
            if(card->getPiecesCurNum() >= card->getSummonNeed() && !hasCurOfficer)
            {
                temp = true;
                break;
            }
        }
    }
    return temp;

}

