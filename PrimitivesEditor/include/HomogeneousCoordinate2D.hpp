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
    HomogeneousCoordinate2D(POINT const& coordinate) noexcept
    :
    point{ { static_cast<double>(coordinate.x) * w, static_cast<double>(coordinate.y) * w, w } },
    X{ point[0][0] },
    Y{ point[0][1] },
    W{ point[0][2] }
    { }
    HomogeneousCoordinate2D(HomogeneousCoordinate2D const& coordinate) noexcept
    :
    point{ coordinate.point },
    X{ point[0][0] },
    Y{ point[0][1] },
    W{ point[0][2] }
    { }
    HomogeneousCoordinate2D(HomogeneousCoordinate2D&&) noexcept = default;

    HomogeneousCoordinate2D& operator=(POINT const& coordinate) noexcept
    {
        point[0][0] = static_cast<double>(coordinate.x) * W;
        point[0][1] = static_cast<double>(coordinate.y) * W;

        return *this;
    }
    HomogeneousCoordinate2D& operator=(HomogeneousCoordinate2D const& coordinate) noexcept
    {
        point[0][0] = coordinate.X;
        point[0][1] = coordinate.Y;

        return *this;
    }
    HomogeneousCoordinate2D& operator=(HomogeneousCoordinate2D&&) = default;

    ~HomogeneousCoordinate2D() = default;

    HomogeneousCoordinate2D& transform(Matrices::Matrix<3U, 3U> const& affine) noexcept
    {
        point *= affine;

        return *this;
    }

    HomogeneousCoordinate2D get_transformed(Matrices::Matrix<3U, 3U> const& affine) const noexcept
    {
        return HomogeneousCoordinate2D{ *this }.transform(affine);
    }

    static HomogeneousCoordinate2D get_transformed(HomogeneousCoordinate2D const& coordinate, Matrices::Matrix<3U, 3U> const& affine) noexcept
    {
        return coordinate.get_transformed(affine);
    }

private:

    Matrices::Matrix<1U, 3U> point;

public:

    double& X;
    double& Y;
    double const& W;
};

#endif // !HOMOGENEOUSCOORDINATE2D
