//
//  SGUIGuideLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-6.
//
//

#include "SGUIGuideLayer.h"
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
#include "SGAllTaskLayer.h"
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
#include "SGDrawCards.h"
#include "SGFirstSubLayer.h"

SGUIGuideLayer::SGUIGuideLayer(void) : _direction(tip_up), spTouch(NULL), popClick(NULL)
{
    _wordIndex = 0;
    _currGuideId = 0;
    _model = NULL;
    _delegateTouch = NULL;
    _canTouch = false;
    
    label = NULL;
    speaker = NULL;
    bboard = NULL;
    //skipBtn = NULL;
    spriteTag = NULL;
    spriteBlackbg = NULL;
    tableview = NULL;
    actSprite = NULL;
    embattleItem = NULL;
    m_EmbaTimer = 0;
    
    scrollViewPos = ccp(0, 0);
    subLayerPos = ccp(0, 0);
}

SGUIGuideLayer::~SGUIGuideLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_uiguideLayer);
}
SGUIGuideLayer* SGUIGuideLayer::create()
{
    SGUIGuideLayer *model = new SGUIGuideLayer();
    if (model && model->init(NULL, sg_uiguideLayer))
    {
        model->autorelease();
        model->initView();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}
void SGUIGuideLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);//11.20
}

void SGUIGuideLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/djjx_new.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    spriteBlackbg = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    spriteBlackbg->setPosition(SGLayout::getPoint(kMiddleCenter));
    spriteBlackbg->setScaleX(size.width/spriteBlackbg->getContentSize().width);
    spriteBlackbg->setScaleY(size.height/spriteBlackbg->getContentSize().height);
    spriteBlackbg->setOpacity(179);//255*0.7
    this->addChild(spriteBlackbg, -1);
    
    float bottomH = skewing(162.0f/2);
    this->setTouchEnabled(false);

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

    speaker = CCSprite::createWithSpriteFrameName("public_guide_mm.png");

    posH += speaker->getContentSize().height/2;
    float spWid = speaker->getContentSize().width;
    speaker->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-spWid*0.32f, bottomH+posH)));
    this->addChild(speaker);

    CCSize dsize = CCSizeMake(size.width-40, size.height-50);
    label = SGRichLabel::getFomLabel("123456", dsize, FONT_PENGYOU, 32);
    label->setPosition(bboard->getPosition());
    this->addChild(label, 2);

    spriteTag = CCSprite::createWithSpriteFrameName("public_jiantou.png");

    spriteTag->setVisible(false);
    this->addChild(spriteTag, 2);
	
    spTouch = CCSpriterX::create("animationFile/jiantou.scml", true, true);
    spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
    spTouch->setanimaID(0);
    spTouch->setisloop(true);
    spTouch->play();

    spTouch->setVisible(false);
    this->addChild(spTouch,4);
    
    popClick = CCSprite::createWithSpriteFrameName("uplevel_fclick.png");
    popClick->setVisible(false);
    this->addChild(popClick,3);


    tableview = SNSTableView::create(CCRectMake(0, 0, 1, 1), TableViewTypeVertical);
    tableview->setVisible(_canTouch);
    tableview->setMoveEnable(false);
    this->addChild(tableview);
}

void SGUIGuideLayer::setDatas(SGGuideVO *model, SGBoxDelegate *canTouch)
{
    this->setTableViewVisible(false);
    
    if(spriteTag) spriteTag->stopAllActions();
    
    if (_delegateTouch) {
        _delegateTouch = canTouch;
        _delegateTouch->setIsCanTouch(false);
    }
    this->setTouchEnabled(true);
    
    if (_model)
    {
        CC_SAFE_DELETE(_model);
    }
    _model = model;
    _model->retain();
    
    _wordIndex = 0;
    _currGuideId = _model->getModelId();
    
    if (_currGuideId == guide_tag_45)
    {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGAllTaskLayer *nowLayer = (SGAllTaskLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
        if (nowLayer->tableView)
        {
            nowLayer->tableView->setTouchEnabled(false);
        }
    }
    
    int mType = _model->getModelType();
    if (mType == 1)
    {
        this->showVisible(true);
        label->removeFromParentAndCleanup(true);
        CCString *str = (CCString *)_model->getModelMsg()->objectAtIndex(_wordIndex);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
        CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
        label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
        label->setPosition(bboard->getPosition());
        this->addChild(label, 2);
    }
    else if (mType == 2 || mType == 3)
    {
        this->showVisible(false);

        _direction = tip_down;
        if (_currGuideId==guide_tag_38)
        {
           _direction = tip_up; 
        }

		
        float conW = 0;
        float conH = 0;
        float rota = 0;
		if (_currGuideId == guide_tag_14 || _currGuideId == guide_tag_22 || _currGuideId == guide_tag_37)
		{
			conW = 1200.0f;
		}
		//引导位置重置(故事模式中,箭头指向战字)
		if ( _currGuideId == guide_tag_4 || _currGuideId == guide_tag_8 || _currGuideId == guide_tag_21
            ||_currGuideId==guide_tag_51)
		{
			conW = 200.0f;
			if ((CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width ||
				CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width ||
                CCEGLView::sharedOpenGLView()->getFrameSize().width == iphonehdResource.size.width) && _currGuideId != guide_tag_21)
			{
				conW = 0.0f;
			}
            
            
#if (PLATFORM == ANDROID)
            conW = 0.0f;

            //在三关的时候箭头向右侧偏移
            if (_currGuideId == guide_tag_21)
            {
                conW = 200.0f;
            }
#endif
			
		}
        CCRect rect = this->getRectByGuideId(_currGuideId);
					

        if (_direction == tip_down)
        {
            rota = 90.0f;
            conH = spriteTag->getContentSize().height/2 + rect.size.height/2 + skewing(15);
        }
        else if (_direction == tip_up)
        {
            rota = -90.0f;
            conH = -(spriteTag->getContentSize().height/2 + rect.size.height/2 + skewing(15));
        }
        else if (_direction == tip_left)
        {
            rota = 180.0f;
            conW = spriteTag->getContentSize().width/2 + rect.size.width/2 + skewing(15);
        }
        else if (_direction == tip_right)
        {
            rota = 0;
            conW = -(spriteTag->getContentSize().width/2 + rect.size.width/2 + skewing(15));
        }
        if (_currGuideId == guide_tag_40)
        {
            
            spriteTag->setVisible(false);
            spTouch->stopAllActions();
            spTouch->setVisible(true);
            
            spTouch->setPosition(ccp(rect.origin.x+conW/2, rect.origin.y+conH/2));
            
            
            popClick->stopAllActions();
            popClick->setVisible(spTouch->isVisible());
            popClick->setPosition(ccpAdd(spTouch->GetPosition(), ccp(popClick->getContentSize().width/2,10)));
            CCAction *action = CCSequence::create(CCScaleTo::create(0.12f, 1.1f),
                                                  CCScaleTo::create(0.12f, 1.2f),
                                                  CCScaleTo::create(0.12f, 1.1f),
                                                  CCScaleTo::create(0.12f, 1.0f),
                                                  NULL);
            popClick->runAction(CCRepeatForever::create((CCActionInterval*)action));
            
        }
        else
        {
            spriteTag->setRotation(rota);
            spriteTag->setPosition(ccp(rect.origin.x+conW, rect.origin.y+conH));
            CCAction *action = this->getActionByDirection(_direction);
            spriteTag->runAction(action);
        }

    }
}

void SGUIGuideLayer::showVisible(bool visible)
{
    spriteTag->setVisible(!visible);
    
    label->setVisible(visible);
    speaker->setVisible(visible);
    bboard->setVisible(visible);
}

void SGUIGuideLayer::setTableViewVisible(bool visible)
{
    _canTouch = visible;
    if (tableview != NULL)
    {
        tableview->setVisible(_canTouch);
    }
}

void SGUIGuideLayer::skipAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    DDLog("kk------------------skip");
    SGGuideManager::shareManager()->closeGuide();
    
    main::BeginnersGuideRequest *request = new main::BeginnersGuideRequest();
    request->set_id(guide_tag_99);
    SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_ASK, request, false);
    

    SGGuideManager::shareManager()->setMainTouchEnable(true);
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_99);
    CCUserDefault::sharedUserDefault()->flush();
}

