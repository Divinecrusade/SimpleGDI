#include "View.hpp"
#include <wchar.h>


View::~View() noexcept
{
    delete_objects(CANVAS_BORDER_PEN, RUBBER_LINE_PEN, SOLID_LINE_PEN, CANVAS_BACKGROUND_BRUSH, BTN_BACKGROUND_BRUSH, BTN_BORDER_BRUSH);
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

    for (size_t i{ 0U }; i != AppParams::Button::N; ++i)
    {
        FillRect(cur_context, &AppParams::Button::REGIONS[i], BTN_BACKGROUND_BRUSH);

        SIZE string_size{ };
        auto const string_length{ wcslen(AppParams::Button::TEXTS[i]) };
        GetTextExtentPoint32W(cur_context, AppParams::Button::TEXTS[i], string_length, &string_size);
        TextOutW
        (
            cur_context, 
            AppParams::Button::REGIONS[i].left + AppParams::Button::WIDTH / 2 - string_size.cx / 2,
            AppParams::Button::REGIONS[i].top + AppParams::Button::HEIGHT / 2 - string_size.cy / 2,
            AppParams::Button::TEXTS[i],
            string_length
        );
    }
}

void View::draw_rubber_line(POINT const& beg, POINT const& end) const noexcept
{
    auto const OLD_OBJ{ SelectObject(cur_context, RUBBER_LINE_PEN) };
    auto const OLD_ROP{ GetROP2(cur_context) };
    SetROP2(cur_context, AppParams::RubberLine::ROP);

    draw_line(beg.x, beg.y, end.x, end.y);

    SetROP2(cur_context, OLD_ROP);
    SelectObject(cur_context, OLD_OBJ);
}

void View::draw_solid_line(POINT const& beg, POINT const& end) const noexcept
{
    auto const OLD_OBJ{ SelectObject(cur_context, SOLID_LINE_PEN) };
    auto const OLD_ROP{ GetROP2(cur_context) };
    SetROP2(cur_context, AppParams::Line::ROP);

    draw_line(beg.x, beg.y, end.x, end.y);

    SetROP2(cur_context, OLD_ROP);
    SelectObject(cur_context, OLD_OBJ);
}

void View::draw_filled_rect(POINT const& leftTop, POINT const& rightBot) const noexcept
{
    auto const OLD_PEN{ SelectPen(cur_context, STROKE_RECTANGLE) };
    auto const OLD_BRUSH{ SelectBrush(cur_context, FILL_RECTANGLE) };
    auto const OLD_ROP{ GetROP2(cur_context) };
    SetROP2(cur_context, AppParams::Rectangle::Stroke::ROP);
    
    fill_rect(leftTop.x, leftTop.y, rightBot.x, rightBot.y);

    SetROP2(cur_context, OLD_ROP);
    SelectPen(cur_context, OLD_PEN);
    SelectBrush(cur_context, OLD_BRUSH);
}

void View::draw_filled_ellipse(POINT const& leftTop, POINT const& rightBot) const noexcept
{
    auto const OLD_PEN{ SelectPen(cur_context, STROKE_ELLIPSE) };
    auto const OLD_BRUSH{ SelectBrush(cur_context, FILL_ELLIPSE) };
    auto const OLD_ROP{ GetROP2(cur_context) };
    SetROP2(cur_context, AppParams::Ellipse::Stroke::ROP);

    fill_ellipse(leftTop.x, leftTop.y, rightBot.x, rightBot.y);

    SetROP2(cur_context, OLD_ROP);
    SelectPen(cur_context, OLD_PEN);
    SelectBrush(cur_context, OLD_BRUSH);
}

void View::select_btn(size_t i) const noexcept
{
    if (i >= AppParams::Button::N) return;

    FrameRect(cur_context, &AppParams::Button::REGIONS[i], BTN_BORDER_BRUSH);
}

void View::unselect_btn(size_t i) const noexcept
{
    if (i >= AppParams::Button::N) return;

    FrameRect(cur_context, &AppParams::Button::REGIONS[i], BTN_BACKGROUND_BRUSH);
}

void View::clear_canvas() const noexcept
{
    auto const OLD_BRUSH{ SelectBrush(cur_context, CANVAS_BACKGROUND_BRUSH) };
 
    fill_rect(AppParams::Canvas::REGION.left, AppParams::Canvas::REGION.top, AppParams::Canvas::REGION.right, AppParams::Canvas::REGION.bottom);

    SelectBrush(cur_context, OLD_BRUSH);
}

void View::draw_line(int x1, int y1, int x2, int y2) const noexcept
{
    MoveToEx(cur_context, x1, y1, nullptr);
    LineTo(cur_context, x2, y2);
}

void View::fill_rect(int left, int top, int right, int bottom) const noexcept
{
    Rectangle(cur_context, left, top, right, bottom);
}

void View::fill_ellipse(int left, int top, int right, int bottom) const noexcept
{
    Ellipse(cur_context, left, top, right, bottom);
}

