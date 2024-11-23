//
//  condition.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "condition.hpp"
#include "usge/key.hpp"
#include "util/squareroot.hpp"

Condition::Condition() : Control()
{
    _jump                   =
    _fall                   =
    _land                   = false;
    _jump_time              =
    _fall_time              =
    _land_time              = 0;
    _jump_time_rate         = 0.05;
    _fall_time_rate         = 0.012;
    _land_time_rate         = 0.04;
    _jump_maximum           = 1.0;
    _land_maximum           = -0.3;
    _gravity                = 9.80665;
    _current_terrain        =
    _last_terrain           =
    _actual_current_terrain =
    _actual_last_terrain    =
    _actual_current_height  = 0;
    _run_motion_rate        = 0;
}

Condition::~Condition()
{
    
}

void Condition::Reset()
{
    _jump                   =
    _fall                   =
    _land                   = false;
    _jump_time              =
    _fall_time              =
    _land_time              = 0;
    _jump_time_rate         = 0.05;
    _fall_time_rate         = 0.012;
    _land_time_rate         = 0.04;
    _jump_maximum           = 1.0;
    _land_maximum           = -0.3;
    _gravity                = 9.80665;
    _current_terrain        =
    _last_terrain           =
    _actual_current_terrain =
    _actual_last_terrain    =
    _actual_current_height  = 0;
    _run_motion_rate        = 0;
}

void Condition::ProcessCondition()
{
    
    // Jump with KEY_JUMP
    if ( (keycon(KEY_JUMP, KEY_ON) || padcon(PAD_JUMP, KEY_ON)) && !_jump && !_fall && !_land)
    {
        // Syncronize terrain
        _actual_current_terrain = _current_terrain;
        // True jump to start
        _jump = true;
    }
    
    // Natural fall
    if (_last_terrain - _current_terrain > 0.1)
    {
        // Syncronize terrain
        _actual_current_height = _last_terrain;
        // Syncronize last terrain
        _actual_last_terrain = _last_terrain;
        // Start fall or not
        if (_current_terrain < _actual_last_terrain && !_jump && !_fall && !_land) {
            _fall = true;
        }else{
            _fall = false;
        }
    }
    
    float j = this->ConditionJump();
    float f = this->ConditionFall();
    float l = this->ConditionLand();
    
    // Avoid glitch
    if (_jump) {
        f = 0;
        l = 0;
    }
    if (_fall) {
        j = 0;
        l = 0;
    }
    if (_land) {
        j = 0;
        f = 0;
    }
    
    // Update on Control Cls!!
    if (!_jump && !_fall && !_land) {
        Control::_condition.y = _current_terrain;
    } else {
        Control::_condition.y = j+f+l;
    }
    
}

float Condition::ConditionJump()
{
    // Lock another
    if (_jump) _fall = _land = false;
    
    // Jump mode available
    if (_jump) {
        
        // Increase time.
        _jump_time += _jump_time_rate;
        float t = _jump_time;
        
        // Bezier curves vec2 options
        glm::vec2 xy1, xy2, xy3, xy4;
        xy1 = glm::vec2(   0,       0 );
        xy2 = glm::vec2( 0.2,    -0.4 );
        xy3 = glm::vec2( 0.2, _jump_maximum );
        xy4 = glm::vec2(   1, _jump_maximum );
        
        // Switch
        if (t >= 1) {
            _fall = true;
        }else{
            // Example: xy1(0,0) xy2(2,-2.5) xy3(2,5) xy4(4,5)
            _actual_current_height = (1-t)*(1-t)*(1-t)*xy1.y + 3*((1-t)*(1-t))*t*xy2.y + 3*((1-t))*(t*t)*xy3.y + (t*t*t)*xy4.y + _actual_current_terrain;
        }
        
        return _actual_current_height;
        
    }
    
    return 0;
}

