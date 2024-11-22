//
//  o_stuff.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/03.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef o_stuff_hpp
#define o_stuff_hpp

#include <string>
#include "abs/a_stuff.hpp"

class O_Stuff : public A_Stuff
{
private:
    std::string _stuffname;
    int _rayobbnumber;
    glm::vec3 _targetposition;
    glm::vec3 _targetorientation;
    float _targetdistance;
    bool _targetfire;
    bool _hit;
    
public:
    O_Stuff(
            const char *tex_path,
            const char *obj_path,
            const char *obb_path,
            const char *vsh_path,
            const char *fsh_path
    );
    O_Stuff();
    ~O_Stuff();
    O_Stuff* Clone();
    std::string RecieveStuffName();
    void UpdateStuffName(const std::string &stuffname);
    int RecieveRayOBBNumber();
    int UpdateRayOBBNumber(const int &rayobbnumber);
    void UpdateTargetPosition(const glm::vec3 &position);
    void UpdateTargetOrientation(const glm::vec3 &orientation);
    void UpdateTargetDistance(const float &distance);
    void UpdateHit(const bool &value);
    void UpdateTargetFire(const bool &value);
    glm::vec3 RecieveTargetPosition();
    glm::vec3 RecieveTargetOrientation();
    float RecieveTargetDistance();
    bool RecieveHit();
    bool RecieveTargetFire();
};

#endif /* o_stuff_hpp */
