//
//  SGChat.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#include "SGChat.h"
#include "SGMainManager.h"
#include "SGChatMessage.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "FriendModule.pb.h"
#include "SGGamePlayerInfo.h"
#include "SGOtherPlayerInfo.h"
#include "SGFriend.h"
#include "SGPlayerInfo.h"
#include "CCNotificationCenter.h"
#include "SGNikeNameBox.h"
#include "SGForbidChat.h"
#include "SGTools.h"
#include "Utils.h"
#include "SGChatInputBox.h"
#include "SGTestSwitch.h"
#include "cocos-ext.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGRankDetailLayer.h"
#include "SGStringConfig.h"
#include "AppMacros.h"

#define CHATBACKPOPTAG 1111
#define MILITYSIZEH 24
#define CHATGAPTIME 15
SGChat::SGChat()
:
labelNoHorn(NULL),
buttonSend(NULL),
_arrayChat(NULL),
scrollView(NULL),
scrollViewContainer(NULL)
{
    
}

SGChat::~SGChat()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
   
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_chatLayer);
}

SGChat* SGChat::create(CCArray *arrayChat)
{
    SGChat *chat = new SGChat();
    chat->_arrayChat = arrayChat;
    
    if(chat && chat->init(NULL, sg_chatLayer))
    {
        
        chat->constructView();
        chat->autorelease();
        return chat;
    }
    
    CC_SAFE_DELETE(chat);
    return NULL;
}

void SGChat::constructView()
{
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGChat::getPlayerInfoListener));
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SGChat::sendMessage), "SG_CHAT", NULL);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    //标题栏
    CCSprite *spriteTopBG = CCSprite::createWithSpriteFrameName("chattitlebg.png");
    
    SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGChat::buttonClickGoback),CCPointZero,FONT_PANGWA,ccWHITE,36);
	buttonBack->setScale(1.05);
    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("chat_title.png");
    if (spriteTitle) {
        spriteTitle->setPosition(ccpAdd(spriteTopBG->getPosition(), ccp(0, -spriteTopBG->getContentSize().height/2)));
        addChild(spriteTitle);
    }
    spriteTopBG->setAnchorPoint(ccp(0.5, 1));
    buttonBack->setAnchorPoint(ccp(0,0.5));
    spriteTopBG->setPosition(ccpAdd(upCenter, ccp(0, -spriteTopBG->getContentSize().height+3)));
    buttonBack->setPosition(ccp(1,spriteTopBG->getPosition().y - spriteTopBG->getContentSize().height*.5));
    addChild(spriteTopBG);
    addBtn(buttonBack);
    
    
    SGButton* buttonbgSend = SGButton::create("chatmsgbg.png", NULL, this, menu_selector(SGChat::buttonClickSend),CCPointZero,false,true);
    buttonbgSend->setPosition(ccpAdd(bottomCenter, ccp(-60, 150)));
    buttonbgSend->setScaleX(1.35);
    addBtn(buttonbgSend);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
     CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width, bg->getContentSize().height - 113);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    
    int shift = 0;
    if (CCDirector::sharedDirector()->getWinSize().height != 1136)
    {
        bg->setScaleY(0.86);
        shift = -30;
    }
    else
    {
        bg->setScaleY(1.05);
        shift = -20;
    }
    if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height)
    {
        bg->setScaleY(0.96);
    }
    
    
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,133 + shift)));

    
    //聊天框
    labelNoHorn = CCLabelTTF::create(str_Chat_str1, FONT_BOXINFO, 26);
    buttonSend = SGButton::create("store_exchangebtnbg.png", "chat_send.png", this, menu_selector(SGChat::buttonClickSend),CCPointZero,false,true);
    buttonSend->setAnchorPoint(ccp(1, 0.5));
    buttonSend->setPosition(ccp(SGLayout::getSkewing(320), 150));
    labelNoHorn->setColor(ccRED);
    labelNoHorn->setPosition(buttonSend->getPosition());
    labelNoHorn->setVisible(false);
    addBtn(buttonSend);
    addChild(labelNoHorn);
    
    
    SGCCLabelTTF *label_horn_1 = SGCCLabelTTF::create(str_ChatMessageCenter2, FONT_BOXINFO, 26);
    label_horn_1->setPosition(ccp(SGLayout::getSkewing(20), 150));
     label_horn_1->setAnchorPoint(ccp(0, 0.5));
    addChild(label_horn_1,5);
    
    //滑动
    scrollView = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-278));
    //初始化ScrollView的container
    scrollViewContainer = CCLayer::create();
    //把layer设置为container
    scrollView->setContainer(scrollViewContainer);
    //设置container的contentSize
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    //设置scrollView的位置
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    scrollView->setPosition(ccp(0, 101+80-10));
    this->addChild(scrollView);
    
    this->constructScrollView();
    