float Condition::ConditionFall()
{
    // Lock another
    if (_fall) _jump = _land = false;
    
    if (_fall) {
        
        // Increase time.
        _fall_time += _fall_time_rate;
        float t = _fall_time;
        
        // h=1/2gt^2 Ez Physics
        _actual_current_height -= (1.0f/2.0f)*_gravity*(t*t);
        
        // Switch
        if (_actual_current_height <= _current_terrain) {
            _land = true;
        }
        
        return _actual_current_height;
        
    }
    
    return 0;
    
}

float Condition::ConditionLand()
{
    // Lock another
    if (_land) _jump = _fall = false;
    
    if (_land) {
        
        // Increase time.
        _land_time += _land_time_rate;
        float t = _land_time;
        
        // Bezier curves vec2 options
        glm::vec2 xy1, xy2, xy3, xy4;
        xy1 = glm::vec2(   0,       0 );
        xy2 = glm::vec2(   0, _land_maximum );
        xy3 = glm::vec2(   1, _land_maximum );
        xy4 = glm::vec2(   1,       0 );
        
        // Switch
        if (t > 1) {
            _jump = _fall = _land = false;
            _jump_time = _fall_time = _land_time = 0;
        }
        
        // Example: xy1(0,0) xy2(2,-2.5) xy3(2,5) xy4(4,5)
        return (1-t)*(1-t)*(1-t)*xy1.y + 3*((1-t)*(1-t))*t*xy2.y + 3*((1-t))*(t*t)*xy3.y + (t*t*t)*xy4.y + _current_terrain;
        
    }
    
    return 0;
    
}

void Condition::ProcessConditionRunSpeed()
{

    // ON Handler
    if ((
        keycon(KEY_RUN, KEY_ON) &&
        keycon(KEY_STRAIGHT_F, KEY_ON)
        ) || (
        padcon(PAD_RUN, KEY_ON) &&
        stickcon(PAD_STRAIGHT_FB) < -0.8
        ) ){
        // Update dashspeed
        Control::_dashspeed = 9.0f;
    }
    
    // OFF Handler
    if (keycon(KEY_RUN, KEY_OFF) && !padcon(PAD_RUN, KEY_ON)){
        // Reset dashspeed
        Control::_dashspeed = 6.0f;
    }
    
    if (
        (
        keycon(KEY_STRAIGHT_F, KEY_ON) ||
        keycon(KEY_STRAIGHT_B, KEY_ON) ||
        keycon(KEY_STRAIGHT_R, KEY_ON) ||
        keycon(KEY_STRAIGHT_L, KEY_ON)
        ) || (
        stickcon(PAD_STRAIGHT_RL) > 0.1 ||
        stickcon(PAD_STRAIGHT_RL) <-0.1 ||
        stickcon(PAD_STRAIGHT_FB) > 0.1 ||
        stickcon(PAD_STRAIGHT_FB) <-0.1
        )
        ){
        
        _flict += 0.3;
        if (_flict > 1.5)
        {
            _flict = 1.5;
        }
        
        // Calculate unit vector.
        glm::vec3 cp = Control::_position;
        glm::vec3 lp = Control::_lastposition;
        glm::vec2 cp_lp_vec = glm::vec2(cp.x - lp.x, cp.z - lp.z);
        float cp_lp_scala = SquareRoot(cp_lp_vec.x * cp_lp_vec.x + cp_lp_vec.y * cp_lp_vec.y);
        if (cp_lp_scala == 0)
        {
            cp_lp_scala = 1;
        }
        _unitvector = glm::vec2(cp_lp_vec.x/cp_lp_scala, cp_lp_vec.y/cp_lp_scala);

    }
    
    if (
        (
        keycon(KEY_STRAIGHT_F, KEY_OFF) &&
        keycon(KEY_STRAIGHT_B, KEY_OFF) &&
        keycon(KEY_STRAIGHT_R, KEY_OFF) &&
        keycon(KEY_STRAIGHT_L, KEY_OFF)
        ) || (
        stickcon(PAD_STRAIGHT_RL) < 0.1 &&
        stickcon(PAD_STRAIGHT_RL) >-0.1 &&
        stickcon(PAD_STRAIGHT_FB) < 0.1 &&
        stickcon(PAD_STRAIGHT_FB) >-0.1
        )
        ){
        
        _flict -= 0.1;
        if (_flict < 0.0) _flict = 0.0;
        
        Control::_position.x += _unitvector.x * _flict * Control::_deltatime;
        Control::_position.z += _unitvector.y * _flict * Control::_deltatime;
        
    }
    
}

