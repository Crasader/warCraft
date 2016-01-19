//
//  SGSkillItem.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#ifndef __GameSanGuo__SGSkillItem__
#define __GameSanGuo__SGSkillItem__

#include "SNSTableViewCellItem.h"
#include "SGOfficerCard.h"
#include "SGCCLabelTTF.h"
#include "SNSTableView.h"


//////////
///////////////////////
class SGSkillItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
///////////


///////////
///
class SGSkillItem : public SNSTableViewCellItem
{
private:
    CCSprite *spritePlayerHead;
    SGCCLabelTTF *labelName;
    SGCCLabelTTF *labelLevel;
    CCArray *arrayStar;
    CCSprite *sprite;
    SGBaseMilitaryCard *_card;
    SGCCLabelTTF *lab_skill;
    SGCCLabelTTF *lab_skillLvl;
    SGSkillItemDelegate *delegate;
    SNSTableView *tableview;
private:
    void showPlayerStar();
    
public:
    void updataCard(SGOfficerCard *card);
	virtual bool initWithSize();
    SGSkillItem();
	~SGSkillItem();
	static SGSkillItem* create(SGSkillItemDelegate*del,SNSTableView* tableView);
    void buttonClick(CCObject*obj);
};


#endif /* defined(__GameSanGuo__SGSkillItem__) */
