 //
//  SGMainTaskLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-14.
//
//

#include "SGMainTaskLayer.h"
#include "SGMainManager.h"

#include   "SGStaticDataManager.h"
#include "SGPlayerInfo.h"
#include "SGStringConfig.h"

SGMainTaskLayer::SGMainTaskLayer(){
    sectiontableView=NULL;
    lastSelectIndex = 0;
    lastMoveDis = 0;
    lastShowHeight=0;
    subTaskArray=NULL;
    sectionInfoView=NULL;
    isUpdate=false;
    ishasValue=true;
    
}
SGMainTaskLayer::~SGMainTaskLayer(){

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_mainTasklayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_GETMAINTASK_REWARD);
}

#define SECTIONINFOFONTSIZE   12
#define SECTIONTABLEVIEWBOTTOM   8
#define SECTIONINFOVIEWTAG -1000
#define SECTIONCELLGAP 40
#define SKIPHELPERINDEX  0
#define FONT_SIZE 25

SGMainTaskLayer *  SGMainTaskLayer::create(CCArray *data){

    SGMainTaskLayer *layer=new SGMainTaskLayer();
    
    if (layer && layer->init(NULL, sg_mainTasklayer)) {
        layer->datas=SGPlayerInfo::sharePlayerInfo()->getTaskGroup();
        layer->datas->retain();
        layer->initView();
        layer->autorelease();
        return layer;
        
    }
    CC_SAFE_DELETE(layer);
    return layer;
}
void SGMainTaskLayer::initView(){

    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_mainTasklayer);
 
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_GETMAINTASK_REWARD,
                              this,
                              callfuncO_selector(SGMainTaskLayer::getRewardListener));
    subTaskArray=CCArray::create();
    subTaskArray->retain();
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
   
	tableView->setDown(-25);
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH  - 110 ));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
	tableView->setPageNEnable(false);
    
    
}

void SGMainTaskLayer::entrySectionHandler(SGMainTaskCell *item){

}
void SGMainTaskLayer::showSectionInfoHandler(SGMainTaskCell *section){
    if (isMovingSection || section == NULL) {
        return;
    }
    createSectionInfoView(section);
    if (!ishasValue) {
        return;
    }
    
    if (sectiontableView) {
        
        if (sectiontableView->isVisible())
        {
            if (m_lastShowSection == section && !isUpdate) {
              
                /*if (lastMoveDis>0) {
//                    tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
                    
                    tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-lastShowHeight);
                }
                else{
//                    tableView->moveCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                    tableView->moveCellByDistance(lastSelectIndex,lastShowHeight,true);

                }
                tableView->resetDownLimit = 0;
                moveSectiontableViewDown(false);
                lastMoveDis = 0;
                lastSelectIndex = 0;
                 */
                
                closeSubSection(section);
            }
            else
            {
                
               
//                CCLayer *sectionInfoView = (CCLayer *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
                sectiontableView->stopAllActions();
                sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height+4));
                hideSectiontableView();
                
                if (lastMoveDis>0) {
//                    tableView->setCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
//                     tableView->setCellByDistance(lastSelectIndex,lastMoveDis,false,-lastShowHeight);
                     tableView->setCellByDistance(lastSelectIndex,lastShowHeight,true,-lastShowHeight);
                }
                else{
//                    tableView->setCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
                    tableView->setCellByDistance(lastSelectIndex,lastShowHeight,true);
                }
                
                tableView->resetDownLimit = 0;
                
//                lastMoveDis = 0;
//                lastSelectIndex = 0;
                
                SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
                SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
                CCPoint selectViewCellPos = tableViewCell->getPosition();
//                float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height - selectViewCellPos.y;
                float moveDis = SECTIONTABLEVIEWBOTTOM+lastShowHeight-sectiontableView->getContentSize().height - selectViewCellPos.y;
                
                sectiontableView->setVisible(true);
                CCPoint infoViewPos = ccp(tableView->getContentSize().width/2 - section->bgSize.width/2,selectViewCellPos.y + (-sectiontableView->getContentSize().height));
               
              
                lastSelectIndex = selectIndex;
                lastMoveDis = moveDis;
                float posX = (section->bgSize.width - sectionInfoView->getContentSize().width )/2;
                if(moveDis>0)
                {
                     sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                    tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
//                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,moveDis+6)));
//                    tableView->moveCellByDistance(selectIndex,-moveDis,false,sectiontableView->getContentSize().height);
    
                }
                else
                {
                    sectiontableView->setPosition(ccpAdd(infoViewPos, ccp(posX,SECTIONTABLEVIEWBOTTOM)));
                    tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
                }
                
                
                tableView->resetDownLimit = sectiontableView->getContentSize().height-SGLayout::getSkewingY(140);
          
//                tableView->resetDownLimit = lastShowHeight;
               
                moveSectiontableViewDown(true);
              
//                sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
                
//                SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
//                SGMainTaskCell*  newSection = (SGMainTaskCell *)tableView->getCellByIndexPath(newIndexPath);
//                
//                SNSIndexPath* lastIndex = SNSIndexPath::create(lastSelectIndex, 1);
//                SGMainTaskCell*  lastSection = (SGMainTaskCell *)tableView->getCellByIndexPath(lastIndex);
//                
//                
//                closeSubSection(lastSection);
//                openSubSection(lastSection);
                
               
                
                
            }
        }
        else
        {
            /*
            SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
            SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
            CCPoint selectViewCellPos = tableViewCell->getPosition();
            float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height- (selectViewCellPos.y+1000);
            
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
//                tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
                tableView->moveCellByDistance(selectIndex,-lastShowHeight,true);
            }
            
            
//            tableView->resetDownLimit = sectiontableView->getContentSize().height;
             tableView->resetDownLimit = lastShowHeight;
            moveSectiontableViewDown(true);
//            sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
            
           */
            openSubSection(section);
        }
        
        m_lastShowSection = section;
    }

       isUpdate=false;
}

