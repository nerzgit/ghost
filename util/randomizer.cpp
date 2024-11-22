//
//  randm.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/18.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "randomizer.hpp"
#include <random>

std::random_device rnd, rnd1, rnd2, rnd3;
double randm_rnd, randm_rnd1, randm_rnd2, randm_rnd3;


double Randomizer()
{
    return randm_rnd;
}

glm::vec3 Randomizer(const int min, const int max, const int rate)
{
    return glm::vec3(
                     (float)((int)randm_rnd1 % max + min) / rate,
                     (float)((int)randm_rnd2 % max + min) / rate,
                     (float)((int)randm_rnd3 % max + min) / rate
    );
}

double Randomizer(const int min, const int max)
{
    return (int)randm_rnd % max + min;
}

void DoRandomizer()
{
    randm_rnd  = (double)rand();
    randm_rnd1 = rnd1();
    randm_rnd2 = rnd2();
    randm_rnd3 = rnd2();
}
