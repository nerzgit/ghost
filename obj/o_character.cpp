//
//  o_character.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "o_character.hpp"
#include "util/skinning.h"



O_Character::O_Character(CharacterStructShader   &shd_file,
                         CharacterStructTexture  &tex_file,
                         CharacterStructObject   &obj_file,
                         CharacterStructKeyframe &kfm_file,
                         CharacterStructWeight   &wgt_file,
                         CharacterStructStatus   &sts_file,
                         CharacterStructIntel    &ite_file
                         ) : A_Character(
                                         obj_file,
                                         tex_file,
                                         shd_file
                        )
{
    
    // Keyframe can set multiple
    std::vector<const char *> eachkeyframe = {
        kfm_file.action0,
        kfm_file.action1,
        kfm_file.action2,
        kfm_file.action3,
        kfm_file.action4,
        kfm_file.action5,
        kfm_file.action6,
        kfm_file.action7,
        kfm_file.action8,
        kfm_file.action9
    };
    
    
    // Character Status struct
    _sts_file = sts_file;
    
    // Character weight struct
    _wgt_file = wgt_file;
    
    // Character keyfrane struct
    _kfm_file = kfm_file;
    
    // Initialization of Skinning
    _skinning = new Skinning();
    
    // Setup weight
    _skinning->initWeight(wgt_file.wgt);
    
    // Sort weight as vertices order
    // Create each empty array
    std::vector<std::vector<float>> indexed_weights;
    std::vector<glm::vec3> indexed_vertices;
    
    // Recieve indexed vertices
    A_Character::RecieveIndexedVertices(indexed_vertices);
    
    // Sorting
    _skinning->sortWeight(obj_file.obj,indexed_vertices,indexed_weights);
    
    // Update _indexed_weights
    A_Character::UpdateIndexedWeights(indexed_weights);
    
    // Update bone size
    A_Character::UpdateBonesSize((int)indexed_weights.size());
    
    // Weight vbos initialization
    A_Character::InitializeWeightVBO();
    
    // Setup keyframe animation
    _skinning->initKeyframe(eachkeyframe);
    
    // Character start moving? or not?
    _play = false;
    
    // Character power
    _power = _sts_file.power;
    
    // Character speed
    _speed = _sts_file.speed;
    
    // Character life
    _life = _sts_file.life;
    
    // Character dead
    _dead = false;
    
    // Character take damage
    _hit = false;
    
    // Distance to target
    _targetdistance = 0;
    
    // Target firing? or not?
    _targetfire = false;
    
    // Character orientation
    _orientation = glm::vec3(0);
    
    // Character position
    _position = glm::vec3(9999);
    
    // Character scale
    _scale = glm::vec3(1);
    
    // Animation frame
    _vbo0 = 0;
    _vbo1 = 0;
    
    // Scream
    _screaming = {0,0,0};
    _screamtime = {0,0,0};
    _screamaudiobufferindex = {0,0,0};
    
}

O_Character::O_Character():A_Character(){}

O_Character::~O_Character(){
    delete _skinning;
}

O_Character* O_Character::Clone()
{
    O_Character* clone = new O_Character();
    
    // O_Character's clone
    clone->_kfm_file               = this->_kfm_file;
    clone->_wgt_file               = this->_wgt_file;
    clone->_sts_file               = this->_sts_file;
    clone->_play                   = this->_play;
    clone->_power                  = this->_power;
    clone->_speed                  = this->_speed;
    clone->_life                   = this->_life;
    clone->_dead                   = this->_dead;
    clone->_hit                    = this->_hit;
    clone->_targetdistance         = this->_targetdistance;
    clone->_targetfire             = this->_targetfire;
    clone->_orientation            = this->_orientation;
    clone->_position               = this->_position;
    clone->_scale                  = this->_scale;
    clone->_vbo0                   = this->_vbo0;
    clone->_vbo1                   = this->_vbo1;
    clone->_screaming              = this->_screaming;
    clone->_screamtime             = this->_screamtime;
    clone->_screamaudiobufferindex = this->_screamaudiobufferindex;
    
    // Initialization of Skinning
    clone->_skinning = new Skinning();
    // Setup weight
    clone->_skinning->initWeight(clone->_wgt_file.wgt);
    // Setup keyframe animation
    // Keyframe can set multiple
    std::vector<const char *> eachkeyframe = {
        clone->_kfm_file.action0,
        clone->_kfm_file.action1,
        clone->_kfm_file.action2,
        clone->_kfm_file.action3,
        clone->_kfm_file.action4,
        clone->_kfm_file.action5,
        clone->_kfm_file.action6,
        clone->_kfm_file.action7,
        clone->_kfm_file.action8,
        clone->_kfm_file.action9
    };
    clone->_skinning->initKeyframe(eachkeyframe);
    
    // A_Character's clone
    A_Character::Clone(clone);
    
    return clone;
}




void O_Character::Render()
{
    _UpdateModelMatrix();
    _UpdateSkinMeshAnimation();
    A_Character::Render();
    
}