//    setSmallSpeakerCount();
    
}

#pragma mark - ----- 构建聊天界面 -----

CCSprite* SGChat::getBubbleByChatMessage(SGChatMessage *chatMessage)
{
  
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_chatLayer);

    /* 角色头像 */
    
	//12.05
	SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(chatMessage->getSoliderID());//获取静态数据

	CCString *fname = CCString::createWithFormat("head%d.png", temp->getIconId());//12.05
    SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(), sg_chatLayer);
    CCSprite *sprite_head = CCSprite::createWithSpriteFrameName(fname->getCString());
    
    
    
    //    if(sprite_head == NULL)
    //    {
    //        //sprite_head = CCSprite::createWithSpriteFrameName("countryIcon_1.png");
    //    }
    
    
    /* 角色头像的框 */
    //fname = CCString::createWithFormat("country_%d.png", chatMessage->getImage());
    // CCSprite *sprite_head_fontRect = CCSprite::createWithSpriteFrameName(fname->getCString());
    // sprite_head->addChild(sprite_head_fontRect);
    
    /*
     mail/mail_messageBox_2.png 我
     mail/mail_messageBox_1.png 对方
     */
    CCScale9Sprite *scalesprite_background = NULL;
    if(chatMessage->getRoleId() == SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId())
    {
        scalesprite_background = CCScale9Sprite::createWithSpriteFrameName("mail_messageBox_2.png");
    }else{
        scalesprite_background = CCScale9Sprite::createWithSpriteFrameName("mail_messageBox_1.png");
    }
    sprite_head->cocos2d::CCNode::addChild(scalesprite_background, -1, CHATBACKPOPTAG);
    
    
    
    /* //战斗力
     CCString *combateValue=CCString::createWithFormat("战斗力：%d",chatMessage->getCombateValue());
     SGCCLabelTTF *label_combate=SGCCLabelTTF::create(combateValue->getCString(), FONT_BOXINFO, 20);
     ccColor3B outColor;
     outColor.r=0;
     outColor.g=255;
     outColor.b=0;
     
     ccColor3B insideColor;
     insideColor.r=17;
     insideColor.g=63;
     insideColor.b=14;
     label_combate->setOutSideColor(insideColor);
     label_combate->setInsideColor(outColor);
     scalesprite_background->addChild(label_combate);
     
     */
    
    /* 说话这个玩家是不是自己 */
    bool isme = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() == chatMessage->getRoleId();
    //军衔等做好了之后加上去
    //CCString *lenStr=CCString::createWithFormat("%s(%s)",chatMessage->getRoleNickName()->getCString(),chatMessage->getMilitaryName());
    CCString *lenStr=CCString::createWithFormat(str_Chat_str3,chatMessage->getRoleNickName()->getCString());
    
    int strlen1=strlen(lenStr->getCString());
    int strlen2=strlen(chatMessage->getContent()->getCString());
    CCSize scaleSize ;
    if (strlen1<strlen2) {
        scaleSize = getSizeByCharBuffer(chatMessage->getContent()->getCString());
    }
    else
    {
        scaleSize =getSizeByCharBuffer(lenStr->getCString());
    }
    
    //军衔的显示以后加上
    CCString *militaryValue=CCString::createWithFormat("%s (%s)",chatMessage->getRoleNickName()->getCString(),chatMessage->getMilitaryName()->getCString());
    
    //SGCCLabelTTF *label_military=SGCCLabelTTF::create(militaryValue->getCString(), FONT_PANGWA, MILITYSIZEH);
    // SGCCLabelTTF *label_military=SGCCLabelTTF::create(militaryValue->getCString(), FONT_BOXINFO, MILITYSIZEH);
    
    CCLabelTTF *label_military = CCLabelTTF::create(militaryValue->getCString(), FONT_BOXINFO, 24);
    //    ccColor3B color;
    //    color.r=255;
    //    color.g=252;
    //    color.b=42;
    label_military->setColor(ccBLACK);
    scalesprite_background->addChild(label_military);
    
    /* 角色的昵称 */
    SGCCLabelTTF *label_playername = SGCCLabelTTF::create(chatMessage->getRoleNickName()->getCString(), FONT_BOXINFO, 24, CCSizeMake(scaleSize.width-50, scaleSize.height-50));
    //scalesprite_background->addChild(label_playername);
    
    /* 发言时间 */
    CCLabelTTF *label_time = CCLabelTTF::create(chatMessage->getTimeStr()->getCString(), FONT_BOXINFO, 24);
    scalesprite_background->addChild(label_time);
    
    /* 聊天内容 */
    SGCCLabelTTF *label_content = SGCCLabelTTF::create(chatMessage->getContent()->getCString(), FONT_PANGWA, 22, CCSizeMake(scaleSize.width-50, scaleSize.height-50) , ccWHITE);
    //    CCSprite *label_content = SGRichLabel::getFomLabel(chatMessage->getContent()->getCString(), CCSizeMake(480,354), FONT_BOXINFO, 24);
    scalesprite_background->addChild(label_content);
    //modify by:zyc. merge into create.
    //label_content->setColor(ccWHITE);
    
