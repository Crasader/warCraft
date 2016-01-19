 //
//  SGLiuyanLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-1-6.
//
//

#include "SGLiuyanLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGCCTextFieldTTF.h"
#include "SGChatInputBox.h"
#include "SGComplainLayer.h"
#include "CCTextFieldTTF.h"
#include "SGNikeNameBox.h"
#include "SGStringConfig.h"





SGLiuyanLayer::SGLiuyanLayer()
:quexian(NULL),
jianyi(NULL),
jubao (NULL),
chongzhi(NULL),
m_pEditTitle(NULL),
m_pTextField(NULL),
m_pTextFieldqq(NULL),
now_tag(0),
label1(NULL),
label2(NULL),
label3(NULL),
label4(NULL),
ttf(NULL)
{
    now_tag=0;
  
    
}
SGLiuyanLayer::~SGLiuyanLayer()
{
    
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_liuyan);
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->getChildByTag(10000)->setVisible(true);
    
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->removeObserver(this, MSG_SUBQA);
    
}
SGLiuyanLayer *SGLiuyanLayer::create()
{
    SGLiuyanLayer *Layer = new SGLiuyanLayer();
    if (Layer && Layer->init(NULL, sg_liuyan))
    {
        Layer->initView();
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGLiuyanLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_liuyan, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgfindhelplayer/sgfindhelplayer.plist", RES_TYPE_LAYER_UI, sg_liuyan);
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_liuyan);
    
    ////////////////////
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_SUBQA, this, callfuncO_selector(SGLiuyanLayer::sbumitsuccess));
    
    
    
    //////////////////
    
      SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->getChildByTag(10000)->setVisible(false);
    
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float discY = s.height-headH-bottomH+65;
    //////手动加一个bg
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_liuyan);

    CCSprite *background2 = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    background2->setScaleX(s.width/background2->getContentSize().width * 1.01);
    background2->setAnchorPoint(ccp(0.5, 0.5));
    background2->setPosition(ccpAdd(center, ccp(0, 0)));
    this->addChild(background2,-10);
    
    
    ////方形大bg
  
   // CCSprite *infoBg = CCSprite::createWithSpriteFrameName("fight_over_bg_n.png");
    CCScale9Sprite *infoBg = CCScale9Sprite::createWithSpriteFrameName("fight_over_bg_n.png");
    infoBg->setPreferredSize(CCSizeMake(skewing(300) + 10, discY*0.85));
    //infoBg->setPosition(center);
    infoBg->setPosition(ccpAdd(center, ccp( 7 , -infoBg->getContentSize().height*0.1)));
    CCSize sizefd=   infoBg->getContentSize();
    
    
