//
//  control.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef control_hpp
#define control_hpp

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Clock;
extern GLFWwindow* g_WINDOW;
extern glm::mat4   g_PROJECTIONMATRIX;
extern glm::mat4   g_VIEWMATRIX;
extern int         g_SCREENWIDTH;
extern int         g_SCREENHEIGHT;
extern Clock*      g_CLOCK;

class Control
{
private:
    glm::vec3   _right;
    glm::vec3   _up;
    float       _mousespeed;
    float       _maxverticalangle;
    
protected:
    glm::vec3   _orientation;
    glm::vec3   _condition;
    glm::vec3   _direction;
    glm::vec3   _position;
    glm::vec3   _lastposition;
    glm::vec3   _valanceposition;
    glm::vec3   _shakeposition;
    double      _windowcursorpositionX;
    double      _windowcursorpositionY;
    float       _verticalangle;
    float       _horizontalangle;
    float       _deltatime;
    float       _dashspeed;
    float       _speed;
    float       _fov;
    bool        _joystickis;
    bool        _invertxaxis;
    bool        _invertyaxis;
    
    void _InitializeControl();
    
public:
    Control();
    ~Control();
    void Reset();
    void InputComputing();
    void ProcessComputing();
    void OutputComputing();
};


#endif /* control_hpp */
