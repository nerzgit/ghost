//
//  enterdecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef enterdecorator_hpp
#define enterdecorator_hpp

#include "deco/characterdecorator.hpp"

class EnterDecorator : public CharacterDecorator
{
private:
    int _style;
    int _state;
    
public:
    EnterDecorator();
    ~EnterDecorator();
    void ProcessEnter();
    void UpdateEnterStyle(const int &value);
};

#endif /* enterdecorator_hpp */
