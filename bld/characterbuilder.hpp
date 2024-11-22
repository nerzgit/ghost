//
//  characterbuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef characterbuilder_hpp
#define characterbuilder_hpp

#include "obj/o_character.hpp"
#include <string>


class CharacterBuilder
{
    
protected:
    
    std::string _name;
    
    O_Character*   _o_character;
    
    CharacterStructShader    _character_shd_file;
    CharacterStructStatus    _character_sts_file;
    CharacterStructTexture   _character_tex_file;
    CharacterStructKeyframe  _character_kfm_file;
    CharacterStructWeight    _character_wgt_file;
    CharacterStructObject    _character_obj_file;
    CharacterStructIntel     _character_ite_file;
    
public:
    CharacterBuilder(const std::string &path);
    ~CharacterBuilder();
    virtual O_Character* RecieveCharacter(){ return 0; }
    virtual O_Character* RecieveCopyCharacter(){ return 0; }
    CharacterStructIntel RecieveCharacterIntelFile();
};

class CharacterDirector
{
public:
    O_Character* RecieveInitializer(CharacterBuilder* builder);
    O_Character* RecieveCopy(CharacterBuilder* builder);
    CharacterStructIntel RecieveCharacterIntelFile(CharacterBuilder* builder);
};


#endif /* characterbuilder_hpp */
