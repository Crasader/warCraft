//
//  SGArrangeLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-30.
//
//

#include "SGArrangeLayer.h"
#include "SGEmbattleBox.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCardItem.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "ArmyModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "AppMacros.h"
#include "SGEmbattleLayer.h"
#include "SGFirstLayer.h"
#include "SGStringConfig.h"
SGArrangeLayer::SGArrangeLayer()
:source(NULL),
m_combattle(0),
m_speed(0),
m_morValue(0),
m_govValue(0),
position(0),
range(NULL),
enterType(0),
totleGovern(0),
allGovern(0),
emtype(0),
availPosition(0),
_card(NULL),
pageLabel(NULL),
preCombattle(0)
{
    _totalPage = 0;
    _nowPage = 0;
    _allData = NULL;
    pageLabel = NULL;
    for (int i =0; i<11; i++) {
        posArr[i]=0;//0是布阵123，1-10是卡片ID
		itemIdArray[i] = 0;
    }
}
SGArrangeLayer::~SGArrangeLayer()
{

	ResourceManager::sharedInstance()->unBindLayerTexture(sg_arrangeLayer);
    CC_SAFE_RELEASE(datas);
    CC_SAFE_RELEASE(_allData);
    CCLog("~SGArrangeLayer");
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    
//    notification->removeObserver(this, MSG_EMBATTLE_CHANGE);
    notification->removeObserver(this, MSG_EMBATTLE_CHANGEALL);

    if (source) {
        source->release();
    }
}

SGArrangeLayer* SGArrangeLayer::create(int pos,int gov,int type,int em,int _availPosition)
{
    SGArrangeLayer *arrangeLayer = new SGArrangeLayer();
    
    if (arrangeLayer && arrangeLayer->init(NULL, sg_arrangeLayer))
    {
        arrangeLayer->totleGovern = gov;
        arrangeLayer->emtype = em;
        arrangeLayer->enterType = type;
        arrangeLayer->availPosition=_availPosition;
        arrangeLayer->initView(pos);
        arrangeLayer->autorelease();
        return arrangeLayer;
    }
    CC_SAFE_DELETE(arrangeLayer);
    return arrangeLayer;
}

void SGArrangeLayer::initView(int pos)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_arrangeLayer);
	ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_arrangeLayer, LIM_PNG_AS_PNG);
    SGCardItem::addCardItemRes(sg_arrangeLayer);//添加item资源
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->addObserver(MSG_EMBATTLE_CHANGE,
//                              this,
//                              callfuncO_selector(SGArrangeLayer::exchangPositionListener));
    notification->addObserver(MSG_EMBATTLE_CHANGEALL,
                              this, callfuncO_selector(SGArrangeLayer::changeAllEmbatteTeamListener));
    setAllGovern();
    datas = CCArray::create();
    datas->retain();
    
    if(enterType == 11)
    {
        _allData = CCArray::create();
        _allData->retain();
        
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array, obj)
        {
            SGOfficerCard *card = (SGOfficerCard *)obj;
            _allData->addObject(card);
        }
    }
    else
    {
        _allData=CCArray::create();
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array, obj)
        {
            SGOfficerCard *card = (SGOfficerCard *)obj;
            _allData->addObject(card);
        }
        _allData->retain();
       
    }
	memset(itemIdArray, 0, 11);
	CCArray *embattleArray = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
	//初始化itemId数组, 将上阵的每一个武将的itemId放入itemIdArray中
	for(int i = 0; i < embattleArray->count(); i++)
	{
		itemIdArray[i] = static_cast<SGOfficerCard *>(embattleArray->objectAtIndex(i))->getItemId();
	}
	

    initbattleArr();//初始化布阵数组
    
	//默认按统御力来排序， type = 9
    SGSortBox::sortCards(_allData, 9,1);

    checkUpEmbattel();
    int last = _allData->count();
    last > 15 && (last = 15);
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
    
    if (datas->count() == 0)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSize size = CCSize(winSize.width - 200	 , 100);
        SGCCLabelTTF *notice = SGCCLabelTTF::create(str_ArrangeLayer_str1, FONT_BOXINFO, FONTSIZE(14), size ,kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter );
        this->addChild(notice);
        notice->setPosition(SGLayout::getPoint(kMiddleCenter));
        SGButton *Btn = SGButton::create("box_btnbg.png",
                                         "public_font_fanhui.png",
                                         this,
                                         menu_selector(SGArrangeLayer::backHandler),
                                         ccp(0, 0),
                                         false,
                                         true);
        this->addBtn(Btn);
        Btn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 300)));
    }

    _nowPage = 1;
    _totalPage = ceil(_allData->count()/15.0f);
	if (_totalPage <= 0)
	{
		_totalPage = 1;
	}
	
