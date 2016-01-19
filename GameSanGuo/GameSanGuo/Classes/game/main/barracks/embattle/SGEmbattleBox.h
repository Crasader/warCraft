//
//  SGEmbattleBox.h
//  GameSanGuo
// 设置上场武将弹框
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGEmbattleBox__
#define __GameSanGuo__SGEmbattleBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"


///////////////////////////////////////////////////////////
//页面返回Embattle信息
class ERI
{
public:
	//ERI():em(0),ty(0),govern(0),isFromEmbattle(false){}
	int em;//同emtype
	int ty;//同type
	int govern;//同totleGovern
	bool isFromEmbattle;//是否来自于Embattle
};
//////////////////////////////////////////////////////////

class SGEmbattleBox : public SGBaseBox
{
private:
    int _positon;
    SGOfficerCard *officerCard;
    int m_gov;
    int totleGovern;
    //这里貌似是阵容的自然ID，即123，而非012.
    int emtype;
	int fromWhere;//统御力超过提示是否出现

private:
	void strengOfficer(CCObject *obj = NULL);// 强化
	void advanceOfficer(CCObject *obj = NULL);//转生
	void exchangeEquip(CCObject *obj = NULL);//换装
    //设为主将
    void setMainOfficerHandler();
    //下场
    void offHandler();
    void upHandler();
    void changeHandler();
    void setLordOfficerListener(CCObject *obj);
    void setMainOfficerListener(CCObject *obj);
    void offMainOfficerListener(CCObject *obj);    
    void showGeneralInfoLayer();
    void addLabel(CCPoint ponit,int m_value);
	void cancelAction();//取消按钮回调

public:
    SGEmbattleBox();
    ~SGEmbattleBox();
    
	static ERI *shareEriInfo(void);//获取唯一的info
	
    static SGEmbattleBox*create(SGBoxDelegate *delegate, SGOfficerCard *card, int govern, int emtype,int pos = 0, int wid = 535, int hei = 646, int fromWhere = 0);
//    virtual void initMsg();
    void initView();
    virtual void boxClose();


	
};
#endif /* defined(__GameSanGuo__SGEmbattleBox__) */
