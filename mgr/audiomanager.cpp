//
//  audiomanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/11.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "audiomanager.hpp"
#include "del/delegate.hpp"
#include "usge/audio.hpp"

AudioManager::AudioManager()
{
    
    // Get instance only once
    _audio = Audio::getInstance();
    
    // Read audio.aa file
    FILE * file = fopen(std::string("game/game/as/audio/audio.aa").c_str(), "r");
    if( file == NULL ){
        printf(".aa file not found\n");
        return;
    }
    while( 1 ){
        
        char head[128];
        
        // End of file
        int r = fscanf(file, "%s", head);
        if (r == EOF)
            break;
        
        // Initialize
        if ( strcmp( head, "a" ) == 0 ){
            char a[256];
            char b[256];
            fscanf(file, "%s %s\n", a, b);
            int buffer = (int)_audio -> loadWAV(std::string(a), std::string(b));
            _audioname_arr.push_back(std::string(b));
            _audiodataint_arr.push_back(buffer);
            _audioplayint_arr.push_back(-1);
            _audioloopint_arr.push_back(-1);
        }
        
    }
    fclose(file);
    
}

AudioManager::~AudioManager()
{
    for (auto e : _delegate_arr) delete e;
    delete _audio;
}

void AudioManager::Reset()
{
    for (auto e : _audioplayint_arr     ) e=-1;
    for (auto e : _audioloopint_arr     ) e=-1;
    for (auto e : _multiaudioplayint_arr) e=-1;
}

std::string AudioManager::PlaySound(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audiodataint_arr[u];
    
    // Play audio
    _audio -> playSound(buffer);
    
    // Return audio name
    return name;
    
}

std::string AudioManager::PlaySoundOnce(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audiodataint_arr[u];
    
    // Play audio and add new sound buffer
    if (_audioplayint_arr[u] == -1){
        _audioplayint_arr[u] = _audio -> playSound(buffer);
    }
    
    // Return audio name
    return name;
    
}

std::string AudioManager::PlaySoundOnceStop(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audioplayint_arr[u];
    
    // Stop audio
    _audio -> stop(buffer);
    
    // Return audio name
    return name;
    
}

std::string AudioManager::PlaySoundOnceReset(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    _audioplayint_arr[u] = -1;
    
    // Return audio name
    return name;
    
}

std::string AudioManager::LoopSound(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audiodataint_arr[u];
    
    // Loop audio
    _audio -> loopSound(buffer);
    
    // Return audio name
    return name;
    
}

std::string AudioManager::LoopSoundOnce(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audiodataint_arr[u];
    
    // Play audio and add new sound buffer
    if (_audioloopint_arr[u] == -1){
        _audioloopint_arr[u] = _audio -> loopSound(buffer);
    }
    
    // Return audio name
    return name;
    
}

std::string AudioManager::LoopSoundOnceStop(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audioloopint_arr[u];
    
    // Stop audio
    _audio -> stop(buffer);
    
    // Return audio name
    return name;
    
}


std::string AudioManager::LoopSoundOnceReset(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    _audioloopint_arr[u] = -1;
    
    // Return audio name
    return name;
    
}

int AudioManager::CreateMultiPlaySoundBufferIndex(int value){
    
    // Create -1 sound buffer to any index
    _multiaudioplayint_arr.push_back(-1);
    
    // Return index
    return (int)_multiaudioplayint_arr.size()-1;
    
}

std::string AudioManager::MultiPlaySoundOnce(std::string name){
    
    // Search audio order
    int u = SearchAudio(name);
    
    // Get audio buffer
    ALuint buffer = (ALuint)_audiodataint_arr[u];
    
    // Play audio and add new sound buffer
    if (_multiaudioplayint_arr[_multiaudioindex] == -1) {
        _multiaudioplayint_arr[_multiaudioindex] = _audio -> playSound(buffer);
    }
    
    // Sound buffer coordinates
    glm::vec3 offset = _targetposition - _position;
    float distanceToPlayer = length(offset);
    glm::vec3 velocity = (offset / distanceToPlayer) * (float)3.0 * (float)1;
    alSource3f(_multiaudioplayint_arr[_multiaudioindex], AL_POSITION, _position.x, _position.y, _position.z);
    alSource3f(_multiaudioplayint_arr[_multiaudioindex], AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    
    // Return audio name
    return name;
    
}

int AudioManager::MultiPlaySoundOnceStop(int index){

    // Get audio buffer
    ALuint buffer = (ALuint)_multiaudioplayint_arr[index];
    
    // Stop audio
    _audio -> stop(buffer);
    
    // Return index
    return index;
    
}

int AudioManager::MultiPlaySoundOnceReset(int index){

    // Get audio buffer
    _multiaudioplayint_arr[index] = -1;
    
    // Return index
    return index;
    
}

int AudioManager::UpdateMultiAudioindex(int index){
    return _multiaudioindex = index;
}

glm::vec3 AudioManager::UpdateTargetPosition(glm::vec3 position){
    return _targetposition = position;
}

glm::vec3 AudioManager::UpdatePosition(glm::vec3 position){
    return _position = position;
}


void AudioManager::PauseSound(){
    
    // Pause all audio (Play & Loop)
    _audio -> pauseAll();

}

void AudioManager::ResumeSound(){
    
    // Resume all audio (Play & Loop)
    _audio -> resumeAll();
    
}

int AudioManager::SearchAudio(std::string name){
    for (int i=0; i<_audioname_arr.size(); i++) {
        if (_audioname_arr[i] == name) {
            return i;
        }
    }
    return -1;
}

void AudioManager::UpdateDelegator(Delegator* delegator)
{
    /**
     * 
     */
    _delegate_arr.push_back(delegator);
}
