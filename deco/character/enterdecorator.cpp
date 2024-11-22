//
//  enterdecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "enterdecorator.hpp"
#include "util/randomizer.hpp"





EnterDecorator::EnterDecorator()
{
    _style = 0;
    _state = 0;
}

EnterDecorator::~EnterDecorator(){}




/* In Rendering */
void EnterDecorator::ProcessEnter()
{
    
    int v0 = CharacterDecorator::_o_character->RecieveVBO0();
    int v1 = CharacterDecorator::_o_character->RecieveVBO1();
    
    if (v0 != 0 || !CharacterDecorator::_o_character->RecievePlay() ) {
        return;
    }
    
    if (v1 < 2) {
        _style = CharacterDecorator::_enterstyle;
        _state = 0;
    }
    
    if (v1 > 120) {
        CharacterDecorator::_o_character->UpdateVBO0(1);
        CharacterDecorator::_o_character->UpdateVBO1(0);
    }
    
    
    
    // All zombies animation's (movement0 to movementX) amount of animate is
    // Using below arrays. movement_per_Arr is shown separated animate timing.
    // movement_y_Arr is amount of animate Y axis.
    // So X and Z axis amount is expressed by calculate.
    
    // Animate timing
    const std::vector<std::vector<int>> movement_per_Arr = {
        {// Animation 0 - enter - place 0
            0,20,45,70,110,120
        },
        {// Animation 0 - enter - place 1
            0,20,45,70,110,120
        },
    };
    
    // Amount of Y axis and XZ animate
    const std::vector<std::vector<std::vector<float>>> movement_y_Arr = {
        {// Animation 0 - enter - place 0
            {18,0,17},{18,6.4,17},{18,6.4,17},
            {17,6.4,17},{16,0,17},{16,0,17}
        },
        {// Animation 0 - enter - place 1
            {8.75,0,0.5},{8.75,6.4,0.5},{8.75,6.4,0.5},
            {8.75,6.4,0.84},{8.75,0,2.285},{8.75,0,2.285}
        },
    };
    
    // Avoid glitch
    if (_style > movement_per_Arr.size()-1) {
        _style = 0;
    }
    
    // No need to reset to zero. just increasing
    if (movement_per_Arr[_style][_state+1] < v1)
    {
        _state++;
        if (_state > movement_per_Arr[_style].size()-2) {
            _state = (int)movement_per_Arr[_style].size()-2;
        }
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
    
    glm::vec3 Bcoord = glm::vec3(
                                 movement_y_Arr[_style][_state][0],
                                 movement_y_Arr[_style][_state][1],
                                 movement_y_Arr[_style][_state][2]
                                 );
    glm::vec3 Acoord = glm::vec3(
                                 movement_y_Arr[_style][_state+1][0],
                                 movement_y_Arr[_style][_state+1][1],
                                 movement_y_Arr[_style][_state+1][2]
                                 );
    glm::vec3 newposition = (float((v1 - float(ekc))*f_y/(float(ekp) - float(ekc))) * (Acoord - Bcoord) + Bcoord);
    
    CharacterDecorator::_o_character->UpdateOrientation(glm::vec3(0,1.57f,0));
    CharacterDecorator::_o_character->UpdatePosition(newposition);
    
}



