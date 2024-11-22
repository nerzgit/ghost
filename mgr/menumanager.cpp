//
//  menumanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/15.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "menumanager.hpp"
#include "usge/openglc.hpp"
#include "del/delegate.hpp"
#include "abs/a_plane.hpp"
#include "usge/clock.hpp"
#include "usge/key.hpp"

MenuManager::MenuManager()
{
    _p0 = _p1 = _p2 = _p3 = _p4 = 0;
    _u  = _d  = _s  = _e  = false;
    _play = false;
    _page = 0;
    _background = new A_Plane("game/game/oj/_another_/menu/background.dds",100.0f);
    _computer   = new A_Plane("game/game/oj/_another_/menu/laptop.dds", 0.3f);
    _gamepad    = new A_Plane("game/game/oj/_another_/menu/gamepad.dds", 0.3f);
    
}

MenuManager::~MenuManager()
{
    for (auto e : _delegate_arr) delete e;
    delete _background;
    delete _computer;
    delete _gamepad;
}

void MenuManager::Render()
{
    
    //
    // Menu (Toggle)
    //
    
    // For toggle button to show menu
    // This int "a" need to switching
    // a = 4 :: Ready to show
    // a = 1 :: Ready to toggle OFF
    // a = 2 :: Ready to hide
    // a = 3 :: REady to toggle ON
    static int a = 4;
    
    // Each key to switch menu
    bool ps = padcon(PAD_START, KEY_ON);
    bool ke = keycon(KEY_ESC,   KEY_ON);
    bool pe = padcon(PAD_ESC,   KEY_ON);

    
    // On
    if (ps || ke) {
        if (a == 4) {
            a = 1;
            _play = true;
            
            // PlayerManager::playermanager_UpdatePlay(int);
            (*_delegate_arr[6])(0);
            // ControlManager::controlmanager_UpdatePlay(int);
            (*_delegate_arr[7])(0);
            // CharacterManager::charactermanager_UpdatePlay(int);
            (*_delegate_arr[8])(0);
            // AudioManager::audiomanager_PauseSound();
            (*_delegate_arr[10])();
            
        }
    }
    // Toggle on off
    if (!ps && !ke){
        if (a == 1) a = 2;
        else if (a == 3) a = 4;
    }
    
    // Show? or not?
    if (!_play) return;
    
    // p0 need to avoid close menu ignore page0
    if (_p0) {
        // Off
        if (pe || ke) {
            if (a == 2) {
                a = 3;
                _play = false;
                
                // PlayerManager::playermanager_UpdatePlay(int);
                (*_delegate_arr[6])(1);
                // ControlManager::controlmanager_UpdatePlay(int);
                (*_delegate_arr[7])(1);
                // CharacterManager::charactermanager_UpdatePlay(int);
                (*_delegate_arr[8])(1);
                // AudioManager::audiomanager_ResumeSound();
                (*_delegate_arr[9])();
                
            }
        }
    }
    
    
    
    
    //
    // Menu (In menu)
    //
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Draw background
    _background -> UpdatePosition(glm::vec3(0,0,0));
    _background -> UpdateColor(glm::vec4(1,1,1,0.8));
    _background -> Render();
    
    // Control menu
    _ProcessPageSelect();
    
    // Which page is available?
    _p0=_p1=_p2=_p3=_p4=0;
    
    // Available any page
    switch (_page) {
        case 0:
            // Menu
            // Avoid hide menu without page0
            if (!ps && !ke && !pe) _p0=1;
            _ProcessRenderPage0();
            break;
            
        case 1:
            // Control
            _p1=1;
            _ProcessRenderPage1();
            break;
            
        case 2:
            // Credits
            _p2=1;
            _ProcessRenderPage2();
            break;
            
        case 3:
            // Quit game
            _p3=1;
            _ProcessRenderPage3();
            break;
            
        case 4:
            // Quit game
            _p4=1;
            _ProcessRenderPage4();
            break;
            
        default:
            break;
    }
    
}

