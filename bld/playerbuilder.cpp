//
//  playerbuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "playerbuilder.hpp"
#include <stdio.h>

O_Player* PlayerDirector::RecieveInitializer(PlayerBuilder* builder)
{
    builder -> UpdateInitializer();
    return builder -> RecievePlayer();
}

PlayerBuilder::PlayerBuilder(const std::string &path)
{
    
    // Initialize shader file
    _player_shd_file = {"game/game/sd/Player_Arm_ShadowMapping.vertexshader","game/game/sd/Player_Arm_ShadowMapping.fragmentshader"};
    
    //
    // Load file and create weapon struct data for initialization.
    //
    
    // Initialization of chracter assets
    FILE * file = fopen(path.c_str(), "r");
    
    while( 1 ){
        
        char  line[128];
        
        int res = fscanf(file, "%s", line);
        
        // EOF
        if (res == EOF)
            break;

        // TEXTURE [0]->Rightarm [1]->Leftarm
        else if ( strcmp( line, "g" ) == 0 ){
            PlayerStructTexture structtexture;
            _player_tex_file.push_back(structtexture);
        }
        else if ( strcmp( line, "gt" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_player_tex_file[_player_tex_file.size()-1].base,     b) ; break;
                case 1 : strcpy(_player_tex_file[_player_tex_file.size()-1].flash,    b) ; break;
                case 2 : strcpy(_player_tex_file[_player_tex_file.size()-1].normal,   b) ; break;
                case 3 : strcpy(_player_tex_file[_player_tex_file.size()-1].specular, b) ; break;
                default:                                                                   break;
            }
        }
        
        // WEIGHT & OBJ [0~1]->Rightarm [2~3]->Leftarm
        else if ( strcmp( line, "kw" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            PlayerStructWeight structweight;
            _player_wgt_file.push_back(structweight);
            strcpy(_player_wgt_file[_player_wgt_file.size()-1].wgt,b);
        }
        else if ( strcmp( line, "ow" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            PlayerStructObject structobject;
            _player_obj_file.push_back(structobject);
            strcpy(_player_obj_file[_player_obj_file.size()-1].obj,b);
        }
            
    }
        
        
}

PlayerBuilder::~PlayerBuilder()
{
    delete _o_player;
}

void PlayerBuilder::UpdateKeyframeFile(std::vector<PlayerStructKeyframe> &kfm_file)
{
    for (auto e : kfm_file) {
        _player_kfm_file.push_back(e);
    }
}
