//
//  statusmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/15.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef statusmanager_hpp
#define statusmanager_hpp


#include <glm/glm.hpp>
#include <string>
#include <vector>

class Delegator;
class A_Plane;
class Clock;
extern Clock* g_CLOCK;

class StatusManager
{
private:
    bool _play;
    std::vector<Delegator*> _delegate_arr;
    glm::vec3 _fontcolor;
    int _magazine;
    int _extra;
    std::string _weapon;
    int _score;
    int _round;
    int _lastround;
    bool _perk[3];
    int _life;
    int _maxlife;
    A_Plane* _perkicon_coke;
    A_Plane* _perkicon_soda;
    A_Plane* _perkicon_beer;
    A_Plane* _player_blood;
    void _RenderMagazine();
    void _RenderWeapon();
    void _RenderScore();
    void _RenderRound();
    void _RenderPerk();
    void _RenderBlood();
    
public:
    StatusManager();
    ~StatusManager();
    void Render();
    void Reset();
    int UpdateMagazine(int &value);
    int UpdateExtra(int &value);
    std::string UpdateWeapon(std::string &name);
    int UpdateScore(int &value);
    int UpdateRound(int &value);
    int UpdatePerk(int &value);
    int UpdateBlood(int &value);
    int UpdatePlay(int value);
    void UpdateDelegator(Delegator* delegator);
    
};

#endif /* statusmanager_hpp */
