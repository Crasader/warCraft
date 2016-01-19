
//
//  SGActivitySubLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-19.
//
//

#include "SGActivitySubLayer.h"
#include "SGGeneralInfoLayer.h"
#include "SGMainManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGCCLabelTTF.h"
#include "SGStaticDataManager.h"
#include "SGArrangeLayer.h"
#include "SGGuideManager.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "CCScrollView.h"

SGActivitySubLayer::SGActivitySubLayer():
countStateImage(NULL),
enterType(0),
str(""),
isMove(false),
item(NULL),
checkCountState(0),
me_nu(NULL),
iscantouch(false)

{

}

SGActivitySubLayer::~SGActivitySubLayer()
{

}

SGActivitySubLayer* SGActivitySubLayer::create(const std::string& name,CCPoint position, CCSize viewSize,int type,SGActivitySubItem *it,int checkTimeState)
{
    SGActivitySubLayer *sublayer = new SGActivitySubLayer();
    sublayer->_viewSize = viewSize;
    sublayer->enterType = type;
    sublayer->str = name;
    sublayer->item = it;
    sublayer->checkCountState = checkTimeState;
//    sublayer->_officerCard = officerCard;
//    sublayer->index = position.x;
//    if (officerCard) {
//        sublayer->_officerCard->retain();
//    }
    
    if(sublayer && sublayer->init(position,viewSize))
    {
        sublayer->constructView();
        sublayer->autorelease();
        return sublayer;
    }
    
    CC_SAFE_DELETE(sublayer);
    return NULL;
}

void SGActivitySubLayer::constructView()
{
    SGButton *btn = SGButton::create(str.c_str(), NULL, this,menu_selector(SGActivitySubLayer::buttonClickOK),CCPointZero,false,true);
    me_nu = CCMenu::create(btn,NULL);
    CCSprite *spr = CCSprite::createWithSpriteFrameName("equipPinzhi_1_3.png");//guangbiao.png
    spr->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
    this->addChild(spr,1,100);
    if (enterType != 0) {
        spr->setVisible(false);
    }
    this->addChild(me_nu,0,enterType);
    me_nu->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
    
    updateCheckState(checkCountState);

}
void SGActivitySubLayer::updateCheckState(int state)
{
    if (state == 0)
    {
        if (countStateImage)
        {
            countStateImage->setVisible(false);
        }
        return;
    }
    std::string imageName = state == 1 ? "kaifujinxing.png": "kaifujieshu.png";
    
    if (countStateImage)
    {
        countStateImage->setVisible(true);
        countStateImage->setDisplayFrame(CCSprite::createWithSpriteFrameName(imageName.c_str())->displayFrame());
    }
    else
    {
        countStateImage = CCSprite::createWithSpriteFrameName(imageName.c_str());
       
        countStateImage->setPosition(ccp(_viewSize.width - countStateImage->getContentSize().width/2, _viewSize.height - countStateImage->getContentSize().height/2));
        this->addChild(countStateImage,20,110);
    }
}

void SGActivitySubLayer::buttonClickOK(SGButton *sender)
{

    if (!iscantouch || isMove) {
        return;
    }
    item->BtnTouched(this);
    
}

bool SGActivitySubLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
       if (!me_nu->isEnabled()) {
        return false;
    }
    else
    {
        ((CCScrollView*)getParent()->getParent())->ccTouchBegan(pTouch, pEvent);
        touchBeginLocation = pTouch->getLocationInView();
        isMove = false;
       return true;
    }
    
}

void SGActivitySubLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isMove) {
        CCPoint touchLocation = pTouch->getLocationInView();
        float distance = ccpDistance(touchLocation, touchBeginLocation);
        if (distance > 6)
        {
            isMove = true;
        }
    }
    if (isMove) {
        ((CCScrollView*)getParent()->getParent())->ccTouchMoved(pTouch, pEvent);
    }
}

void SGActivitySubLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    ((CCScrollView*)getParent()->getParent())->ccTouchEnded(pTouch, pEvent);
}

// 为显示阴影效果
//void SGActivitySubLayer::selectEmbattle()
//{
////	int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
////    if (guideId < UIG_MAX1) return;
////    
//    if (!iscantouch || isMove) {
//        return;
//    }
////    if (enterType == 0) { //可以导向武将滑动！
////        //获得当前阵营武将列表大全。
////        SGTeamgroup* sgtg = SGTeamgroup::shareTeamgroupInfo();
////        CCArray* ofcList = sgtg->getEmbattleOfficers(sgtg->getiscurrent());
////        SGGeneralInfoLayer::setMyOfficerList(ofcList);
////        //打开武将详情
////        SGMainManager::shareMain()->showGeneralInfoLayer(_officerCard, 3);
////    }else if(enterType == 3)
////    {
////		//        SGMainManager::shareMain()->showEmbattleLayer(1);
////    }else if(enterType == 2)
////    {
////        SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
////        SGMainManager::shareMain()->showMessage(player->getPlayerUnlockTip()->getCString());
////    }else if(enterType == 1)
////    {
////		//        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();
////        int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
////        CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
////        int totleGovern = 0;
////        CCObject *obj = NULL;
////        CCARRAY_FOREACH(array, obj)
////        {
////            SGOfficerCard *card = (SGOfficerCard *)obj;
////			
////            totleGovern += card->getGoverning();
////            if (card->getOfficerEquips())
////            {
////                for (int i = 0; i <card->getOfficerEquips()->count(); i++)
////                {
////                    SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
////                    if ((card->getGoverning()+equip->getGoverning())<0) {
////                        totleGovern -= card->getGoverning();
////                    }
////                    else{
////                        totleGovern += equip->getGoverning();
////                    }
////                }
////            }
////        }
////        
////        int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
////        SGArrangeLayer *layer = SGArrangeLayer::create(index/120 + 1,totleGovern,10,teamid);
////        SGMainManager::shareMain()->showLayer(layer);
////        SGMainManager::shareMain()->showblack();
////	}
//}

void SGActivitySubLayer::selectSubview()
{
	
}

void SGActivitySubLayer::setiscantouch(bool iscan)
{
    iscantouch = iscan;
    me_nu->setEnabled(iscan);

//    if (enterType == 3 && getChildByTag(123)) {
//        ((CCMenu *)getChildByTag(123))->setEnabled(iscan);
//    }
}



