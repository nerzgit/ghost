//
//  game.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include <string>

class Delegator;
class MapManager;
class FontManager;
class MenuManager;
class EventManager;
class AudioManager;
class StatusManager;
class PlayerManager;
class EndingManager;
class ControlManager;
class OpeningManager;
class CollisionManager;
class CharacterManager;

class Game
{
private:
    std::string _map;
    MapManager* _mapmanager;
    FontManager* _fontmanager;
    MenuManager* _menumanager;
    EventManager* _eventmanager;
    AudioManager* _audiomanager;
    StatusManager* _statusmanager;
    PlayerManager* _playermanager;
    EndingManager* _endingmanager;
    ControlManager* _controlmanager;
    OpeningManager* _openingmanager;
    CollisionManager* _collisionmanager;
    CharacterManager* _charactermanager;
    
public:
    Game();
    ~Game();
    void Render();
    void Control();
    void UpdateDelegator();
};

#endif /* game_hpp */
