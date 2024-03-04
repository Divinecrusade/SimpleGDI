#define UNICODE
#define _UNICODE

#define DEBUG
#define _DEBUG

#include <Windows.h>
#include <WinUser.h>
#include <cwchar>


void show_error(wchar_t const* const msg, wchar_t const* const caption)
{
    MessageBoxW(NULL, msg, caption, NULL);
}

SIZE get_window_size(HWND hWnd)
{
    RECT wnd_area{ };
    GetClientRect(hWnd, &wnd_area);

    return SIZE{ wnd_area.right - wnd_area.left, wnd_area.bottom - wnd_area.top };
}

__forceinline static POINT get_cursor_pos_on_screen() noexcept
{
    POINT global_cursor_pos{ };

    GetCursorPos(&global_cursor_pos);

    return global_cursor_pos;
}

__forceinline POINT get_cursor_pos_on_window(HWND hWnd) noexcept
{
    POINT cursor_pos{ get_cursor_pos_on_screen() };

    ScreenToClient(hWnd, &cursor_pos);

    return cursor_pos;
}

__forceinline POINT get_cursor_pos_on_window(LPARAM lParam) noexcept
{
    return POINT{ static_cast<long>(lParam % 0x10000LL), static_cast<long>(lParam / 0x10000LL) };
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static PAINTSTRUCT ps{ };
    static HDC hdc{ };

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);

        break;

    case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
        break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
        break;
    }

    return EXIT_SUCCESS;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr wchar_t const* WND_CLASS_NAME{ L"Simple Desktop App" };
    constexpr wchar_t const* WINDOW_TITLE  { L"Primitives Editor" };

    constexpr int WINDOW_INIT_POS_X{ 300 };
    constexpr int WINDOW_INIT_POS_Y{ 100 };
    static constexpr int WINDOW_WIDTH{ 600 };
    static constexpr int WINDOW_HEIGHT{ WINDOW_WIDTH };


    WNDCLASSEXW const wcex
    {
        sizeof(wcex),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        LoadIconW(hInstance, IDI_APPLICATION),
        LoadCursorW(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        WND_CLASS_NAME,
        LoadIconW(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassExW(&wcex))
    {
        show_error(L"Call to RegisterClassEx failed!", WINDOW_TITLE);

        return EXIT_FAILURE;
    }

    RECT window_pos{ };
    window_pos.left = WINDOW_INIT_POS_X;
    window_pos.right = WINDOW_WIDTH + window_pos.left;
    window_pos.top = WINDOW_INIT_POS_Y;
    window_pos.bottom = WINDOW_HEIGHT + window_pos.top;
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