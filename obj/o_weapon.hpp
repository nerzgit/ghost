//
//  o_weapon.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef o_weapon_hpp
#define o_weapon_hpp

#include "dta/player.h"
#include "abs/a_weapon.hpp"
#include <vector>

class SkinningAux;

class O_Weapon
{
private:
    
    // Status
    int _magazineammo;
    int _extraammo;
    
    int _vbo0;
    int _vbo1;
    int _objsize;
    int _kfmsize;
    glm::vec3 _muzzleflashposition;
    glm::vec3 _lightposition;
    std::vector<A_Weapon*> _a_weapon_arr;
    std::vector<SkinningAux*> _skinningaux_arr;
    
    std::string _name;
    PlayerStructSoundEffect _sfx_file;
    PlayerStructControl _ctl_file;
    PlayerStructStatus _sts_file;
    
public:
    O_Weapon(PlayerStructTexture               &tex_file,
             PlayerStructShader                &shd_file,
             std::vector<PlayerStructObject>   &obj_file_arr,
             std::vector<PlayerStructKeyframe> &kfm_file_arr);
    O_Weapon();
    ~O_Weapon();
    void Render();
    void UpdateConstraintMatrix(const glm::mat4 &matrix);
    void UpdateSkinMeshAnimation(const int &vbo0, const int &vbo1);
    void UpdateModelMatrix(const glm::mat4 &matrix);
    void UpdateStructSoundEffect(const PlayerStructSoundEffect in_struct);
    void UpdateStructControl(const PlayerStructControl in_struct);
    void UpdateStructStatus(const PlayerStructStatus in_struct);
    void UpdateMagazineAmmo(const int &ammo);
    void UpdateExtraAmmo(const int &ammo);
    void UpdateIncreaseMagazineAmmo(const int &ammo);
    void UpdateIncreaseExtraAmmo(const int &ammo);
    void UpdateMuzzleFlashPosition(const glm::vec3 &position);
    void UpdateLightPosition(const glm::vec3 &position);
    void RecieveStructSoundEffect(PlayerStructSoundEffect &out_struct);
    void RecieveStructControl(PlayerStructControl &out_struct);
    void RecieveStructStatus(PlayerStructStatus &out_struct);
    int  RecieveMagazineAmmo();
    int  RecieveExtraAmmo();
    glm::mat4 RecieveModelMatrix(const int &part);
    glm::mat4 RecieveConstraintMatrix(const int &part);
};

#endif /* o_weapon_hpp */
