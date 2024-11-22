//
//  weapondecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/12.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "weapondecorator.hpp"

void WeaponDecorator::UpdateDecorator(O_Weapon *weapon)
{
    _o_weapon = weapon;
}

void WeaponDecorator::ProcessMuzzleFlash(){}
void WeaponDecorator::UpdateMuzzleFlashConstraintMatrix(glm::mat4 &matrix){}
void WeaponDecorator::UpdateMuzzleFlashCounterToZero(){}
void WeaponDecorator::ProcessBallistic(){}
void WeaponDecorator::UpdateBallisticModelMatrix(glm::mat4 &matrix){}
void WeaponDecorator::UpdateBallisticCounterToZero(){}
