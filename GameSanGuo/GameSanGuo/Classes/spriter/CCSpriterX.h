//------------------------------------------------------------------------
//
//	SCMLAnimator : KickStarter project Spriter renderer for cocos2d-x.
//
//	Spriter website : http://www.kickstarter.com/projects/539087245/spriter
//
//	Licensed under the BSD license, see LICENSE in root for details.
// 
//	Copyright (c) 2012 James Hui (a.k.a. Dr.Watson)
// 
//	For latest updates, please visit http://jameshui.com
//
//------------------------------------------------------------------------


#ifndef _CC_SPRITER_X_H_
#define _CC_SPRITER_X_H_

#include <vector>
#include <string>

#include "cocos2d.h"

#include "tinyxml.h"
#include "CCActionInstant.h"

class CCSpriterX;

namespace SCMLHelper
{

	struct File
	{
		File();
		~File();

		void Init(TTiXmlNode *node);

		int id;
		std::string name;
		float width;
		float height;

		cocos2d::CCSprite *sprite;

	};

	class Folder
	{
	public:
		Folder();
		~Folder();
		
		void Init(TTiXmlNode *node);
		
		int GetFileCount();
		File *GetFile(int index);

	private:
		int mId;
		std::string mName;

		std::vector <File *> mFiles;

	};

	struct ObjectRef
	{
		void Init(TTiXmlNode *node);
		int id;
		int timeline;
		int key;
		int z_index;
	};

	struct Object
	{
		void Init(TTiXmlNode *node, CCSpriterX *animator);

		int folder;
		int file;
		float x;
		float y;
		float angle;
		float scaleX;
        float scaleY;
		float pivot_x;
		float pivot_y;
		float alpha;
		int z_index;

		cocos2d::CCSprite *sprite;

	};

	class Key
	{
	public:
		Key();
		~Key();
		
		void Init(TTiXmlNode *node, CCSpriterX *animator);
		
		int GetObjectRefCount();
		ObjectRef *GetObjectRef(int index);
		
		int GetObjectCount();
		Object *GetObject(int index);

		float GetTime();

		bool IsSpinCounterClockwise();

	private:
		int mId;
		float mTime;
		bool mSpinCounterClockwise;

		std::vector <Object *> mObjects;
		std::vector <ObjectRef *> mObjectRefs;
		// will have bones later

	};

	class Timeline
	{
	public:
		Timeline();
		~Timeline();

		void Init(TTiXmlNode *node, CCSpriterX *animator);
		int GetKeyframeCount();
		Key *GetKeyframe(int index);

	private:
		int mId;
		std::vector <Key *> mKeyframes;
	};


	class Animation
	{
	public:
		Animation();
		~Animation();

		void Update(float dt);
		int Init(TTiXmlNode *node, CCSpriterX *animator);
		
		int getmCurrKeyframeinAni();
		
		int gettotallframeinAni();
		
		void Render();

		bool IsDone();
        bool IsLoop();

		void Restart();

		void SetPosition(const cocos2d::CCPoint &pos);
        
		cocos2d::CCPoint GetPosition();
		
        void SetLoop(bool isloop);
        
        //获取总时长
        float getLength() const { return mLength; }
		
		//左右翻转
		void setAnimationFlipX(bool _FlipX);
		
		//上下翻转
		void setAnimationFlipY(bool _FlipY);
		
		//设置透明度
		void setAnimationOpacity(float _Opacity);
		
        void setAnimationOpacity(cocos2d::ccBlendFunc blend);
		int  getAnimaMaxX(int _type);
		
		cocos2d::CCSprite* getSpriteInFrame(int frameID);
		
		int getAtacktnum();
		
		int getFrameNuminAnimator();
		
		bool mDone;

	private:
		int mId;
		std::string mName;
		float mLength;
		bool mLooping;
		
		Timeline *mMainline;
		//当前帧号
		int mCurrKeyframe;

		std::vector <Timeline *> mTimelines;

		float mTimer;

		cocos2d::CCPoint mPosition;

		int Atacktnum;//当前攻击动作的伤害次数，用攻击帧第一帧的图块数替代
		
