//
//  playermanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/05.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "playermanager.hpp"
#include "del/delegate.hpp"
#include "obj/o_weapon.hpp"
#include "bld/weaponbuilder.hpp"
#include "bld/weapon/waltherp38builder.hpp"
#include "bld/weapon/akmbuilder.hpp"
#include "bld/weapon/perkbuilder.hpp"
#include "obj/o_player.hpp"
#include "bld/playerbuilder.hpp"
#include "bld/player/ManBuilder.hpp"
#include "deco/weapondecorator.hpp"
#include "deco/weapon/ballisticdecorator.hpp"
#include "deco/weapon/muzzleflashdecorator.hpp"
#include "util/randomizer.hpp"
#include "util/squareroot.hpp"
#include "usge/key.hpp"
#include "usge/clock.hpp"
#include "usge/quaternion_utils.hpp"




PlayerManager::PlayerManager()
{
    
    _play                       = true;
    _render                     = true;
    _vbo0                       = 0;
    _vbo1                       = 0;
    _playerarrnumber            = 0;
    _weaponarrnumber            = 0;
    _score                      = 500;
    _life                       = 1000;
    _maxlife                    = 1000;
    _dead                       = false;
    _power                      = 40;
    _shoot                      = false;
    _fov                        = 80;
    _actionaim                  =
    _actionsnip                 =
    _actionreload               =
    _actionfire                 =
    _actionchange               =
    _actionrun                  =
    _actiondrink                =
    _usingdrinkchange           =
    _usingrandomweapon          = false;
    _valanceposition            =
    _currentposition            =
    _lastposition               =
    _scaling                    = glm::vec3(0);
    _walkingshakerotationmatrix =
    _runningshakerotationmatrix = glm::mat4(0);
    
    // Initialize weaponasset (We can have 3 weapons)
    _weaponasset = {0,-1,-1};
    _weaponassetarrnumber = 0;
    _lastweaponassetarrnumber = 0;

    
}

PlayerManager::~PlayerManager()
{
    for (auto e : _delegator_arr          ) delete e;
    for (auto e : _weapon_arr             ) delete e;
    for (auto e : _player_arr             ) delete e;
    for (auto e : _playerbuilder_arr      ) delete e;
    for (auto e : _weaponbuilder_arr      ) delete e;
    for (auto e : _weapon_ballistic_arr   ) delete e;
    for (auto e : _weapon_muzzleflash_arr ) delete e;
}




/* After updated delegator */
void PlayerManager::Init()
{
    
    // Create player and weapon
    _InitializePlayer();
    
    PlayerStructStatus out_struct;
    _weapon_arr[1]->RecieveStructStatus(out_struct);
    
    // Create decorator
    _InitializeDecorator();
    
    _ProcessLinkingAllStatus();
    
}




/* ReInitialize */
void PlayerManager::Reset()
{
    
    _play                       = true;
    _render                     = true;
    _vbo0                       = 0;
    _vbo1                       = 0;
    _playerarrnumber            = 0;
    _weaponarrnumber            = 0;
    _score                      = 500;
    _life                       = 1000;
    _maxlife                    = 1000;
    _dead                       = false;
    _power                      = 40;
    _shoot                      = false;
    _fov                        = 80;
    _actionaim                  =
    _actionsnip                 =
    _actionreload               =
    _actionfire                 =
    _actionchange               =
    _actionrun                  =
    _actiondrink                =
    _usingdrinkchange           =
    _usingrandomweapon          = false;
    _valanceposition            =
    _currentposition            =
    _lastposition               =
    _scaling                    = glm::vec3(0);
    _walkingshakerotationmatrix =
    _runningshakerotationmatrix = glm::mat4(0);
    
    _weaponasset = {0,-1,-1};
    _weaponassetarrnumber = 0;
    _lastweaponassetarrnumber = 0;
    
    _ProcessLinkingAllStatus();
    
    _weapon_arr[_weaponarrnumber]->UpdateMagazineAmmo(_weaponstatus.magazinemaxammo);
    _weapon_arr[_weaponarrnumber]->UpdateExtraAmmo(_weaponstatus.extramaxammo);
    
}






