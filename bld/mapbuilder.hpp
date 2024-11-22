//
//  mapbuilder.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/04.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef mapbuilder_hpp
#define mapbuilder_hpp

#include <vector>

class O_Stuff;

class MapBuilder
{
private:
    std::vector<O_Stuff*> _o_stuff_prototype_arr;
    
public:
    MapBuilder(const std::string &map);
    ~MapBuilder();
    O_Stuff* CopyStuff(int stuff);
};

#endif /* mapbuilder_hpp */
