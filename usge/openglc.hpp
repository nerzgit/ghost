//
//  openglc.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/30.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef openglc_hpp
#define openglc_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int g_SCREENWIDTH;
extern int g_SCREENHEIGHT;
extern GLFWwindow* g_WINDOW;

void FullScreenMode();
void WindowMode();
void DoubleViewPort();
void NormalViewPort();
void WindowSize(int w, int h);


#endif /* openglc_hpp */
