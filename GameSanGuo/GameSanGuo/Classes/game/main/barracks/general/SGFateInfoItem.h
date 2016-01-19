//
//  SGFateInfoItem.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/17/13.
//
//  @decription:将缘滑动页面的子节点。
//

#ifndef GameSanGuo_SGFateInfoItem_h
#define GameSanGuo_SGFateInfoItem_h

#include "SNSTableViewCellItem.h"

class SGFateBase;
class SGFateEffect;
class SGOfficerCard;
class SGFateInfoData;

class SGFateInfoItem : public SNSTableViewCellItem
{
//function
public:
    //构造
    SGFateInfoItem();
    
    //析构
    ~SGFateInfoItem();
    
    //创建
    static SGFateInfoItem* create(SGFateInfoData* data, int enterType);
    
    //设置两个数据引用
    bool init(SGFateInfoData* data);

    //初始化界面
    virtual bool initCellItem();
    
//function
private:
    //返回拼接好的字符串
    CCString* getDetailString(CCString* ccstr);
    
    //获得加成类型图标，以及加成文字
    CCArray* getIconAndTextString();
    
//member
public:
    
//member
private:
	int enterType;
    //内部的武将存储
    SGOfficerCard* m_officerCard;
    
    //内部的将缘存储
    SGFateBase* m_fateBase;
    int state;
    
};

#endif
