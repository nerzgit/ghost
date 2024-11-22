//
//  linear_circleCollision.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/08.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "linear_circleCollision.hpp"
#include "util/squareroot.hpp"

bool LinearCircleCollision::linear_circleCollision(float &a,
                                                   float &b,
                                                   float &c,
                                                   float &x,
                                                   float &z,
                                                   float &radius,
                                                   glm::vec3 &A,
                                                   glm::vec3 &B,
                                                   glm::vec3 &O
                                                   )
{
    
    /**
     * Condition1
     * Finding the distance of line from center.
     */
    float dist = (abs(a*x+b*z+c)) / SquareRoot(a*a+b*b);
    
    
    /**
     * Condition2
     * Finding a point on a line segment from a vector
     */
    glm::vec3 BA = B - A;
    glm::vec3 OA = O - A;
    glm::vec3 OB = O - B;
    
    // Checking if the distance is less than,
    // greater than or equal to radius.
    if (radius >= dist){
        if (glm::dot(OA, BA)>=0 && glm::dot(OB, BA)<=0)
        {
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
    
}
