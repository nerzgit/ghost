//
//  mapbuilder.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "mapbuilder.hpp"
#include "obj/o_stuff.hpp"
#include "usge/texture.hpp"

MapBuilder::MapBuilder(const std::string &map)
{
    
    // Load bullet hole
    GLuint bulletholetexture = loadDDS_4_Bullet(std::string("game/game/oj/effect/tex/bullethole.dds").c_str());
    // Read ioz file for initialization O_Stuff
    FILE * file = fopen(std::string("game/"+map+"/oj/map/obj/map.ioz").c_str(), "r");
    if( file == NULL ){
        printf(".ioz file not found\n");
        return;
    }
    while( 1 ){
        
        char head[128];
        
        // End of file
        int r = fscanf(file, "%s", head);
        if (r == EOF)
            break;
        
        // Initialize
        if ( strcmp( head, "o" ) == 0 ){
            int a;
            char n[256];
            fscanf(file, "%i %s\n", &a, n);
            std::string s(n);
            // Texture (Default is uvmap.dds)
            std::string tex_path = std::string("game/"+map+"/oj/map/tex/_uvmap.dds").c_str();
            // Object file
            std::string obj_path = std::string("game/"+map+"/oj/map/obj/"+s+".obj").c_str();
            // Obb collision file
            std::string obb_path = std::string("game/"+map+"/oj/map/obj/"+s+".obb").c_str();
            // Vertex shader for O_Stuff
            std::string vsh_path;
            // Fragment shader for O_Stuff
            std::string fsh_path;
            // Select shader
            switch (a) {
                case 0:
                    vsh_path = std::string("game/game/sd/AmbDiff_Bullet.vertexshader").c_str();
                    fsh_path = std::string("game/game/sd/AmbDiff_Bullet.fragmentshader").c_str();
                    break;
                case 1:
                    vsh_path = std::string("game/game/sd/SpecAmbDiff_Bullet.vertexshader").c_str();
                    fsh_path = std::string("game/game/sd/SpecAmbDiff_Bullet.fragmentshader").c_str();
                    break;
                default:
                    vsh_path = std::string("game/game/sd/AmbDiff_Bullet.vertexshader").c_str();
                    fsh_path = std::string("game/game/sd/AmbDiff_Bullet.fragmentshader").c_str();
                    break;
            }
            // Create
            O_Stuff* stuff = new O_Stuff(tex_path.c_str(), obj_path.c_str(), obb_path.c_str(), vsh_path.c_str(), fsh_path.c_str());
            // Update bullet hole texture
            stuff->UpdateBulletHoleTexture(bulletholetexture);
            // Add
            _o_stuff_prototype_arr.push_back(stuff);
        }
        
    }
    fclose(file);

}

MapBuilder::~MapBuilder()
{
    for (auto x : _o_stuff_prototype_arr) delete x;
}

O_Stuff* MapBuilder::CopyStuff(int stuff)
{
    return _o_stuff_prototype_arr[stuff]->Clone();
}
