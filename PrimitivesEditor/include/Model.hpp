#ifndef MODEL
#define MODEL

#include "HomogeneousCoordinate2D.hpp"
#include "AppParams.hpp"

#include <windows.h>
#include <vector>
#include <stack>


class Model
{
private:

    inline Matrices::Matrix<3U, 3U> get_zoom_matrix() const noexcept
    {
        return Matrices::Matrix<3U, 3U>
        { 
            {
                static_cast<double>(WINDOW.right - WINDOW.left) / (cur_viewport.right - cur_viewport.left), 0., 0.,
                0., static_cast<double>(WINDOW.bottom - WINDOW.top) / (cur_viewport.bottom - cur_viewport.top), 0.,
                static_cast<double>(WINDOW.left - cur_viewport.left), static_cast<double>(WINDOW.top - cur_viewport.top), 1.

                //static_cast<double>(max(cur_viewport.left, cur_viewport.right) - min(cur_viewport.left, cur_viewport.right)) / (max(WINDOW.left, WINDOW.right) - min(WINDOW.left, WINDOW.right)), 0., 0.,
                //0., static_cast<double>(max(cur_viewport.top, cur_viewport.bottom) - min(cur_viewport.top, cur_viewport.bottom)) / (max(WINDOW.top, WINDOW.bottom) - min(WINDOW.top, WINDOW.bottom)), 0.,
                //static_cast<double>(min(cur_viewport.left, cur_viewport.right) - min(WINDOW.left, WINDOW.right)), static_cast<double>(min(cur_viewport.top, cur_viewport.bottom) - min(WINDOW.top, WINDOW.bottom)), 1.
            }
        };
    }

public:

    enum class TypeOfPrimitive
    {
        LINE, RECTANGLE, ELLIPSE
    };

public:

    Model() = default;
    Model(Model const&) = delete;
    Model(Model&&) = delete;

    Model& operator=(Model const&) = delete;
    Model& operator=(Model&&) = delete;

    ~Model() = default;

    void set_viewport(RECT const& new_viewport) noexcept;
    void unset_viewport() noexcept;

    void add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept;

    std::vector<std::pair<TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> get_objects() const noexcept;

private:

    

private:
    
    static constexpr RECT WINDOW{ AppParams::Canvas::REGION };
    
    RECT cur_viewport{ WINDOW };
    std::stack<RECT> viewports;

    std::vector<std::pair<TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::WC>, 2U>>> wc;
};


#endif // !MODEL