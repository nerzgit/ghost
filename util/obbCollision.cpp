//
//  obbCollision.cpp
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2019/01/10.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "obbCollision.h"

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

OBB::OBB(const glm::vec3 &pos, const std::vector<glm::vec3> &dir, const glm::vec3 &len, int group)
{
    i_Pos = pos;
    i_Dir = dir;
    i_Len = len;
    m_Dir = { glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(0,0,0) };
    this->group = group;
}

OBB::~OBB(){
    
}

void OBB::Update(const glm::mat4 &Rotate, const glm::mat4 &Trans, const glm::mat4 &Scale)
{
    m_Pos = Trans * glm::vec4(i_Pos, 1.0f);
    for (int i=0; i<i_Dir.size(); i++){
        m_Dir[i] = Rotate * glm::vec4(i_Dir[i], 1.0f);
    }
    m_Len = Scale * glm::vec4(i_Len, 1.0f);
}

glm::vec3 OBB::GetPos(        ) { return m_Pos;       }
glm::vec3 OBB::GetDir(int elem) { return m_Dir[elem]; }
float     OBB::GetLen(int elem) { return m_Len[elem]; }

// 分離軸に投影された軸成分から投影線分長を算出
float OBBCollision::LenSegOnSeparateAxis( glm::vec3 *Sep, glm::vec3 *e1, glm::vec3 *e2, glm::vec3 *e3 )
{
    // 3つの内積の絶対値の和で投影線分長を計算
    // 分離軸Sepは標準化されていること
    float r1 = fabs(glm::dot( *Sep, *e1 ));
    float r2 = fabs(glm::dot( *Sep, *e2 ));
    float r3 = e3 ? (fabs(glm::dot( *Sep, *e3 ))) : 0;
    return r1 + r2 + r3;
}

// OBB v.s. OBB
bool OBBCollision::ColOBBs( OBB *obb1, OBB *obb2 )
{
    // 各方向ベクトルの確保
    // （N***:標準化方向ベクトル）
    glm::vec3 NAe1 = obb1->GetDir(0), Ae1 = NAe1 * obb1->GetLen(0);
    glm::vec3 NAe2 = obb1->GetDir(1), Ae2 = NAe2 * obb1->GetLen(1);
    glm::vec3 NAe3 = obb1->GetDir(2), Ae3 = NAe3 * obb1->GetLen(2);
    glm::vec3 NBe1 = obb2->GetDir(0), Be1 = NBe1 * obb2->GetLen(0);
    glm::vec3 NBe2 = obb2->GetDir(1), Be2 = NBe2 * obb2->GetLen(1);
    glm::vec3 NBe3 = obb2->GetDir(2), Be3 = NBe3 * obb2->GetLen(2);
    glm::vec3 Interval = obb1->GetPos() - obb2->GetPos();
    
    // 分離軸 : Ae1
    float rA = glm::length( Ae1 );
    float rB = LenSegOnSeparateAxis( &NAe1, &Be1, &Be2, &Be3 );
    float L = fabs(glm::dot( Interval, NAe1 ));
    if( L > rA + rB )
        return false; // 衝突していない
    
    // 分離軸 : Ae2
    rA = glm::length( Ae2 );
    rB = LenSegOnSeparateAxis( &NAe2, &Be1, &Be2, &Be3 );
    L = fabs(glm::dot( Interval, NAe2 ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : Ae3
    rA = glm::length( Ae3 );
    rB = LenSegOnSeparateAxis( &NAe3, &Be1, &Be2, &Be3 );
    L = fabs(glm::dot( Interval, NAe3 ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : Be1
    rA = LenSegOnSeparateAxis( &NBe1, &Ae1, &Ae2, &Ae3 );
    rB = glm::length( Be1 );
    L = fabs(glm::dot( Interval, NBe1 ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : Be2
    rA = LenSegOnSeparateAxis( &NBe2, &Ae1, &Ae2, &Ae3 );
    rB = glm::length( Be2 );
    L = fabs(glm::dot( Interval, NBe2 ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : Be3
    rA = LenSegOnSeparateAxis( &NBe3, &Ae1, &Ae2, &Ae3 );
    rB = glm::length( Be3 );
    L = fabs(glm::dot( Interval, NBe3 ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C11
    glm::vec3 Cross;
    Cross = glm::cross( NAe1, NBe1 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
    rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C12
    Cross = glm::cross( NAe1, NBe2 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
    rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C13
    Cross = glm::cross( NAe1, NBe3 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
    rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C21
    Cross = glm::cross( NAe2, NBe1 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
    rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C22
    Cross = glm::cross( NAe2, NBe2 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
    rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C23
    Cross = glm::cross( NAe2, NBe3 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
    rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C31
    Cross = glm::cross( NAe3, NBe1 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
    rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C32
    Cross = glm::cross( NAe3, NBe2 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
    rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離軸 : C33
    Cross = glm::cross( NAe3, NBe3 );
    rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
    rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
    L = fabs(glm::dot( Interval, Cross ));
    if( L > rA + rB )
        return false;
    
    // 分離平面が存在しないので「衝突している」
    return true;
}
