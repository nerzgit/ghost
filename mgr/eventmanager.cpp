//
//  eventmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "eventmanager.hpp"
#include "del/delegate.hpp"
#include "usge/key.hpp"
#include "usge/clock.hpp"
#include "util/aabbCollision.hpp"

EventManager::EventManager()
{
    
    _perkcola = false;
    _perkcolainterval = 0;
    _weaponbox = false;
    _weaponbox_interval = 0;
    
    // Player position
    _playerposition = glm::vec3(0);
    
    _round = 0;
    _killcount = 0;
    _goalkillcount = 0;
    _lastround = 0;
    _chapter = false;
    
}

EventManager::~EventManager()
{
    for (auto e : _delegator_arr         ) delete e;
    for (auto e : _perkcola_aabb_arr     ) delete e;
    for (auto e : _misterybox_aabb_arr   ) delete e;
    for (auto e : _runningsound_aabb_arr ) delete e;
}

/* After added Delegator */
void EventManager::Init()
{
    _InitializePerkCola();
    _InitializeMisteryBox();
    _InitializeRunningSound();
    _InitializeRound();
}

/* Reinitialize */
void EventManager::Reset()
{
    
    _perkcola = false;
    _perkcolainterval = 0;
    _weaponbox = false;
    _weaponbox_interval = 0;
    _playerposition = glm::vec3(0);
    _round = 0;
    _killcount = 0;
    _goalkillcount = 0;
    _lastround = 0;
    _chapter = false;
    
    // Default having perk
    _perk[0] = false;
    _perk[1] = false;
    _perk[2] = false;
    
}

/* In rendering */
void EventManager::Render()
{
    
    // Syncronize player current position
    _playerposition = (*_delegator_arr[12])(glm::vec3(0));
    
    // Perk cola event (AABB Collision)
    _ProcessPerkCola();
    
    // Mistery box event (AABB Collision)
    _ProcessMisteryBox();
    
    // Running sound change event (AABB Collision)
    _ProcessRunningSound();
    
    // Round
    _ProcessRound();
    
}






void EventManager::_InitializePerkCola()
{
    
    // Default having perk
    _perk[0] = false;
    _perk[1] = false;
    _perk[2] = false;
    
    // Create AABB collision group and get index
    int createdgroup = (*_delegator_arr[0])(0);
    
    // AABB player
    AABB * player_aabb = new AABB(glm::vec3(0),
                                  glm::vec3(0),
                                  createdgroup);
    
    // AABB rootbeer
    AABB * rootbeer_aabb = new AABB(glm::vec3(12.4944, 0.008106, 2.32811),
                                    glm::vec3(13.2426, 1.55426, 1.88008),
                                    createdgroup);
    
    // AABB coke
    AABB * coke_aabb = new AABB(glm::vec3(11.5233, 0.008106, 2.32811),
                                glm::vec3(12.2715, 1.55426, 1.88008),
                                createdgroup);

    // AABB soda
    AABB * soda_aabb = new AABB(glm::vec3(10.5886, 0.008106, 2.19099),
                                glm::vec3(11.3368, 1.55426, 1.74296),
                                createdgroup);
    
    // Submit AABB
    (*_delegator_arr[1])(player_aabb);
    (*_delegator_arr[1])(rootbeer_aabb);
    (*_delegator_arr[1])(coke_aabb);
    (*_delegator_arr[1])(soda_aabb);
    
    // Add
    _perkcola_aabb_arr.push_back(player_aabb);
    _perkcola_aabb_arr.push_back(rootbeer_aabb);
    _perkcola_aabb_arr.push_back(coke_aabb);
    _perkcola_aabb_arr.push_back(soda_aabb);
    
}

