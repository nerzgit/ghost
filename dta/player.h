//
//  player.h
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef player_h
#define player_h

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

struct PlayerStructObject
{
    char obj      [256];
};

struct PlayerStructWeight
{
    char wgt      [256];
};

struct PlayerStructShader
{
    char vs       [256];
    char fs       [256];
};

struct PlayerStructTexture
{
    char base     [256];
    char flash    [256];
    char normal   [256];
    char specular [256];
};

struct PlayerStructKeyframe
{
    char pose     [256];
    char fire     [256];
    char reload   [256];
    char aim      [256];
    char snip     [256];
    char change   [256];
};

struct PlayerStructStatus
{
    int  magazinemaxammo;
    int  magazineammo;
    int  extraammo;
    int  extramaxammo;
    float muzzleflashlengthy;
    float muzzleflashlengthz;
    float ballisticlengthy;
    float ballisticlengthz;
    char name   [256];
};

struct PlayerStructControl
{
    bool single;
    int  f_ActiveFrame;
    int  f_Rate;
    int  f_LastFrame;
    int  f_SoundFrame;
    int  f_StartAttackFrame;
    int  f_EndAttackFrame;
    int  f_StartFlashFrame;
    int  f_EndFlashFrame;
    int  f_StartMuzzleFlashFrame;
    int  f_EndMuzzleFlashFrame;
    int  r_LastFrame;
    int  r_SoundFrame;
    int  a_StartFrame;
    int  a_EndFrame;
    int  s_StartFrame;
    int  s_EndFrame;
    int  c_StartFrame;
    int  c_EndFrame;
    int  c_SoundFrame;
};

struct PlayerStructSoundEffect
{
    char se_Fire   [256];
    char se_Reload [256];
    char se_Change [256];
    char se_Aim    [256];
};


#endif /* player_h */
