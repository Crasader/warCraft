//
//  CCPlaySound.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-6-6.
//
//

#include "CCPlaySound.h"
static CCPlaySound *playMusic = NULL;

CCPlaySound::CCPlaySound()
:m_MusicOn(true)
,m_EffectOn(true)
{
}

CCPlaySound::~CCPlaySound()
{
    //卸载
    SimpleAudioEngine::sharedEngine()->end();
}

CCPlaySound* CCPlaySound::sharePlaySound()
{
    if(NULL == playMusic)
    {
        playMusic = new CCPlaySound();
        playMusic->setVolume(0.5f);
    }
    
    return playMusic;
}

#pragma mark - ------ 播放音乐 -------

void CCPlaySound::preloadMusicByType(const char *musicName)
{
    //    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(musicName));
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(musicName).c_str());
}

void CCPlaySound::playerMusicByType(const char* musicName)
{
    if (!m_MusicOn) {
        return;
    }
    //modified by cgp
    //SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(musicName), true);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(musicName).c_str(), true);
}

void CCPlaySound::setVolume(float volume)
{
    m_nMusicVolume = volume;
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_nMusicVolume);
}

void CCPlaySound::stopMusic()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void CCPlaySound::rewindMusic()
{
    SimpleAudioEngine::sharedEngine()->rewindBackgroundMusic();
}

void CCPlaySound::pauseMusic()
{
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void CCPlaySound::resumeMusic()
{
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void CCPlaySound::setMusicOn(bool on)
{
    m_MusicOn = on;
    if (!on) {
        stopMusic();
    }
}

#pragma mark - ------ 播放音效 ------

void CCPlaySound::preloadeffectByType(const char *effectName)
{
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(effectName).c_str());
}

void CCPlaySound::playEffectByType(const char *effectName)
{
    if (!m_EffectOn) {
        return;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(effectName).c_str());
}

void CCPlaySound::playLoopEffectByType(const char *effectName)
{
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(effectName).c_str(), true);
}

void CCPlaySound::stopEffect()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void CCPlaySound::setEffectVolume(float volume)
{
    m_nEffectVolume = volume;
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_nEffectVolume);
}

void CCPlaySound::stopEffect(unsigned int effectId)
{
    SimpleAudioEngine::sharedEngine()->stopEffect(effectId);
}

void CCPlaySound::rewindEffect(unsigned int effectId)
{
    SimpleAudioEngine::sharedEngine()->resumeEffect(effectId);
}

void CCPlaySound::setEffectOn(bool on)
{
    m_EffectOn = on;
}



void CCPlaySound::playNulSound()
{
    
}