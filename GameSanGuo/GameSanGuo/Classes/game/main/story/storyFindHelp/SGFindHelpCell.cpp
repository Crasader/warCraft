//
//  SGFindHelpCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#include "SGFindHelpCell.h"
#include "GameConfig.h"
#include "SGLayout.h"
#include "SGStoryRewardBox.h"
#include "SGStaticDataManager.h"
#include "SGChat.h"
#include "SGGeneralInfoLayer.h"
#include "SGFindHelpLayer.h"
#include "ResourceManager.h"
#include "SGButton.h"
#include "SGStringConfig.h"

SGFindHelpCell::SGFindHelpCell(void):_player(NULL)
,menu1(NULL)
,itemHeadPic1(NULL)
,roleID(0)
,spriteIcon(NULL)
,spriteIconBg(NULL)
//,spriteFriType(NULL)
,labelHLevel(NULL)
,labelCLevel(NULL)
,labelFriName(NULL)
,_isTouch(false)
,spriteStar(NULL)
,delegate(NULL)
{

}
SGFindHelpCell::~SGFindHelpCell(void)
{

}

SGFindHelpCell* SGFindHelpCell::create( Itembtndelegate *del)
{
    SGFindHelpCell *helpCell = new SGFindHelpCell();
    
    if (helpCell && helpCell->init()) {
        helpCell->delegate = del;
        helpCell->viewDidLoad();
        helpCell->autorelease();
        return helpCell;
    }
    CC_SAFE_DELETE(helpCell);
    return NULL;
}

void SGFindHelpCell::viewDidLoad()
{
    _isTouch = true;

    ///////////////////
     SGButton *spriteBorder = SGButton::create("item_smallbg.png",NULL, this, menu_selector(SGFindHelpCell::buttonClickOK),CCPointZero,false,true);

    bgbgbg = NULL;
    SNSTableViewCellItem::addBtn(spriteBorder);
    this->addChild(spriteBorder,-10,ITEMBIGBGTAG);

    
    bgSize = spriteBorder->getContentSize();
    
    float dx = skewing(93/2)/2;
    float posX1 = -bgSize.width/2+ZoomInX(53)-dx;
    float posY1 = ZoomInX(9);
    float posX2 = ZoomInX(35);
    float posY2 = ZoomInX(-5);
    float fontSize = FONTSIZE(12);
    const char *fontName = FONT_BOXINFO;
    
    menu1 = CCMenu::create();
    itemHeadPic1 = SGButton::create("jinkuang.png","jinkuang.png",this, menu_selector(SGFindHelpCell::buttonClickHeadPic),ccp(-5, -5),false,true);

        itemHeadPic1->setAnchorPoint(ccp(1.0, 0.5));
        menu1 -> addChild(itemHeadPic1,10);
        menu1 -> setPosition(CCPointZero);
        this -> addChild(menu1);
        spriteBorder->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(itemHeadPic1->getContentSize().width/2, 0)));
        itemHeadPic1->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(-spriteBorder->getContentSize().width * 0.475, 4)));
        
        labelCLevel = SGCCLabelTTF::create("LV", FONT_PANGWA, FONTSIZE(12));
        labelCLevel->setAnchorPoint(ccp(0.5f, 0.0f));
        labelCLevel->setPosition(ccp(itemHeadPic1->getPositionX(), itemHeadPic1->getPositionY()-itemHeadPic1->getContentSize().height/2));
        this->addChild(labelCLevel);
        labelCLevel->setVisible(false);
        
        labelHLevel = SGCCLabelTTF::create(str_OverBattleBox_str21, fontName, fontSize);
        labelHLevel->setAnchorPoint(ccp(0, 0.5f));
        labelHLevel->setPosition(ccp(posX2, posY1));
        this->addChild(labelHLevel);
        
        labelFriName = SGCCLabelTTF::create(str_FriendBase_str16, fontName, fontSize);
        labelFriName->setAnchorPoint(ccp(0, 0.5f));
        labelFriName->setPosition(ccp(posX1, posY1));
        this->addChild(labelFriName);

}

