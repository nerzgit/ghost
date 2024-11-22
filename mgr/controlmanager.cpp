//
//  controlmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "controlmanager.hpp"
#include "del/delegate.hpp"
#include "usge/clock.hpp"

ControlManager::ControlManager() : Condition()
{
    _play = true;
}

ControlManager::~ControlManager()
{
    for (auto e : _delegate_arr) delete e;
}

void ControlManager::Render()
{
    if(g_CLOCK->RecieveFPS()){
        
        if (_play){
        
            Control::InputComputing();
            
            Condition::ProcessConditionRunSpeed();
            
            Condition::ProcessCondition();
            
            Condition::ProcessConditionRunShake();
            
            Condition::ProcessConditionWalkRunPose();
            
            Condition::ProcessConditionGunImpact((*_delegate_arr[0])(0));
            
            Control::ProcessComputing();
        
        }
        
        Control::OutputComputing();
        
    }
}

void ControlManager::Reset()
{
    _play = true;
    Control::Reset();
    Condition::Reset();
    
}

float ControlManager::UpdateFOV(float fov){
    return Control::_fov = fov;
}
glm::vec3 ControlManager::UpdatePosition(glm::vec3 position){
    return Control::_position = position;
}
glm::vec2 ControlManager::RecieveCursorPosition(glm::vec2 position){
    return glm::vec2((float)Control::_windowcursorpositionX, (float)Control::_windowcursorpositionY);
}
glm::vec3 ControlManager::RecieveLastPosition(glm::vec3 position){
    return Control::_lastposition;
}
glm::vec3 ControlManager::RecievePosition(glm::vec3 position){
    return Control::_position;
}
glm::vec3 ControlManager::RecieveCondition(glm::vec3 condition){
    return Control::_condition;
}
glm::vec3 ControlManager::RecieveOrientation(glm::vec3 orientation){
    return glm::vec3(Control::_verticalangle, Control::_horizontalangle, 0);
}
glm::vec3 ControlManager::RecieveValancePosition(glm::vec3 position){
    return Control::_valanceposition;
}
float ControlManager::UpdateCurrentTerrain(float terrain){
    return Condition::_current_terrain = terrain;
}
float ControlManager::UpdateLastTerrain(float terrain){
    return Condition::_last_terrain = terrain;
}
glm::vec3 ControlManager::UpdateDirection(glm::vec3 direction){
    return Control::_direction = direction;
}
int ControlManager::UpdatePlay(int value){
    return _play = value;
}
int ControlManager::UpdateInvertXAxis(int value){
    return Control::_invertxaxis = value;
}
int ControlManager::UpdateInvertYAxis(int value){
    return Control::_invertyaxis = value;
}
int ControlManager::RecieveConditionJump(int value){
    return (int)Condition::_jump;
}
int ControlManager::RecieveConditionFall(int value){
    return (int)Condition::_fall;
}
int ControlManager::RecieveConditionLand(int value){
    return (int)Condition::_land;
}
int ControlManager::RecieveConditionWalk(int value){
    return (int)Condition::_walk;
}
int ControlManager::RecieveConditionRun(int value){
    return (int)Condition::_run;
}
int ControlManager::RecieveConditionPose(int value){
    return (int)Condition::_pose;
}
float ControlManager::RecieveCurrentTerrain(float terrain){
    return Condition::_current_terrain;
}

void ControlManager::UpdateDelegator(Delegator* delegator)
{
    
    /**
     * [ 0 ] PlayerMangaer::RecieveShoot(int)
     */
    
    _delegate_arr.push_back(delegator);

}
