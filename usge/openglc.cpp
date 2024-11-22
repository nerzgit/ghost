//
//  openglc.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/30.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "openglc.hpp"

void FullScreenMode()
{
    
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    
    // get reolution of monitor
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    // swithc to full screen
    glfwSetWindowMonitor( g_WINDOW, monitor, 0, 0, mode->width, mode->height, 0 );
}

void WindowMode()
{
    glfwSetWindowMonitor( g_WINDOW, nullptr,  0, 0, g_SCREENWIDTH, g_SCREENHEIGHT, 0 );
}

void DoubleViewPort()
{
    glfwGetWindowSize( g_WINDOW , &g_SCREENWIDTH, &g_SCREENHEIGHT);
    g_SCREENWIDTH *= 2; g_SCREENHEIGHT *= 2;
}

void NormalViewPort()
{
    glfwGetWindowSize( g_WINDOW , &g_SCREENWIDTH, &g_SCREENHEIGHT);
    g_SCREENWIDTH /= 2; g_SCREENHEIGHT /= 2;
}

void WindowSize(int w, int h)
{
    glfwSetWindowSize(g_WINDOW, w, h);
    glfwGetWindowSize( g_WINDOW , &g_SCREENWIDTH, &g_SCREENHEIGHT);
}
