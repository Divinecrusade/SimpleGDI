#define UNICODE
#define _UNICODE

#define DEBUG
#define _DEBUG

#include <Windows.h>
#include <WinUser.h>
#include <cwchar>

static constexpr int CANVAS_WIDTH{ 600 };
static constexpr int CANVAS_HEIGHT{ CANVAS_WIDTH };
static constexpr int MENU_HEIGHT{ 100 };
static constexpr RECT PADDING{ 15, MENU_HEIGHT, 15, 15 };
static constexpr RECT CANVAS_REGION{ PADDING.left, PADDING.top, PADDING.left + CANVAS_WIDTH, PADDING.top + CANVAS_HEIGHT };

static constexpr int WINDOW_WIDTH{ CANVAS_WIDTH + PADDING.left + PADDING.right };
static constexpr int WINDOW_HEIGHT{ CANVAS_HEIGHT + PADDING.top + PADDING.bottom };

static constexpr auto CANVAS_BORDER_COLOR{ RGB(0, 0, 255) };
static constexpr auto CANVAS_BORDER_STYLE{ PS_SOLID };
static constexpr auto CANVAS_BORDER_WIDTH{ 4 };
static constexpr auto CANVAS_BACKGROUND_COLOR{ RGB(0, 0, 0) };

static constexpr auto LINE_COLOR{ RGB(255, 255, 255) };
static constexpr auto LINE_STYLE{ PS_SOLID };
static constexpr auto LINE_WIDTH{ 2 };


void show_error(wchar_t const* const msg, wchar_t const* const caption)
{
    MessageBoxW(NULL, msg, caption, NULL);
}

__forceinline POINT get_cursor_pos_on_window(LPARAM lParam) noexcept
{
    return POINT{ static_cast<long>(LOWORD(lParam)), static_cast<long>(HIWORD(lParam)) };
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static HDC hdc{ };

    static HPEN const CANVAS_BORDER_PEN{ CreatePen(CANVAS_BORDER_STYLE, CANVAS_BORDER_WIDTH, CANVAS_BORDER_COLOR) };
    static HPEN const LINE_PEN{ CreatePen(LINE_STYLE, LINE_WIDTH, LINE_COLOR) };
    
    static HBRUSH const CANVAS_BACKGROUND_BRUSH{ CreateSolidBrush(CANVAS_BACKGROUND_COLOR) };

    static bool LB_pressed{ false };

    static POINT line_beg{ };
    static POINT line_end{ };


    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps{ };
        hdc = BeginPaint(hWnd, &ps);

        SelectObject(hdc, CANVAS_BORDER_PEN);
        Rectangle(hdc, CANVAS_REGION.left, CANVAS_REGION.top, CANVAS_REGION.right, CANVAS_REGION.bottom);
        FillRect(hdc, &CANVAS_REGION, CANVAS_BACKGROUND_BRUSH);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        LB_pressed = true;
        line_beg = line_end = get_cursor_pos_on_window(lParam);
    }
    break;

    case WM_MOUSEMOVE:

        if (LB_pressed)
        {
            hdc = GetDC(hWnd);

            SelectObject(hdc, LINE_PEN);
            SetROP2(hdc, R2_XORPEN);

            MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
            LineTo(hdc, line_end.x, line_end.y);

            line_end = get_cursor_pos_on_window(lParam);

            MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
            LineTo(hdc, line_end.x, line_end.y);

            SetROP2(hdc, R2_COPYPEN);

            ReleaseDC(hWnd, hdc);
        }

    break;

    case WM_LBUTTONUP:
    {
        LB_pressed = false;

        hdc = GetDC(hWnd);

        SelectObject(hdc, LINE_PEN);
        SetROP2(hdc, R2_COPYPEN);

        MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
        LineTo(hdc, line_end.x, line_end.y);

        ReleaseDC(hWnd, hdc);
    }
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