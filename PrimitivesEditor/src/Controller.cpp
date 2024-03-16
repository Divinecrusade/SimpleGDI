#include "Controller.hpp"


void Controller::draw_model() noexcept
{
    auto objects{ logic_space.get_objects() };
    for (auto& obj : objects)
    {
        switch (obj.first)
        {
            case Model::TypeOfPrimitive::LINE:

                renderer.draw_solid_line(obj.second[0U].convert(), obj.second[1U].convert());

            break;

            case Model::TypeOfPrimitive::RECTANGLE:

                renderer.draw_filled_rect(obj.second[0U].convert(), obj.second[1U].convert());

            break;

            case Model::TypeOfPrimitive::ELLIPSE:

                renderer.draw_filled_ellipse(obj.second[0U].convert(), obj.second[1U].convert());

            break;

            default: break;
        }
    }
}

Controller& Controller::get_instance() noexcept
{
    static Controller instance_{};

    return instance_;
}

LRESULT CALLBACK Controller::WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static Controller& instance_{ Controller::get_instance() };
    static HDC hdc{ };

    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps{ };
            hdc = BeginPaint(hWnd, &ps);

            instance_.renderer.update_context(hdc);
            instance_.renderer.draw_stage();

            EndPaint(hWnd, &ps);
        }
        break;

        case WM_LBUTTONDOWN:
        {
            POINT const cursor_pos{ get_cursor_pos_on_window(lParam) };

            instance_.beg = instance_.end = cursor_pos;

            for (size_t i{ static_cast<size_t>(FunMode::LINE) }; i != static_cast<size_t>(FunMode::NONE); ++i)
            {
                if (is_contained(cursor_pos, AppParams::Button::REGIONS[i]))
                {
                    hdc = GetDC(hWnd);

                    instance_.renderer.update_context(hdc);

                    instance_.renderer.select_btn(i);
                    instance_.renderer.unselect_btn(static_cast<size_t>(instance_.choosen_mode));
                    instance_.choosen_mode = (i == static_cast<size_t>(instance_.choosen_mode) ? FunMode::NONE : static_cast<FunMode>(i));

                    ReleaseDC(hWnd, hdc);

                    break;
                }
            }
        }
        break;

        case WM_MOUSEMOVE:
        {
            if (instance_.choosen_mode == FunMode::NONE || instance_.choosen_mode == FunMode::UNZOOM) break;

            switch (wParam)
            {
                case VK_LBUTTON:
                {
                    hdc = GetDC(hWnd);

                    instance_.renderer.update_context(hdc);
                    instance_.renderer.set_clipping();

                    if (instance_.choosen_mode == FunMode::ZOOM) instance_.renderer.draw_rubber_rect(instance_.beg, instance_.end);
                    else instance_.renderer.draw_rubber_line(instance_.beg, instance_.end);

                    instance_.end = get_cursor_pos_on_window(lParam);

                    if (instance_.choosen_mode == FunMode::ZOOM) instance_.renderer.draw_rubber_rect(instance_.beg, instance_.end);
                    else instance_.renderer.draw_rubber_line(instance_.beg, instance_.end);

                    ReleaseDC(hWnd, hdc);
                }
                break;

                default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
            }
        }
        break;

        case WM_LBUTTONUP:
        {
            if (instance_.choosen_mode == FunMode::NONE) break;

            hdc = GetDC(hWnd);

            instance_.renderer.update_context(hdc);
            instance_.renderer.set_clipping();

            if (instance_.choosen_mode == FunMode::ZOOM) instance_.renderer.draw_rubber_rect(instance_.beg, instance_.end);
            else instance_.renderer.draw_rubber_line(instance_.beg, instance_.end);

            if (!is_contained(get_cursor_pos_on_window(lParam), AppParams::Canvas::REGION)) break;

            switch (instance_.choosen_mode)
            {
                case FunMode::LINE:
                {
                    instance_.renderer.draw_solid_line(instance_.beg, instance_.end);
                    instance_.logic_space.add_object(Model::TypeOfPrimitive::LINE, instance_.beg, instance_.end);
                }
                break;

                case FunMode::RECTANGLE:
                {
                    instance_.renderer.draw_filled_rect(instance_.beg, instance_.end);
                    instance_.logic_space.add_object(Model::TypeOfPrimitive::RECTANGLE, instance_.beg, instance_.end);
                }
                break;

                case FunMode::ELLIPSE:
                {
                    instance_.renderer.draw_filled_ellipse(instance_.beg, instance_.end);
                    instance_.logic_space.add_object(Model::TypeOfPrimitive::ELLIPSE, instance_.beg, instance_.end);
                }
                break;

                case FunMode::ZOOM:
                {
                    RECT new_viewport{ instance_.beg.x, instance_.beg.y, instance_.end.x, instance_.end.y };

                    if (new_viewport.left == new_viewport.right || new_viewport.top == new_viewport.bottom) break;

                    instance_.logic_space.zoom(new_viewport);
                    instance_.renderer.clear_canvas();
                    instance_.draw_model();
                }
                break;

                case FunMode::UNZOOM:
                {
                    instance_.logic_space.unzoom();
                    instance_.renderer.clear_canvas();
                    instance_.draw_model();
                }
                break;

                default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
            }

            ReleaseDC(hWnd, hdc);
        }
        break;

        case WM_DESTROY: PostQuitMessage(EXIT_SUCCESS); break;
        default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
    }

    return EXIT_SUCCESS;
}
