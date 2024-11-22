//
//  clock.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "clock.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

Clock* Clock::_Instance    = 0;
double Clock::_currenttime = 0;
double Clock::_lasttime    = 0;
bool   Clock::_fps         = 0;

Clock::Clock()
{
    
}

Clock* Clock::Instance()
{
    if (_Instance == 0)
        _Instance = new Clock;
    return _Instance;
}

void Clock::UpdateFPS()
{
    // Get current time
    double _currenttime = glfwGetTime();
    // Each 120 frame per sec
    if ( _currenttime - _lasttime >= 1.0/float(120) ) {
        _lasttime = _currenttime;
        _fps = true;
    }else{
        _fps = false;
    }
}

bool Clock::RecieveFPS()
{
    return _fps;
}
