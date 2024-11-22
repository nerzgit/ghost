//
//  a_character.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "a_character.hpp"
#include "util/skinning.h"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/tangentspace.hpp"
#include "usge/quaternion_utils.hpp"

A_Character::A_Character(CharacterStructObject   &obj_file,
                         CharacterStructTexture  &tex_file,
                         CharacterStructShader   &shd_file)
{
    
    _usingtexture = 0;
    _lightposition = glm::vec3(0);
    _lightupposition = glm::vec3(0);
    _muzzleflashposition = glm::vec3(0);
    
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    // Set texture to vector for loop
    std::vector<const char *> out_texture = {
        tex_file.base
    };
    
    // Read textures.
    for (int i=0; i<out_texture.size(); i++) {
        if      (strstr(out_texture[i], ".dds")) _texture[i] = loadDDS(out_texture[i]);
        else if (strstr(out_texture[i], ".bmp")) _texture[i] = loadBMP_custom(out_texture[i]);
        else    printf("This is not .dds .bmp x_x(DEAD! Not big surprise.)\n");
    }
    
    // Read OBJ
    loadOBJ(obj_file.obj, _vertices, _uvs, _normals);
    indexVBO(_vertices, _uvs, _normals, _indices, _indexed_vertices, _indexed_uvs, _indexed_normals);
    
    glGenBuffers(1, &_vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, _indexed_vertices.size() * sizeof(glm::vec3), &_indexed_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &_vbos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, _indexed_uvs.size() * sizeof(glm::vec2), &_indexed_uvs[0], GL_STATIC_DRAW);
    glGenBuffers(1, &_vbos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, _indexed_normals.size() * sizeof(glm::vec3), &_indexed_normals[0], GL_STATIC_DRAW);
    glGenBuffers(1, &_vbos[3]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), &_indices[3], GL_STATIC_DRAW);
    
    _programID = LoadShaders( "game/game/sd/Zombie.vertexshader", "game/game/sd/Zombie.fragmentshader" );

    _textureID      = glGetUniformLocation(_programID, "myTextureSampler");
    _mvpID          = glGetUniformLocation(_programID, "MVP");
    _viewmatrixID   = glGetUniformLocation(_programID, "V");
    _modelmatrixID  = glGetUniformLocation(_programID, "M");
    _lightID        = glGetUniformLocation(_programID, "LightPosition_worldspace");
    _muzzleflashID  = glGetUniformLocation(_programID, "FlashPosition_worldspace");
    _bonematrixID   = glGetUniformLocation(_programID, "BoneMatrix");
    
    _obbvertices = {
        glm::vec3(-0.3,  0.0, -0.3),
        glm::vec3( 0.3,  2.0,  0.3)
    };
    
}

A_Character::A_Character()
{}

A_Character::~A_Character()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbos[0]);
    glDeleteBuffers(1, &_vbos[1]);
    glDeleteBuffers(1, &_vbos[2]);
    glDeleteBuffers(1, &_vbos[3]);
    glDeleteProgram(_programID);
    glDeleteTextures(1, &_texture[0]);
    glDeleteTextures(1, &_texture[1]);
    glDeleteTextures(1, &_texture[2]);
    glDeleteTextures(1, &_texture[3]);
}

