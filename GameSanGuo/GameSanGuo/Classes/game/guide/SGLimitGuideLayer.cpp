//
//  SGLimitGuideLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-6.
//
//

#include "SGLimitGuideLayer.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "SGTestSwitch.h"
#include "SGGuideManager.h"
#include "SGMainLayer.h"
#include "SGBattleManager.h"
#include "SGFindHelpLayer.h"
#include "SGGeneralsLayer.h"
#include "SGMenu.h"
#include "SGStrengLayer.h"
#include "SGEmbattleLayer.h"
#include "SGArrangeLayer.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGTaskLayer.h"
#include "SGVisitItem1.h"
#include "SGStorySectionLayer.h"

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PlotModule.pb.h"
#include "CooperationModule.pb.h"
#include "ResourceManager.h"
#include "SGFirstLayer.h"
#include "AppMacros.h"
#include "SGStoryMainLayer.h"
#include "SGFirstLayer.h"
#include "SGFirstSubLayer.h"
#include "SGSoldierItem.h"
#include "SGLootWikiLayer.h"
#include "SGLootWiKiMainLayer.h"
#include "SGPiecesBagCell.h"
#include "SGPiecesBagLayer.h"
#include "SpecialBattleModule.pb.h"


#define SNSCELLTAG 1001
SGLimitGuideLayer::SGLimitGuideLayer(void)
{
    _wordIndex = 0;
    _currGuideId = 0;
    _model = NULL;
    _delegateTouch = NULL;
    _canTouch = false;
    
    label = NULL;
    speaker = NULL;
    bboard = NULL;
    spriteTag = NULL;
    spriteBlackbg = NULL;
    actSprite = NULL;
    btn = NULL;
    generalIcon = NULL;
    path = NULL;
    oldTableview = NULL;
    
    tableview = NULL;
    m_removeRectTag = 0;
    color = NULL;
}

SGLimitGuideLayer::~SGLimitGuideLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_uiguideLayer);
}

SGLimitGuideLayer* SGLimitGuideLayer::create()
{
    SGLimitGuideLayer *model = new SGLimitGuideLayer();
    if (model && model->init(NULL, sg_limitGuideLayer)) {
        model->autorelease();
        model->initView();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}
void SGLimitGuideLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void SGLimitGuideLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/djjx_new.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer, LIM_PNG_AS_PNG);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    spriteBlackbg = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    spriteBlackbg->setPosition(SGLayout::getPoint(kMiddleCenter));
    spriteBlackbg->setScaleX(size.width/spriteBlackbg->getContentSize().width);
    spriteBlackbg->setScaleY(size.height/spriteBlackbg->getContentSize().height);
    spriteBlackbg->setOpacity(179);//255*0.7
    this->addChild(spriteBlackbg, -1);
    
    float bottomH = skewing(162.0f/2);
    this->setTouchEnabled(false);
    
    // init black board
    bboard = CCSprite::createWithSpriteFrameName("guide_board_new.png");
    float posH = bboard->getContentSize().height;
    bboard->setScaleX(size.width/bboard->getContentSize().width);
    bboard->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH+posH/2)));
    this->addChild(bboard, 1);
    
    actSprite = CCSprite::createWithSpriteFrameName("uplevel_click.png");
    float posXX = bboard->getContentSize().width-actSprite->getContentSize().width*.65f;
    float posYY = actSprite->getContentSize().height*.70f;
    actSprite->setPosition(ccp(posXX, posYY));
    bboard->addChild(actSprite);
    
    CCSpriterX *resultx = CCSpriterX::create("animationFile/djjx.scml", true, true);
    resultx->setAnchorPoint(ccp(0.5, 0.5));
    posXX = posXX-actSprite->getContentSize().width*.75f;
    posYY = posYY - actSprite->getContentSize().height*.45f;
    resultx->setPosition(ccp(posXX, posYY));
    resultx->setanimaID(0);
    resultx->setisloop(true);
    bboard->addChild(resultx, 10);
    resultx->play();
    
    // init speaker
    speaker = CCSprite::createWithSpriteFrameName("public_guide_mm.png");
    posH += speaker->getContentSize().height/2;
    float spWid = speaker->getContentSize().width;
    speaker->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-spWid*0.32f, bottomH+posH)));
    this->addChild(speaker);
    
    // init sprite tag
    spriteTag = CCSprite::createWithSpriteFrameName("public_jiantou.png");
    spriteTag->setVisible(false);
    this->addChild(spriteTag, 2);
    
    tableview = SNSTableView::create(CCRectMake(0, 0, 1, 1), TableViewTypeVertical);
    tableview->setVisible(_canTouch);
    tableview->setMoveEnable(false);
    this->addChild(tableview);
    
}
CCAction* SGLimitGuideLayer::getActionByDirection(int direct)
{
    CCAction *action = NULL;
    float x = 0, y = 0;
    if (direct == 1) {//down
        y = skewing(15);
    }else if (direct == 2) {//up
        y = skewing(-15);
    }else if (direct == 3) {//left
        x = skewing(15);
    }else if (direct == 4) {//right
        x = skewing(-15);
    }
    
    action = CCSequence::create(CCMoveBy::create(0.5f, ccp(-x, -y)), CCMoveBy::create(0.5f, ccp(x, y)), NULL);
    return CCRepeatForever::create((CCActionInterval *)action);
}

