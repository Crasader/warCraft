//
//  SGRankLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-2-11.
//
//

#ifndef __GameSanGuo__SGRankLayer__
#define __GameSanGuo__SGRankLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "CCScrollView.h"
#include "SGMenu.h"
class SGRankLayer:public SGBaseTableLayer
{
public:
    SGRankLayer();
    ~SGRankLayer();

    static SGRankLayer *create(CCArray *array,int value ,int ranking,int maxitemnum);
    void receivedetail(CCObject*obj);
private:
    void initDatas(CCArray *array);
    void initView(int value ,int ranking);
    void rankTypeSelect(CCNode*node);
    void countryTypeSelect(CCNode*node);
    void receivePlayerDetail(CCObject* obj);
    void setselfrank(int value ,int ranking);
    void flushData(CCObject* obj);
    void refreshrank(int value ,int ranking);
    void jibunmieru(bool mierukana);
    void gotoEmbattleLayer();
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    virtual void setIsCanTouch(bool isTouch);

private:
    CCScrollView* scrollview;
    int ranktypetag;
    int countrytypetag;
    CCArray*tempdata;
    
    ////我的排名
    int myrank;
    //我的数据(可能是财富 角色等级  战力等等)
    int mydata;

    CCLayer*layer;
    SGMenu *menu1;
    
    
    SGCCLabelTTF *ranklabel;
    SGCCLabelTTF *shengwang;
    SGCCLabelTTF *rank;
    SGCCLabelTTF *ranknum;
    SGCCLabelTTF *name;
    int selectindex;
    int playerplayerid;
    int newindex;
    

    
 
    //当前列表的最大项目数
    int maxnum;
    bool isreload;//是否是本页下翻造成的刷新页面
    int count;
    CCPoint point;
    
    
public:
    
      CC_SYNTHESIZE(CCSize, tableviewsize, TableViewSize);
      CC_SYNTHESIZE(CCPoint, tableviewposition, TablePosition);

};
#endif /* defined(__GameSanGuo__SGRankLayer__) */
