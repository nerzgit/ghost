//
//  fontmanager.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef fontmanager_hpp
#define fontmanager_hpp

#include <string>

#include <glm/glm.hpp>

class FreeTypeLib;

class FontManager
{
private:
    FreeTypeLib* _opensans_regular;
    FreeTypeLib* _marlboro;
    FreeTypeLib* _rockoflf;
    FreeTypeLib* _later;
    FreeTypeLib* _romanserif;
    std::string _text;
    std::string _align;
    glm::vec2 _position;
    glm::vec3 _color;
    float _size;
    
public:
    FontManager();
    ~FontManager();
    std::string UpdateText(std::string text);
    std::string UpdateAlign(std::string align);
    glm::vec2 UpdatePosition(glm::vec2 position);
    glm::vec3 UpdateColor(glm::vec3 color);
    float UpdateSize(float size);
    void RenderOpenSansRegular();
    void RenderMarlboro();
    void RenderRockoFLF();
    void RenderLater();
    void RenderRomanSerif();
};

#endif /* fontmanager_hpp */
