//
//  endingmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/21.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "endingmanager.hpp"
#include "del/delegate.hpp"
#include "abs/a_plane.hpp"
#include "usge/clock.hpp"
#include "usge/key.hpp"

EndingManager::EndingManager()
{
    
    _life = 10000;
    _terrain = 0;
    _position = glm::vec3(0);
    _playerdead = false;
    _playerdeadcount = 0;
    _playerdeadanimation = false;
    _playerdeadbeziercount = 0;
    _playerdeadposition = glm::vec3(0);
    _ending = false;
    _endingscorebeziercount = 0;
    _curtain = new A_Plane("game/game/oj/_another_/menu/background.dds",100.0f);
    _curtainalpha = 1.0;
    
}

EndingManager::~EndingManager()
{
    for (auto e : _delegate_arr) delete e;
    delete _curtain;
}

void EndingManager::Render()
{
    
    // Syncronize player life
    _life = (*_delegate_arr[9])(0);
    _position = (*_delegate_arr[10])(glm::vec3(0));
    
    // Player Dead and camera control
    _ProcessPlayerDead();
    
    // Showing score
    _ProcessEndingScore();
    
}

void EndingManager::_ProcessPlayerDead()
{
    
    // Life is zero
    if (_life < 1) {
        _playerdead = true;
    }
    
    // Bloody animation start? or not?
    if (!_playerdead) {
        return;
    }
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // PlayerManager::playermanager_UpdatePlay(int);
    (*_delegate_arr[11])(0);
    // ControlManager::controlmanager_UpdatePlay(int);
    (*_delegate_arr[12])(0);
    
    float downheight = 1.1f;
    
    // Syncronize player position only once then animation start
    if (!_playerdeadanimation) {
        _playerdeadposition = _position;
        _playerdeadanimation = true;
    }
    
    if (g_CLOCK->RecieveFPS() && _playerdeadanimation) {
        
        
        
        // Increase timer
        _playerdeadcount ++;
        if (_playerdeadcount == 100) {
            /* AudioManager::PlaySoundOnceReset(std::string); */
            (*_delegate_arr[29])("dead");
            /* AudioManager::PlaySoundOnce(std::string); */
            (*_delegate_arr[25])("dead");
        }
        if (_playerdeadcount > 400) {
            
            // Start _ProcessEndingScore();
            _ending = true;
            
        }
        
        
        
        
        
        // Blood animation
        
        // Bezier curve
        glm::vec2 xy1, xy2, xy3, xy4;
        xy1 = glm::vec2( 0.0,        0.0 );
        xy2 = glm::vec2( 0.5,       0.62 );
        xy3 = glm::vec2( 0.5,       0.22 );
        xy4 = glm::vec2( 1.0, downheight );
        
        float f_t = 0;
        if (g_CLOCK->RecieveFPS()) {
            f_t = _playerdeadbeziercount += 0.05f;
        }
        float f_y = (1-f_t)*(1-f_t)*(1-f_t)*xy1.y + 3*((1-f_t)*(1-f_t))*f_t*xy2.y + 3*((1-f_t))*(f_t*f_t)*xy3.y + (f_t*f_t*f_t)*xy4.y;
        
        if (f_t > 1) {
            f_y = downheight;
        }
        
        glm::vec3 newposition = _playerdeadposition - glm::vec3(0, f_y, 0);
        
        // Update position
        (*_delegate_arr[8])(newposition);
        
    }
    
    
    /* GOOD ENGING */
    if (_life > 0 && !_ending) {
        
        (*_delegate_arr[0])("HELL ENDED");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.59));
        (*_delegate_arr[3])(glm::vec3(0.87843,0.91764,0.93333));
        (*_delegate_arr[4])(4.8f);
        (*_delegate_arr[27])();
        
        (*_delegate_arr[0])("You Are The Only Survivor Of This Land");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.47));
        (*_delegate_arr[3])(glm::vec3(0.87843,0.91764,0.93333));
        (*_delegate_arr[4])(2.2f);
        (*_delegate_arr[5])();
    }
    
    
}