//    infoBg->setPosition(ccp(s.width*.5f, bottomH + discY*.49f-20));
    this->addChild(infoBg);
    //线
    CCSprite *line = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
    line->setPosition(ccpAdd(infoBg->getPosition(), ccp(infoBg->getContentSize().width*0.0, infoBg->getContentSize().height*0.37)));
    this->addChild(line);
    
    
    
    

    
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
    
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGLiuyanLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));
    
    
    
    
    //    CCSprite *title = CCSprite::createWithSpriteFrameName("help_font_kptj.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_LiuyanLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);
    ////提交按钮
    SGButton *tijiao = SGButton::createFromLocal("box_btnbg.png", str_LiuyanLayer_str2, this,menu_selector(SGLiuyanLayer::submit),CCPointZero,FONT_BOXINFO,ccWHITE,28);
    tijiao->setPosition(ccpAdd(infoBg->getPosition(), ccp(0,  -infoBg->getContentSize().height*0.42)));
    this->addBtn(tijiao);
    ////////////////////////////////////////
    quexian = SGButton::create("anDian.png",
                                        "anDian.png",
                                        this,
                                        menu_selector(SGLiuyanLayer::setWhiteAndDark),
                                        ccp(0, 0),
                                        false,
                                        true);
    quexian->setPosition(ccpAdd(ccp(0,line->getPositionY()), ccp(s.width*0.15, backBtn->getContentSize().height*0.6)));
    this->addBtn(quexian);
    quexian->setTag(100);
    
    
    
    
    ///缺陷本体字
    label1=SGCCLabelTTF::create(str_LiuyanLayer_str3, FONT_BOXINFO, 30);
    label1->setAnchorPoint(ccp(0, 0.5));
    label1->setPosition(ccpAdd(quexian->getPosition(), ccp(quexian->getContentSize().width*0.3, 0)));
    this->addChild(label1);
  /////////建议
    jianyi = SGButton::create("anDian.png",
                               "anDian.png",
                               this,
                               menu_selector(SGLiuyanLayer::setWhiteAndDark),
                               ccp(0, 0),
                               false,
                               true);
    jianyi->setPosition(ccpAdd(quexian->getPosition(), ccp(s.width*0.2,0)));
    this->addBtn(jianyi);
    jianyi->setTag(101);
    
    ///建议本体字
    label2=SGCCLabelTTF::create(str_LiuyanLayer_str4, FONT_BOXINFO, 30);
    label2->setPosition(ccpAdd(jianyi->getPosition(), ccp(jianyi->getContentSize().width*0.3, 0)));
    label2->setAnchorPoint(ccp(0, 0.5));
    this->addChild(label2);
    //////////////举报
    jubao = SGButton::create("anDian.png",
                               "anDian.png",
                               this,
                               menu_selector(SGLiuyanLayer::setWhiteAndDark),
                               ccp(0, 0),
                               false,
                               true);
    
   jubao->setPosition(ccpAdd(jianyi->getPosition(), ccp(s.width*0.2,0)));
    this->addBtn(jubao);
    jubao->setTag(102);
    
    ///举报本体字
    label3=SGCCLabelTTF::create(str_LiuyanLayer_str5, FONT_BOXINFO, 30);
    label3->setPosition(ccpAdd(jubao->getPosition(), ccp(jubao->getContentSize().width*0.3, 0)));
    label3->setAnchorPoint(ccp(0, 0.5));
    this->addChild(label3);
    /////////////储值
    chongzhi = SGButton::create("anDian.png",
                               "anDian.png",
                               this,
                               menu_selector(SGLiuyanLayer::setWhiteAndDark),
                               ccp(0, 0),
                               false,
                               true);
    chongzhi->setPosition(ccpAdd(jubao->getPosition(), ccp(s.width*0.2, 0)));
    this->addBtn(chongzhi);
    chongzhi->setTag(103);
    
    
    
    ///储值本体字
    label4=SGCCLabelTTF::create(str_LiuyanLayer_str6, FONT_BOXINFO, 30);
    label4->setPosition(ccpAdd(chongzhi->getPosition(), ccp(chongzhi->getContentSize().width*0.3, 0)));
    label4->setAnchorPoint(ccp(0, 0.5));
    this->addChild(label4);

  ////输入框前的字
    SGCCLabelTTF*labelkuang_1=SGCCLabelTTF::create(str_LiuyanLayer_str7, FONT_BOXINFO, 30);
    labelkuang_1->setAnchorPoint(ccp(1, 0.5));
    labelkuang_1->setPosition(ccpAdd(quexian->getPosition(), ccp(quexian->getContentSize().width*0.8, -quexian->getContentSize().height*0.9)));
    this->addChild(labelkuang_1);
    
    
    SGCCLabelTTF*labelkuang_2=SGCCLabelTTF::create(str_LiuyanLayer_str8, FONT_BOXINFO, 30);
    labelkuang_2->setAnchorPoint(ccp(0, 0.5));
    labelkuang_2->setPosition(ccpAdd(labelkuang_1->getPosition(), ccp(-labelkuang_1->getContentSize().width, -quexian->getContentSize().height*0.9+5)));
    this->addChild(labelkuang_2);

    
    
    /////详细描述输入框
    CCScale9Sprite *m_detailinput = CCScale9Sprite::create("sanguobigpic/tipsinside.png");
