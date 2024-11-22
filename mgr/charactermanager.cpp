//
//  charactermanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "charactermanager.hpp"
#include "del/delegate.hpp"
#include "obj/o_character.hpp"
#include "bld/characterbuilder.hpp"
#include "bld/character/zombiebuilder.hpp"
#include "deco/characterdecorator.hpp"
#include "deco/character/enterdecorator.hpp"
#include "deco/character/chasedecorator.hpp"
#include "deco/character/deaddecorator.hpp"
#include "deco/character/blooddecorator.hpp"
#include "util/randomizer.hpp"
#include "util/squareroot.hpp"
#include "util/ray_obbCollision.hpp"
#include "usge/key.hpp"
#include "usge/clock.hpp"
#include "usge/quaternion_utils.hpp"
#include "usge/audio.hpp"





CharacterManager::CharacterManager()
{
    
    _play = true;
    _play1 = true;
    _currentround = 0;
    _lastround = 0;
    _zombiesize = 21;
    _targetposition =
    _targetorientation = glm::vec3(0);
    _targetfire = false;
    
}

CharacterManager::~CharacterManager()
{
    for (auto e : _delegate_arr      ) delete e;
    for (auto e : _rayobb_arr        ) delete e;
    for (auto e : _o_zombie_arr      ) delete e;
    for (auto e : _zombiebuilder_arr ) delete e;
    for (auto e : _enter_arr         ) delete e;
    for (auto e : _chase_arr         ) delete e;
    for (auto e : _dead_arr          ) delete e;
    for (auto e : _blood_arr         ) delete e;
}




/* After update delegator */
void CharacterManager::Init(const std::string &map)
{
    
    // Create character
    _InitializeCharacter(map);
    
    // Create decorator
    _InitializeDecorator(map);
    
    // Initialize RayOBB
    _InitializeRayOBB();
    
    // Initialize Voices of zombie
    _InitializeZombieVoices();
    
}




/* Reinitialize */
void CharacterManager::Reset()
{
    
    // Reset all zombies
    for (auto e : _o_zombie_arr) e->Reset();
    _play = true;
    _play1 = true;
    
}





/* In Rendering */
void CharacterManager::Render()
{
    
    if (_play) {
    
        /* Syncronize */

        /* Controlmanager::RecievePosition(glm::vec3) */
        if (_play1) { _targetposition = (*_delegate_arr[2])(glm::vec3(0)); }
        /* Controlmanager::RecieveOrientation(glm::vec3) */
        _targetorientation = (*_delegate_arr[3])(glm::vec3(0));
        /* PlayerManager::RecieveShoot(int) */
        _targetfire = (bool)(*_delegate_arr[4])(0);
        /* PlayerManager::RecieveLife(int) */
        _targetlife = (*_delegate_arr[18])(0);
        /* EventManager::RecieveRound(int) */
        _currentround = (*_delegate_arr[17])(0);
        
        /* Zombie reborn and start */
        _ProcessZombieStartInOrder();
        _ProcessZombieReset();
        
    }
    
    // RayOBB
    _ProcessRayOBB();
    
    // Zombie
    for (auto x : _o_zombie_arr)
    {
        
        // Update target(player) position
        x->UpdateTargetPosition(_targetposition);
        // Update target(player) orientation
        x->UpdateTargetOrientation(_targetorientation);
        // Update target(player) fire? or not
        x->UpdateTargetFire(_targetfire);
        // Rendering
        x->Render();
        
        if ( _play && x->RecievePlay() && g_CLOCK->RecieveFPS() ) {
            
            // Play animation frame
            x->UpdateVBO1(x->RecieveVBO1()+1);
            
            if ( _targetlife > 0) {
            
                // Voice
                _ProcessRunZombieVoice(x);
                _ProcessDeadZombieVoice(x);
                
                // Zombie damage to player!!!
                if ( x->RecieveFire() ) {
                    /* PlayerManager::UpdateLife(int) */
                    (*_delegate_arr[14])(x->RecievePower() * -1);
                    
                    // Timing to player scream
                    static int playerscreaminterval = 0;
                    playerscreaminterval++;
                    if (playerscreaminterval > 10) {
                        playerscreaminterval = 0;
                    }
                    
                    /* AudioManager::PlaySoundOnce(std::string) */
                    switch (playerscreaminterval) {
                        case 3:
                            (*_delegate_arr[6])("haw");
                            (*_delegate_arr[5])("haw");
                            break;   // Player scream
                        case 8:
                            (*_delegate_arr[6])("ow");
                            (*_delegate_arr[5])("ow");
                            break;   // Player scream
                        default:
                            break;
                    }
                    (*_delegate_arr[6])("peshi");
                    (*_delegate_arr[5])("peshi"); // Zombie punch
                }
                
            }
        
        }
            
    }
    
    if (_play) {
        
        // Enter
        for (auto x : _enter_arr ) x->ProcessEnter();
        // Chase
        for (auto x : _chase_arr ) x->ProcessChase();
        // dead
        for (auto x : _dead_arr  ) x->ProcessDead();
        // Blood
        for (auto x : _blood_arr ) x->ProcessBlood();
        
    }
    
    
}








