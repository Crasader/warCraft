//
//  SGAllTaskLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-3-27.
//
//

#include "SGAllTaskLayer.h"
#include "SGMainManager.h"
#include "SGAllTaskCell.h"
#include "SGFirstLayer.h"
#include "SGStaticDataManager.h"
#include "DayTaskModule.pb.h"
#include "SGShowRewardBox.h"
#include "SGMainTaskLayer.h"
#include <algorithm>
#include "SGStringConfig.h"
#include "SGGuideManager.h"

#define BTNTAG   -2222
#define SECTIONINFOFONTSIZE   12
#define SECTIONTABLEVIEWBOTTOM   8
#define SECTIONINFOVIEWTAG -1000
#define SECTIONCELLGAP 40
#define SKIPHELPERINDEX  0
#define FONT_SIZE 22

#define reward_sprite_tag 1919

bool SGAllTaskLayer::m_buttonCanTouch = true;

SGAllTaskLayer::SGAllTaskLayer()
:_array(NULL),
sectiontableView(NULL),
isMovingSection(false),
m_lastShowSection(NULL),
lastMoveDis(0),
selectIndex(0), // 选中的场景
lastSelectIndex(0), //上次选中的场景
size(CCSizeZero),
bgSize(CCSizeZero),
typetype(0),
allid(0),//各种ID
num(0),//每日完成数量
numnum(0),//非每日完成数量
title(NULL),
isshow(false),
reward_sprite(NULL),
m_itemId(-1),
m_itemType(-1),
//canTouchTitle(true)
_sectionMenu(NULL)
{
    
    selectIndex = 0;
    sectiontableView = NULL;
    lastSelectIndex = 0;
    lastMoveDis = 0;
    size = CCSizeMake(960, 640);
    
    isMovingSection = false;
    typetype=0;
    allid=-10;
    num=0;
    numnum=0;
    isshow=false;
}


SGAllTaskLayer::~SGAllTaskLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_DAYTASK_LIST);
    notification->removeObserver(this, MSG_DAYTASK_STATUS);
    notification->removeObserver(this, MSG_GETMAINTASK_REWARD);
    notification->removeObserver(this, MSG_REWARD_GODENTER);
    notification->removeObserver(this, MSG_REWARD_SALARYENTER);
    datas->release();
    
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_alltasklayer);
    
    //@!@!
    SGAllTaskLayer::m_buttonCanTouch = true;
}

static int IDMore(const CCObject* p1, const CCObject* p2)
{
	return (((SGMainTaskDataModel *)p1)->gettaskId() > ((SGMainTaskDataModel *)p2)->gettaskId());
}
static int IDLess(const CCObject* p1, const CCObject* p2)
{
	return (((SGMainTaskDataModel *)p1)->gettaskId() < ((SGMainTaskDataModel *)p2)->gettaskId());
}



static int ArrivementLess(const CCObject* p1, const CCObject* p2)
{
	return (((SGMainTaskDataModel *)p1)->getArrivement() < ((SGMainTaskDataModel *)p2)->getArrivement());
}




SGAllTaskLayer *SGAllTaskLayer::create(CCArray *array)
{
    SGAllTaskLayer *Layer = new SGAllTaskLayer();
    if (Layer && Layer->init(NULL, sg_alltasklayer))
    {
      
        Layer->_array=array;
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGAllTaskLayer::initView()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_DAYTASK_LIST,
                              this,
                              callfuncO_selector(SGAllTaskLayer::taskListListener));
    notification->addObserver(MSG_DAYTASK_STATUS,
                              this,
                              callfuncO_selector(SGAllTaskLayer::linghandle));
    
    
    notification->addObserver(MSG_GETMAINTASK_REWARD,
                              this,
                              callfuncO_selector(SGAllTaskLayer::getRewardListener));
    
    notification->addObserver(MSG_REWARD_GODENTER,
                              this,
                              callfuncO_selector(SGAllTaskLayer::godEnterListener));
    notification->addObserver(MSG_REWARD_SALARYENTER,
                              this,
                              callfuncO_selector(SGAllTaskLayer::salaryEnterListener));
    
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_alltasklayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_alltasklayer);
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_alltasklayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_alltasklayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_alltasklayer);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist",RES_TYPE_LAYER_UI ,sg_alltasklayer);
    
    datas=CCArray::create();
	//所有的任务
    datas=sortarray(_array);
    datas->retain();
    

    
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
 
//////////////
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    title = SGCCLabelTTF::create(str_AllTaskLayer_1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(center);
    this->addChild(title);
    title->setVisible(false);
    
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,2);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,3);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg,-1);
    
    
    

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 20, bg->getContentSize().width, bg->getContentSize().height - title_bg->getContentSize().height);
    bg->setTextureRect(r);
    bg->setScaleX(size.width/bg->getContentSize().width * 1.01);
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        bg->setScaleY(0.95);
    }
    else
    {
        bg->setScaleY(0.8);
    }
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,SGMainManager::shareMain()->getBottomHeight())));
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
    
////////////
    const char aa[][100]={str_AllTaskLayer_2,str_AllTaskLayer_str3,str_AllTaskLayer_str4,str_AllTaskLayer_str5,str_other};
    int width=0;
    for (int j=0;j<5;++j)
    {
        SGButton *btn = SGButton::createFromLocal("store_exchangebtnbg.png", aa[j], this,
												  menu_selector(SGAllTaskLayer::typeSelect),CCPointZero,FONT_PANGWA,ccWHITE,25);
        
        float tt=  size.width-( btn->getContentSize().width*5+btn->getContentSize().width*0.2);
        
        btn->setPosition(ccp(btn->getContentSize().width*j*1.05+tt*0.5, size.height-55));
        width+=btn->getContentSize().width;
        btn->setAnchorPoint(ccp(0, 1));

        if(j==0)
        btn->setFontColor(ccc3(0xff, 0xc0, 0x00));
        btn->setTag(j);
        
        addBtn(btn);
        
        CCSprite*sp=CCSprite::createWithSpriteFrameName("publc_notice.png");
        sp->setPosition(ccpAdd(btn->getPosition(), ccp(btn->getContentSize().width*0.8, -btn->getContentSize().height*0.25)));
        this->addChild(sp,2,j+500);
        sp->setVisible(false);
        
        
    }
    
    settitle();
    
	tableView->setDown(-25);
    
    
    if(datas->count()>=6)
    {
        tableView->iscreateall=true;
    }
    else
    {
        tableView->iscreateall=false;
    }
 
    tableView->setFrame(CCRectMake(0, 0, size.width, size.height - bottomH  - 110 ));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-size.width/2, bottomH)));
	tableView->setPageNEnable(false);

}