void A_Character::Clone(A_Character* clone)
{

    clone->_bonesize         = this->_bonesize;
    clone->_programID        = this->_programID;
    clone->_mvpID            = this->_mvpID;
    clone->_viewmatrixID     = this->_viewmatrixID;
    clone->_modelmatrixID    = this->_modelmatrixID;
    clone->_bonematrixID     = this->_bonematrixID;
    clone->_texture[0]       = this->_texture[0];
    clone->_texture[1]       = this->_texture[1];
    clone->_texture[2]       = this->_texture[2];
    clone->_texture[3]       = this->_texture[3];
    clone->_textureID        = this->_textureID;
    clone->_lightID          = this->_lightID;
    clone->_muzzleflashID    = this->_muzzleflashID;
    clone->_vertices         = this->_vertices;
    clone->_uvs              = this->_uvs;
    clone->_normals          = this->_normals;
    clone->_indices          = this->_indices;
    clone->_indexed_vertices = this->_indexed_vertices;
    clone->_indexed_uvs      = this->_indexed_uvs;
    clone->_indexed_normals  = this->_indexed_normals;
    clone->_indexed_weights  = this->_indexed_weights;
    clone->_obbvertices      = this->_obbvertices;
    clone->_usingtexture     = this->_usingtexture;
    
    glGenVertexArrays(1, &clone->_vao);
    glBindVertexArray(clone->_vao);
    
    glGenBuffers(1, &clone->_vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, clone->_vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, clone->_indexed_vertices.size() * sizeof(glm::vec3), &clone->_indexed_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &clone->_vbos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, clone->_vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, clone->_indexed_uvs.size() * sizeof(glm::vec2), &clone->_indexed_uvs[0], GL_STATIC_DRAW);
    glGenBuffers(1, &clone->_vbos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, clone->_vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, clone->_indexed_normals.size() * sizeof(glm::vec3), &clone->_indexed_normals[0], GL_STATIC_DRAW);
    glGenBuffers(1, &clone->_vbos[3]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clone->_vbos[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, clone->_indices.size() * sizeof(unsigned short), &clone->_indices[0], GL_STATIC_DRAW);
    
    clone->A_Character::InitializeWeightVBO();
    
}

void A_Character::Render()
{
    
    glUseProgram(_programID);
    
    _mvp = g_PROJECTIONMATRIX * g_VIEWMATRIX * _modelmatrix;
    
    // MVP行列
    glUniformMatrix4fv(_mvpID, 1, GL_FALSE, &_mvp[0][0]);
    glUniformMatrix4fv(_modelmatrixID, 1, GL_FALSE, &_modelmatrix[0][0]);
    glUniformMatrix4fv(_viewmatrixID, 1, GL_FALSE, &g_VIEWMATRIX[0][0]);
    glUniform3f(_lightID, _lightposition.x, _lightposition.y, _lightposition.z);
    glUniform3f(_muzzleflashID, _muzzleflashposition.x, _muzzleflashposition.y, _muzzleflashposition.z);
    glUniformMatrix4fv(_bonematrixID, 12, GL_FALSE, &bonematrix[0][0][0]);
    
    // Basic texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture[_usingtexture]);
    glUniform1i(_textureID, 0);
    
    // 頂点
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glVertexAttribPointer(
                          0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0
                          );
    
    // UV
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[1]);
    glVertexAttribPointer(
                          1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0
                          );
    
    // 法線
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[2]);
    glVertexAttribPointer(
                          2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0
                          );
    // VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[3]);
    
    
    // ウェイト
    for (int i=0; i<_bonesize; i++) {
        glEnableVertexAttribArray(i+4);
        glBindBuffer(GL_ARRAY_BUFFER, _weightvbos[i]);
        glVertexAttribPointer( i+4, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    }
    
    
    glDrawElements(
                   GL_TRIANGLES,
                   (int)_indices.size(),
                   GL_UNSIGNED_SHORT,
                   (void*)0
                   );
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    for (int i=0; i<_bonesize; i++)
        glDisableVertexAttribArray(i+4);
    
}

void A_Character::InitializeWeightVBO()
{
    for (int i=0; i<_bonesize; i++) {
        glGenBuffers(1, &_weightvbos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, _weightvbos[i]);
        glBufferData(GL_ARRAY_BUFFER, _indexed_weights[i].size() * sizeof(float), &_indexed_weights[i][0], GL_STATIC_DRAW);
    }
}

void A_Character::UpdateBonesSize(int size){
    _bonesize = size;
}
void A_Character::UpdateModelMatrix(const glm::mat4 &matrix){
    _modelmatrix = matrix;
}
void A_Character::UpdateLightPosition(const glm::vec3 &position){
    _lightposition = position;
}
void A_Character::UpdateLightUpPosition(const glm::vec3 &position){
    _lightupposition = position;
}
void A_Character::UpdateFlashPosition(const glm::vec3 &position){
    _muzzleflashposition = position;
}
void A_Character::UpdateUsingTexture(const GLuint &number){
    _usingtexture = number;
}
void A_Character::UpdateIndexedWeights(std::vector<std::vector<float>> &indexed_weights){
    _indexed_weights = indexed_weights; // Copy
}
void A_Character::RecieveIndexedVertices(std::vector<glm::vec3> &indexed_vertices){
    indexed_vertices = _indexed_vertices;
}
glm::mat4 A_Character::RecieveMVPMatrix(){
    return _mvp;
}
void A_Character::RecieveOBBVertices(std::vector<glm::vec3> &out_obbvertices){
    out_obbvertices = _obbvertices;
}
void A_Character::RecieveModelMatrix(glm::mat4 &matrix){
    matrix = _modelmatrix;
}
