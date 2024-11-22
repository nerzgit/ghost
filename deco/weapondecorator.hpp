//
//  weapondecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/12.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef weapondecorator_hpp
#define weapondecorator_hpp

#include "obj/o_weapon.hpp"

class WeaponDecorator : public O_Weapon
{
protected:
    O_Weapon* _o_weapon;
    
public:
    void UpdateDecorator(O_Weapon* weapon);
    virtual void ProcessMuzzleFlash();
    virtual void UpdateMuzzleFlashConstraintMatrix(glm::mat4 &matrix);
    virtual void UpdateMuzzleFlashCounterToZero();
    virtual void ProcessBallistic();
    virtual void UpdateBallisticModelMatrix(glm::mat4 &matrix);
    virtual void UpdateBallisticCounterToZero();
};

#endif /* weapondecorator_hpp */
