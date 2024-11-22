//
//  openingmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/21.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "openingmanager.hpp"
#include "del/delegate.hpp"
#include "abs/a_plane.hpp"
#include "usge/clock.hpp"
#include "usge/key.hpp"
#include "usge/freetypelib.hpp"

OpeningManager::OpeningManager()
{
    _page0background = new A_Plane("game/game/oj/_another_/opening/background.dds",100.0f);
    _page1background = new A_Plane("game/game/oj/_another_/opening/control.dds",3.2f);
    _page2background = new A_Plane("game/game/oj/_another_/opening/background.dds",100.0f);
    _page3background = new A_Plane("game/game/oj/_another_/opening/loading.dds",3.2f);
    _curtain         = new A_Plane("game/game/oj/_another_/opening/background.dds",100.0f);
    _processstartgamef_t = 0;
    _processstartgamecurtain_a = 1;
}

OpeningManager::~OpeningManager()
{
    for (auto e : _delegate_arr) delete e;
    delete _page0background;
    delete _page1background;
    delete _page2background;
    delete _page3background;
    delete _curtain;
}

void OpeningManager::Init()
{
    _ProcessPage0();
    _ProcessPage1();
    _ProcessPage2();
    _ProcessPage3();
}

void OpeningManager::Render()
{
    // Stop music
    (*_delegate_arr[9])("opening");
    glClear(GL_DEPTH_BUFFER_BIT);
    _ProcessStartGame();
}

void OpeningManager::Reset()
{
    _processstartgamef_t = 0;
    _processstartgamecurtain_a = 1;
}

void OpeningManager::_ProcessPage0()
{
    
    bool go = false;
    bool st = false;
    
    int f_t = 0;
    float curtain_a = 1;
    
    do {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_SCREENWIDTH, g_SCREENHEIGHT);
        
        _page0background -> UpdatePosition(glm::vec3(0,0,0));
        _page0background -> UpdateColor(glm::vec4(1,1,1,1));
        _page0background -> Render();
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        (*_delegate_arr[0])("CAUTION");             // Text
        (*_delegate_arr[1])("center");              // Align
        (*_delegate_arr[2])(glm::vec2(0.5, 0.70));  // Position
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98)); // Color
        (*_delegate_arr[4])(3.2f);                  // Size
        (*_delegate_arr[5])();                      // Render(OpenSansRegular)
        
        (*_delegate_arr[0])("CALLOFDUTY ZOMBIE MODE FAN-MADE GAME.");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.55));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(2.2f);
        (*_delegate_arr[5])();
        
        (*_delegate_arr[0])("THIS GAME PROGRAMMED CAN BE PLAY EVEN LOW-LEVEL COMPUTER.");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.50));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(2.2f);
        (*_delegate_arr[5])();
        
        (*_delegate_arr[0])("MAXNERD");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.35));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(1.8f);
        (*_delegate_arr[5])();
        
        g_CLOCK->UpdateFPS();
        if (g_CLOCK->RecieveFPS()) {
            
            f_t++;
            
            if (100 > f_t && f_t >   0) curtain_a -= 1.0f/100.0f;
            if (300 > f_t && f_t > 100) curtain_a  = 0;
            if (400 > f_t && f_t > 300) curtain_a += 1.0f/100.0f;
            if (             f_t > 400) curtain_a  = 1;
            
        }
        
        _curtain -> UpdatePosition(glm::vec3(0,0,0));
        _curtain -> UpdateColor(glm::vec4(1,1,1,curtain_a));
        _curtain -> Render();
        
        glfwSwapBuffers(g_WINDOW);
        glfwPollEvents();
        
        if (f_t > 100 && (keycon(KEY_START, KEY_ON) || padcon(PAD_ACTION, KEY_ON))) st = true;
        if (st        && (keycon(KEY_START, KEY_OFF) || padcon(PAD_ACTION, KEY_OFF))) go = true;
        if (go)
        {
            _curtain -> UpdatePosition(glm::vec3(0,0,0));
            _curtain -> UpdateColor(glm::vec4(1,1,1,1));
            _curtain -> Render();
        }
        
    } while (f_t < 400 && !go);
    
}


void OpeningManager::_ProcessPage1()
{
    bool go = false;
    bool st = false;
    int f_t = 0;
    float curtain_a = 1;
    
    do {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_SCREENWIDTH, g_SCREENHEIGHT);
        
        _page1background -> UpdatePosition(glm::vec3(0,-0.65,0));
        _page1background -> UpdateColor(glm::vec4(1,1,1,1));
        _page1background -> Render();
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        (*_delegate_arr[0])("CONTROL");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.094));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(2.8f);
        (*_delegate_arr[5])();
        
        g_CLOCK->UpdateFPS();
        if (g_CLOCK->RecieveFPS()) {
            
            f_t++;
            
            if (100 > f_t && f_t >   0) curtain_a -= 1.0f/100.0f;
            if (300 > f_t && f_t > 100) curtain_a  = 0;
            if (400 > f_t && f_t > 300) curtain_a += 1.0f/100.0f;
            if (             f_t > 400) curtain_a  = 1;
            
        }
        
        _curtain -> UpdatePosition(glm::vec3(0,0,0));
        _curtain -> UpdateColor(glm::vec4(1,1,1,curtain_a));
        _curtain -> Render();
        
        glfwSwapBuffers(g_WINDOW);
        glfwPollEvents();
        
        if (f_t > 100 && (keycon(KEY_START, KEY_ON) || padcon(PAD_ACTION, KEY_ON)))   st = true;
        if (st        && (keycon(KEY_START, KEY_OFF) || padcon(PAD_ACTION, KEY_OFF))) go = true;
        if (go)
        {
            _curtain -> UpdatePosition(glm::vec3(0,0,0));
            _curtain -> UpdateColor(glm::vec4(1,1,1,1));
            _curtain -> Render();
        }
        
    } while (f_t < 400 && !go);
}