void MenuManager::_ProcessPageSelect()
{
    
    static bool loop = false;
    
    static int t = 0;
    if (keycon(KEY_LOOKUP_U, KEY_ON ) || keycon(KEY_LOOKUP_D, KEY_ON ) || stickcon(PAD_STRAIGHT_FB)<-0.6 || stickcon(PAD_STRAIGHT_FB)> 0.6) t++;
    if (keycon(KEY_LOOKUP_U, KEY_OFF) && keycon(KEY_LOOKUP_D, KEY_OFF) && stickcon(PAD_STRAIGHT_FB)< 0.6 && stickcon(PAD_STRAIGHT_FB)>-0.6) t=0;
    if (t > 15) loop = true; else loop = false;
    
    
    // Rapidly
    if (loop) {
        
        _u = _d = 0;
        
        if (g_CLOCK->RecieveFPS()) {
            if (keycon(KEY_LOOKUP_U, KEY_ON ) || stickcon(PAD_STRAIGHT_FB)<-0.6) _u = 1;
            if (keycon(KEY_LOOKUP_D, KEY_ON ) || stickcon(PAD_STRAIGHT_FB)> 0.6) _d = 1;
        }
        
        
        // Single
    }else{
        
        static bool key_free = false;
        
        _u = _d = _s = _e = 0;
        
        
        if (g_CLOCK->RecieveFPS()) {
            
            if ((
                keycon(KEY_LOOKUP_U, KEY_OFF )  &&
                keycon(KEY_LOOKUP_D, KEY_OFF )  &&
                keycon(KEY_START,    KEY_OFF )  &&
                keycon(KEY_ESC,      KEY_OFF )
                ) && (
                !padcon(PAD_ACTION,   KEY_ON )  &&
                !padcon(PAD_ESC,      KEY_ON )  &&
                stickcon(PAD_STRAIGHT_FB)< 0.6  &&
                stickcon(PAD_STRAIGHT_FB)>-0.6)
                )
            {
                key_free = false;
            }
            
            if (key_free == true) return;
            
            if (keycon(KEY_LOOKUP_U, KEY_ON  ) || stickcon(PAD_STRAIGHT_FB)<-0.6) key_free = _u = 1;
            if (keycon(KEY_LOOKUP_D, KEY_ON  ) || stickcon(PAD_STRAIGHT_FB)> 0.6) key_free = _d = 1;
            if (keycon(KEY_START,    KEY_ON  ) || padcon(PAD_ACTION, KEY_ON))     key_free = _s = 1;
            if (keycon(KEY_ESC,      KEY_ON  ) || padcon(PAD_ESC,    KEY_ON))     key_free = _e = 1;
            
        }
        
    }

}

void MenuManager::_ProcessRenderPage0()
{
    
    // Selectable page
    static int max_page = 4;
    static int min_page = 1;
    static int now_page = min_page;
    
    // Pageup
    if (_u) {
        now_page--;
        if (now_page < min_page) {
            now_page = max_page;
        }
    }
    
    // Pagedown
    if (_d) {
        now_page++;
        if (now_page > max_page) {
            now_page = min_page;
        }
    }
    
    // Go to next page
    if (_s) {
        _page = now_page;
        if (_page == 4) {
            // Rand for page4
            _ptrnd = rand() % 10 + 1;
        }
    }
    
    // Draw page0
    std::vector<std::string> txt = {
        "CONTROL" ,
        "DISPLAY",
        "CREDIT" ,
        "QUITGAME",
    };
    float col[3];
    float pos[12] = {
        0.5,0.65,2.8,
        0.5,0.55,2.8,
        0.5,0.45,2.8,
        0.5,0.35,2.8,
    };
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i=min_page-1; i<max_page; i++) {
        int g = i;
        // Change selected text color
        if (i+1 == now_page) {
            col[0] = 1;
            col[1] = 0.6235;
            col[2] = 0.0392;
        }else{
            col[0] = 1;
            col[1] = 1;
            col[2] = 0.98;
        }
        
        (*_delegate_arr[0])(txt[g]);                            // Text
        (*_delegate_arr[1])("center");                          // Align
        (*_delegate_arr[2])(glm::vec2(pos[g*3+0], pos[g*3+1])); // Position
        (*_delegate_arr[3])(glm::vec3(col[0], col[1], col[2])); // Color
        (*_delegate_arr[4])(pos[g*3+2]);                        // Size
        (*_delegate_arr[5])();                                  // Render(OpenSansRegular)
        
    }
    
}

