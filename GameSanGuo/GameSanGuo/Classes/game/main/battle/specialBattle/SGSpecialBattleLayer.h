//
//  SGSpecialBattleLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-12-12.
//
//

#ifndef __GameSanGuo__SGSpecialBattleLayer__
#define __GameSanGuo__SGSpecialBattleLayer__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGShowString.h"
#include "SGBaseBox.h"


//掉落预览,只简单显示图标, 数量暂时不用
typedef struct dropList
{
    int itemType;
    int itemId;
    int itemCount;
}DropList;

//章数据
typedef struct specialBattleData : public CCObject
{
    //章id
    int storyId;
    //是否开启
    int isOpen;
    //章名称
    std::string chapName;
    //章开启描述， 根据isOpen确定显示什么
    std::string chapOpenDesc;
    //章关闭描述
    std::string chapClosedDesc;
    //banner上面的描述
    std::string chapDesc;
    //当前章免费可打的次数
    int freePlayTimes;
    //最大的可打的次数
    int maxPlayTimes;
    //当前章可购买的次数
    int chapCanBuyTimes;
    //banner前要显示的人物的iconId
    int bannerBgId;
    //itemId
    int itemId;
    //itemType
    int itemType;
    //位置
    int pos;
    
}SpecialBattleData;

//节数据
typedef struct specialBattleSectionData : public CCObject
{
    //plotid
    int plotId;
    //chapId
    int chapId;
    //当前节是否可参加
    int canJoin;
    //当前节的背景人物id
    int bgIconId;
    //节名称
    std::string sectionName;
    //开启条件提示文本
    std::string conditionTips;
    //建议战斗力
    int suggestPower;
    //掉落预览
    std::vector<DropList> dropVec;
    
}SpecialBattleSecData;



//显示规则的弹窗
class SpRuleInfoBox : public SGBaseBox
{
public:
    SpRuleInfoBox();
    ~SpRuleInfoBox();
    
    static SpRuleInfoBox *create();
    void initView();
    void pressOkButton(CCObject *obj);

};

class SGRuleItem : public CCLayer
{
public:
	SGRuleItem();
	~SGRuleItem();
	static SGRuleItem *create(int index, std::string ruleInfo);
	void initView();
private:
    std::string ruleInfo;
    int index;
};


class SGSpecialBattleLayer : public SGBaseLayer
{
public:
    SGSpecialBattleLayer();
    ~SGSpecialBattleLayer();
    
    static SGSpecialBattleLayer *create(CCArray *specialBattleData);
    
    void initView();
    
    //背景的相关操作
    //平铺二个背景
    void makeDoubleBg(int shift, int num, int tag);
    //无限移动二个背景
    void moveDoubleBg(float dt);

    //商城入口
    void mallEntry(CCObject *obj);
    void tipsInfo(CCObject *obj);
    
    //创建一个幻境的章节的小layer， 方便操作其他元素
    CCLayerColor *createChapButton(int buttonTag, SpecialBattleData *data);
    //点击某个章节的icon，进入对应的节列表
    void enterSpBattleSection(CCObject *obj);
    void specialBattlePlostListener(CCObject *obj);
    
    //星空上加入随机闪烁的星星
    void flashStarOnSpace();
    
    
private:
    //特殊副本的章数据
    CCArray *chapDatas;
    //点了第几个
    int selectIndex;
    
};




#endif /* defined(__GameSanGuo__SGSpecialBattleLayer__) */
