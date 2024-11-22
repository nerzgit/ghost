//
//  vecmath.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/24.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "vecmath.hpp"
#include "squareroot.hpp"

//! @brief 三次元ベクトルの外積を求める
//! @param [out] return 求めたベクトル
//! @param [in] svec1 一つ目の三次元ベクトル
//! @param [in] svec2 二つ目の三次元ベクトル
//! @return なし
glm::vec3 outervec(const glm::vec3 &svec1, const glm::vec3 &svec2) {
    
    const float xa = svec1[0];
    const float ya = svec1[1];
    const float za = svec1[2];
    const float xb = svec2[0];
    const float yb = svec2[1];
    const float zb = svec2[2];
    
    return glm::vec3(
                     ya * zb - za * yb,
                     za * xb - xa * zb,
                     xa * yb - ya * xb
                     );
    
}


//! @brief ベクトルの内積を求める
//! @param [in] vec1 一つ目の三次元ベクトル
//! @param [in] vec2 二つ目の三次元ベクトル
//! @return 内積
float innervec(const glm::vec3 &vec1, const glm::vec3 &vec2) {
    const float xa = vec1[0];
    const float ya = vec1[1];
    const float za = vec1[2];
    const float xb = vec2[0];
    const float yb = vec2[1];
    const float zb = vec2[2];
    
    return (xa*xb + ya*yb + za*zb);
}



//! @brief ベクトルの長さを求める関数
//! @param [in] vec 三次元ベクトル
//! @return 長さ
float vlen(const glm::vec3 &vec) {
    const float x = vec[0];
    const float y = vec[1];
    const float z = vec[2];
    
    return SquareRoot(x * x + y * y + z * z);
}



//! @brief 二つのベクトルの角度をラジアンで求める関数
//! @param [in] vec1 一つ目の三次元ベクトル
//! @param [in] vec2 二つ目の三次元ベクトル
//! @return 二つのベクトルのなす角（ラジアン）
float vangle(const glm::vec3 &vec1, const glm::vec3 &vec2) {
    return acos(innervec(vec1, vec2) / (vlen(vec1) * vlen(vec2)));
}



//! @brief ある三次元ベクトルに垂直な三次元ベクトルを求める
//! @param [out] return 結果の格納先
//! @param [in] src3 三次元ベクトル
glm::vec3 rightvec(const glm::vec3 &src3) {
    
    glm::vec3 tmp = { 1,0,0 };
    
    //tmpとsrc3の角度０またはそれに近いなら、別なベクトルを用意
    if (vangle(tmp, src3) < 0.00174533) { //0.00174533rad=1degree
        tmp[0] = 0;
        tmp[1] = 1;
        tmp[2] = 0;
    }
    //外積を求める
    return outervec(tmp, src3);
}
