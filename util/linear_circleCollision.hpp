//
//  linear_circleCollision.hpp
//  play
//
//  Created by IDE KENTARO on 2019/06/08.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef linear_circleCollision_hpp
#define linear_circleCollision_hpp


#include <glm/glm.hpp>

class LinearCircleCollision
{
private:
public:
    bool linear_circleCollision(float     &a,
                                float     &b,
                                float     &c,
                                float     &x,
                                float     &y,
                                float     &radius,
                                glm::vec3 &A,
                                glm::vec3 &B,
                                glm::vec3 &O);
};

#endif /* linear_circleCollision_hpp */
