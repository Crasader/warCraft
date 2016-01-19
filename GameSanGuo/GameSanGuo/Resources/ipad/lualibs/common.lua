--***************************
--本文件定义了lua中使用的工程中预定义的一些字体、颜色值
--引用自工程中 sglayout.h文件
--2014-08-22
--!＠！＠！＠！＠！若使用简体版本时，请将version赋值为非0.
--version=0 代表繁体版本
local version = 0 
--targetPlatform  平台类型

targetPlatform = CCApplication:sharedApplication():getTargetPlatform()
CCLuaLog(targetPlatform)
print("@@@@@@@@@@@@@@@@@@@@@@@@"..targetPlatform)
if version==0 then--繁体
	if kTargetAndroid == targetPlatform  then
		FONT_LISHU  ="font/FZPWK--GBK1-0.ttf"
	    FONT_PANGWA ="font/FZPWK--GBK1-0.ttf"    --字体方正胖娃GBK
	    FONT_XINGKAI="font/FZPWK--GBK1-0.ttf"
	    FONT_BOXINFO="font/FZLSK--GBK1-0.ttf"     --字体方正隶书GBK
	    FONT_FZXK   ="font/FZLSK--GBK1-0.ttf"
	    FONT_PENGYOU="font/FZHTK--GBK1-0.ttf"     --字体方正黑体GBK
	    
	else
		FONT_LISHU  ="FZPWK--GBK1-0.ttf"
	    FONT_PANGWA ="FZPWK--GBK1-0.ttf"    --字体方正胖娃GBK
	    FONT_XINGKAI="FZPWK--GBK1-0.ttf"
	    FONT_BOXINFO="FZLSK--GBK1-0.ttf"     --字体方正隶书GBK
	    FONT_FZXK   ="FZLSK--GBK1-0.ttf"
	    FONT_PENGYOU="FZHTK--GBK1-0.ttf"     --字体方正黑体GBK
	end
else --简体
	if kTargetAndroid == targetPlatform  then
	    FONT_LISHU  ="font/FZPWJW--GB1-0.ttf"
	    FONT_PANGWA ="font/FZPWJW--GB1-0.ttf"      --字体胖娃 简体
	    FONT_XINGKAI="font/FZPWJW--GB1-0.ttf"
	    FONT_BOXINFO="font/FZLSJW--GB1-0.ttf"      --字体隶书 简体
	    FONT_FZXK   ="font/FZLSJW--GB1-0.ttf"
	    FONT_PENGYOU="font/FZHTJW--GB1-0.ttf"      --黑体     简体
	else
		
	    FONT_LISHU  ="FZPWJW--GB1-0.ttf"
	    FONT_PANGWA ="FZPWJW--GB1-0.ttf"      --字体胖娃 简体
	    FONT_XINGKAI="FZPWJW--GB1-0.ttf"
	    FONT_BOXINFO="FZLSJW--GB1-0.ttf"      --字体隶书 简体
	    FONT_FZXK   ="FZLSJW--GB1-0.ttf"
	    FONT_PENGYOU="FZHTJW--GB1-0.ttf"      --黑体     简体
	end
end

COLOR_GRAY    =ccc3(0xc4,0xc4,0xc4)
COLOR_ORANGE  =ccc3(0xff,0x72,0x00)
COLOR_YELLOW  =ccc3(0xff, 0xf0, 0x00)
COLOR_GREEN   =ccc3(0x0c, 0xff, 0x00)
COLOR_BLUE    =ccc3(0x26, 0xd9, 0xff)
COLOR_RED     =ccc3(0xf9, 0x00, 0x00)
COLOR_TAN     =ccc3(143,87,0)  --棕黄色  常用作标题处。
COLOR_UNKNOW_TAN=ccc3(0xff , 0x95 , 0x0c)  --未知的棕黄色  。。。


--*****************************file end.