void SGLimitGuideLayer::showVisible(bool visible)
{
    spriteTag->setVisible(!visible);
    if (label) {
        label->setVisible(visible);
    }
    speaker->setVisible(visible);
    bboard->setVisible(visible);
}

void SGLimitGuideLayer::setDatas(SGLimitGuideVO *model, SGBoxDelegate *delegate)
{

    if(spriteTag) spriteTag->stopAllActions();
    
    this->setTouchEnabled(true);

    if (_model) {
        CC_SAFE_DELETE(_model);
    }
    _model = model;
    _model->retain();
    
    _wordIndex = 0;
    _currGuideId = _model->getModelId();
    
    int mType = _model->getModelType();
    //type == 1 ==》等同于显示对话
    if (mType == 1)//对话
    {
        this->showVisible(true);
        if (label) {
            label->removeFromParentAndCleanup(true);
        }
        CCString *str = (CCString *)_model->getModelMsg()->objectAtIndex(_wordIndex);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
        CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
        label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
        label->setPosition(bboard->getPosition());
        this->addChild(label, 2);
    }
    else  if (mType == 3)
    {
        this->showVisible(true);
        if (label) {
            label->removeFromParentAndCleanup(true);
        }
        CCString *str = (CCString *)_model->getModelMsg()->objectAtIndex(_wordIndex);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
        CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
        label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
        label->setPosition(bboard->getPosition());
        this->addChild(label, 2);
    }
    else if (mType == 6 )
    {
        if (_delegateTouch == NULL) {
            _delegateTouch = delegate;
            _delegateTouch->setIsCanTouch(false);
        }
        
        ((SGStoryMainLayer*)delegate)->tableView->setMoveEnable(false);
        this->showVisible(false);
        _canTouch = true;
        CCRect rect = getRectByGuideId();
        
        float rota = -90.0f;
        float conH = spriteTag->getContentSize().height/2 + rect.size.height/2 + skewing(15);
        spriteTag->setRotation(rota);
        spriteTag->setPosition(ccp(rect.origin.x + rect.size.width/2, rect.origin.y - conH/2));
        CCAction *action = this->getActionByDirection(1);
        spriteTag->runAction(action);
    }
    else if (mType == 8 || mType == 9)
    {
        this->showVisible(false);
        _canTouch = true;
        
        CCRect rect = getRectByGuideId();
        
        float rota = 270.0f;
        if (SGGuideManager::shareManager()->limitStartStep == 18)
        {
            rota = 90.0f;
        }
        float conH = spriteTag->getContentSize().height/2 + rect.size.height + skewing(15);
        spriteTag->setRotation(rota);
        spriteTag->setPosition(ccp(rect.origin.x + rect.size.width/2, rect.origin.y - conH/2));
        if (SGGuideManager::shareManager()->limitStartStep == 18)
        {
            spriteTag->setPosition(ccp(rect.origin.x + rect.size.width/2, rect.origin.y + conH));
        }
        CCAction *action = this->getActionByDirection(1);
        spriteTag->runAction(action);
    }
    else if (mType == 16 && _model->getModelId() == 6)//如果是在装备转生的详情页面
    {
        this->showVisible(false);
        _canTouch = true;
        
        if (delegate)
        {
            delegate->setIsCanTouch(false);
        }
        
        CCRect rect = getRectByGuideId();
        
        float rota = 90.0f;
        float conH = spriteTag->getContentSize().height/2 + rect.size.height + skewing(15);
        spriteTag->setRotation(rota);
        spriteTag->setPosition(ccp(rect.origin.x + 60, rect.origin.y + conH));
        CCAction *action = this->getActionByDirection(1);
        spriteTag->runAction(action);
    }
    else if (mType == 17)//如果武将转生页面
    {
        _canTouch = true;
        
        this->spriteBlackbg->setOpacity(0);
        spriteTag->setVisible(false);
        
    }
    else if (mType == 18)
    {
        this->showVisible(false);
        _canTouch = true;
        
        if (delegate)
        {
            delegate->setIsCanTouch(false);
        }
        
        CCRect rect = getRectByGuideId();
        
        int rightShift = 200.0f;

        if (SGGuideManager::shareManager()->limitStartStep == 18 && _model->getModelId() == 21)
        {
            rightShift = 215.0f;
        }
        float rota = 90.0f;
        float conH = spriteTag->getContentSize().height/2 + rect.size.height + skewing(15);
        spriteTag->setRotation(rota);
        spriteTag->setPosition(ccp(rect.origin.x + rect.size.width/2 + rightShift, rect.origin.y + conH));
        CCAction *action = this->getActionByDirection(1);
        spriteTag->runAction(action);
    }
    else
    {
        this->showVisible(false);
        _canTouch = true;
        
        if (delegate)
        {
            delegate->setIsCanTouch(false);
        }
        if (_model->getModelId() == 84)
        {
            SGGuideManager::shareManager()->setMainTouchEnable(false);
        }
        
        CCRect rect = getRectByGuideId();
        
        float rota = 90.0f;
        float conH = spriteTag->getContentSize().height/2 + rect.size.height + skewing(15);
        spriteTag->setRotation(rota);
        spriteTag->setPosition(ccp(rect.origin.x + rect.size.width/2, rect.origin.y + conH));
        CCAction *action = this->getActionByDirection(1);
        spriteTag->runAction(action);
    }
}

