#include "Model.hpp"
#include <iterator>


void Model::zoom(RECT const& window) noexcept
{
    double const Sx{ static_cast<double>(max(VIEWPORT.left, VIEWPORT.right) - min(VIEWPORT.left, VIEWPORT.right)) / (max(window.left, window.right) - min(window.left, window.right)) };
    double const Sy{ static_cast<double>(max(VIEWPORT.bottom, VIEWPORT.top) - min(VIEWPORT.bottom, VIEWPORT.top)) / (max(window.bottom, window.top) - min(window.bottom, window.top)) };
    double const Tx{ min(VIEWPORT.left, VIEWPORT.right) - Sx * min(window.left, window.right) };
    double const Ty{ min(VIEWPORT.bottom, VIEWPORT.top) - Sy * min(window.bottom, window.top) };

    Matrices::Matrix<3U, 3U> transformation
    {
        {
            Sx, 0., 0.,
            0., Sy, 0.,
            Tx, Ty, 1.
        }
    };
    cur_state *= transformation;
    transformations_chain.push(transformation);
}

void Model::unzoom() noexcept
{
    if (transformations_chain.empty()) return;

    cur_state *= !transformations_chain.top();
    transformations_chain.pop();
}

void Model::add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept
{
    objects.emplace_back(type, std::array<HomogeneousCoordinate2D<CoordinateSystem::WC>, 2U>{ world(beg), world(end) });
}

std::vector<std::pair<Model::TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> Model::get_objects() const noexcept
{
    std::vector<std::pair<Model::TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> render_objects{ };
    render_objects.reserve(objects.size());

    std::transform
    (
        objects.cbegin(), 
        objects.cend(), 
        std::back_inserter(render_objects),
        [this](auto const& obj) 
        { 
            return make_pair(obj.first, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>{ device(obj.second[0U]), device(obj.second[1U]) });
        } 
    );

    return render_objects;
}

HomogeneousCoordinate2D<CoordinateSystem::WC> Model::world(HomogeneousCoordinate2D<CoordinateSystem::DC> const& coordinate) const noexcept
{
    auto const tmp{ coordinate.get_transformed(!cur_state) };

    return { tmp.get_X(), tmp.get_Y() };
}

HomogeneousCoordinate2D<CoordinateSystem::DC> Model::device(HomogeneousCoordinate2D<CoordinateSystem::WC> const& coordinate) const noexcept
{
    auto const tmp{ coordinate.get_transformed(cur_state) };

    return { tmp.get_X(), tmp.get_Y()};
}