void MenuManager::_ProcessRenderPage1()
{
    
    // For console like scroll
    static float m_h = 0;
    // For edit key asset
    static bool editmode = false;
    
    // Selectable page
    static int max_page = 38;
    static int min_page = 1;
    static int now_page = min_page;
    
    if (!editmode) {
        
        // Pageup
        if (_u) {
            now_page--;
            if (now_page < min_page) {
                now_page = min_page;
            }
            m_h -= g_SCREENHEIGHT / 10;
        }
        
        // Pagedown
        if (_d) {
            now_page++;
            if (now_page > max_page) {
                now_page = max_page;
            }
            m_h += g_SCREENHEIGHT / 10;
        }
        
        // Go to next page
        if (_s) {
            editmode = true;
        }
        
    }else{
        
        bool updatekey = false;
        
        int i = now_page-1;
        
        if (-1 < i && i < 17) {
            // :: PC key asset ::
            // now_page-1!! because linking key_asset_pc's order ^_^
            updatekey = change_key_asset_pc(now_page-1);
        }else if (i == 19) {
            // :: JOYSTICK key asset (Aim) ::
            updatekey = change_key_asset_joystick(now_page-1);
        }else if (i == 23) {
            // :: JOYSTICK key asset (Fire) ::
            updatekey = change_key_asset_joystick(now_page-1);
        }else if (23 < i && i < 28) {
            // :: JOYSTICK key asset ::
            updatekey = change_key_asset_joystick(now_page-1);
        }else if (i == 34) {
            (*_delegate_arr[11])(1);
            editmode = false;
        }else if (i == 35) {
            (*_delegate_arr[12])(1);
            editmode = false;
        }else if (i == 36) {
            (*_delegate_arr[11])(0);
            editmode = false;
        }else if (i == 37) {
            (*_delegate_arr[12])(0);
            editmode = false;
        }else{
            // :: GAMEPAD key asset ::
            updatekey = change_key_asset_gamepad(now_page-1);
        }
        
        if (updatekey)
        {
            editmode = false;
        }
        
    }
    
    // Escape menu
    if (_e) {
        
        if (editmode) {
            editmode = false;
        }else{
            _page = 0;
        }
        
    }
    
    // Texts
    std::vector<std::string> txt = {
        // KEY
        "WEAPON CHANGE" ,
        "WEAPON RELOAD" ,
        "WEAPON AIM" ,
        "JUMP" ,
        "RUN" ,
        "ACTION" ,
        "WEAPON FIRE" ,
        "MOVE FORWARD" ,
        "MOVE BACKWARD" ,
        "MOVE RIGHT" ,
        "MOVE LEFT" ,
        "LOOK UP" ,
        "LOOK DOWN" ,
        "LOOK RIGHT" ,
        "LOOK LEFT" ,
        "PAUSE" ,
        "BACK" ,
        // GAMEPAD
        "WEAPON CHANGE" ,
        "WEAPON RELOAD" ,
        "WEAPON AIM :: L2 ::" ,
        "JUMP" ,
        "RUN :: L3 ::" ,
        "ACTION" ,
        "WEAPON FIRE :: R2 ::" ,
        "MOVE FORWARD / BACKWARD :: JOYSTICK ::" ,
        "MOVE RIGHT / LEFT :: JOYSTICK ::" ,
        "LOOK UP / DOWN :: JOYSTICK ::" ,
        "LOOK RIGHT / LEFT :: JOYSTICK ::" ,
        "ARROW UP" ,
        "ARROW DOWN" ,
        "ARROW RIGHT" ,
        "ARROW LEFT" ,
        "PAUSE" ,
        "BACK" ,
        // COMMON
        "INVERT X AXIS" ,
        "INVERT Y AXIS" ,
        "UNINVERT X AXIS" ,
        "UNINVERT Y AXIS" ,
    };
    
    // For console like scroll
    static float d = 0;
    if (m_h > g_SCREENHEIGHT) {
        m_h = g_SCREENHEIGHT;
        d ++;
        if (now_page == max_page){
            m_h = g_SCREENHEIGHT;
            d--;
        }
    }else if (m_h < 0) {
        m_h = 0;
        d --;
        if (now_page == min_page){
            m_h = 0;
            d = 0;
        }
    }
    float h = 1.0 + d*0.04;
    
    float img1_h = 0, img2_h = 0;
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Draw
    for (int i=0; i<max_page; i++) {
        int g = i;
        float col[3];
        // Change selected text color
        if (i == now_page-1) {
            if (editmode) {
                col[0] = 0.1882;
                col[1] = 0.8196;
                col[2] = 0.3451;
            }else{
                col[0] = 1;
                col[1] = 0.6235;
                col[2] = 0.0392;
            }
        }else{
            col[0] = 1;
            col[1] = 1;
            col[2] = 0.98;
        }
        
        h -= 0.04;
        if (i == 0) {
            img1_h = h - 0.15;
            h -= 0.2;
        }else if (i == 17) {
            img2_h = h - 0.15;
            h -= 0.2;
        }else if (i == 34) {
            h -= 0.05;
        }
        
        if (i < 34) {
            
            (*_delegate_arr[0])(txt[g]);                            // Text
            (*_delegate_arr[1])("right");                           // Align
            (*_delegate_arr[2])(glm::vec2(0.49, h));                // Position
            (*_delegate_arr[3])(glm::vec3(col[0], col[1], col[2])); // Color
            (*_delegate_arr[4])(2.8f);                              // Size
            (*_delegate_arr[5])();                                  // Render(OpenSansRegular)
            
            (*_delegate_arr[0])(get_key_name(g));                   // Text
            (*_delegate_arr[1])("left");                            // Align
            (*_delegate_arr[2])(glm::vec2(0.51, h));                // Position
            (*_delegate_arr[3])(glm::vec3(col[0], col[1], col[2])); // Color
            (*_delegate_arr[4])(2.8f);                              // Size
            (*_delegate_arr[5])();                                  // Render(OpenSansRegular)
            
        }else{
            
            (*_delegate_arr[0])(txt[g]);                            // Text
            (*_delegate_arr[1])("center");                          // Align
            (*_delegate_arr[2])(glm::vec2(0.49, h));                // Position
            (*_delegate_arr[3])(glm::vec3(col[0], col[1], col[2])); // Color
            (*_delegate_arr[4])(2.8f);                              // Size
            (*_delegate_arr[5])();                                  // Render(OpenSansRegular)
            
        }
        
    }
    
    _computer -> UpdatePosition(glm::vec3( 1.0 - 0.095, img1_h* /*Fetch*/(1.7+(0.04*8)) ,0 ));
    _computer -> UpdateColor(glm::vec4(1,1,1,1));
    _computer -> Render();
    _gamepad  -> UpdatePosition(glm::vec3( 1.0 - 0.095, img2_h* /*Fetch*/(1.7+(0.04*8)) ,0 ));
    _gamepad  -> UpdateColor(glm::vec4(1,1,1,1));
    _gamepad  -> Render();
    
}

