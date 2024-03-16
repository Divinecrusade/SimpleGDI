#ifndef VIEW
#define VIEW

#include "AppParams.hpp"


class View
{
public:
    
    View() noexcept = default;
    View(View const&) = delete;
    View(View&&) = delete;

    View& operator=(View const&) = delete;
    View& operator=(View&&) = delete;

    ~View() noexcept;

    void update_context(HDC const& context) noexcept;
    void set_clipping() const noexcept;

    void draw_stage() const noexcept;
    void draw_rubber_line   (POINT const& beg,     POINT const& end) const noexcept;
    void draw_rubber_rect   (POINT const& beg,     POINT const& end) const noexcept;
    void draw_solid_line    (POINT const& beg,     POINT const& end) const noexcept;
    void draw_filled_rect   (POINT const& leftTop, POINT const& rightBot) const noexcept;
    void draw_filled_ellipse(POINT const& leftTop, POINT const& rightBot) const noexcept;

    void select_btn  (size_t i) const noexcept;
    void unselect_btn(size_t i) const noexcept;

    void clear_canvas() const noexcept;

private:

    void draw_line(int x1, int y1, int x2, int y2) const noexcept;
    void fill_rect(int left, int top, int right, int bottom) const noexcept;
    void fill_ellipse(int left, int top, int right, int bottom) const noexcept;

    template<class ...T>
    void delete_objects(T&... handlers) const noexcept
    {
        (void) std::initializer_list<int>{ (DeleteObject(handlers), 0)... };
    }

private:

    HDC cur_context{ NULL };

    HPEN const CANVAS_BORDER_PEN{ CreatePen(AppParams::Canvas::BORDER_STYLE, AppParams::Canvas::BORDER_WIDTH, AppParams::Canvas::BORDER_COLOR) };
    HPEN const RUBBER_LINE_PEN  { CreatePen(AppParams::RubberLine::STYLE, AppParams::RubberLine::WIDTH, AppParams::RubberLine::COLOR) };
    HPEN const SOLID_LINE_PEN   { CreatePen(AppParams::Line::STYLE, AppParams::Line::WIDTH, AppParams::Line::COLOR) };
    HPEN const STROKE_RECTANGLE { CreatePen(AppParams::Rectangle::Stroke::STYLE, AppParams::Rectangle::Stroke::WIDTH, AppParams::Rectangle::Stroke::COLOR) };
    HPEN const STROKE_ELLIPSE   { CreatePen(AppParams::Ellipse::Stroke::STYLE, AppParams::Ellipse::Stroke::WIDTH, AppParams::Ellipse::Stroke::COLOR) };

    HBRUSH const CANVAS_BACKGROUND_BRUSH{ CreateSolidBrush(AppParams::Canvas::BACKGROUND_COLOR) };
    HBRUSH const BTN_BACKGROUND_BRUSH   { CreateSolidBrush(AppParams::Button::BACKGROUND_COLOR) };
    HBRUSH const BTN_BORDER_BRUSH       { CreateSolidBrush(AppParams::Button::BORDER_COLOR) };
    HBRUSH const FILL_RECTANGLE         { CreateSolidBrush(AppParams::Rectangle::COLOR) };
    HBRUSH const FILL_ELLIPSE           { CreateSolidBrush(AppParams::Ellipse::COLOR) };
};


#endif // !VIEW