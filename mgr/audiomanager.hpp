//
//  audiomanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/11.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef audiomanager_hpp
#define audiomanager_hpp

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Audio;
class Delegator;

class AudioManager
{
private:
    std::vector<Delegator*> _delegate_arr;
    Audio* _audio;
    std::vector<std::string> _audioname_arr;
    std::vector<int> _audiodataint_arr;
    std::vector<int> _audioplayint_arr;
    std::vector<int> _audioloopint_arr;
    std::vector<int> _multiaudioplayint_arr;
    
    glm::vec3 _targetposition;
    glm::vec3 _position;
    int _multiaudioindex;
    
public:
    AudioManager();
    ~AudioManager();
    void Reset();
    
    // Normal
    std::string PlaySound(std::string name);
    std::string PlaySoundOnce(std::string name);
    std::string PlaySoundOnceStop(std::string name);
    std::string PlaySoundOnceReset(std::string name);
    std::string LoopSound(std::string name);
    std::string LoopSoundOnce(std::string name);
    std::string LoopSoundOnceStop(std::string name);
    std::string LoopSoundOnceReset(std::string name);
    int CreateMultiPlaySoundBufferIndex(int index);
    std::string MultiPlaySoundOnce(std::string name);
    int MultiPlaySoundOnceStop(int index);
    int MultiPlaySoundOnceReset(int index);
    int UpdateMultiAudioindex(int index);
    glm::vec3 UpdateTargetPosition(glm::vec3 position);
    glm::vec3 UpdatePosition(glm::vec3 position);
    
    // Common
    void PauseSound();
    void ResumeSound();
    int SearchAudio(std::string name);
    void UpdateDelegator(Delegator* delegator);
};

#endif /* audiomanager_hpp */
