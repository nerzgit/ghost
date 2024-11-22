//
//  blooddecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "blooddecorator.hpp"
#include "usge/clock.hpp"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/quaternion_utils.hpp"






BloodDecorator::BloodDecorator()
{
    
    // Which object rendering? default to 0
    _renderobject = 0;
    
    // Blood coord axis y
    _bloodheight = 1.3f;
    
    // Rendering textures
    std::vector<const char*> texture_arr = {
        "game/game/oj/effect/tex/zombie/000.dds",
        "game/game/oj/effect/tex/zombie/001.dds",
        "game/game/oj/effect/tex/zombie/002.dds",
        "game/game/oj/effect/tex/zombie/003.dds",
        "game/game/oj/effect/tex/zombie/004.dds",
    };
    
    // Initialize object
    for (int i=0; i<texture_arr.size(); i++) {
        if (i==0) {
            _blood_arr.push_back( new Blood("game/game/oj/effect/obj/dust.obj",texture_arr[i]) );
        } else{
            _blood_arr.push_back( _blood_arr[0]->Clone(texture_arr[i]) );
        }
    }
    
}

BloodDecorator::~BloodDecorator()
{
    for (auto e : _blood_arr) delete e;
}






void BloodDecorator::ProcessBlood()
{

    // Calculate dust position (Spherical coordinates :: OpenGL version)
    glm::vec3 playerposition = CharacterDecorator::_o_character->RecieveTargetPosition();
    glm::vec3 orientation = CharacterDecorator::_o_character->RecieveTargetOrientation();
    _bloodheight = 1.3f + CharacterDecorator::_o_character->RecievePosition().y;
    float r = CharacterDecorator::_o_character->RecieveTargetDistance();
    float v = orientation.x;
    float h = orientation.y;
    float d = 0;
    float x = (r-d) * cos(v) * sin(h);
    float y = (r-d) * sin(v) + _bloodheight;
    float z = (r-d) * cos(v) * cos(h);
    glm::vec3 decoratorposition = glm::vec3(x,y,z) + playerposition;
    
    // Hit!! Reset frame and change rendering object
    bool hit = CharacterDecorator::_o_character->RecieveHit();
    bool targetfire = CharacterDecorator::_o_character->RecieveTargetFire();
    bool dead = CharacterDecorator::_o_character->RecieveDead();

    if (hit) {
        
        // If target fire
        if (targetfire) {
            
            // Increase _renderobject.Select show object.
            _renderobject++;
            if (_renderobject > 4) {
                _renderobject = 0;
            }
            
            // Frame set to zero
            _blood_arr[_renderobject]->UpdateFrame(0);
            
            // Update position
            _blood_arr[_renderobject]->UpdatePosition(decoratorposition);
            
            // Update Angle
            _blood_arr[_renderobject]->UpdateHorizontalAngle(h);
            
        }
        
    }else{
        
        // Update position (Going so far)
        _blood_arr[_renderobject]->UpdatePosition(glm::vec3(9999,9999,9999));
        
    }
    
    if (!dead) {
        
        // Render
        _blood_arr[_renderobject]->Render();
        
    }
    
    
}







Blood::Blood(const char* obj_path,
             const char* tex_path)
{
    
    // Default frame set to 9
    _frame = 9;
    
    // Initialization
    _ProcessInitializeObject(obj_path,tex_path);
    
}

Blood::Blood(const char* tex_path)
{
    
    /* For copy method */
    
    // New texture
    _texture = loadDDS(tex_path);
    
}

Blood::~Blood()
{
    
}







void Blood::Render()
{
    
    // Rendering
    _ProcessRenderObject();
    
    // Increase frame
    if ( g_CLOCK -> RecieveFPS() ){
        _frame++;
    }
    
    // Set to 9, If reached 9
    if (_frame > 8) {
        _frame = 9;
    }
    
}

Blood* Blood::Clone(const char* tex_path)
{
    
    Blood* clone = new Blood(tex_path);
    
    clone->_frame            = this->_frame;
    clone->_motion           = this->_motion;
    clone->_programID        = this->_programID;
    clone->_textureID        = this->_textureID;
    clone->_mvpID            = this->_mvpID;
    clone->_uvaID            = this->_uvaID;
    clone->_uvalpha          = this->_uvalpha;
    clone->_vertices         = this->_vertices;
    clone->_uvs              = this->_uvs;
    clone->_normals          = this->_normals;
    clone->_indices          = this->_indices;
    clone->_indexed_vertices = this->_indexed_vertices;
    clone->_indexed_uvs      = this->_indexed_uvs;
    clone->_indexed_normals  = this->_indexed_normals;
    
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
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
    
    return clone;
    
}





void Blood::_ProcessInitializeObject(const char* obj_path,
                                     const char* tex_path)
{
    
    
    _motion = {
        // Scale, Alpha
        0.20, 1.00,
        0.22, 1.00,
        0.24, 1.00,
        0.26, 1.00,
        0.28, 1.00,
        0.30, 1.00,
        0.32, 1.00,
        0.34, 0.80,
        0.36, 0.60,
        0.40, 0.00,
    };
    
    _position             = glm::vec3(0); // Model position
    
    // Initialize VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    // Compile shader to GLSL
    _programID         = LoadShaders( "game/game/sd/Dust.vertexshader", "game/game/sd/Dust.fragmentshader"  );
    _texture           = loadDDS(tex_path);
    _textureID         = glGetUniformLocation(_programID, "myTextureSampler");
    _mvpID             = glGetUniformLocation(_programID, "MVP");
    _uvaID             = glGetUniformLocation(_programID, "UVa");
    
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
    
}





void Blood::_ProcessRenderObject()
{
    
    _scale = _motion[_frame*2];
    _uvalpha = _motion[_frame*2+1];
    
    // ModelMatrix
    glm::mat4 rotationmatrix    = glm::eulerAngleYXZ(_horizontalangle, 0.0f, 0.0f);
    glm::mat4 translationmatrix = translate(glm::mat4(1.0), _position);
    glm::mat4 scalingmatrix     = scale(glm::mat4(1.0), glm::vec3(1) * _scale);
    glm::mat4 modelmatrix       = translationmatrix * rotationmatrix * scalingmatrix;
    _mvp = g_PROJECTIONMATRIX * g_VIEWMATRIX * modelmatrix;
    
    glUseProgram(_programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUniformMatrix4fv(_mvpID, 1, GL_FALSE, &_mvp[0][0]);
    glUniform1f(_uvaID, _uvalpha);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(_textureID, 0);
    
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





void Blood::UpdatePosition(const glm::vec3 &position){
    _position = position;
}
void Blood::UpdateHorizontalAngle(const float &angle){
    _horizontalangle = angle;
}
void Blood::UpdateFrame(const int &value){
    _frame = value;
}
