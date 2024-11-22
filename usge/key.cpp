//
//  key.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/11.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "key.hpp"
#include <iostream>

std::vector<int> key_asset_pc;
std::vector<int> key_asset_pad;

extern GLFWwindow* g_WINDOW;

int present_joystick;

void keysetup()
{
    
    // Default key
    key_asset_pc  = {
    /* KEY_CHANGE       */     GLFW_KEY_Q,
    /* KEY_RELOAD       */     GLFW_KEY_R,
    /* KEY_AIM          */     GLFW_KEY_TAB,
    /* KEY_JUMP         */     GLFW_KEY_SPACE,
    /* KEY_RUN          */     GLFW_KEY_LEFT_SHIFT,
    /* KEY_ACTION       */     GLFW_KEY_E,
    /* KEY_FIRE         */     GLFW_MOUSE_BUTTON_1,
    /* KEY_STRAIGHT_F   */     GLFW_KEY_W,
    /* KEY_STRAIGHT_B   */     GLFW_KEY_S,
    /* KEY_STRAIGHT_R   */     GLFW_KEY_D,
    /* KEY_STRAIGHT_L   */     GLFW_KEY_A,
    /* KEY_LOOKUP_U     */     GLFW_KEY_UP,
    /* KEY_LOOKUP_D     */     GLFW_KEY_DOWN,
    /* KEY_LOOKUP_R     */     GLFW_KEY_RIGHT,
    /* KEY_LOOKUP_L     */     GLFW_KEY_LEFT,
    /* KEY_START        */     GLFW_KEY_ENTER,
    /* KEY_ESC          */     GLFW_KEY_ESCAPE,
    };
    key_asset_pad = {
    /* PAD_CHANGE       */     GLFW_GAMEPAD_BUTTON_Y,
    /* PAD_RELOAD       */     GLFW_GAMEPAD_BUTTON_A,
    /* PAD_AIM          */     GLFW_GAMEPAD_AXIS_RIGHT_Y,
    /* PAD_JUMP         */     GLFW_GAMEPAD_BUTTON_B,
    /* PAD_RUN          */     GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    /* PAD_ACTION       */     GLFW_GAMEPAD_BUTTON_X,
    /* PAD_FIRE         */     GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    /* PAD_STRAIGHT_FB  */     GLFW_GAMEPAD_AXIS_LEFT_Y,
    /* PAD_STRAIGHT_RL  */     GLFW_GAMEPAD_AXIS_LEFT_X,
    /* PAD_LOOKUP_UD    */     GLFW_GAMEPAD_AXIS_RIGHT_X,
    /* PAD_LOOKUP_RL    */     GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    /* PAD_ARROW_U      */     GLFW_GAMEPAD_BUTTON_DPAD_UP,
    /* PAD_ARROW_D      */     GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    /* PAD_ARROW_R      */     GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    /* PAD_ARROW_L      */     GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
    /* PAD_START        */     13,
    /* PAD_ESC          */     8,
    };
    present_joystick = -1;
    
    
    
    // Initialization of Gamepad
    
    // Find presented joystick
    for (int i=0; i<16; i++) {
        if (glfwJoystickPresent(i)) {
            present_joystick = i;
            break;
        }
    }
    
    // Joystick name
    if (present_joystick != -1) {
        const char *name = glfwGetJoystickName(present_joystick);
        std::cout << name << std::endl;
    }
    
}


int pc_key_count()
{
    return (int)key_asset_pc.size();
}

int pad_key_count()
{
    return (int)key_asset_pad.size();
}

bool keycon(int key, int state)
{
    // KEY_ON is true
    if (state == 1) {
        if (glfwGetKey(g_WINDOW, key_asset_pc[key])) return 1;
        // KEY_OFF is true
    }else if (state == 0) {
        if (!glfwGetKey(g_WINDOW, key_asset_pc[key])) return 1;
    }
    return 0;
}

