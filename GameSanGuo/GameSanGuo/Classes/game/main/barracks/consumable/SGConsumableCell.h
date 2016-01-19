//
//  SGConsumableCell.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-15.
//
//

#ifndef __GameSanGuo__SGConsumableCell__
#define __GameSanGuo__SGConsumableCell__
#include "cocos2d.h"
#include "SGStoryBaseCell.h"

class SGConsumableDelegate;
class SGConsumableCell : public SGStoryBaseCell
{
public:
	SGConsumableCell();
	~SGConsumableCell();
	static SGConsumableCell *create(SGConsumableCard *card, SGConsumableDelegate *del = NULL);
	void consumeUseHandler();//使用消耗品
	void consumeUnuseHandler();//道具不能使用时候的回调
	void updateCell(SGConsumableCard *card = NULL);
	bool isCanUseConsumable(SGConsumableCard *card);
	
private:
	void initData(SGConsumableCard *card);//初始化数据
	void initView();
	void showConsumableInfo();
	

private:
	SGConsumableDelegate *consumeDel;//上层代理
	SGCCLabelTTF *consumeName;//消耗品名称
	SGConsumableCard *m_card;//cell所拥有的card信息
	SGCCLabelTTF *cardNum;
	CCSprite *consumeIcon;//头像
	CCSprite *border;//边框
	SGButton *bgBtn;//背景大按钮
public:
	CCMenu *menu;//放置使用button
	SGButton *useConsume;//使用button
	
	
};

//代理
class SGConsumableDelegate
{
public:
	virtual void useConsumable(SGConsumableCard *card){};
	virtual void showConsumeInfoHandler(SGConsumableCell *consumeCell){};
};


//如果是礼包出现详情弹框
class SGGiftDetialBox : public SGBaseBox
{
public:
	SGGiftDetialBox();
	~SGGiftDetialBox();
	static SGGiftDetialBox *create(std::vector<int> itemids, std::vector<int> nums, std::vector<int> types, SGBoxDelegate *del = NULL);
	void initView();
	void okHandler();

private:
	SGBoxDelegate *del;
	std::vector<int> itemid;
	std::vector<int> num;
	std::vector<int> type;
	
};



////礼包中每个item
//class SGGiftItem : public SNSTableViewCellItem
//{
//public:
//	SGGiftItem();
//	~SGGiftItem();
//	static SGGiftItem *create(int id, int num, int type);
//	void initView();
//	void createItemWithInfo();
//private:
//	int id;
//	int num;
//	int type;
//	
//	SGCCLabelTTF *label_name;
//    SGCCLabelTTF *label_count;
//    CCSprite *main_sprite;
//    CCSprite *kuang_sprite;
//    CCSprite *country_sprite;
//};
//
//


#endif /* defined(__GameSanGuo__SGConsumableCell__) */
