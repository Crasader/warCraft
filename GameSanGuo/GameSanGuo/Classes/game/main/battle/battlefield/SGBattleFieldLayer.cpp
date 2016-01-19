//
//  SGBattleFieldLayer.cpp
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#include "SGBattleFieldLayer.h"
#include "PvpFight.pb.h"
#include "SGBattleData.h"
#include "SGBattleMainLayer.h"
#include "SGBattleFieldData.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

#define SECTIONINFOFONTSIZE   8
#define SECTIONTABLEVIEWBOTTOM   50
#define SECTIONINFOVIEWTAG -1000

SGBattleFieldLayer::SGBattleFieldLayer()
:pointer(0),
indexPointer(0),
sectiontableView(NULL),
isact(false),
lastMoveDis(0.0f) //上次选中场景时移动的距离
{
    
}

SGBattleFieldLayer::~SGBattleFieldLayer()
{
    CC_SAFE_RELEASE(datas);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleFieldLayer);
}

SGBattleFieldLayer *SGBattleFieldLayer::create()
{
    SGBattleFieldLayer *battleField = new SGBattleFieldLayer();
    if (battleField && battleField->init(NULL, sg_battleFieldLayer)) {
        battleField->initData();
        battleField->initView();
        battleField->autorelease();
        return battleField;
    }
    
    CC_SAFE_DELETE(battleField);
    return NULL;
}

void SGBattleFieldLayer::initData()
{
    this->datas = CCArray::create();
    this->datas->retain();
    
    int initCount=1;
    int winPvp = SGPlayerInfo::sharePlayerInfo()->getPvpWinPvp();
   
    if (winPvp > SGPlayerInfo::sharePlayerInfo()->getWinPvp()  ) {
        initCount=2;
    }
    
    for (int i = initCount; i < 3; i++) {
        CCString *str = CCString::create((i == 1) ? str_BattleFieldLayer_str1:str_BattleFieldLayer_str2);
        CCString *tip = CCString::create((i == 1) ? str_BattleFieldLayer_str3: str_BattleFieldLayer_str4);
        SGBattleFieldData *data = new SGBattleFieldData();
        data->setBattleFieldType(i);
        data->setBattleFieldName(str);
        data->setBattleFieldTip(tip);
        
        this->datas->addObject(data);
		//10.23//george
		data->autorelease();
    }
    
}

void SGBattleFieldLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_battleFieldLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_battleFieldLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_battleFieldLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_battleFieldLayer);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, 1136/2-(size.height-headH-bottomH)/2, bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setScaleY(size.width / bgImg->getContentSize().height);
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_BattleRankLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(title);
    
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGBattleFieldLayer::backButton),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*0.65f, title->getPosition().y));
    int modelTag = 0;
    
//    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_4);
    
    tableView->setTag(modelTag);
//    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(12)));
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH - headH - skewing(3)));
    tableView->setDown(-20);
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
    tableView->setSelectType(TableViewSelectTypeSingleOpposite);
}

void SGBattleFieldLayer::backButton(cocos2d::CCObject *sender)
{
    
    SGMainManager::shareMain()->showPvpMainLayer();
}


#pragma mark - -----tableView actions-----
SNSTableViewCellItem* SGBattleFieldLayer::tableViewItemForCell(SNSTableView *tableView, SNSIndexPath *indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	CCSize size = tableView->getItemSize();
	SGBattleFieldItem *item = (SGBattleFieldItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGBattleFieldData *data = (SGBattleFieldData *)datas->objectAtIndex(index);
    if (item == NULL)
    {
        item = SGBattleFieldItem::create(this, data);
        item->setTag(index);
    }
    if (index < datas->count()) {
        item->setTag(index);
    }else {
        item->setVisible(false);
    }
    
    return item;
}

unsigned int SGBattleFieldLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGBattleFieldLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    pointer = tableView->getItemPointerWithIndexPath(indexPath);
}

