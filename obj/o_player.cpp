//
//  o_player.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "o_player.hpp"
#include "util/skinning.h"

O_Player::O_Player(PlayerStructShader                &shd_file,
                   std::vector<PlayerStructTexture>  &tex_file_arr,
                   std::vector<PlayerStructObject>   &obj_file_arr,
                   std::vector<PlayerStructKeyframe> &kfm_file_arr,
                   std::vector<PlayerStructWeight>   &wgt_file_arr)
{
    
    // Update _size
    _objsize = (int)obj_file_arr.size();
    _kfmsize = (int)kfm_file_arr.size();
    
    // Initilaize Weapon part by A_Weapon Cls
    for (int i=0; i<_objsize; i++) {
        
        // Push back in array (Texture is only 2!! So on right arm "i is 0". left arm "i is 1". -> (int)(i/2))
        _a_player_arr.push_back(new A_Player(obj_file_arr[i], tex_file_arr[(int)(i/2)], shd_file));
        
    }
    
    
    // Initialize weigth
    for (int i=0; i<_objsize; i++) {
        
        // Initialization of Skinning
        Skinning* _skinning = new Skinning();
        // Setup weight
        _skinning->initWeight(wgt_file_arr[i].wgt);
        
        // Create each empty array
        std::vector<std::vector<float>> indexed_weights;
        std::vector<glm::vec3> indexed_vertices;
        
        // Recieve indexed vertices
        _a_player_arr[i]->RecieveIndexedVertices(indexed_vertices);
        
        // Sort weight in the same order as vertices!! This is important!!
        _skinning->sortWeight(obj_file_arr[i].obj,indexed_vertices,indexed_weights);
        
        // Update _indexed_weights
        _a_player_arr[i]->UpdateIndexedWeights(indexed_weights);
        
        // Update bone size
        _a_player_arr[i]->UpdateBonesSize((int)indexed_weights.size());
        
        // Weight vbos initialization
        _a_player_arr[i]->InitializeWeightVBO();
        
        // Push back in array
        _skinning_arr.push_back(_skinning);
    }
    
    
    // Initialize Keyframe
    for (int i=0; i<_kfmsize; i++) {
        
        // Keyframe can set multiple
        std::vector<const char *> eachkeyframe = {
            kfm_file_arr[i].pose,
            kfm_file_arr[i].fire,
            kfm_file_arr[i].reload,
            kfm_file_arr[i].aim,
            kfm_file_arr[i].snip,
            kfm_file_arr[i].change
        };
        
        // Setup keyframe animation
        _skinning_arr[(int)(i%4)]->initKeyframe(eachkeyframe);
        
    }
    
}
O_Player::~O_Player(){
    for (auto e : _a_player_arr) delete e;
    for (auto e : _skinning_arr) delete e;
}

void O_Player::Render()
{
    int count = 0;
    for (auto e : _a_player_arr)
    {
        glm::mat4 modelmatrix = glm::mat4(1);
        switch (count) {
            case 1:  modelmatrix = _modelmatrix * _skinning_arr[0]->getLocalBoneMatrix(2); break;
            case 3:  modelmatrix = _modelmatrix * _skinning_arr[2]->getLocalBoneMatrix(2); break;
            default: modelmatrix = _modelmatrix; break;
        }
        e->UpdateModelMatrix(modelmatrix);
        e->UpdateFlashPosition(_muzzleflashposition);
        e->UpdateLightPosition(_lightposition);
        e->UpdateLightUpPosition(glm::vec3(12.0,0.0,8.0));
        e->UpdateUsingTexture(0);
        e->Render();
        count++;
    }
}

void O_Player::UpdateSkinMeshAnimation(const int &weapon, const int &vbo0, const int &vbo1){
    // Update each vbo frame
    _weapon = weapon;
    _vbo0   = vbo0;
    _vbo1   = vbo1;
    for (int i=0; i<_objsize; i++) {
        
        // Play skin mesh animation
        // Get mvp matrix from all a_player Cls.
        glm::mat4 mvp = _a_player_arr[i]->RecieveMVPMatrix();
        
        // Calculate skinning matrix using mvp.
        _skinning_arr[i]->playSkinMVP(mvp, _a_player_arr[i]->bonematrix);
        
        // Rendering skin mesh animation
        _skinning_arr[i]->renderSkin(_weapon*6+_vbo0, _vbo1);
        
    }
}

void O_Player::UpdateModelMatrix(const glm::mat4 &modelmatrix){
    _modelmatrix = modelmatrix;
}
void O_Player::UpdateMuzzleFlashPosition(const glm::vec3 &position){
    _muzzleflashposition = position;
}
void O_Player::UpdateLightPosition(const glm::vec3 &position){
    _lightposition = position;
}
glm::mat4 O_Player::RecieveWorldBoneMatrix(const int &object, const int &bone){
    return _skinning_arr[object]->getWorldBoneMatrix(bone);
}

glm::mat4 O_Player::RecieveLocalBoneMatrix(const int &object, const int &bone){
    return _skinning_arr[object]->getLocalBoneMatrix(bone);
}
