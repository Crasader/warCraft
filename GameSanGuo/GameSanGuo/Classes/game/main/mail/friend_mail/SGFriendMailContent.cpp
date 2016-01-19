//
//  SGFriendMailContent.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-28.
//
//

#include "SGFriendMailContent.h"
#include "SGMainManager.h"
#include "SGMailDataModel.h"
#include "SGPlayerInfo.h"
#include "SGMail.h"
#include "SGMailInputBox.h"
#include "SGFriendMailLayer.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGTestSwitch.h"
#include "ResourceManager.h"
#include "SGTools.h"
#include "SGStringConfig.h"

SGFriendMailContent::SGFriendMailContent()
{
    labelNickName = NULL;
    scrollview = NULL;
    _dataModel = NULL;
    _timer = 0;
    roleId = 0;
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_MAIL_READMAIL, this, callfuncO_selector(SGFriendMailContent::readMailListener));
}

SGFriendMailContent::~SGFriendMailContent()
{
  
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_friendMailContent);
    CC_SAFE_RELEASE(_dataModel);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_READMAIL);
}

SGFriendMailContent* SGFriendMailContent::create(SGMailDataModel *dataModel)
{
    SGFriendMailContent *friendContent = new SGFriendMailContent();
    if(friendContent && friendContent->init(NULL, sg_friendMailContent))
    {
        friendContent->_dataModel = dataModel;
        friendContent->_dataModel->retain();
        friendContent->viewDidLoad();
        friendContent->autorelease();
        return friendContent;
    }
    
    CC_SAFE_DELETE(friendContent);
    return NULL;
}

