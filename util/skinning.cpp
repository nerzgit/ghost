//
//  skinning.cpp
//  Gme550
//
//  Created by IDE KENTARO on 2019/03/27.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//
//
//  * * *  ATTENTION  * * *
//  This skinning program is different from normal
//  skinning.hpp because it records the
//  rotation angle for each key frame.
//
//

#include "skinning.h"
#include "usge/shader.hpp"
#include "usge/texture.hpp"
#include "usge/objloader.hpp"
#include "usge/vboindexer.hpp"
#include "usge/quaternion_utils.hpp"

using namespace glm;

Bone::~Bone()
{
    delete boneCH;
    delete boneSL;
}

void Bone::initBone(const int       &id,
                    const glm::vec3 &T,
                    const glm::vec3 &R,
                    const glm::vec3 &S,
                    const glm::vec3 &tail)
{
    
    // Parameter initialization
    this -> id = id;
    this -> T  = T;
    this -> R  = R;
    this -> S  = S;
    
    glm::mat4 TranslationMatrix = translate(glm::mat4(1.0), T);
    glm::mat4 RotationMatrix    = eulerAngleYXZ(R.y, R.x, R.z);
    glm::mat4 ScalingMatrix     = scale(glm::mat4(1.0), S);
    matINIT = TranslationMatrix * RotationMatrix * ScalingMatrix;
    
    this -> matOFFSET = glm::inverse(matINIT);
    
    
    // Rendering initialization
    glGenVertexArrays(1, &boneVAO);
    glBindVertexArray(boneVAO);
    
    boneProgramID = LoadShaders( "game/game/sd/Bone.vertexshader", "game/game/sd/Bone.fragmentshader" );
    boneMatrixID  = glGetUniformLocation(boneProgramID, "MVP");
    
    GLfloat bone_vbo_data[6] = {
        0,0,0,tail.x,tail.y,tail.z
    };
    
    GLfloat bone_color_data[6] = {
        (float)  0/255, (float)255/255, (float)255/255,
        (float) 50/255, (float)205/255, (float) 50/255
    };
    
    glGenBuffers(1, &boneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bone_vbo_data), bone_vbo_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &boneCBO);
    glBindBuffer(GL_ARRAY_BUFFER, boneCBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bone_color_data), bone_color_data, GL_STATIC_DRAW);
    
}

void Bone::playBoneMVP(const glm::mat4 &MVP)
{
    boneMVP = MVP * matBONE;
//    renderBone();
}

void Bone::playBoneVEC3(const glm::vec3 &T,
                        const glm::vec3 &R,
                        const glm::vec3 &S,
                        const glm::mat4 &ProjectionMatrix,
                        const glm::mat4 &ViewMatrix)
{
    glm::mat4 TranslationMatrix = translate(glm::mat4(1.0f),T);
    glm::mat4 RotationMatrix = eulerAngleYXZ(R.y, R.x, R.z);
    glm::mat4 ScalingMatrix = scale(glm::mat4(1.0f), S);
    glm::mat4 MVP = TranslationMatrix * RotationMatrix * ScalingMatrix;
    boneMVP = ProjectionMatrix * ViewMatrix * MVP * matBONE;
//    renderBone();
}

void Bone::renderBone()
{

    // If enable renderBone() Object will not showing
    
    glUseProgram(boneProgramID);
    glUniformMatrix4fv(boneMatrixID, 1, GL_FALSE, &boneMVP[0][0]);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, boneVBO);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, boneCBO);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    glDrawElements(
                   GL_LINES,
                   2,
                   GL_UNSIGNED_SHORT,
                   (void*)0
                   );
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
}





Skinning::Skinning()
{}

Skinning::~Skinning()
{
    for (int i=0; i<bones.size(); i++) { delete bones[i]; }
}

