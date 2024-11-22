//
//  zombiebuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/08.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "zombiebuilder.hpp"
#include "obj/o_character.hpp"

ZombieBuilder::ZombieBuilder(std::string file) : CharacterBuilder(file)
{
    CharacterBuilder::_name = "Zombie";
}

ZombieBuilder::~ZombieBuilder()
{
}

O_Character* ZombieBuilder::RecieveCharacter()
{
    CharacterBuilder::_o_character = new O_Character(CharacterBuilder::_character_shd_file,
                                                     CharacterBuilder::_character_tex_file,
                                                     CharacterBuilder::_character_obj_file,
                                                     CharacterBuilder::_character_kfm_file,
                                                     CharacterBuilder::_character_wgt_file,
                                                     CharacterBuilder::_character_sts_file,
                                                     CharacterBuilder::_character_ite_file);
    return CharacterBuilder::_o_character;
}

O_Character* ZombieBuilder::RecieveCopyCharacter()
{
    return CharacterBuilder::_o_character->Clone();
}
