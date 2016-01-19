//------------------------------------------------------------------------
//
//	CCSpriterX : KickStarter project Spriter renderer for cocos2d-x.
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

#include "CCSpriterX.h"
#include "ResourceManager.h"

USING_NS_CC;


namespace SCMLHelper
{

    //dhd 是否从plist加载图片数据
    static bool s_isPlistLoadImage = false;
//	bool s_isPlayer = false;
	///////////////////////////////////////////////////////////////////////////////////

	File::File()
    : sprite(NULL)
    ,name("")
    ,width(0)
    ,height(0)
    ,id(0)
	{
	}


	File::~File()
	{
		if (sprite)
        {
			sprite->release();
        }
	}

	void File::Init(TTiXmlNode *node)
	{
		TTiXmlElement *element = node->ToElement();
        //是否加载成功的标志，如果失败，后面抢救。
        bool isLoaded = false;
		if (element)
		{
			int intValue;
			float floatValue;

			if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
				id = intValue;
			else
				id = 0;

			name = element->Attribute("name");
			int mp = name.rfind('/');
			int mlen = name.length();
			name = name.substr(mp+1, mlen);

			if (element->QueryFloatAttribute("width", &floatValue) == TTiXml_SUCCESS)
				width = floatValue;
			else
				width = 0;

			if (element->QueryFloatAttribute("height", &floatValue) == TTiXml_SUCCESS)
				height = floatValue;
			else
				height = 0;

			if (name.size()>0)
			{
                if ( s_isPlistLoadImage )
                {
                    sprite = CCSprite::createWithSpriteFrameName(name.c_str());
                    isLoaded = sprite ? true : false;
                }
                /*
                else //目前的机制，这里永远不会走到，走到必须坑爹，注释留档。
                {
                    CCString *str = CCString::createWithFormat("%s%s","animationFile/",name.c_str());
                    sprite = CCSprite::create(str->getCString());
                }
                */
                name.clear();
			}
		}
        
        //MM: 万一出错，加载一个public4里的小图片，并且不显示，备案查错。
        if (!isLoaded)
        {
            sprite = CCSprite::createWithSpriteFrameName("down_arrow.png");
            sprite->setVisible(false);
            CCLog("creating image error! name = %s", name.c_str());
        }

        sprite->retain();
	}

	///////////////////////////////////////////////////////////////////////////////////

	Folder::Folder()
		: mId(0)
	{ 
		mFiles.reserve(50); 
	}


	Folder::~Folder()
	{
		int count = mFiles.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mFiles[i]);

