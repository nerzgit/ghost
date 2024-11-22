//
//  e_rnd.hpp
//  play
//
//  Created by IDE KENTARO on 2019/06/18.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef e_rnd_hpp
#define e_rnd_hpp

#include <glm/glm.hpp>

double    Randomizer();
glm::vec3 Randomizer(const int min, const int max, const int rate);
double    Randomizer(const int min, const int max);
void      DoRandomizer();

#endif /* e_rnd_hpp */
