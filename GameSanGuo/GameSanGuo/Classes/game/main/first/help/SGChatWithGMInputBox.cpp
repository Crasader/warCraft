//
//  SGChatWithGMInputBox.cpp
//  GameSanGuo
//
//  Created by kanata on 14-1-9.
//
//

#include "SGChatWithGMInputBox.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "ResourceManager.h"
#include "SGNikeNameBox.h"
#include "SGChatWithGM.h"
#include "GMQAModule.pb.h"
#include "SGStringConfig.h"

SGChatWithGMInputBox::SGChatWithGMInputBox()
:checkid(0),
array(NULL)
{
    m_pTextField = NULL;
    array=NULL;
    
}

SGChatWithGMInputBox::~SGChatWithGMInputBox()
{
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_chaatwithgminputbox);
    
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_APPENDQUESTION);
    //SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHECKQA);
}

SGChatWithGMInputBox* SGChatWithGMInputBox::create(SGBoxDelegate *delegate,int idid,CCArray*arr)
{
    SGChatWithGMInputBox *input = new SGChatWithGMInputBox();
    
    if (input && input->init(NULL, box_input, CCSize(535, 324)))
    {
        input->checkid=idid;
        input->array=arr;
        input->initview();
        input->autorelease();
        return input;
    }
    CC_SAFE_DELETE(input);
    return NULL;
}

void SGChatWithGMInputBox::initview()
{
    
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_chatinputbox);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_APPENDQUESTION, this, callfuncO_selector(SGChatWithGMInputBox::sendconfirm));
 
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("chat_input_title.png");
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_ChatInputBox_str1, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    this->addChild(title);
    title->setPosition(biao->getPosition());

    //输入框
    m_pTextField = SGCCTextFieldTTF::createWithPlaceHolder(ccpAdd(center, ccp(0, -20)),str_ChatWithGMInputBox_str1, CCSizeMake(360,160),kCCTextAlignmentLeft,FONT_BOXINFO, 28);
    m_pTextField->setFontFillColor(ccBLACK);
    m_pTextField->setLength(50);
    m_pTextField->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    addChild(m_pTextField,50);
    
    //确定
    
    SGButton *buttonSend = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGChatWithGMInputBox::buttonClickSend),CCPointZero,false,true);
    SGButton *buttonCancel = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, menu_selector(SGChatWithGMInputBox::buttonClickCancel),CCPointZero,false,true);
    
    this->addBtn(buttonSend);
    this->addBtn(buttonCancel);
    buttonSend->setPosition(ccpAdd(center, ccp(-145, -324/2+50)));
    buttonCancel->setPosition(ccpAdd(center, ccp(145, -324/2+50)));
}

