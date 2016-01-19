//
//  SGSoldierinfoEffect.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-17.
//
//

#ifndef __GameSanGuo__SGSoldierinfoEffect__
#define __GameSanGuo__SGSoldierinfoEffect__

#include <iostream>
#include "SGBaseBox.h"
#include "SGBaseTableLayer.h"
#include "SGOfficerCard.h"
#endif /* defined(__GameSanGuo__SGSoldierinfoEffect__) */


class SGSoldierinfoEffect: public SGBaseBox
{
public:
      CCScrollView *m_scrollview;
    SGSoldierinfoEffect();
   
    ~SGSoldierinfoEffect();
    void initView();
    void  backhandle(CCObject*obj);

private:
    
    SGBoxDelegate *_dele;
    SGSoldierCard *_card;
    int enterType;

     
private:
  
    void strengHandlerzjj();
    void strengHandlerwjj();

protected:
  
public:
   
    static SGSoldierinfoEffect *create(SGBoxDelegate *bdg, SGSoldierCard *card,int type);
    bool init(SGBoxDelegate *bdg, SGSoldierCard *card,int type);
};
    
    
    
    
    