void SGMainTaskLayer::openSubSection(SGMainTaskCell *section)
{
    SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
    SNSTableViewCell*  tableViewCell = tableView->getCellByIndexPath(newIndexPath);
    CCPoint selectViewCellPos = tableViewCell->getPosition();
    float moveDis = SECTIONTABLEVIEWBOTTOM+sectiontableView->getContentSize().height- (selectViewCellPos.y+1000);
    
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
        //                tableView->moveCellByDistance(selectIndex,-sectiontableView->getContentSize().height,true);
        tableView->moveCellByDistance(selectIndex,-lastShowHeight,true);
    }
    
    
    //            tableView->resetDownLimit = sectiontableView->getContentSize().height;
    tableView->resetDownLimit = lastShowHeight;
    moveSectiontableViewDown(true);
    //            sectiontableView->getChildByTag(SECTIONINFOVIEWTAG)->setScaleX(section->bgSize.width/sectiontableView->getContentSize().width);
}
void SGMainTaskLayer::closeSubSection(SGMainTaskCell *section)
{
    if (lastMoveDis>0) {
        //                    tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-sectiontableView->getContentSize().height);
        
//        tableView->moveCellByDistance(lastSelectIndex,lastMoveDis,false,-lastShowHeight);
         tableView->moveCellByDistance(lastSelectIndex,lastShowHeight,true);
    }
    else{
        //                    tableView->moveCellByDistance(lastSelectIndex,sectiontableView->getContentSize().height,true);
        tableView->moveCellByDistance(lastSelectIndex,lastShowHeight,true);
        
    }
    tableView->resetDownLimit = -SGLayout::getSkewingY(110);
    moveSectiontableViewDown(false);
    lastMoveDis = 0;
    lastSelectIndex = 0;
}