//    CCSprite *label_content = SGRichLabel::getFomLabel(chatMessage->getContent()->getCString(), CCSizeMake(scaleSize.width-50, scaleSize.height-50), FONT_BOXINFO, 24);
//    scalesprite_background->addChild(label_content);


    if(isme)
    {
        sprite_head->setAnchorPoint(ccp(1, 0.5));
        // sprite_head_fontRect->setAnchorPoint(ccp(1, 0.5));
        //  sprite_head_fontRect->setPosition(ccp(sprite_head_fontRect->getContentSize().width/2+sprite_head->getContentSize().width/2, sprite_head->getContentSize().height/2));
        
        
        scalesprite_background->setPreferredSize(scaleSize);
        scalesprite_background->setAnchorPoint(ccp(1, 1));
        scalesprite_background->setPosition(ccp(-10, scalesprite_background->getContentSize().height));
        
        
        //战斗力
        
        /* label_combate->setAnchorPoint(ccp(1, 1));
         label_combate->setPosition(ccp(scalesprite_background->getContentSize().width-30, scalesprite_background->getContentSize().height-10));
         */
        //CCSize combateSize=getSizeByCharBuffer(combateValue->getCString());
        //军衔
        label_military->setAnchorPoint(ccp(1, 1));
        label_military->setPosition(ccp(scalesprite_background->getContentSize().width-30, scalesprite_background->getContentSize().height-10));
        
        //昵称
        //CCSize nickNameSize=getSizeByCharBuffer(chatMessage->getRoleNickName()->getCString());
        label_playername->setAnchorPoint(ccp(1, 1));
        label_playername->setPosition(ccp(scalesprite_background->getContentSize().width-20, scalesprite_background->getContentSize().height-12));
        label_playername->setInsideColor(ccWHITE);
        label_time->setAnchorPoint(ccp(1, 1));
        label_time->setPosition(ccpAdd(label_military->getPosition() ,ccp(-30, 30)));
        label_time->setColor(ccWHITE);
        

        if (chatMessage->getIsChatShowHaHa()) {
            label_content->setVisible(false);
        }
        else
        {
            label_content->setAnchorPoint(ccp(1, 0.5));
            label_content->setPosition(ccp(scalesprite_background->getContentSize().width-25, label_content->getContentSize().height/2));
            //CCLog("Height:%f",scalesprite_background->getContentSize().height);
            //label_content->setString(chatMessage->getContent()->getCString());
        }
        
    }
    else
    {
        sprite_head->setAnchorPoint(ccp(0, 0.5));
        //  sprite_head_fontRect->setAnchorPoint(ccp(0, 0.5));
        //   sprite_head_fontRect->setPosition(ccp(-sprite_head_fontRect->getContentSize().width/2+sprite_head->getContentSize().width/2, sprite_head->getContentSize().height/2));
        
        scalesprite_background->setPreferredSize(scaleSize);
        scalesprite_background->setAnchorPoint(ccp(0, 1));
        scalesprite_background->setPosition(ccp(sprite_head->getContentSize().width+10, scalesprite_background->getContentSize().height));
        
        
        label_playername->setAnchorPoint(ccp(0, 1));
        label_playername->setPosition(ccp(30, scalesprite_background->getContentSize().height-10));
        label_playername->setInsideColor(ccWHITE);
        
        //军衔
        
        //CCSize playNameSize=getSizeByCharBuffer(chatMessage->getRoleNickName()->getCString());
//        label_military->setAnchorPoint(ccp(1, 1));
//        label_military->setPosition(ccpAdd(label_playername->getPosition(), ccp(playNameSize.width+30, 0)));
        
        label_military->setAnchorPoint(ccp(0, 1));
        label_military->setPosition(ccp(30, scalesprite_background->getContentSize().height-10));
        
        //战斗力
        /*  CCSize militarySize=getSizeByCharBuffer(militaryValue->getCString());
         label_combate->setAnchorPoint(ccp(1, 1));
         label_combate->setPosition(ccpAdd(label_military->getPosition(), ccp(militarySize.width-15, 0)));
         
         */
        label_time->setAnchorPoint(ccp(0, 1));
        label_time->setPosition(ccpAdd(label_military->getPosition(), ccp(-20, 30)));
        label_time->setColor(ccWHITE);
        
        if (chatMessage->getIsChatShowHaHa()) {
            label_content->setVisible(false);
        }
        else
        {
            label_content->setAnchorPoint(ccp(1, 0.5));
            label_content->setPosition(ccp(scalesprite_background->getContentSize().width-25, label_content->getContentSize().height/2 + 2));
            //label_content->setString(chatMessage->getContent()->getCString());
        }

    }
    
    return sprite_head;
}