/* In Rendering */
void PlayerManager::Render()
{
    
    // Privent penetrate
    glClear(GL_DEPTH_BUFFER_BIT);
    
    
    
    // Zero life, True dead
    if (_life < 1) {
        _dead = true;
        _life = 0;
    }
    // True dead, Zero life
    if (_dead) {
        _life = 0;
    }
    // Always increase life
    if (_maxlife > _life && _life > 0) {
        _life += 1;
    }
    // Using _weaponasset number when not drinking
    if (!_usingdrinkchange) {
        _weaponarrnumber = _weaponasset[_weaponassetarrnumber];
    }
    
    
    
    // Syncronize
    _currentposition = (*_delegator_arr[3])(glm::vec3(0));
    _lastposition    = (*_delegator_arr[4])(glm::vec3(0));
    _orientation     = (*_delegator_arr[5])(glm::vec3(0));
    _valanceposition = (*_delegator_arr[6])(glm::vec3(0));
    _condition       = (*_delegator_arr[7])(glm::vec3(0));
    
    // Create render position
    _renderposition = (_valanceposition + _condition + _currentposition);
    
    // Muzzle flash
    (*_delegator_arr[2])(glm::vec3(99999,99999,99999));
    _player_arr[_playerarrnumber]->UpdateMuzzleFlashPosition(glm::vec3(99999,99999,99999));
    _weapon_arr[_weaponarrnumber]->UpdateMuzzleFlashPosition(glm::vec3(99999,99999,99999));
    
    
    if (_play) {

    // Action controls
    _ProcessActionRun();
    _ProcessActionFire();
    _ProcessActionAim();
    _ProcessActionSnip();
    _ProcessActionReload();
    _ProcessActionChange();
    _ProcessActionDrink();
        
    }
    
    
    // Create MVP matrix
    _scaling = glm::vec3(1.0,1.0,1.0);
    glm::mat4 rotationmatrix = glm::eulerAngleYXZ(_orientation.y+3.1415926535f, _orientation.x, _orientation.z);
    glm::mat4 translationmatrix = translate(glm::mat4(1.0),  _renderposition);
    glm::mat4 scalingmatrix = scale(glm::mat4(1.0), _scaling);
    _modelmatrix = translationmatrix * rotationmatrix * _walkingshakerotationmatrix * _runningshakerotationmatrix * scalingmatrix;
    
    
    if (_render) {
        
    // Create light position
    _lightposition = glm::vec3(12.0,12.0,-8.0);
        
    // Player rendering
    _player_arr[_playerarrnumber]->UpdateLightPosition(_lightposition);
    _player_arr[_playerarrnumber]->UpdateSkinMeshAnimation(_weaponarrnumber, _vbo0, _vbo1);
    _player_arr[_playerarrnumber]->UpdateModelMatrix(_modelmatrix);
    _player_arr[_playerarrnumber]->Render();
    
    // Weapon rendering
    _weapon_arr[_weaponarrnumber]->UpdateLightPosition(_lightposition);
    _weapon_arr[_weaponarrnumber]->UpdateSkinMeshAnimation(_vbo0, _vbo1);
    _weapon_arr[_weaponarrnumber]->UpdateConstraintMatrix(_player_arr[_playerarrnumber]->RecieveWorldBoneMatrix(0, 2));
    _weapon_arr[_weaponarrnumber]->UpdateModelMatrix(_modelmatrix);
    _weapon_arr[_weaponarrnumber]->Render();
        
    }
    
}





void PlayerManager::_InitializePlayer()
{
    
    
    
    /* Generate player */
    PlayerDirector playerdirector;
    PlayerBuilder *manbuilder = new ManBuilder("game/game/as/player/player00.pa");
    
    

    /* Initialize weapon and Combine Player and weapon */
    
    // Director Cls for all
    WeaponDirector weapondirector;
    
    // Waltherp38
    // Create weapon builder with file path
    WeaponBuilder *waltherp38builder = new WaltherP38Builder("game/game/as/weapon/waltherp38.wa");
    // Initialized O_Weapon instance by Builder.
    O_Weapon *waltherp38 = weapondirector.RecieveInitializer(waltherp38builder);
    // Pushback to array
    _weaponbuilder_arr.push_back(waltherp38builder);
    _weapon_arr.push_back(waltherp38);
    // Add all keyframe data for animate with player.
    std::vector<PlayerStructKeyframe> waltherp38keyframe = weapondirector.RecievePlayerKeyFrameFile(waltherp38builder);
    manbuilder->UpdateKeyframeFile(waltherp38keyframe);
    
    
    
    // AKM
    // Create weapon builder with file path
    WeaponBuilder *akmbuilder = new AKMBuilder("game/game/as/weapon/akm.wa");
    // Initialized O_Weapon instance by Builder.
    O_Weapon *akm = weapondirector.RecieveInitializer(akmbuilder);
    // Pushback to array
    _weaponbuilder_arr.push_back(akmbuilder);
    _weapon_arr.push_back(akm);
    // Add all keyframe data for animate with player.
    std::vector<PlayerStructKeyframe> akmkeyframe = weapondirector.RecievePlayerKeyFrameFile(akmbuilder);
    manbuilder->UpdateKeyframeFile(akmkeyframe);
    
    
    
    // MAC11
    // Create weapon builder with file path
    WeaponBuilder *mac11builder = new AKMBuilder("game/game/as/weapon/mac11.wa");
    // Initialized O_Weapon instance by Builder.
    O_Weapon *mac11 = weapondirector.RecieveInitializer(mac11builder);
    // Pushback to array
    _weaponbuilder_arr.push_back(mac11builder);
    _weapon_arr.push_back(mac11);
    // Add all keyframe data for animate with player.
    std::vector<PlayerStructKeyframe> mac11keyframe = weapondirector.RecievePlayerKeyFrameFile(mac11builder);
    manbuilder->UpdateKeyframeFile(mac11keyframe);
    
    
    
    // PERK
    // Create weapon builder with file path
    WeaponBuilder *perkbuilder = new PerkBuilder("game/game/as/weapon/perk.wa");
    // Initialized O_Weapon instance by Builder.
    O_Weapon *perk = weapondirector.RecieveInitializer(perkbuilder);
    // Pushback to array
    _weaponbuilder_arr.push_back(perkbuilder);
    _weapon_arr.push_back(perk);
    // Add all keyframe data for animate with player.
    std::vector<PlayerStructKeyframe> perkkeyframe = weapondirector.RecievePlayerKeyFrameFile(perkbuilder);
    manbuilder->UpdateKeyframeFile(perkkeyframe);
    
    
    
    /* Initialize Player */
    O_Player *man = playerdirector.RecieveInitializer(manbuilder);
    _playerbuilder_arr.push_back(manbuilder);
    _player_arr.push_back(man);
    
    
    
}





