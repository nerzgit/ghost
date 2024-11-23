//
//  global.h
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef global_h
#define global_h

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "usge/clock.hpp"

GLFWwindow*     g_WINDOW;
glm::mat4       g_PROJECTIONMATRIX    = glm::mat4(1.0);
glm::mat4       g_VIEWMATRIX          = glm::mat4(1.0);
int             g_SCREENWIDTH         = 0;
int             g_SCREENHEIGHT        = 0;
bool            g_GAMEQUIT            = false;
bool            g_GAMESTOP            = false;
bool            g_GAMEOVER            = false;
Clock*          g_CLOCK               = Clock::Instance();

#endif /* global_h */
