//
//  collision.hpp
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2018/12/25.
//  Copyright © 2018 IdeKentaro. All rights reserved.
//

#ifndef kabezuriCollision_hpp
#define kabezuriCollision_hpp

#include <vector>
#include <glm/glm.hpp>

class LinearCircleCollision;
class CircleDotCollision;

class KabezuriCollision
{
    
private:
    
    LinearCircleCollision* linearcircleCollision;
    CircleDotCollision*    circledotCollision;
    
    std::vector<std::vector<glm::vec3>> vertices;
    std::vector<std::vector<float>>     abc;
    bool                                res;
    float                               nowLines[2];

    void initCollision(const char *object);
    
public:
    KabezuriCollision(const char *object);
    ~KabezuriCollision();

    void getCollision(
    glm::vec3 &posC,
    glm::vec3 &posL,
    float     &radius
    );
    
    bool retCollision();
    
};

#endif /* collision_hpp */
