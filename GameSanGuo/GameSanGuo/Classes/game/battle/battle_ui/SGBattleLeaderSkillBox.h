//
//  SGBattleLeaderSkillBox.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-5-8.
//
//

//--------------------------------------------
// 点击战场主将计按钮，显示主将计介绍 （敌方或者我方）
//--------------------------------------------

#ifndef __GameSanGuo__SGBattleLeaderSkillBox__
#define __GameSanGuo__SGBattleLeaderSkillBox__

#include <iostream>
#include "SGBaseBox.h"


class SGBattleLeaderSkillBox : public SGBaseBox
{
private:
    //当前是我方还是敌方的主将计
    bool m_bIsMe;
    //当前我方主将计能否发动
    bool m_bIsCanAttack;

    CCString *m_pLordName;
    int m_nCountryId;
    int m_nGeneralId;
    int m_nStarLevel;
    int m_nLordId;
    int m_nSkillId;
    
    int m_nRoundCount;
    
    void buttonClickOK(CCObject *object);
    void buttonClickSend(CCObject *object);

    SGBoxDelegate *del;
    
public:
    
    void useLeaderSkill(void);   //added by cgp
    //初始化
    SGBattleLeaderSkillBox();
    ~SGBattleLeaderSkillBox();
    static SGBattleLeaderSkillBox *create(SGBoxDelegate *delegate,bool isme,bool iscanAttack,int generalId,int lordid,int round);
    void viewDidLoad();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
};

#endif /* defined(__GameSanGuo__SGBattleLeaderSkillBox__) */
