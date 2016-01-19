//
//  SGFriendSubLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-27.
//
//

#include "SGFriendSubLayer.h"
#include "SGLayout.h"
#include "SGMyFriend.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGFriendSubLayer::SGFriendSubLayer() :
delegate(NULL)
{
    _friendType = 0;
}

SGFriendSubLayer::~SGFriendSubLayer()
{
 	ResourceManager::sharedInstance()->unBindLayerTexture(sg_friendLayer);
}

SGFriendSubLayer* SGFriendSubLayer::create(int friendType,SGFriendSubLayerDelegate*del)
{
    SGFriendSubLayer *friends = new SGFriendSubLayer();
    friends->_friendType = friendType;
    
    if(friends && friends->init())
    {
        friends->delegate=del;
        friends->constrctView();
        friends->autorelease();
        return friends;
    }
    
    CC_SAFE_DELETE(friends);
    return NULL;
}

void SGFriendSubLayer::constrctView()
{
	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_friendLayer);

    ////////////
    bgbgbg=NULL;
    
    SGButton*spriteBorder=SGButton::create("upwar_cell_bg.png", NULL, this, menu_selector(SGFriendSubLayer::buttonClick),CCPointZero,false,true);
//    CCMenu* menu=CCMenu::create();
//    this->addChild(menu, 1);
//    menu->setTouchPriority(10);
//    menu->setPosition(CCPointZero);
//    menu->addChild(spriteBorder);
    spriteBorder->setPosition(CCPointZero);
    
    this->addChild(spriteBorder);
    addBtn(spriteBorder);
    
    if(_friendType == SGFriendTypeFriendRequest
       ||_friendType == SGFriendTypeMyFriend
       ||_friendType == SGFriendTypeSendTiLi)
    {
        int friendTypeCount = 0;
        
        if (_friendType == SGFriendTypeFriendRequest)
        {
            friendTypeCount = SGPlayerInfo::sharePlayerInfo()->getApplyList()->count();
        }
        else if (_friendType == SGFriendTypeMyFriend)
        {
            friendTypeCount = SGPlayerInfo::sharePlayerInfo()->getFriendList()->count();
        }
        else if (_friendType == SGFriendTypeSendTiLi)
        {
            friendTypeCount = SGPlayerInfo::sharePlayerInfo()->getFriendPowerList()->count();
        }
        
        CCSprite *spriteRequestBG = CCSprite::createWithSpriteFrameName("friend_requestBG.png");
        spriteRequestBG->setPosition(ccpAdd(spriteBorder->getPosition(),ccp(spriteBorder->getContentSize().width*0.4, 0) ));
        this->addChild(spriteRequestBG, 2);
        CCString *strFriendApply = CCString::createWithFormat("%i",friendTypeCount);
		SGCCLabelTTF *labelRequestCount = SGCCLabelTTF::create(strFriendApply->getCString(), FONT_BOXINFO, FONTSIZE(18));
        labelRequestCount->setPosition(spriteRequestBG->getPosition());
        this->addChild(labelRequestCount, 2);
    }
    

    SGCCLabelTTF *spriteText1 = NULL;
    std::string nameString;
    
    switch (_friendType) {
        case SGFriendTypeMyFriend:
            nameString.append(str_FriendCountFull_str11);
            break;
        case SGFriendTypeSendTiLi:
            nameString.append(str_FriendSendTiLi_str1);
            break;
        case SGFriendTypeFriendRequest:
            nameString.append(str_SearchFriend_str10);
            break;
        case SGFriendTypeSearchFriend:
            nameString.append(str_SearchFriend_str1);
            break;
        case SGFriendTypeBlackFriend:
            nameString.append(str_BlackFriend_str2);
            break;
//        case SGFriendTypeInviteFriend:
//            nameString.append("邀请好友");
//            break;
        case SGFriendTypeExpandFriend:
            nameString.append(str_ExpandFriend_str1);
            break;
    }
    
    spriteText1 = SGCCLabelTTF::create(nameString.c_str(), FONT_PANGWA, FONTSIZE(18));
    spriteText1->setAnchorPoint(ccp(0, 0.5));
    this->addChild(spriteText1, 2);
    spriteText1->setPosition(ccp(-spriteBorder->getContentSize().width*.35, 0));

}
///////
void SGFriendSubLayer::buttonClick(CCObject* obj)
{
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(ind);

}