CCSize SGChat::getSizeByCharBuffer(const char *charBuffer)
{
    //设置scalesprite_background的大小，使其他所有的组件都可以放在之上
    CCSize scaleSize;
    //计算说话的总长度
    int allwidth = 0;
//    for(int i=0;i<strlen(charBuffer);i++)
//    {
//        char ch = charBuffer[i];
//        if(SGTools::charIsChinese(ch))
//        {
//            allwidth += SGTools::getWidthOfOneChineseWord(FONT_PANGWA, 24);
//        }
//        else
//        {
//            allwidth += SGTools::getWidthOfOneEnglishWord(ch, FONT_PANGWA, 24);
//        }
//    }
    SGCCLabelTTF *playerStrlen = SGCCLabelTTF::create(charBuffer, FONT_PANGWA, 24);
    allwidth = playerStrlen->getContentSize().width;
    //得出发言文字所占的区域
    if(allwidth > SGLayout::getSkewing(180))
    {
        scaleSize.width = SGLayout::getSkewing(180);
        
        scaleSize.height = ((allwidth/SGLayout::getSkewing(180)) + 1)*playerStrlen->getContentSize().height;//SGTools::getHeightOfOneWord(FONT_PANGWA, 24);
    }
    else
    {
        //最小也要设为50
        //       if(allwidth < 350)
        //        {
        //          allwidth = 350;
        //        }
        scaleSize.width = allwidth;
        scaleSize.height = playerStrlen->getContentSize().height;  //SGTools::getHeightOfOneWord(str_Chat_str4, 24);
    }
    
    //加上人物昵称这一行的高度
    scaleSize.height += SGTools::getHeightOfOneWord(FONT_PANGWA, 24);
    
    //稍微扩充一下，让气泡变得大一点
    scaleSize.width += 80;
    scaleSize.height += 20;
    
    return scaleSize;
}

//设置scrollview
void SGChat::constructScrollView()
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_chatLayer, LIM_PNG_AS_PNG);
    scrollViewContainer->removeAllChildrenWithCleanup(true);
    
    //高度计数器，设置每个CCLayerColor的偏移量，循环完后这就是整个ScrollView的ContentSize
    int heigth = 0;
    
    //存放所有加入container的元素
    CCArray *arraySprite = CCArray::create();
    //存放所有加入container的button
    CCArray *arrayButon = CCArray::create();
    
    CCArray *array = SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
    
