#ifndef TYPEOFPRIMITIVE
#define TYPEOFPRIMITIVE

#include "AppParams.hpp"


enum class TypeOfPrimitive : size_t
{
    LINE = AppParams::Button::ChooseLine::I, 
    RECTANGLE = AppParams::Button::ChooseRectangle::I, 
    ELLIPSE = AppParams::Button::ChooseEllipse::I
};

#endif