CCAction* SGUIGuideLayer::getActionByDirection(tipDirect direct)
{
    CCAction *action = NULL;
    float x = 0, y = 0;
    if (direct == tip_down)
    {
        y = skewing(15);
    }
    else if (direct == tip_up)
    {
        y = skewing(-15);
    }
    else if (direct == tip_left)
    {
        x = skewing(15);
    }
    else if (direct == tip_right)
    {
        x = skewing(-15);
    }
    
    action = CCSequence::create(CCMoveBy::create(0.5f, ccp(-x, -y)), CCMoveBy::create(0.5f, ccp(x, y)), NULL);
    return CCRepeatForever::create((CCActionInterval *)action);
}

CCRect SGUIGuideLayer::getRectByGuideId(int ggid)
{
    CCRect rect;
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    
    
    CCNode *node = NULL;
    int nodeTag = _model->getModelTag();
    int nodeType = _model->getModelType(); // 1 NO 2 SGButton 3 SNSTableView
    
    //第十二步引导的时候，要指向对应的上阵武将列表中的第一个数据，这里默认用的noteType == 2，内部使用_currGuideId区分
    if (nodeType == 2)
    {
        if (_currGuideId==guide_tag_21 || _currGuideId == guide_tag_48)
        {
            SGBaseBox *nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
            node = nowBox->getBtnByTag(nodeTag);
        }
        else if (_currGuideId==guide_tag_31 || _currGuideId == guide_tag_18 || _currGuideId == guide_tag_34)//新手引导 强化没有返回，直接点击首页
        {
            node = mainLayer->getBtnByTag(nodeTag);
        }
        else if (_currGuideId==guide_tag_2 || _currGuideId == guide_tag_44)
        { // 闯关模式的按钮tag == 502
            node = nowLayer->getBtnByTag(guide_tag_2+500);//mainLayer
        }
        else if (_currGuideId==guide_tag_11
                  || _currGuideId==guide_tag_43 || _currGuideId == guide_tag_7_1)
        {
            // 首页的 Button
            node = mainLayer->getBtnByTag(11+500);
        }
        //第十二步引导出现的情况，先找到firstlayer上的scrollview，再找到container，再找到sublayer，再找到button
        else if (_currGuideId == guide_tag_12)
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
        else
        {
            node = nowLayer->getBtnByTag(nodeTag);
        }
    }
    else if (nodeType == 3)
    {
        
        //对应新手引导只要到闯关的节列表，这个tableview的tag都是504
        if (_currGuideId==guide_tag_4 ||_currGuideId==guide_tag_8 ||  _currGuideId == guide_tag_21
            || _currGuideId==guide_tag_51 || _currGuideId==guide_tag_54 || _currGuideId==guide_tag_57
            || _currGuideId == guide_tag_47)
        {
            nodeTag = guide_tag_4 + 500; // 场景TableView
        }
        //只要到援军列表的引导tableview的tag都是506
        if (_currGuideId==guide_tag_9 || _currGuideId==guide_tag_55 || _currGuideId==guide_tag_58)
        {
            nodeTag = guide_tag_6 + 500; //援军TableView
        }
        if (_currGuideId == guide_tag_40 )
        {
            nodeTag = guide_tag_36 + 500 - 1; // guide_tag_36的modelTag
        }
        
        node = nowLayer->getChildByTag(nodeTag);
    }
    if (!node)
    {
        return CCRectMake(200, 200, 800, 800);
    }
    
    CCAssert(node , "CCSprite: Batched sprites should use the same texture as the batchnode");
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
    rect = CCRectMake(px, py, node->getContentSize().width, node->getContentSize().height);
    
    if (_currGuideId == guide_tag_32)
    { // 探访
        SNSTableView *view = (SNSTableView *)node;
        view->setMoveEnable(false);
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);
        SGVisitItem1 *item = (SGVisitItem1 *)view->getItemByIndexPath(path);
        if (item != NULL)
        {
            rect.origin.x += item->visit1->getPositionX() - item->visit1->getContentSize().width/2;
            rect.origin.y += item->visit1->getPositionY();
            rect.origin.y -= 136;
        }
    }
    
    //只要是新手引导的 闯关 节列表 tableview的操作如下,(高亮第一个右侧的『战』按钮)
    if ( _currGuideId == guide_tag_4 || _currGuideId == guide_tag_8 || _currGuideId==guide_tag_51 || _currGuideId==guide_tag_54 || _currGuideId==guide_tag_57 || _currGuideId == guide_tag_47)
    {
        //整个tableview都可触
        ((SNSTableView *)node)->setMoveEnable(false);
		((SNSTableView *)node)->setTouchEnabled(false);//11.17
		
        //获取第一个item
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);
        SGStorySectionCell *item = (SGStorySectionCell*)((SNSTableView *)node)->getItemByIndexPath(path);
        
        //尝试做展开奖励操作
        if (_currGuideId == guide_tag_47)
        {
            SGStorySectionLayer *sectionLayer = (SGStorySectionLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);

            if (sectionLayer)
            {
                sectionLayer->showFirstSectionInfo();
            }
        }
        
        
        //这里-20是『战』按钮的tag
        CCNode *viewBt = item->getChildByTag(-20);
        //获取『战』按钮高亮范围
        if (viewBt != NULL)
        {
            rect.origin.x += viewBt->getPositionX();
            rect.origin.y += viewBt->getPositionY();
        }
    }
    
    if (_currGuideId == guide_tag_15)
    {
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);//((SNSTableView *)node)->getRowCount()-1
        SGStorySectionCell *item = (SGStorySectionCell*)((SNSTableView *)node)->getItemByIndexPath(path);
        if (item != NULL)
        {
//            rect.origin.y -= item->getContentSize().height*(((SNSTableView *)node)->getRowCount()-1);

        }
    }
    
    
    this->setHighLightRect(node, nodeType);
    if (_currGuideId == guide_tag_14 || _currGuideId == guide_tag_22 ||_currGuideId == guide_tag_37)
    { //
        px -= _rect.size.width * 2;
        rect.origin.x = px;
        if (_currGuideId == guide_tag_22)
        {
            rect.origin.y -= 100;
        }
    }
    if (_currGuideId == guide_tag_36 || _currGuideId == guide_tag_40)
    { //
        px -= (_rect.size.width+5);
        py -= node->getContentSize().height*.5f;
        
        py += _rect.size.height;
        if (_currGuideId == guide_tag_40)
        {
            px += 25;
            py -= 20;
        }
        if (_currGuideId == guide_tag_36)
        {
            py -= 70;
        }
        rect.origin.x = px;
        rect.origin.y = py;

    }
    DDLog("ccc1-----------------%f, %f, %f, %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    return rect;
}

