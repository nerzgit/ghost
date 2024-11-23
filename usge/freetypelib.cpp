//
//  freetypelib.cpp
//  play
//
//  Created by IDE KENTARO on 2019/05/16.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "freetypelib.hpp"
#include "shader.hpp"
// Std. Includes
#include <iostream>
#include <map>
#include <string>
#include <vector>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H


FreeTypeLib::FreeTypeLib(const char* font)
{
    
    screenWidth = g_SCREENWIDTH;
    screenHeight = g_SCREENHEIGHT;
    
    // ProjectionView is ortho
    projection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f, static_cast<GLfloat>(screenHeight));
    
    // Compile and setup the shader
    freetypeprogramID = LoadShaders("game/game/sd/FreeType.vertexshader", "game/game/sd/FreeType.fragmentshader");
    
    ProjectionMatrixID = glGetUniformLocation(freetypeprogramID, "projection");
    RotateMatrixID = glGetUniformLocation(freetypeprogramID, "R");
    
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    
    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, font, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                     GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer
                     );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    
    // Configure VAO/VBO for texture quads
    // Bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    

    // Resize to object ratio
    float screenW = screenWidth;
    float screenH = screenHeight;
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
    
    sc_scl = (float)screenWidth / 2304.0f / 2.0f;
    
}

void FreeTypeLib::RenderText(const std::string &text,
                             const GLfloat     &x,
                             const GLfloat     &y,
                             const GLfloat     &scale,
                             const glm::vec3   &color)
{

    GLfloat in_s = sc_scl * scale;
    GLfloat in_x = screenWidth * x;
    GLfloat in_y = screenHeight * y;
    
    // Activate corresponding render state
    glUseProgram(freetypeprogramID);
    
    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(freetypeprogramID, "textColor"), color.x, color.y, color.z);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    
    width  = 0;
    height = 0;
    
    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        GLfloat xpos = (in_x + sc_w * ch.Bearing.x * in_s);
        GLfloat ypos = (in_y - (ch.Size.y - ch.Bearing.y) * in_s);
        
        GLfloat w = sc_w * ch.Size.x * in_s;
        GLfloat h = sc_w * ch.Size.y * in_s;
        
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        in_x += (ch.Advance >> 6) * sc_w * in_s; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        
        width += w;
        height = h;
        
    }
    
    glDisable(GL_BLEND);
    
}


void FreeTypeLib::FetchAlignWidth(const std::string    &text,
                                  std::vector<GLfloat> &ch_w,
                                  const GLfloat        &scale)
{
    
    GLfloat t_w = 0;
    
    // Get all width
    int count = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        ch_w[count] = sc_w * (ch.Advance >> 6) * scale;
        t_w += ch_w[count];
        count ++;
    }
    
    for (int i=0; i<count; i++) {
        GLfloat n_w = ch_w[i];
        ch_w[i] = t_w;
        t_w -= n_w;
    }
    
}

void FreeTypeLib::RenderTextR(const std::string &text,
                              const GLfloat     &x,
                              const GLfloat     &y,
                              const GLfloat     &scale,
                              const glm::vec3   &color)
{
    
    GLfloat in_s = sc_scl * scale;
    GLfloat in_x = screenWidth * x;
    GLfloat in_y = screenHeight * y;
    
    std::vector<GLfloat> ch_w(text.length());
    FetchAlignWidth(text, ch_w, in_s);
    
    in_x -= ch_w[0];
    
    // Activate corresponding render state
    glUseProgram(freetypeprogramID);
    
    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(freetypeprogramID, "textColor"), color.x, color.y, color.z);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    
    width  = 0;
    height = 0;
    
    // Iterate through all characters
    int count = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        GLfloat w = sc_w * ch.Size.x * in_s;
        GLfloat h = sc_w * ch.Size.y * in_s;
        
        GLfloat xpos = (in_x + sc_w * ch.Bearing.x * in_s);
        GLfloat ypos = (in_y - (ch.Size.y - ch.Bearing.y) * in_s);
        
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        in_x += (ch.Advance >> 6) * sc_w * in_s; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        
        width += w;
        height = h;
        count ++;
        
    }
    
    glDisable(GL_BLEND);
    
}

void FreeTypeLib::RenderTextC(const std::string &text,
                              const GLfloat     &x,
                              const GLfloat     &y,
                              const GLfloat     &scale,
                              const glm::vec3   &color)
{
    GLfloat in_s = sc_scl * scale;
    GLfloat in_x = screenWidth * x;
    GLfloat in_y = screenHeight * y;
    
    std::vector<GLfloat> ch_w(text.length());
    FetchAlignWidth(text, ch_w, in_s);
    
    in_x -= ch_w[0] / 2;
    
    // Activate corresponding render state
    glUseProgram(freetypeprogramID);
    
    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(freetypeprogramID, "textColor"), color.x, color.y, color.z);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    
    width  = 0;
    height = 0;
    
    // Iterate through all characters
    int count = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        GLfloat w = sc_w * ch.Size.x * in_s;
        GLfloat h = sc_w * ch.Size.y * in_s;
        
        GLfloat xpos = (in_x + sc_w * ch.Bearing.x * in_s);
        GLfloat ypos = (in_y - (ch.Size.y - ch.Bearing.y) * in_s);
        
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        in_x += (ch.Advance >> 6) * sc_w * in_s; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        
        width += w;
        height = h;
        count ++;
        
    }
    
    glDisable(GL_BLEND);
}