void Condition::ProcessConditionRunShake()
{

    float t = 20.6f;
    if (
        (
         keycon(KEY_RUN, KEY_ON)          &&
         keycon(KEY_STRAIGHT_F, KEY_ON)   &&
         keycon(KEY_STRAIGHT_B, KEY_OFF)  &&
         keycon(KEY_STRAIGHT_R, KEY_OFF)  &&
         keycon(KEY_STRAIGHT_L, KEY_OFF)
         ) || (
               padcon(PAD_RUN, KEY_ON)    &&
               stickcon(PAD_STRAIGHT_FB) < -0.8
               )
        ){
        _run_motion_rate += 1.2f;
    }else{
        _run_motion_rate = 0;
    }
    Control::_shakeposition.x = (sin(_run_motion_rate/t)*cos(_run_motion_rate/t))/30.0f;
    Control::_shakeposition.y = sin(_run_motion_rate/3.6f)/30.0f;
    Control::_shakeposition.z = (cos(_run_motion_rate/t)*cos(_run_motion_rate/t))/30.0f;
    
}

void Condition::ProcessConditionWalkRunPose()
{
    
    bool  r    = false;
    bool  w    = false;
    bool  p    = false;
    
    bool  kr   = keycon(KEY_RUN,        KEY_ON);
    bool  ksf  = keycon(KEY_STRAIGHT_F, KEY_ON);
    bool  ksb  = keycon(KEY_STRAIGHT_B, KEY_ON);
    bool  ksr  = keycon(KEY_STRAIGHT_R, KEY_ON);
    bool  ksl  = keycon(KEY_STRAIGHT_L, KEY_ON);
    bool  pr   = padcon(PAD_RUN,        KEY_ON);
    float psfb = stickcon(PAD_STRAIGHT_FB);
    float psrl = stickcon(PAD_STRAIGHT_RL);
    
    if ((kr && ksf && !ksb && !ksr && !ksl) ||
        (pr && psfb < -0.6))
    {
        r = true;
    }
    
    if ((!ksf && !ksb && !ksr && !ksl) &&
        (-0.05 < psfb && psfb < 0.05)  &&
        (-0.05 < psrl && psrl < 0.05))
    {
        p = true;
    }
    
    if (!r && !p) {
        w = true;
    }
    
    _run  = r;
    _walk = w;
    _pose = p;
    
}

