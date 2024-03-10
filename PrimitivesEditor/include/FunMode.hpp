#ifndef FUNMODE
#define FUNMODE

#include "AppParams.hpp"


enum class FunMode : size_t
{
    LINE = AppParams::Button::ChooseLine::I, 
    RECTANGLE = AppParams::Button::ChooseRectangle::I, 
    ELLIPSE = AppParams::Button::ChooseEllipse::I,
    ZOOM = AppParams::Button::ChooseZoom::I,
    UNZOOM = AppParams::Button::ChooseUnzoom::I,
    NONE
};

#endif