void PlayerManager::_InitializeDecorator()
{
    
    /* Initialize weapon decorator */
    for (int i=0; i<_weapon_arr.size(); i++) {
        
        // !!PERK have no status!!
        if (i == 3) continue;
        
        // Ballistic
        WeaponDecorator* ballisticdecorator = new BallisticDecorator("game/game/oj/effect/obj/ballistic.obj","game/game/oj/effect/tex/ballistic_o.dds");
        ballisticdecorator->UpdateDecorator(_weapon_arr[i]);
        _weapon_ballistic_arr.push_back(ballisticdecorator);
        
        // Muzzleflash
        WeaponDecorator* muzzleflashdecorator = new MuzzleFlashDecorator("game/game/oj/effect/obj/waltherp38_muzzle.obj","game/game/oj/effect/tex/waltherp38_muzzle.dds");
        muzzleflashdecorator->UpdateDecorator(_weapon_arr[i]);
        _weapon_muzzleflash_arr.push_back(muzzleflashdecorator);
        
    }
    
}







void PlayerManager::_ProcessActionRun()
{

    if (
        (
         (
          keycon(KEY_RUN, KEY_ON)         &&
          keycon(KEY_STRAIGHT_F, KEY_ON)  &&
          keycon(KEY_STRAIGHT_B, KEY_OFF) &&
          keycon(KEY_STRAIGHT_R, KEY_OFF) &&
          keycon(KEY_STRAIGHT_L, KEY_OFF)
          ) || (
          padcon(PAD_RUN, KEY_ON)         &&
          stickcon(PAD_STRAIGHT_FB)    <-0.8
          )
         ) &&
        !_usingdrinkchange &&
        !_actionfire
        ) {
        _actionfire = false;
        _actionrun = true;
    }else{
        _actionrun = false;
    }
    
    
    static glm::quat walkingrotatetowards;
    static glm::vec3 runnningorientation;
    
    // These code implement shaking arms. (Like a callofdutyBO3)
    if ( g_CLOCK->RecieveFPS() ){
        
        glm::vec3 cp = _currentposition;
        glm::vec3 lp = _lastposition;
        
        // Difference position from the previous frame
        float diffuse = SquareRoot(
                                   (cp.x-lp.x)*(cp.x-lp.x)+
                                   (cp.z-lp.z)*(cp.z-lp.z)
                                   );
        
        // 1: Gun-Arms shaking with waling
        int intdiffuse = int(diffuse*100.0);
        static float walkingamount1 = 0;  // Shaking tunner 1
        static float walkingamount2 = 0;  // Shaking tunner 2
        
        // Level4
        if       (  0 < intdiffuse && intdiffuse < 20) {
            walkingamount1 = 0.0f;
            if (walkingamount2 > 1) {
                walkingamount2 += 0;
            }else{
                walkingamount2 += 0.1f;
            }
        
        // Level3
        }else if (300 < intdiffuse && intdiffuse < 600) {
            walkingamount1 = 0.0f;
            if (walkingamount2 > 5) {
                walkingamount2 += 0;
            }else{
                walkingamount2 += 0.006f;
            }
            
        // Level2
        }else if (600 < intdiffuse && intdiffuse < 900) {
            walkingamount1 = 0.0f;
            if (walkingamount2 > 10) {
                walkingamount2 += 0;
            }else{
                walkingamount2 += 0.016f;
            }
            
        // Level1
        }else{
            if (walkingamount2 <= 0) {
                walkingamount2 = 0;
                walkingamount1 = 25.0f;
            }else{
                walkingamount2 -= 0.2f;
            }
        }
        
        static int p = 0;
        float      a = 1.0f/(walkingamount2+0.5f);
        float      t = 18.0f+walkingamount1;
        float      m = t/2.0f;
        
        p++;
        
        glm::vec3 orientation = glm::vec3(
                                          -sin(p/(m/2.0f))/(120.0f*a),
                                          -sin(p/m)       /(120.0f*a),
                                          -cos(p/m)       /(120.0f*a)
                                          );
        glm::quat walkingnormal = glm::normalize(LookAt(glm::vec3(0.0f, 0.0f, 0.0f), cp));
        glm::quat walkingquarternion = glm::quat(0,orientation);
        walkingrotatetowards = RotateTowards(walkingquarternion, walkingnormal, 1.0f);
        
        // 2: Arms rotating on running
        static float runningamount;
        if       (_actionrun && runningamount < 15.0f) {
            runningamount += 1.0f;
            if (runningamount > 20.0f) {
                runningamount = 20.0f;
            }
        }else if (!_actionrun && runningamount > 0.0f) {
            runningamount -= 1.0f;
            if (runningamount < 0.0f) {
                runningamount = 0.0f;
            }
        }
        runnningorientation = glm::vec3(
                                        (-(runningamount * 3.14f) / 180.0f * 1.6f + (runningamount/180.0f)),
                                        ((runningamount * sin(p/5.98f) * 3.14f) / 180.0f * 0.8f + (runningamount/30.0f)),
                                        ((runningamount * 3.14f) / 180.0f * 0.4f)
                                        );
        
    }
    
    _walkingshakerotationmatrix = glm::mat4_cast(walkingrotatetowards);
    _runningshakerotationmatrix = glm::eulerAngleYXZ(runnningorientation.y, runnningorientation.x, runnningorientation.z);
    
}






