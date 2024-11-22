//
//  playerbuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/06.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef playerbuilder_hpp
#define playerbuilder_hpp

#include "obj/o_player.hpp"
#include <string>


class PlayerBuilder
{
    
protected:
    
    std::string _name;
    
    O_Player*   _o_player;
    
    PlayerStructShader                 _player_shd_file;
    std::vector<PlayerStructTexture>   _player_tex_file;
    std::vector<PlayerStructObject>    _player_obj_file;
    std::vector<PlayerStructKeyframe>  _player_kfm_file;
    std::vector<PlayerStructWeight>    _player_wgt_file;
    
public:
    PlayerBuilder(const std::string &path);
    ~PlayerBuilder();
    virtual O_Player* RecievePlayer(){ return 0; }
    virtual void UpdateInitializer(){}
    void UpdateKeyframeFile(std::vector<PlayerStructKeyframe> &kfm_file);
};

class PlayerDirector
{
public:
    O_Player* RecieveInitializer(PlayerBuilder* builder);
};


#endif /* playerbuilder_hpp */
