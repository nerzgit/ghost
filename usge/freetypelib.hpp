//
//  freetypelib.hpp
//  play
//
//  Created by IDE KENTARO on 2019/05/16.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef freetypelib_hpp
#define freetypelib_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

extern int g_SCREENWIDTH;
extern int g_SCREENHEIGHT;

class FreeTypeLib
{
private:
    
    int screenWidth, screenHeight;
    float sc_w, sc_h, sc_scl;
    
    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        GLuint     TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;        // Size of glyph
        glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
        GLuint     Advance;     // Horizontal offset to advance to next glyph
    };
    
    std::map<GLchar, Character> Characters;
    GLuint    VAO;
    GLuint    VBO;
    GLuint    freetypeprogramID;
    GLuint    ProjectionMatrixID;
    GLuint    RotateMatrixID;
    glm::mat4 projection;
    
    float     width;
    float     height;
    
    void FetchAlignWidth(const std::string &text, std::vector<GLfloat> &ch_w, const GLfloat &scale);
    
public:
    
    FreeTypeLib(const char* font);
    void RenderText (const std::string &text, const GLfloat &x, const GLfloat &y, const GLfloat &scale, const glm::vec3 &color);
    void RenderTextR(const std::string &text, const GLfloat &x, const GLfloat &y, const GLfloat &scale, const glm::vec3 &color);
    void RenderTextC(const std::string &text, const GLfloat &x, const GLfloat &y, const GLfloat &scale, const glm::vec3 &color);
    
};

#endif /* freetypelib_hpp */
