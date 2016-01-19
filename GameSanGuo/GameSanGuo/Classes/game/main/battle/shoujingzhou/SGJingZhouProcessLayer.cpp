//
//  SGJingZhouProcessLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-14.
//
//

#include "SGJingZhouProcessLayer.h"
#include "SGMainManager.h"
#include "SGShouJingZhouProcessCell.h"
#include "SGShouJingZhouProcessData.h"
#include "SGRichLabel.h"
#include "SurvivalModule.pb.h"
#include "SGStringConfig.h"
USING_NS_CC;

#define SECTIONINFOFONTSIZE   12
#define SECTIONTABLEVIEWBOTTOM   8
#define SECTIONINFOVIEWTAG -1000
#define SECTIONCELLGAP 40
#define SKIPHELPERINDEX  0

SGJingZhouProcessLayer::SGJingZhouProcessLayer()
{
	
    selectIndex = 0;
    sectiontableView = NULL;
    lastSelectIndex = 0;
    lastMoveDis = 0;
    size = CCSizeMake(960, 640);
    
    isMovingSection = false;
    
    
    
}
SGJingZhouProcessLayer::~SGJingZhouProcessLayer()
{
    
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_jingzhouprocess);
	
    
}
SGJingZhouProcessLayer *SGJingZhouProcessLayer::create(CCArray*array)
{
    SGJingZhouProcessLayer *Layer = new SGJingZhouProcessLayer();
    if (Layer && Layer->init(NULL, sg_jingzhouprocess))
    {
        Layer->_array=array;
        Layer->initView();
        Layer->autorelease();
        
        
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGJingZhouProcessLayer::initView()
{
	//    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_DELQA, this, callfuncO_selector(SGComplainLayer::deletesuccess));
    
    
    
	
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_jingzhouprocess);
    
    
	//
	//    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
	//    sgnc->addObserver(MSG_CHECKQA, this, callfuncO_selector(SGComplainLayer::checkqaresponse));
    
    datas=CCArray::create();
    datas=_array;
    datas->retain();
	
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGJingZhouProcessLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));
    
    
    
    
    SGCCLabelTTF *label = SGCCLabelTTF::create(str_JingZhouProcessLayer_str1, FONT_PANGWA, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
    //label->setColor(ccc3(0xff, 0x95, 0x0c));
    label->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    //label->setAnchorPoint(ccp(0.5, 1));
    this->addChild(label);
    
	
	
    
    //tableview
	//  tableViewHeight = skewing(65);
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
    // tableViewHeight=130;
    
    
	
}



void SGJingZhouProcessLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    //    ////测试用
    //    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    //    SGChatWithGM *chat = (SGChatWithGM*)mainScene->getChildByTag(sg_chatLayer);
    //    if(!chat)
    //    {
    //
    //        //SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
    //        chat = SGChatWithGM::create(NULL,0);
    //    }
    //
    //    SGMainLayer *mainLayer = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
    //    mainLayer->visitshow();
    //    mainScene->addShowLayer(chat);
    
    
    SGMainManager::shareMain()->showshoujingzhou();
}


void SGJingZhouProcessLayer::showFirstSectionInfo()
{
        
	SNSTableViewCell *cell = NULL;
	int rowCount = tableView->getRowCount();
	size = CCDirector::sharedDirector()->getWinSize();
	CCPoint pos = CCPointZero;
	
	for (int ii = 0; ii < rowCount; ii++) {
		cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
		if (cell != NULL)
		{
			pos = cell->getPosition();
			cell->stopAllActions();
			cell->setPosition(ccpAdd(ccp(0,pos.y), ccp(size.width, 0)));
		}
	}
	
	selectIndex = 0;
	SGShouJingZhouProcessCell *item = (SGShouJingZhouProcessCell*)tableView->dequeueReusableCellItemForIndexPath(SNSIndexPath::create(0, 0, 1));
	setSectionFirstInfo(item);
	CCPoint sectionInfoPos = sectiontableView->getPosition();
	sectiontableView->setPosition(ccp(sectionInfoPos.x + size.width,sectionInfoPos.y));
	this->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME),
									   CCCallFunc::create(this, callfunc_selector(SGJingZhouProcessLayer::dealFirstSectionInfo)),NULL));
		
    SNSTableView::showScrollItemMultiMoveFromRight(tableView);
}