// 根据indexPath返回tableView的cell
SNSTableViewCellItem *SGMainTaskLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath){
    
    SGMainTaskCell  *cell=(SGMainTaskCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (cell==NULL) {
        cell=SGMainTaskCell::create(this);
    }
    int index=indexPath->getRow();
    SGMainTaskGroup *group=(SGMainTaskGroup *)datas->objectAtIndex(index);
    cell->updateInfo(group->gettaskGroupName(),group->gethasFinish());
    return cell;
}
// 返回tableView的cell数
unsigned int SGMainTaskLayer ::tableViewAllItemsNumber(SNSTableView* tableView, int section){

    return datas->count();
}

void SGMainTaskLayer:: tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath){
    
    SGMainTaskGroup *group=(SGMainTaskGroup *)datas->objectAtIndex(indexPath->getRow());
    if (subTaskArray) {
        //         subTaskArray->removeAllObjects();
        //        subTaskArray=CCArray::create();
        //        subTaskArray->retain();
        ishasValue=true;
    }
    
    subTaskArray=SGPlayerInfo::sharePlayerInfo()->getSubTaskByGroupName(group->gettaskGroupName()->getCString());
    
    if (subTaskArray==NULL) {
        ishasValue=false;
        return;
    }
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);
     isMovingSection = false;

    
}
void SGMainTaskLayer::createSectionInfoView(SGMainTaskCell *section){

   
    
    if (section==NULL) {
        return;
    }
    
    SGMainTaskGroup *group=(SGMainTaskGroup *)datas->objectAtIndex(section->getIndexPath()->getRow());
    if (subTaskArray) {
//         subTaskArray->removeAllObjects();
//        subTaskArray=CCArray::create();
//        subTaskArray->retain();
        ishasValue=true;
    }
   
    subTaskArray=SGPlayerInfo::sharePlayerInfo()->getSubTaskByGroupName(group->gettaskGroupName()->getCString());
    
    if (subTaskArray==NULL) {
        ishasValue=false;
        return;
    }
    int count=subTaskArray->count();
//     int count=section->getIndexPath()->getRow()+1;
 
    if (sectiontableView==NULL) {
            sectionInfoView = new CCLayer();
//        CCLayerColor *sectionInfoView = CCLayerColor::create(ccc4(10, 10, 100, 255), 920, 1200);
       
        CCSize winSize=CCDirector::sharedDirector()->getWinSize();
        float sectionLayerwidth=0;
        if (winSize.height==1136) {
            sectionLayerwidth=60;
        }else if (winSize.height==960)
        {
            sectionLayerwidth=60;
        }
        else
            sectionLayerwidth= -section->getPositionX()+section->getContentSize().width/3*2;
            
        
        for (int i=0; i<count; i++) {
             SGMainTaskDataModel *datamodel=new SGMainTaskDataModel();
            datamodel=(SGMainTaskDataModel *)subTaskArray->objectAtIndex(i);
          
            SGMainTaskSubLayer *sectionLayer=SGMainTaskSubLayer::create(this);
             sectionLayer->setTag(i);
            sectionLayer->updateTaskSubLayer(datamodel);
            
            sectionLayer->setAnchorPoint(CCPointZero);
            sectionLayer->setTag(i);
            sectionLayer->ignoreAnchorPointForPosition(false);
//            sectionLayer->setPosition(ccp(-winSize.width/10,sectionLayer->bgSize.height*(count-i-1)+sectionLayer->bgSize.height/2));
             sectionLayer->setPosition(ccp(-sectionLayerwidth,sectionLayer->bgSize.height*(count-i-1)+sectionLayer->bgSize.height/2));
            sectionInfoView->addChild(sectionLayer);
            sectionInfoView->setContentSize(CCSizeMake(sectionLayer->bgSize.width,sectionLayer->bgSize.height*count));
        }
          sectiontableView=new NewScrollView();
        if (sectiontableView && sectiontableView->initGSScrollViewFrame(CCRectMake(0, 0, section->bgSize.width, sectionInfoView->getContentSize().height)) ) {
            sectiontableView->autorelease();
            
            CCPoint tableViewPos = tableView->getParent()->convertToWorldSpace(tableView->getPosition());
            sectiontableView->setParentScissorFrame(CCRectMake(tableViewPos.x, tableViewPos.y, tableView->getScissorFrame().size.width, tableView->getScissorFrame().size.height));
        }
        sectiontableView->setAnchorPoint(CCPointZero);
        sectiontableView->ignoreAnchorPointForPosition(false);
        sectiontableView->addChild(sectionInfoView,1,SECTIONINFOVIEWTAG);

        sectiontableView->setVisible(false);
        tableView->m_body->addChild(sectiontableView,10);
        lastShowHeight=sectionInfoView->getContentSize().height;
        isSectionVisible=false;
        
        
    }
    else
    {
        if(m_lastShowSection!=section || isUpdate)
        {
            
            lastShowHeight=sectiontableView->getContentSize().height;
            
            sectionInfoView->removeAllChildren();
 
            CCSize winSize=CCDirector::sharedDirector()->getWinSize();
            float sectionLayerwidth=0;
            if (winSize.height==1136) {
                sectionLayerwidth=60;
            }else if (winSize.height==960)
            {
                sectionLayerwidth=60;
            }
            else
                sectionLayerwidth= -section->getPositionX()+section->getContentSize().width/3*2;
            for (int i=0; i<count; i++) {
                SGMainTaskDataModel *datamodel=new SGMainTaskDataModel();
                datamodel=(SGMainTaskDataModel *)subTaskArray->objectAtIndex(i);
                
                SGMainTaskSubLayer *sectionLayer=SGMainTaskSubLayer::create(this);
                sectionLayer->setTag(i);
                sectionLayer->updateTaskSubLayer(datamodel);
                sectionLayer->setAnchorPoint(CCPointZero);
                sectionLayer->ignoreAnchorPointForPosition(false);
                sectionLayer->setPosition(ccp(-sectionLayerwidth,sectionLayer->bgSize.height*(count-i-1)+sectionLayer->bgSize.height/2));
                 
               
                sectionInfoView->addChild(sectionLayer);
                sectionInfoView->setContentSize(CCSizeMake(sectionLayer->bgSize.width,sectionLayer->bgSize.height*count));
            }
            
            
//             sectiontableView=new NewScrollView();
            sectiontableView->setContentSize(CCSizeMake(sectionInfoView->getContentSize().width, sectionInfoView->getContentSize().height));
            
            
            sectiontableView->setFrame(CCRectMake(0, 0, section->bgSize.width, sectionInfoView->getContentSize().height));
//            if (sectiontableView && sectiontableView->initGSScrollViewFrame(CCRectMake(0, 0, section->bgSize.width, sectionInfoView->getContentSize().height)) ) {
//                sectiontableView->autorelease();
//                
//                CCPoint tableViewPos = tableView->getParent()->convertToWorldSpace(tableView->getPosition());
//                sectiontableView->setParentScissorFrame(CCRectMake(tableViewPos.x, tableViewPos.y, tableView->getScissorFrame().size.width, tableView->getScissorFrame().size.height));
//            }

            
            if(!sectiontableView->isVisible())
            {
                lastShowHeight=sectiontableView->getContentSize().height;
                isSectionVisible=false;
            
            }else
                isSectionVisible=true;
            
//            sectiontableView->setVisible(true);
        }else {
            
            lastShowHeight=sectiontableView->getContentSize().height;
        }
    }
    
}
void SGMainTaskLayer::resetMovingSectiontableView()
{
    if (sectiontableView != NULL) {
        isMovingSection = false;
    }
}