void SGBattleFieldLayer::showSectionInfoHandler(SGBattleFieldItem *section)
{
    if (isact) {
        return;
    }
    creatSectionInfoView(section);
    if (sectiontableView) {
        if (sectiontableView->isVisible())
        {
            if (lastMoveDis>0) {
                tableView->moveCellByDistance(indexPointer,lastMoveDis,false,-sectiontableView->getContentSize().height);
            }
            else{
                tableView->moveCellByDistance(indexPointer,sectiontableView->getContentSize().height,true);
            }
            moveSectiontableViewDown(false);
            lastMoveDis = 0;
            indexPointer = 0;
            
        }
        else
        {
            SNSIndexPath* newIndexPath = SNSIndexPath::create(pointer, 1);
            SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
            CCPoint selectViewCellPos = tableViewCell->getPosition();
            float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
            
            sectiontableView->setVisible(true);
            CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height+7);
            indexPointer = pointer;
            lastMoveDis = moveDis;
            if(moveDis>0)
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(0,moveDis)));
                tableView->moveCellByDistance(pointer,-moveDis,false,sectiontableView->getContentSize().height);
            }
            else
            {
                sectiontableView->setPosition(infoViewPos);
                tableView->moveCellByDistance(pointer,-sectiontableView->getContentSize().height,true);
                
            }
            moveSectiontableViewDown(true);
        }
    }
}

void SGBattleFieldLayer::moveSectiontableViewDown(bool isDown)
{
    isact = true;
    CCSprite *infoSp = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    infoSp->stopAllActions();
    
    if (isDown)
    {
        infoSp->setPosition(ccp(12,sectiontableView->getContentSize().height));
         CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGBattleFieldLayer::setIsAct));
        infoSp->runAction(CCSequence::create(CCMoveTo::create(ITEMACTIONTIME,ccp(12,0)), call, NULL));
    }
    else
    {
        sectiontableView->stopAllActions();
        infoSp->setPosition(ccp(12,0));
        infoSp->runAction(CCMoveTo::create(ITEMACTIONTIME, ccp(12,sectiontableView->getContentSize().height)));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGBattleFieldLayer::hideSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
}

void SGBattleFieldLayer::hideSectiontableView()
{
    if (sectiontableView != NULL) {
        sectiontableView->setVisible(false);
    }
    setIsAct();
}

void SGBattleFieldLayer::setIsAct()
{
    isact = !isact;
}

void SGBattleFieldLayer::creatSectionInfoView(SGBattleFieldItem *section)
{
    
    
    CCSprite *infoSp = NULL;
    if (sectiontableView == NULL)
    {
        infoSp = CCSprite::createWithSpriteFrameName("rban.png");
        infoSp->setAnchorPoint(CCPointZero);
        sectiontableView = SNSScrollView::create(CCRectMake(0, 0, section->bgSize.width + 20, infoSp->getContentSize().height));
		
        sectiontableView->setAnchorPoint(CCPointZero);
        sectiontableView->addChild(infoSp,1,SECTIONINFOVIEWTAG);
        
        CCLabelTTF *labelT = CCLabelTTF::create(section->data->getBattleFieldTip()->getCString(), FONT_BOXINFO, FONTSIZE(12), CCSizeMake(infoSp->getContentSize().width * 0.8, infoSp->getContentSize().height * 0.8), kCCTextAlignmentLeft);
        labelT->setPosition(ccp(infoSp->getContentSize().width * 0.5, infoSp->getContentSize().height * 0.5));
        
        infoSp->addChild(labelT, 1, 10);
        
        sectiontableView->setVisible(false);
        tableView->m_body->addChild(sectiontableView);
    }else{
        infoSp = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
        CCLabelTTF *label =  (CCLabelTTF*)infoSp->getChildByTag(10);
        label->setString(section->data->getBattleFieldTip()->getCString());
    }
    
}

