//
//  SGHeadIconItem.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-12-30.
//
//

#include "SGHeadIconItem.h"
#include "AppMacros.h"

SGHeadIconItem ::SGHeadIconItem() : label_count(NULL)
{

    label_name=NULL;
    kuang_sprite=NULL;
    main_sprite=NULL;
    country_sprite=NULL;
}
SGHeadIconItem ::~SGHeadIconItem()
{
    
   
}

SGHeadIconItem * SGHeadIconItem::create()
{
    SGHeadIconItem *cell=new SGHeadIconItem();
    if (cell && cell->init()) {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

void SGHeadIconItem::initView(HeadInfo headInfo)
{

     
    main_sprite=CCSprite::createWithSpriteFrameName(headInfo.fileName);
    if (main_sprite)
    addChild(main_sprite);
    
    kuang_sprite=CCSprite::createWithSpriteFrameName(headInfo.kuangName);
    addChild(kuang_sprite);
    
    label_name=SGCCLabelTTF::create(headInfo.reward_name, headInfo.fontName, 18, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    addChild(label_name);
    
    //CCLOG("%s",headInfo.reward_name);
    
    //做一个改动，如果数量为0，不显示X
    if (headInfo.reward_count != NULL)
    {
        label_count=SGCCLabelTTF::create(headInfo.reward_count, headInfo.fontName, 24, CCSizeZero, kCCTextAlignmentCenter,
                                         kCCVerticalTextAlignmentCenter , ccc3(29, 225, 34));

        addChild(label_count);
        label_count->setZOrder(20);
    }
    
    main_sprite->setAnchorPoint(ccp(0, 0.5));
    main_sprite->setPosition(ccp(-SGLayout::getSkewing(25), 15));
    
    CCSprite *mainbg = CCSprite::createWithSpriteFrameName("card_bj.png");
    mainbg->setPosition(ccpAdd(main_sprite->getPosition(), ccp(main_sprite->getContentSize().width*.5,0)));
    this->addChild(mainbg,-1);
    
    
    kuang_sprite->setPosition(ccpAdd(main_sprite->getPosition(), ccp(main_sprite->getContentSize().width/2,0)));
    
    label_name->setAnchorPoint(ccp(0, 0.5));
    label_name->setPosition(ccpAdd(mainbg->getPosition(), ccp(-label_name->getContentSize().width/2+5, -mainbg->getContentSize().height/2-24)));
//    label_name->setColor(ccc3(255, 184, 0));
    CCSprite *namebg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
    namebg->setPosition(ccpAdd(label_name->getPosition(),ccp(label_name->getContentSize().width*.5,0)));
    namebg->setScaleX(1.1);
    namebg->setScaleY(1.05);
    this->addChild(namebg,-1);
    
    if (headInfo.reward_count != NULL)
    {
        label_count->setAnchorPoint(ccp(0, 0.5));
        label_count->setPosition(ccpAdd(main_sprite->getPosition(), ccp(main_sprite->getContentSize().width/2-label_count->getContentSize().width/2,
                                                                        -main_sprite->getContentSize().height * 0.5 + 12)));
    }
    
    if (headInfo.advanceNum > 0)
    {
        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",headInfo.advanceNum)->getCString(), FONT_PANGWA, 24);
        advNum->setInsideColor(ccGREEN);
        advNum->setAnchorPoint(ccp(1,1));
        advNum->ignoreAnchorPointForPosition(false);
        advNum->setPosition(ccp(main_sprite->getContentSize().width * 1.2,main_sprite->getContentSize().height * 1.2));
        main_sprite->addChild(advNum,40,40);
    }
    
    switch (headInfo.type) {
        case officerCard:
        {
            country_sprite=CCSprite::createWithSpriteFrameName(headInfo.countryName);
            addChild(country_sprite);
            country_sprite->setPosition(ccpAdd(main_sprite->getPosition(), ccp(country_sprite->getContentSize().width*0.65, main_sprite->getContentSize().height/2-country_sprite->getContentSize().height/2)));
        }
            break;
            
        case pieceOfficeCard:
        {
            country_sprite=CCSprite::createWithSpriteFrameName(headInfo.countryName);
            addChild(country_sprite);
            country_sprite->setPosition(ccpAdd(main_sprite->getPosition(), ccp(country_sprite->getContentSize().width*0.65, main_sprite->getContentSize().height/2-country_sprite->getContentSize().height/2)));
            
            CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
            addChild(headMask,10);
			// 修正蒙版偏移
			int shiftHgt = -2;
			if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height
                ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
			{
				shiftHgt = -10;
			}
			headMask->setPosition(ccpAdd(headMask->getPosition(), ccp(shiftHgt, 2)));
            headMask->setAnchorPoint(ccp(0.52, 0.38));
            
        }
            break;
        case pieceEquipcard:
        {
            
            CCSprite *headMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
            addChild(headMask,10);
            // 修正蒙版偏移
			int shiftHgt = -1;
			if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height
                ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
			{
				shiftHgt = -10;
			}
            headMask->setPosition(ccpAdd(headMask->getPosition(), ccp(shiftHgt, 2)));
            headMask->setAnchorPoint(ccp(0.53, 0.38));

        }
            break;
        default:
            break;
    }

}