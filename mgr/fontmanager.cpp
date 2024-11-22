//
//  fontmanager.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/13.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "fontmanager.hpp"
#include "usge/freetypelib.hpp"





FontManager::FontManager()
{
    
    _text     = "";
    _align    = "left";
    _position = glm::vec2(0);
    _color    = glm::vec3(1);
    _size     = 2.4f;
    
    _opensans_regular = new FreeTypeLib("game/game/oj/_another_/font/OpenSans-Regular.ttf");
    _marlboro         = new FreeTypeLib("game/game/oj/_another_/font/marlboro.ttf");
    _rockoflf         = new FreeTypeLib("game/game/oj/_another_/font/RockoFLF.ttf");
    _later            = new FreeTypeLib("game/game/oj/_another_/font/later.ttf");
    _romanserif       = new FreeTypeLib("game/game/oj/_another_/font/RomanSerif.ttf");
    
}

FontManager::~FontManager()
{
    delete _opensans_regular;
    delete _marlboro;
    delete _rockoflf;
    delete _later;
    delete _romanserif;
}





std::string FontManager::UpdateText(std::string text){
    return _text = text;
}
std::string FontManager::UpdateAlign(std::string align){
    return _align = align;
}
glm::vec2 FontManager::UpdatePosition(glm::vec2 position){
    return _position = position;
}
glm::vec3 FontManager::UpdateColor(glm::vec3 color){
    return _color = color;
}
float FontManager::UpdateSize(float size){
    return _size = size;
}
void FontManager::RenderOpenSansRegular(){
    if (_align == "right") {
        _opensans_regular  -> RenderTextR(_text, _position.x, _position.y , _size, _color);
    }else if (_align == "center"){
        _opensans_regular  -> RenderTextC(_text, _position.x, _position.y , _size, _color);
    }else{
        _opensans_regular  -> RenderText(_text, _position.x, _position.y , _size, _color);
    }
}
void FontManager::RenderMarlboro(){
    if (_align == "right") {
        _marlboro  -> RenderTextR(_text, _position.x, _position.y , _size, _color);
    }else if (_align == "center"){
        _marlboro  -> RenderTextC(_text, _position.x, _position.y , _size, _color);
    }else{
        _marlboro  -> RenderText(_text, _position.x, _position.y , _size, _color);
    }
}
void FontManager::RenderRockoFLF(){
    if (_align == "right") {
        _rockoflf  -> RenderTextR(_text, _position.x, _position.y , _size, _color);
    }else if (_align == "center"){
        _rockoflf  -> RenderTextC(_text, _position.x, _position.y , _size, _color);
    }else{
        _rockoflf  -> RenderText(_text, _position.x, _position.y , _size, _color);
    }
}
void FontManager::RenderLater(){
    if (_align == "right") {
        _later  -> RenderTextR(_text, _position.x, _position.y , _size, _color);
    }else if (_align == "center"){
        _later  -> RenderTextC(_text, _position.x, _position.y , _size, _color);
    }else{
        _later  -> RenderText(_text, _position.x, _position.y , _size, _color);
    }
}
void FontManager::RenderRomanSerif(){
    if (_align == "right") {
        _romanserif  -> RenderTextR(_text, _position.x, _position.y , _size, _color);
    }else if (_align == "center"){
        _romanserif  -> RenderTextC(_text, _position.x, _position.y , _size, _color);
    }else{
        _romanserif  -> RenderText(_text, _position.x, _position.y , _size, _color);
    }
}