//	if (_allData->count() > 0)
//	{
//		refreshPage(1);
//	}
    
    position = pos;
    
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - 220)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_Ok, this, menu_selector(SGArrangeLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.05);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_38);
    backBtn->setTag(modelTag);
    
    
    range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGArrangeLayer::sortHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 1));
    range->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0, titleBg->getPosition().y)));
    
    CCSprite *title = CCSprite::createWithSpriteFrameName("font_apwjsc.png");
    title->setPosition(ccp(s.width/2, titleBg->getPosition().y - titleBg->getContentSize().height/2));
    this->addChild(title);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_arrangeLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 20,bg->getContentSize().width, bg->getContentSize().height - title->getContentSize().height * 2);
    
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    
    bg->setVisible(false);
    
    if (CCDirector::sharedDirector()->getWinSize().height != 1136)
    {
        bg->setScaleY(0.8);
    }

    
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-5, b - 20 + 20));
	
	//适配IPAD
	int right_shift = -28;
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
		||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		right_shift = 35;
        bg->setScaleY(0.86);

	}
    
    if (s.height == 1136)
    {
        bg->setScaleY(0.96);
        bg->setPosition(ccpAdd(bg->getPosition(), ccp(0, 20)));
    }
	
	//加入统御力显示
	CCSprite *horBar = CCSprite::createWithSpriteFrameName("disp_gov.png");
	horBar->setPosition(ccpAdd(titleBg->getPosition(), ccp(horBar->getContentSize().width / 2 + horBar->getContentSize().width / 2 + right_shift,
														   -titleBg->getContentSize().height - horBar->getContentSize().height)));
	horBar->setScale(1.5);
	//加入字
	SGCCLabelTTF *govFont = SGCCLabelTTF::create(str_ArrangeLayer_str2, FONT_BOXINFO, 32 , ccYELLOW);
    //modify by:zyc. merge into create.
	//govFont->setColor(ccYELLOW);
	govFont->setPosition(ccpAdd(horBar->getPosition(), ccp(-govFont->getContentSize().width / 4, 0)));
	//数值字
//	SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
//	SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playInfo->getPlayerLevel());
	govNum = SGCCLabelTTF::create("123",FONT_BOXINFO, 28);
	govNum->setAnchorPoint(ccp(0, 0));
	govNum->ignoreAnchorPointForPosition(false);
	govNum->setPosition(ccpAdd(govFont->getPosition(), ccp(govFont->getContentSize().width/2*1.3, 0)));
    
    govAllNum = SGCCLabelTTF::create("123",FONT_BOXINFO, 28);
	govAllNum->setAnchorPoint(ccp(0, 0));
	govAllNum->ignoreAnchorPointForPosition(false);
	govAllNum->setPosition(ccpAdd(govNum->getPosition(), ccp(govNum->getContentSize().width, 0)));
	
    updateCurGov(GetCurrentGov());
	this->addChild(horBar);
	this->addChild(govFont);
    this->addChild(govNum);
    this->addChild(govAllNum);
	
	
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_37);
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_37) {
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_40);
    }
    tableView->setTag(modelTag);
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - 220 - 100));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, 170)));
    SGSortBox::updateBtn(range,1);
    
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
    
    CCString *strpg = CCString::createWithFormat(str_ArrangeLayer_str3, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(pagecountbg->getContentSize().width*.5f, pagecountbg->getContentSize().height*.5f)));
}
//计算统御力
int  SGArrangeLayer::GetCurrentGov()
{
	//    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();
//    int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    //得到当前的布阵统御力
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    CCObject *obj = NULL;
    int mor = 0;
    int speed = 0;
    int  totleGovern = 0;
    CCARRAY_FOREACH(array, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        mor += card->getMorale();
        speed += card->getSpeed();
		totleGovern += card->getGoverning();
        if (card->getOfficerEquips())
        {
            for (int i = 0; i <card->getOfficerEquips()->count(); i++)
            {
                SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
                mor += equip->getMorale();
                speed += equip->getSpeed();
                if ((card->getGoverning()+equip->getGoverning())<0)
                {
                    totleGovern -= card->getGoverning();
                }
                else
                {
                    totleGovern += equip->getGoverning();
                }
            }
        }
        
    }
    this->totleGovern = totleGovern;
    return totleGovern;
}

