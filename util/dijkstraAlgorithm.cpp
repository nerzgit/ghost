//
//  dijkstraAlgorithm.cpp
//  play
//
//  Created by IDE KENTARO on 2019/06/05.
//  Copyright © 2019 IdeKentaro. All rights reserved.
//

#include "dijkstraAlgorithm.hpp"
#include <iostream>

/** Dijkstra algorithm
 *
 * The 0th of the received "DijkstraData" array is always
 * the start point, and the end of the array is always the
 * goal point.
 *
 */



DijkstraAlgorithm::DijkstraAlgorithm(std::vector<std::vector<int>>   & child_node_cost,
                                     std::vector<std::vector<float>> & child_node_length)
{
    for (int i = 0; i < child_node_cost.size(); i++)
    {
        if (i==0) nodes.push_back(new DijkstraNode(i,        0, child_node_cost[i], child_node_length[i]));
        else      nodes.push_back(new DijkstraNode(i, 99999999, child_node_cost[i], child_node_length[i]));
    }
}

DijkstraAlgorithm::~DijkstraAlgorithm()
{
    
}

void DijkstraAlgorithm::dijkstra()
{
    for(;;)
    {
        
        // Find start node.
        DijkstraNode* donenode = nullptr;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->done || nodes[i]->cost < 0)
            {
                continue;
            }
            
            if (donenode == nullptr || nodes[i]->cost < donenode->cost)
            {
                donenode = nodes[i];
            }
        }
        
        // If not exist node what can update, break the loop.
        if (donenode == nullptr) break;
        
        donenode->done = true;
        
        for (int i = 0; i < donenode->child.size(); i++)
        {
            int t = donenode->child[i];
            float c = donenode->cost + donenode->child[i];
            
            if (nodes[t]->cost < 0 || c < nodes[t]->cost)
            {
                nodes[t]->cost   = c;
                nodes[t]->parent = donenode -> id;
            }
        }
    }
}

void DijkstraAlgorithm::sample(std::vector<int> & out_sample)
{
    
    DijkstraNode* goalnode = nodes[this->goal];
    
    for(int i=0; i<out_sample.size(); i++){
        out_sample[i] = -1;
    }
    
    int count = 0;
    for(;;)
    {
        out_sample[count] = goalnode->id;
        if (goalnode->id == this->start) {
            break;
        }else{
            goalnode = nodes[goalnode->parent];
            count ++;
        }
    }
    
}

void DijkstraAlgorithm::update(const int &startnode, const int &goalnode)
{
    
    /** Set startnode and goalnode.
     *
     * startnode ->
     * Set cost to 0
     *
     * goalnode  ->
     * Find child as goalnode from child in each nodes,
     * Then change length to 999 on same place of child array.
     *
     */
    
    this->start = startnode;
    this->goal = goalnode;
    
    for (int i=0; i<nodes.size(); i++) {
        
        nodes[i]->parent = -1;
        nodes[i]->done   = false;
        
        if (i == startnode) nodes[i]->cost = 0;
        else                nodes[i]->cost = 99999999;
        
        for (int l=0; l<nodes[i]->child.size(); l++) {
            if (nodes[i]->child[l] == goalnode){
                nodes[i]->length[l] = 999;
            }
        }
        
    }
    
}
