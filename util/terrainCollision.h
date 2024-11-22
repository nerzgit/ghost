//
//  terrain.h
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2018/12/25.
//  Copyright © 2018 IdeKentaro. All rights reserved.
//

#ifndef terrainCollision_h
#define terrainCollision_h

#include <vector>
#include <glm/glm.hpp>

class TerrainCollision
{
private:

    std::vector<glm::vec3> vertices;
    int                    total;
    int                    maxRow;
    float                  lastValue;
    bool                   res;
    float                  posRate;
    
    int                    x;
    int                    z;
    
public:
    
    TerrainCollision(const char *object);
    ~TerrainCollision();
    
    void initOBJ(const char *object);
    void getTerrainCollision(glm::vec3 &pos);
    
};

#endif /* terrain_h */