void Condition::ProcessConditionGunImpact(const bool &fire)
{
    
    glm::vec2 result = glm::vec2(0);
    
    float         divisionparam = 50.0f;  // Shaking rate (1.0f is max)
    static double amttimer      = 0.4;    // Timer increase of amount
    static double mactimer      = 1.0;    // max timer
    static double macpower      = 0.6;    // max increase shake power
    static double minpower      = 0.1;    // min decrease shake power
    
    static double dispower      = 0.0;
    static float  cursor_fps    = 0;
    static float  upperCursor   = 0;
    static float  lowerCursor   = 0;
    static float  uSideCursor   = 0;
    static float  lSideCursor   = 0;
    static bool   plusSymbol    = true;
    static bool   upperBool     = false;
    static bool   lowerBool     = false;
    

    if (fire) {
        dispower  += upperCursor;
        cursor_fps = 0;
        upperBool  = true;
        lowerBool  = false;
        if (rand() % 2 == 0) plusSymbol = true;
        else plusSymbol = false;
    }
    
    if (upperBool) {
        
        // Increase timer
        cursor_fps += amttimer;
        
        // Animation bezier curve
        glm::vec2 xy1, xy2, xy3, xy4;
        float f_t = cursor_fps;
        float f_m = mactimer;
        
        // Upper
        xy1 = glm::vec2(        0.0,              dispower );
        xy2 = glm::vec2( mactimer/3, macpower/3 + dispower );
        xy3 = glm::vec2( mactimer/2, macpower/2 + dispower );
        xy4 = glm::vec2( mactimer/1, macpower/1 + dispower );
        if (f_t < f_m)
            upperCursor += (f_m-f_t)*(f_m-f_t)*(f_m-f_t)*xy1.y + 3*((f_m-f_t)*(f_m-f_t))*f_t*xy2.y + 3*((f_m-f_t))*(f_t*f_t)*xy3.y + (f_t*f_t*f_t)*xy4.y;
        upperCursor /= divisionparam;
        
        // Side
        xy1 = glm::vec2(        0.0,        0.0 );
        xy2 = glm::vec2( mactimer/3, macpower/3 );
        xy3 = glm::vec2( mactimer/2, macpower/2 );
        xy4 = glm::vec2( mactimer/1, macpower/1 );
        if (f_t < f_m)
            uSideCursor += (f_m-f_t)*(f_m-f_t)*(f_m-f_t)*xy1.y + 3*((f_m-f_t)*(f_m-f_t))*f_t*xy2.y + 3*((f_m-f_t))*(f_t*f_t)*xy3.y + (f_t*f_t*f_t)*xy4.y;
        if (!plusSymbol) uSideCursor *= -1;
        uSideCursor /= divisionparam * 5.0;
        
        // If timer reached to the mactime
        if (cursor_fps >= mactimer) {
            cursor_fps  =
            upperCursor =
            lowerCursor = 0;
            upperBool   = false;
            lowerBool   = true;
        }
        
        result = glm::vec2(upperCursor, uSideCursor);
        
    }
    
    if (lowerBool) {
        
        // Increase timer
        cursor_fps += amttimer/6;
        
        // Animation bezier curve
        glm::vec2 xy1, xy2, xy3, xy4;
        float f_t = cursor_fps;
        float f_m = mactimer;
        
        // Lower
        xy1 = glm::vec2(        0.0,              dispower );
        xy2 = glm::vec2( mactimer/3, minpower/3 + dispower );
        xy3 = glm::vec2( mactimer/2, minpower/2 + dispower );
        xy4 = glm::vec2( mactimer/1, minpower/1 + dispower );
        if (f_t < f_m)
            lowerCursor -= (f_m-f_t)*(f_m-f_t)*(f_m-f_t)*xy1.y + 3*((f_m-f_t)*(f_m-f_t))*f_t*xy2.y + 3*((f_m-f_t))*(f_t*f_t)*xy3.y + (f_t*f_t*f_t)*xy4.y;
        lowerCursor /= divisionparam;
        
        // Side
        xy1 = glm::vec2(        0.0,        0.0 );
        xy2 = glm::vec2( mactimer/3, macpower/3 );
        xy3 = glm::vec2( mactimer/2, macpower/2 );
        xy4 = glm::vec2( mactimer/1, macpower/1 );
        if (f_t < f_m)
            lSideCursor -= (f_m-f_t)*(f_m-f_t)*(f_m-f_t)*xy1.y + 3*((f_m-f_t)*(f_m-f_t))*f_t*xy2.y + 3*((f_m-f_t))*(f_t*f_t)*xy3.y + (f_t*f_t*f_t)*xy4.y;
        // Change to increase
        if (uSideCursor < 0) lSideCursor *= -1;
        lSideCursor /= divisionparam * 5.0;
        
        // If timer reached to the mactime
        if (cursor_fps >= mactimer) {
            dispower    =
            cursor_fps  =
            upperCursor =
            lowerCursor = 0;
            upperBool   = false;
            lowerBool   = false;
        }
        
        result = glm::vec2(lowerCursor, lSideCursor);
        
    }
    
    Control::_verticalangle += result.x;
    Control::_horizontalangle += result.y;
    
}