SNSTableViewCellItem* SGAllTaskLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{

	SGAllTaskCell *item = (SGAllTaskCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    CCString * sss;
    SGTaskObj * info;
    int state = -10 ;
    int idid  = -10 ;
    if(typetype==0)
    {
        info = (SGTaskObj *)datas->objectAtIndex(indexPath->getRow());
        SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(info->gettaskId());
        sss=CCString::create(task->gettaskname()->getCString());
        state=info->getisAward();
        idid=info->gettaskId();
    }
 
    else
    {
        SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(indexPath->getRow());
        sss=CCString::create(model->gettaskName()->getCString());
        state=model->getcomplateState();
        idid=model->gettaskId();
    }
    
    if (NULL == item)
    {
        item = SGAllTaskCell::create(this,sss,typetype);
        
    }
    
    item->updateview(state,idid,typetype);

    return  item;
    
}
void SGAllTaskLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);

}
unsigned int SGAllTaskLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGAllTaskLayer::backHandler()
{

}

///////

void SGAllTaskLayer::showSectionInfoHandler(SGAllTaskCell *section,int indexindex)
{
    if (isMovingSection || section == NULL) {
        return;
    }
    creatSectionInfoView(section);

    
    if (sectiontableView)
    {
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

void SGAllTaskLayer::moveSectiontableViewDown(bool isDown)
{
    CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
    sectionInfoView->stopAllActions();
    
    if (isDown)
    {
        sectionInfoView->setPosition(ccp(0,sectiontableView->getContentSize().height));
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME,CCPointZero));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGAllTaskLayer::resetMovingSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    else
    {
        sectiontableView->stopAllActions();
        sectionInfoView->setPosition(CCPointZero);
        sectionInfoView->runAction(CCMoveTo::create(ITEMACTIONTIME, ccp(0,sectiontableView->getContentSize().height+4)));
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGAllTaskLayer::hideSectiontableView));
        sectiontableView->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME), call, NULL));
    }
    isMovingSection = true;
}

void SGAllTaskLayer::hideSectiontableView()
{
    if (sectiontableView != NULL) {
        sectiontableView->setVisible(false);
        isMovingSection = false;
    }
}

void SGAllTaskLayer::resetMovingSectiontableView()
{
    if (sectiontableView != NULL) {
        isMovingSection = false;
    }
}

void SGAllTaskLayer::creatSectionInfoView(SGAllTaskCell *section)
{

    SGTaskObj*info=NULL;
    //SGMainTaskDataModel *sfsfs=NULL;
    if(typetype==0)
    {
		info = (SGTaskObj *)datas->objectAtIndex(selectIndex);
		//SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(info->gettaskId());

        if (info == NULL) {
            return;
        }
    }

    else
    {
		SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(section->getIndexPath()->getRow());
  
        if (model == NULL) {
            return;
        }
    }
 
    CCScale9Sprite*sectionInfoView = NULL;
    float contentHeight = 0;
    
    bool isEveryday=false;
    
    ///////////////////////////////////////////////////////
    CCSprite *bigBg=CCSprite::createWithSpriteFrameName("bban.png");
    bgSize=CCSizeMake(bigBg->getContentSize().width, bigBg->getContentSize().height*0.7);

    
///////////////////////////////////////////////////////
    if (sectiontableView == NULL)
    {
        
        sectionInfoView = CCScale9Sprite::createWithSpriteFrameName("bban.png");
        sectionInfoView->setPreferredSize(CCSizeMake(bigBg->getContentSize().width, bigBg->getContentSize().height*0.75));
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
        CCSprite*  kuang_sprite=CCSprite::createWithSpriteFrameName("equipPinzhi_4_2.png");
        kuang_sprite->setAnchorPoint(ccp(0.5,0.5));
        kuang_sprite->ignoreAnchorPointForPosition(false);
        kuang_sprite->setPosition(ccpAdd(ccp(10+kuang_sprite->getContentSize().width*0.5,bgSize.height*0.7 ), ccp(0, -20)));
//        CCSprite*  reward_sprite=CCSprite::createWithSpriteFrameName("big_yb.png");
        reward_sprite = SGButton::create("big_yb.png", NULL, this, menu_selector(SGAllTaskLayer::onRewardGoodsClicked ));
        reward_sprite->setTag(reward_sprite_tag);
        reward_sprite->setAnchorPoint(ccp(0.5,0.5));
        reward_sprite->ignoreAnchorPointForPosition(false);
        reward_sprite->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(0,0)));
        
        SGCCLabelTTF* task_description= SGCCLabelTTF ::create(str_AllTaskLayer_str6,FONT_BOXINFO,FONT_SIZE,CCSizeMake(350, 100),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
        
        ////////////////////
        task_description->setAnchorPoint(ccp(0,1));
       // task_description->ignoreAnchorPointForPosition(true);
        task_description->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(kuang_sprite->getContentSize().width*0.5,
																			 kuang_sprite->getContentSize().height*0.35)));
        
        SGCCLabelTTF*   task_progress=SGCCLabelTTF::create(str_AllTaskLayer_str7, FONT_BOXINFO, FONT_SIZE , ccGREEN);
        task_progress->setAnchorPoint(ccp(0,1));
        task_progress->ignoreAnchorPointForPosition(true);
        task_progress->setPosition(ccpAdd(kuang_sprite->getPosition(),ccp(kuang_sprite->getContentSize().width*0.5,
																		  bgSize.height/2-reward_sprite->getContentSize().height*1.1)));
        
        SGCCLabelTTF*  reward_count=SGCCLabelTTF::create("500", FONT_BOXINFO, FONT_SIZE , ccYELLOW);
        reward_count->setAnchorPoint(ccp(0,1));
        reward_count->ignoreAnchorPointForPosition(true);
        reward_count->setPosition(ccpAdd(task_progress->getPosition(),ccp(0,-21)));
        
        SGCCLabelTTF*   reward_name=SGCCLabelTTF::create(str_yuanbao, FONT_BOXINFO, FONT_SIZE , ccYELLOW);
        reward_name->setAnchorPoint(ccp(0.5,0.5));
        reward_name->ignoreAnchorPointForPosition(false);
        reward_name->setPosition(ccpAdd(reward_count->getPosition(),ccp(70,0)));
        
        SGCCLabelTTF*    task_finish=SGCCLabelTTF::create(str_AllTaskLayer_str8, FONT_BOXINFO, FONT_SIZE);
        task_finish->setAnchorPoint(ccp(1,1));
        task_finish->ignoreAnchorPointForPosition(true);
        task_finish->setPosition(ccpAdd(reward_count->getPosition(),ccp(kuang_sprite->getContentSize().width*2.5,0)));
        
        
        _sectionMenu = CCMenu::create();
        CCMenu*menu=_sectionMenu;
        menu->setPosition(CCPointZero);
        sectiontableView->addChild(menu,999,999);
        
        
        SGButton *go = SGButton::createFromLocal("store_exchangebtnbg.png", str_AllTaskLayer_str19, this, menu_selector(SGAllTaskLayer::gogogo),CCPointZero,FONT_PANGWA,ccWHITE,25);
        go->setAnchorPoint(ccp(1, 1));
        go->setPosition(ccpAdd(task_finish->getPosition(), ccp(0, task_finish->getContentSize().height*0.5)));
        menu->addChild(go,1,allid);
        go->setVisible(isEveryday);
        go->setScale(0.8);
        
        menu->addChild(reward_sprite , 9 , reward_sprite_tag);

        
        
        sectiontableView->addChild(kuang_sprite,10,-11);
