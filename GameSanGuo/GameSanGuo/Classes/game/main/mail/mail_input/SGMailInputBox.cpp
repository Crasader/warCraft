//
//  SGMailInput.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-15.
//
//

#include "SGMailInputBox.h"
#include "SGNikeNameBox.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGMailInputBox::SGMailInputBox()
{
    m_pTextField = NULL;
    _delegate = NULL;
    menu = NULL;
    buttonSend = NULL;
    buttonCancel = NULL;
}

SGMailInputBox::~SGMailInputBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_mailLayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, TEXTFIELDHIDE);
}

SGMailInputBox* SGMailInputBox::create(SGBoxDelegate *delegate)
{
    SGMailInputBox *input = new SGMailInputBox();
    if(input && input->init())
    {
        input->_delegate = delegate;
        input->viewDidLoad();
        input->autorelease();
        return input;
    }
    
    CC_SAFE_DELETE(input);
    return NULL;
}

void SGMailInputBox::viewDidLoad()
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_mailLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_mailLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_mailLayer);

	
    SGNotificationCenter::sharedNotificationCenter()->addObserver(TEXTFIELDHIDE, this, callfuncO_selector(SGMailInputBox::buttonClickSend));
    //背景
    CCScale9Sprite *scale_sprite_background = CCScale9Sprite::createWithSpriteFrameName("Tips.png");
    CCScale9Sprite *scale_sprite_rect = CCScale9Sprite::createWithSpriteFrameName("box_kuang.png");
//    scale_sprite_background->setPreferredSize(CCSizeMake(400, 320));
    scale_sprite_background->setScaleY(320/scale_sprite_background->getContentSize().height);
    scale_sprite_background->setScaleX(400/scale_sprite_background->getContentSize().width);
    scale_sprite_rect->setPreferredSize(CCSizeMake(400, 320));
    scale_sprite_background->setPosition(ccpAdd(center, ccp(1, 160)));
    scale_sprite_rect->setPosition(ccpAdd(center, ccp(1, 160)));
    addChild(scale_sprite_background);
    addChild(scale_sprite_rect);
    //背景，白色输入框
    
    CCScale9Sprite *m_pInputScale = CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    m_pInputScale->setPreferredSize(CCSizeMake(380, 180));
    m_pInputScale->setPosition(ccpAdd(center, ccp(0, 200)));
    addChild(m_pInputScale);
    //输入框
    m_pTextField = SGCCTextFieldTTF::createWithPlaceHolder(ccpAdd(center, ccp(0, 200)),str_FriendMailLayer_str5, CCSizeMake(360,160),kCCTextAlignmentLeft,FONT_BOXINFO, 24);
    m_pTextField->setKType(1);
    m_pTextField->setColor(ccBLACK);
    m_pTextField->setLength(50);
    m_pTextField->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    addChild(m_pTextField);
    
    //发送消息按钮
    menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    
    //确定
    buttonSend = SGButton::create("public_btn_5.png", "public_font_queding.png", this, menu_selector(SGMailInputBox::buttonClickSend),CCPointZero,false,true);
    buttonSend->setPosition(ccpAdd(center, ccp(-100, 50)));
    menu->addChild(buttonSend);
    
    //取消
    buttonCancel = SGButton::create("public_btn_4.png", "public_font_qixiao.png", this, menu_selector(SGMailInputBox::buttonClickCancel),CCPointZero,false,true);
    buttonCancel->setPosition(ccpAdd(center, ccp(100, 50)));
    menu->addChild(buttonCancel);
    addChild(menu);
    
}

#pragma mark - ----- 按钮点击 -----

void SGMailInputBox::buttonClickSend()
{
    EFFECT_PLAY(MUSIC_BTN);
    char buffer[1024] = {};
    strcpy(buffer, m_pTextField->getString());
    if(SGNikeNameBox::utf8StrLength(buffer)<1)//发言字数限制
    {
        SG_SHOW_WINDOW(str_FriendMailLayer_str6);
        return;
    }
    m_pTextField->detachWithIME();
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("inputBox"), "key");
    CCLOG("===%s",m_pTextField->getString());
    dict->setObject(CCString::create(SGNikeNameBox::Trim(m_pTextField->getString())), "string");
    
   
    
    CCLog("发送文字：%s",m_pTextField->getString());
    if(_delegate)
    {
        _delegate->setIsCanTouch(true);
        _delegate->showBoxCall(dict);
    }
    
    this->removeFromParentAndCleanup(true);
}

void SGMailInputBox::buttonClickCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
    if(_delegate)
    {
        _delegate->setIsCanTouch(true);
    }
    m_pTextField->detachWithIME();
    this->removeFromParentAndCleanup(true);
}

#pragma mark - ----- targetDelegate -----
void SGMailInputBox::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX-1, true);
}

void SGMailInputBox::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool SGMailInputBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //判断键盘是否应该收回
//    bool bRet = m_pTextField->detachWithIME();
//    if (bRet)
//    {
//        CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
//        if (pGlView)
//        {
//            pGlView->setIMEKeyboardState(false);
//        }
//    }
    
    //判断按钮的点击事件
    CCPoint touchPos = pTouch->getLocation();
    if(m_pTextField->boundingBox().containsPoint(touchPos))
    {
        m_pTextField->ccTouchBegan(pTouch, pEvent);
        return true;
    }
    
    if(buttonSend->boundingBox().containsPoint(touchPos))
    {
        buttonSend->selected();
        return true;
    }
    
    if(buttonCancel->boundingBox().containsPoint(touchPos))
    {
        buttonCancel->selected();
        return true;
    }
    
    return true;
}

void SGMailInputBox::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPos = pTouch->getLocation();
    if(buttonSend->boundingBox().containsPoint(touchPos))
    {
        if(buttonSend->isSelected())
        {
            buttonSend->unselected();
            buttonSend->activate();
        }
    }
    
    if(buttonCancel->boundingBox().containsPoint(touchPos))
    {
        if(buttonCancel->isSelected())
        {
            buttonCancel->unselected();
            buttonCancel->activate();
        }
    }
}