bool SGArrangeLayer::checkIsOutGov(SGOfficerCard *card){
    
    int leftgov=allGovern-totleGovern;
//    SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
//	SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playInfo->getPlayerLevel());
//    int leftgov= player->getPlayerGoverning() + SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering()-GetCurrentGov();
    int m_gov=card->getGoverning();
    
    CCObject *obj;
    CCARRAY_FOREACH(card->getOfficerEquips(), obj)
    {
        SGEquipCard *equicard = (SGEquipCard *)obj;
        
        m_gov += equicard->getGoverning();
    }
    
    if (leftgov>= m_gov)
        return true;
    else
        return false;
}

SNSTableViewCellItem *SGArrangeLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGOfficerCard *OfficerCard=(SGOfficerCard *)datas->objectAtIndex(num);
    if (NULL == item)
    {
        item = SGCardItem::create(true ,false,emtype);
    }
    if (num<datas->count())
    {
        
        item ->updataCard((SGOfficerCard *)datas->objectAtIndex(num), cardType_officer);
    
            
        if (checkIsInEmbattle(OfficerCard->getSsid()))
		{
            item->changeSelected(true);
        }
		else if (haveSameNameOfficer(OfficerCard->getItemId()))//如果有同名武将上场,则给提示
		{
			item->setIsCanSelect(false, false, false, true);
			item->changeSelected(false);
		}
        else
        {
            //
            if (availPosition>0)
			{
                if (checkIsOutGov(OfficerCard))
				{
                    item->setIsCanSelect(true,false,true);
                }
                else
                {
                    item->setIsCanSelect(false,false,true);
                }
            }
			else
			{
                item->setIsCanSelect(false,false,false);
            }
            //不可选
            item->changeSelected(false);
        }
            
        
//        item->setIsCanSelect(checkIsOutGov((SGOfficerCard *)datas->objectAtIndex(num)));
    }
    else
    {
        item->setVisible(false);
    }
    return item;
}
void SGArrangeLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    

	
	int index = tableView->getItemPointerWithIndexPath(indexPath);
	_card = (SGOfficerCard*)datas->objectAtIndex(index);
	

	if(checkIsOutGov(_card))//这里应该是没有超过统御力
	{
	   if(checkIsInEmbattle(_card->getSsid()))
	   {
			totleGovern-=_card->getGoverning();
			availPosition++;
			deleteCard(_card->getSsid(), _card->getItemId());
	   }
	   else
	   {
		   //没有超过统御力,而且没有在阵上,检测是不是同名
		   if (haveSameNameOfficer(_card->getItemId()))
		   {
			   SG_SHOW_WINDOW(str_ArrangeLayer_str4);
			   return ;
		   }
		   
		   if (availPosition==0)
		   {
			   return;
		   }
		   totleGovern+=_card->getGoverning();
		   availPosition--;
		   addNewCard( _card->getSsid(), _card->getItemId());
		}
		tableView->reloadData(false);
		updateCurGov(totleGovern);
//                this->showEmbattleBox(_card, 100);
	}
	else
	{
		if (checkIsInEmbattle(_card->getSsid()))
		{
			totleGovern-=_card->getGoverning();
			availPosition++;
			deleteCard(_card->getSsid(), _card->getItemId());
			
			tableView->reloadData(false);
			updateCurGov(totleGovern);
		}
		else if (haveSameNameOfficer(_card->getItemId()))
		{
			SG_SHOW_WINDOW(str_ArrangeLayer_str5);
		}
		else
		{
			SGMainManager::shareMain()->showMessage(str_ArrangeLayer_str6);
		}
	}

}
//检查与阵上同名的武将
bool SGArrangeLayer::haveSameNameOfficer(int itemId)
{
	SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
	int protoId = officer->getOfficerProtoId();
	
	for (int i = 0; i < 11; i++)
	{
		if (itemIdArray[i] == 0)
			continue;
		int onEmbattleProtoId = SGStaticDataManager::shareStatic()->getOfficerById(itemIdArray[i])->getOfficerProtoId();
		if (onEmbattleProtoId == protoId)
		{
			return true;
		}
	}
	return false;
}