		mFiles.clear();

	}


	int Folder::GetFileCount()
	{
		return mFiles.size();

	}

	
	File *Folder::GetFile(int index)
	{
		if (index < (int)mFiles.size())
        {
			return mFiles[index];
        }

		return NULL;

	}


	void Folder::Init(TTiXmlNode *node)
	{
		TTiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;

			if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
				mId= intValue;

            //dhd 目录可以为空
            const char* strName = element->Attribute("name");
			if ( strName ) {
                mName = strName;
            }

			for (TTiXmlNode* fileNode = node->FirstChild(); fileNode; fileNode = fileNode->NextSibling())
			{
				File *file = new File();
				file->Init(fileNode);

				mFiles.push_back(file);
			}

		}

	}

	///////////////////////////////////////////////////////////////////////////////////

	void ObjectRef::Init(TTiXmlNode *node)
	{
		TTiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;

			if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
				id = intValue;
			else
				id = 0;

			if (element->QueryIntAttribute("timeline", &intValue) == TTiXml_SUCCESS)
				timeline = intValue;
			else
				timeline = 0;

			if (element->QueryIntAttribute("key", &intValue) == TTiXml_SUCCESS)
				key = intValue;
			else
				key = 0;

			if (element->QueryIntAttribute("z_index", &intValue) == TTiXml_SUCCESS)
				z_index = intValue;
			else
				z_index = 0;

		}
	}


	///////////////////////////////////////////////////////////////////////////////////

	void Object::Init(TTiXmlNode *node, CCSpriterX *animator)
	{
		sprite = NULL;

		TTiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;
			float floatValue;

			if (element->QueryIntAttribute("folder", &intValue) == TTiXml_SUCCESS)
				folder = intValue;
			else
				folder = 0;

			if (element->QueryIntAttribute("file", &intValue) == TTiXml_SUCCESS)
				file = intValue;
			else
				file = 0;

			if (element->QueryFloatAttribute("x", &floatValue) == TTiXml_SUCCESS)
			{
				x = floatValue;
			}
			else
				x = 0;

			if (element->QueryFloatAttribute("y", &floatValue) == TTiXml_SUCCESS)
			{
				y = floatValue;
			}
			else
				y = 0;

			if (element->QueryFloatAttribute("angle", &floatValue) == TTiXml_SUCCESS)
				angle = floatValue;
			else
				angle = 0;

			if (element->QueryFloatAttribute("scale_x", &floatValue) == TTiXml_SUCCESS)
                scaleX = floatValue;
            else
                scaleX = 1;
			
            if (element->QueryFloatAttribute("scale_y", &floatValue) == TTiXml_SUCCESS)
                scaleY = floatValue;
            else
                scaleY = 1;
			
			
			if (element->QueryFloatAttribute("pivot_x", &floatValue) == TTiXml_SUCCESS)
				pivot_x = floatValue;
			else
				pivot_x = 0;

			if (element->QueryFloatAttribute("pivot_y", &floatValue) == TTiXml_SUCCESS)
				pivot_y = floatValue;
			else
				pivot_y = 1;

			if (element->QueryIntAttribute("z_index", &intValue) == TTiXml_SUCCESS)
				z_index = intValue;
			else
				z_index = 0;

			if (element->QueryFloatAttribute("a", &floatValue) == TTiXml_SUCCESS)
				alpha = floatValue;
			else
				alpha = 1;
			
			
			
			sprite = animator->getSprite(folder, file);

			
		}

	}


	///////////////////////////////////////////////////////////////////////////////////

	Key::Key()
		: mId(0)
		, mTime(0)
		, mSpinCounterClockwise(true)
	{ 
		mObjects.reserve(50);
		mObjectRefs.reserve(50); 
	}


	Key::~Key()
	{
		int count = mObjects.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mObjects[i]);

		mObjects.clear();

		count = mObjectRefs.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mObjectRefs[i]);

		mObjectRefs.clear();

	}


	int Key::GetObjectRefCount()
	{
		return mObjectRefs.size();

	}


	ObjectRef *Key::GetObjectRef(int index)
	{
		if (index < (int)mObjectRefs.size())
			return mObjectRefs[index];

		return NULL;

	}


	int Key::GetObjectCount()
	{
		return mObjects.size();

	}


	Object *Key::GetObject(int index)
	{
		if (index < (int)mObjects.size())
			return mObjects[index];

		return NULL;

	}


	float Key::GetTime()
	{
		return mTime;
	}


	bool Key::IsSpinCounterClockwise()
	{
		return mSpinCounterClockwise;

	}


	void Key::Init(TTiXmlNode *node, CCSpriterX *animator)
	{
		TTiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;
			float floatValue;
			if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
				mId = intValue;

			float time = 0;
			if (element->QueryFloatAttribute("time", &floatValue) == TTiXml_SUCCESS)		// was in milliseconds, convert to seconds instead
				time = floatValue/1000.0f;
			mTime = time;

			if (element->QueryIntAttribute("spin", &intValue) == TTiXml_SUCCESS)
				mSpinCounterClockwise = !(intValue == -1);

			for (TTiXmlNode* objNode = node->FirstChild(); objNode; objNode = objNode->NextSibling())
			{
				element = objNode->ToElement();
				const char *tabObj = element->Value();

				if (strcmp(tabObj, "object_ref")==0)
				{
					ObjectRef *ref = new ObjectRef();
					ref->Init(objNode);

					mObjectRefs.push_back(ref);

				}
				else if (strcmp(tabObj, "object")==0)
				{
					Object *obj = new Object();
					obj->Init(objNode, animator);

					mObjects.push_back(obj);
				}
			}
		}

	}


	///////////////////////////////////////////////////////////////////////////////////
	
	Timeline::Timeline()
		: mId(0)
	{
		mKeyframes.reserve(50);
	}


	Timeline::~Timeline()
	{
		int count = mKeyframes.size();
		for (int i=0;i<count;i++)
		{
			CC_SAFE_DELETE(mKeyframes[i]);

		}
        mKeyframes.clear();
	}


	int Timeline::GetKeyframeCount()
	{
		return mKeyframes.size();

	}


	Key *Timeline::GetKeyframe(int index)
	{
		if (index < (int)mKeyframes.size())
		{
			return mKeyframes[index];
		}

		return NULL;

	}

	void Timeline::Init(TTiXmlNode *node, CCSpriterX *animator)
	{
		int intValue;

		TTiXmlElement *element = node->ToElement();

		if (element)
		{
			if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
				mId = intValue;

			for (TTiXmlNode* keyNode = node->FirstChild(); keyNode; keyNode = keyNode->NextSibling())
			{
				element = keyNode->ToElement();
				if (element)
				{
					Key *keyframe = new Key();

					keyframe->Init(keyNode, animator);

					mKeyframes.push_back(keyframe);
				}
			}

		}

	}

	///////////////////////////////////////////////////////////////////////////////////

	Animation::Animation()
		: mId(0)
		, mCurrKeyframe(0)
		, mMainline(NULL)
		, mDone(false)
		, mTimer(0)
        , mLooping(false)
	{
		mTimelines.reserve(50);
		//是否翻转参数
		FlipX = false;
		FlipY = false;
		AOpacity = 1;
	}


	Animation::~Animation()
	{
		//mMainline做了一操作,但没有在析构中释放//10.23
		//george
		CC_SAFE_DELETE(mMainline);
		int count = mTimelines.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mTimelines[i]);
		mTimelines.clear();
	}


	int Animation::Init(TTiXmlNode *node, CCSpriterX *animator)
	{
		int intValue;
		float floatValue;
		int returnnum = 0;
		TTiXmlElement *element = node->ToElement();

		if (element)
		{
			if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
				mId = intValue;
            
			mName = element->Attribute("name");

			if (element->QueryFloatAttribute("length", &floatValue) == TTiXml_SUCCESS)
				mLength = floatValue/1000.0f;
			// was in milliseconds, convert to seconds instead

//			const char *looping = element->Attribute("looping");
            // was set to "false" in alpha, but in fact looping all the time

			for (TTiXmlNode* lineNode = node->FirstChild(); lineNode; lineNode = lineNode->NextSibling())
			{
				element = lineNode->ToElement();

				const char *tabLine = element->Value();
				if (strcmp(tabLine, "mainline")==0)						// 1 mainline only
				{
					mMainline = new Timeline();
					mMainline->Init(lineNode, animator);

				}
				else if (strcmp(tabLine, "timeline")==0)
				{
					Timeline *timeline = new Timeline();
					timeline->Init(lineNode, animator);

					mTimelines.push_back(timeline);

				}

			}
            
            
            
            
            {
                int count = mMainline->GetKeyframeCount();
                Key *keyframe = mMainline->GetKeyframe(mCurrKeyframe);
                float currTime = keyframe->GetTime();
                
                Key *keyframeNext = NULL;
                
                int next = mCurrKeyframe+1;
                
                if (next > count-1)
                    next = 0;
                
                keyframeNext = mMainline->GetKeyframe(next);
                
                if (keyframeNext)
                {
                    float nextTime = keyframeNext->GetTime();
                    if (next == 0)
                        nextTime = mLength;
                    
                    if (mTimer >= nextTime)
                    {
                        
                        mCurrKeyframe = next;
                        
                        keyframe = keyframeNext;
                        currTime = keyframe->GetTime();
                        
                        next = mCurrKeyframe+1;
                        if (next > count-1&&mLooping)				// looping
                            next = 0;
                        
                        keyframeNext = mMainline->GetKeyframe(next);
                        if (keyframeNext == NULL)
                            return 0;
                        
                        nextTime = keyframeNext->GetTime();
                        if (next == 0)
                            nextTime = mLength;
                        
                    }
                    
                    
                    float t = (mTimer-currTime)/(nextTime-currTime);
                    
					
						
                    int count = keyframe->GetObjectRefCount();//当前帧的图片数量
//					if (strcmp(mName.c_str(), "攻击1")==0)
					{
						Atacktnum = count;
					}
                    for (int i=0;i<count;i++)
                    {
                        ObjectRef *ref = keyframe->GetObjectRef(i);
                        
                        ObjectRef *refNext = keyframeNext->GetObjectRef(i);
                        
                        if (ref && refNext)
                        {
                            
                            Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
                            Object *obj = keyRef->GetObject(0);									// should be only 1 object
                            
                            Key *keyRefNext = mTimelines[refNext->timeline]->GetKeyframe(refNext->key);
                            Object *objNext = keyRefNext->GetObject(0);
                            
                            float x = obj->x+(objNext->x-obj->x)*t;
                            float y = obj->y+(objNext->y-obj->y)*t;
                            float angle = objNext->angle-obj->angle;
                            if (keyRef->IsSpinCounterClockwise())
                            {
                                if (angle < 0)
                                    angle = (objNext->angle+360)-obj->angle;
                            }
                            else
                            {
                                if (angle > 0)
                                {
                                    angle = (objNext->angle-360)-obj->angle;
                                }
                                
                            }
                            
                            if (ref->timeline != refNext->timeline)	
                                t = 0;
                            
                            angle = obj->angle+(angle)*t;
                            
                            if (angle >= 360)
                                angle -= 360;
                            
                            float px = obj->pivot_x+(objNext->pivot_x-obj->pivot_x)*t;
                            float py = obj->pivot_y+(objNext->pivot_y-obj->pivot_y)*t;
                            
                            CCPoint newPos = ccp(mPosition.x+x, mPosition.y+y);
                            obj->sprite->setPosition(newPos);
                            obj->sprite->setRotation(-angle);
                            obj->sprite->setAnchorPoint(ccp(px, py));
                            
                        }
                        
                        
                    }
                }
                
                
            }
            
            
		}
		return returnnum;
	}


	bool Animation::IsDone()
	{
		return mDone;

	}
    
    bool Animation::IsLoop()
    {
        return mLooping;
    }


	void Animation::Restart()
	{
//		mDone = false;
		mTimer = 0;
		mCurrKeyframe = 0;

	}

	cocos2d::CCPoint Animation::GetPosition()
	{
		return mPosition;
	}
	

	void Animation::SetPosition(const cocos2d::CCPoint &pos)
	{
		mPosition = pos;

	}


    void Animation::SetLoop(bool isloop)
    {
        mLooping = isloop;
    }
    
	float lerp(float a, float b, float t){
        return a+(b-a)*t;
    }
	
	void Animation::Update(float dt)
	{
		if (mDone)
		{
			mDone = false;
			if (mLooping)
			{
				Restart();
			}
			return;
		}

		mTimer += dt;
		if (mTimer >= mLength)
		{
			mDone = true;
			if (mLooping)
			{
//				Restart();
				return;
			}
			else
			{
//				mTimer -= dt;
				mCurrKeyframe = mMainline->GetKeyframeCount() -1;
				return;
			}

		}
		//count是得到的总帧数
		int count = mMainline->GetKeyframeCount();
		Key *keyframe = mMainline->GetKeyframe(mCurrKeyframe);
		float currTime = keyframe->GetTime();

		Key *keyframeNext = NULL;

		int next = mCurrKeyframe+1;

		if (next > count-1)		// looping
		{
			if (mLooping)
			{
				next = 0;
			}
			else
			{
				//不做补间，到最后一帧了 用最后一帧做补间  不会乱掉
				next = 0;
				
			}
			
		}
		keyframeNext = mMainline->GetKeyframe(next);
		
		if (keyframeNext)
		{
			float nextTime = keyframeNext->GetTime();
			if (next == 0)
				nextTime = mLength;

			if (mTimer >= nextTime)
			{

				mCurrKeyframe = next;

				keyframe = keyframeNext;
				currTime = keyframe->GetTime();

				next = mCurrKeyframe+1;
				if (next > count-1)				// looping
				{
						next = 0;
				}
					
					
				keyframeNext = mMainline->GetKeyframe(next);
				if (keyframeNext == NULL)
					return;

				nextTime = keyframeNext->GetTime();
				if (next == 0)
					nextTime = mLength;
					
			}


			float t = (mTimer-currTime)/(nextTime-currTime);

			int count = keyframe->GetObjectRefCount();
			for (int i=0;i<count;i++)
			{
				ObjectRef *ref = keyframe->GetObjectRef(i);
			
				ObjectRef *refNext = keyframeNext->GetObjectRef(i);
				
				if (ref)
				{
					if (!refNext)
					{
						refNext = ref;
					}

					Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
					Object *obj = keyRef->GetObject(0);									// should be only 1 object

					Key *keyRefNext = mTimelines[refNext->timeline]->GetKeyframe(refNext->key);
					Object *objNext = keyRefNext->GetObject(0);
					
					CCAssert(objNext != NULL, "objNext should not be null");//10.23//gorge
					
					float angle = objNext->angle-obj->angle;
					if (keyRef->IsSpinCounterClockwise())
					{
						if (angle < 0)
							angle = (objNext->angle+360)-obj->angle;
					}
					else
					{
						if (angle > 0)
						{
							angle = (objNext->angle-360)-obj->angle;
						}

					}

					if (ref->timeline != refNext->timeline)	
						t = 0;

					angle = obj->angle+(angle)*t;

					if (angle >= 360)
						angle -= 360;

					float px = obj->pivot_x+(objNext->pivot_x-obj->pivot_x)*t;
					float py = obj->pivot_y+(objNext->pivot_y-obj->pivot_y)*t;

					obj->sprite->setRotation(-angle);
					obj->sprite->setAnchorPoint(ccp(px, py));
				}
				
				
			}
		}
		

	}

	int Animation::getmCurrKeyframeinAni()
	{
		return mCurrKeyframe;
	}
	
	int Animation::gettotallframeinAni()
	{
		//count是得到的总帧数
		int count = mMainline->GetKeyframeCount();
		return count;
	}
	
	void Animation::Render()
	{
		Key *keyframe = mMainline->GetKeyframe(mCurrKeyframe);
		//count是得到的总帧数
//		int Keyframecount = mMainline->GetKeyframeCount();
		
		int count = keyframe->GetObjectRefCount();
		for (int i=0;i<count;i++)
		{
			ObjectRef *ref = keyframe->GetObjectRef(i);

			if (ref)
			{

				Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
				Object *obj = keyRef->GetObject(0);									// should be only 1 object
				if (FlipX)
				{
					obj->sprite->setFlipX(true);
				}
				else
				{
					obj->sprite->setFlipX(false);
				}
                
				if (FlipY)
				{
					obj->sprite->setFlipY(true);
				}
                else
                {
                    obj->sprite->setFlipY(false);
                }
				
				
				
				float x = obj->x/CCDirector::sharedDirector()->getContentScaleFactor();
				float y = obj->y/CCDirector::sharedDirector()->getContentScaleFactor();
				float scaleX = obj->scaleX;
                float scaleY = obj->scaleY;
				CCPoint newPos = ccp(x, y);
                
				if (FlipX)
				{
					float sptWidth = obj->sprite->getContentSize().width;
					float contentX = (1 - scaleX) * sptWidth;
					newPos.x = contentX - newPos.x - sptWidth;
				}
                
                if (FlipY) {
                    float sptHeight = obj->sprite->getContentSize().height;
                    float contentY = (1 - scaleY) * sptHeight;
                    newPos.y = contentY - newPos.y - sptHeight;
//                    newPos = ccp(newPos.x, contentY - y  + 70); //以前通过挪Y轴实现翻转是错误的。
                }
                
                obj->sprite->setPosition(newPos);
				obj->sprite->setScaleX(obj->scaleX);
                obj->sprite->setScaleY(obj->scaleY);
				obj->sprite->setOpacity(obj->alpha*255*AOpacity);
                
				obj->sprite->visit();
			}
		}
	}

	//左右翻转
	void Animation::setAnimationFlipX(bool _FlipX)
	{
		FlipX = _FlipX;
	}
	
	//上下翻转
	void Animation::setAnimationFlipY(bool _FlipY)
	{
		FlipY = _FlipY;
	}
	
	void Animation::setAnimationOpacity(float _Opacity)
	{
		AOpacity = _Opacity;
	}
    
    void Animation::setAnimationOpacity(cocos2d::ccBlendFunc blend)
	{
		blendFunc = blend;
	}
	
	int Animation::getAnimaMaxX(int _type)//0 得到左值  1  得到右值
	{
		int _maxx = 0;
		if (_type)
		{
			_maxx = 0;
		}
		else
		{
			_maxx = 10000;
		}
		Key *keyframe = mMainline->GetKeyframe(mCurrKeyframe);
		int count = keyframe->GetObjectRefCount();
		for (int i=0;i<count;i++)
		{
			ObjectRef *ref = keyframe->GetObjectRef(i);

			if (ref)
			{
				int tempmax = 0;
				
				Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
				Object *obj = keyRef->GetObject(0);// should be only 1 object
				//keyRef可能出现空指针//10.23
				if (obj)
				{
				if (_type) //  得到右值
				{
					if (FlipX)
					{
						CCPoint newPoint = ccp(obj->sprite->getPositionX(), mPosition.y);
						tempmax = mPosition.x+newPoint.x+obj->sprite->getContentSize().width;
						if (tempmax>_maxx)
						{
							_maxx = tempmax;
						}
					}
					else
					{
						CCPoint newPoint = ccp(obj->sprite->getPositionX(), mPosition.y);
						tempmax = mPosition.x+newPoint.x + obj->sprite->getContentSize().width;
						if (tempmax>_maxx)
						{
							_maxx = tempmax;
						}
					}
				}
				else //  得到左值
				{
					if (FlipX)
					{
						CCPoint newPoint = ccp(obj->sprite->getPositionX(), mPosition.y);
						tempmax = mPosition.x+newPoint.x;
						if (tempmax<_maxx)
						{
							_maxx = tempmax;
						}
					}
					else
					{
						CCPoint newPoint = ccp(obj->sprite->getPositionX(), mPosition.y);
						tempmax = mPosition.x+newPoint.x;
						if (tempmax<_maxx)
						{
							_maxx = tempmax;
						}
					}
				}
				}//10.23//end george
				
			}
		}
		return _maxx;
	}
	
	CCSprite* Animation::getSpriteInFrame(int frameID)
	{
		Key *keyframe = mMainline->GetKeyframe(frameID);
		//count是得到的总帧数
		//		int Keyframecount = mMainline->GetKeyframeCount();
		
//		int count = keyframe->GetObjectRefCount();
		ObjectRef *ref = keyframe->GetObjectRef(0);
		
		if (ref)
		{
			
			Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
			Object *obj = keyRef->GetObject(0);									// should be only 1 object
			return obj->sprite;
		}
		return NULL;
	}

	
	int Animation::getAtacktnum()
	{
		return Atacktnum;
	}
	
	int Animation::getFrameNuminAnimator()
	{
		return mMainline->GetKeyframeCount();
	}
	///////////////////////////////////////////////////////////////////////////////////

	Entity::Entity()
		: mCurrAnimation(0)
		, mId(0)
	{
		mAnimations.reserve(50);
		FlipX = false;
		FlipY = false;
		AOpacity = 1;
		//是否翻转参数
	};


	Entity::~Entity()
	{
		int count = mAnimations.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mAnimations[i]);

		mAnimations.clear();

	}


	void Entity::Update(float dt)
	{
		int _speed = 1;
		Animation *animation = mAnimations[mCurrAnimation];
		if (s_isPlayer)
		{
			if (animation->IsDone() &&
                !animation->IsLoop() &&
                mCurrAnimation != 0)//结束 并且不是播放的胜利动作
			{
                if (mCurrAnimation == 4) {
                    mCurrAnimation = 5;
                    SetLoop(false);
                }else if(mCurrAnimation == 8){
                    mCurrAnimation = 9;
                    SetLoop(false);
                }
                else{
                    if(mCurrAnimation == 5 ||
                       mCurrAnimation == 9)
                    {
                        mCurrAnimation = 1;
                        SetLoop(true);
                    }else{
                        mCurrAnimation = 0;
                        SetLoop(true);
                    }
                }
			}
		}
		animation->Update(dt*_speed);

	}

	bool Entity::IsDone()
	{
		Animation *animation = mAnimations[mCurrAnimation];
		 return animation->IsDone();
		
	}
	

	void Entity::Render()
	{
		Animation *animation = mAnimations[mCurrAnimation];
		animation->Render();

	}


	cocos2d::CCPoint Entity::GetPosition()
	{
		return  mAnimations[mCurrAnimation]->GetPosition();
	}
	
	void Entity::SetPosition(const CCPoint &pos)
	{
		int count = mAnimations.size();
		for (int i=0;i<count;i++)
			mAnimations[i]->SetPosition(pos);

	}


	void Entity::NextAnimation()
	{
		mCurrAnimation++;
		if (mCurrAnimation >= (int)mAnimations.size())
			mCurrAnimation = 0;

		Animation *animation = mAnimations[mCurrAnimation];
		animation->Restart();
		if (FlipX)
		{
			animation->setAnimationFlipX(FlipX);
		}
		if (FlipY)
		{
			animation->setAnimationFlipY(FlipY);
		}
	}


    void Entity::SetAnimation(int aniID)
	{
		if (aniID < (int)mAnimations.size())
        mCurrAnimation = aniID;
            
		Animation *animation = mAnimations[mCurrAnimation];
		animation->Restart();
		animation->mDone = false;
		if (FlipX)
		{
			animation->setAnimationFlipX(FlipX);
		}
		if (FlipY)
		{
			animation->setAnimationFlipY(FlipY);
		}
        
	}
    
	int Entity::GetAnimation()
	{
        return mCurrAnimation;
	}
	
    void Entity::SetLoop(bool isloop)
    {
		Animation *animation = mAnimations[mCurrAnimation];
		animation->SetLoop(isloop);
    }
    
    bool Entity::IsLoop()
	{
		Animation *animation = mAnimations[mCurrAnimation];
        return animation->IsLoop();
		
	}
    
	void Entity::SetId(int id)
	{
		mId = id;

	}


	void Entity::SetName(const char *name)
	{
		mName = name;

	}


	void Entity::AddAnimation(Animation *animation)
	{
		mAnimations.push_back(animation);

	}
    
    float Entity::getTimeLength(int aniID)
    {
        
        return (mAnimations[aniID]->getLength());
    }

	//左右翻转
	void Entity::setEntityFlipX(bool _FlipX)
	{
		FlipX = _FlipX;
		Animation *animation = mAnimations[mCurrAnimation];
		animation->setAnimationFlipX(_FlipX);
	}
	
	//上下翻转
	void Entity::setEntityFlipY(bool _FlipY)
	{
		FlipY = _FlipY;
		Animation *animation = mAnimations[mCurrAnimation];
		animation->setAnimationFlipY(_FlipY);
	}
	
	//设置透明度
	void Entity::setEntityOpacity(float _Opacity)
	{
		AOpacity = _Opacity;
		Animation *animation = mAnimations[mCurrAnimation];
		animation->setAnimationOpacity(_Opacity);
	}
	
	int Entity::getAnimaMaxX(int type)
	{
		int _maxx = 0;
		Animation *animation = mAnimations[mCurrAnimation];
		_maxx = animation->getAnimaMaxX(type);
		return _maxx;
	}
	
	CCSprite* Entity::getSpriteInFrame(int animaID, int frameID)
	{
		Animation *animation = mAnimations[animaID];
		return  animation->getSpriteInFrame(frameID);
	}
	
	int Entity::getAtacktnum(int _animationID)
	{
		Animation *animation = mAnimations[_animationID];
		return  animation->getAtacktnum();
	}
	
	int Entity::getFrameNuminAnimator(int _animationID)
	{
		Animation *animation = mAnimations[_animationID];
		return animation->getFrameNuminAnimator();
	}
}

