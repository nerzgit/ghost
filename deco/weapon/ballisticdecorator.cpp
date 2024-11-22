//
//  ballisticdecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/12.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "ballisticdecorator.hpp"
#include "util/randomizer.hpp"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/quaternion_utils.hpp"





BallisticDecorator::BallisticDecorator(const char* obj_path,
                                       const char* tex_path)
{
    _firstrender = true;
    _updatecounter = 0;
    _ProcessInitializeObject(obj_path, tex_path);
}

BallisticDecorator::~BallisticDecorator()
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
void BallisticDecorator::ProcessBallistic()
{
    _ProcessRenderObject();
}





void BallisticDecorator::_ProcessInitializeObject(const char* obj_path,
                                                  const char* tex_path)
{
    
    _modelmatrix = glm::mat4(1.0);
    
    // Initialize VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    // Compile shader to GLSL
    _programID         = LoadShaders( "game/game/sd/Player_MuzzleFlash.vertexshader", "game/game/sd/Player_MuzzleFlash.fragmentshader"  );
    _texture           = loadDDS(tex_path);
    _textureID         = glGetUniformLocation(_programID, "myTextureSampler");
    _mvpID             = glGetUniformLocation(_programID, "MVP");
    _synthesismatrixID = glGetUniformLocation(_programID, "SynthesisM");
    
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






void BallisticDecorator::_ProcessRenderObject()
{
    
    if (_firstrender) {
        _firstrender = false;
        
        PlayerStructStatus stsfile;
        WeaponDecorator::_o_weapon->RecieveStructStatus(stsfile);
        _valancematrix = glm::translate(glm::mat4(1.0), glm::vec3(0,stsfile.ballisticlengthy, stsfile.ballisticlengthz));

    }
    
    // Only once!!!
    if (_updatecounter == 0) {
        
        _updatecounter++;
        
        // Valiables ->
        // Random glm::vec3 (Sync s_plane_b's randomizer).
        glm::vec3 randmizer = Randomizer(1, 1000, 10000);
        // Length Ballistic
        double len_bal = -0.3;
        // Random float (For RandTranslationMatrix)
        double len = (Randomizer(1, 6) / 5 ) * -1 + len_bal;
        
        // Parameter ->
        // :: Ballistic - muzzle ::    Random rotate on Ballistic local matrix.
        glm::mat4 RandRotationMatrix = glm::eulerAngleYXZ(randmizer.y, randmizer.x, randmizer.z);
        // :: Ballistic - muzzle ::    Length of Ballistic from object.
        glm::mat4 RandTranslationMatrix = translate(glm::mat4(1.0f), glm::vec3(0,0,(float)len));
        // :: Ballistic          ::    Ballistic scaling
        glm::mat4 ScalingMatrix = scale(glm::mat4(1.0), glm::vec3(1.2,1.2,20.0));
        
        // *** In Ballistic, modelmatrix need update from playermanager ***
        _modelmatrix = WeaponDecorator::_o_weapon->RecieveModelMatrix(0);
        _mvp = g_PROJECTIONMATRIX * g_VIEWMATRIX * _modelmatrix * _valancematrix;
        _synthesismatrix = WeaponDecorator::_o_weapon->RecieveConstraintMatrix(0) * RandRotationMatrix * RandTranslationMatrix * ScalingMatrix;
        
    }

    glUseProgram(_programID);
    
    glUniformMatrix4fv(_mvpID, 1, GL_FALSE, &_mvp[0][0]);
    glUniformMatrix4fv(_synthesismatrixID, 1, GL_FALSE, &_synthesismatrix[0][0]);
    
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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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





void BallisticDecorator::UpdateBallisticModelMatrix(glm::mat4 &matrix){
    _modelmatrix = matrix;
}

void BallisticDecorator::UpdateBallisticCounterToZero(){
    _updatecounter = 0;
}