void EventManager::_ProcessPerkCola()
{
    
    // Update player position
    float player_aabb_radius = 0.2;
    _perkcola_aabb_arr[0]->Update(glm::vec3(_playerposition.x-player_aabb_radius,
                                            _playerposition.y,
                                            _playerposition.z+player_aabb_radius),
                                  glm::vec3(_playerposition.x+player_aabb_radius,
                                            _playerposition.y+player_aabb_radius,
                                            _playerposition.z-player_aabb_radius));
    
    if (_perkcola) {
        _perkcolainterval ++;
        if (_perkcolainterval > 100) {
            _perkcolainterval = 0;
            _perkcola = false;
            // Reset sound buffer
            (*_delegator_arr[11])("register");
        }
    }
    
    // Return in interval time
    if (_perkcolainterval > 0) return;
    
    // Collision AABB
    int result = 0;
    for (int i=1; i<_perkcola_aabb_arr.size(); i++)
    {
        if(_perkcola_aabb_arr[i]->hit) result = i;
    }
    
    
    // Text
    
    // Align
    (*_delegator_arr[3])("center");
    // Positon
    (*_delegator_arr[4])(glm::vec2(0.5, 0.4));
    // Color
    (*_delegator_arr[5])(glm::vec3(1,1,0.98));
    // Size
    (*_delegator_arr[6])(2.8f);
    
    int cost = 0;
    switch (result) {
        case 1:
            cost = 2500;
            // Text
            (*_delegator_arr[2])("Action to buy a Duff  [Cost :2500]");
            // Render
            (*_delegator_arr[7])();
            break;
        case 2:
            cost = 1500;
            // Text
            (*_delegator_arr[2])("Action to buy a Coke  [Cost :1500]");
            // Render
            (*_delegator_arr[7])();
            break;
        case 3:
            cost = 1000;
            // Text
            (*_delegator_arr[2])("Action to buy a Soda  [Cost :1000]");
            // Render
            (*_delegator_arr[7])();
            break;
        default:
            break;
    }
    
    // Action
    int playerscore = (*_delegator_arr[9])(0);
    
    if (result != 0 && playerscore > cost && ( keycon(KEY_ACTION, KEY_ON) || padcon(PAD_ACTION, KEY_ON) ) )
    {
        if (!_perkcola) {
            // Perk true
            _perk[result-1] = true;
            // Drinking!!! (Playermanager::ProcessActionDrink)
            (*_delegator_arr[13])();
            // Decrement player score
            (*_delegator_arr[8])(-cost);
            // Play register sound once
            (*_delegator_arr[10])("register");
            _perkcola = true;
        }
    }
    
}







void EventManager::_InitializeMisteryBox()
{
    
    // Create AABB collision group and get index
    int createdgroup = (*_delegator_arr[0])(0);
    
    // AABB player
    AABB * player_aabb = new AABB(glm::vec3(0),
                                  glm::vec3(0),
                                  createdgroup);
    
    // AABB mistery box
    AABB * misterybox_aabb = new AABB(glm::vec3(18.9971, 0.022925, 11.8369),
                                      glm::vec3(19.8055, 1.44072, 10.0052),
                                      createdgroup);
    
    // Submit AABB
    (*_delegator_arr[1])(player_aabb);
    (*_delegator_arr[1])(misterybox_aabb);
    
    // Add
    _misterybox_aabb_arr.push_back(player_aabb);
    _misterybox_aabb_arr.push_back(misterybox_aabb);
    
}

void EventManager::_ProcessMisteryBox()
{
    
    // Update player position
    float player_aabb_radius = 0.2;
    _misterybox_aabb_arr[0]->Update(glm::vec3(_playerposition.x-player_aabb_radius,
                                              _playerposition.y,
                                              _playerposition.z+player_aabb_radius),
                                    glm::vec3(_playerposition.x+player_aabb_radius,
                                              _playerposition.y+player_aabb_radius,
                                              _playerposition.z-player_aabb_radius));

    
    // Interval
    if (_weaponbox) {
        _weaponbox_interval ++;
        if (_weaponbox_interval > 100) {
            _weaponbox_interval = 0;
            _weaponbox = false;
            // Reset sound buffer
            (*_delegator_arr[11])("register");
        }
    }
    
    // Return in interval time
    if (_weaponbox_interval > 0) return;
    
    
    
    int result = 0;
    for (int i=1; i<_misterybox_aabb_arr.size(); i++)
    {
        if(_misterybox_aabb_arr[i]->hit) result = i;
    }
    
    // Text
    
    // Align
    (*_delegator_arr[3])("center");
    // Positon
    (*_delegator_arr[4])(glm::vec2(0.5, 0.4));
    // Color
    (*_delegator_arr[5])(glm::vec3(1,1,0.98));
    // Size
    (*_delegator_arr[6])(2.8f);
    
    int cost = 0;
    switch (result) {
        case 1:
            cost = 750;
            // Text
            (*_delegator_arr[2])("Action for misterybox+  [Cost :750]");
            // Render
            (*_delegator_arr[7])();
            break;
        default:
            break;
    }
    
    // Action
    int playerscore = (*_delegator_arr[9])(0);
    if (result != 0 && playerscore > cost && ( keycon(KEY_ACTION, KEY_ON) || padcon(PAD_ACTION, KEY_ON) ) )
    {
        if (!_weaponbox) {
            // Get new weapon!!! (PlayerManager::ProcessActionMisteryBox)
            (*_delegator_arr[14])();
            // Decrement player score
            (*_delegator_arr[8])(-cost);
            // Play register sound once
            (*_delegator_arr[10])("register");
            _weaponbox = true;
        }
    }
    
}




