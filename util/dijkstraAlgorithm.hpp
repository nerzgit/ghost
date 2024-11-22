//
//  dijkstraAlgorithm.hpp
//  play
//
//  Created by IDE KENTARO on 2019/06/05.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#ifndef dijkstraAlgorithm_hpp
#define dijkstraAlgorithm_hpp

#include <vector>
#include <glm/glm.hpp>

struct DijkstraNode{
    int                id;
    int                parent;
    bool               done;
    float              cost;
    std::vector<int>   child;
    std::vector<float> length;
    DijkstraNode(
                 int                id,
                 float              cost,
                 std::vector<int>   child,
                 std::vector<float> length)
    {
        this -> id      = id;
        this -> parent  = -1;
        this -> done    = false;
        this -> cost    = cost;
        this -> child   = child;
        this -> length  = length;
    }
};

class DijkstraAlgorithm
{
private:
    int start;
    int goal;
    std::vector<DijkstraNode*> nodes;
public:
    DijkstraAlgorithm(std::vector<std::vector<int>>   & child_node_cost,
                      std::vector<std::vector<float>> & child_node_length
                      );
    ~DijkstraAlgorithm();
    void dijkstra();
    void sample(std::vector<int> & out_sample);             // Output sample: {6,5,4,0,-1,-1}
    void update(const int &startnode, const int &goalnode);
};

#endif /* dijkstraAlgorithm_hpp */
