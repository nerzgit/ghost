//
//  charactermanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef charactermanager_hpp
#define charactermanager_hpp

#include "dta/player.h"
#include <vector>
#include <glm/glm.hpp>

class Clock;
class RayOBB;
class Delegator;
class O_Character;
class CharacterBuilder;
class CharacterDecorator;
extern Clock* g_CLOCK;

class CharacterManager
{
private:
    
    bool _play;
    bool _play1;
    int _zombiesize;
    
    std::vector<Delegator*> _delegate_arr;
    std::vector<RayOBB*> _rayobb_arr;
    std::vector<O_Character*> _o_zombie_arr;           // Character : Zombie
    std::vector<CharacterBuilder*> _zombiebuilder_arr; // Builder   : Zombie
    std::vector<CharacterDecorator*> _enter_arr;       // Decorator : Enter the room
    std::vector<CharacterDecorator*> _chase_arr;       // Decorator : Chasing you
    std::vector<CharacterDecorator*> _dead_arr;        // Decorator : Dead zombie
    std::vector<CharacterDecorator*> _blood_arr;       // Decorator : Blood
    std::vector<int> _zombieaudiobuffer_arr;
    
    glm::vec3 _targetposition;
    glm::vec3 _targetorientation;
    bool _targetfire;
    int _targetlife;
    int _currentround;
    int _lastround;
    
    void _InitializeCharacter(const std::string &map);
    void _InitializeDecorator(const std::string &map);
    void _InitializeRayOBB();
    void _InitializeZombieVoices();
    void _ProcessRayOBB();
    void _ProcessZombieStartInOrder();
    void _ProcessZombieReset();
    void _ProcessRunZombieVoice(O_Character* character);
    void _ProcessDeadZombieVoice(O_Character* character);
    
public:
    CharacterManager();
    ~CharacterManager();
    void Init(const std::string &map);
    void Render();
    void Reset();
    int UpdatePlay(int value);
    int UpdatePlay1(int value);
    void UpdateDelegator(Delegator* delegator);
};

#endif /* charactermanager_hpp */