void SGUIGuideLayer::setHighLightRect(CCNode *node, int type, CCArray *data)
{
    if (_currGuideId == guide_tag_40 || _currGuideId == guide_tag_36)
    { //scroll view
        this->setTableViewVisible(false);
        _canTouch = true;
        
        SNSScrollView *view = (SNSScrollView *)node;
        
        SNSScrollView *sview = SNSScrollView::create(view->getFrame());
        sview->setFrame(view->getFrame());
        sview->setPosition(view->getPosition());
        sview->setHorizontal(false);
        sview->setVertical(false);
        sview->setTag(view->getTag());
        this->addChild(sview);
        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGEmbattleLayer *nowLayer = (SGEmbattleLayer *)mainScene->getChildByTag(sg_embattleLayer);
        SGEmbattleItem *item = nowLayer->getItemByIndex(1);
        
        item->removeFromParentAndCleanup(true);
        sview->m_body->addChild(item);
        item->setTag(1001);
        
        
        float rfrw = item->getWidth();
        float rfrh = item->getHeight();
        CCPoint itemPos = sview->getParent()->convertToNodeSpace(item->getPosition());
        itemPos = sview->convertToWorldSpace(itemPos);
        _rect = CCRectMake(itemPos.x-rfrw/2 , itemPos.y-rfrh/2 , rfrw, rfrh);
        
        return;
    }
    
    if (_currGuideId == guide_tag_32)
    { // 探访
        SNSTableView *view = (SNSTableView *)node;
        view->setMoveEnable(false);
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);
        SGVisitItem1 *item = (SGVisitItem1 *)view->getItemByIndexPath(path);
        if (item != NULL) {
            SGButton *btn = (SGButton *)item->visit1;
            btn->setEnabled(true);
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            
            CCRect rect;
            
            float ax = node->getAnchorPoint().x;
            float ay = node->getAnchorPoint().y;
            float px = node->getPositionX();
            float py = node->getPositionY();
            if (ax == 0) {
                px += node->getContentSize().width/2;
            }else if (ax == 1.0f) {
                px += -node->getContentSize().width/2;
            }
            if (ay == 0) {
                py += node->getContentSize().height/2;
            }else if (ay == 1.0f) {
                py += -node->getContentSize().height/2;
            }
            rect = CCRectMake(px, py, node->getContentSize().width, node->getContentSize().height);
            rect.origin.x += item->visit1->getPositionX();
            rect.origin.y = item->convertToWorldSpace(btn->getPosition()).y;

            btn->setPosition(rect.origin);
            this->addBtn(btn);
            btn->release();
        }
        return;
    }
    
    //闯关的 节列表 这里都是需要高亮『战』按钮
    if (_currGuideId == guide_tag_4 || _currGuideId == guide_tag_8 || _currGuideId == guide_tag_21 || _currGuideId==guide_tag_51 || _currGuideId==guide_tag_54 || _currGuideId==guide_tag_57|| _currGuideId == guide_tag_47)
    {
        //整个tablebview，不可动
        SNSTableView *view = (SNSTableView *)node;
        view->setMoveEnable(false);
        _canTouch = true;
        SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);
        
        //第一个的item上面的『战』字
        SGStorySectionCell *item = (SGStorySectionCell*)((SNSTableView *)node)->getItemByIndexPath(path);
        //backBtn 就是那个战按钮  好坑的变量名！！！
        CCNode *viewBt = item->backBtn;
        if (viewBt != NULL)
        {
            SGButton *btn = (SGButton *)viewBt;
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
            if (btn->getChildByTag(SECTIONCELLEFFECTAG))
            {
                btn->getChildByTag(SECTIONCELLEFFECTAG)->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
            }
            if (_currGuideId == guide_tag_4)
            {
                 btn->runAction( CCMoveBy::create(ITEMACTIONTIME*8, ccp(-item->getContentSize().width,0)));
            }
            else
            {
                btn->setPosition(ccp(rect.origin.x-item->getContentSize().width,rect.origin.y));
            }
           
            btn->release();
             _rect = CCRectMake(rect.origin.x -item->getContentSize().width-btn->getContentSize().width/2, rect.origin.y - btn->getContentSize().height/2, rect.size.width, rect.size.height);
        }
        return;
    }
    
    
    if (type == 2)
    {
        
        if (_currGuideId == guide_tag_12)
        {
            SGButton *btn = (SGButton *)node;
            btn->setEnabled(true);
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            
            this->addBtn(btn);
            //btn->setPosition(ccpAdd(scrollViewPos, subLayerPos));
            btn->release();
        }
        else
        {
            SGButton *btn = (SGButton *)node;
            btn->setEnabled(true);
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            this->addBtn(btn);
            btn->release();
        }
        

    }
    else if (type == 3)
    { // tableview
        SNSTableView *view = (SNSTableView *)node;
        view->setMoveEnable(false);
        
        tableview->setFrame(view->getFrame());
        tableview->setTag(view->getTag());
        tableview->setPosition(view->getPosition());
        
        SNSIndexPath *path = NULL;
        if (_currGuideId == guide_tag_45 /*|| _currGuideId == guide_tag_37*/)
        {
            path = SNSIndexPath::create(1, 1);
        }
        else
        {
            path = SNSIndexPath::create(0, 0, 1);
        }
        
        SNSTableViewCellItem *item = view->getItemByIndexPath(path);
        item->retain();
		if (_currGuideId == guide_tag_22)//保留转生tabview选择时特效
		{
			item->removeFromParentAndCleanup(false);
            ((SGCardItem*)item)->setItemColor(ccWHITE);
		}
		else
		{
			item->removeFromParentAndCleanup(true);
        }
        float posX = 0;
        float posY = view->getContentSize().height - item->getContentSize().height/2 + view->getDown();

        item->setPosition(ccpAdd(ccp(item->getPositionX(), 0), ccp(posX, posY)));
        item->setTag(1001);
        tableview->m_body->addChild(item);
        item->release();
        
        float rpox = item->getPositionX()-item->getContentSize().width/2;
        float rpoy = item->getPositionY()-item->getContentSize().height/2 + view->getPositionY();
        float rfrw = item->getContentSize().width;
        float rfrh = item->getContentSize().height;
        
        if (_currGuideId == guide_tag_14 || _currGuideId == guide_tag_22 ||_currGuideId == guide_tag_37) { // // «øªØ◊™…˙øÚ
            rpox = node->getPositionX() + node->getContentSize().width/2 - item->getContentSize().width*.5f;
        }
        
        _rect = CCRectMake(rpox, rpoy, rfrw, rfrh);
        DDLog("ccc2-----------------%f, %f, %f, %f", rpox, rpoy, rfrw, rfrh);
        
        this->setTableViewVisible(true);
    }
}

