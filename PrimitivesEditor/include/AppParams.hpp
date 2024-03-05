#ifndef APPPARAMS
#define APPPARAMS

#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>

#include <array>


namespace AppParams
{
    namespace RubberLine
    {
        static constexpr auto STYLE{ PS_SOLID };
        static constexpr int  WIDTH{ 1 };
        static constexpr auto COLOR{ RGB(255, 255, 255) };
        static constexpr auto ROP  { R2_XORPEN };
    }

    namespace Line
    {
        static constexpr auto STYLE{ PS_SOLID };
        static constexpr int  WIDTH{ 2 };
        static constexpr auto COLOR{ RGB(255, 0, 0) };
        static constexpr auto ROP  { R2_COPYPEN };
    }

    namespace Canvas
    {
        static constexpr int  WIDTH { 600 };
        static constexpr int  HEIGHT{ Canvas::WIDTH };
    }

    namespace Menu
    {
        static constexpr int  WIDTH { Canvas::WIDTH };
        static constexpr int  HEIGHT{ 100 };
    }

    namespace Window
    {
        static constexpr RECT PADDING{ 15, Menu::HEIGHT, 15, 15 };
        static constexpr int  WIDTH  { Canvas::WIDTH + PADDING.left + PADDING.right };
        static constexpr int  HEIGHT { Canvas::HEIGHT + PADDING.top + PADDING.bottom };
    }

    namespace Canvas
    {
        static constexpr RECT REGION{ Window::PADDING.left, Window::PADDING.top, Window::PADDING.left + WIDTH, Window::PADDING.top + HEIGHT };
    
        static constexpr auto BORDER_COLOR{ RGB(0, 0, 255) };
        static constexpr auto BORDER_STYLE{ PS_SOLID };
        static constexpr int  BORDER_WIDTH{ 4 };
        static constexpr auto BACKGROUND_COLOR{ RGB(0, 0, 0) };
    }

    namespace Menu
    {
        static constexpr RECT REGION{ Window::PADDING.left, 0, Window::PADDING.left + WIDTH, HEIGHT };
    }

    namespace Button
    {
        static constexpr auto BACKGROUND_COLOR{ RGB(255, 255, 255) };
        static constexpr int  WIDTH { 100 };
        static constexpr int  HEIGHT{ 80 };
        static constexpr int  N{ 4 };
        static constexpr int  Y_MARGIN{ (Menu::HEIGHT - HEIGHT) / 2 };
        static constexpr int  X_MARGIN{ (Menu::WIDTH  - WIDTH * N) / (N - 1) };

        static consteval std::array<RECT, N> get_btn_regions()
        {
            std::array<RECT, N> regions{ };

            regions[0] =
            { 
                Menu::REGION.left, 
                Menu::REGION.top + Y_MARGIN, 
                Menu::REGION.left + WIDTH, 
                Menu::REGION.top + Y_MARGIN + HEIGHT
            };
            for (int i{ 0 }; i != N; ++i)
            {
                regions[i] = 
                { 
                    Menu::REGION.left + i * X_MARGIN + i * WIDTH,
                    Menu::REGION.top + Y_MARGIN,
                    Menu::REGION.left + i * X_MARGIN + i * WIDTH + WIDTH,
                    Menu::REGION.top + Y_MARGIN + HEIGHT
                };
            }

            return regions;
        }

        static constexpr std::array<RECT, N> REGIONS{ get_btn_regions() };
    }
};

#endif