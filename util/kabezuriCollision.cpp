//
//  collision.cpp
//  play
//
//  Created by PROPPACK DEVELOPER IDE on 2018/12/25.
//  Copyright © 2018 IdeKentaro. All rights reserved.
//

#include "kabezuriCollision.hpp"
#include "usge/objloader.hpp"
#include "util/squareroot.hpp"
#include "linear_circleCollision.hpp"
#include "circle_dotCollision.hpp"

KabezuriCollision::KabezuriCollision(const char *object){
    initCollision(object);
    linearcircleCollision = new LinearCircleCollision();
    circledotCollision    = new CircleDotCollision();
}

KabezuriCollision::~KabezuriCollision(){}

void KabezuriCollision::initCollision(const char *object)
{
    res = loadOBJC(object, vertices);
    
    
    /**
     * Calculate the equation of the straight line
     * (X2 - X1) (Y - Y1) = (Y2 - Y) (X - X1) -> ax+by+c=0
     * a = (Y2 - Y1)
     * b = -(X2 - X1)
     * c = Y2(X2 - X1) - X2(Y2 - Y1)
     */
    
    for (unsigned int i=0; i<vertices.size(); i++) {
        glm::vec3 pointA = vertices[i][0];
        glm::vec3 pointB = vertices[i][1];
        float a,b,c;
        a = (pointB.z - pointA.z);
        b = -(pointB.x - pointA.x);
        c = pointB.z*(pointB.x - pointA.x) - pointB.x*(pointB.z - pointA.z);
        abc.push_back({a,b,c});
    }
    
}

void KabezuriCollision::getCollision(
                        glm::vec3 &posC,
                        glm::vec3 &posL,
                        float &radius
                        )
{
    
    int totalCollision = 0;
    
    float a, b, c, x, z;
    glm::vec3 A, B, posR;
    
    x = posC.x;
    z = posC.z;
    
    
    // Collision 2 : Circle and point
    int totalCollision2 = 0;
    for (unsigned int i=0; i<vertices.size(); i++)
        for (unsigned int l=0; l<vertices[i].size(); l++) {
            glm::vec3 p = vertices[i][l];
            if (circledotCollision->circle_dotCollision(p.x, p.z, x, z, radius)) totalCollision2 ++;
        }
    
    // Collision 1 : KABEZURI
    for (unsigned int i=0; i<abc.size(); i++) {
        
        a = abc[i][0];                                      // Linear function's a
        b = abc[i][1];                                      // Linear function's b
        c = abc[i][2];                                      // Linear function's c
        
        A = vertices[i][0];                                 // Coordinate A of linear
        B = vertices[i][1];                                 // Coordinate B of linear
        
        // Check linear circle collision. Just hit or not.
        res = linearcircleCollision->linear_circleCollision( a, b, c, x, z, radius, A, B, posC );
        
        // If hit, calculate KABEZURI vector.
        if (res) {
            
            // The target linear of the KABEZURI vector.
            float tA = 0.0, tB = 0.0;
            
            // When there are multiple judgment, Do not make to penetrate.
            if (totalCollision2 > 1) {
                tA = nowLines[0];
                tB = nowLines[1];
            }else{
                tA = a;
                tB = b;
                // Save the target linear.
                nowLines[0] = a;
                nowLines[1] = b;
            }
        
            /**
             * Calculation of KABEZURI vector
             * Progressing vector is calculated from the difference in
             * coordinates between collision and one frame before collision.
             * Normal vectors are calculated from the slope of the
             * linear function and converted into unit vectors.
             * Fitted to "w=f-Dot(f*n)*n" solution and add it with the current coordinates.
             */
            
            glm::vec3 vecF = posC - posL;                                     // Math to vector
            
            float sqrtF = SquareRoot(vecF.x*vecF.x+vecF.z*vecF.z);
            glm::vec3 unitvecF = glm::vec3(vecF.x/sqrtF, 0.0, vecF.z/sqrtF);  // convert to unit vector
            
            float sqrtN = SquareRoot(tA*tA+tB*tB);
            glm::vec3 unitvecN = glm::vec3(tA/sqrtN, 0.0, tB/sqrtN);            // convert to unit vector
            
            float dotvec = glm::dot(unitvecF, unitvecN);
            glm::vec3 vecW = unitvecF - dotvec * unitvecN;
            
            vecW.x /= 30.0f;
            vecW.z /= 30.0f;
            
            posR = posL + vecW;                                               // Next frame position
            
            totalCollision += 1;
            
        }
    
    }

    
    // When multiple collisions are detected, return to the coordinates one frame before
    if (totalCollision > 1) {
        posC.x = posL.x;
        posC.z = posL.z;
    }else if(totalCollision == 1){
        posC.x = posR.x;
        posC.z = posR.z;
    }else{}
    
    // To avoid a bug when the calculated value is Nan
    if ( isfinite(posR.x) == 0 && totalCollision > 0 ) {
        posC = posL;
    }
    
}

bool KabezuriCollision::retCollision()
{
    return res;
}
