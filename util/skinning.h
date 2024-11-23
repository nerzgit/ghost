//
//  skinning.h
//  play
//
//  Created by IDE KENTARO on 2019/04/03.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef skinning_h
#define skinning_h

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

class Bone
{
private:
    GLuint     boneVAO;
    GLuint     boneProgramID;
    GLuint     boneMatrixID;
    glm::mat4  boneMVP;
    GLuint     boneVBO;
    GLuint     boneCBO;
    
    void renderBone();
    
public:
    int                screenWidth;
    int                screenHeight;
    
    int                id;
    Bone               *boneCH = 0;
    Bone               *boneSL = 0;
    glm::vec3          T,R,S;
    glm::mat4          matOFFSET;
    glm::mat4          matINIT;
    glm::mat4          matBONE;
    glm::mat4          matLOCAL;
    std::vector<float> arrWeight;
    
    ~Bone();
    // Bone initialization
    void initBone(const int &id, const glm::vec3 &T, const glm::vec3 &R, const glm::vec3 &S, const glm::vec3 &tail);
    // Also you can use playBoneMVP
    void playBoneVEC3(const glm::vec3 &T, const glm::vec3 &R, const glm::vec3 &S, const glm::mat4 &ProjectionMatrix, const glm::mat4 &ViewMatrix);
    // Also you can use playBoneVEC3
    void playBoneMVP(const glm::mat4 &MVP);
};

class Skinning
{
private:
    int        fps;
    int        frame;
    // Keyframe [Animation type][Bone][vec3]
    std::vector<std::vector<std::vector<glm::vec3>>> bones_keyframe;
    
    void convertToRelativeMatrix(Bone *me, glm::mat4 &matParentOFFSET);
    void updateBone(Bone *me, glm::mat4 &matParentWORLD);
    
public:
    std::vector<Bone*> bones;
    
    Skinning();
    ~Skinning();
    
    // Rendering skin mesh animation
    void renderSkin(const int &vbos0, const int &vbos1);
    // Also you can use playSkinMVP
    void playSkinVEC3(const glm::vec3 &T, const glm::vec3 &R, const glm::vec3 &S, glm::mat4 *out_boneMatrices);
    // Also you can use playSkinVEC3
    void playSkinMVP(const glm::mat4 &MVP, glm::mat4 *out_boneMatrices);
    // Read and compile weight file
    void initWeight(const char *weightfile);
    // Read and compile keyframe file
    void initKeyframe(std::vector<const char*> keyframefile);
    // Sorting weight by in_vertices order
    void sortWeight(const char                      * object,
                    std::vector<glm::vec3>          & in_vertices,
                    std::vector<std::vector<float>> & out_weights);
    // For constraint another objs...
    glm::mat4 getWorldBoneMatrix(const int &bone);
    glm::mat4 getLocalBoneMatrix(const int &bone);
    int RecieveBonesSize();

};

#endif /* skinning_h */
