//
//  condition.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef condition_hpp
#define condition_hpp

#include "control.hpp"

class Condition : protected Control
{
    
protected:
    
    float   _current_terrain;
    float   _last_terrain;
    bool    _jump;
    bool    _fall;
    bool    _land;
    bool    _walk;
    bool    _run;
    bool    _pose;
    
private:
    
    // common
    float   _actual_current_terrain;
    float   _actual_last_terrain;
    float   _actual_current_height;
    
    // jump
    float   _jump_time;
    float   _jump_time_rate;
    float   _jump_maximum;
    
    // fall
    float   _fall_time;
    float   _fall_time_rate;
    float   _gravity;
    
    // land
    float   _land_time;
    float   _land_time_rate;
    float   _land_maximum;
    
    // run (speed)
    float       _flict;
    glm::vec2   _unitvector;
    
    // run (shake)
    float   _run_motion_rate;
    
    float ConditionJump();
    float ConditionFall();
    float ConditionLand();

public:
    
    Condition();
    ~Condition();
    void Reset();
    void ProcessCondition();
    void ProcessConditionRunSpeed();
    void ProcessConditionRunShake();
    void ProcessConditionWalkRunPose();
    void ProcessConditionGunImpact(const bool &fire);
    
};

#endif /* condition_hpp */