CCRect SGLimitGuideLayer::getRectByGuideId()
{
    CCRect rect;
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    
    CCNode *node = NULL;
    int nodeTag = _model->getHighlightTag();
    int nodeType = _model->getModelType(); 
    int nodeId = _model->getModelId();
    
    if (   nodeType == 2
        || nodeType == 6
        || nodeType == 8)
    {
        node = nowLayer->getBtnByTag(nodeTag); //autoequip 的 nodeTag写为13
        
    }
    else if (nodeType == 7)
    {
        node = ((SGFirstLayer*)nowLayer)->scrollview->getContainer()->getChildByTag(512);//默认的第一个Tag
    }
    else if (nodeType == 9 || nodeType == 12)
    {
        SGBaseBox *nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
        node = nowBox->getBtnByTag(nodeTag);
        nowBox->setIsCanTouch(false);
    }
    else if (nodeType == 10)
    {
        SGMainLayer * mainLayer = SGMainManager::shareMain()->getMainLayer();
        node = mainLayer->getBtnByTag(nodeTag);
    }
    else if (nodeType == 11)
    {
        //_model->setHighlightTag(SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_12));
        node = nowLayer->getBtnByTag(_model->getHighlightTag());//firstlayer 强化按钮
    }
    else if (nodeType == 13)
    {
       //SNSTableView
        if (nodeId == 73 || nodeId == 103)
        {
            nodeTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_35);
        }
        else if (nodeId == 84)
        {
            nodeTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_22);
        }
        else if (nodeId == 13 || nodeId == 11)
        {
            nodeTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_52);
        }
        node = nowLayer->getChildByTag(nodeTag);
        if (node)
        {
            oldTableview = (SNSTableView *)node;
            (oldTableview)->setMoveEnable(false);
            (oldTableview)->setTouchEnabled(false);
            if (nodeId == 73 || nodeId == 103)
            {
                oldTableview = NULL;
            }
        }
    }
    else if (nodeType == 14)
    {
        SGSoldierItem *item = (SGSoldierItem *)nowLayer->getChildByTag(MENUTAG);
        node = item->getChildByTag(MENUTAG)->getChildByTag(nodeTag);
    }
    else if (nodeType == 15)
    {
        
        if (nodeTag == 512)//主页上对应的布阵界面的第一个武将
        {
            //获取首页的layer
            SGFirstLayer *firstLayer = static_cast<SGFirstLayer *>(mainScene->getChildByTag(mainScene->nowLayerTag));
            //再由首页中获取对应的布阵武将的scrollview
            CCScrollView *tempScrollView = static_cast<CCScrollView *>(firstLayer->getChildByTag(222));
            //由subLayer中获取对应的scrollview中的container
            CCLayer *containerLayer = static_cast<CCLayer *>(tempScrollView->getContainer());
            //再由container中获取对应的单个放button的sublayer
            SGFirstSubLayer *subLayer = static_cast<SGFirstSubLayer *>(containerLayer->getChildByTag(nodeTag));
            //如果有对应的subLayer，再找个这个sublayer中的menu
            if (subLayer)
            {
                CCMenu *menu = static_cast<CCMenu *>(subLayer->getChildByTag(124));
                //找到menu，再找到内部的button
                if (menu)
                {
                    SGButton *tempButton = static_cast<SGButton *>(menu->getChildByTag(nodeTag));
                    if (tempButton)
                    {
                        node = static_cast<CCNode *>(tempButton);
                        scrollViewPos = tempScrollView->getPosition();
                        subLayerPos = subLayer->getPosition();
                        node->setPosition(ccpAdd(scrollViewPos, ccpAdd(subLayerPos,
                                                                       ccp(node->getContentSize().width / 1.7, node->getContentSize().height / 1.7))));
                    }
                }
            }
            
        }
        else if (nodeTag == 527)//装备详情中的材料button
        {
            //获取装备详情的layer
            SGFirstLayer *equipInfoLayer = static_cast<SGFirstLayer *>(mainScene->getChildByTag(mainScene->nowLayerTag));
            CCLayerColor *colorLayer = static_cast<CCLayerColor *>(equipInfoLayer->getChildByTag(33));
            if (colorLayer)
            {
                CCMenu *menu = static_cast<CCMenu *>(colorLayer->getChildByTag(55));
                if (menu)
                {
                    node = menu->getChildByTag(0);//equipInfoLayer, button的tag有其他作用，这里直接使用
                    
                    node->setPosition(ccpAdd(colorLayer->getPosition(), node->getPosition()));
                    
                }
                
            }
            
        }
        else
        {
            node = nowLayer->getBtnByTag(nodeTag);
        }

    }
    else if (nodeType == 16)//转生页面的高亮区
    {
        node = nowLayer->getChildByTag(nodeTag);
        
        if (nodeTag == 525)
        {
            color = static_cast<CCLayerColor *>(nowLayer->getChildByTag(33));
        }
    }
    else if (nodeType == 18)
    {
        //闯关节列表 tableView
        node = nowLayer->getChildByTag(nodeTag);
        //整个tableview都不可触
        ((SNSTableView *)node)->setMoveEnable(false);
		((SNSTableView *)node)->setTouchEnabled(false);//11.17
		
        //获取第一个item
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);
        
        
        //碎片召唤
        if (SGGuideManager::shareManager()->limitStartStep == 10 && _model->getModelId() == 6)
        {
            SGPiecesBagCell *item = (SGPiecesBagCell *)((SNSTableView *)node)->getItemByIndexPath(path);
            CCNode *callButton = item->callBtn;
            
            if (callButton)
            {
                rect.origin.x += callButton->getPositionX();
                rect.origin.y += callButton->getPositionY();
            }
            
        }
        else if (SGGuideManager::shareManager()->limitStartStep == 18)
        {
            SGAreanItemInfo *item = static_cast<SGAreanItemInfo *>(((SNSTableView *)node)->getItemByIndexPath(path));
            CCNode *fightButton  = item->getChildByTag(11);
            if (fightButton)
            {
                rect.origin.x += fightButton->getPositionX();
                rect.origin.y += fightButton->getPositionY();
            }
        }
        else
        {
            SGStorySectionCell *item = (SGStorySectionCell*)((SNSTableView *)node)->getItemByIndexPath(path);
            //这里-20是『战』按钮的tag
            CCNode *viewBt = item->getChildByTag(-20);
            //获取『战』按钮高亮范围
            if (viewBt != NULL)
            {
                rect.origin.x += viewBt->getPositionX();
                rect.origin.y += viewBt->getPositionY();
            }
        }

    }
    else if (nodeType == 19)//装备转生最后看来源，高亮第一个cell
    {
        SGBaseBox *nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
        
        SGLootWiKiLayer *wikiLayer = static_cast<SGLootWiKiLayer *>(nowBox->getChildByTag(666));
        
        //这里node是tableview
        node = (wikiLayer->getChildByTag(666));
    }

    //如果找不到就不引导了，别直接崩溃了
    if (!node)
    {
        SGGuideManager::shareManager()->closeLimitGuide();
        return CCRectMake(0, 0, 0, 0);
    }
    
    CCAssert(node , "CCSprite: Batched sprites should use the same texture as the batchnode");
    
    CCPoint localPoint = this->convertToWorldSpace(node->boundingBox().origin);

    if (nodeType == 7)
    {
        localPoint = ccpAdd(localPoint, ((SGFirstLayer*)nowLayer)->scrollview->getPosition());
    }
    else if (nodeType == 14)
    {
        localPoint = ccpAdd(localPoint, nowLayer->getChildByTag(MENUTAG)->getPosition());
    }
    int scaleHgt = 0;
    if (nodeType == 16)//武将转生页面的九宫格背景
    {
        scaleHgt = node->boundingBox().size.height;
    }
    else
    {
        scaleHgt = node->boundingBox().size.height;
    }
    rect = CCRectMake(localPoint.x, localPoint.y, node->boundingBox().size.width, scaleHgt);
    
    setHighLightRect(node, nodeType ,nodeId,rect);
    return rect;
}

