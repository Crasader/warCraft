//
//  SGChatInputBox.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#include "SGChatInputBox.h"
#include "SGNikeNameBox.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGChatInputBox::SGChatInputBox()
{
    m_pTextField = NULL;
//    _delegate = NULL;
//    menu = NULL;
//    buttonSend = NULL;
//    buttonCancel = NULL;
    
}

SGChatInputBox::~SGChatInputBox()
{
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_chatinputbox);

     SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, TEXTFIELDHIDE);
}

SGChatInputBox* SGChatInputBox::create(SGBoxDelegate *delegate)
{
    SGChatInputBox *input = new SGChatInputBox();

    if (input && input->init(delegate, box_input, CCSize(535, 324)))
    {
        input->initview();
        input->autorelease();
        return input;
    }
    CC_SAFE_DELETE(input);
    return NULL;
}

void SGChatInputBox::initview()
{
 
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_chatinputbox);
  
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(TEXTFIELDHIDE, this, callfuncO_selector(SGChatInputBox::buttonClickSend));
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("chat_input_title.png");
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_ChatInputBox_str1, FONT_PANGWA, 40 ,ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    this->addChild(title);
    title->setPosition(biao->getPosition());
//    //背景
//    CCScale9Sprite *scale_sprite_background = CCScale9Sprite::create("public/public_1.png");
//    CCScale9Sprite *scale_sprite_rect = CCScale9Sprite::create("public/public_jiao1.png");
//    scale_sprite_background->setPreferredSize(CCSizeMake(400, 320));
//    scale_sprite_rect->setPreferredSize(CCSizeMake(400, 320));
//    scale_sprite_background->setPosition(ccpAdd(center, ccp(1, 160)));
//    scale_sprite_rect->setPosition(ccpAdd(center, ccp(1, 160)));
//    addChild(scale_sprite_background);
//    addChild(scale_sprite_rect);
    
//    //背景，白色输入框
//    CCScale9Sprite *m_pInputScale = CCScale9Sprite::create("sanguobigpic/tipsinside.png");
//    m_pInputScale->setPreferredSize(CCSizeMake(360, 160));
//    m_pInputScale->setPosition(ccpAdd(center, ccp(0, 200)));
//    addChild(m_pInputScale);
    
    
   
    //输入框
    m_pTextField = SGCCTextFieldTTF::createWithPlaceHolder(ccpAdd(center, ccp(0, -20)),str_ChatInputBox_str2, CCSizeMake(380,180),kCCTextAlignmentLeft,FONT_BOXINFO, 28);
    
//    m_pTextField=CCEditBox::create(CCSizeMake(360,160), m_pInputScale);
 
    m_pTextField->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    m_pTextField->setFontFillColor(ccBLACK);
    
//    m_pTextField->setColorSpaceHolder(ccRED);
    m_pTextField->setLength(50);
    addChild(m_pTextField,50);
    
//    //发送消息按钮
//    menu = CCMenu::create();
//    menu->setPosition(CCPointZero);
    //确定
    
    SGButton *buttonSend = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGChatInputBox::buttonClickSend),CCPointZero,false,true);
    SGButton *buttonCancel = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, menu_selector(SGChatInputBox::buttonClickCancel),CCPointZero,false,true);
    
    this->addBtn(buttonSend);
    this->addBtn(buttonCancel);
    buttonSend->setPosition(ccpAdd(center, ccp(-145, -324/2+50)));
    buttonCancel->setPosition(ccpAdd(center, ccp(145, -324/2+50)));

    
//    buttonSend = SGButton::create("public/public_btn_5.png", "public/public_font_queding.png", this, menu_selector(SGChatInputBox::buttonClickSend),CCPointZero,false,false);

//    menu->addChild(buttonSend);

    //取消
//    buttonCancel = SGButton::create("public/public_btn_4.png", "public/public_font_qixiao.png", this, menu_selector(SGChatInputBox::buttonClickCancel),CCPointZero,false,false);
//    menu->addChild(buttonCancel);
//    addChild(menu);

    
}

