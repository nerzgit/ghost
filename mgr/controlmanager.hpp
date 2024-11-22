//
//  controlmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef controlmanager_hpp
#define controlmanager_hpp

#include "ctrl/condition.hpp"
#include <vector>

class  Delegator;
class  Clock;
extern Clock*      g_CLOCK;

class ControlManager : public Condition
{
private:
    bool _play;
    std::vector<Delegator*> _delegate_arr;
    
public:
    ControlManager();
    ~ControlManager();
    void Render();
    void Reset();
    float UpdateFOV(float fov);
    glm::vec3 UpdateDirection(glm::vec3 direction);
    glm::vec3 UpdatePosition(glm::vec3 position);
    glm::vec2 RecieveCursorPosition(glm::vec2 position);
    glm::vec3 RecieveLastPosition(glm::vec3 position);
    glm::vec3 RecievePosition(glm::vec3 position);
    glm::vec3 RecieveCondition(glm::vec3 condition);
    glm::vec3 RecieveOrientation(glm::vec3 orientation);
    glm::vec3 RecieveValancePosition(glm::vec3 position);
    float UpdateCurrentTerrain(float terrain);
    float UpdateLastTerrain(float terrain);
    int UpdatePlay(int value);
    int UpdateInvertXAxis(int value);
    int UpdateInvertYAxis(int value);
    int RecieveConditionJump(int value);
    int RecieveConditionFall(int value);
    int RecieveConditionLand(int value);
    int RecieveConditionWalk(int value);
    int RecieveConditionRun(int value);
    int RecieveConditionPose(int value);
    float RecieveCurrentTerrain(float terrain);
    void UpdateDelegator(Delegator* delegator);
};

#endif /* controlmanager_hpp */
