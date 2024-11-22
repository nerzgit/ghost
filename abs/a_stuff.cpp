//
//  a_stuff.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/03.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "a_stuff.hpp"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/quaternion_utils.hpp"
#include "usge/key.hpp"


A_Stuff::A_Stuff(
                 const char *tex_path,
                 const char *obj_path,
                 const char *obb_path,
                 const char *vsh_path,
                 const char *fsh_path
                 )
{
    
    _angle               = glm::vec3(0); // Model angle
    _scale               = glm::vec3(1); // Model scale
    _postion             = glm::vec3(0); // Model position
    _lightposition       = glm::vec3(0); // Light position
    _muzzleflashposition = glm::vec3(0); // Muzzleflash model position
    _randomizer          = glm::vec3(0); // For reallistic!!!
    _bh_action           = false;
    _bh_number           = 0;
    
    
    // Initialize VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    // Compile shader to GLSL
    _programID     = LoadShaders( vsh_path, fsh_path );
    _mvpID         = glGetUniformLocation(_programID, "MVP");
    _viewmatrixID  = glGetUniformLocation(_programID, "V");
    _modelmatrixID = glGetUniformLocation(_programID, "M");
    _lightID       = glGetUniformLocation(_programID, "LightPosition_worldspace");
    _muzzleflashID = glGetUniformLocation(_programID, "FlashPosition_worldspace");
    _texture       = loadDDS(tex_path);
    _textureID     = glGetUniformLocation(_programID, "myTextureSampler");
    
    // Loading objfile and delete same vertex
    loadOBJ(obj_path, _vertices, _uvs, _normals);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), &_indices[0], GL_STATIC_DRAW);
    
    // Loading OBB vertices
    if (!loadOBJVertex(obb_path, _obbvertices)){
        if (loadOBJVertex("game/e1m1/oj/map/obj/__none__.obb", _obbvertices))
            printf(" -> Loading __none__.obb instead of %s\n", obb_path);
    }
    
    // Initialization Bullethole texture
    // 10 maximum bullet hole
    for (int i=0; i<10; i++) {
        
        // Bullet hole texture ID
        std::string sampler_name = "bulletHoleTextureSampler" + std::to_string(i);
        _bh_arrID.push_back(glGetUniformLocation( _programID, sampler_name.c_str() ) );
        
        // Bullet hole matrices for projection mapping
        std::string mvp_name = "bulletHoleMVP" + std::to_string(i);
        _bh_arrmvpID.push_back(glGetUniformLocation( _programID, mvp_name.c_str() ) );
        _bh_arrmvp.push_back(glm::mat4(0));
        
    }
    
}

A_Stuff::A_Stuff()
{}

A_Stuff::~A_Stuff()
{
    glDeleteBuffers(1, &_vbos[0]);
    glDeleteBuffers(1, &_vbos[1]);
    glDeleteBuffers(1, &_vbos[2]);
    glDeleteBuffers(1, &_vbos[3]);
    glDeleteProgram(_programID);
    glDeleteTextures(1, &_texture);
    glDeleteVertexArrays(1, &_vao);
}

void A_Stuff::Clone(A_Stuff* clone)
{
    // Always clone from Derivative class
    clone->_vertices         = this->_vertices;
    clone->_uvs              = this->_uvs;
    clone->_normals          = this->_normals;
    clone->_indices          = this->_indices;
    clone->_indexed_vertices = this->_indexed_vertices;
    clone->_indexed_uvs      = this->_indexed_uvs;
    clone->_indexed_normals  = this->_indexed_normals;
    clone->_programID        = this->_programID;
    clone->_mvpID            = this->_mvpID;
    clone->_viewmatrixID     = this->_viewmatrixID;
    clone->_modelmatrixID    = this->_modelmatrixID;
    clone->_lightID          = this->_lightID;
    clone->_muzzleflashID    = this->_muzzleflashID;
    clone->_texture          = this->_texture;
    clone->_textureID        = this->_textureID;
    clone->_bh_action        = this->_bh_action;
    clone->_bh_image         = this->_bh_image;
    clone->_bh_number        = this->_bh_number;
    clone->_obbvertices      = this->_obbvertices;
    
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
    
    // Initialization Bullethole texture
    // 10 maximum bullet hole
    for (int i=0; i<10; i++) {
        
        // Bullet hole texture ID
        std::string sampler_name = "bulletHoleTextureSampler" + std::to_string(i);
        clone->_bh_arrID.push_back(glGetUniformLocation( clone->_programID, sampler_name.c_str() ) );
        
        // Bullet hole matrices for projection mapping
        std::string mvp_name = "bulletHoleMVP" + std::to_string(i);
        clone->_bh_arrmvpID.push_back(glGetUniformLocation( clone->_programID, mvp_name.c_str() ) );
        clone->_bh_arrmvp.push_back(glm::mat4(0));
        
    }
    
}