void SGJingZhouProcessLayer::dealFirstSectionInfo()
{
    SNSTableViewCell *cell = NULL;
    int rowCount = tableView->getRowCount();
    size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    
    for (int ii = 0; ii < rowCount; ii++) {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            pos = cell->getPosition();
            cell->stopAllActions();
            cell->setPosition(ccp(0,pos.y));
        }
    }
    SNSTableView::showScrollItemMultiMoveFromRight(tableView);//1126效果优化
    CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-size.width*1.05, 0));
    CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(size.width*0.1, 0));
    CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-size.width*0.05, 0));
	
    CCAction *action = CCSequence::create((CCDelayTime::create(0.15f)),moveby1,moveby2,moveby3,NULL);
	
	
	sectiontableView->runAction(action);
}

void SGJingZhouProcessLayer::setSectionFirstInfo(SGShouJingZhouProcessCell *section)
{
    if (section == NULL) {
        return;
    }
    creatSectionInfoView(section);
	
    if (sectiontableView) {
        selectIndex = 0;
        m_lastShowSection = section;
        SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
        SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
        CCPoint selectViewCellPos = tableViewCell->getPosition();
        float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
		
        sectiontableView->setVisible(true);
        CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
        lastSelectIndex = selectIndex;
        lastMoveDis = moveDis;
        float posX = (section->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
        if(moveDis<0)
        {
            sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
            
            SNSTableViewCell *cell = NULL;
            for (int ii = 1; ii < tableView->getRowCount(); ii++) {
                cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
                if (cell != NULL) {
                    cell->stopAllActions();
                    cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,-sectiontableView->getContentSize().height)));
                }
            }
        }
        
        CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
        sectionInfoView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        resetMovingSectiontableView();
        tableView->resetDownLimit = sectionInfoView->getContentSize().height;
        sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
    }
}


SNSTableViewCellItem* SGJingZhouProcessLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	int index = tableView->getItemPointerWithIndexPath(indexPath);//从0开始
    //	CCSize size = tableView->getItemSize();
    //
	SGShouJingZhouProcessCell *item = (SGShouJingZhouProcessCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGShouJingZhouProcessData *data = (SGShouJingZhouProcessData*)datas->objectAtIndex(indexPath->getRow());
    
    if (item == NULL) {
        item = SGShouJingZhouProcessCell::create(this,data,index);
    }
    
    
    else
    {
        //   cell->update(_isDeleteModel,data);
    }
    //////////////////////////////////////
    return item;
    
    
    
}
void SGJingZhouProcessLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);
    ///////
}
unsigned int SGJingZhouProcessLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)

{
    return datas->count();
}



void SGJingZhouProcessLayer::showSectionInfoHandler(SGShouJingZhouProcessCell *section,int indexindex)
{
    if (isMovingSection || section == NULL) {
        return;
    }
    creatSectionInfoView(section);
    
    if (sectiontableView) {
        
        if (sectiontableView->isVisible())
        {
            if (m_lastShowSection == section) {
                if (lastMoveDis>0) {
                    tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                }
                else{
                    tableView->moveCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                }
                tableView->resetDownLimit = 0;
                moveSectiontableViewDown(false);
                lastMoveDis = 0;
                lastSelectIndex = 0;
            }
            else
            {
                CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
                sectiontableView->stopAllActions();
                sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height+4));
                hideSectiontableView();
                
                if (lastMoveDis>0) {
                    tableView->setCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                }
                else{
                    tableView->setCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                }
                
                tableView->resetDownLimit = 0;
                lastMoveDis = 0;
                lastSelectIndex = 0;
                
                SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
                SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
                CCPoint selectViewCellPos = tableViewCell->getPosition();
                float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
                
                sectiontableView->setVisible(true);
                CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
                lastSelectIndex = selectIndex;
                lastMoveDis = moveDis;
                float posX = (section->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
                if(moveDis>0)
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
                    tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
                }
                else
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                    tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
                }
                tableView->resetDownLimit = sectiontableView->getContentSize().height;
                moveSectiontableViewDown(true);
                sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
                
            }
        }
        else
        {
            SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
            SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
            CCPoint selectViewCellPos = tableViewCell->getPosition();
            float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
            
            sectiontableView->setVisible(true);
            CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().height);
            lastSelectIndex = selectIndex;
            lastMoveDis = moveDis;
            float posX = (section->bgSize.width - sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->getContentSize().width )/2;
            if(moveDis>0)
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
                tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
            }
            else
            {
                sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
            }
            tableView->resetDownLimit = sectiontableView->getContentSize().height;
            moveSectiontableViewDown(true);
            sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
        }
        
        m_lastShowSection = section;
    }
}