void Skinning::initWeight(const char *weightfile)
{
    
    FILE * file = fopen(weightfile, "r");
    
    //
    // Bone initialization
    //
    
    std::vector<int>                    op_i;    // arr_id
    std::vector<int>                    op_c;    // arr_child
    std::vector<int>                    op_s;    // arr_sibling
    std::vector<std::vector<float>>     op_w;    // arr_weight
    std::vector<glm::vec3>              op_h;    // arr_head
    std::vector<glm::vec3>              op_t;    // arr_tail
    
    while( 1 ){
        
        if (file == NULL)
        {
            continue;
        }
        
        char      line[128];
        int       i = 0;           // id
        int       c;               // child
        int       s;               // sibling
        float     w;               // weight
        glm::vec3 h;               // head
        glm::vec3 t;               // tail
        
        int res = fscanf(file, "%s", line);
        
        // EOF
        if (res == EOF)
            break;
        
        // ID
        if ( strcmp( line, "i" ) == 0 ){
            fscanf(file, "%i\n", &i);
            op_i.push_back(i);
            op_w.push_back({});
            // CHILD
        }else if ( strcmp( line, "c" ) == 0 ){
            fscanf(file, "%i\n", &c);
            op_c.push_back(c);
            
            // SIBLING
        }else if ( strcmp( line, "s" ) == 0 ){
            fscanf(file, "%i\n", &s);
            op_s.push_back(s);
            
            // WEIGHT
        }else if ( strcmp( line, "w" ) == 0 ){
            fscanf(file, "%f\n", &w);
            op_w[op_i.back()].push_back(w);
            
            // HEAD
        }else if ( strcmp( line, "h" ) == 0 ){
            fscanf(file, "%f %f %f\n", &h.x, &h.z, &h.y);
            h.y *= -1;
            op_h.push_back(h);
            
            // TAIL
        }else if ( strcmp( line, "t" ) == 0 ){
            fscanf(file, "%f %f %f\n", &t.x, &t.z, &t.y);
            t.y *= -1;
            op_t.push_back(t);
            
        }
        
    }
    fclose(file);
    
    // Bone -> Add ID,WEIGHT,HEAD,TAIL
    for (int i=0; i<op_i.size(); i++) {
        Bone *bone = new Bone();
        for (int l=0; l<op_w[i].size(); l++) {
            bone -> arrWeight.push_back(op_w[i][l]);
        }
        bone -> initBone(op_i[i], op_h[i], glm::vec3(0), glm::vec3(1), op_t[i] - op_h[i]);
        bones.push_back(bone);
    }
    
    // Bone -> Add CHILD
    for (int i=0; i<op_c.size(); i++) {
        if (op_c[i] != -1) {
            bones[i]->boneCH = bones[op_c[i]];
        }
    }
    
    // Bone -> Add SIBLING
    for (int i=0; i<op_s.size(); i++) {
        if (op_s[i] != -1) {
            bones[i]->boneSL = bones[op_s[i]];
        }
    }
    
    // Convert initial pose to relative pose on parent space (Run rootbone)
    glm::mat4 iden = glm::mat4(1.0f);
    convertToRelativeMatrix(bones[0], iden);
}

void Skinning::initKeyframe(std::vector<const char*> keyframefile)
{
    
    //
    // KEYFRAME Initialization
    //
    
    for (int i=0; i<keyframefile.size(); i++) {
        
        FILE * file2 = fopen(keyframefile[i], "r");
        if (file2 == NULL)
        {
            continue;
        }
        
        std::vector<int>                    kf_i;  // Bones id
        
        bones_keyframe.push_back({});
        
        for (int l=0; l<bones.size(); l++) {
            bones_keyframe[bones_keyframe.size()-1].push_back({});
        }
        
        while( 1 ){
            
            char      line[128];
            int res = fscanf(file2, "%s", line);
            
            // EOF
            if (res == EOF)
                break;
            
            // FRAME
            if ( strcmp( line, "r" ) == 0 ){
                fscanf(file2, "%i\n", &frame);
            }
            
            // FPS
            if ( strcmp( line, "f" ) == 0 ){
                fscanf(file2, "%i\n", &fps);
            }
            
            // ID
            else if ( strcmp( line, "i" ) == 0 ){
                int i;
                fscanf(file2, "%i\n", &i);
                kf_i.push_back(i);
            }
            
            // KEYFRAME
            else if ( strcmp( line, "m" ) == 0 ){
                glm::vec3 m;
                fscanf(file2, "%f %f %f\n", &m.x, &m.y, &m.z);
                bones_keyframe[bones_keyframe.size()-1][kf_i.back()].push_back(m);
            }
            
        }
        fclose(file2);
        
    }
    
}

