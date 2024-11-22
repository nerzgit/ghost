//
//  audio.hpp
//  Gme550
//
//  Created by PROPPACK DEVELOPER IDE on 2019/03/14.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef audio_hpp
#define audio_hpp

#pragma once

#include "glm/glm.hpp"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <vector>
#include <string>

class Audio
{
private:
    static const int MAX_SOURCES;
    
    static Audio *instance;            // singleton instance
    
    ALCdevice *alDevice;                   // our audio device
    ALCcontext *alContext;                 // our OpenAL context
    
    ALuint *sources;                        // sources playing or available to be played
    int nextFreeSourceIndex;                // best guess as to where the next available source is, in "sources"
    
    double volume;                            // volume of entire sound system
    
    Audio();                            // force use of getInstance()
    
    ALuint getUnusedSource();                // returns an ALuint from "sources" that is currently unused
    
    std::vector<std::string> soundBufferTitles;    // ALuint buffer names list
    
    std::vector<ALuint> soundBufferArray;   // sound buffer list
    
public:
    static Audio *getInstance();            // singleton design pattern
    
    ~Audio();                            // deallocates OpenAL resources
    
    ALuint getSoundBuffer(std::string title);
    
    ALuint loadWAV(std::string file, std::string title);    // load a .WAV file and return an OpenAL buffer associated with it
    
    void setVolume(double volume);                // sets or gets volume of entire system
    double getVolume();
    
    // play sound with default settings
    ALuint playSound(ALuint);
    
    // play sound with specified 3D settings
    ALuint playSound(ALuint, glm::vec3 pos, double refDist, double maxDist);
    
    // loop sound with default settings
    ALuint loopSound(ALuint);
    
    // loop sound with specified 3D settings
    ALuint loopSound(ALuint, glm::vec3 pos, double refDist, double maxDist);
    
    // is the given source currently playing or paused?
    bool isSoundPlaying(ALuint);
    
    // sound control---pass in an OpenAL source, get the desired effect
    void stop(ALuint);
    void pause(ALuint);
    void resume(ALuint);
    
    // global sound controls
    void pauseAll();
    void resumeAll();
    void stopAllPaused();
    void stopAll();
    
    // 3D listener control
    void setListenerPos(glm::vec3);
    void setListenerVelocity(glm::vec3);
    void setListenerOrientation(glm::vec3, glm::vec3);
};



#endif /* audio_hpp */