//        sectiontableView->addChild(reward_sprite,9,-12);
        sectiontableView->addChild(task_description,9,-13);
        sectiontableView->addChild(task_progress,9,-14);
        sectiontableView->addChild(task_finish,9,-15);
        sectiontableView->addChild(reward_count,9,-16);
        sectiontableView->setVisible(false);
        
        
        if(typetype==0)/////如果是每日
        {
            SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(info->gettaskId());
            task_progress->setVisible(false);
            task_description->setString(task->gettaskinfo()->getCString());
            reward_count->setString(CCString::createWithFormat(str_AllTaskLayer_str9,info->getgold())->getCString());
            
            if(info->getisAward()==0)//0不满足  1已领取 2满足未领取
            {
                isEveryday=true;
                task_finish->setVisible(false);
            }
            else
            {
                task_finish->setString(CCString::create(str_AllTaskLayer_str10)->getCString());
                task_finish->setInsideColor(ccGREEN);
                go->setVisible(false); //如果是已完成状态，不再显示前往按钮
            }
            //元宝
            m_itemType = 0;
        }

        else/////不是每日
        {
			//selectIndex 数值大于数组的最大值, 不合法
			if (selectIndex > datas->count() - 1)
			{
				return ;
			}
			
            SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(selectIndex);
    //        updateinfo(model);
            SGRewardDataModel* rewardModel = model->rewardModel;
            
            CCSprite*spsp= kuang_sprite;
//            CCSprite*spsp2= reward_sprite;
            SGButton * spsp2 = reward_sprite;
            SGCCLabelTTF*label3= task_description;
            SGCCLabelTTF*label4= task_progress;
            SGCCLabelTTF*label5= task_finish;
            SGCCLabelTTF*label6= reward_count;
            
            label3->setString(model->gettaskDeprision()->getCString());
            
            if(model->getcomplateState()==2)
            {
                task_finish->setString(CCString::create(str_AllTaskLayer_str10)->getCString());
                task_finish->setVisible(true);
                task_finish->setInsideColor(ccGREEN);
                go->setVisible(false); //如果是已完成的任务，不再显示对应的前往按钮
            }
            
            else
            {
                task_finish->setVisible(false);
                isEveryday=true;
            }
            
            
            
            if(model->getshowType()==2)
            {
                label4->setString(CCString::createWithFormat(str_AllTaskLayer_str11,model->getcurProgress(),model->getallProgress())->getCString());
                label4->setVisible(true);
            }
            else
                label4->setVisible(false);

            if (rewardModel == NULL)
                return;
            
            int type = 0;
            int value = 0;
            int number = 0;
            CCSprite* itemIcon = NULL;
            CCSprite *jinkuang = NULL;
            SGCCLabelTTF * itemName = NULL;
            SGMainManager * sgMainManager = SGMainManager::shareMain();
            //CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
            int rewardCount = rewardModel->rewardArray->count();
            for(int i=0; i<rewardCount && i<4; i++)
            {
                sscanf(((CCString*)rewardModel->rewardArray->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
                SGBaseItemType typeEnum = (SGBaseItemType)type;
                m_itemType = type;
                switch (typeEnum)
                {
                    case BIT_INGOT:
                    {
                        //                        cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("big_yb.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("big_yb.png");
//                        spsp2->setTag(type);
                        //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("元宝x%d",value)->getCString(), FONT_BOXINFO, 20);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str9,number)->getCString());
                        m_itemType = 0;
                        m_itemId = -1;
                    }
                        break;
                    case BIT_GOLD:
                    {
                        sgMainManager->addHeadIconPlistByNum(9999,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9999.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str12,number)->getCString());
                        //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("铜钱x%d",value)->getCString(), FONT_BOXINFO, 20);
                    }
                        break;
                    case BIT_GRADE:
                    {
                        sgMainManager->addHeadIconPlistByNum(9999,sg_storySectionLayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                        
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9999.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str13,number)->getCString());
                        //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("Pvp积分x%d",value)->getCString(), FONT_BOXINFO, 20);
                    }
                        break;
                    case BIT_FEAT:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9997,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9997.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9997.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str14,number)->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_AllTaskLayer_str14,number)->getCString(), FONT_BOXINFO, 20);
                    }
                        break;
                        //小喇叭
                    case BIT_HORN:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9998,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9998.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str15,number)->getCString());
                        //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("小喇叭x%d",value)->getCString(), FONT_BOXINFO, 20);
                        
                    }
                        break;
                    case BIT_RECIPE:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9998,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9998.png");
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str16,number)->getCString());
                        
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_liandian,number)->getCString(), FONT_BOXINFO, 20);
                    }
                        break;
                    case BIT_OFFICER:
                    {
                        
                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(value);
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                        int starlvl = temp->getOfficerCurrStarLevel();
                        if (starlvl>1) {
                            starlvl-=1;
                        }
                        jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
//                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                        
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                        spsp->addChild(countryName,30,30);
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str17,temp->getOfficerName()->getCString(),number)->getCString());
                        
                        m_itemId = temp->getOfficerId();
                        
                    }
                        break;
                    case BIT_EQUIP:
                    {
                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                        //                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                        jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
//                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
   
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getEquipName()->getCString(),number)->getCString());
                        m_itemId = temp->getEquipmentId();
                        
                    }
                        break;
                    case BIT_CONSUMABLE:
                    {
                        SGConsumableDataModel * model = SGStaticDataManager::shareStatic()->getConsumableById(value);
                        SGMainManager::shareMain()->addHeadIconPlistByNum(model->getIconId(), sg_mainTasklayer);
                        CCSprite *jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",model->getConsumeStarLvl())->getCString());
                        spsp->setDisplayFrame(jinkuang->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",model->getIconId())->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,model->getConsumeName()->getCString(),number)->getCString());
                        m_itemId = model->getConsumePicId();
                    }
                        break;
                    case BIT_PROP:
                    {
                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(value);
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                        
                        m_itemId = temp->getIconId();
  
                        int starlvl = temp->getPropsCurrStarLevel();
                        if (starlvl>1) {
                            starlvl-=1;
                        }
                        if (temp->getPropsPropsType()==1) {
                            jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
//                            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                            itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                            
                            
                            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                            countryName->setAnchorPoint(ccp(1,1));
                            countryName->ignoreAnchorPointForPosition(false);
                            countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                            spsp->addChild(countryName,30,30);
                            
                            spsp->setDisplayFrame(jinkuang->displayFrame());
//                            spsp2->setDisplayFrame(itemIcon->displayFrame());
                            spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                            spsp2->setTag(type);
                            label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getPropsName()->getCString(),number)->getCString());
                            
                        }
                        else
                        {
                            
                            jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
//                            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                            itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                            
                            spsp->setDisplayFrame(jinkuang->displayFrame());
//                            spsp2->setDisplayFrame(itemIcon->displayFrame());
                            spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                            spsp2->setTag(type);
                            label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getPropsName()->getCString(),number)->getCString());
                        }
                        m_itemId = temp->getPropsId();
                    }
                        break;
                    case BIT_PIECE:
                    {
                        SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
                        SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(value);
                        int starLv = pieces->getStarLevel();
                        if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
                        {
                            starLv -= 1;
                        }
                        m_itemId = pieces->getPiecesId();
                        if (pieces->getCallingCardId() == -1)//如果是通用碎片
                        {
                            jinkuang  = CCSprite::createWithSpriteFrameName("common_pieces_border.png");
                        }
                        else if (pieces->getPiecesType() == 1)//装备碎片
                        {
                            jinkuang  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starLv)->getCString());
                        }
                        else if (pieces->getPiecesType() == 0)//武将碎片
                        {
                            jinkuang  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starLv)->getCString());
                            
                        }
                        if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
                        {
                            CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                            mask->setPosition(ccp(jinkuang->getContentSize().width / 2, jinkuang->getContentSize().height / 2));
                            spsp->addChild(mask,0,31);
                        }
                        
                        
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",pieces->getPiecesName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                        
                        int iconId = 0;
                        if (pieces->getPiecesType() == 0)//武将
                        {
                            SGOfficerDataModel *of = staticDataManager->getOfficerById(pieces->getCallingCardId());
                            iconId = of->getIconId();
                        }
                        else if (pieces->getPiecesType() == 1)//装备
                        {
                            SGEquipmentDataModel *eq = staticDataManager->getEquipById(pieces->getCallingCardId());
                            iconId = eq->getIconId();
                        }
                        
                        if (iconId < 0)
                        {
                            break;
                        }
                        
                        SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_landingRewardLayer);
                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
                        
                        
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",iconId)->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,pieces->getPiecesName()->getCString(),number)->getCString());
                        m_itemId = pieces->getPiecesId();
                    }
                        break;
                    
                    case BIT_MATERIAL:
                    {
                        SGMaterialDataModel * model = SGStaticDataManager::shareStatic()->getMaterialById(value);
                        SGMainManager::shareMain()->addHeadIconPlistByNum(model->getMaterialIconId(), sg_mainTasklayer);
                        //表数据时自然数索引  但是资源从0开始索引
                        CCSprite * materialKuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png" , model->getMaterialBorderColor() - 1)->getCString());
                        spsp->setDisplayFrame(materialKuang->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png" , model->getMaterialIconId() )->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,model->getMaterialName()->getCString(),number)->getCString());
                        m_itemId = model->getMaterialId();
                    }
                        break;
                    case BIT_JADE: //勾玉
                    {
                        CCSprite * gouyu = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                        spsp->setDisplayFrame(gouyu->displayFrame());
                        spsp2->setImage("big_gouyu.png");
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18, str_chinese_jade, number)->getCString());
                        m_itemId = -1;
                        break;
                    }
                    case BIT_ARENA_SCORE: //征服點
                    {
                        CCSprite * arena = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                        spsp->setDisplayFrame(arena->displayFrame());
                        spsp2->setImage("arean_coin_big.png");
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18, str_arena_score, number)->getCString());
                        m_itemId = -1;
                    }
                        break;
                    default:
                        break;
                }
                
            }

        }
        
        if(typetype==0)
        {
            SGTaskObj*  info = (SGTaskObj *)datas->objectAtIndex(selectIndex);
            allid=info->gettaskId();
        }
        
        else
        {
            SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(selectIndex);
            
            allid=model->getArrivement();
            if(allid==-1)
            {
                allid=-101;
                isEveryday=false;
            }
        }
        
        go->setTag(allid);
        
        if(typetype == 0)
        {
            if( info->getisAward()==0)
                isEveryday=true;
        }
        else
        {
            SGTaskObj *info = (SGTaskObj *)datas->objectAtIndex(selectIndex);
            if( info && info->getisAward()==0)
                isEveryday=true;
        }
            
        go->setVisible(isEveryday);
        go->setEnabled(isEveryday);
        
        tableView->m_body->addChild(sectiontableView);
    }
    else
    {

        sectionInfoView = (CCScale9Sprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
        contentHeight = sectiontableView->getContentSize().height;
        
        CCMenu*menu=(CCMenu*)sectiontableView->getChildByTag(999);
        SGButton*btn=  (SGButton*) menu->getChildByTag(allid);
//        sectionInfoView->removeAllChildren();
        if(typetype==0)
        {
            info = (SGTaskObj *)datas->objectAtIndex(selectIndex);
            SGDayTaskDataModel *task = SGStaticDataManager::shareStatic()->getTaskById(info->gettaskId());
           // CCLOG("%s",task->gettaskinfo()->getCString());

            ////////////////////////////////////////////////////
            SGCCLabelTTF* label=   (SGCCLabelTTF*) sectiontableView->getChildByTag(-13);
            label ->setString(task->gettaskinfo()->getCString());
            
            SGCCLabelTTF* label1 = (SGCCLabelTTF*)(sectiontableView->getChildByTag(-16));
            label1->setString(CCString::createWithFormat(str_AllTaskLayer_str9,info->getgold())->getCString());
            
            
            bool isEveryday=false;
            if( info->getisAward()==0)
            {
               isEveryday=true;
            }
            
            
            btn->setVisible(isEveryday);
            btn->setEnabled(isEveryday);
            
            ///////
            if(typetype==0)
            {
                SGTaskObj*  info = (SGTaskObj *)datas->objectAtIndex(selectIndex);
                allid=info->gettaskId();
            }
            
            else
            {
                SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(selectIndex);
                
                allid=model->getArrivement();
                if(allid==-1)
                    allid=-101;
                
            }
            
            btn->setTag(allid);
            
            SGCCLabelTTF*label2=   (SGCCLabelTTF*) sectiontableView->getChildByTag(-15);
            label2->setString(CCString::create(str_AllTaskLayer_str10)->getCString());
            label2->setInsideColor(ccGREEN);
            label2->setVisible(!isEveryday);
            
        }
        else
        {
            SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(section->getIndexPath()->getRow());
            //        updateinfo(model);
            SGRewardDataModel* rewardModel = model->rewardModel;
            
            CCSprite*spsp= (CCSprite*)sectiontableView->getChildByTag(-11);
            spsp->removeAllChildren();
//            CCSprite*spsp2= (CCSprite*)sectiontableView->getChildByTag(-12);
            SGButton * spsp2 = (SGButton*)menu->getChildByTag(reward_sprite_tag);
            SGCCLabelTTF*label3= (SGCCLabelTTF*)sectiontableView->getChildByTag(-13);
            SGCCLabelTTF*label4= (SGCCLabelTTF*)sectiontableView->getChildByTag(-14);
            SGCCLabelTTF*label5= (SGCCLabelTTF*)sectiontableView->getChildByTag(-15);
            SGCCLabelTTF*label6= (SGCCLabelTTF*)sectiontableView->getChildByTag(-16);
            
            
            label3->setString(model->gettaskDeprision()->getCString());
            
            bool mieru=false;
            if(model->getcomplateState()==2)
            {
                label5->setString(CCString::create(str_AllTaskLayer_str10)->getCString());
                label5->setInsideColor(ccGREEN);
                label5->setVisible(true);
                btn->setVisible(false);//已完成的不再显示对应的前往按钮
            }
            else
            {
                label5->setVisible(false);
                mieru=true;
            }
            
            if(model->getshowType()==2)
            {
                label4->setString(CCString::createWithFormat(str_AllTaskLayer_str11,model->getcurProgress(),model->getallProgress())->getCString());
                label4->setVisible(true);
            }
            else
            {
                label4->setVisible(false);
                
            }
 
            if (rewardModel == NULL) {
                return;
            }
            //
            int type = 0;
            int value = 0;
            int number = 0;
            CCSprite* itemIcon = NULL;
            CCSprite *jinkuang = NULL;
            SGCCLabelTTF * itemName = NULL;
            SGMainManager * sgMainManager = SGMainManager::shareMain();
            //CCSprite *sectionInfoView = (CCSprite *)sectiontableView->getChildByTag(SECTIONINFOVIEWTAG);
            int rewardCount = rewardModel->rewardArray->count();
            for(int i=0; i<rewardCount && i<4; i++)
            {
                sscanf(((CCString*)rewardModel->rewardArray->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
                SGBaseItemType typeEnum = (SGBaseItemType)type;
                m_itemType = type;
                switch (typeEnum)
                {
                    case BIT_INGOT:
                    {
                        //                        cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("big_yb.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("big_yb.png");
//                        spsp2->setTag(type);
                        //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("元宝x%d",value)->getCString(), FONT_BOXINFO, 20);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str9,number)->getCString());
                        m_itemId = -2;
                    }
                        break;
                    case BIT_GOLD:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9999,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9999.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str12,number)->getCString());
                        m_itemId = -2;
                        //                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("铜钱x%d",value)->getCString(), FONT_BOXINFO, 20);
                    }
                        break;
                    case BIT_GRADE:
                    {
                        sgMainManager->addHeadIconPlistByNum(9999,sg_storySectionLayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9999.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str13,number)->getCString());
                        m_itemId = -2;
                    }
                        break;
                    case BIT_FEAT:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9997,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9997.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9997.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str14,number)->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_AllTaskLayer_str14,number)->getCString(), FONT_BOXINFO, 20);
                        m_itemId = -2;
                    }
                        break;
                    case BIT_HORN:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9998,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9998.png");