void EndingManager::_ProcessEndingScore()
{
    
    if (!_ending) {
        return;
    }
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // StatusManager::statusmanager_UpdatePlay(int);
    (*_delegate_arr[13])(0);
    // PlayerManager::playermanager_UpdateRender(int);
    (*_delegate_arr[14])(0);
    // CharacterManager::charactermanager_UpdatePlay1(int);
    (*_delegate_arr[15])(0);
    // ControlManager::controlmanager_UpdateDirection(glm::vec3);
    (*_delegate_arr[16])(glm::vec3(0,0,-1));
    
    // _ProcessPlayerDead() false
    _playerdead = false;
    
    
    

    
    // Text
    
    (*_delegate_arr[0])("GAME OVER");                           // Text
    (*_delegate_arr[1])("center");                              // Align
    (*_delegate_arr[2])(glm::vec2(0.5, 0.59));                  // Position
    (*_delegate_arr[3])(glm::vec3(0.87843,0.91764,0.93333));    // Color
    (*_delegate_arr[4])(6.2f);                                  // Size
    (*_delegate_arr[6])();                                      // Render(Later)
    
    /* EventManager::RecieveRound(int) */
    int round = (*_delegate_arr[22])(0);
    std::string rounttext;
    
    if (round == 1) {
        rounttext = "You Could Not Survive Even One Round";
    }else{
        rounttext = "You Survived " + std::to_string(round) + " Round";
    }
    
    (*_delegate_arr[0])(rounttext);
    (*_delegate_arr[1])("center");
    (*_delegate_arr[2])(glm::vec2(0.5, 0.47));
    (*_delegate_arr[3])(glm::vec3(0.87843,0.91764,0.93333));
    (*_delegate_arr[4])(2.2f);
    (*_delegate_arr[5])();
    
    /* EventManager::RecieveKillCount(int); */
    int killcount = (*_delegate_arr[23])(0);
    
    std::string killcounttext = "And Killed " + std::to_string(killcount) + " Zombie";
    
    (*_delegate_arr[0])(killcounttext);
    (*_delegate_arr[1])("center");
    (*_delegate_arr[2])(glm::vec2(0.5, 0.42));
    (*_delegate_arr[3])(glm::vec3(0.87843,0.91764,0.93333));
    (*_delegate_arr[4])(2.2f);
    (*_delegate_arr[5])();
    
    
    
    // Camera animation
    float f_t = 0;
    
    if ( g_CLOCK->RecieveFPS() ) {
        
        f_t = _endingscorebeziercount += 0.0006f;
        
        // Bezier curve
        glm::vec3 xyz1, xyz2, xyz3, xyz4;
        xyz1 = glm::vec3( 12.0, 3.0, 18.0 );
        xyz2 = glm::vec3( 12.0, 3.0, 18.0 );
        xyz3 = glm::vec3( 12.0, 3.0, 0.0 );
        xyz4 = glm::vec3( 12.0, 3.0, 0.0 );
        float f_x = (1-f_t)*(1-f_t)*(1-f_t)*xyz1.x + 3*((1-f_t)*(1-f_t))*f_t*xyz2.x + 3*((1-f_t))*(f_t*f_t)*xyz3.x + (f_t*f_t*f_t)*xyz4.x;
        float f_y = (1-f_t)*(1-f_t)*(1-f_t)*xyz1.y + 3*((1-f_t)*(1-f_t))*f_t*xyz2.y + 3*((1-f_t))*(f_t*f_t)*xyz3.y + (f_t*f_t*f_t)*xyz4.y;
        float f_z = (1-f_t)*(1-f_t)*(1-f_t)*xyz1.z + 3*((1-f_t)*(1-f_t))*f_t*xyz2.z + 3*((1-f_t))*(f_t*f_t)*xyz3.z + (f_t*f_t*f_t)*xyz4.z;
        
        if (f_t > 1) {
            f_x = 12.0;
            f_y = 3.0;
            f_z = 0.0;
        }
        
        glm::vec3 newposition = glm::vec3(f_x, f_y, f_z);
        
        // Update position
        (*_delegate_arr[8])(newposition);
        
    }
    
    

    
    // Curtain
    
    _curtain -> UpdatePosition(glm::vec3(0,0,0));
    _curtain -> UpdateColor(glm::vec4(1,1,1,_curtainalpha));
    _curtain -> Render();

    if (f_t > 0.78) {
        _curtainalpha += 0.01f;
        if (_curtainalpha > 1) {
            _curtainalpha = 1;
        }
    }else {
        _curtainalpha -= 0.01f;
        if (_curtainalpha < 0) {
            _curtainalpha = 0;
        }
    }
    
    
    
    
    // Reset
    
    if (f_t > 1.0) {
        
        /* MapManager::Reset(); */
        (*_delegate_arr[17])();
        /* PlayerManager::Reset(); */
        (*_delegate_arr[18])();
        /* CharacterManager::Reset(); */
        (*_delegate_arr[19])();
        /* ControlManager::Reset(); */
        (*_delegate_arr[20])();
        /* EventManager::Reset(); */
        (*_delegate_arr[21])();
        /* AudioManager::Reset(); */
        (*_delegate_arr[24])();
        /* OpeningManager::Reset(); */
        (*_delegate_arr[26])();
        /* StatusManager::Reset(); */
        (*_delegate_arr[28])();
        
        _life = 10000;
        _terrain = 0;
        _position = glm::vec3(0);
        _playerdead = false;
        _playerdeadcount = 0;
        _playerdeadanimation = false;
        _playerdeadbeziercount = 0;
        _playerdeadposition = glm::vec3(0);
        _endingscorebeziercount = 0;
        _curtainalpha = 1.0f;
        
        _ending     = false;
        _playerdead = false;
        
    }
    
}

