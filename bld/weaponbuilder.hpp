//
//  weaponbuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef weaponbuilder_hpp
#define weaponbuilder_hpp

#include "obj/o_weapon.hpp"
#include <string>


class WeaponBuilder
{
    
protected:
    
    std::string _name;
    
    O_Weapon*       _o_weapon;
    
    PlayerStructSoundEffect            _weapon_sfx_file;
    PlayerStructControl                _weapon_ctl_file;
    PlayerStructStatus                 _weapon_sts_file;
    PlayerStructTexture                _weapon_tex_file;
    PlayerStructShader                 _weapon_shd_file;
    std::vector<PlayerStructObject>    _weapon_obj_file;
    std::vector<PlayerStructKeyframe>  _weapon_kfm_file;
    std::vector<PlayerStructKeyframe>  _player_kfm_file;
    
public:
    WeaponBuilder(const std::string &path);
    ~WeaponBuilder();
    virtual O_Weapon* RecieveWeapon(){ return 0; }
    std::vector<PlayerStructKeyframe> RecievePlayerKeyFrameFile();
};

class WeaponDirector
{
public:
    O_Weapon* RecieveInitializer(WeaponBuilder* builder);
    std::vector<PlayerStructKeyframe> RecievePlayerKeyFrameFile(WeaponBuilder* builder);
};


#endif /* weaponbuilder_hpp */
