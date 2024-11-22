//
//  clock.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef clock_hpp
#define clock_hpp

class Clock
{
private:
    static Clock* _Instance;
    static double _currenttime;
    static double _lasttime;
    static bool   _fps;
    
protected:
    Clock();
    
public:
    static Clock* Instance();
    static void UpdateFPS();
    static bool RecieveFPS();
};

#endif /* clock_hpp */
