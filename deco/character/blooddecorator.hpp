//
//  blooddecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef blooddecorator_hpp
#define blooddecorator_hpp

#include "deco/characterdecorator.hpp"
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

class Clock;
extern Clock* g_CLOCK;
extern glm::mat4 g_PROJECTIONMATRIX;
extern glm::mat4 g_VIEWMATRIX;

class Blood : public CharacterDecorator
{
private:
    std::vector<float> _motion;
    int       _frame;
    glm::vec3 _position;
    float     _scale;
    float     _horizontalangle;
    
    GLuint                      _vao;
    GLuint                      _vbos[4];
    GLuint                      _programID;
    GLuint                      _texture;
    GLuint                      _textureID;
    GLuint                      _mvpID;
    GLuint                      _uvaID;
    float                       _uvalpha;
    
    std::vector<glm::vec3>      _vertices;
    std::vector<glm::vec2>      _uvs;
    std::vector<glm::vec3>      _normals;
    std::vector<unsigned short> _indices;
    std::vector<glm::vec3>      _indexed_vertices;
    std::vector<glm::vec2>      _indexed_uvs;
    std::vector<glm::vec3>      _indexed_normals;
    
    glm::mat4                   _mvp;
    
    void _ProcessInitializeObject(const char* obj_path,const char* tex_path);
    void _ProcessRenderObject();
    
public:
    Blood(const char* obj_path,const char* tex_path);
    Blood(const char* tex_path);
    ~Blood();
    Blood* Clone(const char* tex_path);
    void Render();
    void UpdatePosition(const glm::vec3 &position);
    void UpdateHorizontalAngle(const float &angle);
    void UpdateFrame(const int &value);
};

class BloodDecorator : public CharacterDecorator
{
private:
    std::vector<Blood*> _blood_arr;
    float _bloodheight;
    int _renderobject;
    
public:
    BloodDecorator();
    ~BloodDecorator();
    void ProcessBlood();
};


#endif /* blooddecorator_hpp */
