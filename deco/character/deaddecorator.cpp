//
//  deaddecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "deaddecorator.hpp"
#include "del/delegate.hpp"





DeadDecorator::DeadDecorator()
{
    _style = 0;
    _state = 0;
}

DeadDecorator::~DeadDecorator()
{
    
}





/* In Rendering */
void DeadDecorator::ProcessDead()
{
    int v0 = CharacterDecorator::_o_character->RecieveVBO0();
    int v1 = CharacterDecorator::_o_character->RecieveVBO1();
    
    if (v0 != 2 || !CharacterDecorator::_o_character->RecievePlay() ) {
        return;
    }
    
    if (v1 < 2) {
        _style = _state = 0;
    }
    
    if (v1 > 60) {
        CharacterDecorator::_o_character->UpdateVBO1(60);
    }
    
    
    

    // Animate timing
    const std::vector<std::vector<int>> movement_per_Arr = {
        {// Animation 0 - enter - place 0
            0,40,60
        },
    };
    
    // Amount of Y axis and XZ animate
    const std::vector<std::vector<float>> movement_y_Arr = {
        {// Animation 0 - enter - place 0
            0,0,-0.8
        }
    };
    
    if (movement_per_Arr[_style][_state+1] < v1) _state++;
    if (_state > movement_per_Arr[_style].size()-2) {
        _state = (int)movement_per_Arr[_style].size()-2;
    }
    
    int ekc = movement_per_Arr[_style][_state];     // Start
    int ekp = movement_per_Arr[_style][_state+1];   // End
    
    // Bezier curve
    glm::vec2 xy1, xy2, xy3, xy4;
    xy1 = glm::vec2( 0.0, 0.0 );
    xy2 = glm::vec2( 0.5, 0.0 );
    xy3 = glm::vec2( 0.5, 1.0 );
    xy4 = glm::vec2( 1.0, 1.0 );
    float f_t = float(v1 - float(ekc))/(float(ekp) - float(ekc));
    float f_y = (1-f_t)*(1-f_t)*(1-f_t)*xy1.y + 3*((1-f_t)*(1-f_t))*f_t*xy2.y + 3*((1-f_t))*(f_t*f_t)*xy3.y + (f_t*f_t*f_t)*xy4.y;
    
    float Bcoord = movement_y_Arr[_style][_state];
    float Acoord = movement_y_Arr[_style][_state+1];
    float diffcoord = (float((v1 - float(ekc))*f_y/(float(ekp) - float(ekc))) * (Acoord - Bcoord) + Bcoord);
    
    glm::vec3 position = CharacterDecorator::_o_character->RecievePosition();
    
    /* CollisionManager::UpdateTargetCurrentPosition(glm::vec3); */
    (*CharacterDecorator::_delegate_arr[0])(position);
    /* CollisionManager::RecieveTerrainCollision(glm::vec3); */
    glm::vec3 terrainpos = (*CharacterDecorator::_delegate_arr[3])(glm::vec3(0));
    
    position.y = diffcoord + terrainpos.y;
    
    CharacterDecorator::_o_character->UpdatePosition(position);
    
}
