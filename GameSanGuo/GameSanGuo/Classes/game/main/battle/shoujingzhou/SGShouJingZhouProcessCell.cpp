//
//  SGShouJingZhouProcessCell.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-14.
//
//

#include "SGShouJingZhouProcessCell.h"
#include "SimpleAudioEngine.h"
#include "GameMusicmanage.h"
#include "SGRichLabel.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"


SGShouJingZhouProcessCell::SGShouJingZhouProcessCell()
{
 
}
SGShouJingZhouProcessCell::~SGShouJingZhouProcessCell()
{
    
    
}


SGShouJingZhouProcessCell* SGShouJingZhouProcessCell::create(SGJingZhouProcessDelegate*del,SGShouJingZhouProcessData*data,int index)
{
    SGShouJingZhouProcessCell *cell = new SGShouJingZhouProcessCell();
    if (cell && cell->init()) {
        cell->delegate=del;
        cell->initView(data,index);
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
    
}
void SGShouJingZhouProcessCell::initView(SGShouJingZhouProcessData*data,int index)
{
    
    ResourceManager::sharedInstance()->bindTexture("sgshoujingzhou/sgshoujingzhou.plist", RES_TYPE_LAYER_UI, sg_jingzhouprocess);
    
    
    zhan=SGButton::create("fightnostar.png", NULL, this,menu_selector(SGShouJingZhouProcessCell::entrySectionHandler),CCPointZero,false,true );
    zhan->setPosition(ccp(200/*btn->getContentSize().width*0.5-zhan->getContentSize().width*0.78*/, 0));
    SNSTableViewCellItem::addBtn(zhan);
        btn=SGButton::create("item_bigbg.png", NULL, this,menu_selector(SGShouJingZhouProcessCell::showSectionInfoHandler),CCPointZero,false,true );
    this->addChild(btn);
    SNSTableViewCellItem::addBtn(btn);
    this->addChild(zhan);
    zhan->setTag(index);
    btn->setTag(index);
    
    
    bgSize = btn->getContentSize();
    
    
    CCString * words=CCString::createWithFormat(str_ShouJingZhou_str16, data->getlevel());
    
    CCSprite* info = SGRichLabel::getFomLabel(words->getCString(), CCSizeMake(400,100),FONT_PANGWA, 32,ccc3(0xff, 0xc0, 0x00));
    info->setAnchorPoint(ccp(0, 0.5));
    info->setPosition(ccp(-200, -50));
    this->addChild(info);

}

void SGShouJingZhouProcessCell::showSectionInfoHandler(CCNode*node)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int tag=node->getTag();
    if (delegate) {
        delegate->showSectionInfoHandler(this,tag);
    }
    
}
void SGShouJingZhouProcessCell::entrySectionHandler(CCNode*node)
{
    EFFECT_PLAY(MUSIC_BTN);
    int tag=node->getTag();
    if (delegate) {
        delegate->entrySectionHandler(this,tag);
    }
}