///////////////////////////////////////////////////////////////////////////////////

using namespace SCMLHelper;


CCSpriterX::CCSpriterX() : mCurrEntity(0), s_isPlayer(false)
{
	mFolders.reserve(50);
	mEntities.reserve(50);
    mSpeedTime = 1.0f;
	isplay = false;
    doneCallFunc = NULL;
}

CCSpriterX::~CCSpriterX()
{
    int count = mFolders.size();
    for (int i=0;i<count;i++)
        CC_SAFE_DELETE(mFolders[i]);
    
    mFolders.clear();
	//george
    count = mEntities.size();
    for (int i=0;i<count;i++)
	{
        CC_SAFE_DELETE(mEntities[i]);//entity内存的new出的animation,需要先对mEntities[i]进行释放clear//10.23
    }
	mEntities.clear();
}

CCSpriterX *CCSpriterX::create(const char *filename, bool isPlist , bool isPlayer)
{
	CCSpriterX *animator = new CCSpriterX();
	if (animator && animator->initWithFile(filename, isPlist , isPlayer))
	{
		animator->autorelease();
		return animator;
	}
	CC_SAFE_DELETE(animator);
	return NULL;
}

CCSpriterX *CCSpriterX::create(const char *filename, bool isPlist ,cocos2d::CCCallFunc* doneFunc ,bool isPlayer )
{
	CCSpriterX *animator = new CCSpriterX();
	if (animator && animator->initWithFile(filename, isPlist , isPlayer))
	{
		animator->autorelease();
        animator->doneCallFunc = doneFunc;
        animator->doneCallFunc->retain();
		return animator;
	}
	CC_SAFE_DELETE(animator);
	return NULL;
}