void Skinning::sortWeight(const char                      * object,
                      std::vector<glm::vec3>          & in_vertices,
                      std::vector<std::vector<float>> & out_weights
                      ){
    
    std::vector<glm::vec3>          temp_vertices;
    std::vector<std::vector<float>> temp_weights;
    
    // Get vertices from obj file
    FILE * filevtx = fopen(object, "r");
    while( 1 ){
        char  line[128];
        int res = fscanf(filevtx, "%s", line);
        if (res == EOF)
            break;
        if ( strcmp( line, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(filevtx, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
    }
    fclose(filevtx);
    
    // Sort weights in the order of indexed_vertices.
    // vec3(x1,y1,z1)'sweight   is   W1
    // vec3(x2,y2,z2)'sweight   is   W2
    // vec3(x3,y3,z3)'sweight   is   W3
    // vec3(x4,y4,z4)'sweight   is   W4
    // ...
    for (int z=0; z<bones.size(); z++) {
        temp_weights.push_back({});
        std::cout << "Bones sorting system is available -> " << z << std::endl;
        for (int i=0; i<in_vertices.size(); i++) {
            for (int l=0; l<temp_vertices.size(); l++) {
                if (in_vertices[i].x == temp_vertices[l].x &&
                    in_vertices[i].y == temp_vertices[l].y &&
                    in_vertices[i].z == temp_vertices[l].z) {
                    temp_weights[z].push_back(bones[z]->arrWeight[l]);
                }
            }
        }
    }
    
    out_weights = temp_weights;
    
}

void Skinning::playSkinMVP(const glm::mat4 &MVP, glm::mat4 *out_boneMatrices)
{
    for (int i=0; i<bones.size(); i++) {
        bones[i]->playBoneMVP(MVP);
        out_boneMatrices[i]=bones[i]->matLOCAL;
    }
}

void Skinning::playSkinVEC3(const     glm::vec3 &T,
                        const     glm::vec3 &R,
                        const     glm::vec3 &S,
                        glm::mat4 *out_boneMatrices)
{
    glm::mat4 TranslationMatrix = translate(glm::mat4(1.0f),T);
    glm::mat4 RotationMatrix = eulerAngleYXZ(R.y, R.x, R.z);
    glm::mat4 ScalingMatrix = scale(glm::mat4(1.0f), S);
    glm::mat4 MVP = TranslationMatrix * RotationMatrix * ScalingMatrix;
    for (int i=0; i<bones.size(); i++) {
        bones[i]->playBoneMVP(MVP);
        out_boneMatrices[i]=bones[i]->matLOCAL;
    }
}

void Skinning::renderSkin(const int &vbos0, const int &vbos1)
{

    // 5: Calculate matrix after change of each bone
    for (int i=0; i<bones.size(); i++) {
        // play bone
        glm::vec3 resB = bones_keyframe[vbos0][i][vbos1];
        bones[i]->matBONE = bones[i]->matINIT * eulerAngleYXZ(resB.y, resB.x, resB.z);
    }
    
    // 6: Convert from parent space base to local space base
    glm::mat4 iden = glm::mat4(1.0f);
    updateBone(bones[0], iden);
    
}

void Skinning::convertToRelativeMatrix(Bone *me, glm::mat4 &matParentOFFSET)
{
    me -> matINIT = matParentOFFSET * me -> matINIT;
    if (me -> boneCH) {
        convertToRelativeMatrix(me->boneCH, me->matOFFSET);
    }
    if (me -> boneSL) {
        convertToRelativeMatrix(me->boneSL, matParentOFFSET);
    }
}

void Skinning::updateBone(Bone *me, glm::mat4 &matParentWORLD)
{
    me -> matBONE  = matParentWORLD * me -> matBONE;
    me -> matLOCAL = me -> matBONE  * me -> matOFFSET;
    if (me -> boneCH) {
        updateBone(me -> boneCH, me -> matBONE);
    }
    if (me -> boneSL) {
        updateBone(me -> boneSL, matParentWORLD);
    }
}

glm::mat4 Skinning::getWorldBoneMatrix(const int &bone)
{
    return this->bones[bone]->matBONE;
}

glm::mat4 Skinning::getLocalBoneMatrix(const int &bone)
{
    return this->bones[bone]->matLOCAL;
}

int Skinning::RecieveBonesSize()
{
    return (int)this->bones.size();
}
