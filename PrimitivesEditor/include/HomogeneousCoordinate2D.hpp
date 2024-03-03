#ifndef HOMOGENEOUSCOORDINATE2D
#define HOMOGENEOUSCOORDINATE2D

#include <CartesianCoordinate2D.hpp>

template<double w = 1.>
class HomogeneousCoordinate2D
{
public:

    HomogeneousCoordinate2D() = delete;
    HomogeneousCoordinate2D(CartesianCoordinate2D const& coordinate) noexcept
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
    HomogeneousCoordinate2D(HomogeneousCoordinate2D&&) = delete;

    HomogeneousCoordinate2D& operator=(CartesianCoordinate2D const& coordinate) noexcept
    {
        point[0][0] = static_cast<double>(coordinate.x) * W;
        point[0][1] = static_cast<double>(coordinate.y) * W;
    }
    HomogeneousCoordinate2D& operator=(HomogeneousCoordinate2D const& coordinate) noexcept
    {
        point[0][0] = coordinate.X;
        point[0][1] = coordinate.Y;
    }
    HomogeneousCoordinate2D& operator=(HomogeneousCoordinate2D&& coordinate) = delete;

    ~HomogeneousCoordinate2D() = default;

    CartesianCoordinate2D convert_to_cartesian() const noexcept
    {
        return CartesianCoordinate2D{ static_cast<int>(X / W), static_cast<int>(Y / W) };
    }

private:

    Matrices::Matrix<1U, 3U> point;

public:

    double& X;
    double& Y;
    double const& W;
};

#endif // !HOMOGENEOUSCOORDINATE2D
