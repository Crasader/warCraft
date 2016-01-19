//
//  SGMailCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-26.
//
//

#include "SGMailCell.h"
#include "SGLayout.h"
#include "SGPlayerInfo.h"
#include "SGMailDataModel.h"
#include "SGMailSystemDataModel.h"
#include "SGCCLabelTTF.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGMailCell::SGMailCell() : tableview(NULL)
{
    _index = 0;
    spriteRequestBG = NULL;
    labelRequestCount = NULL;
}

SGMailCell::~SGMailCell()
{
}

SGMailCell* SGMailCell::create(int index,SGMailCellDelegate*del,SNSTableView*table)
{
    SGMailCell *cell = new SGMailCell();
    cell->_index = index;
    if(cell && cell->init())
    {
        cell->tableview=table;
        cell->delegate=del;
        cell->viewDidLoad();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;

}

void SGMailCell::viewDidLoad()
{
	ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_mailLayer);

	
    //边框
//    CCScale9Sprite *spriteBorder = CCScale9Sprite::createWithSpriteFrameName("item_bigbg.png");
//    spriteBorder->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 102));
//    this->addChild(spriteBorder);
//    bgbgbg = (CCSprite*)spriteBorder;
    SGButton*btn=SGButton::create("item_bigbg.png", NULL, this, menu_selector(SGMailCell::buttonClick),CCPointZero,false,true);
    btn->setScaleX(1.05);
   

    this->addChild(btn);
    addBtn(btn);

    //文字
    SGCCLabelTTF *spriteText = NULL;
    std::string subTitle;
    switch(_index)
    {
        case MAIL_FRIEND:
        {
            subTitle.append(str_SearchFriend_str14);
            break;
        }
        case MAIL_SYSTEM:
        {
            subTitle.append(str_SystemMailLayer_str1);
            break;
        }
        case MAIL_GM:
        {
            subTitle.append(str_SystemMailLayer_str5);
            break;
        }
    }
    spriteText = SGCCLabelTTF::create(subTitle.c_str(), FONT_PANGWA, FONTSIZE(18));
    spriteText->setAnchorPoint(ccp(0, 0.5));
    spriteText->setPosition(ccp(-SGLayout::getSkewing(100), 0));
    addChild(spriteText);
    
    //显示消息数量    
    spriteRequestBG = CCSprite::createWithSpriteFrameName("friend_requestBG.png");
    spriteRequestBG->setPosition(ccp(SGLayout::getSkewing(100), 0));
    this->addChild(spriteRequestBG);
    
    labelRequestCount = CCLabelAtlas::create("0", "sanguobigpic/friend_requestNumber.png", 12, 20,'0');
    labelRequestCount->setAnchorPoint(ccp(0.5, 0.5));
    labelRequestCount->setPosition(spriteRequestBG->getPosition());
    this->addChild(labelRequestCount);
    
    //设置消息数量，控制这消息的显示和隐藏
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int friendUnRead = 0;
    int systemUnRead = 0;
    int GMUnRead = 0;
    switch(_index)
    {
        case MAIL_FRIEND:
        {
            for(int i=0;i<playerInfo->getFriendMails()->count();i++)
            {
                SGMailDataModel *mailData = (SGMailDataModel*)playerInfo->getFriendMails()->objectAtIndex(i);
                if(!mailData->getMailIsRead())
                {
                    friendUnRead ++;
                }
            }
            
            labelRequestCount->setString(CCString::createWithFormat("%d",friendUnRead)->getCString());
            break;
        }
            
        case MAIL_SYSTEM:
        {
            int systemCount = playerInfo->getSystemMails()->count();
            for(int i=0;i<systemCount;i++)
            {
                SGMailSystemDataModel *mailData = (SGMailSystemDataModel*)playerInfo->getSystemMails()->objectAtIndex(i);
                if(!mailData->getIsRead())
                {
                    systemUnRead ++;
                }
            }
            
            labelRequestCount->setString(CCString::createWithFormat("%d",systemUnRead)->getCString());
            break;
        }
            
        case MAIL_GM:
        {
            for(int i=0;i<playerInfo->getGMMails()->count();i++)
            {
                SGMailDataModel *mailData = (SGMailDataModel*)playerInfo->getGMMails()->objectAtIndex(i);
                if(!mailData->getMailIsRead())
                {
                    GMUnRead ++;
                }
            }
            
            labelRequestCount->setString(CCString::createWithFormat("%d",GMUnRead)->getCString());
            break;
        }
    }
}


//////////
void SGMailCell::buttonClick(CCObject*obj)
{
    CCLOG("SGMailCell::buttonClick(CCObject*obj) begin");
    SNSIndexPath *indexpath = this->getIndexPath();
    SNSIndexPath *ind = SNSIndexPath::create(indexpath->getRow(), indexpath->getColumn(), 0);
    delegate->itemselect(tableview,ind);
    CCLOG("SGMailCell::buttonClick(CCObject*obj) end");
}


