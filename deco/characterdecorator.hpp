//
//  characterdecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef characterdecorator_hpp
#define characterdecorator_hpp

#include "obj/o_character.hpp"
#include <vector>

class Delegator;

class CharacterDecorator : public O_Character
{
protected:
    std::vector<Delegator*> _delegate_arr;
    O_Character* _o_character;
    int _enterstyle;
    
public:
    
    virtual void ProcessEnter();
    virtual void ProcessChase();
    virtual void ProcessDead();
    virtual void ProcessBlood();
    
    void UpdateDecorator(O_Character* character);
    void UpdateDelegator(Delegator* delegator);
    void UpdateEnterStyle(const int value);
    
};

#endif /* characterdecorator_hpp */