void OpeningManager::_ProcessPage2()
{
    bool go = false;
    bool st = false;
    int f_t = 0;
    
    do {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_SCREENWIDTH, g_SCREENHEIGHT);
        
        _page2background -> UpdatePosition(glm::vec3(0,0,0));
        _page2background -> UpdateColor(glm::vec4(1,1,1,1));
        _page2background -> Render();
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        (*_delegate_arr[0])("PRESS ENTER KEY TO START ROUND");
        (*_delegate_arr[1])("center");
        (*_delegate_arr[2])(glm::vec2(0.5, 0.49));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(2.8f);
        (*_delegate_arr[5])();
        
        g_CLOCK->UpdateFPS();
        if (g_CLOCK->RecieveFPS()) f_t++;
        
        glfwSwapBuffers(g_WINDOW);
        glfwPollEvents();
        
        if (f_t > 100 && (keycon(KEY_START, KEY_ON) || padcon(PAD_ACTION, KEY_ON)))   st = true;
        if (st        && (keycon(KEY_START, KEY_OFF) || padcon(PAD_ACTION, KEY_OFF))) go = true;
        if (go)
        {
            _curtain -> UpdatePosition(glm::vec3(0,0,0));
            _curtain -> UpdateColor(glm::vec4(1,1,1,1));
            _curtain -> Render();
        }
        
    } while (!go);
    
    
    
}

void OpeningManager::_ProcessPage3()
{
    
    int f_t = 0;
    float curtain_a = 1;
    
    do {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_SCREENWIDTH, g_SCREENHEIGHT);
        
        _page3background -> UpdatePosition(glm::vec3(0,-0.6,0));
        _page3background -> UpdateColor(glm::vec4(1,1,1,1));
        _page3background -> Render();
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        (*_delegate_arr[0])("Geheime Kirche");
        (*_delegate_arr[1])("left");
        (*_delegate_arr[2])(glm::vec2(0.077,  0.8));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(6.2f);
        (*_delegate_arr[6])();
        
        (*_delegate_arr[0])("Berlin, Deutschland");
        (*_delegate_arr[1])("left");
        (*_delegate_arr[2])(glm::vec2(0.077, 0.78));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(2.8f);
        (*_delegate_arr[7])();
        
        (*_delegate_arr[0])("Now loading...");
        (*_delegate_arr[1])("right");
        (*_delegate_arr[2])(glm::vec2(0.90, 0.094));
        (*_delegate_arr[3])(glm::vec3(1, 1, 0.98));
        (*_delegate_arr[4])(2.8f);
        (*_delegate_arr[7])();
        
        g_CLOCK->UpdateFPS();
        if (g_CLOCK->RecieveFPS()) {
            
            f_t++;
            
            if (100 > f_t && f_t >   0) curtain_a -= 1.0f/100.0f;
            if (             f_t > 100) curtain_a  = 0;
            
        }
        
        if (f_t > 30) {
            // Play music
            (*_delegate_arr[8])("opening");
        }
        
        _curtain -> UpdatePosition(glm::vec3(0,0,0));
        _curtain -> UpdateColor(glm::vec4(1,1,1,curtain_a));
        _curtain -> Render();
        
        glfwSwapBuffers(g_WINDOW);
        glfwPollEvents();
        
    } while (f_t < 100);
    
}

void OpeningManager::_ProcessStartGame()
{
    
    int f_t = 0;
    
    if ( g_CLOCK->RecieveFPS() ) {
        f_t = _processstartgamef_t += 0.0004f;
    }
    
    _curtain -> UpdatePosition(glm::vec3(0,0,0));
    _curtain -> UpdateColor(glm::vec4(1,1,1,_processstartgamecurtain_a));
    _curtain -> Render();
    
    if (f_t > 0.78) {
        _processstartgamecurtain_a += 0.01f;
        if (_processstartgamecurtain_a > 1) {
            _processstartgamecurtain_a = 1;
        }
    }else {
        _processstartgamecurtain_a -= 0.01f;
        if (_processstartgamecurtain_a < 0) {
            _processstartgamecurtain_a = 0;
        }
    }
    
}

void OpeningManager::UpdateDelegator(Delegator* delegator)
{
    /**
     * [ 0 ] FontManager::UpdateText(std::string);
     * [ 1 ] FontManager::UpdateAlign(std::string);
     * [ 2 ] FontManager::UpdatePosition(glm::vec2);
     * [ 3 ] FontManager::UpdateColor(glm::vec3);
     * [ 4 ] FontManager::UpdateSize(float);
     * [ 5 ] FontManager::RenderOpenSansRegular();
     * [ 6 ] FontManager::RenderLater();
     * [ 7 ] FontManager::RenderMarlbolo();
     * [ 8 ] AudioManager::PlaySoundOnce(std::string);
     * [ 9 ] AudioManager::PlaySoundStop(std::string);
     */
    _delegate_arr.push_back(delegator);
}