void CCSpriterX::update(float dt)
{
	if (dt > 0.0167f)
		dt = 0.0167f;

	Entity *entity = mEntities[mCurrEntity];
	entity->s_isPlayer = s_isPlayer;
	entity->Update(dt*mSpeedTime);
    
    if (this->IsDone() &&
        !this->IsLoop() &&
        this->getanimaID() == 0) {
        if (doneCallFunc != NULL) {
            doneCallFunc->update(dt);
            doneCallFunc->release();
        }
//        this->removeAllChildrenWithCleanup(true);
        this->removeFromParentAndCleanup(true);
        
    }

}

void CCSpriterX::draw(void)
{
	Entity *entity = mEntities[mCurrEntity];
	entity->Render();

}

bool CCSpriterX::IsDone()
{
	Entity *entity = mEntities[mCurrEntity];
	return entity->IsDone();
	
}

void CCSpriterX::PlayNext()
{
	Entity *entity = mEntities[mCurrEntity];
	entity->NextAnimation();

}

void CCSpriterX::setisloop(bool isloop)
{
    Entity *entity = mEntities[mCurrEntity];
    entity->SetLoop(isloop);
}

bool CCSpriterX::IsLoop()
{
    Entity *entity = mEntities[mCurrEntity];
	return entity->IsLoop();
}