void SGArrangeLayer::refreshPage(int type)
{
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
    
    CCString *str = CCString::createWithFormat(str_ArrangeLayer_str3, _nowPage, _totalPage);
    if(pageLabel) pageLabel->setString(str->getCString());
    tableView->reloadData();
}

void SGArrangeLayer::showChangeOfficer(SGOfficerCard *card, SGOfficerCard *source)
{
	CCLOG("ShowChangeOfficer");
	SGChangeOfficerBox *b = SGChangeOfficerBox::create(this, _card, emtype, source);
	SGMainManager::shareMain()->showBox(b);
}

void SGArrangeLayer::showEmbattleBox(SGOfficerCard *card, int fromWhere)
{
	int wid = 535;
	int hei = 646;
	if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,card)<=0)
	{
		wid = 535;
		hei = 476;
	}
    SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, card,totleGovern,emtype,position, wid, hei, fromWhere);
    SGMainManager::shareMain()->showBox(embattleBox);
}
void SGArrangeLayer::showEmbattleBoxByIndex(int index)
{
    _card = (SGOfficerCard*)datas->objectAtIndex(index);
    if(checkIsOutGov(_card))
    {
        
        if(checkIsInEmbattle(_card->getSsid()))
        {
            totleGovern-=_card->getGoverning();
            availPosition++;
            deleteCard(_card->getSsid(), _card->getItemId());
        }
        else{
            
            if (availPosition==0) {
                return;
            }
            totleGovern+=_card->getGoverning();
            availPosition--;
            addNewCard( _card->getSsid(), _card->getItemId());
        }
        tableView->reloadData(false);
        updateCurGov(totleGovern);
    }
    else
    {
        if (checkIsInEmbattle(_card->getSsid())) {
            totleGovern-=_card->getGoverning();
            availPosition++;
            deleteCard(_card->getSsid());
            
            tableView->reloadData(false);
            updateCurGov(totleGovern);
        }else
            SGMainManager::shareMain()->showMessage(str_ArrangeLayer_str6);
    }
    
    //SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, _card, totleGovern,emtype, position);
    //SGMainManager::shareMain()->showBox(embattleBox);
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_0) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_38);
    }
}
void SGArrangeLayer::sortHandler()
{
    SGSortBox *Box = SGSortBox::create(this, this, _allData,1);
    SGMainManager::shareMain()->showBox(Box);
}
void SGArrangeLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
//    if (enterType == 10) {
//        this->changeAllEmbatteTeamhandle();
//    }
//    else if (enterType==1)
//    {
//        this->changeAllEmbatteTeamhandle();
//    }
//    
//    else{
    
    preCombattle=SGPlayerInfo::sharePlayerInfo()->getcombatValue();
        this->changeAllEmbatteTeamhandle();
 
    
//        SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
//    }
}

void SGArrangeLayer::sortLayerCallBack(CCArray *cardArray)
{
    _nowPage = 1;
    CCString *str = CCString::createWithFormat(str_ArrangeLayer_str3, _nowPage, _totalPage);
    if(pageLabel) pageLabel->setString(str->getCString());

//    datas = cardArray;
//    tableView->reloadData();
    datas->removeAllObjects();
    checkUpEmbattel();
    int last = _allData->count();
    last > 15 && (last = 15);
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
//    datas = arr;
    tableView->reloadData();
//    refreshPage(1);
    SGSortBox::updateBtn(range,1);
}
void SGArrangeLayer::exchangPosition()
{
    main::ChangeOfficerRequest *request = new main::ChangeOfficerRequest();
    request->set_sourceid(source->getSsid());
    request->set_targetid(_card->getSsid());
    request->set_teamid(emtype-1);
    SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_CHANGE, request);
}
void SGArrangeLayer::exchangPositionListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ChangeOfficerResponse *response = (main::ChangeOfficerResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            
            source->setIsProt(0);
            _card->setIsProt(1);
            SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_fail);
        }
    }
    
}
int SGArrangeLayer::getgov(SGOfficerCard *m_card)
{
    int gov = m_card->getGoverning();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(m_card->getOfficerEquips(), obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
               gov += card->getGoverning();
    }
    if (gov< 0) {
        gov = 0;
    }
    return gov;
}