void SGLimitGuideLayer::setHighLightRect(CCNode *node, int type,int nodeId,CCRect &rect)
{
    m_removeRectTag = 0;
    if (type == 2
        || type == 6
        || type == 8
        || type == 10
        || type == 11
        || type == 14
        || type == 15)
    {
        
        btn = (SGButton *)node;
        btn->setEnabled(false);
        btn->retain();
        btn->removeFromParentAndCleanup(true);
        this->addBtn(btn);
        btn->release();
        
        

        m_removeRectTag = 1;
        if (type == 14)
        {
            btn->setPosition(ccp(rect.origin.x+rect.size.width/2, rect.origin.y));
        }
        
    }
    else if (type == 9 || type == 12)
    {
        btn = (SGButton *)node;
        btn->setEnabled(false);
        btn->retain();
        btn->removeFromParentAndCleanup(true);
        this->addBtn(btn);
        btn->release();
        m_removeRectTag = 3;
    }
    else if (type == 7)
    {
        generalIcon = node;
        ((SGFirstSubLayer*)generalIcon)->setiscantouch(false);
        generalIcon->retain();
        generalIcon->removeFromParentAndCleanup(true);
        generalIcon->setPosition(rect.origin);
        this->addChild(generalIcon);
        generalIcon->release();
        m_removeRectTag = 2;
    }
    else if(type == 13)
    {
        SNSTableView *view = (SNSTableView *)node;
        view->setMoveEnable(false);

        tableview->setFrame(view->getFrame());
        tableview->setTag(view->getTag());
        tableview->setPosition(view->getPosition());
        
        if (path) {
            path->release();
            path = NULL;
        }
        if (nodeId == 73 )
        {
             path = SNSIndexPath::create(4, 0, 1);
        }
        else if (nodeId == 84)
        {
             path = SNSIndexPath::create(0, 0, 1);
        }
        else if (nodeId == 103)
        {
           path = SNSIndexPath::create(3, 0, 1);
        }
        else
        {
            path = SNSIndexPath::create(0, 0, 1);//默认是第一个cell
        }
        path->retain();
        
        SNSTableViewCellItem *item = view->getItemByIndexPath(path);
        
        //如果这空，直接结束引导
        if (!item)
        {
            this->setVisible(false);
            this->setTouchEnabled(false);
            this->removeHighLightRect();
            SGGuideManager::shareManager()->isGuide = false;
            SGMainManager::shareMain()->showFirstLayer();
            return ;
        }
        
        item->retain();
        item->removeFromParentAndCleanup(true);

        float posX = 0;
        float posY = view->getContentSize().height - item->getContentSize().height/2 + view->getDown();
        
        if (nodeId == 73)
        {
            posY -= item->getContentSize().height * 4;
        }
        else if (nodeId == 103)
        {
            posY -= item->getContentSize().height * 3;
        }
        item->setPosition(ccpAdd(ccp(item->getPosition().x,0), ccp(posX, posY)));
        item->setTag(SNSCELLTAG);
        tableview->m_body->addChild(item);
        item->release();
        
        float rpox = item->getPositionX()-item->getContentSize().width/2;
        float rpoy = item->getPositionY()-item->getContentSize().height/2 + view->getPositionY();
        float rfrw = item->getContentSize().width;
        float rfrh = item->getContentSize().height;
        
        _rect = CCRectMake(rpox, rpoy, rfrw, rfrh);
        rect = _rect;
        this->setTableViewVisible(true);
        
        m_removeRectTag = 4;
        
    }
    else if (type == 16)
    {
        if (_model->getModelId() == 19 && SGGuideManager::shareManager()->limitStartStep == 18)
        {
            CCSprite *frame2bg = static_cast<CCSprite *>(node);
            frame2bg->retain();
            frame2bg->removeFromParentAndCleanup(true);
            this->addChild(frame2bg, 1, 121);
            frame2bg->release();
        }
        else
        {
            CCScale9Sprite *frame2bg = static_cast<CCScale9Sprite *>(node);
            frame2bg->retain();
            frame2bg->removeFromParentAndCleanup(true);
            this->addChild(frame2bg, 1, 121);
            frame2bg->release();
        }
        

        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);

        SGButton *btn = (SGButton *)nowLayer->getBtnByTag(526);
        if (btn)
        {
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            this->addChild(btn, 3);
            btn->release();
        }
        if (color)
        {
            color->retain();
            color->removeFromParentAndCleanup(true);
            static_cast<CCMenu *>(color->getChildByTag(66))->setEnabled(false);//menu设置不可用，防止穿透
            this->addChild(color, 2);
            color->release();
        }
        
    }
    else if (type == 18)
    {
        m_removeRectTag = 5;
        //整个tablebview，不可动
        SNSTableView *view = (SNSTableView *)node;
        view->setMoveEnable(false);
        _canTouch = true;
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);
        
        
        if (SGGuideManager::shareManager()->limitStartStep == 10 && _model->getModelId() == 6)
        {
            //第一个的item上面的『召』字
            SGPiecesBagCell *item = (SGPiecesBagCell*)((SNSTableView *)node)->getItemByIndexPath(path);
            CCNode *viewBt = item->callBtn;
            
            
            tableview = (SNSTableView *)node;
            path = SNSIndexPath::create(0, 0, 1);
            
            if (viewBt != NULL)
            {
                btn = (SGButton *)viewBt;
                btn->setEnabled(false);
                btn->retain();
                //旧的移除掉
                btn->removeFromParentAndCleanup(true);
                
                //加上新的『召』按钮
                btn->setEnabled(false);
                this->addBtn(btn);
                
                int posY = ((SNSTableView *)node)->getContentSize().height + item->getContentSize().height / 2 + ((SNSTableView *)node)->getDown();
                
                int leftShift = 0;
                if (CCDirector::sharedDirector()->getWinSize().height == 1024)
                {
                    leftShift = btn->getContentSize().width / 1.6;
                }
                btn->setPosition(ccpAdd(ccpAdd(item->getPosition(), viewBt->getPosition()), ccp(btn->getContentSize().width / 8 + leftShift, posY)));
                
                btn->setTag(110);;
                btn->release();
            }
        }
        else if (SGGuideManager::shareManager()->limitStartStep == 18)
        {
            //
            SGAreanItemInfo *item = (SGAreanItemInfo*)((SNSTableView *)node)->getItemByIndexPath(path);
            CCNode *viewBt = item->getChildByTag(11);
            
            
            tableview = (SNSTableView *)node;
            path = SNSIndexPath::create(0, 0, 1);
            
            if (viewBt != NULL)
            {
                btn = (SGButton *)viewBt;
                btn->setEnabled(false);
                btn->retain();
                //旧的移除掉
                btn->removeFromParentAndCleanup(true);
                
                //加上新的
                btn->setEnabled(false);
                this->addBtn(btn);
                
                int posY = ((SNSTableView *)node)->getContentSize().height + item->getContentSize().height * 1.22 + ((SNSTableView *)node)->getDown();
                
                int leftShift = -10;
                if (CCDirector::sharedDirector()->getWinSize().height == 1024)
                {
                    //leftShift = btn->getContentSize().width / 2;
                }
                btn->setPosition(ccpAdd(ccpAdd(item->getPosition(), viewBt->getPosition()), ccp(btn->getContentSize().width / 8 + leftShift, posY)));
                
                btn->setTag(110);;
                btn->release();
            }
        }
        else
        {
            //第一个的item上面的『战』字
            SGStorySectionCell *item = (SGStorySectionCell*)((SNSTableView *)node)->getItemByIndexPath(path);
            //backBtn 就是那个战按钮  好坑的变量名！！！
            CCNode *viewBt = item->backBtn;
            if (viewBt != NULL)
            {
                btn = (SGButton *)viewBt;
                btn->setEnabled(false);
                btn->retain();
                //旧的移除掉
                btn->removeFromParentAndCleanup(false);
                btn->unselected();
                CCRect rect;
                
                //计算『战』字要高亮的范围
                float ax = node->getAnchorPoint().x;
                float ay = node->getAnchorPoint().y;
                float px = node->getPositionX();
                float py = node->getPositionY();
                if (ax == 0)
                {
                    px += node->getContentSize().width/2;
                }
                else if (ax == 1.0f)
                {
                    px += -node->getContentSize().width/2;
                }
                if (ay == 0)
                {
                    py += node->getContentSize().height/2;
                }
                else if (ay == 1.0f)
                {
                    py += -node->getContentSize().height/2;
                }
                //给出高亮区域
                rect = CCRectMake(px, py, btn->getContentSize().width, btn->getContentSize().height);
                rect.origin.x +=  btn->getPositionX() + item->getContentSize().width;
                rect.origin.y -= (item->getPositionY() + btn->getPositionY());
                rect.origin.y += node->getContentSize().height/2.2 ;
                
                //加上新的『战』按钮
                btn->setPosition(rect.origin);
                btn->setTag(110);
                this->addBtn(btn);
                btn->setEnabled(false);
                btn->stopAllActions();
                if (btn->getChildByTag(SECTIONCELLEFFECTAG)) {
                    btn->getChildByTag(SECTIONCELLEFFECTAG)->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
                }
                
                btn->runAction( CCMoveBy::create(ITEMACTIONTIME*8, ccp(-item->getContentSize().width,0)));
                
                btn->release();
            }
        }
    }
    else if (type == 19)//来源弹出
    {

    }
    
}