void MenuManager::_ProcessRenderPage2()
{
    
    // For console like scroll
    static float m_h = 0;
    // For edit key asset
    static bool editmode = false;
    
    // Selectable page
    static int max_page = 24;
    static int min_page = 1;
    static int now_page = min_page;
    
    if (!editmode) {
        
        // Pageup
        if (_u) {
            now_page--;
            if (now_page < min_page) {
                now_page = min_page;
            }
            m_h -= g_SCREENHEIGHT / 10;
        }
        
        // Pagedown
        if (_d) {
            now_page++;
            if (now_page > max_page) {
                now_page = max_page;
            }
            m_h += g_SCREENHEIGHT / 10;
        }
        
        // Go to next page
        if (_s) {
            editmode = true;
        }
        
    }else{
        
        int i = now_page-1;
        
        switch (i) {
            case  0: FullScreenMode(); break;
            case  1: WindowMode(); break;
            case  2: DoubleViewPort(); break;
            case  3: NormalViewPort(); break;
            case  4: WindowSize(2560, 1440); break;
            case  5: WindowSize(2304, 1440); break;
            case  6: WindowSize(1920, 1080); break;
            case  7: WindowSize(1680, 1050); break;
            case  8: WindowSize(1536, 864 ); break;
            case  9: WindowSize(1440, 900 ); break;
            case 10: WindowSize(1366, 768 ); break;
            case 11: WindowSize(1280, 1024); break;
            case 12: WindowSize(1280, 800 ); break;
            case 13: WindowSize(1280, 720 ); break;
            case 14: WindowSize(1152, 720 ); break;
            case 15: WindowSize(960 , 640 ); break;
            case 16: WindowSize(960 , 540 ); break;
            case 17: WindowSize(854 , 480 ); break;
            case 18: WindowSize(640 , 360 ); break;
            case 19: WindowSize(576 , 480 ); break;
            case 20: WindowSize(480 , 320 ); break;
            case 21: WindowSize(320 , 240 ); break;
            case 22: WindowSize(176 , 144 ); break;
            case 23: WindowSize(160 , 120 ); break;
            default: break;
        }
        
        editmode = false;
        
    }
    
    // Escape menu
    if (_e) {
        
        _page = 0;
        
    }
    
    // Texts
    std::vector<std::string> txt = {
        "CHANGE TO FULLSCREEN MODE",
        "CHANGE TO WINDOW MODE",
        "DOUBLE VIEWPORT (FOR RETINA)",
        "NORMAL VIEWPORT",
        "RESOLUTION TO 2560 * 1440",
        "RESOLUTION TO 2304 * 1440",
        "RESOLUTION TO 1920 * 1080",
        "RESOLUTION TO 1680 * 1050",
        "RESOLUTION TO 1536 * 864",
        "RESOLUTION TO 1440 * 900",
        "RESOLUTION TO 1366 * 768",
        "RESOLUTION TO 1280 * 1024",
        "RESOLUTION TO 1280 * 800",
        "RESOLUTION TO 1280 * 720",
        "RESOLUTION TO 1152 * 720",
        "RESOLUTION TO 960 * 640",
        "RESOLUTION TO 960 * 540",
        "RESOLUTION TO 854 * 480",
        "RESOLUTION TO 640 * 360",
        "RESOLUTION TO 576 * 480",
        "RESOLUTION TO 480 * 320",
        "RESOLUTION TO 320 * 240",
        "RESOLUTION TO 176 * 144",
        "RESOLUTION TO 160 * 120",
    };
    
    // For console like scroll
    static float d = 0;
    if (m_h > g_SCREENHEIGHT) {
        m_h = g_SCREENHEIGHT;
        d ++;
        if (now_page == max_page){
            m_h = g_SCREENHEIGHT;
            d--;
        }
    }else if (m_h < 0) {
        m_h = 0;
        d --;
        if (now_page == min_page){
            m_h = 0;
            d = 0;
        }
    }
    float h = 1.0 + d*0.04;
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Draw
    for (int i=0; i<max_page; i++) {
        int g = i;
        float col[3];
        // Change selected text color
        if (i == now_page-1) {
            if (editmode) {
                col[0] = 0.1882;
                col[1] = 0.8196;
                col[2] = 0.3451;
            }else{
                col[0] = 1;
                col[1] = 0.6235;
                col[2] = 0.0392;
            }
        }else{
            col[0] = 1;
            col[1] = 1;
            col[2] = 0.98;
        }
        
        h -= 0.04;
        if (i == 0) {
            h -= 0.2;
        }
        
        (*_delegate_arr[0])(txt[g]);                            // Text
        (*_delegate_arr[1])("center");                          // Align
        (*_delegate_arr[2])(glm::vec2(0.50, h));                // Position
        (*_delegate_arr[3])(glm::vec3(col[0], col[1], col[2])); // Color
        (*_delegate_arr[4])(2.8f);                              // Size
        (*_delegate_arr[5])();                                  // Render(OpenSansRegular)
        
    }
    
}