void SGJingZhouProcessLayer::moveSectiontableViewDown(bool isDown)
{
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    sectionInfoView->stopAllActions();
    
    if (isDown)
    {
        sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height));
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME,CCPointZero));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGJingZhouProcessLayer::resetMovingSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    else
    {
        sectiontableView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME, ccp(0,sectiontableView->getContentSize().height+4)));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGJingZhouProcessLayer::hideSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    isMovingSection = true;
}

void SGJingZhouProcessLayer::hideSectiontableView()
{
    if (sectiontableView != NULL) {
        sectiontableView->setVisible(false);
        isMovingSection = false;
    }
}

void SGJingZhouProcessLayer::resetMovingSectiontableView()
{
    if (sectiontableView != NULL) {
        isMovingSection = false;
    }
}

void SGJingZhouProcessLayer::creatSectionInfoView(SGShouJingZhouProcessCell *section)
{

	
    SGShouJingZhouProcessData *storyBean = (SGShouJingZhouProcessData *)datas->objectAtIndex(selectIndex);
    if (storyBean == NULL) {
        return;
    }
    CCSprite *sectionInfoView = NULL;
	
    float contentHeight = 0;
    
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_jingzhouprocess);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_jingzhouprocess, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_jingzhouprocess);
    
    if (sectiontableView == NULL)
    {
		
		sectionInfoView = CCSprite::createWithSpriteFrameName("bban.png");
        
        sectionInfoView->setAnchorPoint(CCPointZero);
        sectionInfoView->ignoreAnchorPointForPosition(false);
        sectionInfoView->setPosition(ccp((section->bgSize.width - sectionInfoView->getContentSize().width)/2,150));
        sectionInfoView->setOpacity(255*0.6);
        sectiontableView = new NewScrollView();
        if (sectiontableView && sectiontableView->initGSScrollViewFrame(CCRectMake(0, 0, section->bgSize.width, sectionInfoView->getContentSize().height)))
        {
            sectiontableView->autorelease();
            CCPoint tableViewPos = tableView->getParent()->convertToWorldSpace(tableView->getPosition());
            sectiontableView->setParentScissorFrame(CCRectMake(tableViewPos.x, tableViewPos.y, tableView->getScissorFrame().size.width, tableView->getScissorFrame().size.height));
        }
        
        sectiontableView->setAnchorPoint(CCPointZero);
        sectiontableView->ignoreAnchorPointForPosition(false);
        sectiontableView->addChild(sectionInfoView,1,SECTIONINFOVIEWTAG);
        
        contentHeight = sectionInfoView->getContentSize().height;
        
		//        infoLabel1 = SGCCLabelTTF::create("体力消耗:", FONT_BOXINFO, fontWid);
		//        infoLabel1->setPosition(ccp(30,contentHeight- SECTIONCELLGAP));
		//        infoLabel1->setAnchorPoint(CCPointZero);
		//        infoLabel1->setColor(ccYELLOW);
		//        sectionInfoView->addChild(infoLabel1,1,-9);
		//
		int mm = datas->count()-selectIndex-1;
		if(mm == 0)
		{
			SGCCLabelTTF *label = SGCCLabelTTF::create(str_JingZhouProcessLayer_str2, FONT_BOXINFO, 32);
			sectionInfoView->addChild(label);
			label->setPosition(ccp(sectionInfoView->getContentSize().width*0.5, sectionInfoView->getContentSize().height*0.5));
		}
		else
		{
			//从第5波开始，您将已0损失进入下一波，同时获得前5波的所有军功奖励xxxx
			//您确认要从第5波开始吗？
			CCString * words=CCString::createWithFormat(str_JingZhouProcessLayer_str3,
														storyBean->getlevel(), storyBean->getlevel()-1, storyBean->getjunhun());
			
			info = SGRichLabel::getFomLabel(words->getCString(), CCSizeMake(460,350),FONT_BOXINFO, 28,ccc3(0xff, 0xc0, 0x00));
			
			info->setAnchorPoint(ccp(0.5, 0.5));
			sectionInfoView->addChild(info,10,10);
			info->setPosition(ccp(sectionInfoView->getContentSize().width*0.5, sectionInfoView->getContentSize().height*0.15));
		}
		
        sectiontableView->setVisible(false);
        tableView->m_body->addChild(sectiontableView);
    }
    else
    {
        
		SGShouJingZhouProcessData *story = (SGShouJingZhouProcessData *)datas->objectAtIndex(selectIndex);
        sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
        contentHeight = sectiontableView->getContentSize().height;
        
        sectionInfoView->removeAllChildren();
		//        CCString * words=CCString::createWithFormat("可直接获得军魂/#%d/#铜钱/#%d/#(您到本关的最高记录)。若想刷新记录，可以尝试从前面的关卡开始。并努力在最后一击扩大对敌人的伤害.",story->getjunhun(),story->getgold());
		//        info=NULL;
		//        info = SGRichLabel::getFomLabel(words->getCString(), CCSizeMake(450,300),FONT_BOXINFO, 29,ccc3(0xff, 0xc0, 0x00));
		////        SGRichLabel::setString(words->getCString(), info,(CCNode*) sectionInfoView);
		//
		//     //  sectionInfoView->getChildByTag(10))
		//        sectionInfoView->addChild(info);
		//        info->setAnchorPoint(ccp(0.5, 0.5));
		//        info->setPosition(ccp(sectionInfoView->getContentSize().width*0.5, sectionInfoView->getContentSize().height*0.15));
		//
		int mm=datas->count()-selectIndex-1;
        if(mm==0)
        {
            SGCCLabelTTF *label = SGCCLabelTTF::create(str_JingZhouProcessLayer_str4, FONT_BOXINFO, 32);
            sectionInfoView->addChild(label);
            label->setPosition(ccp(sectionInfoView->getContentSize().width*0.5, sectionInfoView->getContentSize().height*0.5));
            
            
        }
        else
        {
            CCString * words=CCString::createWithFormat(str_JingZhouProcessLayer_str5,
														storyBean->getlevel(), storyBean->getlevel()-1, storyBean->getjunhun());

            
            info = SGRichLabel::getFomLabel(words->getCString(), CCSizeMake(460,350),FONT_BOXINFO, 28,ccc3(0xff, 0xc0, 0x00));
            
            info->setAnchorPoint(ccp(0.5, 0.5));
            sectionInfoView->addChild(info,10,10);
            info->setPosition(ccp(sectionInfoView->getContentSize().width*0.5, sectionInfoView->getContentSize().height*0.15));
        }
        
        
        
		
    }
    
}
//void SGStorySectionLayer::dealHideSingleMove()
//{
//    int rowCount = tableView->getRowCount();
//    SGStorySectionCell *item = NULL;
//    SNSTableViewCell *cell =  NULL;
//    for (int ii = 0; ii < rowCount; ii++) {
//        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
//        if (cell != NULL) {
//            item = (SGStorySectionCell *)(cell->getChildByTag(0));
//            if (item->_bigBGBtn) {
//                item->_bigBGBtn->setEnabled(false);
//            }
//        }
//    }
//    hideScrollItemSingleMove();
//}

void SGJingZhouProcessLayer::entrySectionHandler(SGShouJingZhouProcessCell *section,int index)
{
    if (section == NULL ) {
        return;
    }
    if (isMovingSection) {
        return;
    }
    
    
    main::SurvivalStartRequest* req = new main::SurvivalStartRequest();
    if(index == datas->count()-1)
    {//如果是从头开始
		req->set_isjump(0);
    }
    else
    {
		//如果不是
		req->set_isjump(1);
    }
    /////取点击的关卡数
	
    
    SGShouJingZhouProcessData *data = (SGShouJingZhouProcessData*)datas->objectAtIndex(selectIndex);
    
    ///////////
    req->set_levels(data->getlevel());
    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_START, req);
    
    
}