//    SGMenu *menu = SGMenu::create(CCRectMake(0,SGLayout::getSkewing(70),SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-278));
	CCMenu *menu = CCMenu::create();
	menu->setTouchPriority(128);
    menu->setPosition(CCPointZero);
    menu->setTag(99999);
    int chatCount = array->count();
    for(int i=chatCount-1;i>=0;i--)
    {
        SGChatMessage *chatMessage = (SGChatMessage*)array->objectAtIndex(i);
        
        CCSprite *sprite = getBubbleByChatMessage(chatMessage);
        
        //注意，这里是作为container的CCLayer添加
        int touchTag = chatMessage->getRoleId();
        if(chatMessage->getRoleId() == SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId())
        {
            sprite->setPosition(ccp(SGLayout::getSkewing(310), heigth+10));
            sprite->setAnchorPoint(ccp(1, 0));
        }else{
            touchTag = chatMessage->getRoleId();
            sprite->setPosition(ccp(SGLayout::getSkewing(10), heigth+10));
            sprite->setAnchorPoint(ccp(0, 0));
        }
        
        
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(chatMessage->getSoliderID());
        
        int levle=temp->getOfficerCurrStarLevel();
        if (levle>1) {
            levle=levle-1;
        }
        
        CCString *countryStr = CCString::createWithFormat("equipPinzhi_%d_0.png",levle);
        
        
        SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(), sg_chatLayer);
		CCString *fname2 = CCString::createWithFormat("head%d.png", temp->getIconId());//12.06
		
        SGButton *button = SGButton::create(fname2->getCString(),countryStr->getCString(), this,
                                            menu_selector(SGChat::showSpeakerInfo), ccp(0, 0),
                                            false, true);
        
        //vip等级
        CCString *vipNum;
        CCSprite *sprite_vip;
        
        if (chatMessage->getVipLevel()>0 && SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
            vipNum=CCString::createWithFormat("vip%d_chat.png",chatMessage->getVipLevel());
            sprite_vip=CCSprite::createWithSpriteFrameName(vipNum->getCString());
            
            sprite_vip->setVisible(true);
        }
        else
        {
            sprite_vip=CCSprite::createWithSpriteFrameName("vip1_chat.png");
            sprite_vip->setVisible(false);
        }
        sprite_vip->setPosition(ccpAdd(button->getPosition(), ccp(60, 10)));
        button->addChild(sprite_vip);
        
        button->setTag(touchTag);
        menu->addChild(button);
        
        button->setAnchorPoint(ccp(sprite->getAnchorPoint().x, 0.5f));
        button->setPosition(ccpAdd(sprite->getPosition(), ccp(0, sprite->getContentSize().height*.5f)));
        scrollViewContainer->addChild(sprite);
        
        arraySprite->addObject(sprite);
        arrayButon->addObject(button);
        
        //设置偏移量
        //int spriteHeight = 0;
        CCScale9Sprite *scaleSprite = (CCScale9Sprite*)sprite->getChildByTag(CHATBACKPOPTAG);
        //spriteHeight = scaleSprite->getPreferredSize().height;
        
        /*
         * 记录scrollview的高度
         * 偏移量计数器加加,108金框的高度
         */
        if(scaleSprite->getContentSize().height > 108)
        {
            heigth += scaleSprite->getContentSize().height;
        }else{
            heigth += 108;
        }
        //加一些缝隙，显得不那么拥挤
        heigth += 20;
        
        //添加展示物品
        if (chatMessage->getIsChatShowHaHa()) {
            SGCCLabelTTF *label_ShowName = SGCCLabelTTF::create(chatMessage->getChatShowyName()->getCString(), FONT_PANGWA, 24);
            
            int showStar = chatMessage->getChatShowStarLevel();
            showStar -= 1;
            
            if (showStar >= 0 && showStar < 5) {
                label_ShowName->setInsideColor(ccStarLevelColor[showStar]);
            }
            else
            {
                label_ShowName->setInsideColor(ccWHITE);
            }
            
            CCMenuItemSprite *chatShowItem = CCMenuItemSprite::create(label_ShowName, label_ShowName, this, menu_selector(SGChat::showItemClickSend));
            chatShowItem->setTag(i);
            
            int posX = 0;
            if(chatMessage->getRoleId() == SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId())
            {
                chatShowItem->setAnchorPoint(ccp(0.5, 1));
                posX = -scaleSprite->getPreferredSize().width/2  - sprite->getContentSize().width;
            }
            else
            {
                chatShowItem->setAnchorPoint(ccp(0.5, 1));
                touchTag = chatMessage->getRoleId();
                posX = scaleSprite->getPreferredSize().width/2  + sprite->getContentSize().width;
            }
            
            chatShowItem->setPosition(ccpAdd(sprite->getPosition(), ccp(posX, scaleSprite->getContentSize().height-MILITYSIZEH-16)));
            menu->addChild(chatShowItem);
            arrayButon->addObject(chatShowItem);
        }
    }
    
    //设置container的contentSize
    if(heigth > scrollView->cocos2d::CCNode::getContentSize().height)
    {
        scrollViewContainer->setContentSize(CCSizeMake(SGLayout::getSkewing(300), heigth));
        scrollView->setContentSize(CCSizeMake(SGLayout::getSkewing(300), heigth));
    }else if(0 !=  heigth){
        
        CCSprite *sprite_last = (CCSprite*)arraySprite->lastObject();
        
        /*
         计算最上面一个ccsprite与顶端的距离
         */
        int distance = 0;
        int scaleHeight = 0;
        int headHeight = 108;
        int scrollviewHeight = 0;
        
        scrollviewHeight = scrollView->cocos2d::CCNode::getContentSize().height;
        scaleHeight = sprite_last->getChildByTag(CHATBACKPOPTAG)->getContentSize().height;
        distance = scrollviewHeight - sprite_last->getPosition().y;
        if(headHeight > scaleHeight)
        {
            distance -= headHeight;
        }else{
            distance -= scaleHeight;
        }
        
        //遍历所有数组，调整差距
        CCObject *object = NULL;
        CCARRAY_FOREACH(arraySprite, object)
        {
            CCSprite *sprite_bubble = (CCSprite*)object;
            sprite_bubble->setPositionY(sprite_bubble->getPositionY()+distance);
        }
        CCARRAY_FOREACH(arrayButon, object)
        {
            CCMenuItemSprite  *sprite_button = (CCMenuItemSprite*)object;
            sprite_button->setPositionY(sprite_button->getPositionY()+distance);
        }
        
        //设置滑动区域为整个scrollview的高度
        scrollViewContainer->setContentSize(CCSizeMake(SGLayout::getSkewing(300),scrollviewHeight));
        scrollView->setContentSize(CCSizeMake(SGLayout::getSkewing(300), scrollviewHeight));
    }
    //    float scHigh = scrollViewContainer->getContentSize().height;
    //    menu->setPosition(ccp(0, scHigh*.5f));
    scrollViewContainer->addChild(menu);
    scrollView->setTouchEnabled(true);
    //设置方向，为纵向
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    
}

