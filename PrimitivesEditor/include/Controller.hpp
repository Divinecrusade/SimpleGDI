#ifndef CONTROLLER
#define CONTROLLER

#include "View.hpp"
#include "Model.hpp"
#include "FunMode.hpp"


class Controller
{
private:

    __forceinline static POINT get_cursor_pos_on_window(LPARAM lParam) noexcept
    {
        return POINT{ static_cast<long>(GET_X_LPARAM(lParam)), static_cast<long>(GET_Y_LPARAM(lParam)) };
    }

    __forceinline static bool is_contained(POINT const& pos, RECT const& frame) noexcept
    {
        return pos.x >= frame.left && pos.x <= frame.right && pos.y >= frame.top && pos.y <= frame.bottom;
    }

    __forceinline static POINT form_square(POINT const& beg, POINT const& end)
    {
        auto const width{ max(beg.x, end.x) - min(beg.x, end.x) };
        auto const height{ max(beg.y, end.y) - min(beg.y, end.y) };
        auto const sqr_length{ min(width, height) };

        return POINT{ (beg.x < end.x ? beg.x + sqr_length : beg.x - sqr_length), (beg.y < end.y ? beg.y + sqr_length : beg.y - sqr_length) };
    }

    Controller() noexcept = default;

    void draw_model() noexcept;

public:

    Controller(Controller const&) = delete;
    Controller(Controller&&) = delete;

    Controller& operator=(Controller const&) = delete;
    Controller& operator=(Controller&&) = delete;

    static Controller& get_instance() noexcept;

    ~Controller() = default;

    static LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

private:

    HomogeneousCoordinate2D<CoordinateSystem::DC> const COLLISION_PADDING_MOUSE{ 10., 10. };

    POINT beg{ };
    POINT end{ };
    FunMode choosen_mode{ FunMode::NONE };

    View renderer{ };
    Model logic_space{ };
};


#endif