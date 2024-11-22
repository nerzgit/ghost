//
//  a_plane.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/15.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "a_plane.hpp"
#include "usge/shader.hpp"
#include "usge/texture.hpp"

A_Plane::A_Plane(const char *tex_path, const float &size)
{
    
    _projection = glm::ortho(0.0f, static_cast<GLfloat>(g_SCREENWIDTH), 0.0f, static_cast<GLfloat>(g_SCREENHEIGHT));
    
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    _programID = LoadShaders( "game/game/sd/Ortho_Plane.vertexshader", "game/game/sd/Ortho_Plane.fragmentshader" );
    _projectionMatrixID = glGetUniformLocation(_programID, "projection");
    _colorID = glGetUniformLocation(_programID, "texcolor");
    _alphaID = glGetUniformLocation(_programID, "alpha");
    
    _texture = loadDDS(tex_path);
    _textureID = glGetUniformLocation(_programID, "tex");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    // Resize to object ratio
    float sc_w, sc_h;
    float screenW = g_SCREENWIDTH;
    float screenH = g_SCREENHEIGHT;
    if (screenW > screenH) {
        sc_w = screenH / screenW;
        sc_h = 1;
    }else if(screenW < screenH) {
        sc_w = 1;
        sc_h = screenW / screenH;
    }else{
        sc_w = 1;
        sc_h = 1;
    }
    
    static GLfloat input_vertices[] = {
        sc_w*(0.0f), sc_h*(1.0f), 0.0f,
        sc_w*(0.0f), sc_h*(0.0f), 0.0f,
        sc_w*(1.0f), sc_h*(0.0f), 0.0f,
        sc_w*(1.0f), sc_h*(0.0f), 0.0f,
        sc_w*(1.0f), sc_h*(1.0f), 0.0f,
        sc_w*(0.0f), sc_h*(1.0f), 0.0f,
    };
    
    static GLfloat input_uvs[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };
    
    for (int i=0; i<18; i++) _vertices[i] = input_vertices[i] * size - 1.0f;
    for (int i=0; i<12; i++) _uvs[i] = input_uvs[i] - 1.0f;
    
    glGenBuffers(1, &_vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &_vbos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_uvs), _uvs, GL_STATIC_DRAW);
    
}

A_Plane::~A_Plane()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbos[0]);
    glDeleteBuffers(1, &_vbos[1]);
    glDeleteProgram(_programID);
    glDeleteTextures(1, &_texture);
}

void A_Plane::Render()
{
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(_programID);
    
    glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), _position);
    glUniformMatrix4fv(_projectionMatrixID, 1, GL_FALSE, &TranslateMatrix[0][0]);
    glUniform4f(_colorID,_color.x,_color.y,_color.z,_color.w);
    
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
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
    glDisable(GL_BLEND);
    
}

void A_Plane::UpdatePosition(const glm::vec3 &position)
{
    _position = position;
}

void A_Plane::UpdateColor(const glm::vec4 &color)
{
    _color = color;
}
