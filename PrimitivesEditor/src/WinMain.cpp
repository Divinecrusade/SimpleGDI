#define UNICODE
#define _UNICODE

#define DEBUG
#define _DEBUG

#include <AppParams.hpp>

#include <cwchar>


void show_error(wchar_t const* const msg, wchar_t const* const caption)
{
    MessageBoxW(NULL, msg, caption, NULL);
}

__forceinline POINT get_cursor_pos_on_window(LPARAM lParam) noexcept
{
    return POINT{ static_cast<long>(GET_X_LPARAM(lParam)), static_cast<long>(GET_Y_LPARAM(lParam)) };
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static HDC hdc{ };

    static HPEN const CANVAS_BORDER_PEN{ CreatePen(AppParams::Canvas::BORDER_STYLE, AppParams::Canvas::BORDER_WIDTH, AppParams::Canvas::BORDER_COLOR) };
    static HPEN const RUBBER_LINE_PEN  { CreatePen(AppParams::RubberLine::STYLE, AppParams::RubberLine::WIDTH, AppParams::RubberLine::COLOR) };
    static HPEN const SOLID_LINE_PEN   { CreatePen(AppParams::Line::STYLE, AppParams::Line::WIDTH, AppParams::Line::COLOR) };
    
    static HBRUSH const CANVAS_BACKGROUND_BRUSH{ CreateSolidBrush(AppParams::Canvas::BACKGROUND_COLOR) };
    static HBRUSH const BTN_BACKGROUND_BRUSH   { CreateSolidBrush(AppParams::Button::BACKGROUND_COLOR) };

    static POINT line_beg{ };
    static POINT line_end{ };


    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps{ };
            hdc = BeginPaint(hWnd, &ps);

            SelectObject(hdc, CANVAS_BORDER_PEN);
            Rectangle(hdc, AppParams::Canvas::REGION.left, AppParams::Canvas::REGION.top, AppParams::Canvas::REGION.right, AppParams::Canvas::REGION.bottom);
            FillRect(hdc, &AppParams::Canvas::REGION, CANVAS_BACKGROUND_BRUSH);

            for (auto& rect : AppParams::Button::REGIONS)
            {
                FillRect(hdc, &rect, BTN_BACKGROUND_BRUSH);
            }

            EndPaint(hWnd, &ps);
        }
        break;

        case WM_LBUTTONDOWN:
        {
            line_beg = line_end = get_cursor_pos_on_window(lParam);
        }
        break;

        case WM_MOUSEMOVE:
        {
            if (wParam == VK_LBUTTON)
            {
                hdc = GetDC(hWnd);

                SelectObject(hdc, RUBBER_LINE_PEN);
                auto const OLD_ROP{ GetROP2(hdc) };
                SetROP2(hdc, AppParams::RubberLine::ROP);

                MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
                LineTo(hdc, line_end.x, line_end.y);

                line_end = get_cursor_pos_on_window(lParam);

                MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
                LineTo(hdc, line_end.x, line_end.y);

                SetROP2(hdc, OLD_ROP);
                ReleaseDC(hWnd, hdc);
            }
        }
        break;

        case WM_LBUTTONUP:
        {
            hdc = GetDC(hWnd);

            SelectObject(hdc, RUBBER_LINE_PEN);
            auto const OLD_ROP{ GetROP2(hdc) };
            SetROP2(hdc, AppParams::RubberLine::ROP);

            MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
            LineTo(hdc, line_end.x, line_end.y);

            SelectObject(hdc, SOLID_LINE_PEN);
            SetROP2(hdc, AppParams::Line::ROP);

            MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
            LineTo(hdc, line_end.x, line_end.y);

            SetROP2(hdc, OLD_ROP);
            ReleaseDC(hWnd, hdc);
        }
        break;

        case WM_DESTROY: PostQuitMessage(EXIT_SUCCESS); break;
        default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
    }

    return EXIT_SUCCESS;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr wchar_t const* WND_CLASS_NAME   { L"Simple Desktop App" };
    constexpr wchar_t const* CANVAS_CLASS_NAME{ L"Canvas" };
    constexpr wchar_t const* WINDOW_TITLE     { L"Primitives Editor" };

    constexpr int WINDOW_INIT_POS_X{ 100 };
    constexpr int WINDOW_INIT_POS_Y{ 100 };
    

    WNDCLASSEXW const window_class
    {
        sizeof(window_class),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        LoadIconW(hInstance, IDI_APPLICATION),
        LoadCursorW(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 2),
        NULL,
        WND_CLASS_NAME,
        LoadIconW(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassExW(&window_class))
    {
        show_error(L"Call to RegisterClassEx failed!", WINDOW_TITLE);

        return EXIT_FAILURE;
    }

    RECT window_pos{ };
    window_pos.left = WINDOW_INIT_POS_X;
    window_pos.right = AppParams::Window::WIDTH + window_pos.left;
    window_pos.top = WINDOW_INIT_POS_Y;
    window_pos.bottom = AppParams::Window::HEIGHT + window_pos.top;
    DWORD const style{ static_cast<DWORD>(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) };
    DWORD const ex_style{ static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW) };
    AdjustWindowRectEx(&window_pos, style, FALSE, ex_style);
    HWND const hWnd
    {
        CreateWindowExW
        (
            ex_style,
            WND_CLASS_NAME,
            WINDOW_TITLE,
            style,
            window_pos.left, window_pos.top,
            window_pos.right - window_pos.left, window_pos.bottom - window_pos.top,
            NULL,
            NULL,
            hInstance,
            NULL
        )
    };
    if (!hWnd)
    {
        show_error(L"Call to CreateWindowEx failed!", WINDOW_TITLE);
        
        return EXIT_FAILURE;
    }
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    MSG msg{ };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}