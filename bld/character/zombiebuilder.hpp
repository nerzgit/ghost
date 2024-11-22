//
//  zombiebuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef zombiebuilder_hpp
#define zombiebuilder_hpp

#include "bld/characterbuilder.hpp"

class ZombieBuilder : public CharacterBuilder
{
public:
    ZombieBuilder(std::string file);
    ~ZombieBuilder();
    O_Character* RecieveCharacter();
    O_Character* RecieveCopyCharacter();
};

#endif /* zombiebuilder_hpp */
