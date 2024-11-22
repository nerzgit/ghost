//
//  o_character.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef o_character_hpp
#define o_character_hpp

#include "dta/player.h"
#include "abs/a_character.hpp"
#include <vector>

class Skinning;

class O_Character : public A_Character
{
private:
    Skinning*   _skinning;
    int         _vbo0;
    int         _vbo1;
    
    // Status
    CharacterStructKeyframe _kfm_file;
    CharacterStructWeight   _wgt_file;
    CharacterStructStatus   _sts_file;
    bool        _play;
    int         _power;
    int         _speed;
    int         _life;
    bool        _fire;
    bool        _dead;
    bool        _hit;
    glm::vec3   _orientation;
    glm::vec3   _position;
    glm::vec3   _scale;
    
    // Scream
    std::vector<bool> _screaming;
    std::vector<int>  _screamtime;
    std::vector<int>  _screamaudiobufferindex;
    
    // Target
    glm::vec3   _targetposition;
    glm::vec3   _targetorientation;
    float       _targetdistance;
    bool        _targetfire;
    
    void _UpdateSkinMeshAnimation();
    void _UpdateModelMatrix();
    
public:
    O_Character(CharacterStructShader   &shd_file,
                CharacterStructTexture  &tex_file,
                CharacterStructObject   &obj_file,
                CharacterStructKeyframe &kfm_file,
                CharacterStructWeight   &wgt_file,
                CharacterStructStatus   &sts_file,
                CharacterStructIntel    &ite_file);
    O_Character();
    ~O_Character();
    void Render();
    void Reset();
    O_Character* Clone();
    void UpdateVBO0(const int &vbo);
    void UpdateVBO1(const int &vbo);
    void UpdateOrientation(const glm::vec3 &orientation);
    void UpdatePosition(const glm::vec3 &position);
    void UpdateScale(const glm::vec3 &scale);
    void UpdateTargetPosition(const glm::vec3 &position);
    void UpdateTargetOrientation(const glm::vec3 &orientation);
    void UpdateTargetDistance(const float &distance);
    void UpdateTargetFire(const bool &fire);
    void UpdatePlay(const bool &value);
    void UpdateHit(const bool &value);
    void UpdateFire(const bool &value);
    void UpdateLife(const int &value);
    void UpdateDead(const bool &value);
    void UpdateScreaming(const int &index, const bool &value);
    void UpdateScreamtime(const int &index, const int &value);
    void UpdateScreamAudioBufferIndex(const int &index, const int &value);
    glm::mat4 RecieveWorldBoneMatrix(const int &bone);
    glm::mat4 RecieveLocalBoneMatrix(const int &bone);
    int RecieveVBO0();
    int RecieveVBO1();
    glm::vec3 RecieveOrientation();
    glm::vec3 RecievePosition();
    glm::vec3 RecieveScale();
    glm::vec3 RecieveTargetPosition();
    glm::vec3 RecieveTargetOrientation();
    float RecieveTargetDistance();
    bool RecievePlay();
    int RecievePower();
    bool RecieveHit();
    bool RecieveFire();
    int  RecieveLife();
    bool RecieveDead();
    bool RecieveTargetFire();
    bool RecieveScreaming(const int &index);
    int RecieveScreamtime(const int &index);
    int RecieveScreamAudioBufferIndex(const int &index);
};


#endif /* o_character_hpp */
