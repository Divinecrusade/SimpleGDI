#include "CartesianCoordinate2D.hpp"
#include "AppParams.hpp"


class View
{
public:
    
    static constexpr int BASE_X{ 0 };
    static constexpr int BASE_Y{ 0 };


    View() noexcept = default;
    View(View const&) = delete;
    View(View&&) = delete;

    View& operator=(View const&) = delete;
    View& operator=(View&&) = delete;

    ~View() noexcept;

    void update_context(HDC const& context) noexcept;
    void set_clipping() const noexcept;

    void draw_stage() const noexcept;
    void draw_rubber_line(CartesianCoordinate2D const& beg, CartesianCoordinate2D const& end) const noexcept;
    void draw_solid_line (CartesianCoordinate2D const& beg, CartesianCoordinate2D const& end) const noexcept;

private:

    void draw_line(int x1, int y1, int x2, int y2) const noexcept;

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

    HBRUSH const CANVAS_BACKGROUND_BRUSH{ CreateSolidBrush(AppParams::Canvas::BACKGROUND_COLOR) };
    HBRUSH const BTN_BACKGROUND_BRUSH   { CreateSolidBrush(AppParams::Button::BACKGROUND_COLOR) };
};