//
//  collisionmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef collisionmanager_hpp
#define collisionmanager_hpp

#include <vector>

#include <glm/glm.hpp>
#include <string>

struct AABB;
struct OBB;
struct RayOBB;
class Delegator;
class KabezuriCollision;
class TerrainCollision;
class AABBCollision;
class RayOBBCollision;
extern glm::mat4 g_PROJECTIONMATRIX;
extern glm::mat4 g_VIEWMATRIX;
extern int g_SCREENWIDTH;
extern int g_SCREENHEIGHT;

class CollisionManager
{
private:
    std::vector<Delegator*> _delegate_arr;
    
    /**
     *
     * AABB
     * Group 0 : EventManager >> Player = PerkCola( From [1] )
     * Group 1 : EventManager >> Player = MisteryBox( From [1] )
     * Group 2 : EventManager >> Player = Running( From [1] )
     * Group 3 : EventManager >> Player = Escape( From [1] )
     *
     * OBB
     * Group 0 : CharacterManager >> Player = Zombie( From [0] )
     * Group 1 : StuffManager >> Player = Stuff( From [0] )
     *
     * RayOBB
     *
     *
     */
    
    std::vector<std::vector<AABB*>> _aabb_arr;       // 1 dim is group. 2 dim is group's AABB.
    std::vector<std::vector<OBB*>> _obb_arr;         // 1 dim is group. 2 dim is group's OBB.
    std::vector<std::vector<RayOBB*>> _rayobb_arr;   // 1 dim is group. 2 dim is group's RAYOBB.
    
    glm::vec3 _playercurrentterrain;
    glm::vec3 _playerlastterrain;
    glm::vec3 _playercurrentposition;
    glm::vec3 _playerlastposition;
    glm::vec2 _windowcursorposition;
    
    glm::vec3 _targetcurrentposition;
    glm::vec3 _targetlastposition;
    float     _targetradius;
    
    KabezuriCollision* _kabezuricollision;
    TerrainCollision* _terraincollision;
    AABBCollision* _aabbcollision;
    RayOBBCollision* _rayobbcollision;
    
    
    void _ProcessTerrainCollision();
    void _ProcessKabezuriCollision();
    void _AABB_Player_Perk();
    void _AABB_Player_MisteryBox();
    void _AABB_Player_RunningSound();
    void _AABB_Player_Escape();
    void _RayOBB_Player_Zombie();
    void _RayOBB_Player_Stuff();
    
public:
    CollisionManager();
    ~CollisionManager();
    void Init(const std::string &map);
    void Render();
    int ProcessCreateAABBGroup(int none=0);
    int ProcessCreateOBBGroup(int none=0);
    int ProcessCreateRayOBBGroup(int none=0);
    AABB* ProcessSubmitAABB(AABB* aabb);
    OBB* ProcessSubmitOBB(OBB* obb);
    RayOBB* ProcessSubmitRayOBB(RayOBB* rayobb);
    glm::vec3 UpdateTargetCurrentPosition(glm::vec3 position);
    glm::vec3 UpdateTargetLastPosition(glm::vec3 position);
    float UpdateTargetRadius(float value);
    glm::vec3 RecieveTerrainCollision(glm::vec3 position);
    glm::vec3 RecieveKabezuriCollision(glm::vec3 position);
    void UpdateDelegator(Delegator* delegator);
    
};

#endif /* collisionmanager_hpp */
