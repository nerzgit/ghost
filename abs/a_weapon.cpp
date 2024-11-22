//
//  a_weapon.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "a_weapon.hpp"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/tangentspace.hpp"
#include "usge/quaternion_utils.hpp"

A_Weapon::A_Weapon(PlayerStructObject   &obj_file,
                   PlayerStructTexture  &tex_file,
                   PlayerStructShader   &shd_file)
{

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    // Set texture to vector for loop
    std::vector<const char *> out_texture = {
        tex_file.base,
        tex_file.flash,
        tex_file.normal,
        tex_file.specular
    };
    
    // Read textures.
    for (int i=0; i<out_texture.size(); i++) {
        if      (strstr(out_texture[i], ".dds")) _texture[i] = loadDDS(out_texture[i]);
        else if (strstr(out_texture[i], ".bmp")) _texture[i] = loadBMP_custom(out_texture[i]);
        else    printf("This is not .dds .bmp x_x\n");
    }
    
    // Read OBJ
    loadOBJ(obj_file.obj, _vertices, _uvs, _normals);
    
    computeTangentBasis(_vertices, _uvs, _normals, _tangents, _bitangents);
    indexVBO_TBN(_vertices, _uvs, _normals, _tangents, _bitangents, _indices, _indexed_vertices, _indexed_uvs, _indexed_normals, _indexed_tangents, _indexed_bitangents);
    
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
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[3]);
    glBufferData(GL_ARRAY_BUFFER, _indexed_tangents.size() * sizeof(glm::vec3), &_indexed_tangents[0], GL_STATIC_DRAW);
    glGenBuffers(1, &_vbos[4]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[4]);
    glBufferData(GL_ARRAY_BUFFER, _indexed_bitangents.size() * sizeof(glm::vec3), &_indexed_bitangents[0], GL_STATIC_DRAW);
    glGenBuffers(1, &_vbos[5]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), &_indices[0], GL_STATIC_DRAW);
    
    
    _programID            = LoadShaders( shd_file.vs, shd_file.fs );
    _textureID            = glGetUniformLocation(_programID, "myTextureSampler");
    _mvpID                = glGetUniformLocation(_programID, "MVP");
    _viewmatrixID         = glGetUniformLocation(_programID, "V");
    _modelmatrixID        = glGetUniformLocation(_programID, "M");
    _depthbiasID          = glGetUniformLocation(_programID, "DepthBiasMVP");
    _shadowmapID          = glGetUniformLocation(_programID, "shadowMap");
    _lightID              = glGetUniformLocation(_programID, "LightInvDirection_worldspace");
    _systhesismatrixID    = glGetUniformLocation(_programID, "SynthesisM");
    _muzzleflashID        = glGetUniformLocation(_programID, "FlashPosition_worldspace");
    _modelview3x3matrixID = glGetUniformLocation(_programID, "MV3x3");
    _normaltextureID      = glGetUniformLocation(_programID, "NormalTextureSampler");
    _speculartextureID    = glGetUniformLocation(_programID, "SpecularTextureSampler");
    
    glGenTextures(1, &_depthtexture);
    glBindTexture(GL_TEXTURE_2D, _depthtexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, g_SCREENWIDTH, g_SCREENHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    
}

A_Weapon::~A_Weapon()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbos[0]);
    glDeleteBuffers(1, &_vbos[1]);
    glDeleteBuffers(1, &_vbos[2]);
    glDeleteBuffers(1, &_vbos[3]);
    glDeleteBuffers(1, &_vbos[4]);
    glDeleteBuffers(1, &_vbos[5]);
    glDeleteProgram(_programID);
    glDeleteTextures(1, &_texture[0]);
    glDeleteTextures(1, &_texture[1]);
    glDeleteTextures(1, &_texture[2]);
    glDeleteTextures(1, &_texture[3]);
    glDeleteTextures(1, &_depthtexture);
}

void A_Weapon::Render()
{

    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
    glm::mat4 depthViewMatrix = glm::lookAt(_lightposition, _lightupposition, glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
    
    glUseProgram(_programID);
    
    _sysnthesismatrix = _constraintmatrix * _skinningmatrix;
    
    glm::mat4 biasMatrix{
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    };
    glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
    
    glm::mat4 MVP = g_PROJECTIONMATRIX * g_VIEWMATRIX * _modelmatrix;
    
    // MVP行列
    glUniformMatrix4fv(_mvpID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(_modelmatrixID, 1, GL_FALSE, &_modelmatrix[0][0]);
    glUniformMatrix4fv(_viewmatrixID, 1, GL_FALSE, &g_VIEWMATRIX[0][0]);
    glUniformMatrix4fv(_depthbiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
    glUniformMatrix4fv(_systhesismatrixID, 1, GL_FALSE, &_sysnthesismatrix[0][0]);
    glUniformMatrix3fv(_modelview3x3matrixID, 1, GL_FALSE, &glm::mat3(g_VIEWMATRIX * _modelmatrix)[0][0]);
    glUniform3f(_muzzleflashID, _muzzleflashposition.x, _muzzleflashposition.y, _muzzleflashposition.z);
    glUniform3f(_lightID, _lightposition.y, _lightposition.y, _lightposition.z);
    
    // Basic texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture[_usingtexture]);
    glUniform1i(_textureID, 0);
    
    // Depth texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _depthtexture);
    glUniform1i(_shadowmapID, 1);
    
    // Normal texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texture[2]);
    glUniform1i(_normaltextureID, 2);
    
    // Specular texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _texture[3]);
    glUniform1i(_speculartextureID, 3);
    
    
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
    
    // 4th attribute buffer : tangents
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[3]);
    glVertexAttribPointer(
                          3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0
                          );
    
    // 5th attribute buffer : bitangents
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[4]);
    glVertexAttribPointer(
                          4,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0
                          );
    
    // VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[5]);
    
    
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
    glDisableVertexAttribArray(4);
    
}

void A_Weapon::UpdateSkinningMatrix(const glm::mat4 &matrix) {
    _skinningmatrix = matrix;
}
void A_Weapon::UpdateConstraintMatrix(const glm::mat4 &matrix) {
    _constraintmatrix = matrix;
}
void A_Weapon::UpdateModelMatrix(const glm::mat4 &matrix){
    _modelmatrix = matrix;
}
void A_Weapon::UpdateLightPosition(const glm::vec3 &position){
    _lightposition = position;
}
void A_Weapon::UpdateLightUpPosition(const glm::vec3 &position){
    _lightupposition = position;
}
void A_Weapon::UpdateFlashPosition(const glm::vec3 &position){
    _muzzleflashposition = position;
}
void A_Weapon::UpdateUsingTexture(const GLuint &number){
    _usingtexture = number;
}
glm::mat4 A_Weapon::RecieveConstraintMatrix(){
    return _sysnthesismatrix;
}
glm::mat4 A_Weapon::RecieveModelMatrix(){
    return _modelmatrix;
}
