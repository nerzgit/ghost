//
//  aabbCollision.hpp
//  play
//
//  Created by IDE KENTARO on 2019/05/10.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef aabbCollision_hpp
#define aabbCollision_hpp

#include <vector>
#include <glm/glm.hpp>

struct AABB
{
    
    // *** Using in CollisionManager ***
    int  group = -1;
    int  index = -1;
    bool hit   = false;
    
    glm::vec3 pos0;
    glm::vec3 pos1;
    
    AABB(const glm::vec3 &pos0, const glm::vec3 &pos1, const int &group) :pos0(pos0), pos1(pos1), group(group){}
    void Update(const glm::vec3 &pos0, const glm::vec3 &pos1){this->pos0 = pos0; this->pos1 = pos1;}

};

class AABBCollision
{
public:
    bool AABBtoAABB(AABB *aabb1, AABB *aabb2);
};

#endif /* aabbCollision_hpp */
