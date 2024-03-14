#ifndef MODEL
#define MODEL

#include "HomogeneousCoordinate2D.hpp"
#include "AppParams.hpp"

#include <windows.h>
#include <vector>
#include <stack>


class Model
{
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

    void zoom(RECT const& window) noexcept;
    void unzoom() noexcept;

    void add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept;

    std::vector<std::pair<TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> get_objects() const noexcept;

private:

    HomogeneousCoordinate2D<CoordinateSystem::NDC> normalize(HomogeneousCoordinate2D<CoordinateSystem::DC> const& coordinate) const noexcept;

    HomogeneousCoordinate2D<CoordinateSystem::DC>  device(HomogeneousCoordinate2D<CoordinateSystem::NDC> const& coordinate) const noexcept;

private:
    
    static constexpr RECT VIEWPORT{ AppParams::Canvas::REGION };

    static constexpr double MIN_X{ 0. };
    static constexpr double MAX_X{ static_cast<double>(AppParams::Canvas::WIDTH) };

    static constexpr double MIN_Y{ 0. };
    static constexpr double MAX_Y{ static_cast<double>(AppParams::Canvas::HEIGHT) };

    RECT window{ VIEWPORT };

    std::vector<std::pair<TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::NDC>, 2U>>> objects;
};


#endif // !MODEL