void PlayerManager::_ProcessActionAim()
{
    
    // Activate Aiming
    if ((keycon(KEY_AIM, KEY_ON) || stickcon(PAD_AIM) > 0.6) &&
        !_actionsnip     &&
        !_actionreload   &&
        !_actionfire     &&
        !_actionchange   &&
        !_actionrun      &&
        !_usingdrinkchange
        )
    {
        _ProcessActionReset();
        _actionrun = false;
        _actionaim = true;
    }
    
    if ( _actionaim && g_CLOCK->RecieveFPS() ) {
        _vbo0 = 3;
        // Release, decrease aim frame
        if ((keycon(KEY_AIM, KEY_OFF) && stickcon(PAD_AIM) < 0.6)) {
            _fov += 20.0 / (_weaponcontrols.a_EndFrame - _weaponcontrols.a_StartFrame);
            // Change fov in here
            (*_delegator_arr[0])(_fov);
            if (_fov > 80) {
                _fov = 80.0f;
            }
            _vbo1 --;
            if (_vbo1 < _weaponcontrols.a_StartFrame) {
                _vbo1 = _weaponcontrols.a_StartFrame;
                _actionaim = false;
            }
        }
        // Long pressed key, increase the aim frame
        if ((keycon(KEY_AIM, KEY_ON) || stickcon(PAD_AIM) > 0.6)) {
            _fov -= 20.0 / (_weaponcontrols.a_EndFrame - _weaponcontrols.a_StartFrame);
            // Change fov in here
            (*_delegator_arr[0])(_fov);
            if (_fov < 60.0f) {
                _fov = 60.0f;
            }
            _vbo1 ++;
            // Aim max frame is 10
            if (_vbo1 > _weaponcontrols.a_EndFrame) {
                _vbo1 = _weaponcontrols.a_EndFrame;
            }
        }
    }
    
}