void SGUIGuideLayer::removeHighLightRect()
{
    if(_model == NULL) return;
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
	SGBaseBox *nowBox = NULL;
	if ((int)mainScene->nowBoxTag != -1)
	{
		nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
    }


    if (_currGuideId == guide_tag_45)
    {
        SGAllTaskLayer *nowLayer = (SGAllTaskLayer *)mainScene->getChildByTag(sg_taskLayer);
        bool dismiss = SGGuideManager::shareManager()->isDisMiss;
        if(nowLayer != NULL)
        {
            if (!dismiss)
            {
                nowLayer->tableView->setTouchEnabled(true);
            }
            CCAction *action = CCSequence::create(CCDelayTime::create(0.5f),
                                                  CCCallFunc::create(this, callfunc_selector(SGUIGuideLayer::resetTaskTouch)),
                                                  NULL);
            this->runAction(action);
        }
        return;
    }
    
    if (_currGuideId == guide_tag_32 || _currGuideId == guide_tag_4|| _currGuideId == guide_tag_8
        ||_currGuideId == guide_tag_21 ||_currGuideId==guide_tag_51
        || _currGuideId==guide_tag_54 || _currGuideId==guide_tag_57 || _currGuideId==guide_tag_47)
    {
        // 移除 闯关 节列表中『战』字的高亮
        SGButton *btn = (SGButton *)this->getBtnByTag(110);
        if (btn) btn->removeFromParentAndCleanup(true);
    }
    
    // 1 NO 2 SGButton 3 SNSTableView
    int nodeType = _model->getModelType();
    
    if (nodeType == 2)
    {
        CCNode *node = this->getBtnByTag(_model->getModelTag());

        if (_currGuideId==guide_tag_43)
        {
            node = this->getBtnByTag(11+500);
        }
        SGButton *btn = (SGButton *)node;
        if (btn != NULL)
        {
            btn->setEnabled(true);
            btn->retain();
            btn->removeFromParentAndCleanup(true);
            
            if (_currGuideId==guide_tag_21  )
            { // Now Box Button
                SGBaseBox *nowBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
                if(nowBox) nowBox->addBtn(btn);
            }
            else if (_currGuideId==guide_tag_31 )
            { // Other MainLayer Button
                if(mainLayer) mainLayer->addBtn(btn);
            }
            else if (_currGuideId==guide_tag_2|| _currGuideId==guide_tag_19 || _currGuideId == guide_tag_44)//点击闯关之后不再显示闯关按钮
            { // Story Button
                //if(mainLayer) mainLayer->addBtn(btn);
            }
            else if (_currGuideId==guide_tag_11 || _currGuideId==guide_tag_19
                      || _currGuideId==guide_tag_43 || _currGuideId == guide_tag_7_1
                     || _currGuideId == guide_tag_18 || _currGuideId == guide_tag_34)//添加之后要重新添加，否则会出现对应的homebutton丢失的问题
            { // First Button
                if(mainLayer) mainLayer->addBtn(btn);
            }
            else
            { // Now Layer Button
                if(nowLayer)
                { // tag = 17
                    if (_currGuideId != guide_tag_24
                        && _currGuideId != guide_tag_20
                        &&_currGuideId != guide_tag_22_1
                        && _currGuideId != guide_tag_38
                        && _currGuideId != guide_tag_41
                        &&_currGuideId != guide_tag_7_2
                        &&_currGuideId != guide_tag_12
                        &&_currGuideId != guide_tag_13//点击首页闯关，之后不再显示，这里不再addBtn
                        &&_currGuideId != guide_tag_48)//最后领取奖励的弹出框的确定按钮
                    {
                        nowLayer->addBtn(btn);
                    }
                }
            }
            
            btn->release();
        }
		//防止强化完后出现二个返回按钮
		if (_currGuideId == guide_tag_18 && nowLayer->getBtnByTag(518))
		{
			(nowLayer->getBtnByTag(518))->setVisible(false);
		}

		
    }
    else if (nodeType == 3)
    {
        if ( _currGuideId == guide_tag_36)
        {
            //scroll view
            this->removeChildByTag(_model->getModelTag(), true);
        }
        else if (_currGuideId == guide_tag_40)
        {
            CCNode *node = embattleItem->getParent();
            node->removeChildByTag(10000,true);//拿到加的那个临时蒙黑图
            removeChildByTag(10000,true);//拿到加的那个临时蒙黑图
            embattleItem->removeFromParentAndCleanup(true);
            
            SNSScrollView *view = (SNSScrollView *)this->getChildByTag(guide_tag_36+500);
            node = view->m_body->getChildByTag(1001);
            if(node)
            {
                node->removeFromParentAndCleanup(true);
            }
        }
        else
        {
            CCLayer *m_body = tableview->m_body;
            if (m_body != NULL) {
                SNSTableViewCellItem *item = (SNSTableViewCellItem *)m_body->getChildByTag(1001);
                if(item != NULL) item->removeFromParentAndCleanup(true);
            }
            int nodeTag = _model->getModelTag();
            //获取闯关 节列表 『战』的tag
            if ( _currGuideId==guide_tag_4 || _currGuideId==guide_tag_8 || _currGuideId==guide_tag_51 || _currGuideId==guide_tag_54 || _currGuideId==guide_tag_57)
            {
                nodeTag = guide_tag_4 + 500;
            }
            //援军列表 tableview的tag
            if (_currGuideId==guide_tag_9 || _currGuideId==guide_tag_55 || _currGuideId==guide_tag_58)
            {
                nodeTag = guide_tag_6 + 500;
            }
            SGBaseTableLayer *ttlayer = (SGBaseTableLayer *)nowLayer;
            SNSTableView *ttview = (SNSTableView *)ttlayer->getChildByTag(nodeTag);
            if (ttview != NULL) ttview->reloadData();
            if (_currGuideId == guide_tag_15 && ttview != NULL) {
                SNSIndexPath *path = SNSIndexPath::create(0, 0, 1);//view->getRowCount()-1
                Tabel_Item *item = (Tabel_Item *)ttview->getItemByIndexPath(path);
                if(item != NULL)
                {
                    item->setItemSelect(true);
                    ((SGStrengLayer*) ttlayer)->setSelectCard(item);
                }
            }
        }
    }
}


