//
//  menumanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/15.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef menumanager_hpp
#define menumanager_hpp

#include <vector>
#include <string>

class A_Plane;
class Delegator;
class  Clock;
extern Clock* g_CLOCK;
extern int g_SCREENWIDTH;
extern int g_SCREENHEIGHT;
extern bool g_GAMEQUIT;

class MenuManager
{
private:
    std::vector<Delegator*> _delegate_arr;
    bool         _p0, _p1, _p2, _p3, _p4;
    bool         _u, _d, _s, _e;
    bool         _play;
    int          _page;
    int          _pagenumber[2];
    A_Plane* _background;
    A_Plane* _computer;
    A_Plane* _gamepad;
    void _ProcessPageSelect();
    void _ProcessRenderPage0();
    void _ProcessRenderPage1();
    void _ProcessRenderPage2();
    int  _ptrnd;
    void _ProcessRenderPage3();
    void _ProcessRenderPage4();
    
public:
    MenuManager();
    ~MenuManager();
    void Render();
    void UpdateDelegator(Delegator* delegator);
};

#endif /* menumanager_hpp */
