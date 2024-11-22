//
//  a_stuff.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/03.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef a_stuff_hpp
#define a_stuff_hpp

#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/string_cast.hpp>

extern GLFWwindow*  g_WINDOW;
extern glm::mat4    g_PROJECTIONMATRIX;
extern glm::mat4    g_VIEWMATRIX;

class A_Stuff
{
private:
    
    glm::vec3                   _angle;
    glm::vec3                   _scale;
    glm::vec3                   _postion;
    glm::vec3                   _lightposition;
    glm::vec3                   _muzzleflashposition;
    glm::vec3                   _randomizer;
    
    GLuint                      _vao;
    GLuint                      _vbos[4];
    GLuint                      _programID;
    GLuint                      _mvpID;
    GLuint                      _viewmatrixID;
    GLuint                      _modelmatrixID;
    GLuint                      _lightID;
    GLuint                      _muzzleflashID;
    GLuint                      _texture;
    GLuint                      _textureID;
    
    std::vector<glm::vec3>      _vertices;
    std::vector<glm::vec2>      _uvs;
    std::vector<glm::vec3>      _normals;
    std::vector<unsigned short> _indices;
    std::vector<glm::vec3>      _indexed_vertices;
    std::vector<glm::vec2>      _indexed_uvs;
    std::vector<glm::vec3>      _indexed_normals;
    
    glm::mat4                   _rotationmatrix;
    glm::mat4                   _translationmatrix;
    glm::mat4                   _scalingmatrix;
    glm::mat4                   _modelmatrix;
    glm::mat4                   _mvp;
    
    // Bullet hole
    glm::mat4                   _bh_mvp;
    bool                        _bh_action;
    GLuint                      _bh_image;
    int                         _bh_number;
    std::vector<GLuint>         _bh_arrID;
    std::vector<GLuint>         _bh_arrmvpID;
    std::vector<glm::mat4>      _bh_arrmvp;
    
    // OBB
    std::vector<glm::vec3>      _obbvertices;
    
public:
    
    A_Stuff();
    A_Stuff(
            const char *tex_path,
            const char *obj_path,
            const char *obb_path,
            const char *vsh_path,
            const char *fsh_path
            );
    ~A_Stuff();
    void Clone(A_Stuff* clone);
    void Render();
    void RenderBulletHole();
    void UpdateAngle(const glm::vec3 &angle);
    void UpdateScale(const glm::vec3 &scale);
    void UpdatePosition(const glm::vec3 &position);
    void UpdateLightPosition(const glm::vec3 &position);
    void UpdateFlashPosition(const glm::vec3 &position);
    void UpdateRandomizer(const glm::vec3 &randomizer);
    void UpdateBulletHoleTexture(GLuint &texture);
    void UpdateTexture(GLuint &texture);
    void UpdateBulletHoleAction(const bool &value);
    void UpdateAllBulletHoleMVP(const glm::mat4 &matrix);
    void RecieveOBBVertices(std::vector<glm::vec3> &out_obbvertices);
    void RecieveModelMatrix(glm::mat4 &matrix);
    
};


#endif /* a_stuff_hpp */
