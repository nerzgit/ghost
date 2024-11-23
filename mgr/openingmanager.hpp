//
//  openingmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/21.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef openingmanager_hpp
#define openingmanager_hpp

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class A_Plane;
class Delegator;
class  Clock;
extern Clock* g_CLOCK;
extern int g_SCREENWIDTH;
extern int g_SCREENHEIGHT;
extern GLFWwindow* g_WINDOW;

class OpeningManager
{
private:
    std::vector<Delegator*> _delegate_arr;
    A_Plane* _page0background;
    A_Plane* _page1background;
    A_Plane* _page2background;
    A_Plane* _page3background;
    A_Plane* _curtain;
    void _ProcessPage0();
    void _ProcessPage1();
    void _ProcessPage2();
    void _ProcessPage3();
    
    // _ProcessStartGame();
    int _processstartgamef_t = 0;
    float _processstartgamecurtain_a = 1;
    void _ProcessStartGame();
    
public:
    OpeningManager();
    ~OpeningManager();
    void Init();
    void Render();
    void Reset();
    void UpdateDelegator(Delegator* delegator);
};

#endif /* openingmanager_hpp */