void MenuManager::_ProcessRenderPage3()
{
    
    static float d = 0;
    d += 0.05;
    
    // Selectable page
    static int max_page = (int)72;
    static int min_page = 1;
    
    // Go to next page
    if (_s) {
        d += 0.4;
    }
    
    // Escape menu
    if (_e) {
        _page = d = 0;
    }
    
    // Draw page0
    std::vector<std::string> txt = {
        "COD SOZ VER 0.9"," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        "PROGRAMMER","IDEKENTARO",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        "ARTIST","IDEKENTARO",
        "DESIGNER","IDEKENTARO",
        "MUSIC","IDEKENTARO" ,
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        " "," ",
        "THANK YOU FOR PLAYING THIS GAME."," ",
        "ALSO THIS GAME IS STILL TEST VERSION."," ",
        "I THINK NEED MORE PROGRESS TO BE A FUN GAME."," ",
        "BUT NOW I'M LITTLE TIRED."," ",
        "I STILL NOT DECIDE WHEN RELEASE THE NEXT VERSION."," ",
        "AND LAST WORDS, THIS GAME WAs PROGRAMMED BY OPENGL AND C++ ONLY."," ",
        " "," ",
        " "," ",
    };
    float h = 0.0 + d*0.04;
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i=min_page-1; i<max_page; i+=2) {
        int g = i;
        
        h -= 0.04;
        
        if (txt[g] != " " && txt[g+1] == " "){
            (*_delegate_arr[0])(txt[g]);                // Text
            (*_delegate_arr[1])("center");              // Align
            (*_delegate_arr[2])(glm::vec2(0.5, h));     // Position
            (*_delegate_arr[3])(glm::vec3(1, 1, 0.98)); // Color
            (*_delegate_arr[4])(2.8f);                  // Size
            (*_delegate_arr[5])();                      // Render(OpenSansRegular)
        }else if(txt[g] == " " && txt[g+1] == " "){
            continue;
        }else{
            (*_delegate_arr[0])(txt[g]);                // Text
            (*_delegate_arr[1])("right");               // Align
            (*_delegate_arr[2])(glm::vec2(0.49, h));    // Position
            (*_delegate_arr[3])(glm::vec3(1, 1, 0.98)); // Color
            (*_delegate_arr[4])(2.8f);                  // Size
            (*_delegate_arr[5])();                      // Render(OpenSansRegular)
            
            (*_delegate_arr[0])(txt[g+1]);              // Text
            (*_delegate_arr[1])("left");                // Align
            (*_delegate_arr[2])(glm::vec2(0.51, h));    // Position
            (*_delegate_arr[3])(glm::vec3(1, 1, 0.98)); // Color
            (*_delegate_arr[4])(2.8f);                  // Size
            (*_delegate_arr[5])();                      // Render(OpenSansRegular)
        }
        
    }
}

