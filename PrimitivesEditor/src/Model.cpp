#include "Model.hpp"
#include <iterator>


void Model::set_viewport(RECT const& new_viewport) noexcept
{
    viewports.push(cur_viewport);
    cur_viewport = new_viewport;
    cur_viewport_dims = { cur_viewport.right - cur_viewport.left, cur_viewport.bottom - cur_viewport.top };
}

void Model::unset_viewport() noexcept
{
    if (viewports.empty()) return;

    cur_viewport = viewports.top();
    cur_viewport_dims = { cur_viewport.right - cur_viewport.left, cur_viewport.bottom - cur_viewport.top };
    viewports.pop();
}

void Model::add_object(TypeOfPrimitive type, CartesianCoordinate2D const& left_top, CartesianCoordinate2D const& right_bottom) noexcept
{
    HomogeneousCoordinate2D<1.> h_left_top{ left_top };
    h_left_top.transform(!get_window_viewport_transformation_matrix());
    h_left_top.X /= WINDOW_DIMS.cx;
    h_left_top.Y /= WINDOW_DIMS.cy;

    objects.emplace_back(type, h_left_top);
}

std::vector<std::pair<Model::TypeOfPrimitive, CartesianCoordinate2D>> Model::get_objects() const noexcept
{
    std::vector<std::pair<TypeOfPrimitive, CartesianCoordinate2D>> converted_objects{ };
    
    std::transform
    (
        objects.begin(),
        objects.end(),
        std::back_inserter(converted_objects),
        [this](auto const& obj)
        {
            return std::make_pair(obj.first, obj.second.get_transformed(get_window_viewport_transformation_matrix()).convert_to_cartesian());
        }
    );

    return converted_objects;
}