bool mousecon(int key, int state)
{
    // KEY_ON is true
    if (state == 1) {
        if (glfwGetMouseButton(g_WINDOW, key_asset_pc[key])) return 1;
        // KEY_OFF is true
    }else if (state == 0) {
        if (!glfwGetMouseButton(g_WINDOW, key_asset_pc[key])) return 1;
    }
    return 0;
}

bool padcon(int key, int state)
{
    // If joystick available
    if (present_joystick != -1) {
        // Get gamepad button assets
        int count;
        const unsigned char *btn = glfwGetJoystickButtons( present_joystick, &count );
        // KEY_ON is true
        if (state == 1) {
            if (btn[key_asset_pad[key]]) return 1;
        // KEY_OFF is true
        }else if (state == 0) {
            if (!btn[key_asset_pad[key]]) return 1;
        }
    }
    return 0;
}

float stickcon(int key)
{
    if (present_joystick != -1) {
        int count;
        const float *axes = glfwGetJoystickAxes( present_joystick, &count );
        return axes[key_asset_pad[key]];
    }
    return 0;
}

bool joystick_is()
{
    if (present_joystick != -1) return true;
    else return false;
}

bool change_key_asset_pc(int key_asset)
{
    // Check all key
    for (int i=0; i<349; i++) {
        if (glfwGetKey(g_WINDOW, i) && i != GLFW_KEY_ENTER && i != GLFW_KEY_ESCAPE) {
            key_asset_pc[key_asset] = i;
            return true;
        }
    }
    return false;
}

bool change_key_asset_gamepad(int key_asset)
{
    // If joystick available
    if (present_joystick > -1) {
        // Get gamepad button assets
        int count;
        const unsigned char *btn = glfwGetJoystickButtons( present_joystick, &count );
        // Check all pad buttons
        for (int i=0; i<15; i++) {
            if (btn[i])
            {
                key_asset_pad[key_asset - 17] = i;
                return true;
            }
        }
    }
    return false;
}

bool change_key_asset_joystick(int key_asset)
{
    // If joystick available
    if (present_joystick > -1) {
        int count;
        const float *axes = glfwGetJoystickAxes( present_joystick, &count );
        // Check all joystick
        for (int i=0; i<6; i++) {
            if (axes[i] > 0.9) {
                key_asset_pad[key_asset - 17] = i;
                return true;
            }
        }
    }
    return false;
}

std::string get_key_name(int key_asset)
{
    
    int i = key_asset;
    
    if (-1 < i && i < 17) {
        // :: PC key asset ::[
        if (glfwGetKeyName(key_asset_pc[key_asset], 0) == nullptr) {
            return ("KEY_" + std::to_string(key_asset_pc[key_asset]));
        }else {
            return std::string(glfwGetKeyName(key_asset_pc[key_asset], 0));
        }
    }else if (i == 19) {
        // :: JOYSTICK key asset (Aim) ::
        return ("JOYSTICK_" + std::to_string(key_asset_pad[key_asset - 17]));
    }else if (i == 23) {
        // :: JOYSTICK key asset (Fire) ::
        return ("JOYSTICK_" + std::to_string(key_asset_pad[key_asset - 17]));
    }else if (23 < i && i < 28) {
        // :: JOYSTICK key asset ::
        return ("JOYSTICK_" + std::to_string(key_asset_pad[key_asset - 17]));
    }else{
        // :: GAMEPAD key asset ::
        return ("GAMEPAD_" + std::to_string(key_asset_pad[key_asset - 17]));
    }
    
}

