//
//  ballisticdecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/12.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef ballisticdecorator_hpp
#define ballisticdecorator_hpp

#include "deco/weapondecorator.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern GLFWwindow*  g_WINDOW;
extern glm::mat4    g_PROJECTIONMATRIX;
extern glm::mat4    g_VIEWMATRIX;

class BallisticDecorator : public WeaponDecorator
{
private:
    
    bool _firstrender;
    
    int _updatecounter;
    
    GLuint                      _vao;
    GLuint                      _vbos[4];
    GLuint                      _programID;
    GLuint                      _texture;
    GLuint                      _textureID;
    GLuint                      _mvpID;
    GLuint                      _synthesismatrixID;
    
    std::vector<glm::vec3>      _vertices;
    std::vector<glm::vec2>      _uvs;
    std::vector<glm::vec3>      _normals;
    std::vector<unsigned short> _indices;
    std::vector<glm::vec3>      _indexed_vertices;
    std::vector<glm::vec2>      _indexed_uvs;
    std::vector<glm::vec3>      _indexed_normals;
    
    glm::mat4                   _mvp;
    glm::mat4                   _modelmatrix;
    glm::mat4                   _synthesismatrix;
    glm::mat4                   _valancematrix;
    
    void _ProcessInitializeObject(const char* obj_path, const char* tex_path);
    void _ProcessRenderObject();
    
public:
    BallisticDecorator(const char* obj_path, const char* tex_path);
    ~BallisticDecorator();
    void ProcessBallistic();
    void UpdateBallisticModelMatrix(glm::mat4 &matrix);
    void UpdateBallisticCounterToZero();
};

#endif /* ballisticdecorator_hpp */
