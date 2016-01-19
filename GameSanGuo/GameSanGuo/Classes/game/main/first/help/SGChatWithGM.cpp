//
//  SGChat.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#include "SGChatWithGM.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
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
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGComplainLayer.h"
#include "SGChatWithGMInputBox.h"
#include "SGStringConfig.h"
#include "AppMacros.h"

SGChatWithGM::SGChatWithGM()
:_array(NULL),
temptemp(NULL),
buttonSend(NULL),
checkid(0),
scrollView(NULL),
scrollViewContainer(NULL),
fontlabel(NULL)
{

    
}

SGChatWithGM::~SGChatWithGM()
{

    CC_SAFE_RELEASE(temptemp);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_chatwithgm);
    ///
    SGMainLayer *lay = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
   
    SGButton*btn= (SGButton*)  lay->getBtnByTag(54321);
    btn->setEnabled(true);
}

SGChatWithGM* SGChatWithGM::create(CCArray *array,int idid)
{
    SGChatWithGM *chat = new SGChatWithGM();
  
    
    if(chat && chat->init(NULL, sg_chatwithgm))
    {
        chat->_array = array;
        
        chat->checkid=idid;
        chat->constructView();
        chat->autorelease();
        return chat;
    }
    
    CC_SAFE_DELETE(chat);
    return NULL;
}

void SGChatWithGM::constructView()
{
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    
    
    temptemp=CCArray::create();
    temptemp->retain();
    int count=_array->count();
    for(int i=0;i<=count-1;i++)
        {
            temptemp->addObject(_array->objectAtIndex(i));
        }
    
    
    
    //标题栏
    CCSprite *spriteTopBG = CCSprite::createWithSpriteFrameName("chattitlebg.png");
    
    SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGChatWithGM::buttonClickGoback),CCPointZero,FONT_PANGWA,ccWHITE,36);
	buttonBack->setScale(1.05);
    
    
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_CardsLayer_str6, FONT_PANGWA, 36 , COLOR_UNKNOW_TAN);
    
    spriteTopBG->setAnchorPoint(ccp(0.5, 1));
    buttonBack->setAnchorPoint(ccp(0,0.5));
    spriteTopBG->setPosition(ccpAdd(upCenter, ccp(0, -spriteTopBG->getContentSize().height+3)));
    buttonBack->setPosition(ccp(1,spriteTopBG->getPosition().y - spriteTopBG->getContentSize().height*.5));
    title->setPosition(ccpAdd(spriteTopBG->getPosition(), ccp(0, -spriteTopBG->getContentSize().height/2)));
    addChild(spriteTopBG);
    addBtn(buttonBack);
    addChild(title);
    
    //发言按钮左边黑条
    //    CCScale9Sprite *spriteRoleBoard = CCScale9Sprite::createWithSpriteFrameName("chatmsgbg.png");
    //    spriteRoleBoard->setPreferredSize(CCSizeMake(s.width- 140, 48));
    //    this->addChild(spriteRoleBoard);
    //    spriteRoleBoard->setPosition(ccpAdd(bottomCenter, ccp(-60, 135)));
    SGButton* buttonbgSend = SGButton::create("chatmsgbg.png", NULL, this, menu_selector(SGChatWithGM::buttonClickSend),CCPointZero,false,true);
    buttonbgSend->setPosition(ccpAdd(bottomCenter, ccp(-60, 135)));
    buttonbgSend->setScaleX(1.35);
    // buttonbgSend->setAnchorPoint(ccp(0.5, 0));
    addBtn(buttonbgSend);
   //发言按钮
   SGButton* buttonSend = SGButton::create("store_exchangebtnbg.png", "chat_send.png", this, menu_selector(SGChatWithGM::buttonClickSend),CCPointZero,false,true);
    buttonSend->setAnchorPoint(ccp(1, 1));
    buttonSend->setPosition(ccp(SGLayout::getSkewing(315), 158));
    this->addBtn(buttonSend);

    
    
    
    ///黑条里默认的字
    fontlabel= SGCCLabelTTF::create(str_CardsLayer_str7, FONT_BOXINFO, 28);
    fontlabel->setPosition(buttonbgSend->getPosition());
    this->addChild(fontlabel,5);
    
   //大bg
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_chatwithgm);

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

    ////
    //滑动
    scrollView = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-278));
    //初始化ScrollView的container
    scrollViewContainer = CCLayer::create();
    //把layer设置为container
    scrollView->setContainer(scrollViewContainer);
    //设置container的contentSize
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    //设置scrollView的位置
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    scrollView->setPosition(ccp(0, 101+80-10));
    this->addChild(scrollView);
    
    this->constructScrollView();
    

    
}


