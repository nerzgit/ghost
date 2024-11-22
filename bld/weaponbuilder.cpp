//
//  weaponbuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "weaponbuilder.hpp"
#include <stdio.h>

O_Weapon* WeaponDirector::RecieveInitializer(WeaponBuilder* builder)
{
    return builder -> RecieveWeapon();
}

std::vector<PlayerStructKeyframe> WeaponDirector::RecievePlayerKeyFrameFile(WeaponBuilder* builder)
{
    return builder -> RecievePlayerKeyFrameFile();
}

WeaponBuilder::WeaponBuilder(const std::string &path)
{
    
    // Initialize shader file
    _weapon_shd_file = {"game/game/sd/Player_Weapon_ShadowMapping.vertexshader", "game/game/sd/Player_Weapon_ShadowMapping.fragmentshader"};
    
    //
    // Load file and create weapon struct data for initialization.
    //
    
    // Initialization of Weapon assets
    FILE * file = fopen(path.c_str(), "r");
    
    while( 1 ){
        
        char  line[128];
        
        int res = fscanf(file, "%s", line);
        
        // EOF
        if (res == EOF)
            break;
        
        // CONTROL (FOR FRAME CONTROL)
        if ( strcmp( line, "c" ) == 0 ){
            int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t;
            fscanf(file, "%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n",
                   &a,&b,&c,&d,&e,&f,&g,&h,&i,&j,&k,&l,&m,&n,&o,&p,&q,&r,&s,&t);
            _weapon_ctl_file = {(bool)a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t};
        }
        
        // STATUS (FOR WEAPON)
        if ( strcmp( line, "st" ) == 0 ){
            int a,b,c,d;
            float e,g,h,i;
            char f[256];
            fscanf(file, "%i %i %i %i %f %f %f %f %s\n", &a,&b,&c,&d,&e,&g,&h,&i,f);
            _weapon_sts_file = {a,b,c,d,e,g,h,i,"None"};
            strcpy(_weapon_sts_file.name,f);
        }
        
        // TEXTURE (FOR WEAPON)
        else if ( strcmp( line, "tg" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_weapon_tex_file.base,     b) ; break;
                case 1 : strcpy(_weapon_tex_file.flash,    b) ; break;
                case 2 : strcpy(_weapon_tex_file.normal,   b) ; break;
                case 3 : strcpy(_weapon_tex_file.specular, b) ; break;
                default:                                        break;
            }
        }
        
        // KEYFRAME & OBJ (FOR WEAPON)
        else if ( strcmp( line, "w" ) == 0 ){
            PlayerStructKeyframe structkeyframe;
            _weapon_kfm_file.push_back(structkeyframe);
        }
        else if ( strcmp( line, "kw" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_weapon_kfm_file[_weapon_kfm_file.size()-1].pose,   b) ; break;
                case 1 : strcpy(_weapon_kfm_file[_weapon_kfm_file.size()-1].fire,   b) ; break;
                case 2 : strcpy(_weapon_kfm_file[_weapon_kfm_file.size()-1].reload, b) ; break;
                case 3 : strcpy(_weapon_kfm_file[_weapon_kfm_file.size()-1].aim,    b) ; break;
                case 4 : strcpy(_weapon_kfm_file[_weapon_kfm_file.size()-1].snip,   b) ; break;
                case 5 : strcpy(_weapon_kfm_file[_weapon_kfm_file.size()-1].change, b) ; break;
                default:                                                                 break;
            }
        }
        else if ( strcmp( line, "ow" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            PlayerStructObject structobject;
            strcpy(structobject.obj, b);
            _weapon_obj_file.push_back(structobject);
        }
        
        // KEYFRAME (FOR PLAYER ARMS ANIMATION)
        else if ( strcmp( line, "a" ) == 0 ){
            PlayerStructKeyframe structkeyframe;
            _player_kfm_file.push_back(structkeyframe);
        }
        else if ( strcmp( line, "ka" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_player_kfm_file[_player_kfm_file.size()-1].pose,   b) ; break;
                case 1 : strcpy(_player_kfm_file[_player_kfm_file.size()-1].fire,   b) ; break;
                case 2 : strcpy(_player_kfm_file[_player_kfm_file.size()-1].reload, b) ; break;
                case 3 : strcpy(_player_kfm_file[_player_kfm_file.size()-1].aim,    b) ; break;
                case 4 : strcpy(_player_kfm_file[_player_kfm_file.size()-1].snip,   b) ; break;
                case 5 : strcpy(_player_kfm_file[_player_kfm_file.size()-1].change, b) ; break;
                default:                                                                 break;
            }
        }
        
        // SOUNDEFFECT (FOR WEAPON)
        else if ( strcmp( line, "se" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_weapon_sfx_file.se_Fire,   b) ; break;
                case 1 : strcpy(_weapon_sfx_file.se_Reload, b) ; break;
                case 2 : strcpy(_weapon_sfx_file.se_Aim,    b) ; break;
                case 3 : strcpy(_weapon_sfx_file.se_Change, b) ; break;
                default:                                         break;
            }
        }
        
        else {}
        
        
    }
    fclose(file);
    
}

WeaponBuilder::~WeaponBuilder()
{
    delete _o_weapon;
}

std::vector<PlayerStructKeyframe> WeaponBuilder::RecievePlayerKeyFrameFile()
{
    return _player_kfm_file;
}