void EventManager::_InitializeRunningSound()
{
    
    // Status
    _runsoundonconcreate = false;
    _walksoundonconcreate = false;
    _runsoundonwood = false;
    _walksoundonwood = false;
    
    // Create AABB collision group and get index
    int createdgroup = (*_delegator_arr[0])(0);
    
    // AABB player
    AABB * player_aabb = new AABB(glm::vec3(0),
                                  glm::vec3(0),
                                  createdgroup);
    
    // AABB wooden 1
    AABB * wooden1_aabb = new AABB(glm::vec3(5.17135, 0.0, 13.5262),
                                   glm::vec3(9.46807, 2.0, 4.38191),
                                   createdgroup);
    // AABB wooden 2
    AABB * wooden2_aabb = new AABB(glm::vec3(9.17135, 0.0, 10.5262),
                                   glm::vec3(10.2183, 2.0, 7.50432),
                                   createdgroup);
    
    // Submit AABB
    (*_delegator_arr[1])(player_aabb);
    (*_delegator_arr[1])(wooden1_aabb);
    (*_delegator_arr[1])(wooden2_aabb);
    
    // Add
    _runningsound_aabb_arr.push_back(player_aabb);
    _runningsound_aabb_arr.push_back(wooden1_aabb);
    _runningsound_aabb_arr.push_back(wooden2_aabb);
}

void EventManager::_ProcessRunningSound()
{
    
    bool jump = (*_delegator_arr[15])(0);
    bool fall = (*_delegator_arr[16])(0);
    bool land = (*_delegator_arr[17])(0);
    bool walk = (*_delegator_arr[18])(0);
    bool run  = (*_delegator_arr[19])(0);
    bool pose = (*_delegator_arr[20])(0);
    
    // Update player position
    float player_aabb_radius = 0.2;
    _runningsound_aabb_arr[0]->Update(glm::vec3(_playerposition.x-player_aabb_radius,
                                                _playerposition.y,
                                                _playerposition.z+player_aabb_radius),
                                      glm::vec3(_playerposition.x+player_aabb_radius,
                                                _playerposition.y+player_aabb_radius,
                                                _playerposition.z-player_aabb_radius));
    
    // Stop all audio in flying
    if (jump || fall || pose) {
        // Stop all sound using in Running
        _ProcessRunningSoundAllStop();
        // False all boolean usin in running
        _ProcessRunningBooleanAllFalse();
        // Break void
        return;
    }

    // Collision
    int result = 0;
    for (int i=1; i<_runningsound_aabb_arr.size(); i++)
    {
        // Hit!!
        if(_runningsound_aabb_arr[i]->hit) result = i;
    }
    
    switch (result) {
        case 1:
        case 2:
            // On Wood
            if (run && !_runsoundonwood) {
                _ProcessRunningBooleanAllFalse();
                _runsoundonwood = true;
                _ProcessRunningSoundAllStop();
                (*_delegator_arr[21])("run_wood");
            }else if(walk && !_walksoundonwood) {
                _ProcessRunningBooleanAllFalse();
                _walksoundonwood = true;
                _ProcessRunningSoundAllStop();
                (*_delegator_arr[21])("walk_wood");
            }
            break;
        default:
            // On Concreate
            if (run && !_runsoundonconcreate) {
                _ProcessRunningBooleanAllFalse();
                _runsoundonconcreate = true;
                _ProcessRunningSoundAllStop();
                (*_delegator_arr[21])("run_concreate");
            }else if(walk && !_walksoundonconcreate) {
                _ProcessRunningBooleanAllFalse();
                _walksoundonconcreate = true;
                _ProcessRunningSoundAllStop();
                (*_delegator_arr[21])("walk_concreate");
            }
            break;
    }
    
}