void SGLimitGuideLayer::removeHighLightRect()
{
    if(_model == NULL) return;
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    
    
    if (_model->getModelType() == 16)//九宫格的显示背景移除
    {
        if (_model->getModelId() == 19 && SGGuideManager::shareManager()->limitStartStep == 18)
        {
            CCSprite *frame2bg = static_cast<CCSprite *>(this->getChildByTag(121));
            frame2bg->retain();
            frame2bg->removeFromParentAndCleanup(true);
            nowLayer->addChild(frame2bg);
            frame2bg->setTag(21);
        }
        else
        {
            CCScale9Sprite *frame2bg = static_cast<CCScale9Sprite *>(this->getChildByTag(121));
            frame2bg->retain();
            frame2bg->removeFromParentAndCleanup(true);
            nowLayer->addChild(frame2bg);
            frame2bg->setTag(21);
        }
        SGButton *btn = (SGButton *)this->getChildByTag(526);
        if (btn)
        {
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            nowLayer->addBtn(btn);
            btn->release();
        }
        
        if (color)
        {
            color->retain();
            color->removeFromParentAndCleanup(true);
            nowLayer->addChild(color);
            color->release();
        }
    }
    
    
    if (m_removeRectTag == 1)
    {
        CCNode *node = this->getBtnByTag(_model->getHighlightTag());
        
        //最后一步特殊处理，tag在原页面有其他作用
        if (_model->getHighlightTag() == 527)
        {
             node = this->getBtnByTag(0);
        }
        
        SGButton *btn = (SGButton *)node;
        if (btn != NULL)
        {
            btn->setEnabled(true);
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            
            if (btn->getTag() == 72 || btn->getTag() == 511 || btn->getTag() == 534)
            {
                SGMainLayer *mainlayer = SGMainManager::shareMain()->getMainLayer();
                mainlayer->addBtn(btn);
            }
            else
            {
                if(nowLayer)
                {
                    nowLayer->addBtn(btn);
                }
            }
        }
    }
    else if (m_removeRectTag == 2)
    {
        if (generalIcon) {
            ((SGFirstSubLayer*)generalIcon)->setiscantouch(true);
            generalIcon->retain();
            CCPoint localPoint = ccpSub(generalIcon->getPosition(), ((SGFirstLayer*)nowLayer)->scrollview->getPosition());
            generalIcon->setPosition(localPoint);
            generalIcon->removeFromParentAndCleanup(true);
            if(nowLayer) {
                ((SGFirstLayer*)nowLayer)->scrollview->getContainer()->addChild(generalIcon);
            }
            generalIcon->release();
        }
    }
    else if (m_removeRectTag == 3)
    {
        SGBaseBox *nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
        CCNode *node = this->getBtnByTag(_model->getHighlightTag());
        SGButton *btn = (SGButton *)node;
        if (btn != NULL) {
            btn->setEnabled(true);
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            if(nowBox) {
                nowBox->addBtn(btn);
            }
            btn->release();
        }
    }
    else if (m_removeRectTag == 4)
    {
        CCLayer *m_body = tableview->m_body;
        if (m_body != NULL)
        {
            SNSTableViewCellItem *item = (SNSTableViewCellItem *)m_body->getChildByTag(SNSCELLTAG);
            if(item != NULL) item->removeFromParentAndCleanup(true);
        }

        if (oldTableview)
        {
            oldTableview->reloadData();
            oldTableview = NULL;
        }
    }
    else if (m_removeRectTag == 5)
    {
        // 移除 闯关 节列表中『战』字的高亮
        SGButton *btn = (SGButton *)this->getBtnByTag(110);
        if (btn) btn->removeFromParentAndCleanup(true);
    }
    else if (m_removeRectTag == 0)//如果等于0
    {
        
    }
}