CCSprite *CCSpriterX::getSprite(int folderId, int fileId)
{
	if (folderId < (int)mFolders.size())
	{
		Folder *folder = mFolders[folderId];

		if (folder)
		{
			File *file = folder->GetFile(fileId);

			if (file)
				return file->sprite;
		}
	}

	return NULL;
}

CCSprite *CCSpriterX::getSpriteInFrame(int animaID, int frameID)
{
	Entity *entity = mEntities[mCurrEntity];
	
    return entity->getSpriteInFrame(animaID, frameID);
}


void CCSpriterX::setPosition(const cocos2d::CCPoint &pos)
{
	CCNode::setPosition(pos);

	Entity *entity = mEntities[mCurrEntity];
	entity->SetPosition(pos);

}

cocos2d::CCPoint CCSpriterX::GetPosition()
{
	Entity *entity = mEntities[mCurrEntity];
	return entity->GetPosition();
}

//cgpSpine
void CCSpriterX::setanimaID(int _id, bool isLoop)
{
    Entity *entity = mEntities[mCurrEntity];
    entity->SetAnimation(_id);
}

int CCSpriterX::getanimaID()
{
	int aniID = 0;
    Entity *entity = mEntities[mCurrEntity];
    aniID = entity->GetAnimation();
	return aniID;
}