//                        spsp2->setTag(type);
                        
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str15,number)->getCString());
                        m_itemId = -2;
                    }
                        break;
                    case BIT_RECIPE:
                    {
                        
                        sgMainManager->addHeadIconPlistByNum(9998,sg_mainTasklayer);
                        jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
//                        itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage("head9998.png");
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str16,number)->getCString());
                        
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_liandian,number)->getCString(), FONT_BOXINFO, 20);
                        m_itemId = -2;
                    }
                        break;
                    case BIT_OFFICER:
                    {
                        
                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(value);
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                        int starlvl = temp->getOfficerCurrStarLevel();
                        if (starlvl>1) {
                            starlvl-=1;
                        }
                        jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
//                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                        
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                        spsp->addChild(countryName,30,30);

                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str17,temp->getOfficerName()->getCString(),number)->getCString());
                        
                        m_itemId = temp->getOfficerId();
                        
                    }
                        break;
                    case BIT_EQUIP:
                    {
                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                        //                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(value);
                        jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
//                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                        
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getEquipName()->getCString(),number)->getCString());
                        
                        m_itemId = temp->getEquipmentId();
                    }
                        break;
                    case BIT_PROP:
                    {
                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(value);
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_mainTasklayer);
                        int starlvl = temp->getPropsCurrStarLevel();
                        if (starlvl>1) {
                            starlvl-=1;
                        }
                        if (temp->getPropsPropsType()==1) {
                            jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
//                            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                            itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                            
                            
                            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getPropsCountry()))->getCString());
                            countryName->setAnchorPoint(ccp(1,1));
                            countryName->ignoreAnchorPointForPosition(false);
                            countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                            spsp->addChild(countryName,30,30);
                            
                            spsp->setDisplayFrame(jinkuang->displayFrame());