void O_Character::_UpdateSkinMeshAnimation(){
 
    // Play skin mesh animation
    glm::mat4 mvp = A_Character::RecieveMVPMatrix();
    _skinning->playSkinMVP(mvp, A_Character::bonematrix);
    
    // Rendering skin mesh animation
    _skinning->renderSkin(_vbo0, _vbo1);
    
}

void O_Character::_UpdateModelMatrix(){
    glm::mat4 rotationmatrix = glm::eulerAngleYXZ(_orientation.y, _orientation.x, _orientation.z);
    glm::mat4 translationmatrix = translate(glm::mat4(1.0), _position);
    glm::mat4 scalingmatrix = scale(glm::mat4(1.0), _scale);
    glm::mat4 modelmatrix = translationmatrix * rotationmatrix * scalingmatrix;
    A_Character::UpdateModelMatrix(modelmatrix);
}


void O_Character::Reset(){
    
    // Character power
    _power = _sts_file.power;
    
    // Character speed
    _speed = _sts_file.speed;
    
    // Character life
    _life = _sts_file.life;
    
    // Character start moving? or not?
    _play = false;
    
    // Character dead
    _dead = false;
    
    // Character take damage
    _hit = false;
    
    // Distance to target
    _targetdistance = 0;
    
    // Target firing? or not?
    _targetfire = false;
    
    // Character orientation
    _orientation = glm::vec3(0);
    
    // Character position
    _position = glm::vec3(9999);
    
    // Character scale
    _scale = glm::vec3(1);
    
    // Animation frame
    _vbo0 = 0;
    _vbo1 = 0;
    
    // Scream
    _screaming = {0,0,0};
    _screamtime = {0,0,0};
    
}


void O_Character::UpdateVBO0(const int &vbo){
    _vbo0 = vbo;
}
void O_Character::UpdateVBO1(const int &vbo){
    _vbo1 = vbo;
}
void O_Character::UpdateOrientation(const glm::vec3 &orientation){
    _orientation = orientation;
}
void O_Character::UpdatePosition(const glm::vec3 &position){
    _position = position;
}
void O_Character::UpdateScale(const glm::vec3 &scale){
    _scale = scale;
}
void O_Character::UpdateTargetPosition(const glm::vec3 &position){
    _targetposition = position;
}
void O_Character::UpdateTargetOrientation(const glm::vec3 &orientation){
    _targetorientation = orientation;
}
void O_Character::UpdateTargetDistance(const float &distance){
    _targetdistance = distance;
}
void O_Character::UpdatePlay(const bool &value){
    _play = value;
}
void O_Character::UpdateHit(const bool &value){
    _hit = value;
}
void O_Character::UpdateFire(const bool &value){
    _fire = value;
}
void O_Character::UpdateLife(const int &value){
    _life += value;
}
void O_Character::UpdateDead(const bool &value){
    _dead = value;
}
void O_Character::UpdateTargetFire(const bool &fire){
    _targetfire = fire;
}
void O_Character::UpdateScreaming(const int &index, const bool &value){
    _screaming[index] = value;
}
void O_Character::UpdateScreamtime(const int &index, const int &value){
    _screamtime[index] = value;
}
void O_Character::UpdateScreamAudioBufferIndex(const int &index, const int &value){
    _screamaudiobufferindex[index] = value;
}
glm::mat4 O_Character::RecieveWorldBoneMatrix(const int &bone){
    return _skinning->getWorldBoneMatrix(bone);
}
glm::mat4 O_Character::RecieveLocalBoneMatrix(const int &bone){
    return _skinning->getLocalBoneMatrix(bone);
}
int O_Character::RecieveVBO0(){
    return _vbo0;
}
int O_Character::RecieveVBO1(){
    return _vbo1;
}
glm::vec3 O_Character::RecieveOrientation(){
    return _orientation;
}
glm::vec3 O_Character::RecievePosition(){
    return _position;
}
glm::vec3 O_Character::RecieveScale(){
    return _scale;
}
glm::vec3 O_Character::RecieveTargetPosition(){
    return _targetposition;
}
glm::vec3 O_Character::RecieveTargetOrientation(){
    return _targetorientation;
}
float O_Character::RecieveTargetDistance(){
    return _targetdistance;
}
bool O_Character::RecievePlay(){
    return _play;
}
int O_Character::RecievePower(){
    return _power;
}
bool O_Character::RecieveHit(){
    return _hit;
}
bool O_Character::RecieveFire(){
    return _fire;
}
int O_Character::RecieveLife(){
    return _life;
}
bool O_Character::RecieveDead(){
    return _dead;
}
bool O_Character::RecieveTargetFire(){
    return _targetfire;
}
bool O_Character::RecieveScreaming(const int &index){
    return _screaming[index];
}
int O_Character::RecieveScreamtime(const int &index){
    return _screamtime[index];
}
int O_Character::RecieveScreamAudioBufferIndex(const int &index){
    return _screamaudiobufferindex[index];
}
