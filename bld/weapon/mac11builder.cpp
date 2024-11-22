//
//  mac11builder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/08/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "mac11builder.hpp"
#include "obj/o_weapon.hpp"

MAC11Builder::MAC11Builder(std::string file) : WeaponBuilder(file)
{
    WeaponBuilder::_name = "MAC11";
}

MAC11Builder::~MAC11Builder()
{
    
}

O_Weapon* MAC11Builder::RecieveWeapon()
{
    WeaponBuilder::_o_weapon = new O_Weapon(WeaponBuilder::_weapon_tex_file,
                                            WeaponBuilder::_weapon_shd_file,
                                            WeaponBuilder::_weapon_obj_file,
                                            WeaponBuilder::_weapon_kfm_file);
    _o_weapon->UpdateStructSoundEffect(WeaponBuilder::_weapon_sfx_file);
    _o_weapon->UpdateStructControl(WeaponBuilder::_weapon_ctl_file);
    _o_weapon->UpdateStructStatus(WeaponBuilder::_weapon_sts_file);
    return WeaponBuilder::_o_weapon;
}
