//
//  eventmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef eventmanager_hpp
#define eventmanager_hpp

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct AABB;
class Delegator;
class Clock;
extern Clock* g_CLOCK;

class EventManager
{
private:
    std::vector<Delegator*> _delegator_arr;
    
    glm::vec3 _playerposition;
    
    // Perkcola
    std::vector<AABB*> _perkcola_aabb_arr;
    bool _perk[3];
    bool _perkcola;
    int  _perkcolainterval;
    void _InitializePerkCola();
    void _ProcessPerkCola();
    
    // MisteryBox
    std::vector<AABB*> _misterybox_aabb_arr;
    bool _weaponbox;
    int  _weaponbox_interval;
    void _InitializeMisteryBox();
    void _ProcessMisteryBox();
    
    // Running sound
    std::vector<AABB*> _runningsound_aabb_arr;
    bool _runsoundonconcreate;
    bool _walksoundonconcreate;
    bool _runsoundonwood;
    bool _walksoundonwood;
    bool _runningsound;
    int  _runningsound_interval;
    void _InitializeRunningSound();
    void _ProcessRunningSound();
    void _ProcessRunningSoundAllStop();
    void _ProcessRunningBooleanAllFalse();
    
    // Round
    std::vector<AABB*> _escape_aabb_arr;
    int _round;
    int _killcount;
    int _goalkillcount;
    void _InitializeRound();
    void _ProcessRound();
    
    // Chapter (_ProcessRound();)
    int _lastround;
    bool _chapter;
    
public:
    EventManager();
    ~EventManager();
    void Init();
    void Render();
    void Reset();
    int RecievePerk(int value);
    int RecieveRound(int value);
    int RecieveKillCount(int value);
    int UpdateKillCount(int value);
    void UpdateDelegator(Delegator* delegator);
};
#endif /* eventmanager_hpp */