void PlayerManager::_ProcessActionSnip()
{
    
    // Magazine ammo
    int magazine = _weapon_arr[_weaponarrnumber]->RecieveMagazineAmmo();
    
    // Avoid multiple click
    static bool stop_automatic = false;
    
    // Activate Aim firing
    if ((mousecon(KEY_FIRE, KEY_ON) || stickcon(PAD_FIRE) > 0.6) &&
        _actionaim       &&
        !_actionreload   &&
        !_actionfire     &&
        !_actionchange   &&
        !_actionrun      &&
        !_usingdrinkchange &&
        magazine > 0
        )
    {
        _ProcessActionReset();
        _actionrun = false;
        _actionsnip = true;
        _vbo1 = _weaponcontrols.f_ActiveFrame + _weaponcontrols.s_StartFrame;
    }
    
    if ( _actionsnip && g_CLOCK->RecieveFPS() ) {
        
        _vbo0 = 4;
        _vbo1 ++;
        
        // Avoid auto reload glitch!!!
        if (magazine < 1) {
            stop_automatic = false;
        }
        
        // Rapid fire at any per second.
        if (_vbo1 > _weaponcontrols.s_StartFrame + _weaponcontrols.f_Rate && magazine > 0) {
            if ((mousecon(KEY_FIRE, KEY_ON) || stickcon(PAD_FIRE) > 0.6)) {
                if(_weaponcontrols.single) {
                    if (!stop_automatic) {
                        _vbo1 = _weaponcontrols.s_StartFrame + 3;
                        stop_automatic = true;
                    }
                }else{
                    _vbo1 = _weaponcontrols.s_StartFrame + 3;
                }
            }
            if ((mousecon(KEY_FIRE, KEY_OFF) && stickcon(PAD_FIRE) < 0.6)) {
                if(_weaponcontrols.single) {
                    if (stop_automatic) {
                        stop_automatic = false;
                    }
                }
            }
        }
        
        // 15フレーム以上だと最終フレームを維持させる
        if (_vbo1 > _weaponcontrols.s_EndFrame) {
            _vbo1 = _weaponcontrols.s_EndFrame;
            // 打ち終わったらマウスを離してリセット
            if ((mousecon(KEY_FIRE, KEY_OFF) && stickcon(PAD_FIRE) < 0.6)) {
                _actionsnip = false;
                _actionaim = true;
                _vbo0 = 4;
                _vbo1 = _weaponcontrols.s_StartFrame;
            }
        }
        
        // 2フレーム目で射撃音を再生
        // 弾薬も使う
        if (_vbo1 == _weaponcontrols.s_StartFrame + _weaponcontrols.f_SoundFrame) {
            (*_delegator_arr[1])(std::string(_weaponsoundeffect.se_Fire));
            _weapon_arr[_weaponarrnumber]->UpdateIncreaseMagazineAmmo(-1);
        }
        
        // 2~3フレーム目だけ攻撃をオンにする
        if (_vbo1 == _weaponcontrols.s_StartFrame + _weaponcontrols.f_StartAttackFrame) _shoot = true;
        if (_vbo1 == _weaponcontrols.s_StartFrame + _weaponcontrols.f_EndAttackFrame)   _shoot = false;

        // Decorator
        if (_weaponcontrols.s_StartFrame + _weaponcontrols.f_EndMuzzleFlashFrame   > _vbo1 &&
            _weaponcontrols.s_StartFrame + _weaponcontrols.f_StartMuzzleFlashFrame < _vbo1) {
            
            // Flash position
            (*_delegator_arr[2])(_renderposition);
            _player_arr[_playerarrnumber]->UpdateMuzzleFlashPosition(_renderposition);
            _weapon_arr[_weaponarrnumber]->UpdateMuzzleFlashPosition(_renderposition);
            
            // Lock position after the first frame!!!
            // Flash
            _weapon_muzzleflash_arr[_weaponarrnumber]->ProcessMuzzleFlash();
            // Ballistic
            _weapon_ballistic_arr[_weaponarrnumber]->ProcessBallistic();
            
        }else{
            
            // Flash
            _weapon_muzzleflash_arr[_weaponarrnumber]->UpdateMuzzleFlashCounterToZero();
            // Ballistic
            _weapon_ballistic_arr[_weaponarrnumber]->UpdateBallisticCounterToZero();
            
        }
        
    }
    

}




void PlayerManager::_ProcessActionReload()
{
    
    // Magazine and extra ammo
    int magazine = _weapon_arr[_weaponarrnumber]->RecieveMagazineAmmo();
    int extra    = _weapon_arr[_weaponarrnumber]->RecieveExtraAmmo();
    
    // If magazine is 0 and have extra mag, Do auto reload.
    if (!_actionaim    &&
        !_actionsnip   &&
        !_actionreload &&
        !_actionfire   &&
        !_actionchange &&
        !_usingdrinkchange &&
        magazine < 1 &&
        magazine < _weaponstatus.magazinemaxammo &&
        extra > 0
        )
    {
//        on_reload();
    }
    
    // Activate reload
    if ((keycon(KEY_RELOAD, KEY_ON) || padcon(PAD_RELOAD, KEY_ON)) &&
        !_actionaim    &&
        !_actionsnip   &&
        !_actionreload &&
        !_actionfire   &&
        !_actionchange &&
        !_usingdrinkchange &&
        magazine < _weaponstatus.magazinemaxammo &&
        extra > 0
        )
    {
        _ProcessActionReset();
        _actionreload = true;
    }
    
    // df_r true and each "1/df_per" sec
    if ( _actionreload && g_CLOCK->RecieveFPS() ) {
        _vbo0 = 2;
        _vbo1 ++;
        
        // リロードの最終フレーム
        if (_vbo1 > _weaponcontrols.r_LastFrame) {
            _vbo1 = _weaponcontrols.r_LastFrame;
            if ((keycon(KEY_RELOAD, KEY_OFF) || padcon(PAD_RELOAD, KEY_OFF))) {
                _actionreload = false;
                _vbo0 = 0;
                _vbo1 = 0;
            }
        }
        
        // 25フレーム目でリロード音を再生
        if (_vbo1 == _weaponcontrols.r_SoundFrame) {
            (*_delegator_arr[1])(std::string(_weaponsoundeffect.se_Reload));
        }
        
        // 弾薬を装填
        if (_vbo1 == _weaponcontrols.r_LastFrame - 40) {
            int diff = _weaponstatus.magazinemaxammo - magazine;
            _weapon_arr[_weaponarrnumber]->UpdateMagazineAmmo(_weaponstatus.magazinemaxammo);
            _weapon_arr[_weaponarrnumber]->UpdateExtraAmmo(extra-diff);
            // If extra ammo less 0
            if (_weapon_arr[_weaponarrnumber]->RecieveExtraAmmo() < 0) {
                _weapon_arr[_weaponarrnumber]->UpdateIncreaseMagazineAmmo(extra);
                _weapon_arr[_weaponarrnumber]->UpdateExtraAmmo(0);
            }
        }
        
    }
    
}




