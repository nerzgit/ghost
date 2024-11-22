//
//  waltherp38builder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef waltherp38builder_hpp
#define waltherp38builder_hpp

#include "bld/weaponbuilder.hpp"

class WaltherP38Builder : public WeaponBuilder
{
    
public:
    WaltherP38Builder(std::string file);
    ~WaltherP38Builder();
    O_Weapon* RecieveWeapon();
};

#endif /* waltherp38builder_hpp */
