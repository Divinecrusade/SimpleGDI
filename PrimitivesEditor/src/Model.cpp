#include "Model.hpp"
#include <iterator>


void Model::zoom(RECT const& new_window) noexcept
{
    window = new_window;
}

void Model::unzoom() noexcept
{
}

void Model::add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept
{
    objects.emplace_back(type, std::array<HomogeneousCoordinate2D<CoordinateSystem::NDC>, 2U>{ normalize(beg), normalize(end) });
}

std::vector<std::pair<Model::TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> Model::get_objects() const noexcept
{
    std::vector<std::pair<Model::TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> render_objects{ };

    for (auto const& obj : objects)
    {
        render_objects.emplace_back(obj.first, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>{ device(obj.second[0U]), device(obj.second[1U]) });
    }

    return render_objects;
}

HomogeneousCoordinate2D<CoordinateSystem::NDC> Model::normalize(HomogeneousCoordinate2D<CoordinateSystem::DC> const& coordinate) const noexcept
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

    auto tmp{ coordinate.get_transformed(!transformation) };

    return { tmp.X, tmp.Y };
}

HomogeneousCoordinate2D<CoordinateSystem::DC> Model::device(HomogeneousCoordinate2D<CoordinateSystem::NDC> const& coordinate) const noexcept
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

    auto tmp{ coordinate.get_transformed(transformation) };

    return { tmp.X, tmp.Y };
}