bool SGLimitGuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    _touchPoint = pTouch->getLocation();

    return true;
}

void SGLimitGuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (_model != NULL && _model->getModelMsg() != NULL)
    {
        _wordIndex += 1;
        int count = _model->getModelMsg()->count();
        if (_wordIndex < count) {
            CCString *str = (CCString *)_model->getModelMsg()->objectAtIndex(_wordIndex);
            label->removeFromParentAndCleanup(true);
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
            CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
            label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
            label->setPosition(bboard->getPosition());
            this->addChild(label, 2);
        }
        else
        {
            SGGuideManager::shareManager()->startLimitGuide(-11);
        }
    }
    else
    {
        if (_canTouch)
        {
            if(btn)
            {
                if (btn->boundingBox().containsPoint(_touchPoint))
                {
                    removeHighLightRect();
                    if (_model->getModelType() == 8
                        || _model->getModelType() == 9
                        || _model->getModelType() == 10
                        || _model->getModelType() == 11
                        || _model->getModelType() == 12
                        || _model->getModelType() == 14
                        || _model->getModelType() == 15
                        || _model->getModelType() == 18
                        || (SGGuideManager::shareManager()->limitStartStep == 18 && _model->getModelId() == 16)
                        || (SGGuideManager::shareManager()->limitStartStep == 3 && _model->getModelId() == 2))
                    {
                        
                        if (SGGuideManager::shareManager()->limitStartStep == 10 && _model->getModelId() == 6)
                        {
                            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
                            SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
                            SGPiecesBagLayer *layer = (SGPiecesBagLayer *)nowLayer;
                            layer->setSelectIndex(0);
                            layer->virtualPressBtn();
                        }
                        else
                        {
                            btn->setEnabled(true);
                            btn->activate();
                        }
                    }
                    //对兑技场单独处理
                    
                    _canTouch = false;
                    btn = NULL;
                    
                    if ((SGGuideManager::shareManager()->limitStartStep == 10 && _model->getModelId() == 6) || (SGGuideManager::shareManager()->limitStartStep == 8 && _model->getModelId() == 7))
                    {
                        spriteBlackbg->setOpacity(0);
                        spriteTag->setVisible(false);
                    }
                    //针对幻境单独的处理
                    else if (SGGuideManager::shareManager()->limitStartStep == 17 && _model->getModelId() == 3)
                    {
                        main::SpecialBattleStoryListRequest *storyList = new main::SpecialBattleStoryListRequest;
                        SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_CHAP, storyList);
                        SGGuideManager::shareManager()->startLimitGuide(-11);
                    }
                    else
                    {
                        SGGuideManager::shareManager()->startLimitGuide(-11);
                    }
                }
            }
            else if (generalIcon)
            {
                if (generalIcon->boundingBox().containsPoint(_touchPoint))
                {
                    removeHighLightRect();
                    ((SGFirstSubLayer*)generalIcon)->selectEmbattle();

                    _canTouch = false;
                    
                    generalIcon = NULL;
                    SGGuideManager::shareManager()->startLimitGuide(-11);
                }
            }
            else if(tableview)
            {
                if (_rect.containsPoint(_touchPoint))
                {
                    //
                    removeHighLightRect();
                    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
                    
                    if (_model->getModelId() == 73)
                    {
                        ((SGBarracksLayer*)mainScene->getChildByTag(mainScene->nowLayerTag))->consumeHandler();
                    }
                    else if (_model->getModelId() == 84)
                    {
                        ((SGGeneralsLayer*)mainScene->getChildByTag(mainScene->nowLayerTag))->itemselectByIndex(0);
                    }
                    else if (_model->getModelId() == 103)
                    {
                        ((SGBarracksLayer*)mainScene->getChildByTag(mainScene->nowLayerTag))->piecesHandler();
                    }
                    else if (_model->getModelId() == 12)
                    {
                        SGBaseBox *nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
                        SGLootWiKiLayer *wiki = ((SGLootWiKiLayer *)nowBox->getChildByTag(666));
                        wiki->tableViewSelectItem(tableview, path);
                    }
                    else if (_model->getModelId() == 13)//请求第二章,装备强化的最后一步
                    {
                        main::PlotListRequest *request = new main::PlotListRequest();
                        request->set_storyid(2);
                        request->set_poltaskid(0);
                        SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
                    }
                    else if (_model->getModelId() == 11 && SGGuideManager::shareManager()->limitStartStep == 9)//请求第五章,士兵强化的最后一步
                    {
                        main::PlotListRequest *request = new main::PlotListRequest();
                        request->set_storyid(5);
                        request->set_poltaskid(0);
                        SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
                    }
                    else if (_model->getModelId() == 11 && SGGuideManager::shareManager()->limitStartStep == 8)//请求第六章，技能强化
                    {
                        main::PlotListRequest *request = new main::PlotListRequest();
                        request->set_storyid(6);
                        request->set_poltaskid(0);
                        SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
                    }
                    SGGuideManager::shareManager()->startLimitGuide(-11);
                    
                }
                //技能强化对付穿透，如果点到除第一个之外的其他cell，不给反应
                else if (_model->getModelId() == 84 && _model->getModelType() == 13 && SGGuideManager::shareManager()->limitStartStep == 8)
                {
                    return ;
                }
                else
                {
                    removeHighLightRect();
                    SGGuideManager::shareManager()->startLimitGuide(-11);
                }
            }
            else
            {
                removeHighLightRect();
                SGGuideManager::shareManager()->startLimitGuide(-11);
            }

        }
    }
}
void SGLimitGuideLayer::setTableViewVisible(bool visible)
{
    _canTouch = visible;
    if (tableview != NULL) {
        tableview->setVisible(_canTouch);
    }
}

