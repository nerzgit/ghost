//
//  playermanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef playermanager_hpp
#define playermanager_hpp

#include "dta/player.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Delegator;
class O_Weapon;
class O_Player;
class PlayerBuilder;
class WeaponBuilder;
class WeaponDecorator;
class Clock;
extern GLFWwindow*  g_WINDOW;
extern glm::mat4    g_PROJECTIONMATRIX;
extern glm::mat4    g_VIEWMATRIX;
extern Clock*       g_CLOCK;

class PlayerManager
{
private:
    
    std::vector<Delegator*> _delegator_arr;
    std::vector<O_Weapon*> _weapon_arr;
    std::vector<O_Player*> _player_arr;
    std::vector<PlayerBuilder*> _playerbuilder_arr;
    std::vector<WeaponBuilder*> _weaponbuilder_arr;
    std::vector<WeaponDecorator*> _weapon_ballistic_arr;
    std::vector<WeaponDecorator*> _weapon_muzzleflash_arr;
    
    // Gameplay
    bool _play;
    bool _render;

    // Frame
    int _vbo0;
    int _vbo1;

    // Status
    int _playerarrnumber;  // Using character
    int _weaponarrnumber;  // Using weapon
    int _score;            // Player's score
    int _life;             // Player's life
    int _maxlife;          // Player's max life
    bool _dead;            // Is player dead
    int _power;            // Weapon's power
    bool _shoot;           // Is shooting
    float _fov;
    bool _actionaim;
    bool _actionsnip;
    bool _actionreload;
    bool _actionfire;
    bool _actionchange;
    bool _actionrun;
    bool _actiondrink;
    bool _usingdrinkchange;
    bool _usingrandomweapon;
    PlayerStructSoundEffect _weaponsoundeffect;
    PlayerStructControl _weaponcontrols; // Weapon frame controller (Always syncronize when change weapon)
    PlayerStructStatus _weaponstatus; // Weapon status (Always syncronize when change weapon)
    std::vector<int> _weaponasset;
    int _weaponassetarrnumber;
    int _lastweaponassetarrnumber;
    
    // Matrices and vectors rendering valiables
    glm::vec3 _condition;
    glm::vec3 _orientation;
    glm::vec3 _valanceposition;
    glm::vec3 _currentposition;
    glm::vec3 _lastposition;
    glm::vec3 _scaling;
    glm::mat4 _walkingshakerotationmatrix;
    glm::mat4 _runningshakerotationmatrix;
    glm::mat4 _modelmatrix;
    glm::vec3 _lightposition;
    glm::vec3 _renderposition;
    
    void _InitializePlayer();
    void _InitializeDecorator();
    void _ProcessActionRun();
    void _ProcessActionAim();
    void _ProcessActionSnip();
    void _ProcessActionReload();
    void _ProcessActionFire();
    void _ProcessActionChange();
    void _ProcessActionDrink();
    void _ProcessActionReset();
    void _ProcessLinkingAllStatus();
    
public:
    PlayerManager();
    ~PlayerManager();
    void Init();
    void Render();
    void Reset();
    std::string RecieveWeaponName(std::string value="");
    glm::vec3 RecieveCurrentPosition(glm::vec3 position);
    glm::vec3 RecieveLastPosition(glm::vec3 position);
    int RecieveShoot(int value=0);
    bool RecieveDead(bool value=0);
    int RecievePower(int value=0);
    int RecieveLife(int value=0);
    int RecieveMaxLife(int value=0);
    int RecieveMagazineAmmo(int value=0);
    int RecieveExtraAmmo(int value=0);
    int RecieveScore(int value=0);
    int UpdateScore(int value);
    int UpdateLife(int value);
    int UpdatePlay(int value);
    int UpdateRender(int value);
    void ProcessActionDrink();
    void ProcessActionMisteryBox();
    void UpdateDelegator(Delegator* delegator);
};

#endif /* playermanager_hpp */
