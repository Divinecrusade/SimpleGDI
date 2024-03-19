#include "Model.hpp"
#include <iterator>


void Model::zoom(RECT const& window) noexcept
{
    double const Sx{ static_cast<double>(max(VIEWPORT.left, VIEWPORT.right) - min(VIEWPORT.left, VIEWPORT.right)) / (max(window.left, window.right) - min(window.left, window.right)) };
    double const Sy{ static_cast<double>(max(VIEWPORT.bottom, VIEWPORT.top) - min(VIEWPORT.bottom, VIEWPORT.top)) / (max(window.bottom, window.top) - min(window.bottom, window.top)) };
    double const Tx{ min(VIEWPORT.left, VIEWPORT.right) - Sx * min(window.left, window.right) };
    double const Ty{ min(VIEWPORT.bottom, VIEWPORT.top) - Sy * min(window.bottom, window.top) };

    Matrices::Matrix<3U, 3U> const transformation
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

void Model::translate(HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept
{
    double const Tx{ end.get_X() - beg.get_X() };
    double const Ty{ end.get_Y() - beg.get_Y() };

    Matrices::Matrix<3U, 3U> const transformation
    {
        {
            1., 0., 0.,
            0., 1., 0.,
            Tx, Ty, 1.
        }
    };
    cur_state *= transformation;
}

void Model::add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept
{
    objects.emplace_back(type, std::array<HomogeneousCoordinate2D<CoordinateSystem::WC>, 2U>{ world(beg), world(end) });
}

void Model::delete_object(HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end, HomogeneousCoordinate2D<CoordinateSystem::DC> const& center) noexcept
{
    auto beg_{ world(beg) };
    auto end_{ world(end) };
    auto center_{ world(center) };

    auto it{ objects.rbegin() };
    for (; it != objects.rend(); ++it)
    {
        switch (it->first)
        {
            case TypeOfPrimitive::LINE: 
            {
                if ((it->second[0U].get_X() > beg_.get_X() && it->second[0U].get_X() < end_.get_X() &&
                    it->second[0U].get_Y() > beg_.get_Y() && it->second[0U].get_Y() < end_.get_Y()) ||
                    (it->second[1U].get_X() > beg_.get_X() && it->second[1U].get_X() < end_.get_X() &&
                    it->second[1U].get_Y() > beg_.get_Y() && it->second[1U].get_Y() < end_.get_Y()))
                {
                    goto OUT_LOOP;
                }
            }
            break;
            
            case TypeOfPrimitive::RECTANGLE: 
            {
                //HomogeneousCoordinate2D<CoordinateSystem::WC> const center{ beg_.get_X() + (end_.get_X() - beg_.get_X()) / 2., beg_.get_Y() + (end_.get_Y() - beg_.get_Y()) / 2. };
                double const left{ min(it->second[0U].get_X(), it->second[1U].get_X()) };
                double const right{ max(it->second[0U].get_X(), it->second[1U].get_X())};
                double const bottom{ min(it->second[0U].get_Y(), it->second[0U].get_Y()) };
                double const top{ max(it->second[0U].get_Y(), it->second[1U].get_Y()) };

                if (center_.get_X() > left && center_.get_X() < right && center_.get_Y() < top && center_.get_Y() > bottom)
                {
                    goto OUT_LOOP;
                }
            }
            break;
            
            default: break;
        }

    }
OUT_LOOP:
    if (it != objects.rend()) objects.erase(std::prev(it.base()));
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
