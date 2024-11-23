//
//  collisionmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "collisionmanager.hpp"
#include "del/delegate.hpp"
#include "util/aabbCollision.hpp"
#include "util/obbCollision.h"
#include "util/ray_obbCollision.hpp"
#include "util/kabezuriCollision.hpp"
#include "util/terrainCollision.h"





CollisionManager::CollisionManager()
{
    
    _playercurrentterrain   = glm::vec3(0);
    _playerlastterrain      = glm::vec3(0);
    _playercurrentposition  = glm::vec3(0);
    _playerlastposition     = glm::vec3(0);
    _windowcursorposition   = glm::vec2(0);

}

CollisionManager::~CollisionManager()
{
    for (auto e : _delegate_arr) delete e;
    for (auto e : _aabb_arr    ) { for (auto g : e) delete g; }
    for (auto e : _obb_arr     ) { for (auto g : e) delete g; }
    for (auto e : _rayobb_arr  ) { for (auto g : e) delete g; }
    delete _kabezuricollision;
    delete _terraincollision;
    delete _aabbcollision;
    delete _rayobbcollision;
}


/* After updated delegate */
void CollisionManager::Init(const std::string &map)
{
    
    // Kabezuri collision file (.obj)
    std::string kabezuri_path = "game/" + map + "/oj/map/obj/map_collision.obj";
    const char *kabezuri_file = kabezuri_path.c_str();
    
    // Read and initialize kabezuri collision
    _kabezuricollision = new KabezuriCollision(kabezuri_file);
    
    // Terrain collision file (.obj)
    std::string terrain_path = "game/" + map + "/oj/map/obj/map_terrain.obj";
    const char *terrain_file = terrain_path.c_str();
    
    // Read and initialize terrain collision
    _terraincollision = new TerrainCollision(terrain_file);
    
    // Initialize AABBCollision
    _aabbcollision = new AABBCollision();
    
    // Initiaize RayOBBCollision
    _rayobbcollision = new RayOBBCollision();
    
}


/* In Rendering */
void CollisionManager::Render()
{
    
    // Syncronize player current/last position
    /* PlayerManager::RecieveCurrentPosition(glm::vec3) */
    _playercurrentposition = (*_delegate_arr[0])(glm::vec3(0));
    /* PlayerManager::RecieveLastPosition(glm::vec3) */
    _playerlastposition = (*_delegate_arr[1])(glm::vec3(0));
    /* ControlManager::UpdatePosition(glm::vec3) */
    _windowcursorposition = (*_delegate_arr[3])(glm::vec2(0));
    
    // Processing terrain collision
    _ProcessTerrainCollision();
    
    // Processing Kabezuri collision
    _ProcessKabezuriCollision();
    
    // Processing AABB (Player ~ Perk)
    _AABB_Player_Perk();
    
    // Processing AABB (Player ~ MisteryBox)
    _AABB_Player_MisteryBox();
    
    // Processing AABB (Player ~ Wood)
    _AABB_Player_RunningSound();
    
    // Processing AABB (Player ~ Escape)
    _AABB_Player_Escape();
    
    // Processing RayOBB (Player ~ Zombie)
    _RayOBB_Player_Zombie();
    
    // Processing RayOBB (Player ~ Stuff)
    _RayOBB_Player_Stuff();
    
    /* ControlManager::UpdatePosition(glm::vec3) */
    (*_delegate_arr[2])(_playercurrentposition);
    
}




int CollisionManager::ProcessCreateAABBGroup(int none)
{
    
    // Add dim
    _aabb_arr.push_back({});
    
    // Return dim size
    return (int)_aabb_arr.size()-1;
    
}




int CollisionManager::ProcessCreateOBBGroup(int none)
{
    
    // Add dim
    _obb_arr.push_back({});
    
    // Return dim size
    return (int)_obb_arr.size()-1;
    
}




int CollisionManager::ProcessCreateRayOBBGroup(int none)
{
    
    // Add dim
    _rayobb_arr.push_back({});
    
    // Return dim size
    return (int)_rayobb_arr.size()-1;
    
}




AABB* CollisionManager::ProcessSubmitAABB(AABB* aabb)
{
    
    // Which group insert?
    int insertgroup = aabb->group;
    
    // Add aabb
    _aabb_arr[insertgroup].push_back(aabb);
    
    // Update data
    aabb->index=(int)_aabb_arr[insertgroup].size()-1;
    
    // Return
    return aabb;
    
}





OBB* CollisionManager::ProcessSubmitOBB(OBB* obb)
{
    
    // Which group insert?
    int insertgroup = obb->group;
    
    // Add obb
    _obb_arr[insertgroup].push_back(obb);
    
    // Update data
    obb->index=(int)_obb_arr[insertgroup].size()-1;
    
    // Return
    return obb;
    
}




RayOBB* CollisionManager::ProcessSubmitRayOBB(RayOBB* rayobb)
{
    
    // Which group insert?
    int insertgroup = rayobb->group;
    
    // Add rayobb
    _rayobb_arr[insertgroup].push_back(rayobb);
    
    // Update data
    rayobb->index=(int)_rayobb_arr[insertgroup].size()-1;
    
    // Return
    return rayobb;
    
}




void CollisionManager::_ProcessTerrainCollision()
{
    
    // ControlManager::UpdateLastTerrain(float);
    _playerlastterrain = _playercurrentterrain;
    (*_delegate_arr[7])(_playerlastterrain.y);
    
    // Rewrite _playercurrentposition
    _playercurrentterrain.x = _playercurrentposition.x;
    _playercurrentterrain.z = _playercurrentposition.z;
    _terraincollision -> getTerrainCollision(_playercurrentterrain);
    
    // ControlManager::UpdateCurrentTerrain(float);
    (*_delegate_arr[6])(_playercurrentterrain.y);
    
}




