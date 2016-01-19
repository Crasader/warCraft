--[[
activity 消費排名
by:angel
2015-2-18
--]]

--引入了工程中预定义的字体、颜色值的lua版本。
require "lualibs/common.lua"


function __G__TRACKBACK__(msg)
    print("----------------------------------------")  
    print("LUA ERROR: " .. tostring(msg) .. "\n")  
    print(debug.traceback())  
    print("----------------------------------------")  
end

local function main()
	collectgarbage("setpause",100)
	collectgarbage("setstepmul",5000)
	
	--ToDo:在这里绑定资源
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activityConsumeRang.plist" , 1 , 191)
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activityDoubleDrop.plist" , 1 , 191)
	
	
	--通用。
	local visibleSize = CCDirector:sharedDirector():getVisibleSize()
    local scaleRate = visibleSize.height/960
    local scaleRate2 = visibleSize.width/640
	
	
	--strBg 背景图片   str 是前景字串  strSend 点击响应发送的消息
	--ret tag = baseTag , 背景 baseTag+1 前景图 baseTag + 2  前景label baseTag + 3
	local function createLayerBtn(baseTag, strBg ,str, strSend, isSendWithObj) --baseTag=101
		local ret = CCLayer:create()
		--背景 ，图片在public1中。
		local btnBg = CCSprite:createWithSpriteFrameName(strBg) -- "an.png"
		btnBg:setAnchorPoint(ccp(0,0))
		btnBg:setPosition(0,0)
		btnBg:setTag(baseTag + 1 )
		print("bn背景图片 = "..baseTag+1)
		
		
		local btnBgSize = btnBg:getContentSize()
		--设置该layer的大小
		ret:setContentSize(btnBgSize)
		ret:addChild(btnBg)
		
		
        --前景图片字 或者 前景图片  目前没有使用

        --[[
        local spriteLabel = CCSprite:createWithSpriteFrameName("firstChargeLabel.png");
		spriteLabel:setPosition(ccp(btnBgSize.width * 0.5 , btnBgSize.height * 0.5) )
		spriteLabel:setTag(baseTag + 2 )
		print("btn图片字Tag = "..baseTag + 2 )
		btnBg:addChild(spriteLabel)
        --]]
		

		--前景文字
		local fronLabel = SGCCLabelTTF:create(str , FONT_PANGWA , 33)
		fronLabel:setPosition(ccp(btnBgSize.width * 0.5 , btnBgSize.height * 0.5) )
		fronLabel:setTag(baseTag + 3)
		print("btn字Tag = "..baseTag + 3)
		btnBg:addChild(fronLabel)
				
		--关于new_local请参考：http://blog.csdn.net/wtyqm/article/details/8977975
		--c++ tolua时构造函数自动生成new_local
		local rect = CCRect:new_local(0,0 , btnBgSize.width , btnBgSize.height)
		local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
		local beganPosY = 0
		--touch callback
		local function onTouchBegan(x, y)
			if rect:containsPoint(ret:convertToNodeSpace(ccp(x,y ) ) ) and ret:getParent():isVisible()  then
				isTouchMoved = false
				beganPosY = y
				return true
			else
				return false	
			end
		end
		
		--触控结束时发送消息 
		local function onTouchEnded(x , y)
			if (false == ret:isVisible() ) then
				return
			end

			local sender = SGNotificationCenter:sharedNotificationCenter()
			if isSendWithObj then
                sender:postNotificationWithObj(strSend,ret)  -- "sendRewardReq"  set the get reward msg 
		    else
		    	sender:postNotification(strSend) --"showStoreLayer" go to recharge layer  ,
		    end
			
			--btnBg:setColor(ccc3(255,255,255))
		end
		
		--处理触控的回调
		local function onTouch(kind , x , y)
			print(kind)--观察下kind具体的内容。
			if "began"==kind then
				return onTouchBegan(x , y)
			elseif "moved" == kind then
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			elseif "ended" == kind then
				if false == isTouchMoved then
					return onTouchEnded(x, y)
				end
			end
		end
		
		--注册回调
		ret:registerScriptTouchHandler(onTouch)
		ret:setTouchEnabled(true)
		
		ret:setTag(baseTag)
		print("按钮LayerTag = "..baseTag)
		
		return ret		
	end
	
	--************************************function createChargeLayerBtn(baseTag)  end.
	
	
	
	
	
	
	
	
	------本lua返回的layer
	local function createRetLayer(baseTag)
		
		--
		
        local retLayer = CCLayer:create()
	
		local visibleSize = CCDirector:sharedDirector():getVisibleSize()
		local orign = CCDirector:sharedDirector():getVisibleOrigin()
		local margin = 350 --上下的边距和 活动scroll view 距上下底边的和
		local spaceing  = 15 -- 大的item之间的距离
         --排名情况
        local rankbg  =  CCSprite:createWithSpriteFrameName("activityConsumeRankBg2.png")
        rankbg:setAnchorPoint(ccp(0.5,0.5))
        rankbg:setPosition(visibleSize.width * 0.5, rankbg:getContentSize().height * 0.5)

        retLayer:addChild(rankbg)

        for i = 1, 10 do
        	local rankNum = SGCCLabelTTF:create( ""..i , FONT_PANGWA , 18)
        	rankNum:setPosition(visibleSize.width* 0.5 - 70 , rankbg:getContentSize().height - 40 - (i*23))
        	rankbg:addChild(rankNum)

        	local rankName = SGCCLabelTTF:create( "游戏土豪1" , FONT_PANGWA , 18)
        	rankName:setPosition(visibleSize.width* 0.5 +60 , rankbg:getContentSize().height - 40 - (i*23))
        	rankbg:addChild(rankName)
        	rankName:setTag(200+i)

        	local rankSum = SGCCLabelTTF:create( "100000" , FONT_PANGWA , 18)
        	rankSum:setPosition(visibleSize.width* 0.5 +190 , rankbg:getContentSize().height - 40 - (i*23))
        	rankbg:addChild(rankSum)
        	rankSum:setTag(300+i)
        end

        --自己的情况
        local mysumstr = SGCCLabelTTF:create( "我的消費金額:" , FONT_PANGWA , 18)
        mysumstr:setPosition(300,47)
        rankbg:addChild(mysumstr)

        local yuanbao = CCSprite:createWithSpriteFrameName("goldicon.png")
        yuanbao:setPosition(380,47)
        rankbg:addChild(yuanbao)

        local mysum  = SGCCLabelTTF:create( "999999" , FONT_PANGWA , 18)
        mysum:setTag(401)
        mysum:setPosition(450,47)
        rankbg:addChild(mysum)

        local myrankstr = SGCCLabelTTF:create("我的排名:" , FONT_PANGWA, 18)
        myrankstr:setPosition(283,22)
        rankbg:addChild(myrankstr)

        local myrank = SGCCLabelTTF:create("10000", FONT_PANGWA, 18)
        myrank:setTag(402)
        myrank:setPosition(360,22)
        rankbg:addChild(myrank)

        --排名情况end

        --奖励说明
        local rankKuang = CCScale9Sprite:createWithSpriteFrameName("doubleDropBox.png")
        rankKuang:setPreferredSize(CCSize:new_local(rankbg:getContentSize().width , 140))
        rankKuang:setAnchorPoint(ccp(0.5,0.5))
        rankKuang:setPosition(visibleSize.width * 0.5,rankbg:getPositionY() + rankbg:getContentSize().height * 0.5 + rankKuang:getContentSize().height/2 + 5)

        retLayer:addChild(rankKuang)


        for i = 1 ,10 do
        	local label = SGCCLabelTTF:create("第1名可獲得:吕布碎片X1808" , FONT_PANGWA , 18)
        	label:setTag(100+i)
        	if i==10 then
        		i=10
        	end
        	if i <= 5 then
        	     label:setPosition(visibleSize.width* 0.5 - 170, rankKuang:getContentSize().height-5 - (i*23) )
        	else
        		label:setPosition(visibleSize.width* 0.5 + 130, rankKuang:getContentSize().height-5 - ((i-5)*23) )
        	end
        	rankKuang:addChild(label)
        end
        --奖励说明end
		--该活动的背景图片
		local spriteBg = CCSprite:createWithSpriteFrameName("activityConsumeRangBg.png")
		spriteBg:setAnchorPoint(ccp(0.5,0.5))
		retLayer:addChild(spriteBg)
		
        --居中适配
        local isCenter = false 
        local nsub = visibleSize.height - margin - (spriteBg:getContentSize().height + spaceing)
        if nsub >=  0 then
        	isCenter = true
        end
        local noffset = 0;
        if isCenter then 
        	--内容要居中 居中偏移量为 nsub的一半
           noffset = nsub * 0.5

        else
        	--内容要顶住上边 偏移为0 
           noffset = 0
        end

        spriteBg:setPosition(visibleSize.width * 0.5, rankKuang:getPositionY()+ rankKuang:getContentSize().height/2 + (spriteBg:getContentSize().height * 0.5))

		-- 添加前往按钮
		--local gotoButton = createLayerBtn(88,"an.png","前往","_goto_visit",false)
		--gotoButton:setPosition(spriteBg:getContentSize().width - 250,280)
		--retLayer:addChild(gotoButton)
        
        -- end	
        
        local downoffset = 150
        --2个label，第一个  活动时间
        local labelTime = SGCCLabelTTF:create("活動時間:" , FONT_PANGWA , 20,CCSizeMake(150,30),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter,ccc3(0,255,0),ccc3(0,0,0),2,true);
        labelTime:setPosition(spriteBg:getContentSize().width * 0.5-185 , 230-downoffset)
        spriteBg:addChild(labelTime)

		local labelActivityTime = SGCCLabelTTF:create("2014/08/22 15:16--2014/08/22 15:16" , FONT_PANGWA , 20)
		labelActivityTime:setPosition(spriteBg:getContentSize().width * 0.5 + 55 , 230 - downoffset)
		labelActivityTime:setTag(baseTag)
		print("活动时间tag = "..baseTag)
		baseTag = baseTag + 1
		spriteBg:addChild(labelActivityTime)
		--第二个label  活动描述
        local labeldes = SGCCLabelTTF:create("活動說明:" , FONT_PANGWA , 20,CCSizeMake(150,70),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter,ccc3(0,255,0),ccc3(0,0,0),2,true);
        labeldes:setPosition(spriteBg:getContentSize().width * 0.5-185 , 200 -downoffset)
        spriteBg:addChild(labeldes)
        --CCSizeMake(450,30),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter,ccc3(255,255,255),ccc3(0,0,0),2,true
		local labelActivityDes = SGCCLabelTTF:create("在活动时间内，活动探访无VIP等级要求即可参与使用！" , FONT_PANGWA , 20 ,CCSizeMake(400,70),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter,ccc3(255,255,255),ccc3(0,0,0),2,true)
		labelActivityDes:setPosition(spriteBg:getContentSize().width * 0.5 + 45 , 185  - downoffset)
		labelActivityDes:setTag(baseTag)
		print("活动描述tag = "..baseTag)
		baseTag = baseTag + 1
		spriteBg:addChild(labelActivityDes)



        

        --添加内容 end 
		

		isCenter = false
        
        if isCenter then 
        	retLayer:setContentSize( CCSize:new_local( visibleSize.width , visibleSize.height - margin + 10 ) )
        else
        	retLayer:setContentSize( CCSize:new_local( visibleSize.width , spriteBg:getPositionY() + spriteBg:getContentSize().height * 0.5 + 15) )
        end
        
        print("最顶层高**** "..retLayer:getContentSize().height)

        --红色的背景
		local spriteRedBg = CCSprite:createWithSpriteFrameName("activitybg.png")
		local scaleX = 1.0 * visibleSize.width / 245
        local scaleY = 1.0 * (retLayer:getContentSize().height ) / 233
		spriteRedBg:setScaleX(scaleX)
		spriteRedBg:setScaleY(scaleY)
		spriteRedBg:setAnchorPoint(ccp(0.5,0.5 ))
        spriteRedBg:setPosition(retLayer:getContentSize().width * 0.5 , retLayer:getContentSize().height*0.5 )


		retLayer:addChild(spriteRedBg , -100 , -100)
        retLayer:setTouchEnabled(true)
		retLayer:setTag(baseTag)
		print("最顶层layer的Tag = "..baseTag)
		baseTag = baseTag + 1
		return retLayer
	end
	
	--添加到当前执行的scene中。
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createRetLayer(12) , 1  ,654366)
end

xpcall ( main , __G__TRACKBACK__)

--******************************file end.************************************