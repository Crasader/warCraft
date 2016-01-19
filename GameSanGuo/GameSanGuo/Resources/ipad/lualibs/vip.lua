--[[
*ÐÂÔö»î¶¯lua vip
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
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activity_vipbg.plist" , 1 , 191)
	
	local function createBtn(msg ,textPng)
		local ret = CCLayer:create()
		local bg = CCSprite:createWithSpriteFrameName("an.png")
		bg:setAnchorPoint(ccp(0,0))
		bg:setPosition(0,0)			
		ret:addChild(bg)
			
		--´óÐ¡
		local size = bg:getContentSize()
		ret:setContentSize(size)
		
		--Ç°Íù´¢ÖµµÄÍ¼Æ¬×Ö
		local spriteLabel = CCSprite:createWithSpriteFrameName(textPng);
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
				sender:postNotification(msg)
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
		
		local spriteBg = CCSprite:createWithSpriteFrameName("activity_vipbg.png")
		local size = CCSize:new_local(spriteBg:getContentSize().width , spriteBg:getContentSize().height+45)
        retLayer:setContentSize(CCSize:new_local(CCDirector:sharedDirector():getWinSize().width , size.height))
        spriteBg:setPosition(retLayer:getContentSize().width*0.5 , retLayer:getContentSize().height*0.5 + 45)
        retLayer:addChild(spriteBg , -10)

		--储值
		local btn_1 = createBtn("showStoreLayer" , "activity_vip_charge.png")
        btn_1:ignoreAnchorPointForPosition(false)
		btn_1:setAnchorPoint(ccp(1,1))
		btn_1:setPosition(size.width*0.5-15 , 0)
		spriteBg:addChild(btn_1)
		
		--购买
		local btn_2 = createBtn("showVipInfo" , "activity_vip_buy.png")
        btn_2:ignoreAnchorPointForPosition(false)
        btn_2:setAnchorPoint(ccp(0,1))
		btn_2:setPosition(size.width*0.5+15 , 0)
		spriteBg:addChild(btn_2)
		

        return retLayer
	
	end
	
	--Ìí¼Óµ½µ±Ç°Ö´ÐÐµÄsceneÖÐ¡£
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createReturnLayer() , 1  ,654347)
end

xpcall ( main , __G__TRACKBACK__)




----file end.