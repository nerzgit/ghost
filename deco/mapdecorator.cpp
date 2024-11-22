//
//  mapdecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/18.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "mapdecorator.hpp"

void MapDecorator::UpdateDecorator(O_Stuff* stuff)
{
    _o_stuff = stuff;
}

void MapDecorator::ProcessDust(){}
void MapDecorator::UpdateDustRandomizer(const glm::vec3 &randomizer){}
