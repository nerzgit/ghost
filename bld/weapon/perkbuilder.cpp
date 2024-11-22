//
//  perkbuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/23.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "perkbuilder.hpp"
#include "obj/o_weapon.hpp"

PerkBuilder::PerkBuilder(std::string file) : WeaponBuilder(file)
{
    WeaponBuilder::_name = "PerkCola";
}

PerkBuilder::~PerkBuilder()
{
    
}

O_Weapon* PerkBuilder::RecieveWeapon()
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