void SGUIGuideLayer::addTimer()
{
    m_EmbaTimer++;

    if (m_EmbaTimer >= 5)
    {
        this->unschedule(schedule_selector(SGUIGuideLayer::addTimer));
        
        if (embattleItem) {
            embattleItem->isCanMove= true;
            embattleItem->firstPoint = embattleItem->face->getPosition();
            
            CCNode *node = embattleItem->getParent();
            if (node->getChildByTag(10000) == NULL)
            {
                ///在移动的时候有一个灰色的底还放在那里
                CCSprite * sprite = SGDrawCards::createNormalSprite(embattleItem->officerCard->getItemId(),6,sg_emBattleItem);
                sprite->setPosition(ccpAdd(embattleItem->getPosition(),ccp(embattleItem->face->getContentSize().width*0, embattleItem->face->getContentSize().height*0)));
                CCSprite *cardbg = CCSprite::createWithSpriteFrameName("card_bj.png");
                cardbg->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));//蒙黑的图
                sprite->addChild(cardbg,100);
                
                node->addChild(sprite);
                sprite->setTag(10000);
            }
           
            embattleItem->namelabel->setColor(ccGRAY);
            
            node->reorderChild(embattleItem, 10);
            embattleItem->startRock();
            
            ///
            spTouch->stopAllActions();
            spTouch->setanimaID(0);
            spTouch->stop();
            spTouch->setScale(1.0);
            spTouch->setVisible(true);
            float distX = -50;
            float distY = 160;
            CCAction *moveAction = CCRepeatForever::create(
                                                           CCSequence::create(
                                                                              CCMoveBy::create( 0.5, ccp(distX,distY)),
                                                                              CCDelayTime::create(0.8),
                                                                              CCMoveBy::create(0.01, ccp(-distX ,-distY)),
                                                                              NULL));
            spTouch->runAction(moveAction);
            popClick->stopAllActions();
            popClick->setVisible(false);
        }
    }
}
bool SGUIGuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (_currGuideId == guide_tag_1)
    {
        _canTouch = true;
    }
    
    _touchPoint = pTouch->getLocation();
    if ( _currGuideId == guide_tag_40 && _canTouch && _rect.containsPoint(_touchPoint))
    {
        
        SGEmbattleLayer *nowLayer = (SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer);
         embattleItem = (SGEmbattleItem*) (nowLayer->cardArray)->objectAtIndex(1);
        if (embattleItem->officerCard && embattleItem->_islord==0)//主将框不让动
        {
             m_EmbaTimer = 0;
            this->unschedule(schedule_selector(SGUIGuideLayer::addTimer));
            this->schedule(schedule_selector(SGUIGuideLayer::addTimer),0.1);
            embattleItem->isSelect = true;
        }
    }
    return true;
}
void SGUIGuideLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (_currGuideId == guide_tag_40 && embattleItem)
    {
        if (embattleItem->isCanMove)
        {
            embattleItem->stopRock();
            CCPoint point =  embattleItem->convertTouchToNodeSpace(pTouch);
            embattleItem->face->setPosition(point);
            
            if (embattleItem->delegate)
            {
                CCPoint pointMove = convertTouchToNodeSpace(pTouch);
                pointMove = convertToWorldSpace(pointMove);
                embattleItem->delegate->moveToLordItemPoint(pointMove, embattleItem->index);
            }
        }
    }
    return;
}
void SGUIGuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (_wordIndex == 1)
	{
		//return ;
	}
    int currId =  _model->getModelId();
    if (_model->getModelMsg() != NULL)
    {
        _wordIndex += 1;
        int count = _model->getModelMsg()->count();
        if (_wordIndex < count)
        {
            CCString *str = (CCString *)_model->getModelMsg()->objectAtIndex(_wordIndex);
            label->removeFromParentAndCleanup(true);
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
            CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
            label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
            label->setPosition(bboard->getPosition());
            this->addChild(label, 2);
            
            if (_currGuideId==UIG_MAX1)
            {

            }
            else if (_currGuideId==UIG_MAX2)
            {

            }
            
            if ((_currGuideId == guide_tag_45) && _wordIndex==count-1)
            {

            }
        }
        else
        {
            if (_currGuideId==UIG_MAX1)
            {
                SGGuideManager::shareManager()->dismissLayer();
                SGMainManager::shareMain()->getMainLayer()->requestStory();
				CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_45);
            }
            else if (_currGuideId==UIG_MAX2)
            {
                SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
                SGPlayerInfo::sharePlayerInfo()->setPlotId(SUPERGUIDPLOTID);
                SGFindHelpLayer *nowLayer = (SGFindHelpLayer *)mainScene->getChildByTag(sg_findHelplayer);
                SGBattleManager::sharedBattle()->pveStart(5, SUPERGUIDPLOTID, nowLayer->getRoleId());
                
                SGGuideManager::shareManager()->closeGuide();
            }
            else if (_currGuideId == guide_tag_25)
            {
                main::PlotListRequest *request = new main::PlotListRequest();
                request->set_storyid(1);
                request->set_poltaskid(1);
                SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
                return;
            }
            else
            {
               if (_currGuideId == guide_tag_17)
               {
                        SGStrengLayer* strenglayer = (SGStrengLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_strengLayer);
                        if (strenglayer != NULL)
                        {
                            SGMainManager::shareMain()->showLayer(strenglayer);
                        }
               }

                SGGuideManager::shareManager()->checkIsDone(currId+1);
            }
        }
    }
    if (_canTouch)
    {
        DDLog("touch-----1=======================%f, %f", _touchPoint.x, _touchPoint.y);
        if (_rect.containsPoint(_touchPoint))
        {
            DDLog("touch-----2=======================%f, %f", _touchPoint.x, _touchPoint.y);
            _canTouch = false;
            
            if (_currGuideId==UIG_MAX1)
            {
                SGGuideManager::shareManager()->dismissLayer();
                return;
            }
            if (_currGuideId== UIG_MAX2)
            {
                SGGuideManager::shareManager()->closeGuide();
                return;
            }
            
            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
            
            //最后一个引导，请求第二关
            if (_currGuideId == guide_tag_52)
            {
                //点击第二章
                SGMainManager::sendOperatorRecord(207006);
                
                main::PlotListRequest *request = new main::PlotListRequest();
                request->set_storyid(2);
                request->set_poltaskid(0);
                SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
            }
            else if (_currGuideId == guide_tag_3  || _currGuideId == guide_tag_7_3 || _currGuideId == guide_tag_20 || _currGuideId == guide_tag_45)
            {
                
                //新手引导记录 点击第一章
                if(_currGuideId == guide_tag_3)  SGMainManager::sendOperatorRecord(201001);
                else if(_currGuideId == guide_tag_7_3)  SGMainManager::sendOperatorRecord(202002);
                else if(_currGuideId == guide_tag_20)  SGMainManager::sendOperatorRecord(204002);
                else if(_currGuideId == guide_tag_45) SGMainManager::sendOperatorRecord(207002);
                    
                main::PlotListRequest *request = new main::PlotListRequest();
                request->set_storyid(1);
                request->set_poltaskid(0);
                SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
            }
            else if (_currGuideId==guide_tag_4 || _currGuideId==guide_tag_8 || _currGuideId==guide_tag_21
                     || _currGuideId==guide_tag_51
                     || _currGuideId==guide_tag_54 || _currGuideId==guide_tag_57)
            {
                
                //点击则针对高亮区域 进行操作， 这里是闯关的 节列表的 『战』点击的触发逻辑
                if ( _currGuideId==guide_tag_4 )//新手引导  闯关1-1
                {
                    SGMainManager::sendOperatorRecord(201002);
                }
                else if (_currGuideId==guide_tag_8)//新手引导  闯关1-2
                {
                    SGMainManager::sendOperatorRecord(202003);
                }
                else if (_currGuideId==guide_tag_21 )//新手引导 闯关1-3
                {
                    SGMainManager::sendOperatorRecord(204003);
                }
                
                main::CooperationRequest *request = new main::CooperationRequest();
                SGSocketClient::sharedSocketClient()->send(MSG_STORY_FINDHELPER, request);

                if (SGGuideManager::shareManager()->getGuideLayer())
                {
                    SGGuideManager::shareManager()->getGuideLayer()->setVisible(false);
                    SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
                }

            }
            //在援军列表进入战斗的时候，确定进哪个章
            else if (_currGuideId==guide_tag_6 || _currGuideId==guide_tag_9 || _currGuideId==guide_tag_52 || _currGuideId==guide_tag_55 || _currGuideId==guide_tag_58)
            {
                int storyId = 0, plotId = 0;
                if (_currGuideId==guide_tag_58 || _currGuideId==guide_tag_55 || _currGuideId==guide_tag_52)
                {
                    //第五章
                    storyId = 5;
                }
                else if (_currGuideId==guide_tag_6 || _currGuideId==guide_tag_9 )
                {
                    //第一章
                    storyId = 1;
                }

                switch (_currGuideId )
                {
//                    case guide_tag_6://新手引导 1-1 找个人帮忙
//                        SGMainManager::sendOperatorRecord(201002);
//                        plotId = 1;
//                        break;
//                    case guide_tag_9://新手引导 1-2 找个人帮忙
//                        plotId = 2;
//                        SGMainManager::sendOperatorRecord(202001);
//                        break;
//                    case guide_tag_29://新手引导 1-3 找个人帮忙
//                        SGMainManager::sendOperatorRecord(203001);
//                        plotId = 3;
//                        break;
                    case guide_tag_49:
                        plotId = 13;
                        break;
                    case guide_tag_52:
                        plotId = 14;
                        break;
                    case guide_tag_55:
                        plotId = 15;
                        break;
                    case guide_tag_58:
                        plotId = 16;
                        break;
                    default:
                        break;
                }
                SGPlayerInfo::sharePlayerInfo()->setPlotId(plotId);
                
                SGFindHelpLayer *nowLayer = (SGFindHelpLayer *)mainScene->getChildByTag(sg_findHelplayer);
                SGBattleManager::sharedBattle()->pveStart(storyId, plotId, nowLayer->getRoleId());
                
//                // for test
//                main::PlotQuitEndRequest *request = new main::PlotQuitEndRequest();
//                request->set_cooid(nowLayer->getRoleId());
//                request->set_plotid(plotId);
//                request->set_storyid(storyId);
//                SGSocketClient::sharedSocketClient()->send(MSG_STORY_BATTLE_QEND, request);
                
                if (SGGuideManager::shareManager()->getGuideLayer())
                {
                    SGGuideManager::shareManager()->getGuideLayer()->setVisible(false);
                    SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
                }
            }
            else if (_currGuideId == guide_tag_14)
            {
                SGGeneralsLayer *nowLayer = (SGGeneralsLayer *)mainScene->getChildByTag(sg_generalsLayer);
                nowLayer->openStrengView(0);
            }
            else if (_currGuideId == guide_tag_15)
            {
                SGStrengLayer *nowLayer = (SGStrengLayer *)mainScene->getChildByTag(sg_strengLayer);
                
                nowLayer->selectItemByIndex(0);//nowLayer->tableView->getRowCount()-1
                CCLayer *m_body = tableview->m_body;
                if (m_body != NULL) {
                    Tabel_Item *item = (Tabel_Item *)m_body->getChildByTag(1001);
                    if(item != NULL) item->setItemSelect(true);
                }
                SGGuideManager::shareManager()->checkIsDone(currId+1);
            }else if (_currGuideId == guide_tag_22)
            {
                SGGeneralsLayer *nowLayer = (SGGeneralsLayer *)mainScene->getChildByTag(sg_generalsLayer);
                //nowLayer->openAdvanceView(0);
                nowLayer->itemselectByIndex(0);
            }
            else if (_currGuideId == guide_tag_35)
            {
                SGMainManager::shareMain()->showEmbattleLayer(0);
            }
            else if (_currGuideId == guide_tag_36 )
            {
                SGEmbattleLayer *nowLayer = (SGEmbattleLayer *)mainScene->getChildByTag(sg_embattleLayer);
                SGEmbattleItem* item = (SGEmbattleItem*) (nowLayer->cardArray)->objectAtIndex(1);
                nowLayer->selectedIndex(item);
            }
            else if (_currGuideId == guide_tag_40)
            {
                _canTouch = true;
                this->unschedule(schedule_selector(SGUIGuideLayer::addTimer));
                if (embattleItem && embattleItem->isCanMove)
                {
                    embattleItem->isCanMove= false;
                    
                    embattleItem->face->setPosition(embattleItem->firstPoint);
                    embattleItem->stopRock();
                    //
                    if (embattleItem->delegate && embattleItem->isSelect )
                    {
                        //		face->setColor(ccWHITE);
                        embattleItem->removeChildByTag(321);
                        if (m_EmbaTimer <2)
                        {
                            //embattleItem->delegate->selectedIndex(embattleItem); //没有点击事件
                            
                        }
                        
                        CCNode *node = embattleItem->getParent();
                        node->removeChildByTag(10000,true);//拿到加的那个临时蒙黑图
                        removeChildByTag(10000,true);//拿到加的那个临时蒙黑图
                        
                        if (embattleItem->namelabel) {
                            embattleItem->namelabel->setColor(ccWHITE);
                        }
                        
                        
                        embattleItem->isSelect = false;
                        embattleItem->delegate->setIsSelect(false);
                        if(embattleItem->delegate->trade())
                        {
                            spTouch->stop();
                            spTouch->removeFromParent();
                            spTouch = NULL;
                        }
                        else
                        {
                            CCRect rect = this->getRectByGuideId(_currGuideId);
                            float conH = spriteTag->getContentSize().height/2 + rect.size.height/2 + skewing(15);
                            
                            spTouch->stopAllActions();
                            spTouch->play();
                            spTouch->setVisible(true);
                            spTouch->setPosition(ccp(rect.origin.x, rect.origin.y+conH/2));
                            
                            popClick->stopAllActions();
                            popClick->setVisible(spTouch->isVisible());
                            popClick->setPosition(ccpAdd(spTouch->GetPosition(), ccp(popClick->getContentSize().width/2,10)));
                            CCAction *action = CCSequence::create(CCScaleTo::create(0.12f, 1.1f),
                                                                  CCScaleTo::create(0.12f, 1.2f),
                                                                  CCScaleTo::create(0.12f, 1.1f),
                                                                  CCScaleTo::create(0.12f, 1.0f),
                                                                  NULL);
                            popClick->runAction(CCRepeatForever::create((CCActionInterval*)action));
                            
                        }
                    }
                    else
                    {
                       // embattleItem->setPosition(embattleItem->firstPoint);
                    }
                    
                    m_EmbaTimer = 0;
                }
            }
            else if (_currGuideId == guide_tag_37)
            {
                SGArrangeLayer *nowLayer = (SGArrangeLayer *)mainScene->getChildByTag(sg_arrangeLayer);
                nowLayer->showEmbattleBoxByIndex(0);
            }
            else if (_currGuideId == guide_tag_47)//发送领取奖励
            {
                //引导记录。
                SGMainManager::sendOperatorRecord(207003);
                
                main::StoryRewardRequest *request = new main::StoryRewardRequest();
                request->set_storyid(1);// 只领取第一关的
                SGSocketClient::sharedSocketClient()->send(MSG_STORY_XINREWARD, request);
            }
        }
    }
}