void SGChat::showSpeakerInfo(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
   
    
    int sid = ((CCMenuItem *)sender)->getTag();
    if (sid == -1) return; //自己，直接返回
    
    CCLOG("===================howSpeakerInfo");
    main::FriendInfoRequest *info = new main::FriendInfoRequest();
    info->set_friendid(sid);
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);
}

#pragma mark - ----- 按钮点击 -----

void SGChat::buttonClickGoback()
{
    SGMainManager::shareMain()->showFirstLayer();
}

void SGChat::buttonClickSend()
{
    EFFECT_PLAY(MUSIC_BTN);
    //检查聊天等级
    if(!getPlayerLevel())
    {
        SG_SHOW_WINDOW(str_Chat_str5);
        return;
    }
    
    
    if (SGPlayerInfo::sharePlayerInfo()->getchatTime() < CHATGAPTIME)
    {
        SGMainManager::shareMain()->showMessage(str_ChatMessageCenter2);
        return;
    }
    
    //判断小喇叭
//    if(getSmallTrumpetCount() <= 0)
//    {
//        SGForbidChat *forbid = SGForbidChat::create(this);
//        SGMainManager::shareMain()->showBox(forbid);
//        return;
//    }
    
    SGChatInputBox *inputBox = SGChatInputBox::create(this);
    //    SGMainManager::shareMain()->getMainScene()->addChild(inputBox);
    SGMainManager::shareMain()->showBox(inputBox);
}
void SGChat::showItemClickSend(CCObject *obj)
{
    CCMenuItemSprite *chatShowItem = (CCMenuItemSprite *)obj;
    int index = chatShowItem->getTag();
    CCArray *array = SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
    if (index < array->count()) {
        SGChatMessage *chatMessage = (SGChatMessage*)array->objectAtIndex(index);
        main::ViewSpectacularItemRequest *info = new main::ViewSpectacularItemRequest();
        info->set_ssid(chatMessage->getChatShowSSid());
        info->set_roleid(chatMessage->getRoleId());
        info->set_type(chatMessage->getChatShowType());
        SGSocketClient::sharedSocketClient()->send(MSG_CHAT_SHOWITEM, info);
    }
}


#pragma mark - ----- 处理小喇叭数量 -----

