//
//  SNSColorSprite.h
//  tableView
//	version 2.0
//  Created by yang jie on 28/11/2011.
//  Copyright 2011 ourpalm.com. All rights reserved.
//

#include "SNSColorfulSprite.h"

SNSColorfulSprite::SNSColorfulSprite():m_isUseEffect(true), m_type(SNSColorfulTypeStandard),
m_time(0), m_isNeedTime(false), m_isNeedOffset(false),tcOffsetColumns(5), tcOffsetRows(5), uniformTime(0), uniformOffset(0)
{
	
}


SNSColorfulSprite::~SNSColorfulSprite()
{
	
}

SNSColorfulSprite* SNSColorfulSprite::create( const char* pszFileName )
{
	SNSColorfulSprite* pSprGrayScale = new SNSColorfulSprite;
	if (pSprGrayScale && pSprGrayScale->initWithFile(pszFileName)) {
		pSprGrayScale->autorelease();
		return pSprGrayScale;
	}
	CC_SAFE_DELETE(pSprGrayScale);
	return NULL;
}

SNSColorfulSprite* SNSColorfulSprite::createWithSpriteFrameName(const char* pszFileName)
{
	SNSColorfulSprite* pSprGrayScale = new SNSColorfulSprite;
	if (pSprGrayScale && pSprGrayScale->initWithSpriteFrameName(pszFileName)) {
		pSprGrayScale->autorelease();
		return pSprGrayScale;
	}
	CC_SAFE_DELETE(pSprGrayScale);
	return NULL;
}

#pragma mark - overwrite father's function

bool SNSColorfulSprite::initWithTexture( cocos2d::CCTexture2D* pTexture, const cocos2d::CCRect& tRect )
{
	do {
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));

		// 默认启用灰度图
		this->displayColorfulWithType(SNSColorfulTypeGrayscale);
		
		return true;
	} while (0);
	return false;
}

void SNSColorfulSprite::update(float dt)
{
	if (m_isNeedTime) {
		m_time += dt;
	}
}

void SNSColorfulSprite::draw()
{
	ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	getShaderProgram()->use();
	getShaderProgram()->setUniformsForBuiltins();
    //setUniformForModelViewProjectionMatrix();
	
	ccGLBindTexture2D( this->getTexture()->getName());

//	glClearColor(0, 0, 0, 0);
	//
	// Uniforms
	//
	
//	this->getShaderProgram()->setUniformLocationWith2f(uniformCenter, m_center.x, m_center.y);
//	this->getShaderProgram()->setUniformLocationWith2f(uniformResolution, m_resolution.x, m_resolution.y);
	if (m_isNeedTime) this->getShaderProgram()->setUniformLocationWith1f(uniformTime, m_time);
	if (m_isNeedOffset) this->getShaderProgram()->setUniformLocationWith2fv(uniformOffset, texCoordOffsets, 25);
	
	//
	// Attributes
	//
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CC_INCREMENT_GL_DRAWS(1);
	
	CHECK_GL_ERROR_DEBUG();
}

void SNSColorfulSprite::setPosition(CCPoint pos)
{
	CCSprite::setPosition(pos);
//	m_center = (ccVertex2F) { m_tPosition.x * CC_CONTENT_SCALE_FACTOR(), m_tPosition.y * CC_CONTENT_SCALE_FACTOR() };
}

#pragma mark - control function

