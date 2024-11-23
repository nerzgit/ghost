//
//  dustdecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/18.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef dustdecorator_hpp
#define dustdecorator_hpp

#include "deco/mapdecorator.hpp"
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

class Dust : public MapDecorator
{
private:
    std::vector<float>          _motion;
    int                         _frame;
    float                       _scale;
    glm::vec3                   _position;
    glm::vec3                   _orientation;
    glm::vec3                   _randomizer;
    
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
    
    void _ProcessInitializeObject(const char* obj_path, const char* tex_path);
    void _ProcessRenderObject();
    
public:
    Dust(const char* obj_path,
         const char* tex_path);
    Dust(const char* tex_path);
    ~Dust();
    Dust* Clone(const char* tex_path);
    void Render();
    void UpdateFrame(const int &value);
    void UpdateRandomizer(const glm::vec3 &randomizer);
    void UpdatePosition(const glm::vec3 &position);
    void UpdateOrientation(const glm::vec3 &orientation);
};

class DustDecorator : public MapDecorator
{
private:
    std::vector<Dust*> _dust_arr;
    int _renderobject;
    bool _updatedust;
    float _dustheight;
    glm::vec3 _randomizer;
    
public:
    DustDecorator();
    ~DustDecorator();
    void ProcessDust();
    void UpdateDustRandomizer(const glm::vec3 &randomizer);
};

#endif /* dustdecorator_hpp */