void debug_keyconfig()
{
    
    std::cout << "\n" << std::endl;

    // Mouse & Keyboard
    std::cout << "KEY_CHANGE      " << keycon(KEY_CHANGE     , KEY_ON) << std::endl;
    std::cout << "KEY_RELOAD      " << keycon(KEY_RELOAD     , KEY_ON) << std::endl;
    std::cout << "KEY_AIM         " << keycon(KEY_AIM        , KEY_ON) << std::endl;
    std::cout << "KEY_JUMP        " << keycon(KEY_JUMP       , KEY_ON) << std::endl;
    std::cout << "KEY_RUN         " << keycon(KEY_RUN        , KEY_ON) << std::endl;
    std::cout << "KEY_ACTION      " << keycon(KEY_ACTION     , KEY_ON) << std::endl;
    std::cout << "KEY_FIRE        " << keycon(KEY_FIRE       , KEY_ON) << std::endl;
    std::cout << "KEY_STRAIGHT_F  " << keycon(KEY_STRAIGHT_F , KEY_ON) << std::endl;
    std::cout << "KEY_STRAIGHT_B  " << keycon(KEY_STRAIGHT_B , KEY_ON) << std::endl;
    std::cout << "KEY_STRAIGHT_R  " << keycon(KEY_STRAIGHT_R , KEY_ON) << std::endl;
    std::cout << "KEY_STRAIGHT_L  " << keycon(KEY_STRAIGHT_L , KEY_ON) << std::endl;
    std::cout << "KEY_LOOKUP_U    " << keycon(KEY_LOOKUP_U   , KEY_ON) << std::endl;
    std::cout << "KEY_LOOKUP_D    " << keycon(KEY_LOOKUP_D   , KEY_ON) << std::endl;
    std::cout << "KEY_LOOKUP_R    " << keycon(KEY_LOOKUP_R   , KEY_ON) << std::endl;
    std::cout << "KEY_LOOKUP_L    " << keycon(KEY_LOOKUP_L   , KEY_ON) << std::endl;
    std::cout << "KEY_START       " << keycon(KEY_START      , KEY_ON) << std::endl;
    std::cout << "KEY_ESC         " << keycon(KEY_ESC        , KEY_ON) << std::endl;

    // Gamepad
    std::cout << "PAD_CHANGE      " << padcon(PAD_CHANGE      , KEY_ON) << std::endl;
    std::cout << "PAD_RELOAD      " << padcon(PAD_RELOAD      , KEY_ON) << std::endl;
    std::cout << "PAD_AIM         " << stickcon(PAD_AIM)                << std::endl;
    std::cout << "PAD_JUMP        " << padcon(PAD_JUMP        , KEY_ON) << std::endl;
    std::cout << "PAD_RUN         " << padcon(PAD_RUN         , KEY_ON) << std::endl;
    std::cout << "PAD_ACTION      " << padcon(PAD_ACTION      , KEY_ON) << std::endl;
    std::cout << "PAD_FIRE        " << stickcon(PAD_FIRE)               << std::endl;
    std::cout << "PAD_STRAIGHT_FB " << stickcon(PAD_STRAIGHT_FB)        << std::endl;
    std::cout << "PAD_STRAIGHT_RL " << stickcon(PAD_STRAIGHT_RL)        << std::endl;
    std::cout << "PAD_LOOKUP_UD   " << stickcon(PAD_LOOKUP_UD  )        << std::endl;
    std::cout << "PAD_LOOKUP_RL   " << stickcon(PAD_LOOKUP_RL  )        << std::endl;
    std::cout << "PAD_ARROW_U     " << padcon(PAD_ARROW_U     , KEY_ON) << std::endl;
    std::cout << "PAD_ARROW_D     " << padcon(PAD_ARROW_D     , KEY_ON) << std::endl;
    std::cout << "PAD_ARROW_R     " << padcon(PAD_ARROW_R     , KEY_ON) << std::endl;
    std::cout << "PAD_ARROW_L     " << padcon(PAD_ARROW_L     , KEY_ON) << std::endl;
    std::cout << "PAD_START       " << padcon(PAD_START       , KEY_ON) << std::endl;
    std::cout << "PAD_ESC         " << padcon(PAD_ESC         , KEY_ON) << std::endl;

    
}