//设置小喇叭显示数量
//void SGChat::setSmallSpeakerCount()
//{
//    labelHornCount->setString(CCString::createWithFormat("%d",getSmallTrumpetCount())->getCString());
//}

//减少小喇叭个数
//void SGChat::minusSmallTrumpetCount()
//{
//    int smallTrumpet  = SGPlayerInfo::sharePlayerInfo()->getSpeakerCount();
//    smallTrumpet --;
//    SGPlayerInfo::sharePlayerInfo()->setSpeakerCount(smallTrumpet);
//}

//获得小喇叭个数
//int SGChat::getSmallTrumpetCount()
//{
//    return SGPlayerInfo::sharePlayerInfo()->getSpeakerCount();
//}

//检验玩家是否到了5级，5级才能开启聊天功能
bool SGChat::getPlayerLevel()
{
    int level = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if(level < 5)
    {
        return false;
    }
    
    return true;
}

#pragma mark - ----- 数据交互 -----

void SGChat::sendMessage(CCObject *chatObject)
{
    _arrayChat = SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
    constructScrollView();
}

//获取玩家信息监听
void SGChat::getPlayerInfoListener(cocos2d::CCObject *object)
{
    if(this->isVisible()){
    CCLOG("接收到了玩家信息");
    SGMainLayer *mainlayer =  (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    mainlayer->setPortraitMenuCanTouch(false);
    
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("getPlayerInfoListener error !!! ");
        return;
    }
    
   
    main::FriendInfoResponse *roleInfo = (main::FriendInfoResponse*)request->m_msg;
    if (roleInfo->role().roleid()) {
        
        SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(roleInfo->role());
//        SGOtherPlayerInfo *other = SGOtherPlayerInfo::create(playerInfo, this);
//        SGMainManager::shareMain()->showBox(other);
        if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank));
        {
            SGMainManager::shareMain()->getMainScene()->removeChildByTag(box_rank);
        }
        SGRankDetailLayer*box=SGRankDetailLayer::create(playerInfo,1);
        SGMainManager::shareMain()->showBox(box);
    }
    }
}


//回调函数
void SGChat::showBoxCall(CCObject *obj)
{
    CCDictionary *dict = (CCDictionary*)obj;
    CCString *key = (CCString*)dict->objectForKey("key");
    if(!key)
    {
        return;
    }
    
    if(key->isEqual(CCString::create("inputBox")))
    {
        //输入框回调
        CCString *sendMessage = (CCString*)dict->objectForKey("string");
        main::SendMessageRequest *chat = new main::SendMessageRequest();
        chat->set_chid(1);
        chat->set_content(sendMessage->getCString());
        SGSocketClient::sharedSocketClient()->send(MSG_CHAT_MESSAGE, chat);
        
        //修改小喇叭数量
//        minusSmallTrumpetCount();
//        setSmallSpeakerCount();
        
        
        if(SGPlayerInfo::sharePlayerInfo()->getchatTime()>30)
        {
            SGMainLayer *mainlayer=SGMainManager::shareMain()->getMainLayer();
            
            SGPlayerInfo::sharePlayerInfo()->setchatTime(0);
            mainlayer->schedule(schedule_selector(SGMainLayer::computeChateTime), 1);
        }
        
    }
    else if(key->isEqual(CCString::create("forbid")))
    {
        //小喇叭不足弹窗回调
//        setSmallSpeakerCount();
    }
}

void SGChat::setIsCanTouch(bool isTouch)
{
      //this->setIsCanTouch(isTouch);
    ((CCMenu *)scrollViewContainer->getChildByTag(99999))->setTouchEnabled(isTouch);
    menu->setTouchEnabled(isTouch);
    scrollView->setTouchEnabled(isTouch);
}
//void SGChat::onEnter(){
//    SGBaseLayer::onEnter();
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, false);
//}
// 
//bool SGChat::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    CCPoint m_clickPos = convertTouchToNodeSpace(pTouch);
//    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
//    
//    CCLOG("m_clickPos.x=%f,m_clickPos.y=%f",m_clickPos.x,m_clickPos.y);
//    CCRect touchBounds=CCRectMake( 0, winSize.height/22-5, winSize.width, winSize.height/23*19);
//    if (touchBounds.containsPoint(m_clickPos)) {
//        setIsCanTouch(true);
//    }
//    else
//        setIsCanTouch(false);
//    return true;
//
// 
////    return true;
//}