void SGMainTaskLayer::hideSectiontableView()
{
    if (sectiontableView != NULL) {
        sectiontableView->setVisible(false);
        isMovingSection = false;
    }
}
void SGMainTaskLayer::moveSectiontableViewDown(bool isDown)
{
//    CCLayer *sectionInfoView = (CCLayer *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    sectionInfoView->stopAllActions();
    
    if (isDown)
    {
        sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height));
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME,CCPointZero));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGMainTaskLayer::resetMovingSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    else
    {
        sectiontableView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME, ccp(0,sectiontableView->getContentSize().height+4)));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGMainTaskLayer::hideSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    isMovingSection = true;
}

void SGMainTaskLayer::showIsFinishTask(){

    selectIndex=getTaskFinishIndex();
    updateMainTaskSubLayer(NULL);
    
}

void SGMainTaskLayer::showRunAction(){
     SNSTableView::showScrollItemMultiMoveFromRight(this->getSNSTavleView());
    CCSize size= CCDirector::sharedDirector()->getWinSize();
    
    if (sectionInfoView) {
        sectionInfoView->setPosition(ccpAdd(sectionInfoView->getPosition(), ccp(size.width*1 , 0)));
        CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-size.width*1.05, 0));
        CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(size.width*0.1, 0));
        CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-size.width*0.05, 0));
        
        CCAction *action = CCSequence::create((CCDelayTime::create(0.15f)),moveby1,moveby2,moveby3,NULL);
        
        
        sectiontableView->runAction(action);
    }
   
}
int SGMainTaskLayer::getTaskFinishIndex(){
   
    int hasTaskIndex=0;
    bool firstTask=false;
    for (int i=0; i<datas->count(); i++) {
        SGMainTaskGroup *dataGroup=(SGMainTaskGroup *)datas->objectAtIndex(i);
        if (dataGroup->gethasFinish()) {
            return i;
        }
        if (dataGroup->gethasTask() && !firstTask) {
            hasTaskIndex =i ;
            firstTask=true;
        }
    }
    
    return hasTaskIndex;

}
void SGMainTaskLayer::getRewardListener(CCObject *obj){
    
    SGSocketRequest *sr=(SGSocketRequest *)obj;
    if (sr) {
        main::ReceiveQuestRewardResponse *rs=(main::ReceiveQuestRewardResponse *)sr->m_msg;
        if (rs->state()) {
            
            //            SGPlayerInfo::sharePlayerInfo()->removeSubTask(_dataModel);
            this->updateMainTaskSubLayer(NULL);
            
            SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str20);
            
        }else
        {
            
            SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str21);
        }
    }
}