void CharacterManager::_InitializeCharacter(const std::string &map)
{
    
    /* Initialize character */
    
    CharacterDirector characterdirector;
    
    // Create zombie on builder design pattern
    CharacterBuilder *zombiebuilder = new ZombieBuilder("game/"+map+"/as/zombie/zombie.pa");
    _zombiebuilder_arr.push_back(zombiebuilder);
    for (int i=0; i<_zombiesize; i++) {
        if (i == 0) {
            _o_zombie_arr.push_back(characterdirector.RecieveInitializer(zombiebuilder));
        }else{
            _o_zombie_arr.push_back(characterdirector.RecieveCopy(zombiebuilder));
        }
    }
    
}






void CharacterManager::_InitializeDecorator(const std::string &map)
{
    
    /* Initialize decorator */
    
    for (auto x : _o_zombie_arr) {
        
        DoRandomizer();
        int randomizer = Randomizer(1, 2) - 1;
        
        // Enter
        CharacterDecorator* enterdecorator = new EnterDecorator();
        enterdecorator->UpdateEnterStyle(randomizer);
        enterdecorator->UpdateDecorator(x);
        _enter_arr.push_back(enterdecorator);
        
        // Chase
        CharacterDecorator* chasedecorator = new ChaseDecorator(map);
        chasedecorator->UpdateDelegator(_delegate_arr[19]);
        chasedecorator->UpdateDelegator(_delegate_arr[20]);
        chasedecorator->UpdateDelegator(_delegate_arr[21]);
        chasedecorator->UpdateDelegator(_delegate_arr[22]);
        chasedecorator->UpdateDelegator(_delegate_arr[23]);
        chasedecorator->UpdateDecorator(x);
        _chase_arr.push_back(chasedecorator);
        
        // Dead
        CharacterDecorator* deaddecorator = new DeadDecorator();
        deaddecorator->UpdateDelegator(_delegate_arr[19]);
        deaddecorator->UpdateDelegator(_delegate_arr[20]);
        deaddecorator->UpdateDelegator(_delegate_arr[21]);
        deaddecorator->UpdateDelegator(_delegate_arr[22]);
        deaddecorator->UpdateDelegator(_delegate_arr[23]);
        deaddecorator->UpdateDecorator(x);
        _dead_arr.push_back(deaddecorator);
        
        // Blood
        CharacterDecorator* blooddecorator = new BloodDecorator();
        blooddecorator->UpdateDecorator(x);
        _blood_arr.push_back(blooddecorator);
        
    }
    
}






void CharacterManager::_InitializeZombieVoices()
{
    
    for (auto x : _o_zombie_arr) {
        
        // Audio
        // Create audio buffer in AudioManager
        x->UpdateScreamAudioBufferIndex(0, (*_delegate_arr[7])(0));
        x->UpdateScreamAudioBufferIndex(1, (*_delegate_arr[7])(0));
        
    }
    
}






void CharacterManager::_InitializeRayOBB()
{
    
    // Create RayOBB collision group and get index
    /* CollisionManager::ProcessCreateRayOBBGroup(int) */
    int createdgroup = (*_delegate_arr[0])(0);
    
    // Initialize and set to CollisionManager
    for (auto e : _o_zombie_arr) {
        
        // Get obb vertices
        std::vector<glm::vec3> _obbvertices;
        e->RecieveOBBVertices(_obbvertices);
        
        // Initialize RayOBB
        RayOBB * rayobb = new RayOBB(_obbvertices[0], _obbvertices[1], glm::mat4(1.0f), createdgroup);
        
        // Set to CollisionManager
        /* CollisionManager::ProcessSubmitRayOBB(RayOBB) */
        (*_delegate_arr[1])(rayobb);
        
        // Push back
        _rayobb_arr.push_back(rayobb);
        
    }
    
}






