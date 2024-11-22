//
//  terrain.cpp
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2018/12/25.
//  Copyright © 2018 IdeKentaro. All rights reserved.
//

#include "terrainCollision.h"
#include "usge/objloader.hpp"

TerrainCollision::TerrainCollision(const char *object)
{
    initOBJ(object);
}

TerrainCollision::~TerrainCollision()
{
    
}

void TerrainCollision::initOBJ(const char *object)
{
    
    res = loadOBJV(object, vertices, maxRow, lastValue);
    posRate = lastValue / maxRow;
    total = (int)vertices.size();

}

void TerrainCollision::getTerrainCollision(glm::vec3 &pos)
{
    
    x = int(pos.x / posRate);
    z = int(pos.z / posRate);
    
    int num = (maxRow*z)+x+z;
    if (pos.x > 0 &&
        pos.z > 0 &&
        pos.x < lastValue &&
        pos.z < lastValue
        ){
        pos.y = vertices[num].y;
    }else{
        pos.y = 0;
    }
    
}