void SGChatWithGMInputBox::buttonClickSend()
{
    EFFECT_PLAY(MUSIC_BTN);
    ////////////////////////20秒判定
    long nowtime=getCurrentTime();
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    long oldtime=player->getwithgmoldtime();
    long distance=nowtime-oldtime;
    
    //2014.6.23  不要限制
//    if(distance<20)
//    {
//    
//        SG_SHOW_WINDOW(str_ChatWithGMInputBox_str2);
//        m_pTextField->detachWithIME();
//        SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
//        layer->setIsCanTouch(true);
//        layer->setPortraitMenuCanTouch(false);
//        
//        
//        ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatwithgm))->setIsCanTouch(true);
//        this->boxClose();
//
//        return;
//    }
    

    /////////////////////////
    
    char buffer[1024] = {};
    strcpy(buffer, m_pTextField->getString());
    
//    char bufferStr[1024];
//    const char *buffer = m_pTextField->getString();
//    strcpy(bufferStr, buffer);
    
    if(SGNikeNameBox::utf8StrLength(buffer)==0 || SGNikeNameBox::utf8StrLength(buffer)>100)
    {
        SG_SHOW_WINDOW(str_ChatWithGMInputBox_str3);
        return;
    }
    
    if (GameConfig::isContainsEmoji(buffer)) {
        SG_SHOW_WINDOW(str_NikeNameBox_str3);
        return;
    }

    
    for(int i=0;i<sizeof(buffer)-1;i++){
        int number = buffer[i];
       // CCLOG("%d",number);
        
    }
    m_pTextField->detachWithIME();
    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatwithgm))->setIsCanTouch(true);
    ///////send  message
    main::AppendQuestionRequest*request=new main::AppendQuestionRequest();
    request->set_id(checkid);
    request->set_content(buffer);
    
    SGSocketClient::sharedSocketClient()->send(MSG_APPENDQUESTION, request);

}
void SGChatWithGMInputBox::sendconfirm(CCObject*obj) ///确认有没有发送成功
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::AppendQuestionResponse *response = (main::AppendQuestionResponse *)sr->m_msg;
    int i= response->state();
    if(i)
    {
        
        
//        char buffer[1024] = {};
//        strcpy(buffer, m_pTextField->getString());
//        CCString* ns=CCString::createWithFormat("%s",buffer);
//        
//        QADetail*detail=new QADetail();
//        detail->setcontent(ns);
//        detail->setgmid(0);
//        CCString*temp=CCString::create("刚刚    ");
//        detail->settime(temp);
//        array->addObject(detail);
// ////////////////////////////////
//        SGMainLayer *lay = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
//        lay->setIsCanTouch(true);
//        SGButton*btn= (SGButton*)  lay->getBtnByTag(54321);
//        btn->setEnabled(false);
//        
//    
//        
//        lay->setPortraitMenuCanTouch(false);
//        SGChatWithGM* layer = (SGChatWithGM*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatwithgm);
//       // layer->setIsCanTouch(true);
//        layer->send(array);
//        
      
        
//////////////////////////////////
        SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CHECKQA, this, callfuncO_selector(SGChatWithGMInputBox::checkqaresponse));
        callback();
        this->boxClose();
        
        SG_SHOW_WINDOW(str_ChatWithGMInputBox_str4);
        //////////////////保存这次时间  下次在发言留作判断
        long nowtime=getCurrentTime();
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
        player->setwithgmoldtime(nowtime);
        
        /////////////////
    }
    else
    {
        this->boxClose();
        SG_SHOW_WINDOW(str_ChatWithGMInputBox_str5);
    }

}





void SGChatWithGMInputBox::buttonClickCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
    m_pTextField->detachWithIME();
    
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
    layer->setPortraitMenuCanTouch(false);
    
    
    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatwithgm))->setIsCanTouch(true);
    this->boxClose();

}


/////////////
void SGChatWithGMInputBox::callback()
{
    int tagtag=checkid;
    ///存储id
    
    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatwithgm))->setIsCanTouch(true);
    /////////////////////////////////////////////////////本地刷新客服列表查看状态
    
//    CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
//    int count=0;
//    for(int i=0;i<temp->count();i++)
//    {
//        SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
//        
//        if(qa->getID()==tagtag)
//        {
//            qa->setState(1); ////手动设置查看状态  貌似1是已查看
//        }
//    }
//    
//    
//    
//    
//    
//    
//    
//    ///////////////////////////////////////////////////////
//    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
//    
//    playInfo->setcheckid(tagtag);
//    
    
    
    //////////////////////////////////////////////////////发给服务器刷新列表
    main::CheckQARequest*request=new main::CheckQARequest();
    request->set_id(tagtag);
    
    SGSocketClient::sharedSocketClient()->send(MSG_CHECKQA, request);

    
}

void SGChatWithGMInputBox::checkqaresponse(CCObject*obj)
{
    //////
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHECKQA);
    
    
    ///////
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::CheckQAResponse *response = (main::CheckQAResponse *)sr->m_msg;
    
    CCArray *array=CCArray::create();
    for(int i=0;i< response->answer_size();i++)
    {
        main::AnswerProto proto= response->answer(i);
        //////////////////////////
        
        QADetail *detail=new QADetail();
        detail->setgmid(proto.gmid());
        
        std::string str2(proto.time());
        CCString* nn=CCString::createWithFormat("%s",str2.c_str());
        detail->settime(nn);
        
        std::string str(proto.content());
        
        CCString* ns=CCString::createWithFormat("%s",str.c_str());
        
        detail->setcontent(ns);
        
        array->addObject(detail);
        
        
        
        
    }
            SGMainLayer *lay = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
            lay->setIsCanTouch(true);
            SGButton*btn= (SGButton*)  lay->getBtnByTag(54321);
            btn->setEnabled(false);
            lay->setPortraitMenuCanTouch(false);
    
    
    
           SGChatWithGM* layer = (SGChatWithGM*) SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatwithgm);
           // layer->setIsCanTouch(true);
            layer->send(array);
    
    
}







long SGChatWithGMInputBox::getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec;
}