void EndingManager::GameOver()
{
    _playerdead = true;
}

void EndingManager::UpdateDelegator(Delegator* delegator)
{
    /**
     * [ 0 ] FontManager::UpdateText(std::string);
     * [ 1 ] FontManager::UpdateAlign(std::string);
     * [ 2 ] FontManager::UpdatePosition(glm::vec2);
     * [ 3 ] FontManager::UpdateColor(glm::vec3);
     * [ 4 ] FontManager::UpdateSize(float);
     * [ 5 ] FontManager::RenderOpenSansRegular();
     * [ 6 ] FontManager::RenderLater();
     * [ 7 ] FontManager::RenderMarlbolo();
     * [ 8 ] ControlManager::UpdatePosition(glm::vec3);
     * [ 9 ] PlayerManager::RecieveLife();
     * [10 ] ControlManager::RecievePosition(glm::vec3);
     * [11 ] PlayerManager::playermanager_UpdatePlay(int);
     * [12 ] ControlManager::controlmanager_UpdatePlay(int);
     * [13 ] StatusManager::statusmanager_UpdatePlay(int);
     * [14 ] PlayerManager::playermanager_UpdateRender(int);
     * [15 ] CharacterManager::charactermanager_UpdatePlay1(int);
     * [16 ] ControlManager::controlmanager_UpdateDirection(glm::vec3);
     * [17 ] MapManager::mapmanager_Reset();
     * [18 ] PlayerManager::playermanager_Reset();
     * [19 ] CharacterManager::charactermanager_Reset();
     * [20 ] ControlManager::controlmanager_Reset();
     * [21 ] EventManager::eventmanager_Reset();
     * [22 ] EventManager::RecieveRound(int);
     * [23 ] EventManager::RecieveKillCount(int);
     * [24 ] AudioManager::Reset();
     * [25 ] AudioManager::PlaySoundOnce(std::string);
     * [26 ] OpeningManager::Reset();
     * [27 ] FontManager::RenderRomanSerif();
     * [28 ] StatusManager::Reset();
     * [29 ] AudioManager::PlaySoundOnceReset(std::string);
     */
    _delegate_arr.push_back(delegator);
}