void EventManager::_ProcessRunningSoundAllStop()
{
    
    // Stop
    (*_delegator_arr[22])("run_concreate");
    (*_delegator_arr[22])("run_wood");
    (*_delegator_arr[22])("walk_concreate");
    (*_delegator_arr[22])("walk_wood");
    
    // Reset
    (*_delegator_arr[23])("run_concreate");
    (*_delegator_arr[23])("run_wood");
    (*_delegator_arr[23])("walk_concreate");
    (*_delegator_arr[23])("walk_wood");
    
}

void EventManager::_ProcessRunningBooleanAllFalse()
{
    _runsoundonconcreate  = false;
    _walksoundonconcreate = false;
    _runsoundonwood       = false;
    _walksoundonwood      = false;
}




void EventManager::_InitializeRound()
{
    
    // Create AABB collision group and get index
    int createdgroup = (*_delegator_arr[0])(0);
    
    // AABB player
    AABB * player_aabb = new AABB(glm::vec3(0),
                                  glm::vec3(0),
                                  createdgroup);
    
    // AABB mistery box
    AABB * escape_aabb = new AABB(glm::vec3(5.17135, 0.0, 13.5262),
                                  glm::vec3(9.46807, 2.0, 4.38191),
                                  createdgroup);
    
    // Submit AABB
    (*_delegator_arr[1])(player_aabb);
    (*_delegator_arr[1])(escape_aabb);
    
    // Add
    _escape_aabb_arr.push_back(player_aabb);
    _escape_aabb_arr.push_back(escape_aabb);
    
}

void EventManager::_ProcessRound()
{
    
    
    // Round
    
    // Trigger only once
    if (_lastround != _round) {
        
        // Trigger reset
        _lastround = _round;
        
        // Chapter on
        _chapter = true;
        
        // Next goal kill count (Basiclly Added 20 count)
        _goalkillcount = _killcount + 19;
        
        // Sound
        /* AudioManager::audiomanager_PlaySoundOnce(std::string); */
        (*_delegator_arr[11])("roundstart");
        (*_delegator_arr[10])("roundstart");
    
    }
    
    // Round 1~3
    if (_round < 3) {
        
        switch (_killcount) {
            case    0: _round = 1  ; break;
            case    5: _round = 2  ; break;
            case   12: _round = 3  ; break;
            default  :               break;
        }
        
    // Another round
    }else{

        // Reached goal kill count
        if (_goalkillcount < _killcount) {

            // Go to next round
            _round ++;
            
        }
        
    }


    
    
    // Chapter
    
    std::string title;
    
    switch (_round) {
        case   1: title = "OPENING, WELCOME TO THE NAZI PARADE"      ; break;
        case   5: title = "CHAPTER ONE, WHERE AM I"                  ; break;
        case  10: title = "CHAPTER TWO, DARK SIDE"                   ; break;
        case  15: title = "CHAPTER THREE, KEEP GOING"                ; break;
        case  20: title = "CHAPTER FOUR, WELCOME TO THE HELL"        ; break;
        case  30: title = "CHAPTER FIVE, THE CENTER OF THE HELL"     ; break;
        case  50: title = "CHAPTER SIX, RUNNER'S HIGH"               ; break;
        case  70: title = "CHAPTER SEVEN, SILVER LINING"             ; break;
        case  90: title = "CHAPTER EIGHT, SIGNAL RECEPTION"          ; break;
        case 100: title = "ENDED, OPERATION DYSTOPIA"                ; break;
        case 101: title = "CHAPTER, OVER MY DEAD BODY"               ; break;
        default : title = ""                                         ; break;
    }
    
    static float d = 0;
    if (_chapter) {
        d += 0.001;
    }else{
        d = 0;
        title = "";
    }
    
    if (_round == 1 && d == 0) {
        (*_delegator_arr[11])("radiomorus");
        (*_delegator_arr[11])("a1259");
        (*_delegator_arr[11])("gooutsidenow");
        (*_delegator_arr[11])("operationdinnerout");
    }
    
    if (_round == 70 && d > 0.5) {
        /* (I heard something...) AudioManager::audiomanager_PlaySoundOnce(std::string) */
        (*_delegator_arr[10])("radiomorus");
    }
    
    if (_round == 90) {
        if (d > 0) {
            /* (OMG, Singnal reception from USArmy!!!) AudioManager::audiomanager_PlaySoundOnce(std::string) */
            (*_delegator_arr[10])("a1259");
        }
        if (d > 0.7) {
            /* (And last battle rock'n roll!!!) AudioManager::audiomanager_PlaySoundOnce(std::string) */
            (*_delegator_arr[10])("operationdinnerout");
        }
    }
    
    if (_round == 100) {
        if (d > 0) {
            /* (This is the last battle!!!) AudioManager::audiomanager_PlaySoundOnce(std::string) */
            (*_delegator_arr[10])("gooutsidenow");
        }
    }
    
    if (d > 1) {
        _chapter = false;
    }
    
    // Text
    // Align
    (*_delegator_arr[3])("left");
    // Positon
    (*_delegator_arr[4])(glm::vec2(0.03, 0.90));
    // Color
    (*_delegator_arr[5])(glm::vec3(0.87843,0.91764,0.93333));
    // Size
    (*_delegator_arr[6])(2.2f);
    // Text
    (*_delegator_arr[2])(title);
    // Render
    (*_delegator_arr[7])();
    
    
    
    
    
    
    // Escape
    
    // Update player position
    float player_aabb_radius = 0.2;
    _escape_aabb_arr[0]->Update(glm::vec3(_playerposition.x-player_aabb_radius,
                                          _playerposition.y,
                                          _playerposition.z+player_aabb_radius),
                                glm::vec3(_playerposition.x+player_aabb_radius,
                                          _playerposition.y+player_aabb_radius,
                                          _playerposition.z-player_aabb_radius));
    

    int result = 0;
    for (int i=1; i<_escape_aabb_arr.size(); i++)
    {
        if(_escape_aabb_arr[i]->hit) result = i;
    }
    
    // Action
    if (result != 0 && _round == 100)
    {
        /* EndingManager::GameOver(); */
        (*_delegator_arr[24])();
    }
    
    
}





