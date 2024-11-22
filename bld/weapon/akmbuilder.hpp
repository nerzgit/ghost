//
//  akmbuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/23.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef akmbuilder_hpp
#define akmbuilder_hpp

#include "bld/weaponbuilder.hpp"

class AKMBuilder : public WeaponBuilder
{
    
public:
    AKMBuilder(std::string file);
    ~AKMBuilder();
    O_Weapon* RecieveWeapon();
};


#endif /* akmbuilder_hpp */