bool CCSpriterX::initWithFile(const char *filename, bool isPlist ,bool isPlayer)
{
    setAnchorPoint(ccp(0, 0));
    
    //MMDEBUG: BGN
    
//    CCLayerColor* layer = CCLayerColor::create(ccc4(0, 255, 0, 255), 60, 60);
//    layer->setPosition(CCPointZero);
//    layer->setAnchorPoint(CCPointZero);
//    layer->ignoreAnchorPointForPosition(false);
//    this->addChild(layer, -2);
    
    //MMDEBUG: END
    
    
	mCurrEntity = 0;

	unsigned long filesize;
    //modified by cgp
	//const char *path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(filename);
    //char *buffer = (char *)CCFileUtils::sharedFileUtils()->getFileData(path, "rb", lesize);
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	char *buffer = (char *)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &filesize);

	if (buffer == NULL)
		return false;

    //全局设置的，是否使用plist方式加载图片，每次都会设置
    s_isPlistLoadImage = isPlist;
	s_isPlayer = isPlayer;
	
	TTiXmlDocument doc;
    
	doc.Parse(buffer);

	TTiXmlNode *root = doc.FirstChild("spriter_data"); 
	if (root)
	{
		
		TTiXmlElement *element = root->ToElement();
//		const char *version = element->Attribute("scml_version");
//		const char *generator = element->Attribute("generator");
//		const char *generatorVersion = element->Attribute("generator_version");
//        CCLog("SCML version:   %s",version);
//        CCLog("SCML generator: %s",generator);
//        CCLog("SCML generatorVersion: %s",generatorVersion);
        
        

		for (TTiXmlNode* entityNode = root->FirstChild(); entityNode; entityNode = entityNode->NextSibling())
		{
			element = entityNode->ToElement();

			if (element)
			{
				const char *tab = element->Value();

				if (strcmp(tab, "folder")==0)
				{
					Folder *folder = new Folder();
                    
					folder->Init(entityNode);

					mFolders.push_back(folder);

				}
                
                //<entity> </entity>                
				else if (strcmp(tab, "entity")==0)
				{
					int intValue;
					Entity *entity = new Entity();

					if (element->QueryIntAttribute("id", &intValue) == TTiXml_SUCCESS)
						entity->SetId(intValue);

					entity->SetName(element->Attribute("name"));

					for (TTiXmlNode* animationNode = entityNode->FirstChild(); animationNode; animationNode = animationNode->NextSibling())
					{
						Animation *animation = new Animation();
						
						 animation->Init(animationNode, this);
						entity->AddAnimation(animation);
						
					}

					mEntities.push_back(entity);

				}

			}

		}

	}

	CC_SAFE_DELETE_ARRAY(buffer);
    
    //默认就开始播放
