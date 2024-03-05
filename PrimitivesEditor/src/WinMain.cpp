#define UNICODE
#define _UNICODE

#define DEBUG
#define _DEBUG

#include "View.hpp"

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

    static View renderer{ };
    static POINT line_beg{ };
    static POINT line_end{ };


    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps{ };
            hdc = BeginPaint(hWnd, &ps);

            renderer.update_context(hdc);
            renderer.draw_stage();

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
            switch(wParam) 
            {
                case VK_LBUTTON:
                {
                    hdc = GetDC(hWnd);

                    renderer.update_context(hdc);
                    renderer.set_clipping();

                    renderer.draw_rubber_line({ line_beg.x, line_beg.y }, { line_end.x, line_end.y });

                    line_end = get_cursor_pos_on_window(lParam);

                    renderer.draw_rubber_line({ line_beg.x, line_beg.y }, { line_end.x, line_end.y });

                    ReleaseDC(hWnd, hdc);
                }
                default: return DefWindowProcW(hWnd, message, wParam, lParam); break;
            }
        }
        break;

        case WM_LBUTTONUP:
        {
            hdc = GetDC(hWnd);

            renderer.update_context(hdc);
            renderer.set_clipping();

            renderer.draw_rubber_line({ line_beg.x, line_beg.y }, { line_end.x, line_end.y });
            renderer.draw_solid_line({ line_beg.x, line_beg.y }, { line_end.x, line_end.y });

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