void SGFriendMailContent::viewDidLoad()
{
 
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMailContent);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_friendMailContent, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_friendMailContent);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMailContent);
    //读取邮件
    if(!_dataModel->getMailIsRead())
    {
        main::ShowMailRequest *request = new main::ShowMailRequest();
        request->set_mailid(_dataModel->getMailSsid());
        SGSocketClient::sharedSocketClient()->send(MSG_MAIL_READMAIL, request);
    }
    
    
    //界面布局
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    //金线
    CCSprite *spriteLine_1 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    CCSprite *spriteLine_2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    spriteLine_1->setScaleX(SGLayout::getSkewing(320)/spriteLine_1->getContentSize().width);
    spriteLine_2->setScaleX(SGLayout::getSkewing(320)/spriteLine_2->getContentSize().width);
    spriteLine_1->setPosition(ccpAdd(bottomCenter, ccp(0, 120)));
    spriteLine_2->setPosition(ccpAdd(bottomCenter, ccp(0, 180)));
    addChild(spriteLine_1);
    addChild(spriteLine_2);
    
    
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    //一个黑条
    SGButton *spriteRoleBoard = SGButton::create("hd.png", NULL, this, menu_selector(SGFriendMailContent::buttonClickSendMessage),CCPointZero,false,true);
    spriteRoleBoard->setPosition(ccpAdd(bottomCenter, ccp(0, btmhgt)));
    addBtn(spriteRoleBoard);
    
    //标题栏
    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("title_bg.png");
    CCSprite *spriteTitle_Line_1 = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    CCSprite *spriteTitle_Line_3 = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    spriteTitle->setAnchorPoint(ccp(0.5, 1));
    spriteTitle_Line_1->setAnchorPoint(ccp(0, 0));
    spriteTitle_Line_3->setAnchorPoint(ccp(0, 0));
    spriteTitle->setPosition(ccpAdd(upCenter,ccp(0, 0)));
    spriteTitle_Line_1->setPosition(ccpAdd(upLeft,ccp(0, -spriteTitle->getContentSize().height-10)));
    spriteTitle_Line_1->setScaleX(SGLayout::getSkewing(160)/spriteTitle_Line_1->getContentSize().width);
    spriteTitle_Line_3->setPosition(ccpAdd(upLeft, ccp(SGLayout::getSkewing(320), -spriteTitle->getContentSize().height-10)));
    spriteTitle_Line_3->setScaleX(SGLayout::getSkewing(160)/spriteTitle_Line_3->getContentSize().width);
    spriteTitle_Line_3->setRotation(180);
    spriteTitle_Line_3->setScaleY(-1);
    addChild(spriteTitle);
    addChild(spriteTitle_Line_1);
    addChild(spriteTitle_Line_3);
    
    //标题
    SGCCLabelTTF *spriteTitleText = SGCCLabelTTF::create(str_SearchFriend_str14, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    spriteTitleText->setPosition(ccpAdd(upCenter, ccp(0, -spriteTitleText->getContentSize().height/1.4)));
    this->addChild(spriteTitleText);
    
    //星形背景
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_friendMailContent);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    //CCRect r = CCRectMake(0, 1136/2 - (skewingY(480)  - spriteTitle->getContentSize().height - 107)/2,
                          //bg->getContentSize().width, skewingY(480) - spriteTitle->getContentSize().height - 107);
    this->addChild(bg,-5);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    bg->setScaleY(1.3);
    //bg->setTextureRect(r);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt*0.7)));
    
    //五张图片的尺寸   -79-150-72-56-101-30
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGFriendMailContent::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    buttonBack->setPosition(ccpAdd(spriteTitleText->getPosition(), ccp(-SGLayout::getSkewing(160), 0)));
    addBtn(buttonBack);
    
    //发信息
    SGButton *buttonSendMessage = SGButton::create("public_btn_1.png", "mail_send.png", this, menu_selector(SGFriendMailContent::buttonClickSendMessage),CCPointZero,false,true);
    buttonSendMessage->setPosition(ccpAdd(bottomCenter, ccp(240, btmhgt)));
    addBtn(buttonSendMessage);
    
    //好友昵称
    SGCCLabelTTF *labelNick_1 = SGCCLabelTTF::create(str_SearchFriend_str15, FONT_BOXINFO, 24);
    SGCCLabelTTF *labelNick_2 = SGCCLabelTTF::create(str_SearchFriend_str16, FONT_BOXINFO, 24);
    labelNickName = SGCCLabelTTF::create(str_SearchFriend_str17, FONT_BOXINFO, 24);
    labelNickName->setString(_dataModel->getMailOtherNickname()->getCString());
    
    labelNick_1->setAnchorPoint(ccp(0, 0.5));
    labelNickName->setAnchorPoint(ccp(0, 0.5));
    labelNick_2->setAnchorPoint(ccp(0, 0.5));
    
    labelNick_1->setPosition(ccpAdd(bottomCenter, ccp(-300, 150)));
    labelNickName->setPosition(ccpAdd(labelNick_1->getPosition(), ccp(labelNick_1->getContentSize().width, 0)));
    labelNick_2->setPosition(ccpAdd(labelNickName->getPosition(), ccp(labelNickName->getContentSize().width, 0)));
    
    addChild(labelNick_1,5);
    addChild(labelNick_2,5);
    addChild(labelNickName,5);
    
    //scrollview
    scrollview = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(300), SGLayout::getSkewingY(480)-180-72));
    CCLayer* container = CCLayer::create();
    scrollview->setContainer(container);
    scrollview->setDirection(kCCScrollViewDirectionVertical);
    scrollview->setPosition(ccp(0, 180));
    addChild(scrollview);
    
    //construct scrollview
    constructScrollView();
    
}

void SGFriendMailContent::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendMail);
    if(layer)
    {
        SGMainManager::shareMain()->showLayer(layer);
        this->removeFromParentAndCleanup(true);
        
    }else{
        SGFriendMailLayer *mail = SGFriendMailLayer::create(SGPlayerInfo::sharePlayerInfo()->getFriendMails());
        SGMainManager::shareMain()->showLayer(mail);
    }
}

void SGFriendMailContent::buttonClickSendMessage()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMailInputBox *input = SGMailInputBox::create(this);
    SGMainManager::shareMain()->getMainScene()->addChild(input);
    
}

