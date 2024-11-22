//
//  o_weapon.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "o_weapon.hpp"
#include "util/skinningaux.h"

/**
 * Weapon has multiple parts as A_Weapon
 * So some struct is vector array
 */

O_Weapon::O_Weapon(PlayerStructTexture               &tex_file,
                   PlayerStructShader                &shd_file,
                   std::vector<PlayerStructObject>   &obj_file_arr,
                   std::vector<PlayerStructKeyframe> &kfm_file_arr
                   )
{

    // Update _objsize & _kfmsize (Basiclly same both size in o_weapon)
    _objsize = (int)obj_file_arr.size();
    _kfmsize = (int)kfm_file_arr.size();
    
    // Initilaize Weapon part by A_Weapon Cls
    for (auto e : obj_file_arr) {
        // Push back in array
        _a_weapon_arr.push_back(new A_Weapon(e, tex_file, shd_file));
    }
    
    // Initialize Keyframe
    for (auto e : kfm_file_arr) {
        // Keyframe can set multiple
        std::vector<const char *> eachkeyframe = {
            e.pose,
            e.fire,
            e.reload,
            e.aim,
            e.snip,
            e.change
        };
        // Initialization of SkinningAux
        SkinningAux* _skinningaux = new SkinningAux();
        // Setup keyframe animation
        _skinningaux->initKeyframe(eachkeyframe);
        // Push back in array
        _skinningaux_arr.push_back(_skinningaux);
    }
    
}

O_Weapon::O_Weapon(){}

O_Weapon::~O_Weapon(){
    for (auto e : _a_weapon_arr   ) delete e;
    for (auto e : _skinningaux_arr) delete e;
}

void O_Weapon::Render()
{
    for (auto e : _a_weapon_arr)
    {
        e->UpdateFlashPosition(_muzzleflashposition);
        e->UpdateLightPosition(_lightposition);
        e->UpdateLightUpPosition(glm::vec3(12.0,0.0,8.0));
        e->UpdateUsingTexture(0);
        e->Render();
    }
}


void O_Weapon::UpdateConstraintMatrix(const glm::mat4 &matrix)
{
    
    // constraint matrix using the first loop matrix.
    // Another loop object using first object's Synthesismatrix.
    
    // For another object.
    glm::mat4 weaponconstraintmatrix;
    for (int i=0; i<_objsize; i++) {
        
        // First object.
        if (i == 0) {
            // Apply constraint matrix.
            _a_weapon_arr[i]->UpdateConstraintMatrix(matrix);
            // Recieve first object synthesis matrix.
            weaponconstraintmatrix = _a_weapon_arr[i]->RecieveConstraintMatrix();
            
        // Another.
        }else{
            // Apply first objects sysnthesis matrix.
            _a_weapon_arr[i]->UpdateConstraintMatrix(weaponconstraintmatrix);
        }
    }
}

void O_Weapon::UpdateSkinMeshAnimation(const int &vbo0, const int &vbo1){
    // Update each vbo frame
    _vbo0 = vbo0;
    _vbo1 = vbo1;
    for (int i=0; i<_kfmsize; i++) {
        // Play skin mesh animation
        _skinningaux_arr[i]->playSkin(_vbo0, _vbo1);
        // Update animation matrix
        _a_weapon_arr[i]->UpdateSkinningMatrix(_skinningaux_arr[i]->RecieveAnimationMartix());
    }
}
void O_Weapon::UpdateModelMatrix(const glm::mat4 &matrix){
    // Apply all
    for (int i=0; i<_objsize; i++) {
        _a_weapon_arr[i]->UpdateModelMatrix(matrix);
    }
}
void O_Weapon::UpdateStructSoundEffect(const PlayerStructSoundEffect in_struct){
    _sfx_file = in_struct;
}
void O_Weapon::UpdateStructControl(const PlayerStructControl in_struct){
    _ctl_file = in_struct;
}
void O_Weapon::UpdateStructStatus(const PlayerStructStatus in_struct){
    _sts_file = in_struct;
    // Initialize ammo
    _magazineammo = _sts_file.magazineammo;
    _extraammo = _sts_file.extraammo;
}
void O_Weapon::UpdateMagazineAmmo(const int &ammo){
    _magazineammo = ammo;
}
void O_Weapon::UpdateExtraAmmo(const int &ammo){
    _extraammo = ammo;
}
void O_Weapon::UpdateIncreaseMagazineAmmo(const int &ammo){
    _magazineammo += ammo;
}
void O_Weapon::UpdateIncreaseExtraAmmo(const int &ammo){
    _extraammo += ammo;
}
int O_Weapon::RecieveMagazineAmmo(){
    return _magazineammo;
}
int O_Weapon::RecieveExtraAmmo(){
    return _extraammo;
}
void O_Weapon::RecieveStructSoundEffect(PlayerStructSoundEffect &out_struct){
    out_struct = _sfx_file;
}
void O_Weapon::RecieveStructControl(PlayerStructControl &out_struct){
    out_struct = _ctl_file;
}
void O_Weapon::RecieveStructStatus(PlayerStructStatus &out_struct){
    out_struct = _sts_file;
}
void O_Weapon::UpdateMuzzleFlashPosition(const glm::vec3 &position){
    _muzzleflashposition = position;
}
void O_Weapon::UpdateLightPosition(const glm::vec3 &position){
    _lightposition = position;
}
glm::mat4 O_Weapon::RecieveModelMatrix(const int &part){
    return _a_weapon_arr[part]->RecieveModelMatrix();
}
glm::mat4 O_Weapon::RecieveConstraintMatrix(const int &part){
    return _a_weapon_arr[part]->RecieveConstraintMatrix();
}
