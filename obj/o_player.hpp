//
//  o_player.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef o_player_hpp
#define o_player_hpp

#include "dta/player.h"
#include "abs/a_player.hpp"
#include <vector>

class Skinning;

class O_Player
{
private:
    int             _weapon;
    int             _vbo0;
    int             _vbo1;
    int             _objsize;
    int             _kfmsize;
    std::vector<A_Player*> _a_player_arr;
    std::vector<Skinning*> _skinning_arr;
    
    std::string _name;
    
    glm::vec3 _muzzleflashposition;
    glm::vec3 _lightposition;
    glm::mat4 _modelmatrix;
    
public:
    O_Player(PlayerStructShader                &shd_file,
             std::vector<PlayerStructTexture>  &tex_file_arr,
             std::vector<PlayerStructObject>   &obj_file_arr,
             std::vector<PlayerStructKeyframe> &kfm_file_arr,
             std::vector<PlayerStructWeight>   &wgt_file_arr);
    ~O_Player();
    void Render();
    void UpdateSkinMeshAnimation(const int &weapon, const int &vbo0, const int &vbo1);
    void UpdateModelMatrix(const glm::mat4 &modelmatrix);
    void UpdateMuzzleFlashPosition(const glm::vec3 &position);
    void UpdateLightPosition(const glm::vec3 &position);
    glm::mat4 RecieveWorldBoneMatrix(const int &object, const int &bone);
    glm::mat4 RecieveLocalBoneMatrix(const int &object, const int &bone);
};

#endif /* o_player_hpp */