#pragma mark - ----- 按钮点击 -----

void SGChatInputBox::sendMsgConfirm()
{
    char buffer[1024] = {};
    strcpy(buffer, m_pTextField->getString());

    //if(SGNikeNameBox::utf8StrLength(buffer)==0)
    int length = getUtf8Length(buffer);
    if ( length == 0 )
    {
        SG_SHOW_WINDOW(str_ChatInputBox_str4);
        return;
    }
    //if( SGNikeNameBox::utf8StrLength(buffer) > 50 )
    if (50 < length)
    {
        SG_SHOW_WINDOW(str_ChatInputBox_str3);
        return;
    }
    

    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("inputBox"), "key");
    dict->setObject(CCString::create(SGNikeNameBox::Trim(m_pTextField->getString())), "string");
    data = dict;
    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer))->setIsCanTouch(true);
    
    this->boxClose();

}

void SGChatInputBox::sendMsgCancel()
{
    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer))->setIsCanTouch(true);
    this->boxCloseWithOutCallBack();
}


void SGChatInputBox::buttonClickSend()
{
    EFFECT_PLAY(MUSIC_BTN);
    char buffer[1024] = {};
    strcpy(buffer, m_pTextField->getString());
    int length = getUtf8Length(buffer);
    if ( length == 0 )
    {
        SG_SHOW_WINDOW(str_ChatInputBox_str4);
        return;
    }
    if (50 < length)
    {
        SG_SHOW_WINDOW(str_ChatInputBox_str3);
        return;
    }

//    if(SGNikeNameBox::utf8StrLength(buffer)==0 || SGNikeNameBox::utf8StrLength(buffer)>50)
//    {
//        SG_SHOW_WINDOW(str_ChatInputBox_str3);
//        return;
//    }
    m_pTextField->detachWithIME();

    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("inputBox"), "key");
    dict->setObject(CCString::create(SGNikeNameBox::Trim(m_pTextField->getString())), "string");
    data = dict;
    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer))->setIsCanTouch(true);
 
    this->boxClose();
//    if(_delegate)
//    {
//        _delegate->setIsCanTouch(true);
//        _delegate->showBoxCall(dict);
//    }
    
//    this->removeFromParentAndCleanup(true);
}

void SGChatInputBox::buttonClickCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
    m_pTextField->detachWithIME();

    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer))->setIsCanTouch(true);
    this->boxCloseWithOutCallBack();
//    if(_delegate)
//    {
//        _delegate->setIsCanTouch(true);
//    }
//    
//    this->removeFromParentAndCleanup(true);
}

void SGChatInputBox::textFiledDidhide(CCObject *obj)
{
    CCLOG("SGChatInputBox:: ChatInputBox did hide");
    buttonClickSend();
}

#pragma mark - ----- targetDelegate -----
//void SGChatInputBox::onEnter()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
//}
//
//void SGChatInputBox::onExit()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//}
//
//bool SGChatInputBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//    CCPoint touchPos = pTouch->getLocation();
//    if(m_pTextField->boundingBox().containsPoint(touchPos))
//    {
//        m_pTextField->ccTouchBegan(pTouch, pEvent);
//        return true;
//    }
//    
//    if(buttonSend->boundingBox().containsPoint(touchPos))
//    {
//        buttonSend->selected();
//        return true;
//    }
//    
//    if(buttonCancel->boundingBox().containsPoint(touchPos))
//    {
//        buttonCancel->selected();
//        return true;
//    }
//    
//    return true;
//}
//
//void SGChatInputBox::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//    CCPoint touchPos = pTouch->getLocation();
//    if(buttonSend->boundingBox().containsPoint(touchPos))
//    {
//        if(buttonSend->isSelected())
//        {
//            buttonSend->unselected();
//            buttonSend->activate();
//        }
//    }
//    
//    if(buttonCancel->boundingBox().containsPoint(touchPos))
//    {
//        if(buttonCancel->isSelected())
//        {
//            buttonCancel->unselected();
//            buttonCancel->activate();
//        }
//    }
//}