void SGFriendMailContent::constructScrollView()
{
    //如果有以前的消息，先清除，然后再构建，有点浪费资源，有点慢，但是好用
    scrollview->getContainer()->removeAllChildrenWithCleanup(true);
    
    //我的roleId
    int myRoleId = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
    
    //记录所有的高度
    int height = 0;
    
    int count = _dataModel->getMailContent()->count();
    _timer=0;
    for(int i=0;i<count;i++)
    {
        SGMailDetailDataModel *detail = (SGMailDetailDataModel*)_dataModel->getMailContent()->objectAtIndex(i);
        CCScale9Sprite *sprite = getBubbleOfTalk(detail->getMailDeSenderId() == myRoleId, detail->getMailDeContent()->getCString());
        height += sprite->getPreferredSize().height;
        height += 20;
        
        //时间标签
        //判断时间是否已经过去了15分钟

        if(detail->getMailDeSendDate()/1000/60 - _timer > 15)
        {
            _timer = detail->getMailDeSendDate()/1000/60;
            height += getHeightOfOneWord();
            height += 20;
        }
    }
    
    //聊天信息还没有填充满屏幕呢，只能缩小scrollview的范围了
    scrollview->setTouchEnabled(height > scrollview->cocos2d::CCNode::getContentSize().height);
    scrollview->setContentSize(CCSizeMake(SGLayout::getSkewing(300), height));
    //如果没有那么大就设置成这么大
    if(height < scrollview->cocos2d::CCNode::getContentSize().height)
    {
        height = scrollview->cocos2d::CCNode::getContentSize().height;
    }
    
    //添加进去
    _timer = 0;
    int bubbleCount = _dataModel->getMailContent()->count();
    for(int i=0;i<bubbleCount;i++)
    {
        SGMailDetailDataModel *detail = (SGMailDetailDataModel*)_dataModel->getMailContent()->objectAtIndex(i);
        
        //时间标签
        //判断时间是否已经过去了15分钟
        if(detail->getMailDeSendDate()/1000/60 - _timer > 15)
        {
            _timer = detail->getMailDeSendDate()/1000/60;
            char time_buffer[128];
            sprintf(time_buffer, "------- %s -------",SGMail::getTimeStringByTimeStamp(detail->getMailDeSendDate()).c_str());
            SGCCLabelTTF *labelTime = SGCCLabelTTF::create(time_buffer, FONT_BOXINFO, 24);
            labelTime->setPosition(ccp(SGLayout::getSkewing(150), height - labelTime->getContentSize().height/2));
            scrollview->getContainer()->addChild(labelTime);
            height -= getHeightOfOneWord();
            height -= 20;
        }
        
        
        CCScale9Sprite *sprite = getBubbleOfTalk(detail->getMailDeSenderId() == myRoleId, detail->getMailDeContent()->getCString());
        if(myRoleId == detail->getMailDeSenderId())
        {
            sprite->setPosition(ccp(SGLayout::getSkewing(300)-sprite->getPreferredSize().width/2,height - sprite->getPreferredSize().height/2));
        }else
        {
            sprite->setPosition(ccp(sprite->getPreferredSize().width/2, height - sprite->getPreferredSize().height/2));
        }
        
        scrollview->getContainer()->addChild(sprite);
        
        height -= sprite->getPreferredSize().height;
        height -= 20;
        
    }
}

CCScale9Sprite* SGFriendMailContent::getBubbleOfTalk(bool isMe,const char *message)
{
	 ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendMailContent);
    CCScale9Sprite *spriteBubble = NULL;
    
    //根据对方与我，分别设置不同气泡
    if(isMe)
    {
        spriteBubble = CCScale9Sprite::createWithSpriteFrameName("mail_messageBox_2.png");
    }else{
        spriteBubble = CCScale9Sprite::createWithSpriteFrameName("mail_messageBox_1.png");
    }
 CCSize sizesize= getSizeByCharBuffer(message);
    //计算说话的总长度
    //把label添加进气泡
    
    SGCCLabelTTF *label = SGCCLabelTTF::create(message, FONT_BOXINFO, 24,CCSizeMake(sizesize.width+20, sizesize.height));
    spriteBubble->setPreferredSize(CCSizeMake(sizesize.width+40, sizesize.height+20));
    label->setPosition(ccp(spriteBubble->getContentSize().width/2,spriteBubble->getContentSize().height/2));
    spriteBubble->addChild(label);
    
    return spriteBubble;
    
}