void SGArrangeLayer::updateCurGov(int goving){
    
    if (goving>allGovern) {
        govAllNum->setInsideColor(ccRED);
    }
    govNum->setString(CCString::createWithFormat("%d",goving)->getCString());
    govAllNum->setString(CCString::createWithFormat("/%d",allGovern)->getCString());
    govAllNum->setPosition(ccpAdd(govNum->getPosition(), ccp(govNum->getContentSize().width, 0)));
    
    
}
void  SGArrangeLayer:: setAllGovern(){

    SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
	SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playInfo->getPlayerLevel());
	allGovern=player->getPlayerGoverning() + SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering();
}

void SGArrangeLayer:: initbattleArr(){

    posArr[0]=emtype;
    CCObject *obj=NULL;
    CCARRAY_FOREACH(_allData, obj)
    {
        SGOfficerCard *card=(SGOfficerCard *)obj;
        int p = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,card);
        if (p>0) {
            posArr[p]=card->getSsid();
        }
        
        
        
    }
   SGOfficerCard *card= SGTeamgroup::shareTeamgroupInfo()->getlordofficer(emtype);
    int p = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,card);
    if (p>0) {
        posArr[p]=card->getSsid();
    }
    _allData->removeObject(card);
}

void SGArrangeLayer:: addNewCard(int cardId, int itemId){
    
    for (int i=1; i<11; i++) {
        int currId=posArr[i];
        if (currId<1) {
            posArr[i]=cardId;
            break;
        }
    }
	
	for (int i = 0; i < 11; i++)
	{
		if (itemIdArray[i] == 0)
		{
			itemIdArray[i] = itemId;
			break;
		}
	}

}
void SGArrangeLayer:: deleteCard(int cardId, int itemId){
    
	for (int i = 0; i < 11; i++)
	{
		if (itemIdArray[i] == itemId)
		{
			itemIdArray[i] = 0;
			break;
		}
	}
	
    for (int i=1; i<11; i++) {
        int currId=posArr[i];
        if (currId==cardId) {
            posArr[i]=0;
            return;
        }
    }
    
}
bool SGArrangeLayer::checkIsInEmbattle(int cardId){
    
    for (int i=1; i<11; i++) {
        int currId=posArr[i];
        if (currId==cardId) {
            return true;
        }
    }
    return false;
}

void SGArrangeLayer::changeAllEmbatteTeamhandle(){

    main::BatchChangeOfficerRequest *request=new main::BatchChangeOfficerRequest();
    request->set_teamid(posArr[0]-1);
    for (int i=1; i<11; i++)
        request->add_officerids(posArr[i]);
     
    SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_CHANGEALL, request);
}
void SGArrangeLayer::changeAllEmbatteTeamListener(CCObject *obj){
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::BatchChangeOfficerResponse *response = (main::BatchChangeOfficerResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            if (enterType==10) {
                //CCLog("%d",SGPlayerInfo::sharePlayerInfo()->getcombatValue());
                //CCLog("%d",this->getComBattle());
                
                int buffCombattle=-this->getComBattle()+SGPlayerInfo::sharePlayerInfo()->getcombatValue();
                SGMainManager::shareMain()->showFirstLayer();
                
                SGFirstLayer *layer=(SGFirstLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
                layer->showAddAttributeEffect(0, 0, buffCombattle);
                
            }else if(enterType==1 )
            {
                int mor=this->getMorValue();
                int speed=this->getSpeedValue();
                int combattle=this->getComBattle();
                int gov=this->getGovValue();
                SGMainManager::shareMain()->showEmbattleLayer(1,emtype);
                SGEmbattleLayer *battlelayer=(SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer);
                
                battlelayer->showAddAttributeEffect( battlelayer->getmorValue()-mor,battlelayer->getspeedvalue()-speed, battlelayer->getcombattle()-combattle ,battlelayer->getgovValue()-gov);
            }
            else
            {
                
                int mor=this->getMorValue();
                int speed=this->getSpeedValue();
                int combattle=this->getComBattle();
                 int gov=this->getGovValue();
                SGMainManager::shareMain()->showEmbattleLayer(0,emtype);
                SGEmbattleLayer *battlelayer=(SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer);
                
               battlelayer->showAddAttributeEffect( battlelayer->getmorValue()-mor,battlelayer->getspeedvalue()-speed, battlelayer->getcombattle()-combattle,battlelayer->getgovValue()-gov );
            }
            
            //首页布阵的提示
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_fail);
        }
    }
}
void SGArrangeLayer::showEffectOfAddAttribute(){
    
  

}

