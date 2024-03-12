#include "HomogeneousCoordinate2D.hpp"
#include "AppParams.hpp"

#include <windows.h>
#include <vector>
#include <stack>


class Model
{
private:

    inline Matrices::Matrix<3U, 3U> get_window_viewport_transformation_matrix() const noexcept
    {
        return Matrices::Matrix<3U, 3U>
        { 
            {
                static_cast<double>(cur_viewport_dims.cx) / WINDOW_DIMS.cx, 0., 0.,
                0., static_cast<double>(cur_viewport_dims.cy) / WINDOW_DIMS.cy, 0.,
                static_cast<double>(cur_viewport_dims.cx - WINDOW_DIMS.cx), static_cast<double>(cur_viewport_dims.cy - WINDOW_DIMS.cy), 1.
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

    void add_object(TypeOfPrimitive type, CartesianCoordinate2D const& left_top, CartesianCoordinate2D const& right_bottom) noexcept;

    std::vector<std::pair<TypeOfPrimitive, CartesianCoordinate2D>> get_objects() const noexcept;

private:
    
    static constexpr RECT WINDOW{ AppParams::Canvas::REGION };
    static constexpr SIZE WINDOW_DIMS{ AppParams::Canvas::WIDTH, AppParams::Canvas::HEIGHT };
    
    RECT cur_viewport{ WINDOW };
    SIZE cur_viewport_dims{ WINDOW_DIMS };
    std::stack<RECT> viewports;

    std::vector<std::pair<TypeOfPrimitive, HomogeneousCoordinate2D<1.>>> objects;
};