///////////////////////////////////
void SGFindHelpCell::buttonClickOK(CCObject *sender)//cgp_pro
{
    if (NULL == itemHeadPic1)
    {
        return;
    }
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(ind);
}
///////////////////////
// reset all the cell's infomation
void SGFindHelpCell::setData(SGPveHelper* hero)
{
    if (NULL == itemHeadPic1) {
        return;
    }
    labelFriName->setString(hero->getRoleName()->getCString());
    
    CCString *levStr = CCString::createWithFormat("LV%d", hero->getItemLevel());
    labelCLevel->setString(levStr->getCString());
    
    CCString *levelDesc = CCString::createWithFormat(str_OverBattleBox_str23, hero->getRoleLevel());
    labelHLevel->setString(levelDesc->getCString());
    roleID = hero->getRoleId();
	//12.06
	SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(hero->getItemId());
	
    SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(),sg_findHelplayer);
	CCString *hIcon = CCString::createWithFormat("head%d.png", temp->getIconId());//12.06
    
    SGOfficerDataModel *model = SGStaticDataManager::shareStatic()->getOfficerById(hero->getItemId());
    int starlvl = model->getOfficerCurrStarLevel();
    if (starlvl>1) {
        starlvl-=1;
    }

    itemHeadPic1 -> setImage(hIcon->getCString());//itemHeadPic1
    
    CCString *cIcon = CCString::createWithFormat("officerPinzhi_%d_0.png", starlvl);
    CCSprite * iconName = CCSprite::createWithSpriteFrameName(cIcon -> getCString());
    if(iconName)
    itemHeadPic1 -> setFontImage(iconName -> displayFrame());
    itemHeadPic1 -> setTag(roleID);
    
//    CCLabelTTF *levelt = CCLabelTTF::create(CCString::createWithFormat("LV%d",hero->getItemLevel())->getCString(), FONT_PANGWA, 20);
    SGCCLabelTTF *levelt = SGCCLabelTTF::create(CCString::createWithFormat("LV%d",hero->getItemLevel())->getCString(), FONT_PANGWA, 20 , ccWHITE , ccBLACK);
    //modify by:zyc  merge into create.
    //levelt->setOutSideColor(ccBLACK);
    itemHeadPic1->addChild(levelt);
//    levelt->setColor(ccBLACK);
    levelt->setPosition(ccp( itemHeadPic1->getContentSize().width * 0.5, levelt->getContentSize().height * 0.6));
    
    
    
    
//  spriteIconBg->setDisplayFrame(CCSprite::createWithSpriteFrameName(cIcon->getCString())->displayFrame());
//    CCArray *array = SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
//    SGChat *sgChat = SGChat::create(array);

    //if (str_1->isEqual(CCString::create("SGOfficerCard")))
    {
        itemHeadPic1->removeChildByTag(30, true);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",model->getOfficerRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(iconName->getContentSize().width*.2,iconName->getContentSize().height - countryName->getContentSize().height*1.9));
        countryName->setVisible(false);
        itemHeadPic1->addChild(countryName,30,30);
        
        itemHeadPic1->removeChildByTag(250, true);
        if (temp->getAdvNum()>0)
        {
//            CCSprite *advancenumber = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("+%d.png",model->getAdvNum()))->getCString());
            SGCCLabelTTF * advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d", model->getAdvNum())->getCString(), FONT_PANGWA, 24, ccGREEN);

            advancenumber->setPosition(ccpAdd(itemHeadPic1->getContentSize(),ccp(-advancenumber->getContentSize().width + 8, -advancenumber->getContentSize().height + 8)  ));
            itemHeadPic1->addChild(advancenumber,50,250);
        }

    }
    
    
//    CCString *friType = CCString::createWithFormat("font_friArmy%d.png", hero->getRoleType());
//    CCSprite *_s = CCSprite::createWithSpriteFrameName(friType->getCString());
//    spriteFriType->setDisplayFrame(_s->displayFrame());

     
    //统一修改为3星
//    this->drawStars(hero->getItemStar());
    this->drawStars(3);
}

void SGFindHelpCell::drawStars(int count)
{
    if (NULL == itemHeadPic1) {
        return;
    }
    if (spriteStar) {
//        spriteStar->removeAllChildrenWithCleanup(true);
        spriteStar->removeFromParentAndCleanup(true);
    }
    float dx = skewing(93/2)/2;
    spriteStar = spriteWithStars(count);
    spriteStar->setAnchorPoint(ccp(0, 0.5f));
    spriteStar->setPosition(ccp(-bgSize.width/2+ZoomInX(52)-dx, ZoomInX(-5)));
    this->addChild(spriteStar);
    
}

void SGFindHelpCell::buttonClickHeadPic()
{
    if (NULL == itemHeadPic1) {
        return;
    }
    EFFECT_PLAY(MUSIC_BTN);
    this -> setIsCanTouch(false);
    main::FriendInfoRequest *info = new main::FriendInfoRequest();

    info->set_friendid(roleID);

    CCLOG("玩家信息：SGFindHelpCell");
    SGFindHelpLayer *findHelpLayer=(SGFindHelpLayer *)this->getParent();
    findHelpLayer->setRoleID(roleID);
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);

}

void SGFindHelpCell::setIsCanTouch(bool isTouch)
{
    if (NULL == itemHeadPic1) {
        return;
    }
    menu1 -> setTouchEnabled(isTouch);
    menu1 -> setEnabled(isTouch);
    
}

