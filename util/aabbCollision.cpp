//
//  aabbCollision.cpp
//  play
//
//  Created by IDE KENTARO on 2019/05/10.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "aabbCollision.hpp"

#include <vector>
#include <glm/glm.hpp>

bool AABBCollision::AABBtoAABB(AABB *aabb1, AABB *aabb2)
{
    
    //Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
    return(aabb1->pos1[0] > aabb2->pos0[0] &&
           aabb1->pos0[0] < aabb2->pos1[0] &&
           aabb1->pos1[1] > aabb2->pos0[1] &&
           aabb1->pos0[1] < aabb2->pos1[1] &&
           aabb1->pos0[2] > aabb2->pos1[2] &&
           aabb1->pos1[2] < aabb2->pos0[2]);
    
    //If not, it will return false
    
}