//                            spsp2->setDisplayFrame(itemIcon->displayFrame());
                            spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                            spsp2->setTag(type);
                            label6->setString(CCString::createWithFormat("奖励 %sx%d",temp->getPropsName()->getCString(),number)->getCString());
                        }
                        else
                        {
                            
                            jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
//                            itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                            itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                            
                            spsp->setDisplayFrame(jinkuang->displayFrame());
//                            spsp2->setDisplayFrame(itemIcon->displayFrame());
                            spsp2->setImage(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
//                            spsp2->setTag(type);
                            label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,temp->getPropsName()->getCString(),number)->getCString());
                        }
                        m_itemId = temp->getPropsId();
                    }
                        break;
                    case BIT_PIECE:
                    {
                        SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
                        SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(value);
                        int starLv = pieces->getStarLevel();
                        if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
                        {
                            starLv -= 1;
                        }
                        if (pieces->getCallingCardId() == -1)//如果是通用碎片
                        {
                            jinkuang  = CCSprite::createWithSpriteFrameName("common_pieces_border.png");
                        }
                        else if (pieces->getPiecesType() == 1)//装备碎片
                        {
                            jinkuang  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starLv)->getCString());
                        }
                        else if (pieces->getPiecesType() == 0)//武将碎片
                        {
                            jinkuang  = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starLv)->getCString());
                            
                        }
                        if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
                        {
                            CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                            mask->setPosition(ccp(jinkuang->getContentSize().width / 2, jinkuang->getContentSize().height / 2));
                            spsp->addChild(mask,0,31);
                        }
                        
                        
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%sx%d",pieces->getPiecesName()->getCString(),number)->getCString(), FONT_BOXINFO, 20);
                        
                        int iconId = 0;
                        if (pieces->getPiecesType() == 0)//武将
                        {
                            SGOfficerDataModel *of = staticDataManager->getOfficerById(pieces->getCallingCardId());
                            iconId = of->getIconId();
                        }
                        else if (pieces->getPiecesType() == 1)//装备
                        {
                            SGEquipmentDataModel *eq = staticDataManager->getEquipById(pieces->getCallingCardId());
                            iconId = eq->getIconId();
                        }
                        
                        if (iconId < 0)
                        {
                            break;
                        }
                        
                        SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_landingRewardLayer);
//                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
                        

                    
                        spsp->setDisplayFrame(jinkuang->displayFrame());