void A_Stuff::Render()
{
    
    // Matrix
    _rotationmatrix    = glm::eulerAngleYXZ(_angle.y, _angle.x, _angle.z);
    _translationmatrix = translate(glm::mat4(1.0), _postion);
    _scalingmatrix     = scale(glm::mat4(1.0), _scale);
    _modelmatrix       = _translationmatrix * _rotationmatrix * _scalingmatrix;
    _mvp               = g_PROJECTIONMATRIX * g_VIEWMATRIX * _modelmatrix;

    glUseProgram(_programID);
    
    glUniformMatrix4fv(_mvpID, 1, GL_FALSE, &_mvp[0][0]);
    glUniformMatrix4fv(_modelmatrixID, 1, GL_FALSE, &_modelmatrix[0][0]);
    glUniformMatrix4fv(_viewmatrixID, 1, GL_FALSE, &g_VIEWMATRIX[0][0]);
    
    // Light
    glUniform3f(_lightID, _lightposition.x, _lightposition.y, _lightposition.z);
    
    // Muzzleflash
    glUniform3f(_muzzleflashID, _muzzleflashposition.x, _muzzleflashposition.y, _muzzleflashposition.z);
    
    // Base texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(_textureID, 0);
    
    // Bullet texture unit from GL_TEXTURE1 not zero.
    for (int i=0; i<10; i++) {
        glUniformMatrix4fv(_bh_arrmvpID[i],1,GL_FALSE,&_bh_arrmvp[i][0][0]);
        switch (i){
            case 0: glActiveTexture(GL_TEXTURE1);  break;
            case 1: glActiveTexture(GL_TEXTURE2);  break;
            case 2: glActiveTexture(GL_TEXTURE3);  break;
            case 3: glActiveTexture(GL_TEXTURE4);  break;
            case 4: glActiveTexture(GL_TEXTURE5);  break;
            case 5: glActiveTexture(GL_TEXTURE6);  break;
            case 6: glActiveTexture(GL_TEXTURE7);  break;
            case 7: glActiveTexture(GL_TEXTURE8);  break;
            case 8: glActiveTexture(GL_TEXTURE9);  break;
            case 9: glActiveTexture(GL_TEXTURE10); break;
        }
        glBindTexture(GL_TEXTURE_2D, _bh_image);
        glUniform1i(_bh_arrID[i], (GLint)i+1);
    }
    
    // VERTEX
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
    
    // NORMAL
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
    
    glEnable(GL_BLEND);
    
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
    
    glDisable(GL_BLEND);
    
}

void A_Stuff::RenderBulletHole()
{
    
    if (_bh_action) {
        
        // Stop handler
        _bh_action = false;
        
        // Below matrices for Bullethole
        // FOV is 45d, Aspect ratio need a little retouch. Each Clipping plane should be so far.
        glm::mat4 bulletHoleProjectionMatrix = glm::perspective<float>(glm::radians(45.0f), 0.8f, 0.1f, 1000.0f);
        // MVP: (Model matrices should be same as object matrices.)
        glm::mat4 atranslationmatrix = translate(glm::mat4(1.0), _randomizer * -1.0f);
        _bh_mvp = bulletHoleProjectionMatrix * atranslationmatrix * g_VIEWMATRIX * _modelmatrix;
        
        // Set now mvp.
        _bh_arrmvp[_bh_number] = _bh_mvp;
        
        // Switch to next MVP.
        _bh_number++;
        
        // Maximum, turn to 0
        if (_bh_number > 9) _bh_number = 0;
        
    }

}





void A_Stuff::UpdateAngle(const glm::vec3 &angle){
    this->_angle = angle;
}
void A_Stuff::UpdateScale(const glm::vec3 &scale){
    this->_scale = scale;
}
void A_Stuff::UpdatePosition(const glm::vec3 &position){
    this->_postion = position;
}
void A_Stuff::UpdateLightPosition(const glm::vec3 &position){
    this->_lightposition = position;
}
void A_Stuff::UpdateFlashPosition(const glm::vec3 &position){
    this->_muzzleflashposition = position;
}
void A_Stuff::UpdateBulletHoleTexture(GLuint &texture){
    this->_bh_image = texture;
}
void A_Stuff::UpdateTexture(GLuint &texture){
    this->_texture = texture;
}
void A_Stuff::UpdateRandomizer(const glm::vec3 &randomizer){
    this->_randomizer = randomizer;
}
void A_Stuff::UpdateBulletHoleAction(const bool &value){
    this->_bh_action = value;
}
void A_Stuff::UpdateAllBulletHoleMVP(const glm::mat4 &matrix){
    for (int i=0; i<10; i++) {
        _bh_arrmvp[i] = matrix;
        glUniformMatrix4fv(_bh_arrmvpID[i],1,GL_FALSE,&_bh_arrmvp[i][0][0]);
        switch (i){
            case 0: glActiveTexture(GL_TEXTURE1);  break;
            case 1: glActiveTexture(GL_TEXTURE2);  break;
            case 2: glActiveTexture(GL_TEXTURE3);  break;
            case 3: glActiveTexture(GL_TEXTURE4);  break;
            case 4: glActiveTexture(GL_TEXTURE5);  break;
            case 5: glActiveTexture(GL_TEXTURE6);  break;
            case 6: glActiveTexture(GL_TEXTURE7);  break;
            case 7: glActiveTexture(GL_TEXTURE8);  break;
            case 8: glActiveTexture(GL_TEXTURE9);  break;
            case 9: glActiveTexture(GL_TEXTURE10); break;
        }
        glBindTexture(GL_TEXTURE_2D, _bh_image);
        glUniform1i(_bh_arrID[i], (GLint)i+1);
    }
}
void A_Stuff::RecieveOBBVertices(std::vector<glm::vec3> &out_obbvertices){
    out_obbvertices = _obbvertices;
}
void A_Stuff::RecieveModelMatrix(glm::mat4 &matrix){
    matrix = _modelmatrix;
}

