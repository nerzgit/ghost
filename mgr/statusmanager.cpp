//
//  statusmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/15.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "statusmanager.hpp"
#include "abs/a_plane.hpp"
#include "del/delegate.hpp"
#include "usge/clock.hpp"





StatusManager::StatusManager()
{
    _play       = true;
    _fontcolor  = glm::vec3(0.87843,0.91764,0.93333);
    _magazine   = 0;
    _extra      = 0;
    _weapon     = "";
    _score      = 0;
    _round      = 0;
    _lastround  = 0;
    _perk[0]    = false;
    _perk[1]    = false;
    _perk[2]    = false;
    _life       = 100000;
    _perkicon_coke = new A_Plane("game/game/oj/_another_/status/badge_m_coke.DDS",0.38f);
    _perkicon_soda = new A_Plane("game/game/oj/_another_/status/badge_m_soda.DDS",0.38f);
    _perkicon_beer = new A_Plane("game/game/oj/_another_/status/badge_m_rootbeer.DDS",0.38f);
    _player_blood  = new A_Plane("game/game/oj/_another_/status/blood.dds",3.3f);
}

StatusManager::~StatusManager()
{
    for (auto e : _delegate_arr) delete e;
    delete _perkicon_coke;
    delete _perkicon_soda;
    delete _perkicon_beer;
    delete _player_blood;
}

void StatusManager::Reset()
{
    _play       = true;
    _fontcolor  = glm::vec3(0.87843,0.91764,0.93333);
    _magazine   = 0;
    _extra      = 0;
    _weapon     = "";
    _score      = 0;
    _round      = 0;
    _lastround  = 0;
    _perk[0]    = false;
    _perk[1]    = false;
    _perk[2]    = false;
    _life       = 100000;
}





/* In Rendering */
void StatusManager::Render()
{
    
    if (!_play) {
        return;
    }
    
    _score    = (*_delegate_arr[6])(0); // Score(int);
    _magazine = (*_delegate_arr[7])(0); // MagazineAmmo(int);
    _extra    = (*_delegate_arr[8])(0); // ExtraAmmo(int);
    _weapon   = (*_delegate_arr[9])(""); // WeaponName(std::string);
    _round    = (*_delegate_arr[13])(0); // RecieveRound(int);
    for (int i=0; i<3; i++) {
        _perk[i] = (*_delegate_arr[10])(i); // Perk(int);
    }
    _life = (*_delegate_arr[11])(0); // Life(int);
    _maxlife = (*_delegate_arr[12])(0); // MaxLife(int);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    _RenderBlood();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    _RenderPerk();
    _RenderMagazine();
    _RenderWeapon();
    _RenderScore();
    _RenderRound();
    
}




void StatusManager::_RenderMagazine(){
    
    // Int to String
    std::string m = std::to_string(_magazine);
    std::string s = std::to_string(_extra);
    
    // If not empty data
    if ("" != m)
    {
        (*_delegate_arr[0])(m);                         // Text
        (*_delegate_arr[1])("right");                   // Align
        (*_delegate_arr[2])(glm::vec2(0.786, 0.08));    // Position
        (*_delegate_arr[3])(_fontcolor);                // Color
        (*_delegate_arr[4])(4.8f);                      // Size
        (*_delegate_arr[5])();                          // Render(RomanSerif)
    }
    
    // If not empty data
    if ("" != s)
    {
        (*_delegate_arr[0])(s);                         // Text
        (*_delegate_arr[1])("left");                    // Align
        (*_delegate_arr[2])(glm::vec2(0.79666, 0.08));  // Position
        (*_delegate_arr[3])(_fontcolor);                // Color
        (*_delegate_arr[4])(2.6f);                      // Size
        (*_delegate_arr[5])();                          // Render(RomanSerif)
    }
    
}

void StatusManager::_RenderWeapon(){
    
    // If not empty data
    if ("" != _weapon)
    {
        (*_delegate_arr[0])(_weapon);                   // Text
        (*_delegate_arr[1])("center");                  // Align
        (*_delegate_arr[2])(glm::vec2(0.5, 0.08));      // Position
        (*_delegate_arr[3])(_fontcolor);                // Color
        (*_delegate_arr[4])(2.2f);                      // Size
        (*_delegate_arr[5])();                          // Render(RomanSerif)
    }
    
}

