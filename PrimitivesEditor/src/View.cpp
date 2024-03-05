#include "View.hpp"


View::~View() noexcept
{
    delete_objects(CANVAS_BORDER_PEN, RUBBER_LINE_PEN, SOLID_LINE_PEN, CANVAS_BACKGROUND_BRUSH, BTN_BACKGROUND_BRUSH);
}

void View::update_context(HDC const& context) noexcept
{
    cur_context = context;
}

void View::set_clipping() const noexcept
{
    SelectClipRgn
    (
        cur_context, 
        CreateRectRgn
        (
            AppParams::Canvas::REGION.left,
            AppParams::Canvas::REGION.top,
            AppParams::Canvas::REGION.right,
            AppParams::Canvas::REGION.bottom
        )
    );
}

void View::draw_stage() const noexcept
{
    SelectObject(cur_context, CANVAS_BORDER_PEN);
    Rectangle(cur_context, AppParams::Canvas::REGION.left, AppParams::Canvas::REGION.top, AppParams::Canvas::REGION.right, AppParams::Canvas::REGION.bottom);
    FillRect(cur_context, &AppParams::Canvas::REGION, CANVAS_BACKGROUND_BRUSH);

    for (auto& rect : AppParams::Button::REGIONS)
    {
        FillRect(cur_context, &rect, BTN_BACKGROUND_BRUSH);
    }
}

void View::draw_rubber_line(CartesianCoordinate2D const& beg, CartesianCoordinate2D const& end) const noexcept
{
    auto const OLD_OBJ{ SelectObject(cur_context, RUBBER_LINE_PEN) };
    auto const OLD_ROP{ GetROP2(cur_context) };
    SetROP2(cur_context, AppParams::RubberLine::ROP);

    draw_line(beg.x, beg.y, end.x, end.y);

    SetROP2(cur_context, OLD_ROP);
    SelectObject(cur_context, OLD_OBJ);
}

void View::draw_solid_line(CartesianCoordinate2D const& beg, CartesianCoordinate2D const& end) const noexcept
{
    auto const OLD_OBJ{ SelectObject(cur_context, SOLID_LINE_PEN) };
    auto const OLD_ROP{ GetROP2(cur_context) };
    SetROP2(cur_context, AppParams::Line::ROP);

    draw_line(beg.x, beg.y, end.x, end.y);

    SetROP2(cur_context, OLD_ROP);
    SelectObject(cur_context, OLD_OBJ);
}

void View::draw_line(int x1, int y1, int x2, int y2) const noexcept
{
    MoveToEx(cur_context, x1, y1, nullptr);
    LineTo(cur_context, x2, y2);
}

