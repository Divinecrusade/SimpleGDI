#ifndef HOMOGENEOUSCOORDINATE2D
#define HOMOGENEOUSCOORDINATE2D

#include "Matrix.hpp"
#include "CoordinateSystem.hpp"

#include <Windows.h>


template<CoordinateSystem C, double w = 1.>
class HomogeneousCoordinate2D
{
public:

    HomogeneousCoordinate2D() = delete;
    HomogeneousCoordinate2D(double x, double y) noexcept
    :
    point{ { x * w, y * w, w } }
    { }
    HomogeneousCoordinate2D(POINT const& coordinate) noexcept
    :
    point{ { static_cast<double>(coordinate.x) * w, static_cast<double>(coordinate.y) * w, w } }
    { }
    HomogeneousCoordinate2D(HomogeneousCoordinate2D const&) noexcept = default;
    HomogeneousCoordinate2D(HomogeneousCoordinate2D&&) noexcept = default;

    HomogeneousCoordinate2D& operator=(POINT const& coordinate) noexcept
    {
        point[0][0] = static_cast<double>(coordinate.x) * get_W();
        point[0][1] = static_cast<double>(coordinate.y) * get_W();

        return *this;
    }
    HomogeneousCoordinate2D& operator=(HomogeneousCoordinate2D const& coordinate) noexcept
    {
        point[0][0] = coordinate.X;
        point[0][1] = coordinate.Y;

        return *this;
    }
    HomogeneousCoordinate2D& operator=(HomogeneousCoordinate2D&&) = default;

    ~HomogeneousCoordinate2D() noexcept = default;

    POINT convert() const noexcept
    {
        return POINT{ static_cast<int>(get_X() / get_W()), static_cast<int>(get_Y() / get_W()) };
    }

    HomogeneousCoordinate2D& transform(Matrices::Matrix<3U, 3U> const& affine) noexcept
    {
        point *= affine;

        return *this;
    }

    HomogeneousCoordinate2D get_transformed(Matrices::Matrix<3U, 3U> const& affine) const noexcept
    {
        return HomogeneousCoordinate2D{ *this }.transform(affine);
    }

    double& get_X() noexcept
    {
        return point[0U][INDEX_X];
    }
    double get_X() const noexcept
    {
        return point[0U][INDEX_X];
    }

    double& get_Y() noexcept
    {
        return point[0U][INDEX_Y];
    }
    double get_Y() const noexcept
    {
        return point[0U][INDEX_Y];
    }

    double& get_W() noexcept
    {
        return point[0U][INDEX_W];
    }
    double get_W() const noexcept
    {
        return point[0U][INDEX_W];
    }

    static HomogeneousCoordinate2D get_transformed(HomogeneousCoordinate2D const& coordinate, Matrices::Matrix<3U, 3U> const& affine) noexcept
    {
        return coordinate.get_transformed(affine);
    }

private:

    static constexpr size_t INDEX_X{ 0U };
    static constexpr size_t INDEX_Y{ 1U };
    static constexpr size_t INDEX_W{ 2U };

    Matrices::Matrix<1U, 3U> point;
};

#endif // !HOMOGENEOUSCOORDINATE2D