void PlayerManager::_ProcessActionFire()
{
    
    // Magazine ammo
    int magazine = _weapon_arr[_weaponarrnumber]->RecieveMagazineAmmo();
    
    // Avoid multiple click
    static bool stop_automatic = false;
    
    // Activate fire
    if ((mousecon(KEY_FIRE, KEY_ON) || stickcon(PAD_FIRE) > 0.6) &&
        !_actionaim     &&
        !_actionsnip    &&
        !_actionreload  &&
        !_actionfire    &&
        !_actionchange  &&
        !_actionrun     &&
        !_usingdrinkchange &&
        magazine > 0
        )
    {
        _ProcessActionReset();
        _actionrun = false;
        _actionfire = true;
        // Start f_ActiveFrame, to avoid rapid fire glitch
        _vbo1 = _weaponcontrols.f_ActiveFrame;
    }
    
    // df_f true and each "1/df_per" sec
    if ( _actionfire && g_CLOCK->RecieveFPS() ) {
        
        _vbo0 = 1;
        _vbo1 ++;
        
        // Avoid to glitch after zero reload.
        if (magazine < 1) {
            stop_automatic = false;
        }
        
        // Rapid fire on any persec
        if (_vbo1 > _weaponcontrols.f_Rate && magazine > 0) {
            if ((mousecon(KEY_FIRE, KEY_ON) || stickcon(PAD_FIRE) > 0.6)) {
                if(_weaponcontrols.single) {
                    if (!stop_automatic) {
                        _vbo1 = 3;
                        stop_automatic = true;
                    }
                }else{
                    _vbo1 = 3;
                }
            }
            if ((mousecon(KEY_FIRE, KEY_OFF) && stickcon(PAD_FIRE) < 0.6)) {
                if(_weaponcontrols.single) {
                    if (stop_automatic) {
                        stop_automatic = false;
                    }
                }
            }
        }
        
        // 15フレーム以上だと最終フレームを維持させる
        if (_vbo1 > _weaponcontrols.f_LastFrame) {
            _vbo1 = _weaponcontrols.f_LastFrame;
            // 打ち終わったらマウスを離してリセット
            if ((mousecon(KEY_FIRE, KEY_OFF) && stickcon(PAD_FIRE) < 0.6)) {
                _actionfire = false;
                _vbo0 = 0;
                _vbo1 = 0;
            }
        }
        
        // 2フレーム目で射撃音を再生
        // 弾薬も使う
        if (_vbo1 == _weaponcontrols.f_SoundFrame) {
            (*_delegator_arr[1])(std::string(_weaponsoundeffect.se_Fire));
            _weapon_arr[_weaponarrnumber]->UpdateIncreaseMagazineAmmo(-1);
        }
        
        // 射撃後に薬莢の音を再生
        if (_vbo1 == _weaponcontrols.f_SoundFrame + 10) {
            std::string random = std::to_string((int)Randomizer(1, 4));
            std::string cartridge = "cartridge" + random;
            (*_delegator_arr[1])(cartridge);
        }
        
        // 2~3フレーム目だけ攻撃をオンにする
        if (_vbo1 == _weaponcontrols.f_StartAttackFrame) _shoot = true;
        if (_vbo1 == _weaponcontrols.f_EndAttackFrame) _shoot = false;

        // Decorator
        if ( _weaponcontrols.f_EndMuzzleFlashFrame > _vbo1 && _vbo1 > _weaponcontrols.f_StartMuzzleFlashFrame) {
            
            // Flash position
            (*_delegator_arr[2])(_renderposition);
            _player_arr[_playerarrnumber]->UpdateMuzzleFlashPosition(_renderposition);
            _weapon_arr[_weaponarrnumber]->UpdateMuzzleFlashPosition(_renderposition);
            
            // Lock position after the first frame!!!
            // Flash
            _weapon_muzzleflash_arr[_weaponarrnumber]->ProcessMuzzleFlash();
            // Ballistic
            _weapon_ballistic_arr[_weaponarrnumber]->ProcessBallistic();
            
        }else{
            
            // Flash
            _weapon_muzzleflash_arr[_weaponarrnumber]->UpdateMuzzleFlashCounterToZero();
            // Ballistic
            _weapon_ballistic_arr[_weaponarrnumber]->UpdateBallisticCounterToZero();
            
        }
        
    }

}




