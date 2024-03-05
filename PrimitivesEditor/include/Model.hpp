#include <HomogeneousCoordinate2D.hpp>

#include <windows.h>
#include <vector>


class Model
{
public:

    enum class TypeOfPrimitive
    {
        LINE, RECTANGLE, ELLIPSE
    };

private:

    struct Primitive
    {
        TypeOfPrimitive type;
        std::array<HomogeneousCoordinate2D<1.>, 2U> points;
    };

public:

    Model() = default;
    Model(Model const&) = delete;
    Model(Model&&) = delete;

    Model& operator=(Model const&) = delete;
    Model& operator=(Model&&) = delete;

    ~Model() = default;

    void add(TypeOfPrimitive type, std::array<CartesianCoordinate2D, 2U> const& points, int canvas_width, int canvas_height) noexcept;

private:

    std::vector<Primitive> objects;
};