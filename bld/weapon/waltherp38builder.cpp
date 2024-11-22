//
//  waltherp38builder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "waltherp38builder.hpp"
#include "obj/o_weapon.hpp"

WaltherP38Builder::WaltherP38Builder(std::string file) : WeaponBuilder(file)
{
    WeaponBuilder::_name = "WaltherP38";
}

WaltherP38Builder::~WaltherP38Builder()
{
    
}

O_Weapon* WaltherP38Builder::RecieveWeapon()
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