#pragma mark SGMainTaskSubDelegate 
void SGMainTaskLayer:: updateMainTaskSubLayer(SGMainTaskSubLayer *subLayer)
{
    
    SNSIndexPath* newIndexPath = SNSIndexPath::create(selectIndex, 1);
    SGMainTaskCell  *cell=(SGMainTaskCell *)tableView->dequeueReusableCellItemForIndexPath(newIndexPath);
    isUpdate=true;
    showSectionInfoHandler(cell);
     SGMainTaskGroup *group=(SGMainTaskGroup *)datas->objectAtIndex(selectIndex);
    cell->updateNotice(group->gethasFinish());
    showRunAction();
//    showIsFinishTask();
    
}

#pragma mark SGMainTaskSubLayer



SGMainTaskSubLayer::SGMainTaskSubLayer()
:_dataModel(NULL),
bgSize(CCSizeZero),
questId(0),
kuang_sprite(NULL),
reward_sprite(NULL),
task_Name(NULL),
reward_count(NULL),
reward_name(NULL),
task_description(NULL),
task_progress(NULL),
task_finish(NULL),
button(NULL)
{}
SGMainTaskSubLayer::~SGMainTaskSubLayer(){


}

SGMainTaskSubLayer * SGMainTaskSubLayer::create(SGMainTaskSubLayerDelegate *delegate){
    SGMainTaskSubLayer *subLayer=new SGMainTaskSubLayer();
    subLayer->_delegate=delegate;
    if (subLayer && subLayer->init()) {
        
        subLayer->initMsg();
        subLayer->autorelease();
        return subLayer;
    }
    CC_SAFE_DELETE(subLayer);
    return subLayer;
}

void SGMainTaskSubLayer::initMsg(){
    
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_mainTasklayer);
   
    
    CCSprite *bigBg=CCSprite::createWithSpriteFrameName("yban.png");
    bgSize=CCSizeMake(bigBg->getContentSize().width, bigBg->getContentSize().height*0.7);
    bigBg->setTextureRect( CCRectMake(0, bigBg->getContentSize().height*0.3, bigBg->getContentSize().width, bigBg->getContentSize().height*0.7));
    bigBg->setAnchorPoint(ccp(0.5,0.5));
    bigBg->ignoreAnchorPointForPosition(false);
    bigBg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter),ccp(0,0)));
    bigBg->setOpacity(255*0.6);
    this->addChild(bigBg);
    
    
    kuang_sprite=CCSprite::createWithSpriteFrameName("equipPinzhi_4_2.png");
    kuang_sprite->setAnchorPoint(ccp(0.5,0.5));
    kuang_sprite->ignoreAnchorPointForPosition(false);
    kuang_sprite->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter),ccp(-bigBg->getContentSize().width/2+kuang_sprite->getContentSize().width/2,bgSize.height/2-kuang_sprite->getContentSize().height/2-10)));
    this->addChild(kuang_sprite,1);
   
    
    reward_sprite=CCSprite::createWithSpriteFrameName("big_yb.png");
    reward_sprite->setAnchorPoint(ccp(0.5,0.5));
    reward_sprite->ignoreAnchorPointForPosition(false);
    reward_sprite->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(0,0)));
    this->addChild(reward_sprite,0);
    
    
    task_Name=CCLabelTTF::create(str_MainTaskLayer_str1, "", FONT_SIZE);
