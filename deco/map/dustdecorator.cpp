//
//  dustdecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/18.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "dustdecorator.hpp"
#include "util/vecmath.hpp"
#include "util/squareroot.hpp"
#include "usge/clock.hpp"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/quaternion_utils.hpp"







DustDecorator::DustDecorator()
{
    
    _updatedust = false;
    // Dust number will show
    _renderobject = 0;
    // Dust coord axis y
    _dustheight = 1.3f;
    // Dust coord shake to be more realistic!!
    _randomizer = glm::vec3(0);
    
    // Rendering textures
    std::vector<const char*> texture_arr = {
        "game/game/oj/effect/tex/stuff/000.dds",
        "game/game/oj/effect/tex/stuff/001.dds",
        "game/game/oj/effect/tex/stuff/002.dds",
        "game/game/oj/effect/tex/stuff/003.dds",
        "game/game/oj/effect/tex/stuff/004.dds",
    };
    
    // Initialize object
    for (int i=0; i<texture_arr.size(); i++) {
        if (i==0) {
            _dust_arr.push_back( new Dust("game/game/oj/effect/obj/dust.obj",texture_arr[i]) );
        } else{
            _dust_arr.push_back( _dust_arr[0]->Clone(texture_arr[i]) );
        }
    }
    
}

DustDecorator::~DustDecorator()
{
    for (auto e : _dust_arr) delete e;
}





/* In Rendering */
void DustDecorator::ProcessDust()
{
    
    // If no stuff!!!
    if (MapDecorator::_o_stuff == nullptr) {
        return;
    }
    
    // Calculate dust position (Spherical coordinates :: OpenGL version)
    glm::vec3 playerposition = MapDecorator::_o_stuff->RecieveTargetPosition();
    glm::vec3 orientation = MapDecorator::_o_stuff->RecieveTargetOrientation();
    float r = MapDecorator::_o_stuff->RecieveTargetDistance();
    float v = orientation.x;
    float h = orientation.y;
    float d = 0.3;
    float x = (r-d) * cos(v) * sin(h);
    float y = (r-d) * sin(v) + _dustheight;
    float z = (r-d) * cos(v) * cos(h);
    glm::vec3 decoratorposition = glm::vec3(x,y,z) + playerposition;
    
    // Hit!! Reset frame and change rendering object
    bool hit = MapDecorator::_o_stuff->RecieveHit();
    bool targetfire = MapDecorator::_o_stuff->RecieveTargetFire();
    
    if (hit) {
        // If target fire
        if (targetfire) {
            
            if (!_updatedust) {
                _updatedust = true;
                // Choose which dust will show
                _renderobject++;
                if (_renderobject > 4) {
                    _renderobject = 0;
                }
                // Update randomizer
                _dust_arr[_renderobject]->UpdateRandomizer(_randomizer);
                // Update position
                _dust_arr[_renderobject]->UpdatePosition(decoratorposition);
                // Frame set to zero
                _dust_arr[_renderobject]->UpdateFrame(0);
            }
            
        }else{
            _updatedust = false;
        }
    }
    
    // Update orientation
    _dust_arr[_renderobject]->UpdateOrientation(orientation);
    // Render
    _dust_arr[_renderobject]->Render();
    
}

void DustDecorator::UpdateDustRandomizer(const glm::vec3 &randomizer){
    _randomizer = randomizer;
}














Dust::Dust(const char* obj_path, const char* tex_path)
{
    
    // Default frame set to 9
    _frame = 9;
    
    // Initialization
    _ProcessInitializeObject(obj_path,tex_path);
    
}

/* Clone constructor */
Dust::Dust(const char* tex_path)
{
    // New texture
    _texture = loadDDS(tex_path);
    
}

Dust::~Dust()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbos[0]);
    glDeleteBuffers(1, &_vbos[1]);
    glDeleteBuffers(1, &_vbos[2]);
    glDeleteBuffers(1, &_vbos[3]);
    glDeleteProgram(_programID);
    glDeleteTextures(1, &_texture);
}

/* In Rendering */
void Dust::Render()
{
    
    // Set to 9, If reached 9
    if (_frame < 10) {
        if ( g_CLOCK -> RecieveFPS() ){
            // Rendering
            _ProcessRenderObject();
            // Increase frame
            _frame++;
        }
    }
    
}

Dust* Dust::Clone(const char* tex_path)
{
    
    Dust* clone = new Dust(tex_path);
    
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

void Dust::_ProcessInitializeObject(const char* obj_path,
                                    const char* tex_path)
{
    
    
    _motion = {
        // Scale, Alpha
        0.03, 1.00,
        0.10, 1.00,
        0.17, 1.00,
        0.19, 1.00,
        0.21, 1.00,
        0.23, 1.00,
        0.25, 1.00,
        0.27, 0.80,
        0.29, 0.60,
        0.31, 0.00,
    };
    
    _position   =
    _randomizer = glm::vec3(0);
    
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

void Dust::_ProcessRenderObject()
{
    
    _scale = _motion[_frame*2];
    _uvalpha = _motion[_frame*2+1];
    
    // ModelMatrix
    glm::mat4 rotationmatrix = glm::eulerAngleYXZ(_orientation.y, 0.0f, 0.0f);
    glm::mat4 translationmatrix = translate(glm::mat4(1.0), _position);
    glm::mat4 scalingmatrix = scale(glm::mat4(1.0), glm::vec3(1) * _scale);
    glm::mat4 modelmatrix = translationmatrix * rotationmatrix * scalingmatrix;
    glm::mat4 atranslationmatrix = translate(glm::mat4(1.0), _randomizer);
    glm::mat4 mvp = g_PROJECTIONMATRIX * atranslationmatrix * g_VIEWMATRIX * modelmatrix;
    
    glUseProgram(_programID);
    
    glEnable(GL_BLEND);
    
    glUniformMatrix4fv(_mvpID, 1, GL_FALSE, &mvp[0][0]);
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


void Dust::UpdateFrame(const int &value){
    _frame = value;
}
void Dust::UpdateRandomizer(const glm::vec3 &randomizer){
    _randomizer = randomizer;
}
void Dust::UpdatePosition(const glm::vec3 &position){
    _position = position;
}
void Dust::UpdateOrientation(const glm::vec3 &orientation){
    _orientation = orientation;
}

