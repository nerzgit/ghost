//
//  linear_linearCollision.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/08.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "linear_linearCollision.hpp"

bool LinearLinearCollision::linear_linearCollision(float &ax,
                                                   float &ay,
                                                   float &bx,
                                                   float &by,
                                                   float &cx,
                                                   float &cy,
                                                   float &dx,
                                                   float &dy)
{
    
    float ta = (cx - dx) * (ay - cy) + (cy - dy) * (cx - ax);
    float tb = (cx - dx) * (by - cy) + (cy - dy) * (cx - bx);
    float tc = (ax - bx) * (cy - ay) + (ay - by) * (ax - cx);
    float td = (ax - bx) * (dy - ay) + (ay - by) * (ax - dx);
    
    return(tc * td < 0 && ta * tb < 0);
    
}
