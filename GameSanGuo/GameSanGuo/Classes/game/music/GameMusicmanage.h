//
//  GameMusicmanage.h
//  GameSanGuo
//
//  Created by wenlong w on 13-6-6.
//
//

#ifndef GameSanGuo_GameMusicmanage_h
#define GameSanGuo_GameMusicmanage_h

#include "../../libs/CocosDenshion/playSound/CCPlaySound.h"

#define PLAY_SWITCH 0

#define MUSIC_PVP     "music/pvp.mp3"               //pvp音乐
#define MUSIC_NORMAL  "music/normal_battle.mp3"     //正常战斗音乐
#define MUSIC_BOSS    "music/boss.mp3"              //boss战音乐
#define MUSIC_SCENE   "music/scene.mp3"             //场景音乐 换为 LNZSG-主界面 01-06 片段.mp3
#define MUSIC_BOSSBATTLE      "music/boss_bm.mp3" //boss 战背景音乐
#define MUSIC_ITEM     "music/itemeffect.mp3" //按钮起效
#define MUSIC_BTN     "music/btneffect.mp3" //按钮起效
#define MUSIC_1       "music/1.mp3"     //切换回合 *
#define MUSIC_2       "music/2.mp3"     //016C_0018（组成武将阵列）*
#define MUSIC_3       "music/3.mp3"     //0166_0007（点击未起效按钮）
#define MUSIC_4       "music/4.mp3"     //016B_0002（杀死散兵）*
#define MUSIC_5       "music/5.mp3"     //0167_0004(组成小兵阵列）*
#define MUSIC_6       "music/6.mp3"     //0167_005E放下兵 *
#define MUSIC_7       "music/7.mp3"     //0167_005拿起兵 *
#define MUSIC_8       "music/8.mp3"     //0167_0006（删除墙）*
#define MUSIC_9       "music/9.mp3"     //0167_0008（小兵拉弓）*
#define MUSIC_10       "music/10.mp3"   //0167_0009（小兵射箭）
#define MUSIC_11       "music/11.mp3"   //0167_0015（墙融合）*
#define MUSIC_12       "music/12.mp3"   //0167_0037（魏国组墙）*
#define MUSIC_13       "music/13.mp3"   //0167_0044（拆墙）*
#define MUSIC_14       "music/14.mp3"   //0167_0049（蜀国组墙）*
#define MUSIC_15       "music/15.mp3"   //0167_0052（吴国组墙）*
#define MUSIC_16       "music/16.mp3"   //0167_0056（普通攻击）近身攻击打到攻击阵列   只是箭消失和穿阵列   打到底线 
#define MUSIC_17       "music/17.mp3"   //0165_004E（删除小兵）*
#define MUSIC_18       "music/18.mp3"   //016C_0005（召唤友军）*
#define MUSIC_19       "music/19.mp3"   //0166_0004（降低武将回合）*
#define MUSIC_20       "music/20.mp3"   //0167_001D（弩车攻击）*
#define MUSIC_21       "music/21.mp3"   //补兵脚步1 *
//#define MUSIC_22       "music/22.mp3"   //我方敌方回合框弹出时 *
#define MUSIC_23       "music/23.mp3"   //武将流光 *
#define MUSIC_24       "music/24.mp3"   //战斗开始 *
#define MUSIC_25       "music/25.mp3"   //0167_001C（火球飞出）*
//#define MUSIC_26       "music/26.mp3"   //0167_0039（爆炸）*
#define MUSIC_27       "music/27.mp3"   //放置
#define MUSIC_28       "music/28.mp3"   //切换界面
//#define MUSIC_29       "music/29.mp3"   //。。。
//#define MUSIC_30       "music/30.mp3"   //。。。
//#define MUSIC_31       "music/31.mp3"   //关卡音效
#define MUSIC_32       "music/32.mp3"   //进入战斗
#define MUSIC_33       "music/33.mp3"   //。。。
#define MUSIC_34       "music/34.mp3"   //。。。
#define MUSIC_35       "music/35.mp3"   //。。。
//#define MUSIC_36       "music/36.mp3"   //。。。
#define MUSIC_37       "music/37.mp3"   //。。。
//#define MUSIC_38       "music/38.mp3"   //祭天
#define MUSIC_39       "music/39.mp3"   //武将前冲+飞行道具发出 补充
#define MUSIC_40       "music/40.mp3"   //武将攻击  补充
#define MUSIC_41       "music/41.mp3"   //城墙掉血  补充
#define MUSIC_42       "music/42.mp3"   //士兵前攻击前
#define MUSIC_43       "music/43.mp3"   //军魂开宝箱
//#define MUSIC_44       "music/44.mp3"   //
//#define MUSIC_45       "music/45.mp3"   //
//#define MUSIC_46       "music/46.mp3"   //
//#define MUSIC_47       "music/47.mp3"   //
//#define MUSIC_48       "music/48.mp3"   //
//#define MUSIC_49       "music/49.mp3"   //
//#define MUSIC_50       "music/50.mp3"   //

#define MUSIC_51       "music/51.mp3"   //门出现
#define MUSIC_52       "music/52.mp3"   //开门瞬间
#define MUSIC_53       "music/53.mp3"   //抽单张
#define MUSIC_54       "music/54.mp3"   //10连抽
#define MUSIC_55      "music/55.mp3"   //抽奖结束
#define MUSIC_56      "music/56.mp3"   //选中卡
#define MUSIC_57      "music/57.mp3"   //选中卡

#define MUSIC_58       "music/58.mp3" //boss战士兵合体
#define MUSIC_59       "music/59.mp3" //boss战我方攻击
#define MUSIC_60       "music/60.mp3" //boss战我方命中
#define MUSIC_61       "music/61.mp3"  //boss战敌方攻击
#define MUSIC_62       "music/62.mp3"  //boss战敌方命中

#define MUSIC_63        "music/63.wav" //胜利图片出现的音效
#define MUSIC_64        "music/64.wav" //文字出现
#define MUSIC_65        "music/65.wav" //进度条涨
#define MUSIC_66        "music/66.wav" //宝箱打开
#define MUSIC_67        "music/67.wav" //失败音效

#define MUSIC_68        "music/68.wav" //銅雀台 夺宝。旋转
#define MUSIC_69        "music/69.wav" //宝箱点亮




#define MUSIC_PLAY(__xxx__)     CCPlaySound::sharePlaySound()->playerMusicByType(__xxx__)
#define MUSIC_STOP              CCPlaySound::sharePlaySound()->stopMusic()
#define SET_MUSIC_ON(__xxx__)   CCPlaySound::sharePlaySound()->setMusicOn(__xxx__)
#define PAUSE_MUSIC             CCPlaySound::sharePlaySound()->pauseMusic()
#define RESUME_MUSIC            CCPlaySound::sharePlaySound()->resumeMusic()
#define MUSIC_VOLUME(__xxxx__)  CCPlaySound::sharePlaySound()->setVolume(__xxxx__)

#define EFFECT_PLAY(__yyyy__)   CCPlaySound::sharePlaySound()->playEffectByType( __yyyy__)
#define EFFECT_LOOP_PLAY(xxx)   CCPlaySound::sharePlaySound()->playLoopEffectByType( __yyyy__)
#define SET_EFFECT_ON(__xxx__)  CCPlaySound::sharePlaySound()->setEffectOn(__xxx__)
#define EFFCT_VOLUME(__xxxx__)  CCPlaySound::sharePlaySound()->setEffectVolume(__xxxx__)




#endif
