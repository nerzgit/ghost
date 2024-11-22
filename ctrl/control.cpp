//
//  control.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "control.hpp"
#include "util/squareroot.hpp"
#include "usge/key.hpp"
#include "usge/clock.hpp"

Control::Control()
{
    
    _direction        = glm::vec3(0);
    _orientation      = glm::vec3(0);
    _right            = glm::vec3(0);
    _up               = glm::vec3(0);
    _condition        = glm::vec3(0);
    _position         = glm::vec3(12.0, 0, 8.0); // Default
    _lastposition     = glm::vec3(12.0, 0, 8.0); // Default
    _shakeposition    = glm::vec3(0);
    _valanceposition  = glm::vec3(0,1.3,0);
    _verticalangle    = 0;
    _horizontalangle  = 3.14f;
    _mousespeed       = 0.002f;
    _speed            = 0.6f;
    _deltatime        = 0.01f;
    _fov              = 80.0f;
    _dashspeed        = 0.0f;
    _joystickis       = false;
    _invertxaxis      = false;
    _invertyaxis      = false;
    
}

Control::~Control()
{
    
}

void Control::Reset()
{
    
    _direction        = glm::vec3(0);
    _orientation      = glm::vec3(0);
    _right            = glm::vec3(0);
    _up               = glm::vec3(0);
    _condition        = glm::vec3(0);
    _position         = glm::vec3(12.0, 0, 8.0); // Default
    _lastposition     = glm::vec3(12.0, 0, 8.0); // Default
    _shakeposition    = glm::vec3(0);
    _valanceposition  = glm::vec3(0,1.3,0);
    _verticalangle    = 0;
    _horizontalangle  = 3.14f;
    _mousespeed       = 0.002f;
    _speed            = 0.6f;
    _deltatime        = 0.01f;
    _fov              = 80.0f;
    _dashspeed        = 0.0f;
    
}

void Control::InputComputing()
{
    
    _joystickis = joystick_is();
    
    _lastposition = _position;
    
    _condition = glm::vec3(0);
    
    // Get mouse position
    glfwGetCursorPos(g_WINDOW, &_windowcursorpositionX, &_windowcursorpositionY);
    
    if (_joystickis){
        _windowcursorpositionX += stickcon(PAD_LOOKUP_RL) * 20.0;
        _windowcursorpositionY += stickcon(PAD_LOOKUP_UD) * 15.0;
    }
    
    // Reset mouse position.
    glfwSetCursorPos(g_WINDOW, g_SCREENWIDTH/2, g_SCREENHEIGHT/2);
    
    // Compute new orientation
    if (_invertyaxis) {
        _horizontalangle += _mousespeed * float( g_SCREENHEIGHT/2 - _windowcursorpositionY );
    }else{
        _verticalangle += _mousespeed * float( g_SCREENHEIGHT/2 - _windowcursorpositionY );
    }
    if (_invertxaxis) {
        _verticalangle += _mousespeed * float(  g_SCREENWIDTH/2 - _windowcursorpositionX );
    }else{
        _horizontalangle += _mousespeed * float(  g_SCREENWIDTH/2 - _windowcursorpositionX );
    }
    
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    // YDirection is only 180 degree move.
    if (-1.5 <= _verticalangle && _verticalangle <= 1.5){
        _maxverticalangle = _verticalangle;
    }
    _verticalangle = _maxverticalangle;
    
    _direction = glm::vec3(
                           cos(_verticalangle) * sin(_horizontalangle),
                           sin(_verticalangle),
                           cos(_verticalangle) * cos(_horizontalangle)
                           );
    
    _orientation = glm::vec3(
                             sin(_horizontalangle),
                             0,
                             cos(_horizontalangle)
                             );
    
    _right = glm::vec3(
                       sin(_horizontalangle - 3.14f/2.0f),
                       0,
                       cos(_horizontalangle - 3.14f/2.0f)
                       );
    
    _up = glm::cross( _right, _direction );
    
    _InitializeControl();
    
}


void Control::ProcessComputing()
{

    // Gamepad
    float gFB = 0;
    float gRL = 0;
    if (_joystickis){
        gFB = stickcon(PAD_STRAIGHT_FB) * -1;
        gRL = stickcon(PAD_STRAIGHT_RL) * -1;
    }
    
    // Move forward
    if (keycon(KEY_STRAIGHT_F, KEY_ON)){
        _position += _orientation * _deltatime * _speed * _dashspeed;
    }else if (gFB < 0){
        _position += _orientation * _deltatime * _speed * _dashspeed * (gFB);
    }
    // Move backward
    if (keycon(KEY_STRAIGHT_B, KEY_ON)){
        _position -= _orientation * _deltatime * _speed * _dashspeed;
    }else if (gFB > 0){
        _position -= _orientation * _deltatime * _speed * _dashspeed * (gFB * -1);
    }
    // Strate right
    if (keycon(KEY_STRAIGHT_R, KEY_ON)){
        _position += _right * _deltatime * _speed * _dashspeed;
    }else if (gRL < 0){
        _position += _right * _deltatime * _speed * _dashspeed * (gRL * -1);
    }
    // Strate left
    if (keycon(KEY_STRAIGHT_L, KEY_ON)){
        _position -= _right * _deltatime * _speed * _dashspeed;
    }else if (gRL > 0){
        _position -= _right * _deltatime * _speed * _dashspeed * (gRL);
    }
    
    float cFoV = _fov;// - 5 * glfwGetMouseWheel();
    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    g_PROJECTIONMATRIX = glm::perspective(glm::radians(cFoV), 4.0f / 3.0f, 0.1f, 100.0f);
    
}

void Control::OutputComputing()
{
    
    glm::vec3 cposition = _position + _condition + _valanceposition + _shakeposition;

    g_VIEWMATRIX = glm::lookAt(
                              cposition,             // Camera is here
                              cposition+_direction,  // and looks here : at the same position, plus "direction"
                              _up                    // Head is up (set to 0,-1,0 to look upside-down)
                              );
}

void Control::_InitializeControl()
{
    static int count = 0;
    if (count == 0) {
        _verticalangle = _horizontalangle = 0;
        _position      = glm::vec3(12.0, 0, 8.0); // Default
        _lastposition  = glm::vec3(12.0, 0, 8.0); // Default
        count = 1;
    }

}
