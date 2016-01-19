//
//  SGAutoEquipBox.h
//  GameSanGuo
//
//  Created by kanata on 13-10-31.
//
//

#ifndef __GameSanGuo__SGAutoEquipBox__
#define __GameSanGuo__SGAutoEquipBox__


class SGAutoEquipBox : public SGBaseBox
{
private:
    SGOfficerCard *officerCard;
    SGEquipCard *equipCard;
    int state;
    SGBoxDelegate *dlg;
    int enterType;
    int autotype;
//private:
//    void boxCloseWithOutCallBack(CCObject*obj);
//    void boxClose(CCObject*obj);
public:
    SGAutoEquipBox();
    ~SGAutoEquipBox();
    
   // static SGAutoEquipBox*create(SGBoxDelegate *delegate,int autotype);
    static   SGAutoEquipBox *create(SGBoxDelegate *delegate, SGOfficerCard *card,int x);
    void initView(int x);
    virtual void boxClose(CCObject*obj);
    //virtual void Listener(CCObject*obj);
    void boxClosesendmsg(CCObject*obj);
};































#endif /* defined(__GameSanGuo__SGAutoEquipBox__) */
