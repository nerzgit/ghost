//
//  skinning_s.h
//  play
//
//  Created by IDE KENTARO on 2019/04/24.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef skinning_s_h
#define skinning_s_h

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

class SkinningAux
{
private:
    int        fps;
    int        frame;
    
    glm::mat4  animationmatrix;
    
    // Keyframe [Animation type][frame][vec3TRS]
    std::vector<std::vector<std::vector<glm::vec3>>> vec_keyframe;
    
public:
    
    SkinningAux();
    ~SkinningAux();
    
    // Read and compile keyframe file
    void initKeyframe(std::vector<const char*> &keyframefile);
    // Creation matrix for play animation keyframe
    void playSkin(const int &animation, const int &frame);
    
    glm::mat4 RecieveAnimationMartix();
    
};

#endif /* skinning_s_h */
