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

    Model() noexcept = default;
    Model(Model const&) = delete;
    Model(Model&&) = delete;

    Model& operator=(Model const&) = delete;
    Model& operator=(Model&&) = delete;

    ~Model() = default;

    void zoom(RECT const& window) noexcept;
    void unzoom() noexcept;

    void translate(HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept;

    void add_object(TypeOfPrimitive type, HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end) noexcept;
    void delete_object(HomogeneousCoordinate2D<CoordinateSystem::DC> const& beg, HomogeneousCoordinate2D<CoordinateSystem::DC> const& end, HomogeneousCoordinate2D<CoordinateSystem::DC> const& center) noexcept;

    std::vector<std::pair<TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::DC>, 2U>>> get_objects() const noexcept;

private:

    HomogeneousCoordinate2D<CoordinateSystem::WC> world(HomogeneousCoordinate2D<CoordinateSystem::DC> const& coordinate) const noexcept;

    HomogeneousCoordinate2D<CoordinateSystem::DC> device(HomogeneousCoordinate2D<CoordinateSystem::WC> const& coordinate) const noexcept;

private:
    
    static constexpr RECT VIEWPORT{ AppParams::Canvas::REGION };

    static constexpr Matrices::Matrix<3U, 3U> IDENTITY{ std::array<std::array<double, 3U>, 3U>{1., 0., 0., 0., 1., 0., 0., 0., 1.} };

    Matrices::Matrix<3U, 3U> cur_state{ IDENTITY };
    std::stack<Matrices::Matrix<3U, 3U>> transformations_chain;

    std::vector<std::pair<TypeOfPrimitive, std::array<HomogeneousCoordinate2D<CoordinateSystem::WC>, 2U>>> objects;
};


#endif // !MODEL