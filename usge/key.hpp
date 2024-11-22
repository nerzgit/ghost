//
//  key.hpp
//  play
//
//  Created by IDE KENTARO on 2019/06/11.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef key_hpp
#define key_hpp

#include <GLFW/glfw3.h>
#include <vector>

// STATE
#define KEY_ON          1
#define KEY_OFF         0
// PC
#define KEY_CHANGE      0
#define KEY_RELOAD      1
#define KEY_AIM         2
#define KEY_JUMP        3
#define KEY_RUN         4
#define KEY_ACTION      5
#define KEY_FIRE        6
#define KEY_STRAIGHT_F  7
#define KEY_STRAIGHT_B  8
#define KEY_STRAIGHT_R  9
#define KEY_STRAIGHT_L  10
#define KEY_LOOKUP_U    11
#define KEY_LOOKUP_D    12
#define KEY_LOOKUP_R    13
#define KEY_LOOKUP_L    14
#define KEY_START       15
#define KEY_ESC         16
// GAMEPAD
#define PAD_CHANGE      0
#define PAD_RELOAD      1
#define PAD_AIM         2
#define PAD_JUMP        3
#define PAD_RUN         4
#define PAD_ACTION      5
#define PAD_FIRE        6
#define PAD_STRAIGHT_FB 7
#define PAD_STRAIGHT_RL 8
#define PAD_LOOKUP_UD   9
#define PAD_LOOKUP_RL   10
#define PAD_ARROW_U     11
#define PAD_ARROW_D     12
#define PAD_ARROW_R     13
#define PAD_ARROW_L     14
#define PAD_START       15
#define PAD_ESC         16

void         keysetup();

int          pc_key_count();
int          pad_key_count();

bool         keycon(int key, int state);
bool         mousecon(int key, int state);
bool         padcon(int key, int state);
float        stickcon(int key);

bool         joystick_is();

bool         change_key_asset_pc(int key_asset);
bool         change_key_asset_gamepad(int key_asset);
bool         change_key_asset_joystick(int key_asset);

std::string  get_key_name(int key_asset);

void         debug_keyconfig();

#endif /* key_hpp */