//    play();
//	mSpeedTime = 0.1f;
    //MMDEBUG: BGN STAT
    ResourceManager::sharedInstance()->addFileStat(filename, kFT_SCML);
    //MMDEBUG: END STAT
	return true;

}

float CCSpriterX::getAnimaTimeLength(int _id, int _aniID) const
{
    return mEntities[mCurrEntity]->getTimeLength(_aniID);
}

//停止播放
void CCSpriterX::stop()
{
    this->unscheduleUpdate();
    //让动画重置
    this->setanimaID(mCurrEntity);
	isplay = false;
}

//播放
void CCSpriterX::play()
{
    this->setPosition(this->getPosition());
    this->scheduleUpdate();
	isplay = true;
}

bool CCSpriterX::isplaying()
{
	return isplay;
}

//左右翻转
void CCSpriterX::setCCSpriterXFlipX(bool _FlipX)
{
	mEntities[mCurrEntity]->setEntityFlipX(_FlipX);
}

//上下翻转
void CCSpriterX::setCCSpriterXFlipY(bool _FlipY)
{
    mEntities[mCurrEntity]->setEntityFlipY(_FlipY);
}

//设置透明度
void CCSpriterX::setCCSpriterXOpacity(float _Opacity)
{
	mEntities[mCurrEntity]->setEntityOpacity(_Opacity);
}

void CCSpriterX::setAnimaSpeed(float _speedTime)
{
    mSpeedTime = _speedTime;
}

//未完成 稍等
int CCSpriterX::getAnimaFrameNum()
{
    return 0;
//	return mEntities[mCurrEntity]->getTimeLength(_aniID);
}

int CCSpriterX::getAtacktnum(int _animationID)
{
//	return Atacktnum;
	 Entity *entity = mEntities[mCurrEntity];
	return entity->getAtacktnum(_animationID);
}

//无调用的地方
int CCSpriterX::getAnimaMaxX(int type)
{
	int _maxx = 0;
	_maxx = mEntities[mCurrEntity]->getAnimaMaxX(type);
	return _maxx;
}

int CCSpriterX::getFrameNuminAnimator(int _animationID)
{
	Entity *entity = mEntities[mCurrEntity];
	return entity->getFrameNuminAnimator(_animationID);
}

void CCSpriterX::restart()
{
    //让动画重置
    stop();
    play();
}