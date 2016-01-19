//
//  SGLootWiKiLayer.h 武将和装备来源详情
//  GameSanGuo
//
//  by ： zyc  14-7-10.
//
//

#include "SGMainLayer.h"
#include "SGBaseBox.h"


#ifndef __GameSanGuo__SGLootWiKiMainLayer__
#define __GameSanGuo__SGLootWiKiMainLayer__

class SGLootWiKiMainLayer : virtual public SGBaseBox
{
public:
    //dtor
    ~SGLootWiKiMainLayer();
    //static create method   isMaterial=1 表示 材料来源。
    static SGLootWiKiMainLayer * create( int m_officerProtoId , int isMaterial = -1);
    //初始化数据
    void initView(int pieceId );
    
    //关闭按钮的回调
    void onCloseBtnClicked();
private:
    //ctor
    SGLootWiKiMainLayer();
    
//    //精英闯关已参加次数 、上限
//    int m_jingYingCout;
//    int m_jingYingMax;
    
    void sortData();
    
    
    //碎片Id
    int m_pieceId;
    //标志 是否是材料来源  值为1时=材料来源，初始化为-1.
    int m_isMaterial;
    //碎片来源信息
    CCArray * m_pieceLootInfo;
};

#endif /* end */
