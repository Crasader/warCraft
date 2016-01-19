//
//  SNSAction.cpp
//  WarCraftCardGame
//
//  Created by ourpalm on 5/30/13.
//
//

#include "SNSActionNode.h"

SNSActionNode::SNSActionNode():m_radius(0.0f), m_speed(0.0f), m_angle(0)
{
	
}

SNSActionNode::~SNSActionNode()
{
	this->unschedule(schedule_selector(SNSActionNode::updateCircle));
}

bool SNSActionNode::init()
{
//	CCSprite* spr = CCSprite::create("Icon.png");
//	spr->setScale(0.8f);
//	spr->setPosition(CCPointZero);
//	addChild(spr);
	return true;
}

#pragma mark - update function

void SNSActionNode::updateCircle(float delta)
{
	CCNode* child = this->getChildByTag(9527);
	if (child) {
		if (m_angle >= 360.0f) m_angle = 0;
		float nowAngel = m_angle * (M_PI / 180.0f);
		child->setPosition(ccp(m_radius * cos(nowAngel), m_radius * sin(nowAngel)));
		m_angle += m_speed;
	}
}

void SNSActionNode::runActionAroundCircle(CCNode* child, float radius, float speed)
{
	m_radius = radius;
	m_speed = speed;
//	child->setPosition(CCPointZero);
	this->addChild(child, 0, 9527);
	this->schedule(schedule_selector(SNSActionNode::updateCircle));
}