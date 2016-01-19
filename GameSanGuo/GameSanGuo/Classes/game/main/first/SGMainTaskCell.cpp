//
//  SGMainTaskCell.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-14.
//
//

#include "SGMainTaskCell.h"

SGMainTaskCell::SGMainTaskCell()
:
titleLabel(NULL),
noticeSprite(NULL),
bgSize(CCSizeZero)
{
    
}
SGMainTaskCell::~SGMainTaskCell()
{
    
}

SGMainTaskCell *  SGMainTaskCell::create(SGMainTaskLayerDelegate *_delegate){

    SGMainTaskCell *cell=new SGMainTaskCell();
    cell->delegate=_delegate;
    if (cell && cell->init()) {
        cell->viewDidLoad();
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return cell;
    
}
 
void SGMainTaskCell::viewDidLoad(){

   SGButton *bigBGBtn = SGButton::create("upwar_cell_bg.png",
                                NULL,
                                this,
                                menu_selector(SGMainTaskCell::showSectionInfoHandler),
                                ccp(0, 0),
                                false,
                                true);
    bigBGBtn->setAnchorPoint(ccp(0.5,0.5));
    bigBGBtn->ignoreAnchorPointForPosition(false);
    bigBGBtn->setPosition(ccp(0, 0));
//    bigBGBtn->setContentSize(CCSizeMake(bigBGBtn->getContentSize().width- backBtn->getContentSize().width*1.2, bigBGBtn->getContentSize().height));
    addBtn(bigBGBtn);
    this->addChild(bigBGBtn);
    
    bgSize=bigBGBtn->getContentSize();
    
    titleLabel = SGCCLabelTTF::create("", FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//titleLabel->setColor(ccc3(0xff, 0x95, 0x0c));
    titleLabel->setPosition(ccpAdd(bigBGBtn->getPosition(), ccp(0, 0)));
    this->addChild(titleLabel);
    
    noticeSprite=CCSprite::createWithSpriteFrameName("publc_notice.png");
    this->addChild(noticeSprite);
    noticeSprite->setPosition(ccpAdd(bigBGBtn->getPosition(), ccp(bigBGBtn->getContentSize().width/2-noticeSprite->getContentSize().width, 0)));
    noticeSprite->setVisible(false);
}

void SGMainTaskCell::showSectionInfoHandler(){
    EFFECT_PLAY(MUSIC_BTN);
    if (delegate) {
        delegate->showSectionInfoHandler(this);
    }
}
void SGMainTaskCell::updateInfo(CCString *titleName,bool isVisible){
    titleLabel->setString(titleName->getCString());
    noticeSprite->setVisible(isVisible);
}
void SGMainTaskCell::updateNotice(bool isVisible)
{
    noticeSprite->setVisible(isVisible);
}