//                        spsp2->setDisplayFrame(itemIcon->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",iconId)->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,pieces->getPiecesName()->getCString(),number)->getCString());
                        m_itemId = pieces->getPiecesId();
                    }
                        break;
                    case BIT_CONSUMABLE:
                    {
                        SGConsumableDataModel * model = SGStaticDataManager::shareStatic()->getConsumableById(value);
                        SGMainManager::shareMain()->addHeadIconPlistByNum(model->getIconId(), sg_mainTasklayer);
                        CCSprite *jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",model->getConsumeStarLvl())->getCString());
                        spsp->setDisplayFrame(jinkuang->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png",model->getIconId())->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,model->getConsumeName()->getCString(),number)->getCString());
                        m_itemId = model->getConsumId();
                    }
                        break;
                    case BIT_MATERIAL:
                    {
                        SGMaterialDataModel * model = SGStaticDataManager::shareStatic()->getMaterialById(value);
                        SGMainManager::shareMain()->addHeadIconPlistByNum(model->getMaterialIconId(), sg_mainTasklayer);
                        //表数据时自然数索引  但是资源从0开始索引
                        CCSprite * materialKuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png" , model->getMaterialBorderColor() - 1)->getCString());
                        spsp->setDisplayFrame(materialKuang->displayFrame());
                        spsp2->setImage(CCString::createWithFormat("head%d.png" , model->getMaterialIconId() )->getCString());
//                        spsp2->setTag(type);
                        label6->setString(CCString::createWithFormat(str_AllTaskLayer_str18,model->getMaterialName()->getCString(),number)->getCString());
                        m_itemId = model->getMaterialId();
                    }
                        break;
                    default:
                        break;
                }
                
            }
       
            
            CCMenu*menu=(CCMenu*)sectiontableView->getChildByTag(999);
            SGButton*btn=  (SGButton*) menu->getChildByTag(allid);
            if(allid==-101)
                mieru=false;
            btn->setVisible(mieru);
            btn->setEnabled(mieru);

            
            SGMainTaskDataModel *modelmodel=(SGMainTaskDataModel *)datas->objectAtIndex(selectIndex);
            
            allid=modelmodel->getArrivement();
            if(allid==-1)
            {
                allid=-101;
                btn->setVisible(false);
                btn->setEnabled(false);
            }
            else
            {
                //不知为何如此写法，这么多判断，只能进一步再判断了,2貌似代表完成
                if (modelmodel->getcomplateState() == 2)
                {
                    btn->setVisible(false);
                    btn->setEnabled(false);
                }
                else
                {
                    btn->setVisible(true);
                    btn->setEnabled(true);
                }
            }

           btn->setTag(allid);

        }
        
    }
}


void SGAllTaskLayer::entrySectionHandler(SGAllTaskCell *section,int index)
{
    if (section == NULL ) {
        return;
    }
    if (isMovingSection) {
        return;
    }
    
    
}

void SGAllTaskLayer::typeSelect(CCNode*node)
{
//    if (!canTouchTitle)
//    {
//        return ;
//    }
    int tag=node->getTag();
    if(typetype==tag)
    {
        return;
    }
    typetype=tag;
    
    //注意先后顺序
    if(!SGAllTaskLayer::m_buttonCanTouch)
        return ;
    SGAllTaskLayer::m_buttonCanTouch = false;
    
    tableView->setVisible(true);
    
    
    for(int i=0;i<=4;i++)
    {
        SGButton*btnbtn =(SGButton* ) getBtnByTag(i);
        
        if(tag==i)
		{
			btnbtn->setFontColor(ccc3(0xff, 0xc0, 0x00));
		}
        else
		{
			btnbtn->setFontColor(ccWHITE);
		}
    }
   
    if(tag==0) ///是每日
    {
        EFFECT_PLAY(MUSIC_BTN);
        main::DayTaskListRequest *request = new main::DayTaskListRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_LIST, request);
        title->setVisible(false);
    }
    else  //不是每日
    {
		CCArray *temp=SGPlayerInfo::sharePlayerInfo()->getTaskGroup();
		SGMainTaskGroup *group=(SGMainTaskGroup *)temp->objectAtIndex(tag-1);
		
		//本地任务表
		CCArray *temptemp = SGPlayerInfo::sharePlayerInfo()->getSubTaskByGroupName(group->gettaskGroupName()->getCString());
        
        if (temptemp == NULL)
        {
            title->setVisible(true);
            tableView->setVisible(false);
            SGAllTaskLayer::m_buttonCanTouch = true;
            return;
        }
        datas=sortmainichijanaiarray(temptemp);
        if(datas==NULL || datas->count() < 1)
        {
            title->setVisible(true);
            tableView->setVisible(false);
            SGAllTaskLayer::m_buttonCanTouch = true;
            return;
        }
        else
        {
			datas->retain();
        }
        
//////////
        lastSelectIndex = 0;
        lastMoveDis = 0;
        isMovingSection = false;
        selectIndex=0;
        sectiontableView=NULL;
        if(datas->count()>=6)
        {
			tableView->iscreateall=true;
        }
        else
        {
			tableView->iscreateall=false;
        }
        tableView->reloadData();
        firstShow();

        if(datas->count()==0)
        {
            title->setVisible(true);
            
        }
        else
        {
            title->setVisible(false);

        }
        SGAllTaskLayer::m_buttonCanTouch = true;
    }
    
}



void SGAllTaskLayer::taskListListener(cocos2d::CCObject *obj)
{

    num=0;
    numnum=0;///////
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        CCArray *array = CCArray::create();
        main::DayTaskListResponse *response = (main::DayTaskListResponse *)sr->m_msg;
        int len = response->daytaskmodel_size();
        for (int i = 0; i<len; i++)
        {
            main::DayTaskModel info = response->daytaskmodel(i);
            SGTaskObj *taskobj = new SGTaskObj;
            taskobj->settaskId(info.taskid());
            taskobj->settaskCount(info.count());
            taskobj->setgold(info.gold());
            taskobj->setisAward(info.isaward());
            array->addObject(taskobj);
            taskobj->autorelease();
            
            if(info.isaward()==2)
                num++;
                
        }

        if (len > 0)
        {
            lastSelectIndex = 0;
            lastMoveDis = 0;
            isMovingSection = false;
            selectIndex=0;
            sectiontableView=NULL;
            
            ///////////////
            datas= sortarray(array);
            settitle();
            datas->retain();
            if(datas->count()>=6)
            {
                tableView->iscreateall=true;
            }
            else
            {
                tableView->iscreateall=false;
            }
            tableView->reloadData();
            firstShow();
        }

    }
    
    SGAllTaskLayer::m_buttonCanTouch = true;

}

void SGAllTaskLayer::linghandle(CCObject*obj)
{
    //点击领取的时候，暂时不让上面按钮点击
//    canTouchTitle = false;
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::DayTaskGetRewardResponse *rsp = (main::DayTaskGetRewardResponse *) sr->m_msg;
        if (rsp)
        {
            
          if( rsp->state())
          {
             
//              CCArray *data=CCArray::create();
//              data->retain();
//              SGMailSystemAccessoryDataModel *datamodel= new SGMailSystemAccessoryDataModel() ;
//              datamodel->setType(0);
//              datamodel->setCount(5);
//              data->addObject(datamodel);
              
              //SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
              //SGMainManager::shareMain()->showBox(rewardBox);
              SGMainManager::shareMain()->showMessage(str_SoulExchangeLayer_str10);
              /////////////刷新cell  //重新请求数据
              
              EFFECT_PLAY(MUSIC_BTN);
              
              refreshTaskCell();
              
//              schedule(schedule_selector(SGAllTaskLayer::refreshTaskCell), 0.04);

          }
  
        }
        
    }
    SGAllTaskLayer::m_buttonCanTouch = true;
}

