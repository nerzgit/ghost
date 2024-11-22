//
//  vecmath.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/24.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef vecmath_hpp
#define vecmath_hpp

#include <glm/glm.hpp>

glm::vec3 outervec(const glm::vec3 &svec1, const glm::vec3 &svec2);
float innervec(const glm::vec3 &vec1, const glm::vec3 &vec2);
float vlen(const glm::vec3 &vec);
float vangle(const glm::vec3 &vec1, const glm::vec3 &vec2);
glm::vec3 rightvec(const glm::vec3 &src3);

#endif /* vecmath_hpp */