void SGUIGuideLayer::showJoinBtn()
{
	//如果是在第五关引导，不让暂不参加出现
	if (_currGuideId == guide_tag_46)
	{
		//skipBtn->setVisible(false);
	}
    if (_currGuideId == guide_tag_45)
	{
		return;
	}
    //MMDEBUG: FIXME 有可能到btn_guide_join.png时已经被释放了，非常不科学，暂时即用即添。
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    SGButton *joinBtn = SGButton::create("public_btn_2.png", "btn_guide_join.png",
                                        this, menu_selector(SGUIGuideLayer::buttonAction), ccp(0, 0), false, true);
    float posX = joinBtn->getContentSize().width*0.75f;
    float posY = bboard->getPositionY() - bboard->getContentSize().height/2 + joinBtn->getContentSize().height*0.75f;
    
    joinBtn->setTag(1);
    joinBtn->setScale(0.75f);
    joinBtn->setPosition(ccp(bboard->getPositionX()-posX, posY));
    this->addBtn(joinBtn);
    
    SGButton *rejectBtn = SGButton::create("public_kuang_hui.png", "btn_guide_nojoin.png",
                                           this, menu_selector(SGUIGuideLayer::buttonAction), ccp(0, 0), false, true);
    rejectBtn->setTag(2);
    rejectBtn->setScale(0.75f);
    rejectBtn->setPosition(ccp(bboard->getPositionX()+posX - 70, posY));
    this->addBtn(rejectBtn);
}