bool SGFriendMailContent::charIsChinese(char ch)
{
    return (ch < 0);  //ascll码小于0，判断为中文，不过可能不太靠谱
}

//获得一个汉字的宽度
int SGFriendMailContent::getWidthOfOneChineseWord(const char* fontName,int fontSize)
{
    
    
    CCLabelTTF *label = CCLabelTTF::create("你", fontName, fontSize);
    return label->getContentSize().width/3;
}

//获得一个英文字母的宽度
int SGFriendMailContent::getWidthOfOneEnglishWord(char ch,const char* fontName,int fontSize)
{
    char buffer[2];
    sprintf(buffer, "%c",ch);
    CCLabelTTF *label = CCLabelTTF::create(buffer, fontName, fontSize);
    return label->getContentSize().width;
}

int SGFriendMailContent::getHeightOfOneWord(const char* fontName ,int fontSize )
{
    CCLabelTTF *label = CCLabelTTF::create("你", fontName, fontSize);
    return label->getContentSize().height;
}

//绝对是跟OC一模一样的set方法
void SGFriendMailContent::setDataModel(SGMailDataModel *newDataModel)
{
    if(_dataModel)
    {
        _dataModel->release();
    }
    
    _dataModel = newDataModel;
    _dataModel->retain();
}

void SGFriendMailContent::showBoxCall(cocos2d::CCObject *obj)
{
    roleId = _dataModel->getMailOtherId();
    CCDictionary *dict = (CCDictionary*)obj;
    CCString *flagStr = (CCString*)dict->objectForKey("key");
    if(flagStr->isEqual(CCString::create("inputBox")))
    {
        main::SendMailRequest *request = new main::SendMailRequest();
        CCString* contentStr = (CCString*)dict->objectForKey("string");
        request->set_content(contentStr->m_sString);
        request->set_type(1);
        request->set_receiverid(roleId);
        SGSocketClient::sharedSocketClient()->send(MSG_MAIL_SENDMESSAGE, request);
        
    }
    
}

void SGFriendMailContent::readMailListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("readMailListener error !!! ");
        return;
    }
    
    main::ShowMailResponse *response = (main::ShowMailResponse*)request->m_msg;
    if(0 == response->state())
    {
        CCLOG("读取邮件失败");
    }else{
        CCLOG("读取邮件成功");
        _dataModel->setMailIsRead(true);
    }
    
}



CCSize SGFriendMailContent::getSizeByCharBuffer(const char *charBuffer)
{
    //设置scalesprite_background的大小，使其他所有的组件都可以放在之上
    CCSize scaleSize;
    SGCCLabelTTF* templable = SGCCLabelTTF::create(charBuffer, FONT_BOXINFO, 24);
    
    
    
    //计算说话的总长度
    int allwidth = 0;
//    for(int i=0;i<strlen(charBuffer);i++)
//    {
//        char ch = charBuffer[i];
//        if(SGTools::charIsChinese(ch))
//        {
//            allwidth += getWidthOfOneChineseWord(FONT_BOXINFO, 24);
//        }
//        else
//        {
//            allwidth += getWidthOfOneEnglishWord(ch, FONT_BOXINFO, 24);
//        }
//    }
    
    
    allwidth = templable->getContentSize().width;
    //得出发言文字所占的区域
    if(allwidth > SGLayout::getSkewing(160))
    {
        scaleSize.width = SGLayout::getSkewing(160);
        scaleSize.height = ((int)(allwidth/SGLayout::getSkewing(160))+1)*SGTools::getHeightOfOneWord(FONT_BOXINFO, 24);
    }
    else
    {
        //最小也要设为50
       if(allwidth < 48*2)
            {
            allwidth = 48*2-10;
            }
        scaleSize.width = allwidth;
        scaleSize.height = 30;
    }
    
    return scaleSize;
}
