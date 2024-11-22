//
//  circle_dotCollision.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/08.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "circle_dotCollision.hpp"

bool CircleDotCollision::circle_dotCollision(
                                            const float &xp,
                                            const float &yp,
                                            const float &xc,
                                            const float &yc,
                                            const float &radius
                                            )
{
    // Whether the point is contained in a circle or not
    if ((xp-xc)*(xp-xc)+(yp-yc)*(yp-yc) <= radius*radius) return true;
    else return false;
}