void SGBattleFieldLayer::entrySectionHandler(SGBattleFieldItem *section)
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCLOG("section->data->getBattleFieldType()==%d",section->data->getBattleFieldType());
    playerInfo->setBattleType(section->data->getBattleFieldType());
    int pvpWinPvp = playerInfo->getPvpWinPvp();
    int winPvp = playerInfo->getWinPvp();
//    SGBattleFieldData *data = (SGBattleFieldData *)datas->objectAtIndex(pointer);//这个是什么意思
    SGBattleFieldData *data=(SGBattleFieldData *) section->data;
    if (data == NULL) {
        return;
    }
    if (data->getBattleFieldType() == 1 ||pvpWinPvp >= winPvp) {
        hideSectiontableView();
        //    SGPlayerInfo::sharePlayerInfo()->setBattleType(section->data->getBattleFieldType());//    section->getTag();
        //////kanata  MT效果
//        CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleFieldLayer::hideScrollItemScale));
        CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleFieldLayer::hideScrollItemSingleMove));
        CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGBattleFieldLayer::gotoMatchRequest));
        
        this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*2),speedFun2,NULL));
    }
    else if (data->getBattleFieldType() == 2 && pvpWinPvp < winPvp)
    {
         CCString *tip = CCString::createWithFormat(str_BattleFieldLayer_str5, winPvp);
         SGMainManager::shareMain()->showMessage(tip->getCString());
    }
}

//void SGBattleFieldLayer::aabb(int type)
//{
//    hideSectiontableView();
//    SGPlayerInfo::sharePlayerInfo()->setBattleType(type);//    section->getTag();
//    CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGBattleFieldLayer::hideScrollItemScale));
//    CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGBattleFieldLayer::gotoMatchRequest));
//    
//    this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*2),speedFun2,NULL));
//}


void SGBattleFieldLayer::gotoMatchRequest()
{
//    return;
//    main::PvpEntryRequest *request = new main::PvpEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
    
    SGMainManager::shareMain()->showPvpMatchLayer();
}

void SGBattleFieldLayer::showSeasonMainLayer(CCObject *sender)
{
//    SGSocketRequest *request = (SGSocketRequest *)sender;
//    main::PvpEntryResponse *response = (main::PvpEntryResponse *)request->m_msg;
//    SGBattleData *battleData = NULL;
//    SGRankPlayer *selfPlayer = NULL;
//    if (response) {
//        int var1 = response->isopen();
//        int var2 = response->toppos();
//        int var3 = response->toplevel();
//        int var4 = response->fighttimes();
//        int var5 = response->score();
//        int var6 = response->joinmax();
//        int var7 = response->jointoday();
//        
//        
//        std::string str = response->seasontime();
//        float winr = response->winratio();
//        
//        CCString *name = SGPlayerInfo::sharePlayerInfo()->getNickName();
//        int iconId = SGPlayerInfo::sharePlayerInfo()->getPlayerImageID();
//        // create(const char *name, int hid, int rank, int levl, int ftime, float rwin)
//        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var2, var3, var4, winr);//不知道要哪些
//        battleData = SGBattleData::create(str.c_str(), var1, var5, var7, var6);
//        battleData->setPlayer(selfPlayer);
//        
//        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var2, var3, var4, winr);
//        if (selfPlayer) {
//            SGPlayerInfo::sharePlayerInfo()->setPvpRank(selfPlayer);
//            SGPlayerInfo::sharePlayerInfo()->setplayerPvpLevel(selfPlayer->getLevel());
//            SGPlayerInfo::sharePlayerInfo()->setplayerPvpNum(response->score());
//        }
//    }
//    if (battleData != NULL) {
//        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//        SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
//        if (!layer) {
//            layer = SGBattleMainLayer::create(battleData);
//        }
//
//        mainScene->setVisible(true);
//        mainScene->addShowLayer(layer);
//    }
}