void StatusManager::_RenderScore(){
    
    // Int to String
    std::string s = std::to_string(_score);
    
    // If not empty data
    if ("" != s)
    {
        (*_delegate_arr[0])(s);                         // Text
        (*_delegate_arr[1])("right");                   // Align
        (*_delegate_arr[2])(glm::vec2(0.22533, 0.08));  // Position
        (*_delegate_arr[3])(_fontcolor);                // Color
        (*_delegate_arr[4])(2.6f);                      // Size
        (*_delegate_arr[5])();                          // Render(RomanSerif)
    }
    
}

void StatusManager::_RenderRound(){
    
    // counter...
    static int counter = 0;
    
    // Trigger
    if (_lastround != _round && g_CLOCK->RecieveFPS() ) {
        
        // increase counter
        counter++;
        
        // Uppper than 300 (300 frame later)
        if (counter > 200) {
            counter = 0;
            _lastround = _round;
            
        }
    }
    
    // Int to String
    std::string s = std::to_string(_lastround);
    
    // If not empty data
    if ("" != s)
    {
        (*_delegate_arr[0])(s);                         // Text
        (*_delegate_arr[1])("left");                   // Align
        (*_delegate_arr[2])(glm::vec2(0.23666, 0.08));  // Position
        (*_delegate_arr[3])(_fontcolor);                // Color
        (*_delegate_arr[4])(4.8f);                      // Size
        (*_delegate_arr[5])();                          // Render(RomanSerif)
    }
    
}

void StatusManager::_RenderPerk(){
    
    // Render mini perk badge
    if (_perk[0]) {
        // RootBeer
        glClear(GL_DEPTH_BUFFER_BIT);
        _perkicon_beer -> UpdatePosition(glm::vec3(1.75,0.8,0));
        _perkicon_beer -> UpdateColor(glm::vec4(1,1,1,1));
        _perkicon_beer -> Render();
    }
    if (_perk[1]) {
        // Coke
        glClear(GL_DEPTH_BUFFER_BIT);
        _perkicon_coke -> UpdatePosition(glm::vec3(1.75,0.8,0));
        _perkicon_coke -> UpdateColor(glm::vec4(1,1,1,1));
        _perkicon_coke -> Render();
    }
    if (_perk[2]) {
        // Soda
        glClear(GL_DEPTH_BUFFER_BIT);
        _perkicon_soda -> UpdatePosition(glm::vec3(1.75,0.8,0));
        _perkicon_soda -> UpdateColor(glm::vec4(1,1,1,1));
        _perkicon_soda -> Render();
    }
    
}

void StatusManager::_RenderBlood(){
    
    float alpha = ((float)_maxlife - (float)_life) / (float)_maxlife;
    
    if (alpha > 0.8) {
        alpha = 0.8;
    }
    
    _player_blood -> UpdatePosition(glm::vec3(0,-0.8,0));
    _player_blood -> UpdateColor(glm::vec4(1,1,1,alpha));
    _player_blood -> Render();
    
}

int StatusManager::UpdatePlay(int value){
    return _play = value;
}

void StatusManager::UpdateDelegator(Delegator* delegator)
{
    
    /**
     * [ 0 ] FontManager::UpdateText(std::string);
     * [ 1 ] FontManager::UpdateAlign(std::string);
     * [ 2 ] FontManager::UpdatePosition(glm::vec2);
     * [ 3 ] FontManager::UpdateColor(glm::vec3);
     * [ 4 ] FontManager::UpdateSize(float);
     * [ 5 ] FontManager::RenderRomanSerif();
     * [ 6 ] PlayerManager::RecieveScore(int);
     * [ 7 ] PlayerManager::RecieveMagazineAmmo(int);
     * [ 8 ] PlayerManager::RecieveExtraAmmo(int);
     * [ 9 ] PlayerManager::RecieveWeaponName(std::string);
     * [10 ] PlayerManager::RecievePerk(int);
     * [11 ] PlayerManager::RecieveLife(int);
     * [12 ] PlayerManager::RecieveMaxLife(int);
     * [13 ] EventManager::RecieveRound(int);
     */
    
    _delegate_arr.push_back(delegator);
    
}
