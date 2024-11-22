//
//  mac11builder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/08/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef mac11builder_hpp
#define mac11builder_hpp

#include "bld/weaponbuilder.hpp"

class MAC11Builder : public WeaponBuilder
{
    
public:
    MAC11Builder(std::string file);
    ~MAC11Builder();
    O_Weapon* RecieveWeapon();
};

#endif /* mac11builder_hpp */