void SGAllTaskLayer::refreshTaskCell()
{
//    unschedule(schedule_selector(SGAllTaskLayer::refreshTaskCell));
    main::DayTaskListRequest *request = new main::DayTaskListRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_DAYTASK_LIST, request);

}



void SGAllTaskLayer::getRewardListener(CCObject *obj)
{
    
    SGSocketRequest *sr=(SGSocketRequest *)obj;
    if (sr) {
        main::ReceiveQuestRewardResponse *rs=(main::ReceiveQuestRewardResponse *)sr->m_msg;
        if (rs->state()) {
            
            //            SGPlayerInfo::sharePlayerInfo()->removeSubTask(_dataModel);
            this->updateview();
            
            SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str20);
            
            SGGuideManager::shareManager()->setLimitGuideTag(SGPlayerInfo::sharePlayerInfo()->checkStartLimitGuide(3,m_questId));
        }
        else
        {
            
            SGMainManager::shareMain()->showMessage(str_AllTaskLayer_str21);
        }
    }
    SGAllTaskLayer::m_buttonCanTouch = true;
}



void SGAllTaskLayer::updateview()
{
    //////////////////弹出框
    
    
    CCArray *data=CCArray::create();
    data->retain();
    
    SGMainTaskDataModel *model=(SGMainTaskDataModel *)datas->objectAtIndex(selectIndex);
    SGRewardDataModel* rewardModel = model->rewardModel;
    
    int type = 0;
    int value = 0;
    int number = 0;
    if (rewardModel)
    {
        int rewardCount = rewardModel->rewardArray->count();
        for(int i=0; i<rewardCount; i++)
        {
            SGMailSystemAccessoryDataModel *access=new SGMailSystemAccessoryDataModel();
            sscanf(((CCString*)rewardModel->rewardArray->objectAtIndex(i))->getCString(), "%d-%d-%d", &type,&value,&number);
            switch (type) {
                case 0:
                {
                    access->setType(0);
                    access->setCount(number);
                    
                }
                    break;
                case 1:
                {
                    access->setType(1);
                    access->setCount(number);
                }
                    break;
                case 2:
                {
                    access->setType(14);
                    access->setCount(number);
                }
                    break;
                case 3:
                {
                    access->setType(3);
                    access->setCount(number);
                }
                    break;
                case 4:
                {
                    access->setType(4);
                    access->setCount(number);
                    
                }
                    break;
                case 5:
                {
                    access->setType(5);
                    access->setCount(number);
                }
                    break;
                case 6:
                {
                    access->setType(6);
                    access->setCount(number);
                    access->setItemId(value);
                    
                    
                }
                    break;
                case 7:
                {
                    
                    access->setType(7);
                    access->setCount(number);
                    access->setItemId(value);
                }
                    break;
//                case 8:
//                {
//                    
//                    access->setType(8);
//                    access->setCount(number);
//                    access->setItemId(value);
//                }
//                    break;
                case 9:
                {
                    access->setType(9);
                    access->setCount(number);
                    access->setItemId(value);
                    
                }
                    break;
                case 10:
                {
                    access->setType(10);
                    access->setCount(number);
                    access->setItemId(value);
                    
                }
                    break;
                case 15://消耗品掉落
                {
                    access->setType(15);
                    access->setCount(number);
                    access->setItemId(value);
                }
                    break;
                default:
                    break;
            }
            if (access->getCount()>0) {
                data->addObject(access);
            }
        }
        
        
        
        
        SGShowRewardBox *rewardBox=SGShowRewardBox::create(this, data);
        SGMainManager::shareMain()->showBox(rewardBox);
    }

    
	//////////////////////////////////////
    CCArray*temp=SGPlayerInfo::sharePlayerInfo()->getTaskGroup();

    SGMainTaskGroup *group=(SGMainTaskGroup *)temp->objectAtIndex(typetype-1);
	CCArray*array=SGPlayerInfo::sharePlayerInfo()->getSubTaskByGroupName(group->gettaskGroupName()->getCString());
    datas=sortmainichijanaiarray(array);
    datas->retain();
    numnum=0;
    settitle();
    sectiontableView=NULL;
    tableView->reloadData();
	
	// 如果选择当前值大于数组值, 直接返回
	if (selectIndex < datas->count())
    {
		firstShow();
    }
}



CCArray* SGAllTaskLayer::sortarray(CCArray*arr)
{
    
    int len = arr->count();
    CCArray*shuuryou_array=CCArray::create();///领奖終了array
    CCArray*can_array=CCArray::create();//可领array
    CCArray*cannot_array=CCArray::create();//不满足条件array
    CCArray*return_array=CCArray::create();//return array
    
       num=0;
    for (int i = len-1; i>=0; i--)
	{
        
        SGTaskObj *taskobj = (SGTaskObj *)arr->objectAtIndex(i);
		int state=  taskobj->getisAward();       //0不满足  1已领取 2满足未领取
        
        switch (state) {
            case 0:
                cannot_array->addObject(taskobj);
                break;
            case 1:
                shuuryou_array->addObject(taskobj);
                break;
            case 2:
                can_array->addObject(taskobj);
                num++;
                break;
                
            default:
                break;
        }
     }
    
    can_array->addObjectsFromArray(cannot_array);
    can_array->addObjectsFromArray(shuuryou_array);
    return_array=can_array;
    
    return   return_array;
    
    

}


