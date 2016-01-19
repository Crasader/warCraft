//
//  SGPiecesCell.h
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#ifndef __GameSanGuo__SGPiecesCell__
#define __GameSanGuo__SGPiecesCell__

#include <iostream>
#include "cocos2d.h"
#include "SGStoryBaseCell.h"
class SGPiecesDelegate;
class SGPiecesCell : public SGStoryBaseCell
{
public:
	SGPiecesCell();
	~SGPiecesCell();
	static SGPiecesCell* create(SGPiecesDelegate *_delegate, SGPiecesCard *info, bool useCommon, LayerTag _tag);//创建碎片的cell
	//void summon();//召唤回调
	void initData(SGPiecesCard *card);//初始化数据
	void updatePiecesCard(SGPiecesCard *card);
    
private:
	void viewDidLoad();
	SGCCLabelTTF *totalAndCurProgress;//碎片数量总进度与当前进度
	//SGCCLabelTTF *commPieces;// 通用碎片数量
	SGCCLabelTTF *piecesName;//碎片名称
	SGPiecesDelegate *delegate;//代理
    void showinfo();
    
public:
	virtual void onEnter();
	//CCMenu *menu;//放置cell后边的召唤button
	//SGButton *callBtn;
	SGPiecesCard *info;//显示所需要的数据
private:
	//bool isUseCommon;//是否使用了通用碎片
    //bool isable;//“召”是否有效
    LayerTag m_tag;
	
};


class SGPiecesDelegate
{
public:
    virtual void summonPieces(SGPiecesCell *piecesItem){};
    virtual void showPiecesinfo(SGPiecesCell *piecesItem){};
};

#endif /* defined(__GameSanGuo__SGPiecesCell__) */