void CollisionManager::_ProcessKabezuriCollision()
{
    
    // Radius
    float playerradius = 0.3;
    // Rewrite _playercurrentposition
    _kabezuricollision -> getCollision(_playercurrentposition, _playerlastposition, playerradius);
    
}




void CollisionManager::_AABB_Player_Perk()
{
    
    // AABB collision [0 group]
    for (int i=1; i<_aabb_arr[0].size(); i++) {
        // Null pointer
        if (_aabb_arr[0][i] == nullptr) {
            continue;
        }
        // Check collision ( 0 : i )
        _aabb_arr[0][i]->hit = _aabbcollision->AABBtoAABB(_aabb_arr[0][0], _aabb_arr[0][i]);
    }
    
}




void CollisionManager::_AABB_Player_MisteryBox()
{
    
    // AABB collision [1 group]
    for (int i=1; i<_aabb_arr[1].size(); i++) {
        // Null pointer
        if (_aabb_arr[1][i] == nullptr) {
            continue;
        }
        // Check collision ( 0 : i )
        _aabb_arr[1][i]->hit = _aabbcollision->AABBtoAABB(_aabb_arr[1][0], _aabb_arr[1][i]);
    }
    
}




void CollisionManager::_AABB_Player_RunningSound()
{
    
    // AABB collision [2 group]
    for (int i=1; i<_aabb_arr[2].size(); i++) {
        // Null pointer
        if (_aabb_arr[2][i] == nullptr) {
            continue;
        }
        // Check collision ( 0 : i )
        _aabb_arr[2][i]->hit = _aabbcollision->AABBtoAABB(_aabb_arr[2][0], _aabb_arr[2][i]);
    }
    
}




void CollisionManager::_AABB_Player_Escape()
{
    
    // AABB collision [3 group]
    for (int i=1; i<_aabb_arr[3].size(); i++) {
        // Null pointer
        if (_aabb_arr[3][i] == nullptr) {
            continue;
        }
        // Check collision ( 0 : i )
        _aabb_arr[3][i]->hit = _aabbcollision->AABBtoAABB(_aabb_arr[3][0], _aabb_arr[3][i]);
    }
    
}




void CollisionManager::_RayOBB_Player_Zombie()
{
    
    // Convert to double
    double cursorx = (double)_windowcursorposition.x;
    double cursory = (double)_windowcursorposition.y;
    
    // Ready to RAY
    _rayobbcollision -> updateRay(cursorx, cursory, g_SCREENWIDTH, g_SCREENHEIGHT, g_VIEWMATRIX, g_PROJECTIONMATRIX);
    
    // Check RayOBB
    for (int i=0; i<_rayobb_arr[1].size(); i++) {
        // Update hit ( xpt (ray distance) is automatically update in method )
        _rayobb_arr[1][i]->hit = _rayobbcollision -> ray_obbCollision(_rayobb_arr[1][i]);
    }
    
}




void CollisionManager::_RayOBB_Player_Stuff()
{
    
    // Convert to double
    double cursorx = (double)_windowcursorposition.x;
    double cursory = (double)_windowcursorposition.y;
    
    // Ready to RAY
    _rayobbcollision -> updateRay(cursorx, cursory, g_SCREENWIDTH, g_SCREENHEIGHT, g_VIEWMATRIX, g_PROJECTIONMATRIX);
    
    // Check RayOBB
    for (int i=0; i<_rayobb_arr[0].size(); i++) {
        // Update hit ( xpt (ray distance) is automatically update in method )
        _rayobb_arr[0][i]->hit = _rayobbcollision -> ray_obbCollision(_rayobb_arr[0][i]);
    }
    
}





glm::vec3 CollisionManager::UpdateTargetCurrentPosition(glm::vec3 position){
    return _targetcurrentposition = position;
}
glm::vec3 CollisionManager::UpdateTargetLastPosition(glm::vec3 position){
    return _targetlastposition = position;
}
float CollisionManager::UpdateTargetRadius(float value){
    return _targetradius = value;
}
glm::vec3 CollisionManager::RecieveTerrainCollision(glm::vec3 position){
    glm::vec3 cposition = _targetcurrentposition;
    _terraincollision -> getTerrainCollision(cposition);
    return cposition;
}
glm::vec3 CollisionManager::RecieveKabezuriCollision(glm::vec3 position){
    glm::vec3 cposition = _targetcurrentposition;
    _kabezuricollision -> getCollision(cposition, _targetlastposition, _targetradius);
    return cposition;
}
void CollisionManager::UpdateDelegator(Delegator* delegator)
{
    
    /**
     * [ 0 ] PlayerManager::RecieveCurrentPosition(glm::vec3);
     * [ 1 ] PlayerManager::RecieveLastPosition(glm::vec3);
     * [ 2 ] ControlManager::UpdatePosition(glm::vec3);
     * [ 3 ] ControlManager::RecieveCursorPosition(glm::vec2);
     * [ 4 ] ControlManager::RecieveConditionJump(int);
     * [ 5 ] ControlManager::RecieveConditionFall(int);
     * [ 6 ] ControlManager::UpdateCurrentTerrain(float);
     * [ 7 ] ControlManager::UpdateLastTerrain(float);
     */
    
    _delegate_arr.push_back(delegator);

}


