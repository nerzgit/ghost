//
//  characterdecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "characterdecorator.hpp"

void CharacterDecorator::ProcessEnter(){}
void CharacterDecorator::ProcessChase(){}
void CharacterDecorator::ProcessDead(){}
void CharacterDecorator::ProcessBlood(){}

void CharacterDecorator::UpdateEnterStyle(const int value){
    _enterstyle = value;
}
void CharacterDecorator::UpdateDecorator(O_Character *character){
    _o_character = character;
}
void CharacterDecorator::UpdateDelegator(Delegator* delegator){
    /**
     * [ 0 ] CollisionManager::UpdateTargetCurrentPosition(glm::vec3);
     * [ 1 ] CollisionManager::UpdateTargetLastPosition(glm::vec3);
     * [ 2 ] CollisionManager::UpdateTargetRadius(float);
     * [ 3 ] CollisionManager::RecieveTerrainCollision(glm::vec3);
     * [ 4 ] CollisionManager::RecieveKabezuriCollision(glm::vec3);
     */
    _delegate_arr.push_back(delegator);
}
