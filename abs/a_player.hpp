//
//  a_player.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef a_player_hpp
#define a_player_hpp

#include "dta/player.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

class  Bone;
extern GLFWwindow*  g_WINDOW;
extern glm::mat4    g_PROJECTIONMATRIX;
extern glm::mat4    g_VIEWMATRIX;
extern int          g_SCREENWIDTH;
extern int          g_SCREENHEIGHT;

class A_Player
{
private:
    glm::mat4                    _modelmatrix;
    glm::mat4                    _sysnthesismatrix;
    glm::mat4                    _skinningmatrix;
    glm::mat4                    _constraintmatrix;
    glm::vec3                    _lightposition;
    glm::vec3                    _lightupposition;
    glm::vec3                    _muzzleflashposition;
    glm::mat4                    _mvp;
    GLuint                       _usingtexture;
    int                          _bonesize;
    
    GLuint                       _vao;
    GLuint                       _vbos[6];
    GLuint                       _programID;
    GLuint                       _mvpID;
    GLuint                       _systhesismatrixID;
    GLuint                       _viewmatrixID;
    GLuint                       _modelmatrixID;
    GLuint                       _weightvbos[11];
    GLuint                       _bonematrixID;
    GLuint                       _texture[4];
    GLuint                       _textureID;
    GLuint                       _depthprogramID;
    GLuint                       _depthmatrixID;
    GLuint                       _depthbiasID;
    GLuint                       _shadowmapID;
    GLuint                       _depthtexture;
    GLuint                       _lightID;
    GLuint                       _muzzleflashID;
    GLuint                       _modelview3x3matrixID;
    GLuint                       _normaltextureID;
    GLuint                       _speculartextureID;
    
    std::vector<glm::vec3>            _vertices;
    std::vector<glm::vec2>            _uvs;
    std::vector<glm::vec3>            _normals;
    std::vector<glm::vec3>            _tangents;
    std::vector<glm::vec3>            _bitangents;
    std::vector<unsigned short>       _indices;
    std::vector<glm::vec3>            _indexed_vertices;
    std::vector<glm::vec2>            _indexed_uvs;
    std::vector<glm::vec3>            _indexed_normals;
    std::vector<glm::vec3>            _indexed_tangents;
    std::vector<glm::vec3>            _indexed_bitangents;
    std::vector<std::vector<float>>   _indexed_weights;

public:
    A_Player(PlayerStructObject   &obj_file,
             PlayerStructTexture  &tex_file,
             PlayerStructShader   &shd_file);
    ~A_Player();
    glm::mat4 bonematrix[11];
    void Render();
    void InitializeWeightVBO();
    void UpdateBonesSize(int size);
    void UpdateModelMatrix(const glm::mat4 &matrix);
    void UpdateLightPosition(const glm::vec3 &position);
    void UpdateLightUpPosition(const glm::vec3 &position);
    void UpdateFlashPosition(const glm::vec3 &position);
    void UpdateUsingTexture(const GLuint &number);
    void UpdateIndexedWeights(std::vector<std::vector<float>> indexed_weights);
    void RecieveIndexedVertices(std::vector<glm::vec3> &indexed_vertices);
    glm::mat4 RecieveMVPMatrix();
};

#endif /* a_player_hpp */
