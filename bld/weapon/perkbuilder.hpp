//
//  perkbuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/23.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef perkbuilder_hpp
#define perkbuilder_hpp

#include "bld/weaponbuilder.hpp"

class PerkBuilder : public WeaponBuilder
{
    
public:
    PerkBuilder(std::string file);
    ~PerkBuilder();
    O_Weapon* RecieveWeapon();
};


#endif /* perkbuilder_hpp */
