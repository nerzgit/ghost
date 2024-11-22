//
//  skinning_s.cpp
//  play
//
//  Created by IDE KENTARO on 2019/04/24.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "skinningaux.h"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/quaternion_utils.hpp"

using namespace glm;

SkinningAux::SkinningAux(){}
SkinningAux::~SkinningAux(){}

void SkinningAux::initKeyframe(std::vector<const char*> &keyframefile)
{
    
    //
    // KEYFRAME Initialization
    //
    
    for (int i=0; i<keyframefile.size(); i++) {
        
        vec_keyframe.push_back({});

        FILE * file2 = fopen(keyframefile[i], "r");
        while( 1 ){
            
            char      line[128];
            
            int res = fscanf(file2, "%s", line);
            
            // EOF
            if (res == EOF)
                break;
            
            // FRAME
            if ( strcmp( line, "r" ) == 0 ){
                fscanf(file2, "%i\n", &frame);
            }
            
            // FPS
            if ( strcmp( line, "f" ) == 0 ){
                fscanf(file2, "%i\n", &fps);
            }
            
            // KEYFRAME
            else if ( strcmp( line, "m" ) == 0 ){
                glm::vec3 t,r,s;
                fscanf(file2, "%f %f %f %f %f %f %f %f %f\n", &t.x, &t.y, &t.z, &r.x, &r.y, &r.z, &s.x, &s.y, &s.z);
                vec_keyframe[i].push_back({t,r,s});
            }
            
        }
        fclose(file2);
        
    }
    
}

void SkinningAux::playSkin(const int &animation, const int &frame)
{
    glm::vec3 res_t = vec_keyframe[animation][frame][1];  // *** I don't know why but T and R was reversed... ***
    glm::vec3 res_r = vec_keyframe[animation][frame][0];  // *** I don't know why but T and R was reversed... ***
    glm::mat4 RotateMatrix = eulerAngleYXZ(res_r.y, res_r.x, res_r.z);
    glm::mat4 TransitionMatrix = translate(glm::mat4(1.0f), res_t);
    animationmatrix = TransitionMatrix * RotateMatrix;
}

glm::mat4 SkinningAux::RecieveAnimationMartix()
{
    return animationmatrix;
}
