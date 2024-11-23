//
//  obbCollision.h
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2019/01/10.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef obbCollision_h
#define obbCollision_h

#include <vector>

#include <glm/glm.hpp>

struct OBB
{
    
public:
    
    // *** Using in CollisionManager ***
    int  group = -1;
    int  index = -1;
    bool hit   = false;
    
    OBB(const glm::vec3 &pos, const std::vector<glm::vec3> &dir, const glm::vec3 &len, int group);
    ~OBB();
    
    void Update(const glm::mat4 &Rotate, const glm::mat4 &Trans, const glm::mat4 &Scale);
    
    glm::vec3 GetPos();                  // 位置を取得
    glm::vec3 GetDir( int elem );        // 指定軸番号の方向ベクトルを取得
    float     GetLen( int elem );        // 指定軸方向の長さを取得

    
protected:
    
    glm::vec3              i_Pos;        // 初期位置
    std::vector<glm::vec3> i_Dir;        // 初期方向ベクトル
    glm::vec3              i_Len;        // 初期各軸方向の長さ
    
    glm::vec3              m_Pos;        // 利用位置
    std::vector<glm::vec3> m_Dir;        // 利用方向ベクトル
    glm::vec3              m_Len;        // 利用各軸方向の長さ
    
};

class OBBCollision{
public:
    float LenSegOnSeparateAxis( glm::vec3 *Sep, glm::vec3 *e1, glm::vec3 *e2, glm::vec3 *e3 = 0 );
    bool ColOBBs( OBB *obb1, OBB *obb2 );
    
};


#endif /* obbCollision_h */