void PlayerManager::_ProcessActionChange()
{

    // Reverse true false
    static bool reverseframe = false;
    
    // Activate change
    if ((keycon(KEY_CHANGE, KEY_ON) || padcon(PAD_CHANGE, KEY_ON)) &&
        !_actionaim    &&
        !_actionsnip   &&
        !_actionreload &&
        !_actionfire   )
    {
        _ProcessActionReset();
        _actionchange = true;
    }
    
    // df_c true and each "1/df_per" sec
    if ( _actionchange && g_CLOCK->RecieveFPS() ) {
        _vbo0 = 5;
        if (!reverseframe) {
            _vbo1 ++;
        } else{
            _vbo1 --;
        }
        
        
        
        // :: FOR DRINK THE PERK ::
        // _actiondrink is true
        if (_vbo1 > _weaponcontrols.c_EndFrame && _usingdrinkchange && !_actiondrink) {
            
            // stop change animation
            _actionchange = false;
            
            // start drink animation
            _actiondrink = true;
            
            // Reset frame
            _vbo1 = 0;
            
            // Change to drink weapon number
            _weaponarrnumber = 3;
            
            _ProcessLinkingAllStatus();
            
            // Break void
            return;
            
        }
        
        

        // :: FOR WEAPON BOX ::
        // _usingrandomweapon is true
        if (_vbo1 > _weaponcontrols.c_EndFrame && _usingrandomweapon) {
            
            // Get new weapon
            if (_weaponasset[1] == -1) {
                // If first use mistery box, always return AKM
                // Get AKM (Set to _weaponasset[1])
                _weaponassetarrnumber = 1;
                _weaponasset[_weaponassetarrnumber] = 1;
            }
            
            //
            // !!! 武器が３種類以上になったら以下のコードを解禁 !!!
            //
//            else if(_weaponasset[1] != -1 && _weaponasset[2] == -1)
//            {
//                // Get new weaponarrnumber (Set to _weaponasset[2])
//                _weaponasset[2] == Randomizer(0, 1);
//            }
            
            else{
                
                // Get new weaponarrnumber
                int randomizer = Randomizer(1, 3) - 1;
                
                // For if statement
                int newweaponassetarrnumber = -1;
                
                // If having same weapon change that and just reload.
                for (int i=0; i<_weaponasset.size(); i++) {
                    if (_weaponasset[i] == randomizer) {
                        _weaponassetarrnumber = newweaponassetarrnumber = i;
                    }
                }
                
                // If new weapon
                if (newweaponassetarrnumber == -1){
                    _weaponasset[_weaponassetarrnumber] = randomizer;
                }
                
            }
            
            // Linking status
            _weaponarrnumber = _weaponasset[_weaponassetarrnumber];
            _ProcessLinkingAllStatus();
            
            // Max ammo
            _weapon_arr[_weaponarrnumber]->UpdateMagazineAmmo(_weaponstatus.magazinemaxammo);
            _weapon_arr[_weaponarrnumber]->UpdateExtraAmmo(_weaponstatus.extramaxammo);
            
            // Reverse frame
            reverseframe = true;
            
            // Break void
            return;
            
        }
        
        
        
        // Weapon change last frame
        if (_vbo1 > _weaponcontrols.c_EndFrame && !reverseframe) {
            
            // If Misterybox or Perk is true
            if (_usingdrinkchange || _usingrandomweapon) {
                
                // Turn to false (Not change weapon)
                _usingrandomweapon = false;
                _usingdrinkchange = false;
                
            }else{
                
                // Next weapon (Just change weapon)
                _weaponassetarrnumber ++;
                
            }
            
            // If _weaponassetarrnumber over 2
            if (_weaponasset[_weaponassetarrnumber] == -1 || _weaponassetarrnumber > 2) {
                // Reset
                _weaponassetarrnumber = 0;
            }
            
            // Set last frame
            _vbo1 = _weaponcontrols.c_EndFrame;
            
            // Reverse frame
            reverseframe = true;
            
            // Linking status
            _weaponarrnumber = _weaponasset[_weaponassetarrnumber];
            _ProcessLinkingAllStatus();
            
        }
        
        // End
        if (_vbo1 < _weaponcontrols.c_StartFrame && reverseframe) {
            _vbo1 = _weaponcontrols.c_StartFrame;
            if ((keycon(KEY_CHANGE, KEY_OFF) || padcon(PAD_CHANGE, KEY_OFF))) {
                reverseframe = false;
                _actionchange = false;
                _vbo0 = 0;
                _vbo1 = 0;
                _usingrandomweapon = false;
            }
        }
        
        // Play sound
        if (_vbo1 == _weaponcontrols.c_SoundFrame) {
            (*_delegator_arr[1])(std::string(_weaponsoundeffect.se_Change));
        }
        
    }
}