#if (PLATFORM == IOS)
    m_detailinput->setPreferredSize(CCSizeMake(s.width-20, discY*0.5));
    m_detailinput->setPosition(ccpAdd(ccp(s.width/2 +30, 0), ccp(0, tijiao->getPosition().y+m_detailinput->getContentSize().height*0.57)));//0.77)));
#else
    m_detailinput->setPreferredSize(CCSizeMake(s.width -20, discY*0.5));
    m_detailinput->setPosition(ccpAdd(ccp(s.width/2 + 30, 0), ccp(0, tijiao->getPosition().y+m_detailinput->getContentSize().height*0.57)));
#endif
    addChild(m_detailinput);
    
    m_pTextFieldqq = MYtextfieldttf::createWithPlaceHolder(ccpAdd(center, ccp(0, -200)),str_LiuyanLayer_str9, CCSizeMake(s.width*0.9, discY*0.4),kCCTextAlignmentLeft,FONT_BOXINFO, 35);
    m_pTextFieldqq->setKType(1);
    m_pTextFieldqq->setColor(ccBLACK);
    m_pTextFieldqq->setLength(100);
    m_pTextFieldqq->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    m_pTextFieldqq->setPosition(ccpAdd(m_detailinput->getPosition() , ccp(-12 , 0) ));
    this->addChild(m_pTextFieldqq);
    
    
    ///字数限制
    SGCCLabelTTF*labelxianzhi=SGCCLabelTTF::create(str_LiuyanLayer_str10, FONT_BOXINFO, 30 , ccc3(0xc9, 0xa1, 0x5f));
    labelxianzhi->setAnchorPoint(ccp(0, 0));
    //labelxianzhi->setPosition(ccpAdd(m_detailinput->getPosition(), ccp(m_detailinput->getContentSize().width*0.5 - labelxianzhi->getContentSize().width/2 + 22, -m_detailinput->getContentSize().height*0.5)));
    labelxianzhi->setPosition(ccp(tijiao->getPositionX() + tijiao->getContentSize().width*0.5 + 20, tijiao->getPositionY() ) );
    this->addChild(labelxianzhi);
    
    
    
    
    
    
   // CCTextFieldTTF * CCTextFieldTTF::textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize)

//    ttf=CCTextFieldTTF::textFieldWithPlaceHolder("啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊",CCSizeMake(s.width*0.9, discY*0.4),kCCTextAlignmentLeft,FONT_BOXINFO, 35);
//    ttf->setColor(ccRED);
//    ttf->setPosition(m_detailinput->getPosition());
//    this->addChild(ttf);
    

    
    //标题输入框
    
    m_pEditTitle = CCEditBox::create(CCSizeMake(s.width*0.7, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditTitle->setPlaceHolder(str_LiuyanLayer_str11);
    m_pEditTitle->setMaxLength(14);
    m_pEditTitle->setReturnType(kKeyboardReturnTypeDone);
    m_pEditTitle->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditTitle->setReturnType(kKeyboardReturnTypeDone);
    //    m_pEditTitle->setDelegate(this);
    this->addChild(m_pEditTitle,9);
    m_pEditTitle->setAnchorPoint(ccp(0, 0.5));
    m_pEditTitle->setPosition(ccpAdd(labelkuang_1->getPosition(), ccp(10, 0)));
    m_pEditTitle->setFontColor(ccRED);
    
    
   
}




void SGLiuyanLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    
    SGComplainLayer *layer = SGComplainLayer::create(playerInfo->getQAContent());
     SGMainManager::shareMain()->showLayer(layer);
 //   m_pTextField->detachWithIME();
    
 //   SGMainManager::shareMain()->showcomplainlayer();

}


