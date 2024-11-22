//
//  chasedecorator.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "chasedecorator.hpp"
#include "del/delegate.hpp"
#include "util/squareroot.hpp"
#include "util/dijkstraAlgorithm.hpp"
#include "util/linear_linearCollision.hpp"
#include "util/circle_dotCollision.hpp"



ChaseDecorator::ChaseDecorator(const std::string &map)
{
    _ai_on   = 0;
    _s_node  = 0;
    _nz_node = 0;
    _np_node = 0;
    _InitializeDijkstraAlgorithm(std::string("game/"+map+"/as/zombie/group1.dpf").c_str(),
                                 std::string("game/"+map+"/as/zombie/group1.sgf").c_str());
}

ChaseDecorator::~ChaseDecorator()
{
    for (auto e : _node) delete e;
    for (auto e : _line) delete e;
    delete _dijkstra;
    delete _linearlinearcollision;
    delete _circledotcollision;
}





/* In Rendering */
void ChaseDecorator::ProcessChase()
{
    
    int v0 = CharacterDecorator::_o_character->RecieveVBO0();
    int v1 = CharacterDecorator::_o_character->RecieveVBO1();

    if (v0 != 1 || !CharacterDecorator::_o_character->RecievePlay() ) {
        return;
    }
    
    // life less than 0
    if (CharacterDecorator::_o_character->RecieveLife() < 1) {
        
        CharacterDecorator::_o_character->UpdateVBO0(2);
        CharacterDecorator::_o_character->UpdateVBO1(0);
        
    }
    
    // Repeat motion
    if (v1 > 60) {
        CharacterDecorator::_o_character->UpdateVBO1(20);
    }
    
    
    
    
    //
    // [AI] Chasing player
    //
    
    glm::vec3 pp = CharacterDecorator::_o_character->RecieveTargetPosition();    // playerposition
    glm::vec3 zp = CharacterDecorator::_o_character->RecievePosition();    // zombie position
    glm::vec3 ca, cb;    // Hit segmentation
    
    
    // Check exist the wall line between player and zombie
    bool hit = false;
    for (int i=0; i<_line.size(); i++) {
        ca = _line[i]->apoint;
        cb = _line[i]->bpoint;
        hit = _linearlinearcollision -> linear_linearCollision(pp.x,
                                                               pp.z,
                                                               zp.x,
                                                               zp.z,
                                                               ca.x,
                                                               ca.z,
                                                               cb.x,
                                                               cb.z);
        if (hit) {
            if( _ai_on == 0 )_ai_on = 1;
            if( _ai_on == 2 )_ai_on = 2;
            break;
        }
    }
    

    // If there are no hit linear
    if (!hit) {
        _ai_on = 0;
    }
    
    // Hit linear even once
    if (hit && _ai_on == 1) {
        _ai_on = 2;
        
        // Any nodes size.
        size_t node_size = _node.size();
        
        // All length between player and node point.
        float  mw_lgs[node_size];
        
        // Finally, min will be _nz_node and _np_node
        int    min;
        
        
        
        //
        // Zombie - find start node
        //
        
        // Calcualte all mwnode length from player.
        for (int i=0; i<node_size; i++)
        {
            glm::vec3 a = _node[i]->point;
            mw_lgs[i] = SquareRoot((cb.x - a.x)*(cb.x - a.x)+(cb.z - a.z)*(cb.z - a.z));
        }
        
        // Find most least node point from player.
        min = 0;
        for (int i=1; i<sizeof(mw_lgs)/sizeof(*mw_lgs); ++i)
        {
            if (mw_lgs[min] > mw_lgs[i]) {
                mw_lgs[min] = mw_lgs[i];
                min = i;
            }
        }
        
        // This is goal point for zombie.
        _nz_node = min;
        
        
        
        //
        // Player - find goal node
        //
        
        // Calcualte all mwnode length from player.
        for (int i=0; i<node_size; i++)
        {
            glm::vec3 a = _node[i]->point;
            mw_lgs[i] = SquareRoot((ca.x - a.x)*(ca.x - a.x)+(ca.z - a.z)*(ca.z - a.z));
        }
        
        // Find most least node point from player.
        min = 0;
        for (int i=1; i<sizeof(mw_lgs)/sizeof(*mw_lgs); ++i)
        {
            if (mw_lgs[min] > mw_lgs[i]) {
                mw_lgs[min] = mw_lgs[i];
                min = i;
            }
        }
        
        // This is goal point for zombie.
        _np_node = min;
        
        // Do dyijkstra algorithm
        _dijkstra -> update(_nz_node, _np_node);
        _dijkstra -> dijkstra();
        _dijkstra -> sample(_resultnode);
        
        // Reset all node
        for (auto x : _node)
        {
            x->done = false;
        }
        _s_node = (int)node_size-1;
        
    }
    
    
    
    // Need zombies angle and position how far from players
    // and how much need length between player and zombies.
    
    glm::vec3 zGPos,  zSPos;
    float     zSca,  zAngle, zSpeed;
    zSpeed = 1.2f;
    
    if (_ai_on == 2) {
        
        // How many exist " -1 " then turn to " -2 "
        for (int i=0; i<_resultnode.size(); i++) {
            if (_resultnode[i] == -1) {
                _resultnode[i] =  -2;
                _s_node --;
            }
        }
        
        // Turn to true, if zombie reached the next node.
        int d_n = _resultnode[_s_node];
        if (d_n != -2) {
            float pointx = _node[d_n]->point.x;
            float pointz = _node[d_n]->point.z;
            bool reached = _circledotcollision -> circle_dotCollision(pointx, pointz, zp.x, zp.z, 1.2f);
            if (reached) {
                _node[d_n]->done = true;
                _s_node --;
            }
            
            if (_s_node < 0) {
                _ai_on = 0;
            }else{
                pp = _node[d_n]->point;
            }
        }
        
    }
    
    // Calculate zombies angle vector to Player.
    zAngle = 1.5f - atan2(zp.z - pp.z,zp.x - pp.x);
    
    // Calculate zs position and save all Scala.
    zSca = SquareRoot((pp.x - zp.x)*(pp.x - zp.x) + (pp.z - zp.z)*(pp.z - zp.z)) * 40.0f;
    zSPos = pp - zp;
    
    // If close to Player, zombie stop and Fire!!!
    if (zSca < 20.0f) {
        
        // Stop!!
        zGPos = zp;
        
        // Fire!!
        // Update fire interval time
        _fireinterval++;
        
        // If reached any interval
        if (_fireinterval == 60  ||
            _fireinterval == 100 ||
            _fireinterval == 180)
        {
            // Update fire to TRUE
            CharacterDecorator::_o_character->UpdateFire(true);
        }else{
            // Update fire to FALSE
            CharacterDecorator::_o_character->UpdateFire(false);
        }
        
        // Reset interval to zero
        if (_fireinterval > 180)
        {
            // Reset fire interval
            _fireinterval = 0;
        }
        
    }else{
        zSPos = pp - zp;
        zGPos = (zSPos / (zSca / zSpeed)) + zp;
    }
    
    float radius = 0.3f;
    
    /* CollisionManager::UpdateTargetCurrentPosition(glm::vec3); */
    (*CharacterDecorator::_delegate_arr[0])(zGPos);
    /* CollisionManager::UpdateTargetLastPosition(glm::vec3); */
    (*CharacterDecorator::_delegate_arr[1])(zp);
    /* CollisionManager::UpdateTargetRadius(float); */
    (*CharacterDecorator::_delegate_arr[2])(radius);
    /* CollisionManager::RecieveTerrainCollision(glm::vec3); */
    glm::vec3 terrainpos = (*CharacterDecorator::_delegate_arr[3])(glm::vec3(0));
    /* CollisionManager::RecieveKabezuriCollision(glm::vec3); */
    zGPos = (*CharacterDecorator::_delegate_arr[4])(glm::vec3(0));
    zGPos.y = terrainpos.y;
                                    
    CharacterDecorator::_o_character->UpdatePosition(zGPos);
    CharacterDecorator::_o_character->UpdateOrientation(glm::vec3(0,zAngle,0));
    
}