void SGArrangeLayer:: checkUpEmbattel(){
    
    for (int ii = 0; ii < _allData->count(); ii++) {
        SGOfficerCard *card=(SGOfficerCard *)_allData->objectAtIndex(ii);
        if (checkIsInEmbattle(card->getSsid())) {
            _allData->removeObjectAtIndex(ii);
            _allData->insertObject(card, 0);
        }

    }
}

#pragma mark SGChangeOfficerBox

//阵上换人的时候，弹出武将确认框
///////////////////////////////////////////////
SGChangeOfficerBox::SGChangeOfficerBox():
_card(NULL),source(NULL),m_gov(0),emtype(0)
{
	
}
SGChangeOfficerBox::~SGChangeOfficerBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_changeOfficer);
}
//创建对应box
SGChangeOfficerBox *SGChangeOfficerBox::create(SGBoxDelegate *del, SGOfficerCard *card, int type, SGOfficerCard *source)
{
	SGChangeOfficerBox *box = new SGChangeOfficerBox();
	box->_card = card;
	box->emtype = type;
	box->source = source;
	if (box && box->init(del, box_changeOfficer,CCSizeMake(504, 500),true))
    {
        box->initView();
        box->autorelease();
        return box;
    }
	CC_SAFE_RELEASE(box);
    return NULL;
}
void SGChangeOfficerBox::boxClose()
{
	SGBaseBox::boxClose();
	 //设置box关闭后，主场景可以触摸
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer)
	{
		mainLayer->setIsCanTouch(true,false);
	}
}

void SGChangeOfficerBox::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_box_changeOfficer);
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_box_changeOfficer);
	ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_box_changeOfficer);
	ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_box_changeOfficer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_box_changeOfficer);
	
	
	fontbg->setPreferredSize(CCSize(480, 302));
	fontbg->setPosition(ccpAdd(biao->getPosition(), ccp(0,- fontbg->getContentSize().height*.5f-40)));
	
	char spriteName[256] = {0};
	SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_emBattleBox);
    sprintf(spriteName, "head%d.png",_card->getHeadSuffixNum());
	
	
	SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
    int starlvl = temp->getOfficerCurrStarLevel();
	int advance = temp->getAdvNum();//显示进阶到哪个阶段的字段
	
    if (starlvl>1) {
        starlvl-=1;
    }

	CCSprite *portrait = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl))->getCString());
    CCSprite *head = CCSprite::createWithSpriteFrameName(spriteName);
	
    portrait->setPosition(ccpAdd(fontbg->getPosition(),ccp(-fontbg->getContentSize().width/2+portrait->getContentSize().width/2+20,
														   fontbg->getContentSize().height/2-portrait->getContentSize().height/2-15)));
	

    //if (str_1->isEqual(CCString::create("SGOfficerCard")))
    {
        portrait->removeChildByTag(30);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(portrait->getContentSize().width*0.355,
									 portrait->getContentSize().height - countryName->getContentSize().height));//调整国家名字的位置
		portrait->addChild(countryName,30,30);
		
		this->addChild(head, 10);
		head->setPosition(portrait->getPosition());
    }
    
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background, 9);
    background->setPosition(portrait->getPosition());
    this->addChild(portrait, 11);
    
    SGCCLabelTTF* name = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_PANGWA, 26);
    name->setAnchorPoint(ccp(0, 1));
    name->setPosition(ccp(portrait->getPosition().x + portrait->getContentSize().width*.7, portrait->getPosition().y + portrait->getContentSize().height*.4));
    this->addChild(name);
    
	for(int i=0; i < _card->getUpgradestar();i++)
    {
        CCSprite* xingji = CCSprite::createWithSpriteFrameName("xx.png");
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccp(name->getPosition().x + xingji->getContentSize().width * i, name->getPosition().y - xingji->getContentSize().height*1.4));
        this->addChild(xingji,1,i);
        if (i>=_card->getCurrStar()) {
            xingji->setVisible(false);
        }
    }
    for(int i=_card->getCurrStar(); i < _card->getUpgradestar();i++)
    {
        CCSprite* xingji = CCSprite::createWithSpriteFrameName("bluestar.png");
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccpAdd(((CCSprite*)getChildByTag(i))->getPosition(), ccp(0,-0.5)));
        this->addChild(xingji,1,i);
        
    }
    
    int m_atk = _card->getAtk();
    int m_def = _card->getDef();
    int m_round = _card->getRound();
    int m_speed= _card->getSpeed();
    int m_mor = _card->getMorale();
    m_gov = _card->getGoverning();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_card->getOfficerEquips(), obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