//    task_Name->setDimensions(CCSizeMake(530*.85f, 260*.35f));
    task_Name->setAnchorPoint(ccp(0.5,0.5));
    task_Name->ignoreAnchorPointForPosition(true);
    task_Name->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(kuang_sprite->getContentSize().width*0.5,bgSize.height/2-reward_sprite->getContentSize().height*0.5)));
    
    task_Name->setHorizontalAlignment(kCCTextAlignmentLeft);
    task_Name->setColor(ccYELLOW);

    this->addChild(task_Name);
    
    reward_count=CCLabelTTF::create("500", FONT_BOXINFO, FONT_SIZE);
    reward_count->setAnchorPoint(ccp(0.5,0.5));
    reward_count->ignoreAnchorPointForPosition(true);
    reward_count->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(kuang_sprite->getContentSize().width*0.5,-kuang_sprite->getContentSize().height*0.5)));
    this->addChild(reward_count);
    reward_count->setColor(ccYELLOW);
    
    reward_name=CCLabelTTF::create(str_yuanbao, FONT_BOXINFO, FONT_SIZE);
    reward_name->setAnchorPoint(ccp(0.5,0.5));
    reward_name->ignoreAnchorPointForPosition(false);
    reward_name->setPosition(ccpAdd(reward_count->getPosition(),ccp(70,0)));
//    this->addChild(reward_name);
    reward_name ->setColor(ccYELLOW);
    
    task_description=CCLabelTTF::create(str_AllTaskLayer_str6, "", FONT_SIZE-3);
    task_description->setAnchorPoint(ccp(0.5,0.5));
    task_description->ignoreAnchorPointForPosition(true);
    task_description->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(kuang_sprite->getContentSize().width*0.5,bgSize.height/2-reward_sprite->getContentSize().height)));
    this->addChild(task_description);
    task_description->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    task_progress=CCLabelTTF::create(str_AllTaskLayer_str7, "", FONT_SIZE);
    task_progress->setAnchorPoint(ccp(0.5,0.5));
    task_progress->ignoreAnchorPointForPosition(true);
    task_progress->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(kuang_sprite->getContentSize().width*0.5,bgSize.height/2-reward_sprite->getContentSize().height*1.4)));
    this->addChild(task_progress);
    task_progress->setColor(ccGREEN);
    task_progress->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    task_finish=CCLabelTTF::create(str_AllTaskLayer_str8, FONT_BOXINFO, FONT_SIZE);
    task_finish->setAnchorPoint(ccp(0.5,0.5));
    task_finish->ignoreAnchorPointForPosition(true);
    task_finish->setPosition(ccpAdd(reward_count->getPosition(),ccp(250,0)));
    this->addChild(task_finish);
    task_finish->setVisible(false);
    
    
    CCMenu *menu=CCMenu::create();
    menu->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 0)));
    this->addChild(menu,1);
    button = SGButton::createFromLocal("store_exchangebtnbg.png", str_AllTaskLayer_str10, this, menu_selector(SGMainTaskSubLayer::getRewardHandle),CCPointZero,FONT_PANGWA,ccWHITE,22);
	button->setScale(1.05);
    menu->addChild(button);
    button->setPosition(ccpAdd(ccp(0, 0) ,ccp(150,-30)));
    button->setVisible(true);
    
    
    
}
 

void SGMainTaskSubLayer::getRewardHandle(){
//
    main::ReceiveQuestRewardRequest *request = new main::ReceiveQuestRewardRequest();
    request->set_questid(_dataModel->gettaskId());
    SGSocketClient::sharedSocketClient()->send(MSG_GETMAINTASK_REWARD, request);
    
   
    
}

