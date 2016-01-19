//
//  SNSColorSprite.h
//  tableView
//	version 2.0
//  Created by yang jie on 28/11/2011.
//  Copyright 2011 ourpalm.com. All rights reserved.
//

#ifndef __MM_GRAY_SCALE_SPRITE_H__
#define __MM_GRAY_SCALE_SPRITE_H__

//#include "sprite_nodes/CCSprite.h"
//#include "textures/CCTexture2D.h"
//#include "cocoa/CCGeometry.h"
#include "cocos2d.h"

enum SNSColorfulType {
	SNSColorfulTypeStandard = 0,			// 正常图
	SNSColorfulTypeGrayscale,				// 灰度图
	SNSColorfulTypeSepiaTone,				// Sephia特效（旧照片）
	SNSColorfulTypeNegative,				// 反相
	SNSColorfulTypeGaussianBlur,			// 高斯模糊
	SNSColorfulTypeMedianBlur,				// Median模糊
	SNSColorfulTypeSharpen,					// 锐化
	SNSColorfulTypeDilate,					// 膨胀
	SNSColorfulTypeErode,					// 腐蚀
	SNSColorfulTypeLaplacianEdgeDetection,	// 拉普拉斯边缘检测
	SNSColorfulTypeFlagFlutter,				// 旗子飘动的shader
};

USING_NS_CC;

class SNSColorfulSprite : public CCSprite
{
public:
	SNSColorfulSprite();
	virtual ~SNSColorfulSprite();
	static SNSColorfulSprite* create(const char* pszFileName);
	static SNSColorfulSprite* createWithSpriteFrameName(const char* pszFileName);
	bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
	virtual void draw();
	virtual void setPosition(CCPoint pos);
	virtual void update(float dt);
    
	CC_PROPERTY(bool, m_isUseEffect, UseEffect);

public:
	void displayColorfulWithType(SNSColorfulType type);

private:
	// 计算高斯模糊主要用到的偏移函数（暂时用5*5的方式，性能折中）
	void genTexCoordOffsets(GLfloat width, GLfloat height, GLfloat step = 1.0f);
	
private:
	SNSColorfulType				m_type;						// shader类型
	float						m_time;
	
	/*
	 * 状态机控制bool
	 */
	bool						m_isNeedTime;				// 是否需要传时间进去
	bool						m_isNeedOffset;				// 是否需要传texCoordOffset进去
	// 给shader用的一些变量
	GLuint						uniformTime;
	GLuint						uniformOffset;
	
	const GLint tcOffsetColumns;
	const GLint tcOffsetRows;
	GLfloat texCoordOffsets[50];
};

#endif // __MM_GRAY_SCALE_SPRITE_H__