//
//  ManBuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef ManBuilder_hpp
#define ManBuilder_hpp

#include "bld/playerbuilder.hpp"

class ManBuilder : public PlayerBuilder
{
    
public:
    ManBuilder(std::string file);
    ~ManBuilder();
    void UpdateInitializer();
    O_Player* RecievePlayer();
};

#endif /* ManBuilder_hpp */
