//
//  a_plane.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/15.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef a_plane_hpp
#define a_plane_hpp

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

extern GLFWwindow*  g_WINDOW;
extern glm::mat4    g_PROJECTIONMATRIX;
extern glm::mat4    g_VIEWMATRIX;
extern int          g_SCREENWIDTH;
extern int          g_SCREENHEIGHT;

class A_Plane
{
private:
    
    GLuint    _vao;
    GLuint    _vbos[2];
    GLuint    _texture;
    GLuint    _textureID;
    GLuint    _programID;
    GLuint    _colorID;
    GLuint    _alphaID;
    GLfloat   _vertices[18];
    GLfloat   _uvs[12];
    GLuint    _projectionMatrixID;
    glm::mat4 _projection;
    glm::vec3 _position;
    glm::vec4 _color;
    
public:
    
    A_Plane(const char *tex_path, const float &size);
    ~A_Plane();
    void Render();
    void UpdatePosition(const glm::vec3 &position);
    void UpdateColor(const glm::vec4 &color);
    
};


#endif /* a_plane_hpp */
