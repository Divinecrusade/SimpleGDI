#ifndef CARTESIANCOORDINATE2D
#define CARTESIANCOORDINATE2D

#include <Matrix.hpp>

class CartesianCoordinate2D
{
public:

    CartesianCoordinate2D() = delete;
    CartesianCoordinate2D(int x, int y) noexcept
    :
    point{ { x, y } },
    x{ point[0][0] },
    y{ point[0][1] }
    { }
    CartesianCoordinate2D(CartesianCoordinate2D const& coordinate) noexcept
    :
    point{ coordinate.point },
    x{ point[0][0] },
    y{ point[0][1] }
    { }
    CartesianCoordinate2D(CartesianCoordinate2D&&) = delete;    

    CartesianCoordinate2D& operator=(CartesianCoordinate2D const& coordinate) noexcept
    {
        x = coordinate.x;
        y = coordinate.y;
    }
    CartesianCoordinate2D& operator=(CartesianCoordinate2D&&) = delete;

    ~CartesianCoordinate2D() = default;

private:

    Matrices::Matrix<1U, 2U, int> point;

public:

    int& x;
    int& y;
};


#endif // !CARTESIANCOORDINATE2D
