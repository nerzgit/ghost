//
//  mapmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "mapmanager.hpp"
#include "del/delegate.hpp"
#include "obj/o_stuff.hpp"
#include "bld/mapbuilder.hpp"
#include "deco/mapdecorator.hpp"
#include "deco/map/dustdecorator.hpp"
#include "util/randomizer.hpp"
#include "util/ray_obbCollision.hpp"
#include "usge/texture.hpp"




MapManager::MapManager()
{
    
    _playerfire          = false;
    _randomizer          =
    _playerposition      =
    _playerorientation   =
    _muzzleflashposition = glm::vec3(0);
    _lightposition       = glm::vec3(12.0, 12.0, -8.0);

}

MapManager::~MapManager()
{
    for (auto e : _delegate_arr) delete e;
    for (auto e : _rayobb_arr  ) delete e;
    for (auto e : _o_stuff_arr ) delete e;
    for (auto e : _builder_arr ) delete e;
    for (auto e : _dust_arr    ) delete e;
}




/* After updated delegator */
void MapManager::Init(const std::string &map)
{
    
    // Create map
    _InitializeMap(map);
    
    // Create decorate
    _InitializeDecorator();
    
    // Initialize RayOBB
    _InitializeRayOBB();
    
}




/* Reinitialize */
void MapManager::Reset()
{
    
    // Reset all bullet hole
    for (auto e : _o_stuff_arr) e->A_Stuff::UpdateAllBulletHoleMVP(glm::mat4(0));
    
}




/* In Rendering */
void MapManager::Render()
{

    // Syncronize player data
    /* Controlmanager::RecievePosition(glm::vec3) + RecieveCurrentTerrain(float) */
    _playerposition = (*_delegate_arr[2])(glm::vec3(0)) + glm::vec3(0, (*_delegate_arr[5])(0.0f) ,0);
    /* Controlmanager::RecieveOrientation(glm::vec3) */
    _playerorientation = (*_delegate_arr[3])(glm::vec3(0));
    /* PlayerManager::RecieveShoot(int) */
    _playerfire = (bool)(*_delegate_arr[4])(0);
    
    /**
     * _randomizer need for "BulletHole" and "BulletDust"
     * "BulletHole" is in abs/a_stuff and "BulletDust" is
     * in deco/map/dustdecorator. All accesories are lowe
     * r class and also composition.
     */
    glm::vec3 randomvec = Randomizer(1, 1000, 3000);
    _randomizer = glm::vec3(randomvec.x, randomvec.y, 0);
    
    // RayOBB
    _ProcessRayOBB();

    // Update and Render
    for (int i=0; i<_o_stuff_arr.size(); i++) {
        // Muzzle flash
        _o_stuff_arr[i]->A_Stuff::UpdateFlashPosition(_muzzleflashposition);
        // Anbient light
        _o_stuff_arr[i]->A_Stuff::UpdateLightPosition(_lightposition);
        // Target(Player) position
        _o_stuff_arr[i]->UpdateTargetPosition(_playerposition);
        // Target(Player) angle
        _o_stuff_arr[i]->UpdateTargetOrientation(_playerorientation);
        // Render
        _o_stuff_arr[i]->A_Stuff::Render();
    }
    
    // Dust (Decorator)
    for (auto x : _dust_arr) x->ProcessDust();
    
}





void MapManager::_InitializeMap(const std::string &map)
{
    
    MapBuilder* mapbuilder = new MapBuilder(map);
    _builder_arr.push_back(mapbuilder);
    
    // Read foz file for Create map
    FILE * file = fopen(std::string("game/"+map+"/oj/map/obj/map.foz").c_str(), "r");
    if( file == NULL ){
        printf(".foz file not found\n");
        return;
    }
    
    while( 1 ){
        
        char head[128];
        
        // End of file
        int r = fscanf(file, "%s", head);
        if (r == EOF)
            break;
        
        // Create stuff
        // Bring it back from MapBuilder because builder has Clone
        if ( strcmp( head, "o" ) == 0 ){
            float s0,s1,s2; // Rotate
            float s3,s4,s5; // Scale
            float s6,s7,s8; // Position
            char  c0[256];  // texture name
            int   i0,i1;    // Object category number (Using), Shader number (Not using)
            
            fscanf(file, "%f %f %f %f %f %f %f %f %f %i %s %i", &s0,&s1,&s2,&s3,&s4,&s5,&s6,&s7,&s8,&i0,c0,&i1);
            
            // Create as clone
            O_Stuff* copied = mapbuilder->CopyStuff(i0);
            copied -> UpdateAngle(glm::vec3(s0-(90*3.14f)/180,s2,s1));
            copied -> UpdateScale(glm::vec3(s3,s4,s5));
            copied -> UpdatePosition(glm::vec3(s6,s7,s8));
            GLuint texture = loadDDS(std::string("game/"+map+"/oj/map/tex/"+std::string(c0)+".dds").c_str());
            copied -> UpdateTexture(texture);
            _o_stuff_arr.push_back(copied);
            
        }
        
    }
    fclose(file);
    
}





