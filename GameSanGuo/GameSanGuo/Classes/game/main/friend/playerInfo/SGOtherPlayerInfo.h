//
//  SGOtherPlayerInfo.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#ifndef __GameSanGuo__SGOtherPlayerInfo__
#define __GameSanGuo__SGOtherPlayerInfo__

#include <iostream>
#include "SGBaseBox.h"
#include "SGGamePlayerInfo.h"

class SGOtherPlayerInfo : public SGBaseBox {
private:
    SGGamePlayerInfo *_playerInfo;
    int _isFriend;
    
    SGCCLabelTTF *labelPlayerName;
//    CCSprite *spritePlayerCountry;
    SGCCLabelTTF *labelPlayerCountry;
    SGCCLabelTTF *labelPlayerLevel;
//    CCLabelTTF *labelPlayerID;
    CCSprite *spritePlayerImage;
    SGCCLabelTTF *labelOfficerName;
    SGCCLabelTTF *labelOfficerLevel;
    SGButton *buttonOfficerHead;
    SGCCLabelTTF *labelMainOfficerSkill;
    SGCCLabelTTF *labelOfficerSkill;
    CCSprite *spritePlayerImageBG;
    
    SGCCLabelTTF *labelAttack_1;
//    CCLabelTTF *labelAttack_2;
    
    //蓄力
    SGCCLabelTTF *labelStoreUp;
//    CCLabelTTF *labelStoreUp_2;
    
    //防御
    SGCCLabelTTF *labelDefense_1;
 //   CCLabelTTF *labelDefense_2;
    
    //速度
    SGCCLabelTTF *labelSpeed;
  //  CCLabelTTF *labelSpeed_2;
    
    //血量
    SGCCLabelTTF *labelMorale_1;
 //   CCLabelTTF *labelMorale_2;
    //上阵需统御力
    SGCCLabelTTF *labelBattleForce;
  //  CCLabelTTF *labelBattleForce_2;
    
    
    
    void constructView();
    void addLabel(cocos2d::CCPoint ponit, int m_value);
    void buttonClickOpenAll();
    void buttonClickAddFriend();
    void buttonClickCancel();
    void buttonClickOfficerHead();
    void setPlayerInfo();
    void setLordInfo();
    
public:
    
    SGOtherPlayerInfo();
    ~SGOtherPlayerInfo();
    static SGOtherPlayerInfo *create(SGGamePlayerInfo *playerInfo , SGBoxDelegate *delegate);
    
    virtual void boxClose();
};

#endif /* defined(__GameSanGuo__SGOtherPlayerInfo__) */
