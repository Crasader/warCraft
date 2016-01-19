//
//  SGChangeEquipEffectLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-25.
//
//

#include "SGChangeEquipEffectLayer.h"
#include "SGLayout.h"
#include "SGStringConfig.h"

#define Height 180
#define  speed 0.7
#define atkHeight  200//280
#define atcWidth -40 //70-40
#define height2 300 //380
#define up 0.2

#define delayTime1 0.5
#define FadeTime1 0.1
#define FadeOutTime 0.1
#define MOVETIME1 0.1
#define MOVETIME2 0.5

SGChangeEquipEffectLayer:: SGChangeEquipEffectLayer():
atk(NULL),
atk1(NULL),
equipCard_(NULL),
type(0)
{


}
SGChangeEquipEffectLayer::~SGChangeEquipEffectLayer(){


}

SGChangeEquipEffectLayer*  SGChangeEquipEffectLayer::create(){
    SGChangeEquipEffectLayer *sgChangeEquipEffectLayer=new SGChangeEquipEffectLayer();
    if (sgChangeEquipEffectLayer) {
        
        sgChangeEquipEffectLayer->constructView();
        sgChangeEquipEffectLayer->autorelease();
        return sgChangeEquipEffectLayer;
    }
    CC_SAFE_DELETE(sgChangeEquipEffectLayer);
    return NULL;
}
void SGChangeEquipEffectLayer::constructView(){
    CCPoint center=SGLayout::getPoint(KMiddleRight);
//    CCSize s=CCDirector::sharedDirector()->getWinSize();
    
    atk=SGCCLabelTTF::create(str_ChangeEquipEffectLayer_str1, FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
    atk->setAnchorPoint(ccp(1, 0));
    atk->setPosition(ccpAdd(center, ccp(atcWidth, atkHeight)));
    //modify by:zyc. merge into create.
    //atk->setColor(ccc3(0x0c, 0xff, 0x00));
    this->addChild(atk);
   // atk->setVisible(false);
//    defLable=SGCCLabelTTF::create("防御力+", FONT_BOXINFO, 28);
//    defLable->setAnchorPoint(ccp(speed, 0));
//    defLable->setPosition(ccpAdd(center, ccp(100, 350)));
//    defLable->setColor(ccc3(0x0c, 0xff, 0x00));
//    this->addChild(defLable);
//
//    speedLable=SGCCLabelTTF::create("速度+", FONT_BOXINFO, 28);
//    speedLable->setAnchorPoint(ccp(speed, 0));
//    speedLable->setPosition(ccpAdd(center, ccp(100, 350)));
//    speedLable->setColor(ccc3(0x0c, 0xff, 0x00));
//    this->addChild(speedLable);
//    speedLable->setVisible(false);
//    
//    xuliLable=SGCCLabelTTF::create("蓄力+", FONT_BOXINFO, 28);
//    xuliLable->setAnchorPoint(ccp(speed, 0));
//    xuliLable->setPosition(ccpAdd(center, ccp(100, 350)));
//    xuliLable->setColor(ccc3(0x0c, 0xff, 0x00));
//    this->addChild(xuliLable);
//    xuliLable->setVisible(false);
//    
//    shiqiLable=SGCCLabelTTF::create("血量+", FONT_BOXINFO, 28);
//    shiqiLable->setAnchorPoint(ccp(speed, 0));
//    shiqiLable->setPosition(ccpAdd(center, ccp(100, 350)));
//    shiqiLable->setColor(ccc3(0x0c, 0xff, 0x00));
//    this->addChild(shiqiLable);
//    shiqiLable->setVisible(false);
//
//    govLable=SGCCLabelTTF::create("统御力+", FONT_BOXINFO, 28);
//    govLable->setAnchorPoint(ccp(speed, 0));
//    govLable->setPosition(ccpAdd(center, ccp(100, 350)));
//    govLable->setColor(ccc3(0x0c, 0xff, 0x00));
//    this->addChild(govLable);
//    govLable->setVisible(false);
    
    this->setVisible(false);
}
void SGChangeEquipEffectLayer::showEffectallFunction(){
//    SGCCLabelTTF *label=(SGCCLabelTTF *)obj;
    
    CCPoint center=SGLayout::getPoint(KMiddleRight);
    CCMoveTo *moveTo=CCMoveTo::create(up+1.0, ccpAdd(center, ccp(atcWidth, height2)));
    
//    atk->setString(atk_str->getCString());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1+0.5);
    atk->runAction(CCSequence::create(fun1,atk->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk->fadout(FadeOutTime),moveTo2,fun2,NULL));
}
void SGChangeEquipEffectLayer::showEffectall(SGEquipCard *equipCard,int type){
//    CCString *attribute=NULL;
//    if (equipCard->getAtk()>0) {
//        if (equipCard->getDef()>0) {
//            if (equipCard->getMorale()>0) {
//                if (equipCard->getSpeed()) {
//                    attribute=CCString::createWithFormat("攻击力+ %d\n 防御力+ %d\n 血量+ %d \n 速度 %lf",
//                                                        equipCard->getAtk(),
//                                                         equipCard->getDef(),equipCard->getMorale(),
//                                                         equipCard->getSpeed());
//                }else
//                    attribute=CCString::createWithFormat("攻击力+ %d\n 防御力+ %d\n 血量+ %d",
//                                                         equipCard->getAtk(),
//                                                         equipCard->getDef(),equipCard->getMorale());
//            }
//            else
//            {
//                
//            }
//        }
//    }
    this->setVisible(true);
    this->equipCard_=equipCard;
    this->type=type;
    
    this->setTouchEnabled(false);
    std::string attribute;
    
    if (type==1) {
//        attribute=CCString::createWithFormat("攻击力+ %d\n 防御力+ %d\n 血量+ %d \n 速度+ %d",
//                                             equipCard->getAtk(),
//                                             equipCard->getDef(),equipCard->getMorale(),
//                                             (int )equipCard->getSpeed());
        CCString *buffer= NULL;
        if (equipCard->getAtk()>0) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str2,equipCard->getAtk());
            attribute.append(buffer->getCString());
        }
        if (equipCard->getDef()>0) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str3,equipCard->getDef());
            attribute.append(buffer->getCString());
        }
        if (equipCard->getMorale()>0) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str4,equipCard->getMorale());
            attribute.append(buffer->getCString());
        }
        if (equipCard->getSpeed()>1) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str5,(int)equipCard->getSpeed());
            attribute.append(buffer->getCString());
        }
        
        atk->setInsideColor(ccc3(0x0c, 0xff, 0x00));
        
    }else
    {
//        attribute=CCString::createWithFormat("攻击力- %d\n 防御力- %d\n 血量- %d \n 速度- %d",
//                                             equipCard->getAtk(),
//                                             equipCard->getDef(),equipCard->getMorale(),
//                                             (int )equipCard->getSpeed());
        
         CCString *buffer= NULL;
        if (equipCard->getAtk()>0) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str6,equipCard->getAtk());
            attribute.append(buffer->getCString());
        }
        if (equipCard->getDef()>0) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str7,equipCard->getDef());
            attribute.append(buffer->getCString());
        }
        if (equipCard->getMorale()>0) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str8,equipCard->getMorale());
            attribute.append(buffer->getCString());
        }
        if (equipCard->getSpeed()>1) {
            buffer=CCString::createWithFormat(str_ChangeEquipEffectLayer_str9,(int)equipCard->getSpeed());
            attribute.append(buffer->getCString());
        }
        
        atk->setInsideColor(ccRED);
    }
    //CCLog("%s",attribute.c_str());
    atk->setString(attribute.c_str());
     CCCallFunc *funAck=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showEffectallFunction));

    this->runAction(funAck);
    
}