int EventManager::RecievePerk(int value){
    return (int)_perk[value];
}
int EventManager::RecieveRound(int value){
    return _round;
}
int EventManager::RecieveKillCount(int value){
    return _killcount;
}
int EventManager::UpdateKillCount(int value){
    return _killcount += value;
}


void EventManager::UpdateDelegator(Delegator* delegator){
    
    /**
     * [ 0 ] CollisionManager::ProcessCreateAABBGroup(int);
     * [ 1 ] CollisionManager::ProcessSubmitAABB(AABB);
     * [ 2 ] FontManager::UpdateText(std::string);
     * [ 3 ] FontManager::UpdateAlign(std::string);
     * [ 4 ] FontManager::UpdatePosition(glm::vec2);
     * [ 5 ] FontManager::UpdateColor(glm::vec3);
     * [ 6 ] FontManager::UpdateSize(float);
     * [ 7 ] FontManager::RenderMarlboro();
     * [ 8 ] PlayerManager::playermanager_UpdateScore(int);
     * [ 9 ] PlayerManager::playermanager_RecieveScore(int);
     * [10 ] AudioManager::audiomanager_PlaySoundOnce(std::string);
     * [11 ] AudioManager::audiomanager_PlaySoundReset(std::string);
     * [12 ] PlayerManager::playermanager_RecieveCurrentPosition(std::string);
     * [13 ] PlayerManager::playermanager_ProcessActionDrink();
     * [14 ] PlayerManager::playermanager_ProcessActionMisteryBox();
     * [15 ] ControlManager::RecieveConditionJump(int);
     * [16 ] ControlManager::RecieveConditionFall(int);
     * [17 ] ControlManager::RecieveConditionLand(int);
     * [18 ] ControlManager::RecieveConditionWalk(int);
     * [19 ] ControlManager::RecieveConditionRun(int);
     * [20 ] ControlManager::RecieveConditionPose(int);
     * [21 ] AudioManager::LoopSoundOnce(std::string);
     * [22 ] AudioManager::LoopSoundOnceStop(std::string)
     * [23 ] AudioManager::LoopSoundOnceReset(std::string);
     * [24 ] EndingManager::GameOver();
     */
    
    _delegator_arr.push_back(delegator);
}