void SGLiuyanLayer::setWhiteAndDark(CCNode*node)///////四个按钮 tag是100 101 102 103
{
    
    now_tag=node->getTag();
//    SGButton*btn= (SGButton*) getBtnByTag(now_tag);

    changestate(now_tag);
}
///////
void SGLiuyanLayer::setWhiteAndDarktouch(int btntype)///////cctouch
{
    
    now_tag=btntype;
    //    SGButton*btn= (SGButton*) getBtnByTag(now_tag);
    
    changestate(now_tag);
}


///////
void SGLiuyanLayer::changestate(int tag)
{
     SGButton*btn100= (SGButton*) getBtnByTag(100);
     SGButton*btn101= (SGButton*) getBtnByTag(101);
     SGButton*btn102= (SGButton*) getBtnByTag(102);
     SGButton*btn103= (SGButton*) getBtnByTag(103);
    
    SGButton*temp=(SGButton*) getBtnByTag(tag);
      temp->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("liangDian.png")->getCString())->displayFrame());
    
    
    if(tag==100)
    {
        
         btn101->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
         btn102->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
         btn103->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
    
        
    }
    if(tag==101)
    {
        btn100->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
        btn102->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
        btn103->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
    }
    if(tag==102)
    {
        btn101->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
        btn100->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
        btn103->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
    }
    if(tag==103)
    {
        btn101->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
        btn102->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
        btn100->setFontImage(CCSprite::createWithSpriteFrameName(CCString::create("anDian.png")->getCString())->displayFrame());
    }
    
    
}

void SGLiuyanLayer::submit() /////提交的内容
{
   long nowtime=getCurrentTime();
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
   long  oldtime=player->getoldtime();
    
    
    long distance=nowtime-oldtime;
    if(distance<120)
    {
    
        SG_SHOW_WINDOW(str_LiuyanLayer_str12);
        return;
    
    }
    
    
    
    
    
    
    
    
//     const char *str=m_pEditTitle->getText();
//     const char *str2=  m_pTextFieldqq->getString();
    
    CCLOG("%s88888888888888%s",m_pEditTitle->getText(),m_pTextFieldqq->getString());
    if(now_tag==0)
    {

        
        SG_SHOW_WINDOW(str_LiuyanLayer_str13);
        return;
 
        
    }
    
    char buffer[1024] = {};
    strcpy(buffer, m_pEditTitle->getText());
    
    if (GameConfig::isContainsEmoji(buffer)) {
        SG_SHOW_WINDOW(str_NikeNameBox_str3);
        return;
    }

    
    
    
    
    
    if(SGNikeNameBox::utf8StrLength(buffer)==0)
    {
        SG_SHOW_WINDOW(str_LiuyanLayer_str14);
        return;
    }

    char temp[1024] = {};
    strcpy(temp, m_pTextFieldqq->getString());
    
    if (GameConfig::isContainsEmoji(temp)) {
        SG_SHOW_WINDOW(str_NikeNameBox_str3);
        return;
    }
    
    if(SGNikeNameBox::utf8StrLength(temp)==0)
    {
        SG_SHOW_WINDOW(str_LiuyanLayer_str15);
        return;
    }
 
        
    CCString*question=CCString::create(m_pTextFieldqq->getString());
    CCString*title=CCString::create(m_pEditTitle->getText());;
 
    main::SubQARequest*request=new main::SubQARequest();
    request->set_type(now_tag-99);
    request->set_question(question->m_sString);
    request->set_title(title->m_sString);
    
    
    SGSocketClient::sharedSocketClient()->send(MSG_SUBQA, request);
    
}

 void SGLiuyanLayer::sbumitsuccess(CCObject*obj)
{
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::SubQAResponse *response = (main::SubQAResponse *)sr->m_msg;
    int m= response->state();
    if(m)
    {
          SG_SHOW_WINDOW(str_LiuyanLayer_str16);
        ////////
        long  oldtime=getCurrentTime();
        SGPlayerInfo *playerinfo = SGPlayerInfo::sharePlayerInfo();
        playerinfo->setoldtime(oldtime);
        ///////
        
        SGQAConcent *player = new SGQAConcent();
        player->setID(response->id());
        std::string str(response->title());
        
        CCString* ns=CCString::createWithFormat("%s",str.c_str());
        player->setTitle(ns);
        player->setType(response->type());
        std::string str2(response->createdate());
        
        CCString* nn=CCString::createWithFormat("%s",str2.c_str());
        
        player->setcreateDate(nn);

        ////////////////////////
        
        
        std::vector<SGQAConcent*> vec;
        vec.push_back(player);


        
        //////////////////////////////////////////////////////////////////手动刷新列表顺序，将刚提交的放在最上方
        
        SGMainLayer *mainlayer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        
        mainlayer->updateqa(player);
        
        
        
//        
//        CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
//        CCArray* array=CCArray::create();
//         int m=temp->count();
//
//        for(int i=0;i<m;i++)
//        {
//            
//            SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
//            
//            array->addObject(qa);
//
//        }
//
//        SGPlayerInfo::sharePlayerInfo()->getQAContent()->removeAllObjects();////清空原有信息
//
//        ////////添加新信息
//        
//        int qqq=1;
//        if(qqq==1)
//        {
//            SGPlayerInfo::sharePlayerInfo()->addQAContent(player);
//            qqq=0;
//        }
//        
//                for(int i=0;i<m;i++)
//                {
//
//                    SGQAConcent * qa = (SGQAConcent*)array->objectAtIndex(i);
//                    
//               
//                 //   newtemp->addObject(qa);       /////////新顺序
//                    
//                    
//                    SGPlayerInfo::sharePlayerInfo()->addQAContent(qa);
//
//                }
//        
//        
    ////////////////
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
        
        SGComplainLayer *layer = SGComplainLayer::create(playerInfo->getQAContent());
        SGMainManager::shareMain()->showLayer(layer);
        
    /////////////////
    }

  
    else
    {
         SG_SHOW_WINDOW(str_LiuyanLayer_str17);
        
    }

  
    
 


}