void CharacterManager::_ProcessRayOBB()
{
    
    RayOBB* min = nullptr;
    
    /* Find nearest hit object */
    int count = 0;
    for (int i=0; i<_rayobb_arr.size(); i++) {
        
        if (!_o_zombie_arr[i]->RecievePlay()) continue;
        
        bool hit = _rayobb_arr[i]->hit;
        
        // Update hit
        _o_zombie_arr[i]->UpdateHit(hit);
        // Update modelmatrix rayobb
        glm::mat4 modelmatrix;
        _o_zombie_arr[i]->A_Character::RecieveModelMatrix(modelmatrix);
        _rayobb_arr[i]->Update(modelmatrix);
        // Update Muzzle flash position
        _o_zombie_arr[i]->A_Character::UpdateFlashPosition(glm::vec3(99999,99999,99999));
        // Update distance
        _o_zombie_arr[i]->UpdateTargetDistance(_rayobb_arr[i]->xpt);
        
        // Decide min
        if (hit && count == 0) {
            min = _rayobb_arr[i];
            count = 1;
            continue;
        }
        
        // If near than min, switching
        if (hit && min->xpt > _rayobb_arr[i]->xpt) {
            min = _rayobb_arr[i];
        }
        
    }
    
    
    /* Zombie hit!! and player fire!! */
    if (min && _o_zombie_arr[min->index]->RecieveTargetFire()) {
        
        // Update Muzzle flash position
        _o_zombie_arr[min->index]->A_Character::UpdateFlashPosition(_targetposition);
    
        if (!_o_zombie_arr[min->index]->RecieveDead()) {
            
            // Zombie take a damage
            _o_zombie_arr[min->index]->UpdateLife(-40);
            
            // Life is zero!!
            if (_o_zombie_arr[min->index]->RecieveLife() < 1) {
                
                // dead is true
                _o_zombie_arr[min->index]->UpdateDead(true);
                
                /* PlayerManager::UpdateScore(int) */
                (*_delegate_arr[15])(150);
                
                /* EventManager::UpdateKillCount(int) */
                (*_delegate_arr[16])(1);
                
            }else{
                
                /* PlayerManager::UpdateScore(int) */
                (*_delegate_arr[15])(50);
            
            }
            
        }
        
    }

    
}





void CharacterManager::_ProcessZombieStartInOrder()
{
    
    static int counter = 0;
    static int bornedzombie = 0;
    int maxzombie = 0;
    
    // Maximum zombie (See EventManager::_ProcessRound() )
    switch (_currentround) {
        case 0:  maxzombie =  0; break;
        case 1:  maxzombie =  5; break;
        case 2:  maxzombie =  7; break;
        default: maxzombie = 20; break;
    }
    
    // Round changed then bornedzombie count to zero
    if (_currentround != _lastround) {
        _lastround = _currentround;
        bornedzombie = 0;
    }
    
    // Update timer
    if (g_CLOCK->RecieveFPS()) {
        counter++;
    }
    
    // If timer in 100 count
    if (counter < 100 || bornedzombie >= maxzombie) {
        return;
    }
    
    for (auto e : _o_zombie_arr) {
        
        // Start moving zombie!!
        if (!e->RecievePlay()) {
            
            e->Reset();
            e->UpdatePlay(true);
            counter = 0;
            bornedzombie++;
            break;
            
        }
        
    }
    
}





void CharacterManager::_ProcessZombieReset()
{
    
    int deadzombiecounter = 0;
    
    for (auto e : _o_zombie_arr) {
        
        if (e -> RecieveDead()) {
            
            deadzombiecounter++;
            
        }
        
    }
    
    if (deadzombiecounter > _zombiesize - 1) {
        
        for (auto e : _o_zombie_arr) {
            
            if (e -> RecieveDead()) {
                
                e -> Reset();
                
            }
            
        }
        
    }
    
}




