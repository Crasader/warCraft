//
//  SGNikeNameBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#include "SGNikeNameBox.h"
#include "SGSocketClient.h"
#include "RoleModule.pb.h"
#include "SGMsgId.h"
#include "SGStaticDataManager.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGBaseBox.h"
#include "SGStringConfig.h"
 
SGNikeNameBox *SGNikeNameBox::create(SGBoxDelegate *dg, int countryId_)
{
    SGNikeNameBox *nikeNameBox = new SGNikeNameBox();
    if (nikeNameBox && nikeNameBox->init(dg, box_nikeNameBox, CCSizeMake(480,220),false,false))
    {
        nikeNameBox->countryId = countryId_;
        nikeNameBox->initView();
        nikeNameBox->autorelease();
        return nikeNameBox;
    }
    CC_SAFE_DELETE(nikeNameBox);
    return NULL;
}

SGNikeNameBox::SGNikeNameBox()
:countryId(0),
m_pEditName(NULL),
m_pEditNum(NULL)
{
    
}

SGNikeNameBox::~SGNikeNameBox(){
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_nickNameBox);

}

void SGNikeNameBox::initView()
{
 
     ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_nickNameBox);
     ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_nickNameBox, LIM_PNG_AS_PNG);
     ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_nickNameBox);
    
    CCPoint center = ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 50));
    CCSize size = CCSize(580, 474);
    //黑底
    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 220), skewing(320)*3, skewingY(960)*3);
    this->addChild(bgLayer,-3);
    bgLayer->setPosition(ccpAdd(CCPointZero, ccp(-bgLayer->getContentSize().width*.5, -bgLayer->getContentSize().height*.5)));
    
//边框 背景 标题线
    CCSprite* biao = CCSprite::createWithSpriteFrameName("box_bian.png");
//    this->addChild(biao,1);
    biao->setPosition(ccpAdd(center, ccp(0, size.height*.5 -55)));
    
    CCSprite *line = CCSprite::createWithSpriteFrameName("login_name_line.png");
    this->addChild(line,1);
    line->setPosition(ccpAdd(biao->getPosition(), ccp(0, -40)));
    
    cocos2d::extension::CCScale9Sprite *frame = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("box_kuang.png");
    this->addChild(frame,1);
    
    frame->setPreferredSize(CCSizeMake(size.width, size.height));
    frame->setPosition(center);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/box_bg.plist", RES_TYPE_LAYER_UI, sg_nickNameBox);

    CCSprite *spriteBigBG = CCSprite::createWithSpriteFrameName("box_bg.png");
    CCRect r = CCRectMake(0, 0, size.width,size.height);
    spriteBigBG->setTextureRect(r);
    // spriteBigBG->cocos2d::CCNode::setScale(r.size.width / spriteBigBG->getContentSize().width, r.size.height / spriteBigBG->getContentSize().height);
    this->addChild(spriteBigBG);
    spriteBigBG->setPosition(center);
    
    CCSprite *font_name = CCSprite::createWithSpriteFrameName("login_font_name.png");
    this->addChild(font_name);
    font_name->setAnchorPoint(ccp(0, 0.5));
    font_name->setPosition(ccpAdd(center, ccp(-233, 100)));
    
