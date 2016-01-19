--[[
*ÐÂÔö»î¶¯lua 超级福利
*by:zyc
*time:2014-11-11
*weather:sunny
--]]

--ÒýÈëÁË¹¤³ÌÖÐÔ¤¶¨ÒåµÄ×ÖÌå¡¢ÑÕÉ«ÖµµÄlua°æ±¾¡£
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
	
	--ÔÚÕâÀï°ó¶¨×ÊÔ´
    print (""..kTargetAndroid)
	if kTargetAndroid == targetPlatform  then
		ResourceManager:sharedInstance():bindTexture("sanguobigpic/activity_super_benefitsbg.plist" , 1 , 191)
	else
		ResourceManager:sharedInstance():bindTexture("sanguobigpic/activity_super_benefitsbg_new.plist" , 1 , 191)
	end
	
	
	local function createBtn()
		local ret = CCLayer:create()
		local bg = CCSprite:createWithSpriteFrameName("store_exchangebtnbg.png")
		bg:setAnchorPoint(ccp(0,0))
		bg:setPosition(0,0)			
		ret:addChild(bg)
			
		--´óÐ¡
		local size = bg:getContentSize()
		ret:setContentSize(size)
		
		--Ç°Íù´¢ÖµµÄÍ¼Æ¬×Ö
		local spriteLabel = CCSprite:createWithSpriteFrameName("buy.png");
		spriteLabel:setPosition(ccp(size.width * 0.5 , size.height * 0.5) )
		bg:addChild(spriteLabel)
		
		--´¥¿ØÇøÓò
		local rect = CCRect:new_local(0,0 ,size.width , size.height)
		local isTouchMoved = false --Èç¹û±¾´Î°üº¬»¬¶¯ÊÂ¼þ£¬Ôò²»´¦ÀítouchendÏûÏ¢
		local beganPosY = 0
			
		--touch callback			
		local function onTouchBegan(x, y)
			if rect:containsPoint(ret:convertToNodeSpace(ccp(x,y)))  and ret:getParent():isVisible() then
				isTouchMoved = false
				beganPosY = y
				return true
			else
				return false
			end
		end

		local function onTouchEnded(x, y)
            if( false == ret:isVisible()) then
				return
            end
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotification("showStoreLayer")
			end

		local function onTouch(eventType, x, y)
			print(eventType)
			if eventType == "began" then   
				return onTouchBegan(x, y)
			elseif eventType == "moved" then
			local deltaPoxY  = math.abs(y-beganPosY)
			print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
			if deltaPoxY >= 20 then 
		       isTouchMoved = true
		    end
			elseif eventType == "ended" then
				if isTouchMoved == false then
					return onTouchEnded(x, y)
				end
			end
		end
			
		--×¢²á´¥¿Ø»Øµ÷
		ret:registerScriptTouchHandler(onTouch)
		ret:setTouchEnabled(true)
		
		return ret
	end
	
	local function createReturnLayer()
		local retLayer = CCLayer:create()
        local spriteBg = nil
		if kTargetAndroid == targetPlatform  then
		     spriteBg = CCSprite:createWithSpriteFrameName("activity_super_benefitsbg.png")
	    else
		     spriteBg = CCSprite:createWithSpriteFrameName("activity_super_benefitsbg_new.png")
	    end
		

        retLayer:setContentSize(CCSize:new_local(CCDirector:sharedDirector():getWinSize().width , spriteBg:getContentSize().height))
        spriteBg:setPosition(retLayer:getContentSize().width*0.5 , retLayer:getContentSize().height*0.5)
        retLayer:addChild(spriteBg , -10)

        local posX = 450
        local posY = 110
        local increaseY = 185

		--°´Å¥1
		local btn_1 = createBtn()
		btn_1:setPosition(posX ,posY)
		spriteBg:addChild(btn_1)
		
		--°´Å¥2
		local btn_2 = createBtn()
		btn_2:setPosition(posX , posY+increaseY)
		spriteBg:addChild(btn_2)
		
		--°´Å¥3
		local btn_3 = createBtn()
		btn_3:setPosition(posX , posY + 2*increaseY)
		spriteBg:addChild(btn_3)

        return retLayer
	
	end
	
	--Ìí¼Óµ½µ±Ç°Ö´ÐÐµÄsceneÖÐ¡£
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createReturnLayer() , 1  ,654346)
end

xpcall ( main , __G__TRACKBACK__)




----file end.