void SNSColorfulSprite::displayColorfulWithType(SNSColorfulType type)
{
	if (type != SNSColorfulTypeStandard) {
		m_type = type;
	}
	//bool isNeedComputing = false;
	// 顶点shader
	GLchar* pszVertSource = (GLchar*)"	\n\
	attribute vec4 a_position;	\n\
	attribute vec2 a_texCoord;	\n\
	attribute vec4 a_color;	\n\
	\n\
	uniform mat4 u_MVPMatrix;	\n\
	\n\
	#ifdef GL_ES	\n\
	varying lowp vec4 v_fragmentColor;	\n\
	varying mediump vec2 v_texCoord;	\n\
	#else	\n\
	varying vec4 v_fragmentColor;	\n\
	varying vec2 v_texCoord;	\n\
	#endif	\n\
	\n\
	void main() { \n\
		gl_Position = u_MVPMatrix * a_position;	\n\
		v_fragmentColor = a_color;	\n\
		v_texCoord = a_texCoord;	\n\
	}";
	
	/*
	 * 片段shader，默认是正常图
	 */
	m_isNeedTime = false;
	m_isNeedOffset = false;
	GLchar* pszFragSource = (GLchar *)" \n\
	#ifdef GL_ES \n\
	precision mediump float; \n\
	#endif \n \
	uniform sampler2D	u_texture; \n\
	varying vec2		v_texCoord; \n\
	varying vec4		v_fragmentColor; \n\
	void main(void) { \n\
		gl_FragColor = texture2D(u_texture, v_texCoord); \n\
	}";
	switch (type) {
		case SNSColorfulTypeStandard:
		{
			// 正常图
			
		}
			break;
		case SNSColorfulTypeGrayscale:
		{
			// 灰度图
			m_isNeedTime = false;
			m_isNeedOffset = false;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			void main(void) { \n\
				float alpha = texture2D(u_texture, v_texCoord).a; \n \
				float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
				gl_FragColor = vec4(grey, grey, grey, alpha); \n \
			}";
		}
			break;
		case SNSColorfulTypeSepiaTone:
		{
			// Sephia特效（旧照片）
			m_isNeedTime = false;
			m_isNeedOffset = false;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			void main(void) { \n\
				float alpha = texture2D(u_texture, v_texCoord).a; \n \
				float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
				gl_FragColor = vec4(grey * vec3(1.2, 1.0, 0.8), alpha); \n \
			}";
		}
			break;
		case SNSColorfulTypeNegative:
		{
			// 反相
			m_isNeedTime = false;
			m_isNeedOffset = false;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			void main(void) { \n\
				float alpha = texture2D(u_texture, v_texCoord).a; \n \
				vec4 texMapColour = texture2D(u_texture, v_texCoord); \n\
				gl_FragColor = vec4(1.0 - texMapColour.rgb, alpha); \n\
			}";
		}
			break;
		case SNSColorfulTypeGaussianBlur:
		{
			// 高斯模糊
			m_isNeedTime = false;
			m_isNeedOffset = true;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform vec2		tcOffset[25]; \n\
			void main(void) { \n\
				vec4 sample[25]; \n\
				for (int i = 0; i < 25; i++) { \n\
				sample[i] = texture2D(u_texture, v_texCoord + tcOffset[i]); \n\
				} \n\
				gl_FragColor = ( \n\
				(1.0  * (sample[0] + sample[4]  + sample[20] + sample[24])) + \n\
				(4.0  * (sample[1] + sample[3]  + sample[5]  + sample[9] + sample[15] + sample[19] + sample[21] + sample[23])) + \n\
				(7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) + \n\
				(16.0 * (sample[6] + sample[8]  + sample[16] + sample[18])) + \n\
				(26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) + \n\
				(41.0 * sample[12]) \n\
				) / 273.0; \n\
			}";
			//isNeedComputing = true;
		}
			break;
		case SNSColorfulTypeMedianBlur:
		{
			// Median模糊
			m_isNeedTime = false;
			m_isNeedOffset = true;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform vec2		tcOffset[25]; \n\
			void main(void) { \n\
				gl_FragColor = vec4(0.0); \n\
				for (int i = 0; i < 25; i++) { \n\
					l_FragColor += texture2D(u_texture, v_texCoord + tcOffset[i]); \n\
				} \n\
				gl_FragColor *= 0.04; \n\
			}";
			//isNeedComputing = true;
		}
			break;
		case SNSColorfulTypeSharpen:
		{
			// 锐化
			m_isNeedTime = false;
			m_isNeedOffset = true;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform vec2		tcOffset[25]; \n\
			void main(void) { \n\
				vec4 sample[25]; \n\
				for (int i = 0; i < 25; i++) { \n\
					sample[i] = texture2D(u_texture, v_texCoord + tcOffset[i]); \n\
				} \n\
				gl_FragColor = 25.0 * sample[12]; \n\
				for (int i = 0; i < 25; i++) { \n\
					if (i != 12) { \n\
						gl_FragColor -= sample[i]; \n\
					} \n\
				} \n\
			}";
			//isNeedComputing = true;
		}
			break;
		case SNSColorfulTypeDilate:
		{
			// 膨胀
			m_isNeedTime = false;
			m_isNeedOffset = true;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform vec2		tcOffset[25]; \n\
			void main(void) { \n\
				vec4 sample[25]; \n\
				vec4 maxValue = vec4(0.0); \n\
				for (int i = 0; i < 25; i++) { \n\
					sample[i] = texture2D(u_texture, v_texCoord + tcOffset[i]); \n\
					maxValue = max(sample[i], maxValue); \n\
				} \n\
				gl_FragColor = maxValue; \n\
			}";
			//isNeedComputing = true;
		}
			break;
		case SNSColorfulTypeErode:
		{
			// 腐蚀
			m_isNeedTime = false;
			m_isNeedOffset = true;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform vec2		tcOffset[25]; \n\
			void main(void) { \n\
				vec4 sample[25]; \n\
				vec4 minValue = vec4(1.0); \n\
				for (int i = 0; i < 25; i++) { \n\
					sample[i] = texture2D(u_texture, v_texCoord + tcOffset[i]); \n\
					minValue = min(sample[i], minValue); \n\
				} \n\
				gl_FragColor = minValue; \n\
			}";
			//isNeedComputing = true;
		}
			break;
		case SNSColorfulTypeLaplacianEdgeDetection:
		{
			// 拉普拉斯边缘检测
			m_isNeedTime = false;
			m_isNeedOffset = true;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform vec2		tcOffset[25]; \n\
			void main(void) { \n\
				vec4 sample[25]; \n\
				for (int i = 0; i < 25; i++) { \n\
					sample[i] = texture2D(u_texture, v_texCoord + tcOffset[i]); \n\
				} \n\
				gl_FragColor = 24.0 * sample[12]; \n\
				for (int i = 0; i < 25; i++) { \n\
					if (i != 12) {\n\
						gl_FragColor -= sample[i]; \n\
					} \n\
				} \n\
			}";
			//isNeedComputing = true;
		}
			break;
		case SNSColorfulTypeFlagFlutter:
		{
			// 旗子飘动的shader
			m_isNeedTime = true;
			m_isNeedOffset = false;
			pszFragSource = (GLchar *)" \n\
			#ifdef GL_ES \n\
			precision mediump float; \n\
			#endif \n \
			uniform sampler2D	u_texture; \n\
			varying vec2		v_texCoord; \n\
			varying vec4		v_fragmentColor; \n\
			uniform float		u_time; \n\
			const float speed = 2.0; \n\
			const float bendFactor = 0.2; \n\
			void main(void) { \n\
				float alpha = texture2D(u_texture, v_texCoord).a; \n\
				float height = v_texCoord.y; \n\
				float offset = pow(height, 5.5); \n\
				offset *= (sin(u_time * speed) * bendFactor); \n\
				vec3 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y))).rgb; \n\
				gl_FragColor = vec4(normalColor, alpha); \n\
			}";
		}
			break;
		default:
			break;
	}
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(pszVertSource, pszFragSource);
	this->setShaderProgram(pProgram);
	pProgram->release();
	CHECK_GL_ERROR_DEBUG();
	
	pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	CHECK_GL_ERROR_DEBUG();
	
	pProgram->link();
	CHECK_GL_ERROR_DEBUG();
	
	pProgram->updateUniforms();
	CHECK_GL_ERROR_DEBUG();
	
	if (m_isNeedTime) uniformTime = glGetUniformLocation( pProgram->getProgram(), "u_time");
	if (m_isNeedOffset) uniformOffset = glGetUniformLocation( pProgram->getProgram(), "tcOffset");
	CHECK_GL_ERROR_DEBUG();
	
	if (m_isNeedOffset) {
		genTexCoordOffsets(getContentSize().width, getContentSize().height);
	}
	if (m_isNeedTime) {
		schedule(schedule_selector(SNSColorfulSprite::update));
	} else {
		unschedule(schedule_selector(SNSColorfulSprite::update));
	}
}

#pragma mark - set & get method

void SNSColorfulSprite::setUseEffect(bool var)
{
	m_isUseEffect = var;
	if (m_isUseEffect) {
		displayColorfulWithType(m_type);
	} else {
		displayColorfulWithType(SNSColorfulTypeStandard);
	}
}

bool SNSColorfulSprite::getUseEffect()
{
	return m_isUseEffect;
}

#pragma mark - computing function

void SNSColorfulSprite::genTexCoordOffsets(GLfloat width, GLfloat height, GLfloat step/* = 1.0f*/) // 改变这个值可以修改像素取样...
{
	// 可以改成step *，那样会得到另一种模糊效果
    float xInc = step / width;
    float yInc = step / height;
	
    for (int i = 0; i < tcOffsetColumns; i++) {
        for (int j = 0; j < tcOffsetRows; j++) {
            texCoordOffsets[(((i * 5) + j) << 1) + 0] = (-2.0f * xInc) + ((GLfloat)i * xInc);
            texCoordOffsets[(((i * 5) + j) << 1) + 1] = (-2.0f * yInc) + ((GLfloat)j * yInc);
        }
    }
}
