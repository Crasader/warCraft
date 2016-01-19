//
//  SGFateInfoLayer.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/17/13.
//
//

#include "SGFateInfoLayer.h"
#include "SGOfficerCard.h"
#include "SGFateBase.h"
#include "SGFateInfoData.h"
#include "SGFateInfoItem.h"
#include "SGMainManager.h"
#include "SGGeneralInfoLayer.h"

//构造
SGFateInfoLayer::SGFateInfoLayer():enterType(0),
m_card(NULL)
{
    ;
}

//析构
SGFateInfoLayer::~SGFateInfoLayer()
{
//    int a = 5;
}

//创建
SGFateInfoLayer* SGFateInfoLayer::create(SGOfficerCard* card, int enterType)
{
    SGFateInfoLayer *layer = new SGFateInfoLayer();
    if (layer && layer->init(NULL, sg_fateInfoLayer)) {
        layer->setCard(card);
		layer->enterType = enterType;
        layer->initData();
        layer->initView();
//        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

//获得节点数据
void SGFateInfoLayer::initData()
{
    //载入节点数据
    this->datas = CCArray::create();
    this->datas->retain();
    
    std::vector<int> vec = m_card->getFateList();
    std::vector<int>::iterator iter = vec.begin();
    
    if (enterType == CHATSHOWENTRYTAG) {
        int fateNum = 0;
        int fateId = 0;
        int fateState = 0;
        for (; iter != vec.end(); iter++)
        {
            SGFateInfoData* data = new SGFateInfoData();
            data->autorelease();
            data->setCard(m_card);
            fateNum = *iter;
            fateId = fateNum / 10;
            fateState = fateNum - fateId * 10;
            data->setId(fateId);
            data->setState(fateState);
            
            this->datas->addObject(data);
        }
    }
    else
    {
        for (; iter != vec.end(); iter++)
        {
            SGFateInfoData* data = new SGFateInfoData();
            data->autorelease();
            data->setCard(m_card);
            data->setId(*iter);
            this->datas->addObject(data);
        }
    }

}

//初始化视图
void SGFateInfoLayer::initView()
{
    tableViewHeight = 210 * 1;
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("tran/tran.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("ofc_frame_new/ofc_frame_new.plist", RES_TYPE_LAYER_UI, sg_generalInfoLayer);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    //灰图
    CCLayerColor* layer = CCLayerColor::create(ccc4(0, 0, 0, 180), skewing(520)*3, skewingY(960)*3);
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition(center);
    this->addChild(layer, -10);
    
    //背景
    CCSprite* bg = CCSprite::createWithSpriteFrameName("ofc_bkg.png");
    bg->setPosition( center );
    this->addChild(bg, -8);
    
    CCSize size = bg->getContentSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //标题缘分列表
    CCSprite* title = CCSprite::createWithSpriteFrameName("ofc_title_yflb.png");
    title->setPosition(ccpAdd( center,ccp(0, size.height * 0.5 - title->getContentSize().height * 0.5 -15) ));
    this->addChild(title, -7);
    
    //确定按钮
    SGButton *confirm = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGFateInfoLayer::confirmHandler), ccp(0, 0), false, true);
    confirm->setPosition (ccpAdd(center,ccp(0, - size.height * 0.45)));
    this->addBtn(confirm);

    //显示区域
    tableView->setFrame(CCRectMake(0, 0, winSize.width * 2, (size.height - title->getContentSize().height - confirm->getContentSize().height - 10) * 1 ));
    tableView->setDown(-130);
     //tableView->setDown(0);
    tableView->setPosition(ccp(-winSize.width, (winSize.height - size.height) * 0.5 + confirm->getContentSize().height  ));
    
    setTouchPad(false);
    /////////////弹出效果
	this->setScale(.3);
    CCScaleTo *scale1=CCScaleTo::create(0.1,1.1);
    CCScaleTo *scale3=CCScaleTo::create(0.1,1.05);
    CCSequence*action=CCSequence::create(scale1,scale3,NULL);
    this->runAction(action);
}



//确定按钮的回调
void SGFateInfoLayer::confirmHandler()
{
    //放开触控
    setTouchPad(true);
    ////
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    if (enterType==25) {
        mainLayer->setIsCanTouch(false);
        mainLayer->setPortraitMenuCanTouch(false);
    }

    this->removeFromParentAndCleanup(true);
}

//触控开关
void SGFateInfoLayer::setTouchPad(bool bl)
{
    SGGeneralInfoLayer *giLayer = (SGGeneralInfoLayer*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_generalInfoLayer);
    if (giLayer)
        giLayer->setIsCanTouch(bl);
    
    SGMainLayer *mainLayer = (SGMainLayer*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    if (mainLayer)
        mainLayer->setIsCanTouch(bl);
}

// 根据indexPath返回tableView的cell
SNSTableViewCellItem* SGFateInfoLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	SGFateInfoItem *item = (SGFateInfoItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    SGFateInfoData *data = (SGFateInfoData *)datas->objectAtIndex(index);
    if (item == NULL) {
        item = SGFateInfoItem::create(data, enterType);
        item->setTag(index);
    }
    if (index < datas->count()) {
        item->setTag(index);
       // printf("count : %d \n", datas->count());
    }else {
        item->setVisible(false);
    }
    
    return item;
}

// 返回tableView的cell数
unsigned int SGFateInfoLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return this->datas->count();
}

// 每个item的点击事件
void SGFateInfoLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    ;
}

//定义tableView每一行或列的高度
//float SGFateInfoLayer::tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath)
//{
//    return 210 * 1.4;
//}