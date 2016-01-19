//
//  CCPlaySound.h
//  GameSanGuo
//
//  Created by wenlong w on 13-6-6.
//
//

#ifndef __GameSanGuo__CCPlaySound__
#define __GameSanGuo__CCPlaySound__

#include <iostream>
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace CocosDenshion;

class CCPlaySound
{
private:
    float m_nMusicVolume;
    float m_nEffectVolume;

public:
    bool m_MusicOn;
    bool m_EffectOn;
    
public:
    CCPlaySound();
    ~CCPlaySound();
    static CCPlaySound *sharePlaySound();
    
    //预加载背影音乐
    void preloadMusicByType(const char* musicName);
    //预加载音效
    void preloadeffectByType(const char* effectName);
    //播放音乐
    void playerMusicByType(const char* musicName);
    //停止音乐
    void stopMusic();
    //重新播放背景音乐
    void rewindMusic();
    //改变音乐音量
    void setVolume(float volume);
    //暂停背景音乐
    void pauseMusic();
    //恢复播放
    void resumeMusic();
    //播放音效
    void playEffectByType(const char *effectName);
    //播放循环音效
    void playLoopEffectByType(const char *effectName);
    //停止音效
    void stopEffect();
    //改变音效音量
    void setEffectVolume(float volume);
    //停止某个音效
    void stopEffect(unsigned int effectId);
    //重新播放音效
    void rewindEffect(unsigned int effectId);
    
    void playNulSound();
    
    void setMusicOn(bool on);
    void setEffectOn(bool on);
    
};

#endif /* defined(__GameSanGuo__CCPlaySound__) */
