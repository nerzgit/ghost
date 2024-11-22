//
//  characterbuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "characterbuilder.hpp"

O_Character* CharacterDirector::RecieveInitializer(CharacterBuilder* builder)
{
    return builder -> RecieveCharacter();
}

O_Character* CharacterDirector::RecieveCopy(CharacterBuilder* builder)
{
    return builder -> RecieveCopyCharacter();
}

CharacterStructIntel CharacterDirector::RecieveCharacterIntelFile(CharacterBuilder* builder)
{
    return builder -> RecieveCharacterIntelFile();
}


CharacterBuilder::CharacterBuilder(const std::string &path)
{

    // Initialize shader file
    _character_shd_file = {"game/game/sd/Zombie.vertexshader", "game/game/sd/Zombie.fragmentshader"};
    
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
        
        // STATUS
        if ( strcmp( line, "st" ) == 0 ){
            int a,b,c;
            fscanf(file, "%i %i %i\n", &a,&b,&c);
            _character_sts_file = {a,b,c};
        }
        
        // TEXTURE
        else if ( strcmp( line, "gt" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_character_tex_file.base, b) ; break;
                default:                                       break;
            }
        }
        
        // KEYFRAME & OBJ & WEIGHT
        if ( strcmp( line, "kw" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_character_kfm_file.action0, b) ; break;
                case 1 : strcpy(_character_kfm_file.action1, b) ; break;
                case 2 : strcpy(_character_kfm_file.action2, b) ; break;
                case 3 : strcpy(_character_kfm_file.action3, b) ; break;
                case 4 : strcpy(_character_kfm_file.action4, b) ; break;
                case 5 : strcpy(_character_kfm_file.action5, b) ; break;
                case 6 : strcpy(_character_kfm_file.action6, b) ; break;
                case 7 : strcpy(_character_kfm_file.action7, b) ; break;
                case 8 : strcpy(_character_kfm_file.action8, b) ; break;
                case 9 : strcpy(_character_kfm_file.action9, b) ; break;
                default:                                          break;
            }
        }
        else if ( strcmp( line, "gw" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            strcpy(_character_wgt_file.wgt, b);
        }
        else if ( strcmp( line, "ow" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            strcpy(_character_obj_file.obj, b);
        }
        
        // DIJKSTRA & SEGMENT
        else if ( strcmp( line, "dp" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_character_ite_file.dpf, b) ; break;
                default:                                      break;
            }
        }
        else if ( strcmp( line, "sg" ) == 0 ){
            int a; char b[256];
            fscanf(file, "%i %s\n", &a, b);
            switch (a) {
                case 0 : strcpy(_character_ite_file.sgf, b) ; break;
                default:                                      break;
            }
        }
        
        else {}
        
        
    }
    fclose(file);
}

CharacterBuilder::~CharacterBuilder()
{
    delete _o_character;
}

CharacterStructIntel CharacterBuilder::RecieveCharacterIntelFile()
{
    return _character_ite_file;
}