CCSprite* SGChatWithGM::getBubbleByChatMessage(QADetail *detail)
{
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_chatLayer);
    
    //GM的id  0的时候代表玩家本人 非0为GM
    int gm_id=detail->getgmid(); //GM id
  
    CCString*string=detail->getcontent(); //回复内容
    CCString*string2=detail->gettime();   //回复时间
    

    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_chatwithgm);
    //头像
    CCSprite *sprite_head = CCSprite::createWithSpriteFrameName("head1001.png");
    //框
    CCSprite *kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");
    sprite_head->addChild(kuang);
    kuang->setAnchorPoint(ccp(0.5f, 0.5f));
    kuang->setPosition(ccp(sprite_head->getContentSize().width * 0.5f, sprite_head->getContentSize().height * 0.5f));
    //国家
    CCScale9Sprite *scalesprite_background=NULL;
    if(gm_id==0)
    {
        scalesprite_background = CCScale9Sprite::createWithSpriteFrameName("mail_messageBox_1.png");
    }
    else
    {
        scalesprite_background = CCScale9Sprite::createWithSpriteFrameName("mail_messageBox_2.png");
    }
    
    sprite_head->addChild(scalesprite_background, -1, 1111);
    
    CCSize scaleSize;
    if(string->length()>=string2->length())
    {
        scaleSize = getSizeByCharBuffer(string->getCString());
    }
    else
    {
        scaleSize = getSizeByCharBuffer(string2->getCString());
    }
    scalesprite_background->setPreferredSize(scaleSize);
    
    /* 角色的昵称 */
    SGCCLabelTTF *label_playername = SGCCLabelTTF::create("hahaha", FONT_PANGWA, 24, CCSizeMake(scaleSize.width-50, scaleSize.height-50));
    scalesprite_background->addChild(label_playername);
    
    /* 发言时间 */
    CCLabelTTF *label_time = CCLabelTTF::create(string2->getCString(), FONT_BOXINFO, 24);
    scalesprite_background->addChild(label_time);
    
    /* 聊天内容 */
    
    CCLabelTTF *label_content = CCLabelTTF::create(string->getCString(), FONT_BOXINFO, 24, CCSizeMake(scaleSize.width-50, scaleSize.height-50), kCCTextAlignmentLeft);
    scalesprite_background->addChild(label_content);
    label_content->setFontFillColor(ccBLACK);
    
    /* 检查是不是自己 */
    bool isme = (gm_id==0?true:false);
    
    if(gm_id)
    {      //////GM的信息
       
        sprite_head->setAnchorPoint(ccp(1, 0.5));
        //scalesprite_background->setPreferredSize(scaleSize);
        scalesprite_background->setAnchorPoint(ccp(1, 1));
        scalesprite_background->setPosition(ccp(-10, scalesprite_background->getContentSize().height));
        
        
        //昵称
        CCString*temp=CCString::createWithFormat(str_CardsLayer_str8,gm_id);
        label_playername->setString(temp->getCString());
        label_playername->setAnchorPoint(ccp(1, 1));
        label_playername->setPosition(ccp(scalesprite_background->getContentSize().width-20, scalesprite_background->getContentSize().height-12));
        //modify by:zyc. merge into create.
        label_playername->setInsideColor(ccc3(0xff, 0x95, 0x0c));
        /////回复时间
        label_time->setAnchorPoint(ccp(0.5, 0));
        label_time->setPosition(ccp(scalesprite_background->getContentSize().width*0.5, scalesprite_background->getContentSize().height));
        label_time->setColor(ccRED);
        
        label_content->setAnchorPoint(ccp(1, 1));
        label_content->setPosition(ccp(scalesprite_background->getContentSize().width-20, scalesprite_background->getContentSize().height-40));
        
    }
    else
    {    /////自己的信息
     
        
        int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        CCArray *arrayOfficer = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
        SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel());
            SGOfficerCard *card = NULL;
        for (int i = 0; i<player->getPlayerOfficerMax(); i++)
        {
        

            CCObject *obj = NULL;
            CCARRAY_FOREACH(arrayOfficer, obj)
            {
                SGOfficerCard  *pcard = (SGOfficerCard *)obj;
                int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(id,pcard);
                if ((po-1) == i)
                {
                    card = pcard;
                    break;
                }
            }
  
        }
        
        SGOfficerCard *lordOfficer = SGTeamgroup::shareTeamgroupInfo()->getlordofficer(SGTeamgroup::shareTeamgroupInfo()->getiscurrent());
        
        SGMainManager::shareMain()->addHeadIconPlistByNum(lordOfficer->getHeadSuffixNum() ,sg_chatwithgm);

        //////////////
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(lordOfficer->getItemId());
        CCString *kuangstr = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getOfficerVisitType());
        CCSprite*ssss=CCSprite::createWithSpriteFrameName(kuangstr->getCString());
        kuang->setDisplayFrame(ssss->displayFrame());
        
        ///////////////
        
          SGMainManager::shareMain() -> addHeadIconPlistByNum(temp->getIconId(),sg_chatwithgm);
        CCString*string111111= CCString::createWithFormat("head%d.png",temp->getIconId());
        CCSprite*s=CCSprite::createWithSpriteFrameName(string111111->getCString());
        sprite_head->setDisplayFrame(s->displayFrame());
        
        
        
        
        
        ////////////////////////////////////////////////////////////
        sprite_head->setAnchorPoint(ccp(0, 0.5));
        //scalesprite_background->setPreferredSize(scaleSize);
        scalesprite_background->setAnchorPoint(ccp(0, 1));
        scalesprite_background->setPosition(ccp(sprite_head->getContentSize().width+10, scalesprite_background->getContentSize().height));
        
        //CCString*temp=CCString::createWithFormat(playerInfo->getNickName());
        label_playername->setString(playerInfo->getNickName()->getCString());
        label_playername->setAnchorPoint(ccp(0, 1));
        label_playername->setPosition(ccp(30, scalesprite_background->getContentSize().height-10));
        //modify by:zyc. merge into create.
        label_playername->setInsideColor(ccWHITE);
        
        
        label_time->setAnchorPoint(ccp(0.5, 0));
        label_time->setPosition(ccp(scalesprite_background->getContentSize().width*0.5, scalesprite_background->getContentSize().height));
        label_time->setColor(ccRED);
        
        label_content->setAnchorPoint(ccp(1, 0.5));
        label_content->setPosition(ccp(scalesprite_background->getContentSize().width-20,label_content->getContentSize().height/2 + 2));

    }
    
    return sprite_head;
}


