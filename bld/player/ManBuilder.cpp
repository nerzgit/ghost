//
//  ManBuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "ManBuilder.hpp"
#include "obj/o_player.hpp"

ManBuilder::ManBuilder(std::string file) : PlayerBuilder(file)
{
    PlayerBuilder::_name = "Man";
}

ManBuilder::~ManBuilder()
{
    
}

void ManBuilder::UpdateInitializer()
{
    // *** 初期化前のこのタイミングでデリゲートを使ってwaltherp38builderから
    // *** Player用キーフレームファイルを取得してくる。もちろんコピーで.
    PlayerBuilder::_o_player = new O_Player(PlayerBuilder::_player_shd_file,
                                            PlayerBuilder::_player_tex_file,
                                            PlayerBuilder::_player_obj_file,
                                            PlayerBuilder::_player_kfm_file,
                                            PlayerBuilder::_player_wgt_file);
}

O_Player* ManBuilder::RecievePlayer()
{
    return PlayerBuilder::_o_player;
}
