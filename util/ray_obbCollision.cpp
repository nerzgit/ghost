//
//  ray_obbCollision.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/10.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "ray_obbCollision.hpp"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


void RayOBBCollision::updateRay(
                         double& mouseX, double& mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
                         int& screenWidth, int& screenHeight,  // Window size, in pixels
                         glm::mat4& ViewMatrix,               // Camera position and orientation
                         glm::mat4& ProjectionMatrix          // Camera parameters (ratio, field of view, near and far planes)
){
    
    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
    glm::vec4 lRayStart_NDC(
                            ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
                            ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
                            -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
                            1.0f
                            );
//    int in_mouseX = mouseX + (double)(rand() % 100 + 1);
//    std::cout << in_mouseX << " : " << mouseX << std::endl;
    glm::vec4 lRayEnd_NDC(
                          ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
                          ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
                          0.0,
                          1.0f
                          );
    
    
//    // The Projection matrix goes from Camera Space to NDC.
//    // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
//    glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);
//
//    // The View Matrix goes from World Space to Camera Space.
//    // So inverse(ViewMatrix) goes from Camera Space to World Space.
//    glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);
//
//    glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
//    glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
//    glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w;
//    glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;
    
    
    // Faster way (just one inverse)+
    glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
    glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
    glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;
    
    
    glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);
    
    
    ray_origin = glm::vec3(lRayStart_world);
    ray_direction = glm::normalize(lRayDir_world);
}


bool RayOBBCollision::ray_obbCollision(RayOBB* rayobb){
    
    // Intersection method from Real-Time Rendering and Essential Mathematics for Games
    
    float tMin = 0.0f;
    float tMax = 100000.0f;
    
    glm::vec3 OBBposition_worldspace(rayobb->M[3].x, rayobb->M[3].y, rayobb->M[3].z);
    
    glm::vec3 delta = OBBposition_worldspace - ray_origin;
    
    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(rayobb->M[0].x, rayobb->M[0].y, rayobb->M[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(ray_direction, xaxis);
        
        if ( fabs(f) > 0.001f ){ // Standard case
            
            float t1 = (e+rayobb->min.x)/f; // Intersection with the "left" plane
            float t2 = (e+rayobb->max.x)/f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections
            
            // We want t1 to represent the nearest intersection,
            // so if it's not the case, invert t1 and t2
            if (t1>t2){
                float w=t1;t1=t2;t2=w; // swap t1 and t2
            }
            
            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if ( t2 < tMax )
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if ( t1 > tMin )
                tMin = t1;
            
            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin )
                return false;
            
        }else{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if(-e+rayobb->min.x > 0.0f || -e+rayobb->max.x < 0.0f)
                return false;
        }
    }
    
    
    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(rayobb->M[1].x, rayobb->M[1].y, rayobb->M[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(ray_direction, yaxis);
        
        if ( fabs(f) > 0.001f ){
            
            float t1 = (e+rayobb->min.y)/f;
            float t2 = (e+rayobb->max.y)/f;
            
            if (t1>t2){float w=t1;t1=t2;t2=w;}
            
            if ( t2 < tMax )
                tMax = t2;
            if ( t1 > tMin )
                tMin = t1;
            if (tMin > tMax)
                return false;
            
        }else{
            if(-e+rayobb->min.y > 0.0f || -e+rayobb->max.y < 0.0f)
                return false;
        }
    }
    
    
    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(rayobb->M[2].x, rayobb->M[2].y, rayobb->M[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(ray_direction, zaxis);
        
        if ( fabs(f) > 0.001f ){
            
            float t1 = (e+rayobb->min.z)/f;
            float t2 = (e+rayobb->max.z)/f;
            
            if (t1>t2){float w=t1;t1=t2;t2=w;}
            
            if ( t2 < tMax )
                tMax = t2;
            if ( t1 > tMin )
                tMin = t1;
            if (tMin > tMax)
                return false;
            
        }else{
            if(-e+rayobb->min.z > 0.0f || -e+rayobb->max.z < 0.0f)
                return false;
        }
    }
    
    rayobb->xpt = tMin;
    return true;
    
}
