//
//  mapmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef mapmanager_hpp
#define mapmanager_hpp


#include <glm/glm.hpp>
#include <fstream>
#include <vector>

class O_Stuff;
class RayOBB;
class Delegator;
class MapBuilder;
class MapDecorator;

class MapManager
{
private:
    std::vector<Delegator*> _delegate_arr;
    std::vector<RayOBB*> _rayobb_arr;
    std::vector<O_Stuff*> _o_stuff_arr;
    std::vector<MapBuilder*> _builder_arr;
    std::vector<MapDecorator*> _dust_arr;
    glm::vec3 _muzzleflashposition;
    glm::vec3 _lightposition;
    glm::vec3 _playerposition;
    glm::vec3 _playerorientation;
    glm::vec3 _randomizer;
    bool _playerfire;
    void _InitializeRayOBB();
    void _InitializeMap(const std::string &map);
    void _InitializeDecorator();
    void _ProcessRayOBB();
    
public:
    MapManager();
    ~MapManager();
    void Init(const std::string &map);
    void Render();
    void Reset();
    glm::vec3 UpdateMuzzleFlashPosition(glm::vec3 position);
    glm::vec3 UpdateLightPosition(glm::vec3 position);
    void UpdateDelegator(Delegator* delegator);
};

#endif /* mapmanager_hpp */
