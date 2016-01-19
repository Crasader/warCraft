//
//  SNSAction.h
//  WarCraftCardGame
//
//  Created by ourpalm on 5/30/13.
//
//

#ifndef __WarCraftCardGame__SNSAction__
#define __WarCraftCardGame__SNSAction__

#include "cocos2d.h"

USING_NS_CC;

class SNSActionNode : public CCNode
{
public:
	SNSActionNode();
	~SNSActionNode();
	
	virtual bool init();
	
	CREATE_FUNC(SNSActionNode);

public:
	void runActionAroundCircle(CCNode* child, float radius, float speed);

	void updateCircle(float delta);

private:
	float				m_radius;				// 半径
	float				m_speed;				// 环绕速度
	float				m_angle;				// 当前角度
};

#endif /* defined(__WarCraftCardGame__SNSAction__) */
