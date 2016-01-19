//
//  SGChangeEquipEffectLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-25.
//
//

#ifndef __GameSanGuo__SGChangeEquipEffectLayer__
#define __GameSanGuo__SGChangeEquipEffectLayer__

#include <iostream>

#include "cocos2d.h"
#include "SGEquipCard.h"
#include "SGCCLabelTTF.h"
USING_NS_CC;

 

class SGChangeEquipEffectLayer:public CCLayer {
private:
    
    SGCCLabelTTF *atk;
    SGCCLabelTTF *atk1;
    
//    SGCCLabelTTF *defLable;
//    SGCCLabelTTF *speedLable;
//    SGCCLabelTTF *xuliLable;
//    SGCCLabelTTF *shiqiLable;
//    SGCCLabelTTF *govLable;
    
  
    SGEquipCard *equipCard_;
    int type;
private:
      void constructView();
public:
    SGChangeEquipEffectLayer();
   ~ SGChangeEquipEffectLayer();
    static SGChangeEquipEffectLayer *create();
  
private:
    void showLayer();
    void hideLayer();
    void showBezier();
    void showAtk();
    void showDef();
    void showSpeed();
    void showXuli();
    void showGov();
    void showShiqi();
    
    void showEffectallFunction();
    
public:
   void showEffectall(SGEquipCard *equipCard,int type);
    void showEffect(SGEquipCard *equipCard,int type);//1.卸载，2增加
   
};

#endif /* defined(__GameSanGuo__SGChangeEquipEffectLayer__) */
