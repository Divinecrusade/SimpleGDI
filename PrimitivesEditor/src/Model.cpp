#include "Model.hpp"
#include <iterator>


void Model::set_viewport(RECT const& new_viewport) noexcept
{
    viewports.push(cur_viewport);
    cur_viewport = new_viewport;
}

void Model::unset_viewport() noexcept
{
    if (viewports.empty()) return;

    cur_viewport = viewports.top();
    viewports.pop();
}

void Model::add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept
{
}

std::vector<std::pair<Model::TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> Model::get_objects() const noexcept
{
    return {};
}
