//
//  endingmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/21.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef endingmanager_hpp
#define endingmanager_hpp

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
extern bool g_GAMEOVER;

class EndingManager
{
private:
    std::vector<Delegator*> _delegate_arr;
    int _life;
    float _terrain;
    glm::vec3 _position;
    
    
    // _ProcessPlayerDead
    bool _playerdead;
    int _playerdeadcount;
    bool _playerdeadanimation;
    float _playerdeadbeziercount;
    glm::vec3 _playerdeadposition;
    void _ProcessPlayerDead();
    
    // _ProcessEndingScore
    bool _ending;
    A_Plane* _curtain;
    float _curtainalpha;
    float _endingscorebeziercount;
    void _ProcessEndingScore();
    
public:
    EndingManager();
    ~EndingManager();
    void Render();
    void GameOver();
    void UpdateDelegator(Delegator* delegator);
};

#endif /* endingmanager_hpp */
