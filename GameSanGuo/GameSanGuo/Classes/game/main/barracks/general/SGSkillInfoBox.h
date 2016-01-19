//
//  SGSkillInfoBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-21.
//
//

#ifndef __GameSanGuo__SGSkillInfoBox__
#define __GameSanGuo__SGSkillInfoBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
class SGSkillInfoBox : public SGBaseBox
{
private:
    SGBoxDelegate *_dele;    
    SGOfficerCard *_card;
    int enterType;
private:
    void confirmHandler();
    void bindFbHandler();
    void bindHandler();
    void cleanFbHandler();
    void cleanHandler();
    void backHandler();
    void strengHandler();
protected:
    virtual void boxClose();
public:
    SGSkillInfoBox();
    ~SGSkillInfoBox();
    static SGSkillInfoBox *create(SGBoxDelegate *bdg, SGOfficerCard *card,int type);
    bool init(SGBoxDelegate *bdg, SGOfficerCard *card,int type);
};

#endif /* defined(__GameSanGuo__SGSkillInfoBox__) */