void SGAllTaskLayer::gogogo(CCNode*node)
{
    int tag=node->getTag();
 //////////////////////
    /*
     1		进行一次闯关战斗
     2		进行一次精英闯关
     3		开启一次宝箱
     4	    进行一次铜雀夺宝
     5		进行一次天梯对战
     6		进行一次副本战斗
     7		强化一名武将
     8		强化一件装备
     9		强化一种士兵
     10		探访一次强力武将
     11		参与祭天
     12		领取当日俸禄
     13		和大家打个招呼
     14		进行守荆州战斗
     15     强化主将技
     16     强化被动技
     17     转生
     18     军营-碎片
     */
    
    EFFECT_PLAY(MUSIC_BTN);

    switch (tag) {
            
        case 1:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->storyHandler();
            
        }
            break;
        case 2:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->creamBattleHandler();
        }
            break;
        case 3://宝箱
        {
			SGMainManager::shareMain()->showConsumeLayer();
			break;
        }
        case 4:
        {
            SGMainManager::shareMain()->getMainLayer()->entryTongQueShow();
        }
            break;
        case 5:
        {
            SGMainManager::shareMain()->showPvpMatchLayer();
            
        }
            
            
            break;
        case 6:
        {
            SGMainLayer *mainLayer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainLayer->activityHandler();
        }
            break;
        case 7:
            SGMainManager::shareMain()->showGeneralsLayer(2,true,5);
            break;
        case 8:
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL,0,1,true,5);
        }
            break;
        case 9:
            SGMainManager::shareMain()->showSoldierslayer(true,5);
            
            break;
        case 10:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->setPortraitMenuCanTouch(false);
            mainlayer->visithide();
            mainlayer->visitHandler();
        }
            
            break;
            
        case 11:
            showHope();
            break;
        case 12:
            showGetSalary();
            break;
        case 13:
        {
            SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            mainlayer->setPortraitMenuCanTouch(false);
            SGChatMessageCenter::shareChatMessageCenter()->openChatRoom();
        }
            break;
            
            ////守荆州每日
        case 14:
        {
            //发送获取详情
            SGMainManager::shareMain()->sendGetSjzInfoRequest();
            break;
        }
        case 15:
        {

            SGMainManager::shareMain()->showGeneralsLayer(4);


        }
             break;
        case 16:
        {
 
            SGMainManager::shareMain()->showGeneralsLayer(5);
           
        }
             break;
        case 17:
        {
            SGMainManager::shareMain()->showGeneralsLayer(3);


        }
            break;
        case 18:
        {
   	     SGMainManager::shareMain()->showPiecesLayer();
           
        }
             break;
        default:
        {
 
        }
            break;


    
     }
}




void SGAllTaskLayer::showHope()
{
    main::RewardGodPrayEntryRequest *request = new main::RewardGodPrayEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GODENTER, request);
}
void SGAllTaskLayer::godEnterListener(cocos2d::CCObject *obj)
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
        
        
        
        
        
        
        SGMainManager::shareMain()->showJitianLayer(current,max,iscan,array);
        
    }
}


void SGAllTaskLayer::showGetSalary()
{
    main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_SALARYENTER, request);
}
void SGAllTaskLayer::salaryEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::RewardSalaryEntryResponse *response = (main::RewardSalaryEntryResponse *)sr->m_msg;
    int levelCoin = response->levelcoin();
	//int canGetlevelCoin = response->cangetlevelcoin();
	int levelRewardTime = response->levelrewardtime();
	int pvpLevel = response->pvplevel();
	int pvpLevelCoin = response->pvplevelcoin();
	//int canGetPvPLevelCoin = response->cangetpvplevelcoin();
	int pvpRewardTime = response->pvprewardtime();
    int minLevel = response->pveminlevel();
    int minLevelpvp = response->pvpminlevel();
    SGMainManager::shareMain()->showSalaryLayer(levelCoin,levelRewardTime,pvpLevel,pvpLevelCoin,pvpRewardTime,minLevel,minLevelpvp);
}




void  SGAllTaskLayer::setspvisible(int tag,bool mieru)
{
 
    getChildByTag(tag+500)->setVisible(mieru);
}


void  SGAllTaskLayer::setspunvisible(int tag,bool mieru)
{
      getChildByTag(tag+500)->setVisible(mieru);
    
}

void  SGAllTaskLayer::settitle()
{
    if(num!=0)
    {
        getChildByTag(500)->setVisible(true);
    }
    else
    {
        getChildByTag(500)->setVisible(false);
    }
    
    
    CCArray*temp=SGPlayerInfo::sharePlayerInfo()->getTaskGroup();
    for (int mmm=0; mmm<=3; mmm++)
    {
        SGMainTaskGroup *group=(SGMainTaskGroup *)temp->objectAtIndex(mmm);
        
        
        CCArray* arr=SGPlayerInfo::sharePlayerInfo()->getSubTaskByGroupName(group->gettaskGroupName()->getCString());
        
        
        
        int temp=0;
        CCObject *obj = NULL;
        CCARRAY_FOREACH(arr, obj)
        {
            SGMainTaskDataModel *item = (SGMainTaskDataModel *)obj;
            if (item->getcomplateState()==2)
            {
                temp++;

            }
        }
        
        if(temp!=0)
        {
            setspvisible( mmm+1,true);
        }
        else
        {
            setspvisible( mmm+1,false);
        }
       
    }

}






void SGAllTaskLayer:: firstShow()
{
    
    SNSIndexPath* newIndexPath = SNSIndexPath::create(0, 1);
    SGAllTaskCell  *cell=(SGAllTaskCell *)tableView->dequeueReusableCellItemForIndexPath(newIndexPath);
    showSectionInfoHandler(cell,111);
}


CCArray* SGAllTaskLayer::sortmainichijanaiarray(CCArray*arr)
{
    CCArray*return_array=CCArray::create();

    CCArray* temp_arr=arr;
    
    //std::sort(temp_arr->data->arr , temp_arr->data->arr +temp_arr->data->num,ArrivementLess);
    std::sort(temp_arr->data->arr , temp_arr->data->arr +temp_arr->data->num,IDLess);
    
    
    CCArray*can_array=CCArray::create();//可领array
    CCArray*cannot_array=CCArray::create();//不满足条件array
    
    
    for (int i=0; i<temp_arr->count(); i++)
	{
        
       SGMainTaskDataModel*mmm= (SGMainTaskDataModel*)   temp_arr->objectAtIndex(i);
        
      
        int state=mmm->getcomplateState();       //0不满足  1已领取 2满足未领取
        switch (state)
		{
                
            case 2:
                can_array->addObject(mmm);
                break;
                
            default:
                cannot_array->addObject(mmm);
                break;
                
		}

	}
    
    
    
    can_array->addObjectsFromArray(cannot_array);
    return_array=can_array;
    return return_array;
    
    
    
}
void SGAllTaskLayer::showBoxCall(CCObject *obj)
{
    if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
    {
        SGGuideManager::shareManager()->startLimitGuide();
    }
}

void SGAllTaskLayer::onRewardGoodsClicked(CCObject * obj)
{
//    CCNode * node = (CCNode * )obj ;
//    int tag =  node->getTag();
    CCLog("点击了btn： type = %d" , m_itemType);
    SGBaseItemType itemType = (SGBaseItemType)m_itemType;
    if(itemType == BIT_OFFICER || itemType == BIT_EQUIP || itemType== BIT_PROP || itemType==BIT_PIECE || itemType==BIT_CONSUMABLE || itemType == BIT_MATERIAL)
        SGMainManager::shareMain()->showCommonInfoBoxInProto(itemType, m_itemId , this, -1, true, CIBET_ALL_TASK_LAYER);
    
    //重新将node的tag设置为   reward_sprite_tag ，。。。吭
//    node->setTag(reward_sprite_tag);
}

//需要解决一些跟新手引导/任务界面相关的触控问题。
void SGAllTaskLayer::setIsCanTouch(bool isTouch)
{
    if (sectiontableView)
    {
        sectiontableView->setTouchPriority(isTouch);
    }
    if (_sectionMenu)
    {
        _sectionMenu->setEnabled(isTouch);
    }
    SGBaseTableLayer::setIsCanTouch(isTouch);
}