CCSize SGChatWithGM::getSizeByCharBuffer(const char *charBuffer)
{
    //设置scalesprite_background的大小，使其他所有的组件都可以放在之上
    CCSize scaleSize;
    //计算说话的总长度
    int allwidth = 0;
    for(int i=0;i<strlen(charBuffer);i++)
    {
        char ch = charBuffer[i];
        if(SGTools::charIsChinese(ch))
        {
            allwidth += SGTools::getWidthOfOneChineseWord(FONT_BOXINFO, 24);
        }else
        {
            allwidth += SGTools::getWidthOfOneEnglishWord(ch, FONT_BOXINFO, 24);
        }
    }
    
    //得出发言文字所占的区域
    if(allwidth > SGLayout::getSkewing(180))
    {
        scaleSize.width = SGLayout::getSkewing(180);
        scaleSize.height = (allwidth/SGLayout::getSkewing(180)+1)*SGTools::getHeightOfOneWord(FONT_BOXINFO, 24);
    }
    else
    {
        //最小也要设为50
        //       if(allwidth < 350)
        //        {
        //          allwidth = 350;
        //        }
        scaleSize.width = allwidth;
        scaleSize.height = SGTools::getHeightOfOneWord(str_Chat_str4, 24);
    }
    
    //加上人物昵称这一行的高度
    scaleSize.height += SGTools::getHeightOfOneWord(FONT_PANGWA, 24);
    
    //稍微扩充一下，让气泡变得大一点
    scaleSize.width += 80;
    scaleSize.height += 45;
    
    return scaleSize;
}

