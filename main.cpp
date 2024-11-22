//
//  main.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/01.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "dta/global.hpp"
#include "game.hpp"
#include "usge/key.hpp"
#include "usge/clock.hpp"
#include "util/randomizer.hpp"

Game *game;


void initOpenGL();

int main( void )
{
    // OpenGL initialize
    initOpenGL();
    
    // Setup key config
    keysetup();
    
    game = new Game();
    
    do{
        
        DoRandomizer();
        
        // Global clock activate
        g_CLOCK->UpdateFPS();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_SCREENWIDTH, g_SCREENHEIGHT);
        
        game->Render();
        
        glDisable(GL_BLEND);
        
        glfwSwapBuffers(g_WINDOW);
        glfwPollEvents();
        
    }while(!g_GAMEQUIT && glfwWindowShouldClose(g_WINDOW) == 0);
    
    glfwTerminate();
    
    return 0;
}

void initOpenGL()
{
    
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Opengl 4.X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // Opengl X.1 !!!4.1 supported from MacBook Early 2015!!!
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    int windowWidth = 2304/3;
    int windowHeight = 1440/3;
    
    g_WINDOW = glfwCreateWindow( windowWidth, windowHeight, "PLAY", NULL, NULL);
    if( g_WINDOW == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
    }
    glfwMakeContextCurrent(g_WINDOW);
    
    int frameBufferWidth = windowWidth;
    int frameBufferHeight = windowHeight;
    glfwGetFramebufferSize(g_WINDOW, &frameBufferWidth, &frameBufferHeight);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
    }
    
    glfwSetInputMode(g_WINDOW, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(g_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwPollEvents();
    glfwSetCursorPos(g_WINDOW, windowWidth/2, windowHeight/2);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Get screen size
    glfwGetWindowSize(g_WINDOW, &g_SCREENWIDTH, &g_SCREENHEIGHT);
    
    /** ONLY RETINA DISPLAY **/
    g_SCREENWIDTH  *= 2;
    g_SCREENHEIGHT *= 2;
    
    printf("Initialized OpenGL\n");
}
