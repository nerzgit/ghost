//
//  character.h
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/10.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef character_h
#define character_h

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

struct CharacterStructObject
{
    char obj      [256];
};

struct CharacterStructWeight
{
    char wgt      [256];
};

struct CharacterStructShader
{
    char vs       [256];
    char fs       [256];
};

struct CharacterStructTexture
{
    char base     [256];
    char flash    [256];
    char normal   [256];
    char specular [256];
};

struct CharacterStructKeyframe
{
    char action0 [256];
    char action1 [256];
    char action2 [256];
    char action3 [256];
    char action4 [256];
    char action5 [256];
    char action6 [256];
    char action7 [256];
    char action8 [256];
    char action9 [256];
};

struct CharacterStructStatus
{
    int  power;
    int  life;
    int  speed;
};

struct CharacterStructIntel
{
    char dpf      [256];
    char sgf      [256];
};

struct CharacterStructSoundEffect
{
    ALuint se0;
    ALuint se1;
    ALuint se2;
    ALuint se3;
    ALuint se4;
    ALuint se5;
    ALuint se6;
    ALuint se7;
    ALuint se8;
    ALuint se9;
};


#endif /* character_h */
