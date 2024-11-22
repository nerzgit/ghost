//
//  squareroot.cpp
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2018/12/27.
//  Copyright © 2018 IdeKentaro. All rights reserved.
//

#include "squareroot.hpp"
#include <cmath>

// Instead of current C++ sqrt because it's so slow...
float SquareRoot( const float& x )
{
    float xHalf = 0.5f * x;
    int   tmp   = 0x5F3759DF - ( *(int*)&x >> 1 );
    float xRes  = *(float*)&tmp;
    
    xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );
    xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );
    
    return xRes * x;
}
