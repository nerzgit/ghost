//
//  deaddecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef deaddecorator_hpp
#define deaddecorator_hpp

#include "deco/characterdecorator.hpp"

class DeadDecorator : public CharacterDecorator
{
private:
    int _style;
    int _state;
    
public:
    DeadDecorator();
    ~DeadDecorator();
    void ProcessDead();
};

#endif /* deaddecorator_hpp */