#pragma mark editBox delegate

void SGLiuyanLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void SGLiuyanLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void SGLiuyanLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void SGLiuyanLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    
}

/////////////////

void SGLiuyanLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 99, true);
}

void SGLiuyanLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
// void SGLiuyanLayer::registerWithTouchDispatcher()
//{
// CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -222, false);
//}
 bool SGLiuyanLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

    return true;
}


 void SGLiuyanLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)

{

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
//    
//
    
    //        CCRect faceRect = CCRectMake(item->getPosition().x - item->getContentSize().width * .5 ,
    //                                     item->getPosition().y - 54,
    //                                     skewing(320),
    //                                     108);
    
    
    
    
    CCRect labelrect1 = CCRectMake(label1->getPosition().x ,label1->getPosition().y- label1->getContentSize().height * .5,label1->getContentSize().width,label1->getContentSize().height) ;

      CCRect labelrect2 = CCRectMake(label2->getPosition().x ,label2->getPosition().y- label2->getContentSize().height * .5,label2->getContentSize().width,label2->getContentSize().height) ;
      CCRect labelrect3 = CCRectMake(label3->getPosition().x,label3->getPosition().y - label3->getContentSize().height * .5,label3->getContentSize().width,label3->getContentSize().height) ;
      CCRect labelrect4 = CCRectMake(label4->getPosition().x ,label4->getPosition().y- label4->getContentSize().height * .5,label4->getContentSize().width,label4->getContentSize().height) ;
    
    
    
    CCPoint touchpoint=pTouch->getLocation();
    if(labelrect1.containsPoint(touchpoint))
    {
        setWhiteAndDarktouch(100);
    }
    if (labelrect2.containsPoint(touchpoint))
    {
        setWhiteAndDarktouch(101);
    }
    if (labelrect3.containsPoint(touchpoint))
    {
        setWhiteAndDarktouch(102);
    }
    
    if (labelrect4.containsPoint(touchpoint))
    {
        setWhiteAndDarktouch(103);
    }
    
    
    
}


/////////////////////////

long SGLiuyanLayer::getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec;
}






/////////////////////////