		//是否翻转参数
		bool FlipX;
		bool FlipY;
		float AOpacity;
        cocos2d::ccBlendFunc blendFunc;
	};


	class Entity
	{
	public:
		Entity();
		~Entity();
		void restart();
		void Update(float dt);
		void Render();
		bool IsDone();
		void SetId(int id);
		void SetName(const char *name);
		void AddAnimation(Animation *animation);

		void SetPosition(const cocos2d::CCPoint &pos);
		cocos2d::CCPoint GetPosition();
		
		void NextAnimation();

        void SetAnimation(int aniID);
        
		int GetAnimation();
        
        bool IsLoop();
		
        //设置是否循环
        void SetLoop(bool isloop);
        
        //获得指定动画时间长度
        float getTimeLength(int aniID);
		
		//左右翻转
		void setEntityFlipX(bool _FlipX);
		//上下翻转
		void setEntityFlipY(bool _FlipY);
		//设置透明度
		void setEntityOpacity(float _Opacity);
		
		int getAnimaMaxX(int type);
		
		cocos2d::CCSprite* getSpriteInFrame(int animaID, int frameID);
		
		int getAtacktnum(int _animationID);
		
		int getFrameNuminAnimator(int _animationID);
		
		int s_isPlayer;
	private:

		int mId;
		std::string mName;

		std::vector <Animation *> mAnimations;

		int mCurrAnimation;
		
		
		//是否翻转参数
		bool FlipX;
		bool FlipY;
		float AOpacity;
	};

}

/*
 *  spriter精灵操作类
 */
class CCSpriterX : public cocos2d::CCNode
{
public:
	~CCSpriterX();
    //默认使用图片加载，设置为true使用plist
	static CCSpriterX* create(const char *filename, bool isPlist = false , bool isPlayer = false);
    static CCSpriterX *create(const char *filename, bool isPlist, cocos2d::CCCallFunc* doneFunc,bool isPlayer);
    bool initWithFile(const char *filename, bool isPlist, bool isPlayer);
    
protected:
    CCSpriterX();
    
public:
	bool IsDone();
    bool IsLoop();
    
    //获取指定帧精灵
    cocos2d::CCSprite* getSprite(int folderId, int fileId);
    
	cocos2d::CCSprite* getSpriteInFrame(int animaID, int frameID);
	
    //设置做点点
	void setPosition(const cocos2d::CCPoint &pos);
    cocos2d::CCPoint GetPosition();
	
    //设置播放动画ID号
   // void setanimaID(int _id);
    virtual void setanimaID(int _id , bool isLoop = true);  //cgpSpine
    
	//获得播放动画ID号
    int getanimaID();
	
    //切换到下一个动画
	void PlayNext();
    
    //是否循环播放
    void setisloop(bool isloop);
    
    //获取动画播放总时长
    float getAnimaTimeLength(int _id, int _aniID) const;
    
    //停止播放
    void stop();
    
    //播放
    void play();
	
	//是否正在播放
	bool isplaying();
	
    //左右翻转
	void setCCSpriterXFlipX(bool _FlipX);
	//上下翻转
	void setCCSpriterXFlipY(bool _FlipY);
	
	//设置透明度
	void setCCSpriterXOpacity(float _Opacity);
	
    //加速动画播放
    void setAnimaSpeed(float _speedTime);
    
	int getAnimaFrameNum();

	int getAtacktnum(int _animationID = 8);
	
	int getAnimaMaxX(int type);
	
	int getFrameNuminAnimator(int _animationID);
    
    //TODO: 重播动画
    void restart();
protected:
    //绘制
	virtual void draw(void);
    
    //更新
	virtual void update(float dt);

private:
    //资源列表
	std::vector <SCMLHelper::Folder *> mFolders;
    
    //动作列表
	std::vector <SCMLHelper::Entity *> mEntities;
    
    //当前播放的动作索引
	int mCurrEntity;
    
    //动画新时间
    float mSpeedTime;
	
	bool isplay;
	
	bool s_isPlayer;
    
    cocos2d::CCCallFunc* doneCallFunc;
};



#endif
