#include <CartesianCoordinate2D.hpp>
#include <Windows.h>


struct View
{
    static constexpr int BASE_X{ 0 };
    static constexpr int BASE_Y{ 0 };

    enum class StrokeMode : int
    {
        XOR = R2_XORPEN, NORMAL = R2_COPYPEN
    };

    void draw_line(CartesianCoordinate2D const& beg, CartesianCoordinate2D const& end, HDC context, COLORREF c, int stroke_width, StrokeMode mode = StrokeMode::NORMAL)
    {
        POINT const DEV_BEG{ BASE_X + beg.x, BASE_Y + beg.y };
        POINT const DEV_END{ BASE_X + end.x, BASE_Y + end.y };
        HPEN const PEN{ CreatePen(PS_SOLID, stroke_width, c) };

        int const OLD_ROP{ GetROP2(hdc) };
        int const NEW_ROP{ static_cast<int>(mode) };
        SetROP2(hdc, NEW_ROP);

        MoveToEx(hdc, DEV_BEG.x, DEV_BEG.y, nullptr);
        LineTo  (hdc, DEV_END.x, DEV_END.y);

        SetROP2(hdc, OLD_ROP);
    }
};