//设置scrollview
void SGChatWithGM::constructScrollView()
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
    
    CCArray *array =_array;
    
    SGMenu *menu = SGMenu::create(CCRectMake(0,SGLayout::getSkewing(70),SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-278));
    menu->setPosition(CCPointZero);
    menu->setTag(99999);
    int chatCount = array->count();
    for(int i=chatCount-1;i>=0;i--)
    {
       // SGChatMessage *chatMessage = (SGChatMessage*)array->objectAtIndex(i);
        ///////////////kanata
        QADetail* detail=(QADetail*)array->objectAtIndex(i);
        
        
        ///////////////
        CCSprite *sprite = getBubbleByChatMessage(detail);
        
        //注意，这里是作为container的CCLayer添加
      
        if(detail->getgmid()==0)
        {
        
            sprite->setPosition(ccp(SGLayout::getSkewing(10), heigth+10));
            sprite->setAnchorPoint(ccp(0, 0));
            
        }else{
            sprite->setPosition(ccp(SGLayout::getSkewing(310), heigth+10));
            sprite->setAnchorPoint(ccp(1, 0));
          
        }

        scrollViewContainer->addChild(sprite);
        
        arraySprite->addObject(sprite);
     //   arrayButon->addObject(button);
        
        //设置偏移量
        //int spriteHeight = 0;
        CCScale9Sprite *scaleSprite = (CCScale9Sprite*)sprite->getChildByTag(1111);
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
        scaleHeight = sprite_last->getChildByTag(1111)->getContentSize().height;
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
//        CCARRAY_FOREACH(arrayButon, object)
//        {
//            SGButton *sprite_button = (SGButton*)object;
//            sprite_button->setPositionY(sprite_button->getPositionY()+distance);
//        }
        
        //设置滑动区域为整个scrollview的高度
        scrollViewContainer->setContentSize(CCSizeMake(SGLayout::getSkewing(300),scrollviewHeight));
        scrollView->setContentSize(CCSizeMake(SGLayout::getSkewing(300), scrollviewHeight));
    }

    scrollViewContainer->addChild(menu);
    scrollView->setTouchEnabled(true);
    //设置方向，为纵向
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    
}

void SGChatWithGM::send(CCArray*arr)
{
    

    _array=NULL;
    _array=arr;
    constructScrollView();
}



void SGChatWithGM::buttonClickSend()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGChatWithGMInputBox *inputBox = SGChatWithGMInputBox::create(this,checkid,temptemp);
    SGMainManager::shareMain()->showBox(inputBox);
}


 void SGChatWithGM::buttonClickGoback()
{
    
    SGMainLayer *mainlayer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
//    SGButton*btn=(SGButton*)  mainlayer->getBtnByTag(54321);
//    btn->setEnabled(true);
    mainlayer->setIsCanTouch(true);
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    
    SGComplainLayer *layer = SGComplainLayer::create(playerInfo->getQAContent());
    SGMainManager::shareMain()->showLayer(layer);
    
    
    
   // SGMainManager::shareMain()->showFirstLayer();
    
}

/////////

void SGChatWithGM::setIsCanTouch(bool isTouch)
{
    //this->setIsCanTouch(isTouch);
    ((CCMenu *)scrollViewContainer->getChildByTag(99999))->setTouchEnabled(isTouch);
    menu->setTouchEnabled(isTouch);
    scrollView->setTouchEnabled(isTouch);
}

