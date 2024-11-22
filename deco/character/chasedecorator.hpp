//
//  chasedecorator.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef chasedecorator_hpp
#define chasedecorator_hpp

#include "deco/characterdecorator.hpp"
#include <string>

class DijkstraAlgorithm;
class LineSegmentData;
class LinearLinearCollision;
class CircleDotCollision;

class ChaseDecorator : public CharacterDecorator
{
private:
    

    // One map Dijkstra
    std::vector<std::vector<int>>   _childnode;
    std::vector<std::vector<float>> _nodelength;
    struct Node{
        glm::vec3 point;
        bool      done;
        Node(const glm::vec3 point)
        {
            this -> point = point;
            this -> done  = false;
        }
    };
    std::vector<Node*>              _node;
    DijkstraAlgorithm*              _dijkstra;
    std::vector<int>                _resultnode;    // Output
    
    

    // One map segmentation for line collision
    struct Line{
        glm::vec3 apoint;
        glm::vec3 bpoint;
        Line(const glm::vec3 apoint, const glm::vec3 bpoint)
        {
            this -> apoint = apoint;
            this -> bpoint = bpoint;
        }
    };
    std::vector<Line*>              _line;
    
    
    
    // Collision
    LinearLinearCollision*          _linearlinearcollision;
    CircleDotCollision*             _circledotcollision;
    
    
    
    // toggle ai on off
    int _ai_on;
    // Available node number
    int _s_node;
    // Zombie new position
    int _nz_node;
    // Player new position
    int _np_node;
    // Zombie Fire Interval
    int _fireinterval;
    
public:
    ChaseDecorator(const std::string &map);
    ~ChaseDecorator();
    void ProcessChase();
    void _InitializeDijkstraAlgorithm(const char* dpfile, const char* sgfile);
};

#endif /* chasedecorator_hpp */