//        m_atk += card->getAtk();
//        m_def += card->getDef();
//        m_round += card->getRound();
//        m_speed += card->getSpeed();
        m_mor += card->getMorale();
        m_gov += card->getGoverning();
    }
    if (m_gov< 0) {
        m_gov = 0;
    }
    
    CCString* str_lvl = CCString::createWithFormat("%d级",_card->getCurrLevel());
    SGCCLabelTTF* lvl = SGCCLabelTTF::create(str_lvl->getCString(), FONT_PANGWA, 26);
    lvl->setAnchorPoint(ccp(1, 0.5));
	
    this->addChild(lvl);
    lvl->setPosition(ccp(biao->getPosition().x + lvl->getContentSize().height * 2.5, name->getPosition().y - name->getContentSize().height/2));
    
    CCSprite* tiao1 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    tiao1->setScaleX(450/tiao1->getContentSize().width);
    tiao1->setPosition(ccp(biao->getPosition().x, portrait->getPosition().y - 65));
    this->addChild(tiao1,5);
    
    CCSprite* tiao2 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    tiao2->setScaleX(450/tiao1->getContentSize().width);
    tiao2->setPosition(ccpAdd(tiao1->getPosition(),ccp(0, -140)));
    if (SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,_card)>0)
		this->addChild(tiao2,5);

    SGCCLabelTTF* font_zjji = SGCCLabelTTF::create(str_zhujiangji, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_zjji->setColor(COLOR_YELLOW);
    font_zjji->setAnchorPoint(ccp(0, 0.5f));
    font_zjji->setPosition(ccpAdd(portrait->getPosition(), ccp(-portrait->getContentSize().width*.5f - 2,-85)));
    //this->addChild(font_zjji);
	//
    SGCCLabelTTF* font_wjji = SGCCLabelTTF::create(str_beidongji, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_wjji->setColor(COLOR_YELLOW);
    font_wjji->setAnchorPoint(ccp(0, 0.5f));
    font_wjji->setPosition(ccpAdd(font_zjji->getPosition(), ccp(0,-150)));

    
    SGCCLabelTTF *atk_ = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
    //modify by:zyc. merge into create.
    //atk_->setColor(ccc3(0xff,0xc2,0x00));
    atk_->setPosition(ccpAdd(font_wjji->getPosition(), ccp(atk_->getContentSize().width*.5f - 5,145)));
    this->addChild(atk_);
    
    SGCCLabelTTF *def_ = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
    //modify by:zyc. merge into create.
    //def_->setColor(ccc3(0xff,0xc2,0x00));
    def_->setPosition(ccpAdd(atk_->getPosition(), ccp(0,-43)));
    this->addChild(def_);
    
    SGCCLabelTTF *mor_ = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
    //modify by:zyc. merge into create.
    //mor_->setColor(ccc3(0xff,0xc2,0x00));
    mor_->setPosition(ccpAdd(def_->getPosition(), ccp(0,-43)));
    this->addChild(mor_);
    
    SGCCLabelTTF *xuli_ = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
    xuli_->setPosition(ccpAdd(atk_->getPosition(), ccp(230, 0)));
    this->addChild(xuli_);
    //modify by:zyc. merge into create.
    //xuli_->setColor(ccc3(0xff,0xc2,0x00));
    
    SGCCLabelTTF *sudu_ = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 28 , ccc3(0xff, 0xc2, 0x00));
    sudu_->setPosition(ccpAdd(xuli_->getPosition(), ccp(0,-43)));
    this->addChild(sudu_);
    //modify by:zyc. merge into create.
    //sudu_->setColor(ccc3(0xff,0xc2,0x00));
    
    CCString* str_atk = CCString::createWithFormat("%d",m_atk);
    SGCCLabelTTF* atk = SGCCLabelTTF::create(str_atk->getCString(), FONT_BOXINFO, 26);
    atk->setAnchorPoint(ccp(1, 0.5));
    atk->setPosition(ccpAdd(atk_->getPosition(), ccp(105, 0)));
    this->addChild(atk);
    
    CCString* str_def = CCString::createWithFormat("%d",m_def);
    SGCCLabelTTF* def = SGCCLabelTTF::create(str_def->getCString(), FONT_BOXINFO, 26);
    def->setAnchorPoint(ccp(1, 0.5));
    def->setPosition(ccpAdd(def_->getPosition(), ccp(105, 0)));
    this->addChild(def);
    
    CCString* str_shiqi = CCString::createWithFormat("%d",m_mor);
    SGCCLabelTTF* shiqi = SGCCLabelTTF::create(str_shiqi->getCString(), FONT_BOXINFO, 26);
    shiqi->setAnchorPoint(ccp(1, 0.5));
    shiqi->setPosition(ccpAdd(mor_->getPosition(), ccp(105,0)));
    this->addChild(shiqi);
	
    CCString* strxuli = CCString::createWithFormat("%d",m_round);
    SGCCLabelTTF* xuli = SGCCLabelTTF::create(strxuli->getCString(), FONT_BOXINFO, 26);
    xuli->setAnchorPoint(ccp(1, 0.5));
    xuli->setPosition(ccp(xuli_->getPosition().x + 105, atk->getPosition().y));
    this->addChild(xuli);
    
    CCString* str_spd = CCString::createWithFormat("%d",(int)m_speed);
    SGCCLabelTTF* speed = SGCCLabelTTF::create(str_spd->getCString(), FONT_BOXINFO, 26);
    speed->setAnchorPoint(ccp(1, 0.5));
    speed->setPosition(ccp(xuli_->getPosition().x + 105, def->getPosition().y));
    this->addChild(speed);
    
    CCScale9Sprite* font_bg = CCScale9Sprite::createWithSpriteFrameName("labelbg.png");
    font_bg->setPreferredSize(CCSize(215, 24));
    font_bg->setPosition(ccp(xuli_->getPosition().x + 70, shiqi->getPosition().y));
	//    this->addChild(font_bg);
    
    SGCCLabelTTF* font_tongyu = SGCCLabelTTF::create(str_shangzheng_need_tongyuli, FONT_BOXINFO, 26 , ccc3(0xff, 0xc2, 0x00));
    //modify by:zyc. merge into create.
    //font_tongyu->setColor(ccc3(0xff,0xc2,0x00));
    font_tongyu->setPosition(ccp(font_bg->getPosition().x - 21, font_bg->getPosition().y));
    this->addChild(font_tongyu);
    
    CCString* str_tongyu = CCString::createWithFormat("%d",m_gov);
    SGCCLabelTTF* tongyu = SGCCLabelTTF::create(str_tongyu->getCString(), FONT_BOXINFO, 26);
    tongyu->setAnchorPoint(ccp(0, 0.5));
    tongyu->setPosition(ccp(font_tongyu->getPosition().x + font_tongyu->getContentSize().width*.5 + 7, font_tongyu->getPosition().y));
    this->addChild(tongyu);
	
	int modelTag = 0;
	//取消button
	int dis = -265;
	int left = 5;
	
	left = 115;
	dis = -185;
	
	//更换上阵武将
	SGButton *changeBtn = SGButton::create("box_btnbg.png",
									   "font_pqshangzhen.png",
									   this,
									   menu_selector(SGChangeOfficerBox::changePosition),
									   ccp(0, 0),
									   false,
									   true);
	
	this->addBtn(changeBtn);
	changeBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-110, dis)));
	modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_38);
	changeBtn->setTag(modelTag);
	
	
	SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGChangeOfficerBox::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
	cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(left, dis)));

}
//发送换武将消息，通过弹框中是派遣上阵设置换人
void SGChangeOfficerBox::changePosition()
{
    main::ChangeOfficerRequest *request = new main::ChangeOfficerRequest();
    request->set_sourceid(source->getSsid());//通过create方法传过来，同ArrageLayer相同的source
    request->set_targetid(_card->getSsid());//与ArrangeLayer是同一个card
    request->set_teamid(emtype-1);
    SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_CHANGE, request);
	this->boxClose();
}
