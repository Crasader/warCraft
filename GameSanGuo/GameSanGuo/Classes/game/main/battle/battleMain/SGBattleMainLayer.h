//
//  SGBattleMainLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#ifndef __GameSanGuo__SGBattleMainLayer__
#define __GameSanGuo__SGBattleMainLayer__

#include "SGBaseTableLayer.h"
#include "SGBattleData.h"

/**
 * 天梯赛主界面
 * 数据模型 SGBattleData
 **/

typedef enum{
    rank_button_kua = 1,
    rank_button_ben,
    rank_button_wei,
    rank_button_shu,
    rank_button_wu,
}rankAction;

class SGBattleMainLayer : public SGBaseLayer
{
public:
    SGBattleMainLayer(void);
    ~SGBattleMainLayer(void);

    static SGBattleMainLayer *create(SGBattleData *data);

private:
    void initView();
    void initTitle();
    void initViewBgs(); //设置背景
    void initSeasonTimeView(); // 晋级战信息界面
//    void initPlayerInfoView(); // 个人战绩界面
//    void initPlayerRankView(); // 玩家排行界面
    
    //button action
    void backHandler(); // 返回按钮
    void joininAction();    // 加入战斗
    void chgScoreAction();  // 积分兑换
    void helphandle();
    bool checkGovernNeed();
    void jxbuff();
    void setlianshengjiangli();
    void shangjiebangdan();
    void saijijiangli();
    void meirijiangli();
    void lianshenginfo();
public:
    void getRankAction(CCObject *sender); // 获取排行信息
    void showRankLayer(CCObject *sender); // 显示排行信息
    void showChgScoreLayer(CCObject *sender); // 积分兑换界面
//    void showpvpEverydayLayer(CCObject *sender); // 每日奖励界面
    void showSeasonListLayer(CCObject *sender); // 赛季奖励界面
    void showLastListlayer(CCObject *sender); // 上界榜单界面
    //用于保持前一个页面同时刷新本页面，目前只用于SGGeneralInfoLayer
    virtual void refreshView();
private:
    int _selectIndex; // 选中的按钮
    SGBattleData *battleData;

    CCArray *ary;
    ////天梯积分
    SGCCLabelTTF *_ttjfv;
    int _nJifen;
    int m_jieshu;
    
    //军粮label  购买军粮后刷新ui
    SGCCLabelTTF * junliangvalue2;
public:
    //购买军粮后 刷新ui
    void updateJunLiang(const char * );
};

#endif /* defined(__GameSanGuo__SGBattleMainLayer__) */