void ChaseDecorator::_InitializeDijkstraAlgorithm(const char* dpfile, const char* sgfile)
{
    
    // Initialization of dijkstra node
    FILE * file = fopen(dpfile, "r");
    while( 1 ){
        
        char  line[128];
        
        int res = fscanf(file, "%s", line);
        
        // EOF
        if (res == EOF)
            break;
        
        // Node
        if ( strcmp( line, "n" ) == 0 ){
            int       i;
            glm::vec3 a;
            fscanf(file, "%i %f %f %f\n", &i, &a.x, &a.y, &a.z);
            _childnode.push_back({});       // Add space for child node
            _nodelength.push_back({});      // Add space for child node length
            _node.push_back(new Node(a));   // Create node
            _resultnode.push_back(-1);      // Insert -1 for sampling dijkstra nodes
        }
        
        // Child & Length
        if ( strcmp( line, "c" ) == 0){
            int     i;
            int     c;
            float   l;
            fscanf(file, "%i %i %f\n", &i, &c, &l);
            _childnode[i].push_back(c);     // Add child node number
            _nodelength[i].push_back(l);    // Add child node length
        }
        
    }
    fclose(file);
    
    // Create Dijkstra algorithm
    _dijkstra = new DijkstraAlgorithm(_childnode, _nodelength);
    
    
    
    // Initialization of segmentation for Line collision
    file = fopen(sgfile, "r");
    while( 1 ){
        
        char  line[128];
        
        int res = fscanf(file, "%s", line);
        
        // EOF
        if (res == EOF)
            break;
        
        // Segmentation
        if ( strcmp( line, "s" ) == 0 ){
            int g;
            float ax,az,bx,bz;
            fscanf(file, "%i %f %f %f %f\n", &g, &ax, &az, &bx, &bz);
            _line.push_back( new Line( glm::vec3(ax,0,az), glm::vec3(bx,0,bz) ) );
        }
        
    }
    fclose(file);
    

}