void MenuManager::_ProcessRenderPage4()
{
    
    // Selectable page
    static int max_page = 3;
    static int min_page = 1;
    static int now_page = min_page+1;
    
    // Pageup
    if (_u) {
        now_page--;
        if (now_page < min_page + 1) {
            now_page = min_page + 1;
        }
    }
    
    // Pagedown
    if (_d) {
        now_page++;
        if (now_page > max_page) {
            now_page = max_page;
        }
    }
    
    // Go to next page
    if (_s) {
        if (now_page == 2) {
            
            // Quit game
            g_GAMEQUIT = true;
            
        }else if (now_page == 3) {
            _page = 0;
        }
    }
    
    // Escape menu
    if (_e) {
        _page = 0;
    }
    
    // Draw page0
    std::vector<std::string> tnt = {
        "HEY!! PLAYER!! QUIT THIS GAME!?",
        "PLAY PLAY PLAY PLAY! IT'S ALWAYS THE FIRST PRIORITY!",
        "ARE YOU QUIT THE GAME?",
        "DON'T!! DON'T GO!! I WANT TO PLAY WITH YOU MORE ;_;",
        "SIESTA?",
        "ARE YOU QUIET THE GAME?",
        "OOPS!! SORRY BOSS. I WAS PLAYING THE GAME",
        "KILLME!! KILLME RIGHT HERE RIGHT NOW!! I'M NOT AFRAID TO DIE!!",
        "ARE YOU QUIT THE GAME?",
        "KINKYUU SIREI. KIMI HA KAISYA DE GAME WO SHITEIRU. KOKODE YAMERUNOKA?",
    };
    std::vector<std::string> txt = {
        tnt[_ptrnd-1] ,
        "YES" ,
        "NO",
    };
    
    float col[3];
    float pos[9] = {
        0.5,0.55,2.8,
        0.5,0.45,2.8,
        0.5,0.40,2.8,
    };
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i=min_page-1; i<max_page; i++) {
        int g = i;
        // Change selected text color
        if (i+1 == now_page) {
            col[0] = 1;
            col[1] = 0.6235;
            col[2] = 0.0392;
        }else{
            col[0] = 1;
            col[1] = 1;
            col[2] = 0.98;
        }
        
        (*_delegate_arr[0])(txt[g]);                            // Text
        (*_delegate_arr[1])("center");                          // Align
        (*_delegate_arr[2])(glm::vec2(pos[g*3+0], pos[g*3+1])); // Position
        (*_delegate_arr[3])(glm::vec3(col[0], col[1], col[2])); // Color
        (*_delegate_arr[4])(pos[g*3+2]);                        // Size
        (*_delegate_arr[5])();                                  // Render(OpenSansRegular)
        
    }
    
}

void MenuManager::UpdateDelegator(Delegator* delegator)
{
    /**
     * [ 0 ] FontManager::UpdateText(std::string);
     * [ 1 ] FontManager::UpdateAlign(std::string);
     * [ 2 ] FontManager::UpdatePosition(glm::vec2);
     * [ 3 ] FontManager::UpdateColor(glm::vec3);
     * [ 4 ] FontManager::UpdateSize(float);
     * [ 5 ] FontManager::RenderOpenSansRegular();
     * [ 6 ] PlayerManager::playermanager_UpdatePlay(int);
     * [ 7 ] ControlManager::controlmanager_UpdatePlay(int);
     * [ 8 ] CharacterManager::charactermanager_UpdatePlay(int);
     * [ 9 ] AudioManager::audiomanager_ResumeSound();
     * [10 ] AudioManager::audiomanager_PauseSound();
     * [11 ] ControlManager::audiomanager_UpdateInvertXAxis();
     * [12 ] ControlManager::audiomanager_UpdateInvertYAxis();
     */
    _delegate_arr.push_back(delegator);
}