void SGUIGuideLayer::buttonAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    CCMenuItem *menu = (CCMenuItem *)sender;
    DDLog("cc--------------%d", menu->getTag());
    
	if (_currGuideId == guide_tag_45)
	{
		switch (menu->getTag()) {
			case 1: { // join
				SGGuideManager::shareManager()->dismissLayer();
				SGMainManager::shareMain()->getMainLayer()->requestStory();
				CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_45);
			}break;
			case 2: { // not join
				SGGuideManager::shareManager()->dismissLayer();
				CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_45); //
				CCUserDefault::sharedUserDefault()->flush();
			}break;
			default:
				break;
		}
	}
	else if (_currGuideId == guide_tag_46)//在第五关出现第一次引导的时候
	{
		switch (menu->getTag())
        {
			case 1:
            { // join
                main::PlotListRequest *request = new main::PlotListRequest();
                request->set_storyid(5);
                request->set_poltaskid(1);
                SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);

				CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_46);
                
                //点击按钮的同时，播放一下条解说
                if (_currGuideId == guide_tag_46)
                {
                    int currId =  _model->getModelId();
                    SGGuideManager::shareManager()->checkIsDone(currId+1);
                }
			}
                break;
			case 2:
            { // not join
				SGGuideManager::shareManager()->dismissLayer();
				CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_99); //
				CCUserDefault::sharedUserDefault()->flush();
			}
                break;
			default:
				break;
		}
	}

    this->menu->removeChildByTag(1, true);
    this->menu->removeChildByTag(2, true);
}

void SGUIGuideLayer::resetTaskTouch()
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGAllTaskLayer *taskLayer = (SGAllTaskLayer *)mainScene->getChildByTag(sg_taskLayer);
    if (taskLayer != NULL)
    {
        taskLayer->tableView->setTouchEnabled(true);
    }
}