//    CCSprite *font_num = CCSprite::createWithSpriteFrameName("login_font_invatenum.png");
//    this->addChild(font_num);
//    font_num->setAnchorPoint(ccp(0, 0.5));
//    font_num->setPosition(ccpAdd(center, ccp(-233,3)));
    
    
//    CCLabelTTF *label_1 = CCLabelTTF::create("现在，给自己起一个牛X的名字吧：", FONT_BOXINFO, 22);
    
    SGCCLabelTTF *label_2 = SGCCLabelTTF::create(str_NikeNameBox_str1, FONT_BOXINFO, 30 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//label_2->setColor(ccc3(0xff, 0x95, 0x0c));
    CCSprite *spriteNike = CCSprite::createWithSpriteFrameName("country_artWord2.png");
    
    this->addChild(label_2,5);
    this->addChild(spriteNike,2);
    label_2->setPosition(ccpAdd(center, ccp(-25, -42)));
    spriteNike->setPosition(biao->getPosition());
    
    m_pEditName = CCEditBox::create(CCSizeMake(font_name->getContentSize().width, 64), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setMaxLength(16);
    
  

    m_pEditName->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    m_pEditName->setPosition(ccpAdd(center,ccp(-40, 37)));
    
    
    SGButton *buttonOK = SGButton::createFromLocal("box_btnbg.png", str_NikeNameBox_str6, this,
                                          menu_selector(SGNikeNameBox::confirmHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    buttonOK->setPosition(ccpAdd(center, ccp(-buttonOK->getContentSize().width * 0.7, -320/2)));
    addBtn(buttonOK);
    
    //调整，这里加入重选国家按钮
    SGButton *changeCountryBtn = SGButton::createFromLocal("box_btnbg.png", str_NikeNameBox_str7, this,
                                                           menu_selector(SGNikeNameBox::changeCountryHandler),ccp(0,0),FONT_PANGWA,ccWHITE,32);
    changeCountryBtn->setPosition(ccpAdd(center, ccp(changeCountryBtn->getContentSize().width * 0.7, -320/2)));
    this->addBtn(changeCountryBtn);

    //摇色子
    SGButton *buttonRandNickName = SGButton::createFromLocal("randNickName.png", "", this, menu_selector(SGNikeNameBox::buttonClickNickName));
    buttonRandNickName->setAnchorPoint(ccp(0, 0.5));
    buttonRandNickName->setPosition(ccpAdd(center, ccp(185, 37)));
    addBtn(buttonRandNickName);
    
    //先随机起一个名字,显示在输入框中
    buttonClickNickName();
   
    //新手引导  发送  输入昵称 的消息。
    SGMainManager::sendOperatorRecord(100080);
}
void SGNikeNameBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    if(!m_pEditName->getText())
    {
//        label_Error->setVisible(true);
        SG_SHOW_WINDOW(str_NikeNameBox_str2);
        return;
    }
    
    char bufferStr[1024];
    const char *buffer = m_pEditName->getText();
    strcpy(bufferStr, buffer);
    
    //神一样的检查字数
    
    if (GameConfig::isContainsEmoji(bufferStr)) {
        SG_SHOW_WINDOW(str_NikeNameBox_str3);
        return;
    }
    
    if(utf8StrLength(bufferStr) < 2 || utf8StrLength(bufferStr) > 8 )
    {
        //label_Error->setVisible(true);
        SG_SHOW_WINDOW(str_NikeNameBox_str4);
        return;
    }
    std::string token = CCUserDefault::sharedUserDefault()->getStringForKey("deviceToken");

    main::CreateRoleRequest *request = new main::CreateRoleRequest();
    request->set_country(countryId);
    std::string nameTmp(m_pEditName->getText());
    request->set_nickname(SGNikeNameBox::Trim(nameTmp));
    SGSocketClient::sharedSocketClient()->send(MSG_MAIN_CREATEROLE, request);
}

void SGNikeNameBox::changeCountryHandler(CCObject *obj)
{
    //关闭box
    this->boxCloseWithOutCallBack();
    SGMainManager::shareMain()->getMainScene()->showCountryLayer();
}

#pragma mark CCEditBOxDelegate

void SGNikeNameBox::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void SGNikeNameBox::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void SGNikeNameBox::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
     
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    
}

void SGNikeNameBox::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p was returned !");
    
}

/**
 *  计算字符个数
 *  type: 0、计算字符个数 ，1、返回字节Id ，2、返回字符id ，3、是否为标点
 */

//计算字符串长度

std::string LTrim(const std::string& str)
{
    return str.substr(str.find_first_not_of(" \n\r\t"));
}

std::string RTrim(const std::string& str)
{
//    str=str.substr(0,str.find_last_not_of("xf0\x9f\x98\x84")+1));
    return str.substr(0,str.find_last_not_of(" \n\r\t")+1);
}

std::string SGNikeNameBox::Trim(const std::string& str)
{
    return LTrim(RTrim(str));
}//不能都是空格


//随机起名字
void SGNikeNameBox::buttonClickNickName()
{
    EFFECT_PLAY(MUSIC_BTN);
    const char * nickName = SGStaticDataManager::shareStatic()->getRandomNickName().c_str();
    if(strlen(nickName)==0)
    {
        printf(str_NikeNameBox_str5);
        return;
    }
    m_pEditName->setText(nickName);
}

int SGNikeNameBox::utf8StrLength(const char* utf8)
{
    std::string tempStr(utf8);
    int utf8Leng = tempStr.length();
    int i = 0, count = 0;
    unsigned char firstCh;
    int afters;
    while(i < utf8Leng)
    {
        firstCh = utf8[i];
        //Ascii码大于0xC0才需要向后判断，否则，就肯定是单个ANSI字符了
        if (firstCh >= 0xC0)
        {
            //根据首字符的高位判断这是几个字母的UTF8编码
            if ((firstCh & 0xE0) == 0xC0)
            {
                afters = 2;
            }
            else if ((firstCh & 0xF0) == 0xE0)
            {
                afters = 3;
            }
            else if ((firstCh & 0xF8) == 0xF0)
            {
                afters = 4;
            }
            else if ((firstCh & 0xFC) == 0xF8)
            {
                afters = 5;
            }
            else if ((firstCh & 0xFE) == 0xFC)
            {
                afters = 6;
            }
            else
            {
                afters = 1;
            }
            
            //知道了字节数量之后，还需要向后检查一下，如果检查失败，就简单的认为此UTF8编码有问题，或者不是UTF8编码，于是当成一个ANSI来返回处理
            for(int k = 1; k < afters; ++ k)
            {
                if ((utf8[k] & 0xC0) != 0x80)
                {
                    //判断失败，不符合UTF8编码的规则，直接当成一个ANSI字符返回
                    afters = 1;
                    break;
                }
            }
        }
        else
        {
            afters = 1;
        }
        count++;
        i += afters;
    }
    return count;
 }