void SGChangeEquipEffectLayer::showEffect(SGEquipCard *equipCard,int type ){
    
    this->setVisible(true);
    this->equipCard_=equipCard;
    this->type=type;
    
    this->setTouchEnabled(false);
    
    
    CCCallFunc *funAck=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showAtk));
    CCCallFunc *funDef=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showDef));
    CCCallFunc *funSpeed=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showSpeed));
    CCCallFunc *funXuli=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showXuli));
    CCCallFunc *funSq=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showShiqi));
    if (equipCard_->getAtk()>0) {
        
        if (equipCard_->getDef()>0) {
//             this->runAction(CCSequence::create(funAck,CCDelayTime::create(speed+0.2),funDef,CCDelayTime::create(speed+0.2),funSpeed,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(1.5),funXuli,NULL));
//            this->runAction(CCSequence::create(funAck,CCDelayTime::create(speed+0.2),funDef,CCDelayTime::create(speed+0.2),funSpeed,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(1.5),funXuli,NULL));
             this->runAction(CCSequence::create(funAck,CCDelayTime::create(speed+0.2),funDef,CCDelayTime::create(speed+0.2),funSpeed,CCDelayTime::create(speed+0.2),funSq,NULL));
//          this->runAction(CCSequence::create(funAck,CCDelayTime::create(0.6),funDef,CCDelayTime::create(0.6),funSpeed,CCDelayTime::create(0.6),funSq,CCDelayTime::create(0.6),funXuli,NULL));
        }else
        {
            if (equipCard_->getSpeed()>0) {
                this->runAction(CCSequence::create(funAck,CCDelayTime::create(speed+0.2),funSpeed,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(speed),funXuli,NULL));
            }
            else
            {
                this->runAction(CCSequence::create(funAck,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(speed+0.2),funXuli,NULL));
            }
            
        }
    }
    else
    {
        if (equipCard_->getDef()>0) {
            if (equipCard->getDef()>0) {
                 this->runAction(CCSequence::create(funDef,CCDelayTime::create(speed+0.2),funSpeed,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(+speed),funXuli,NULL));
            }
            else
                 this->runAction(CCSequence::create(funSpeed,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(speed+0.2),funXuli,NULL));
           
        }else
        {
            if (equipCard_->getSpeed()>0) {
                this->runAction(CCSequence::create(funSpeed,CCDelayTime::create(speed+0.2),funSq,CCDelayTime::create(speed+0.2),funXuli,NULL));
            }
            else
                 this->runAction(CCSequence::create(funSq,CCDelayTime::create(speed+0.2),funXuli,NULL));
            
        }
    }
   // this->setTouchEnabled(true);
   
}
void SGChangeEquipEffectLayer::showAtk(){
    if (equipCard_->getAtk()<=0) {
        return;
    }
    CCLog("攻击");
 
    int left=rand() %4;
//    int randNum=rand() %100 -200;
//    if (left>1) {
//        randNum=rand() %100 +100;
//    }
    CCString *atk_str;
    //CCJumpTo *jumpTo=CCJumpTo::create(speed, ccpAdd(atk->getPosition(), ccp(randNum, -100)), Height, 1);
    if (type== 2) {
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str6,equipCard_->getAtk());
    }
   else
       atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str2,equipCard_->getAtk());
    
    
    
    CCPoint center=SGLayout::getPoint(kMiddleCenter);
    CCMoveTo *moveTo=CCMoveTo::create(up, ccpAdd(center, ccp(atcWidth, height2)));
    
    atk->setString(atk_str->getCString());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1);
    atk->runAction(CCSequence::create(fun1,atk->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk->fadout(FadeOutTime),moveTo2,fun2,NULL));
    

    
}
void SGChangeEquipEffectLayer::showGov(){
    if (equipCard_->getGoverning()<=1) {
        return;
    }
    CCLog("统御力");
    
    int left=rand() %4;
//    int randNum=rand() %100 -250;
//    if (left>1) {
//        randNum=rand() %100 +150;
//    }
    //CCJumpTo *jumpTo=CCJumpTo::create(speed, ccpAdd(atk->getPosition(), ccp(randNum, -100)), Height, 1);
    CCString *atk_str;
    if (type== 2) {
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str10,equipCard_->getGoverning());
    }
    else
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str11,equipCard_->getGoverning());
    atk->setString(atk_str->getCString());
    CCPoint center=SGLayout::getPoint(kMiddleCenter);
    CCMoveTo *moveTo=CCMoveTo::create(up, ccpAdd(center, ccp(atcWidth, height2)));
    //CCFadeOut *fadeOut=CCFadeOut::create(0.5);
    atk->setString(atk_str->getCString());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1);
    atk->runAction(CCSequence::create(fun1,atk->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk->fadout(FadeOutTime),moveTo2,fun2,NULL));

}
void SGChangeEquipEffectLayer::showShiqi(){
    if (equipCard_->getMorale() <=1) {
        return;
    }
    CCLog("血量");
     CCPoint center=SGLayout::getPoint(kMiddleCenter);
    atk1=SGCCLabelTTF::create("", FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
    atk1->setAnchorPoint(ccp(1, 0));
    atk1->setPosition(ccpAdd(center, ccp(atcWidth, atkHeight)));
    //modify by:zyc. merge into create.
    //atk1->setColor(ccc3(0x0c, 0xff, 0x00));
    this->addChild(atk1);
    int left=rand() %4;
//    int randNum=rand() %100 -250;
//    if (left>1) {
//        randNum=rand() %100 +150;
//    }

    //CCJumpTo *jumpTo=CCJumpTo::create(speed, ccpAdd(atk->getPosition(), ccp(randNum, -100)), Height, 1);
    CCString *atk_str;
    if (type== 2) {
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str12,equipCard_->getMorale());
    }
    else
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str13,equipCard_->getMorale());
    atk1->setString(atk_str->getCString());
   
    CCMoveTo *moveTo=CCMoveTo::create(up, ccpAdd(center, ccp(atcWidth, height2)));

    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1);
    atk1->runAction(CCSequence::create(fun1,atk1->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk1->fadout(FadeOutTime),moveTo2,fun2,NULL));
    CCLOG("hehe");
    
}
void SGChangeEquipEffectLayer::showXuli(){
    if (equipCard_->getRound()<=1) {
        return;
    }
    
   
 
    int left=rand() %4;
//    int randNum=rand() %100 -250;
//    if (left>2) {
//        randNum=rand() %100 +150;
//    }
    //CCJumpTo *jumpTo=CCJumpTo::create(speed, ccpAdd(atk->getPosition(), ccp(randNum, -100)), Height, 1);
    
    CCString *atk_str;
    if (type== 2) {
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str14,equipCard_->getRound());
    }
    else
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str15,equipCard_->getRound());
    atk->setString(atk_str->getCString());
    CCPoint center=SGLayout::getPoint(kMiddleCenter);
    CCMoveTo *moveTo=CCMoveTo::create(up, ccpAdd(center, ccp(atcWidth, height2)));
    //CCFadeOut *fadeOut=CCFadeOut::create(0.5);
    atk->setString(atk_str->getCString());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1);
    atk->runAction(CCSequence::create(fun1,atk->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk->fadout(FadeOutTime),moveTo2,fun2,NULL));
}
void SGChangeEquipEffectLayer::showSpeed(){
    if (equipCard_->getSpeed()<1) {
        return;
    }
    
    CCLog("速度");
 
    CCPoint center=SGLayout::getPoint(kMiddleCenter);
  
    int left=rand() %4;
//    int randNum=rand() %100 -250;
//    if (left>1) {
//        randNum=rand() %100 +150;
//    }
    //CCJumpTo *jumpTo=CCJumpTo::create(speed, ccpAdd(atk->getPosition(), ccp(randNum, -100)), Height, 1);
    CCString *atk_str;
    if (equipCard_->getSpeed()<1) {
        equipCard_->setSpeed(1.0);
    }
    
    if (type== 2) {
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str16,(int)equipCard_->getSpeed());
    }
    else
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str17,(int)equipCard_->getSpeed());
    atk->setString(atk_str->getCString());
  
    CCMoveTo *moveTo=CCMoveTo::create(up, ccpAdd(center, ccp(atcWidth, height2)));
    //CCFadeOut *fadeOut=CCFadeOut::create(0.5);
    atk->setString(atk_str->getCString());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1);
    atk->runAction(CCSequence::create(fun1,atk->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk->fadout(FadeOutTime),moveTo2,fun2,NULL));

    
}
void SGChangeEquipEffectLayer::showDef(){
    if (equipCard_->getDef()<=1) {
        return;
    }
    
    int left=rand() %4;
//    int randNum=rand() %100 -200;
//    if (left>1) {
//        randNum=rand() %100 +100;
//    }
    
    //CCJumpTo *jumpTo=CCJumpTo::create(speed, ccpAdd(atk->getPosition(), ccp(randNum, -100)), Height, 1);
    CCString *atk_str;
    if (type==2) {
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str18,equipCard_->getDef());
    }
    else
        atk_str=CCString::createWithFormat(str_ChangeEquipEffectLayer_str19,equipCard_->getDef());
    atk->setString(atk_str->getCString());
     CCPoint center=SGLayout::getPoint(kMiddleCenter);
    CCMoveTo *moveTo=CCMoveTo::create(up, ccpAdd(center, ccp(atcWidth, height2)));
    //CCFadeOut *fadeOut=CCFadeOut::create(0.5);
  
    
    atk->setString(atk_str->getCString());
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::showLayer));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGChangeEquipEffectLayer::hideLayer));
    
    CCMoveTo *moveTo2=CCMoveTo::create(MOVETIME1, ccpAdd(center, ccp(atcWidth,height2+50)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, MOVETIME1);
    atk->runAction(CCSequence::create(fun1,atk->fadin(FadeTime1),easeIn,CCDelayTime::create(delayTime1),atk->fadout(FadeOutTime),moveTo2,fun2,NULL));
    
}
void SGChangeEquipEffectLayer::showLayer(){
    atk->setVisible(true);
    this->setVisible(true);
}
void SGChangeEquipEffectLayer::hideLayer(){
     atk->setVisible(false);
    CCPoint center=SGLayout::getPoint(KMiddleRight);
    atk->setPosition(ccpAdd(center, ccp(atcWidth, atkHeight)));
    
   // this->setVisible(false);
  
}