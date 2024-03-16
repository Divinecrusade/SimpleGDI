#define UNICODE
#define _UNICODE

#define DEBUG
#define _DEBUG

#include "Controller.hpp"

#include <cwchar>


void show_error(wchar_t const* const msg, wchar_t const* const caption)
{
    MessageBoxW(NULL, msg, caption, NULL);
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
        Controller::WndProc,
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