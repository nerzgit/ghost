//
//  ray_obbCollision.hpp
//  play
//
//  Created by IDE KENTARO on 2019/06/10.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef ray_obbCollision_hpp
#define ray_obbCollision_hpp


#include <glm/glm.hpp>

struct RayOBB{
    
    // *** Using in CollisionManager ***
    int  group = -1;
    int  index = -1;
    bool hit   = false;
    
    glm::vec3 min; // AABB min
    glm::vec3 max; // AABB max
    glm::mat4 M;   // Model matrix
    float     xpt; // intersection distance
    RayOBB(glm::vec3 min, glm::vec3 max, glm::mat4 M, int group) :min(min), max(max), M(M), group(group){}

    void Update(glm::mat4 modelmatrix){M = modelmatrix;}
    
};

class RayOBBCollision{
private:
    glm::vec3 ray_origin;
    glm::vec3 ray_direction;
    
public:
    void updateRay(
                   double& mouseX, double& mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
                   int& screenWidth, int& screenHeight,  // Window size, in pixels
                   glm::mat4& ViewMatrix,               // Camera position and orientation
                   glm::mat4& ProjectionMatrix         // Camera parameters (ratio, field of view, near and far planes)
    );
    
    bool ray_obbCollision(RayOBB* rayobb);
};

#endif /* ray_obbCollision_hpp */
