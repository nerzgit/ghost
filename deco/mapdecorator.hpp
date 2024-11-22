//
//  mapdecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/18.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef mapdecorator_hpp
#define mapdecorator_hpp

#include "obj/o_stuff.hpp"

class MapDecorator : public O_Stuff
{
protected:
    O_Stuff* _o_stuff = nullptr;
    
public:
    void UpdateDecorator(O_Stuff* stuff);
    virtual void ProcessDust();
    virtual void UpdateDustRandomizer(const glm::vec3 &randomizer);
};
#endif /* mapdecorator_hpp */