void CharacterManager::_ProcessRunZombieVoice(O_Character* character)
{
    
    
    if (character->RecieveDead()) return;

    
    // Update zombie position
    /* AudioManager::UpdatePosition(glm::vec3) */
    (*_delegate_arr[12])( character->RecievePosition() );
    
    
    
    /* Run scream */
    
    
    // Get sound buffer index
    int bufferindex = character->RecieveScreamAudioBufferIndex(0);
    
    // If O_Character::_screaming[0] is false
    if (!character->RecieveScreaming(0)) {
        // Update screaming true
        character->UpdateScreaming(0, true);
        
        /* AudioManager::MultiPlaySoundOnceReset(int)) */
        (*_delegate_arr[10])( bufferindex );
        
        /* AudioManager::UpdateMultiAudioindex(int) */
        (*_delegate_arr[13])( bufferindex );
        
        /* AudioManager::MultiPlaySoundOnce(std::string) */
        (*_delegate_arr[8])( "zombie_scream" );
        
    }
    
    
    /* Reset */
    
    
    // Recieve scream time
    int time = character->RecieveScreamtime(0);
    
    // 10000 count later
    if (time > 600) {
        
        // Update scream time to zero (counter)
        character->UpdateScreamtime(0, 0 );
        // Update Screaming false
        character->UpdateScreaming(0, false);
        
    }else{
        
        // Increase scream time (counter)
        character->UpdateScreamtime(0, time + 1 );
        
    }
    
}


void CharacterManager::_ProcessDeadZombieVoice(O_Character* character)
{
    
    
    if (!character->RecieveDead()) return;
    

    /* Off Run Voice*/
    
    // Get sound buffer index (Run screaming)
    int bufferindex = character->RecieveScreamAudioBufferIndex(0);
    /* AudioManager::MultiPlaySoundOnceStop(int) */
    (*_delegate_arr[9])( bufferindex );
    /* AudioManager::MultiPlaySoundOnceReset(int) */
    (*_delegate_arr[10])( bufferindex );
    
    
    /* Dead scream */
    
    // Get sound buffer index (Dead screaming)
    int bufferindex1 = character->RecieveScreamAudioBufferIndex(1);
    
    // If O_Character::_screaming[1] is false
    if (!character->RecieveScreaming(1)) {
        
        /* AudioManager::MultiPlaySoundOnceReset(int) */
        (*_delegate_arr[10])( bufferindex1 );
        
        /* AudioManager::UpdateMultiAudioindex(int) */
        (*_delegate_arr[13])( bufferindex1 );
        
        /* AudioManager::MultiPlaySoundOnce(std::string) */
        (*_delegate_arr[8])( "ahh" );
        
        // Update screaming true
        character->UpdateScreaming(1, true);
        
    }
    
}




int CharacterManager::UpdatePlay(int value){
    return _play = value;
}

int CharacterManager::UpdatePlay1(int value){
    return _play1 = value;
}

void CharacterManager::UpdateDelegator(Delegator* delegator)
{
    /**
     * [ 0 ] CollisionManager::ProcessCreateRayOBBGroup(int);
     * [ 1 ] CollisionManager::ProcessSubmitRayOBB(RayOBB);
     * [ 2 ] Controlmanager::RecievePosition(glm::vec3);
     * [ 3 ] Controlmanager::RecieveOrientation(glm::vec3);
     * [ 4 ] PlayerManager::RecieveShoot(int);
     * [ 5 ] AudioManager::PlaySoundOnce(std::string);
     * [ 6 ] AudioManager::PlaySoundOnceReset(std::string);
     * [ 7 ] AudioManager::CreateMultiPlaySoundBufferIndex(int);
     * [ 8 ] AudioManager::MultiPlaySoundOnce(std::string);
     * [ 9 ] AudioManager::MultiPlaySoundOnceStop(int));
     * [10 ] AudioManager::MultiPlaySoundOnceReset(int));
     * [11 ] AudioManager::UpdateTargetPosition(glm::vec3);
     * [12 ] AudioManager::UpdatePosition(glm::vec3);
     * [13 ] AudioManager::UpdateMultiAudioindex(int);
     * [14 ] PlayerManager::UpdateLife(int);
     * [15 ] PlayerManager::UpdateScore(int);
     * [16 ] EventManager::UpdateKillCount(int);
     * [17 ] EventManager::RecieveRound(int);
     * [18 ] PlayerManager::RecieveLife(int);
     * [19 ] CollisionManager::UpdateTargetCurrentPosition(glm::vec3);
     * [20 ] CollisionManager::UpdateTargetLastPosition(glm::vec3);
     * [21 ] CollisionManager::UpdateTargetRadius(float);
     * [22 ] CollisionManager::RecieveTerrainCollision(glm::vec3);
     * [23 ] CollisionManager::RecieveKabezuriCollision(glm::vec3);
     */
    _delegate_arr.push_back(delegator);
}
