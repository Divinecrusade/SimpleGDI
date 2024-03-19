#include "Controller.hpp"


void Controller::draw_model() noexcept
{
    for (auto const& obj : logic_space.get_objects())
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
    static Controller instance{};

    return instance;
}

LRESULT CALLBACK Controller::WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static Controller& instance{ Controller::get_instance() };
    static HDC hdc{ };

    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps{ };
            hdc = BeginPaint(hWnd, &ps);

            instance.renderer.update_context(hdc);
            instance.renderer.draw_stage();

            EndPaint(hWnd, &ps);
        }
        break;

        case WM_LBUTTONDOWN:
        {
            POINT const cursor_pos{ get_cursor_pos_on_window(lParam) };

            instance.beg = instance.end = cursor_pos;

            for (size_t i{ static_cast<size_t>(FunMode::LINE) }; i != static_cast<size_t>(FunMode::NONE); ++i)
            {
                if (is_contained(cursor_pos, AppParams::Button::REGIONS[i]))
                {
                    hdc = GetDC(hWnd);

                    instance.renderer.update_context(hdc);

                    instance.renderer.select_btn(i);
                    instance.renderer.unselect_btn(static_cast<size_t>(instance.choosen_mode));
                    instance.choosen_mode = (i == static_cast<size_t>(instance.choosen_mode) ? FunMode::NONE : static_cast<FunMode>(i));

                    ReleaseDC(hWnd, hdc);

                    break;
                }
            }
        }
        break;

        case WM_MOUSEMOVE:
        {
            if (instance.choosen_mode == FunMode::NONE || instance.choosen_mode == FunMode::UNZOOM) break;

            switch (wParam)
            {
                case VK_LBUTTON:
                {
                    hdc = GetDC(hWnd);

                    instance.renderer.update_context(hdc);
                    instance.renderer.set_clipping();

                    auto const old_end{ std::exchange(instance.end, get_cursor_pos_on_window(lParam)) };

                    if (instance.choosen_mode == FunMode::ZOOM) 
                    {
                        instance.renderer.draw_rubber_rect(instance.beg, old_end);
                        instance.renderer.draw_rubber_rect(instance.beg, instance.end);
                    }
                    else 
                    {
                        instance.renderer.draw_rubber_line(instance.beg, old_end);
                        instance.renderer.draw_rubber_line(instance.beg, instance.end);
                    }

                    ReleaseDC(hWnd, hdc);
                }
                break;

                default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
            }
        }
        break;

        case WM_LBUTTONUP:
        {
            if (instance.choosen_mode == FunMode::NONE || 
                !is_contained(get_cursor_pos_on_window(lParam), AppParams::Canvas::REGION)) break;

            hdc = GetDC(hWnd);

            instance.renderer.update_context(hdc);
            instance.renderer.set_clipping();

            if (instance.choosen_mode == FunMode::ZOOM || instance.choosen_mode == FunMode::TRANSLATE) instance.renderer.draw_rubber_rect(instance.beg, instance.end);
            else instance.renderer.draw_rubber_line(instance.beg, instance.end);

            switch (instance.choosen_mode)
            {
                case FunMode::LINE:
                {
                    instance.renderer.draw_solid_line(instance.beg, instance.end);
                    instance.logic_space.add_object(Model::TypeOfPrimitive::LINE, instance.beg, instance.end);
                }
                break;

                case FunMode::RECTANGLE:
                {
                    instance.renderer.draw_filled_rect(instance.beg, instance.end);
                    instance.logic_space.add_object(Model::TypeOfPrimitive::RECTANGLE, instance.beg, instance.end);
                }
                break;

                case FunMode::ELLIPSE:
                {
                    instance.renderer.draw_filled_ellipse(instance.beg, instance.end);
                    instance.logic_space.add_object(Model::TypeOfPrimitive::ELLIPSE, instance.beg, instance.end);
                }
                break;

                case FunMode::ZOOM:
                {
                    RECT new_viewport{ instance.beg.x, instance.beg.y, instance.end.x, instance.end.y };

                    if (new_viewport.left == new_viewport.right || new_viewport.top == new_viewport.bottom) break;

                    instance.logic_space.zoom(new_viewport);
                    instance.renderer.clear_canvas();
                    instance.draw_model();
                }
                break;

                case FunMode::UNZOOM:
                {
                    instance.logic_space.unzoom();
                    instance.renderer.clear_canvas();
                    instance.draw_model();
                }
                break;

                case FunMode::TRANSLATE:
                {
                    instance.logic_space.translate(instance.beg, instance.end);
                    instance.renderer.clear_canvas();
                    instance.draw_model();
                }

                default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
            }

            ReleaseDC(hWnd, hdc);
        }
        break;

        case WM_RBUTTONUP:
        {
            POINT const cursor_pos{ get_cursor_pos_on_window(lParam) };

            if (wParam == VK_LBUTTON || !is_contained(cursor_pos, AppParams::Canvas::REGION)) break;

            HomogeneousCoordinate2D<CoordinateSystem::DC> const left_top{ static_cast<double>(cursor_pos.x) - instance.COLLISION_PADDING_MOUSE.get_X(), static_cast<double>(cursor_pos.y) - instance.COLLISION_PADDING_MOUSE.get_Y() };
            HomogeneousCoordinate2D<CoordinateSystem::DC> const right_bottom{ static_cast<double>(cursor_pos.x) + instance.COLLISION_PADDING_MOUSE.get_X(), static_cast<double>(cursor_pos.y) + instance.COLLISION_PADDING_MOUSE.get_Y() };
            instance.logic_space.delete_object(left_top, right_bottom, HomogeneousCoordinate2D<CoordinateSystem::DC>{cursor_pos});
            
            hdc = GetDC(hWnd);
            instance.renderer.update_context(hdc);
            instance.renderer.set_clipping();
            instance.renderer.clear_canvas();
            instance.draw_model();
        }
        break;

        case WM_DESTROY: PostQuitMessage(EXIT_SUCCESS); break;
        default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
    }

    return EXIT_SUCCESS;
}