void PlayerManager::_ProcessActionDrink()
{
    
    // _actiondrink true
    if ( _actiondrink && g_CLOCK->RecieveFPS() ) {
        
        // Change to drink weapon number
        _weaponarrnumber = 3;
        
        // 1(Fire) is drink
        _vbo0 = 1;
        
        // Increase frame
        _vbo1 ++;
        
        // Reached Last frame (End of drink)
        if (_vbo1 > _weaponcontrols.f_LastFrame) {
            
            // Reset all action
            _ProcessActionReset();
            
            // Now weapon <- last used weapon
            _weaponassetarrnumber = _lastweaponassetarrnumber;
            
            _actiondrink = false;
            _actionchange = true;
            _actiondrink = false;
            _usingdrinkchange = false;
            
            // Action is change
            _vbo0 = 5;
            
            // Frame is last change frame
            _vbo1 = _weaponcontrols.c_EndFrame;
            
            // Break void
            return;
            
        }
        
        // 25フレーム目でリロード音を再生
        if (_vbo1 == _weaponcontrols.f_SoundFrame) {
            (*_delegator_arr[1])(std::string(_weaponsoundeffect.se_Fire));
        }
    }
    
}





void PlayerManager::_ProcessActionReset()
{
    _actionaim     =
    _actionsnip    =
    _actionreload  =
    _actionfire    =
    _actionchange  =
    _actionrun     =
    _actiondrink   =  false;
}





void PlayerManager::_ProcessLinkingAllStatus()
{
    _weapon_arr[_weaponarrnumber]->RecieveStructSoundEffect(_weaponsoundeffect);
    _weapon_arr[_weaponarrnumber]->RecieveStructControl(_weaponcontrols);
    _weapon_arr[_weaponarrnumber]->RecieveStructStatus(_weaponstatus);
}




std::string PlayerManager::RecieveWeaponName(std::string value){
    return std::string(_weaponstatus.name);
}
glm::vec3 PlayerManager::RecieveCurrentPosition(glm::vec3 position){
    return _currentposition;
}
glm::vec3 PlayerManager::RecieveLastPosition(glm::vec3 position){
    return _lastposition;
}
int PlayerManager::RecieveShoot(int value){
    return (int)_shoot;
}
bool PlayerManager::RecieveDead(bool value){
    return _dead;
}
int PlayerManager::RecievePower(int value){
    return _power;
}
int PlayerManager::RecieveLife(int value){
    return _life;
}
int PlayerManager::RecieveMaxLife(int value){
    return _maxlife;
}
int PlayerManager::RecieveMagazineAmmo(int value){
    return _weapon_arr[_weaponarrnumber]->RecieveMagazineAmmo();
}
int PlayerManager::RecieveExtraAmmo(int value){
    return _weapon_arr[_weaponarrnumber]->RecieveExtraAmmo();
}
int PlayerManager::RecieveScore(int value){
    return _score;
}
int PlayerManager::UpdateScore(int value){
    return _score += value;
}
int PlayerManager::UpdateLife(int value){
    return _life += value;
}
int PlayerManager::UpdatePlay(int value){
    return _play = value;
}
int PlayerManager::UpdateRender(int value){
    return _render = value;
}
void PlayerManager::ProcessActionDrink(){
    if (!_usingdrinkchange) {
        _ProcessActionReset();
        _usingdrinkchange = true;
        _actionchange = true;
        _actiondrink = false;
        _lastweaponassetarrnumber = _weaponassetarrnumber;
    }
}
void PlayerManager::ProcessActionMisteryBox(){
    if(!_usingrandomweapon && !_actionreload && !_actionfire) {
        _ProcessActionReset();
        _actionchange = true;
        _usingrandomweapon = true;
    }
}
void PlayerManager::UpdateDelegator(Delegator* delegator){
    /**
     * [ 0 ] ControlManager::UpdateFOV(float);
     * [ 1 ] AudioManager::PlaySound(std::string);
     * [ 2 ] MapManager::UpdateMuzzleFlashPosition(glm::vec3);
     * [ 3 ] ControlManager::RecievePosition(glm::vec3);
     * [ 4 ] ControlManager::RecieveLastPosition(glm::vec3);
     * [ 5 ] ControlManager::RecieveOrientation(glm::vec3);
     * [ 6 ] ControlManager::RecieveValancePosition(glm::vec3);
     * [ 7 ] ControlManager::RecieveCondition(glm::vec3);
     */
    _delegator_arr.push_back(delegator);
}
