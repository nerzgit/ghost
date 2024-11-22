//
//  o_stuff.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/03.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "o_stuff.hpp"

O_Stuff::O_Stuff(const char *tex_path,
                 const char *obj_path,
                 const char *obb_path,
                 const char *vsh_path,
                 const char *fsh_path
                 ):A_Stuff(  tex_path,
                             obj_path,
                             obb_path,
                             vsh_path,
                             fsh_path)
{
    _rayobbnumber      = 0;
    _targetorientation = glm::vec3(0);
    _targetposition    = glm::vec3(0);
    _targetdistance    = 0;
    _targetfire        = false;
    _hit               = false;
}

O_Stuff::O_Stuff():A_Stuff()
{}

O_Stuff::~O_Stuff()
{}

O_Stuff* O_Stuff::Clone()
{
    O_Stuff* clone = new O_Stuff();
    A_Stuff::Clone(clone);
    clone->_rayobbnumber      = this->_rayobbnumber;
    clone->_targetorientation = this->_targetorientation;
    clone->_targetposition    = this->_targetposition;
    clone->_targetdistance    = this->_targetdistance;
    clone->_targetfire        = this->_targetfire;
    clone->_hit               = this->_hit;
    return clone;
}

std::string O_Stuff::RecieveStuffName(){
    return _stuffname;
}
void O_Stuff::UpdateStuffName(const std::string &stuffname){
    _stuffname = stuffname;
}
int O_Stuff::RecieveRayOBBNumber(){
    if (_rayobbnumber != -1)
        return _rayobbnumber;
    return -1;
}
int O_Stuff::UpdateRayOBBNumber(const int &rayobbnumber){
    return _rayobbnumber = rayobbnumber;
}
void O_Stuff::UpdateTargetPosition(const glm::vec3 &position){
    _targetposition = position;
}
void O_Stuff::UpdateTargetOrientation(const glm::vec3 &orientation){
    _targetorientation = orientation;
}
void O_Stuff::UpdateTargetDistance(const float &distance){
    _targetdistance = distance;
}
void O_Stuff::UpdateHit(const bool &value){
    _hit = value;
}
void O_Stuff::UpdateTargetFire(const bool &value){
    _targetfire = value;
}
glm::vec3 O_Stuff::RecieveTargetPosition(){
    return _targetposition;
}
glm::vec3 O_Stuff::RecieveTargetOrientation(){
    return _targetorientation;
}
float O_Stuff::RecieveTargetDistance(){
    return _targetdistance;
}
bool O_Stuff::RecieveHit(){
    return _hit;
}
bool O_Stuff::RecieveTargetFire(){
    return _targetfire;
}