void MapManager::_InitializeDecorator()
{

    // Initialize decorator
    MapDecorator* mapdecorator = new DustDecorator();
    
    // Push back decorator array
    _dust_arr.push_back(mapdecorator);
    
}





void MapManager::_InitializeRayOBB()
{
    
    // Create RayOBB collision group and get index
    int createdgroup = (*_delegate_arr[0])(0);
    
    // Initialize and set to CollisionManager
    for (auto e : _o_stuff_arr) {
        
        // Get obb vertices
        std::vector<glm::vec3> _obbvertices;
        e->RecieveOBBVertices(_obbvertices);
        
        // Initialize RayOBB
        RayOBB * rayobb = new RayOBB(_obbvertices[0], _obbvertices[1], glm::mat4(1.0f), createdgroup);
        
        // Set to CollisionManager
        (*_delegate_arr[1])(rayobb);
        
        // Push back
        _rayobb_arr.push_back(rayobb);
        
    }
    
}





void MapManager::_ProcessRayOBB()
{
    
    for (int i=0; i<_rayobb_arr.size(); i++) {
        
        // Get modelmatrix
        glm::mat4 modelmatrix;
        _o_stuff_arr[i]->A_Stuff::RecieveModelMatrix(modelmatrix);
        
        // Update modelmatrix rayobb
        _rayobb_arr[i]->Update(modelmatrix);
        
        bool hit = _rayobb_arr[i]->hit;
        
        // HIT!!
        if (hit) {
            
            // Update Dust decorator stuff
            _dust_arr[0]->UpdateDecorator(_o_stuff_arr[i]);
            
            // Update distance
            _o_stuff_arr[i]->UpdateTargetDistance(_rayobb_arr[i]->xpt);
            
            // Update target(player) fire? or not
            _o_stuff_arr[i]->UpdateTargetFire(_playerfire);
            
            // Update hit
            _o_stuff_arr[i]->UpdateHit(hit);
            
            // FIRE!!
            if (_playerfire) {
                
                // Update randomizer
                _dust_arr[0]->UpdateDustRandomizer(_randomizer);
                _o_stuff_arr[i]->A_Stuff::UpdateRandomizer(_randomizer);
                
                // Render bullet hole (Not Decorator!!!)
                _o_stuff_arr[i]->A_Stuff::RenderBulletHole();
                
            }else{
                
                // _bh_action turn to true (Not Decorator!!!)
                _o_stuff_arr[i]->A_Stuff::UpdateBulletHoleAction(true);
                
            }
            
        }
        
    }
    
}





glm::vec3 MapManager::UpdateMuzzleFlashPosition(glm::vec3 position){
    return _muzzleflashposition = position;
}
glm::vec3 MapManager::UpdateLightPosition(glm::vec3 position){
    return _lightposition = position;
}
void MapManager::UpdateDelegator(Delegator* delegator){
    /**
     * [ 0 ] CollisionManager::ProcessCreateRayOBBGroup(int);
     * [ 1 ] CollisionManager::ProcessSubmitRayOBB(RayOBB);
     * [ 2 ] Controlmanager::RecievePosition(glm::vec3);
     * [ 3 ] Controlmanager::RecieveOrientation(glm::vec3);
     * [ 4 ] PlayerManager::RecieveShoot(int);
     * [ 5 ] ControlManager::RecieveCurrentTerrain(glm::vec3);
     */
    _delegate_arr.push_back(delegator);
}