void SGMainTaskSubLayer::updateTaskSubLayer(SGMainTaskDataModel *dataModel){
    _dataModel=dataModel;
//    return;
    if (dataModel->getcomplateState()!=2) {
        task_finish->setVisible(true);
        button->setVisible(false);
    }
    else
    {
        button->setVisible(true);
        task_finish->setVisible(false);
       
    }
    if(dataModel->getshowType()==2) {
        task_progress->setVisible(true);
        
    }else
        task_progress ->setVisible(false);
    
//    return;
    SGRewardDataModel* rewardModel = dataModel->rewardModel;
    task_Name->setString(dataModel->gettaskName()->getCString());
    task_description->setString(dataModel->gettaskDeprision()->getCString());
    task_progress->setString(CCString::createWithFormat(str_AllTaskLayer_str11,dataModel->getcurProgress(),dataModel->getallProgress())->getCString());
    if (rewardModel == NULL) {
        return;
    }
    
    int type = 0;
    int value = 0;
    int number = 0;
    CCSprite* itemIcon = NULL;
    CCSprite *jinkuang = NULL;
    SGCCLabelTTF * itemName = NULL;
    SGMainManager * sgMainManager = SGMainManager::shareMain();
    //            CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    int rewardCount = rewardModel->rewardArray->count();
    for(int i=0; i<rewardCount && i<4; i++)
    {
        sscanf(((CCString*)rewardModel->rewardArray->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
        switch (type) {
            case 0:
            {
                //                        cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("big_yb.png");
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("元宝x%d",value)->getCString(), FONT_BOXINFO, 20);
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str9,value)->getCString());
            }
                break;
            case 1:
            {
                
                sgMainManager->addHeadIconPlistByNum(9999,sg_mainTasklayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str12,value)->getCString());
                //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("铜钱x%d",value)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case 2:
            {
                sgMainManager->addHeadIconPlistByNum(9999,sg_storySectionLayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str13,value)->getCString());
                //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("Pvp积分x%d",value)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case 3:
            {
                
                sgMainManager->addHeadIconPlistByNum(9997,sg_mainTasklayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9997.png");
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str14,value)->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_AllTaskLayer_str14,value)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case 4:
            {
                
                sgMainManager->addHeadIconPlistByNum(9998,sg_mainTasklayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str15,value)->getCString());
                //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("小喇叭x%d",value)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case 5:
            {
                
                sgMainManager->addHeadIconPlistByNum(9998,sg_mainTasklayer);
                jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str16,value)->getCString());
                
                itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_liandian,value)->getCString(), FONT_BOXINFO, 20);
            }
                break;
            case 6:
            {
                
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(value);
                sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                 countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                kuang_sprite->addChild(countryName,30,30);
                //
                //                        if (temp->getAdvNum() > 0) {
                //                            CCSprite *advNum = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("+%d.png",temp->getAdvNum()))->getCString());
                //                            advNum->setAnchorPoint(ccp(1,1));
                //                            advNum->ignoreAnchorPointForPosition(false);
                //                            advNum->setPosition(ccp(jinkuang->getContentSize().width*0.8,jinkuang->getContentSize().height - advNum->getContentSize().height));
                //                            jinkuang->addChild(advNum,40,40);
                //                        }
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str17,temp->getOfficerName()->getCString(),number)->getCString());
                
            }
                break;
            case 7:
            {
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
//                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
                itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                //                        if (temp->getAdvNum() > 0) {
                //                            CCSprite *advNum = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("+%d.png",temp->getAdvNum()))->getCString());
                //                            advNum->setAnchorPoint(ccp(1,1));
                //                            advNum->ignoreAnchorPointForPosition(false);
                //                            advNum->setPosition(ccp(jinkuang->getContentSize().width*0.8,jinkuang->getContentSize().height - advNum->getContentSize().height));
                //                            jinkuang->addChild(advNum,40,40);
                //                        }
                
                kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getEquipName()->getCString(),number)->getCString());
                
            }
                break;
            case 9:
            {
                  SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(value);
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
//                SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(value);
                
                int starlvl = temp->getPropsCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                if (temp->getPropsPropsType()==1) {
                    jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
                    itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                    
                    
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                    kuang_sprite->addChild(countryName,30,30);
                    
                    kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                    reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                    reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getPropsName()->getCString(),number)->getCString());
                }else
                {
                
                    jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
                    itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                    
                    kuang_sprite->setDisplayFrame(jinkuang->displayFrame());
                    reward_sprite->setDisplayFrame(itemIcon->displayFrame());
                    reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getPropsName()->getCString(),number)->getCString());
                }
            }
                break;
            default:
                break;
        }
        
    }
    

   
}


