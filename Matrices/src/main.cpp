#include <Matrix.hpp>

#include <iostream>


using namespace Matrices;

template<size_t nCols, size_t nRows, typename T>
void print_matrix(Matrix<nCols, nRows, T> const& matrix)
{
    for (auto const& col : matrix)
    {
        std::copy(col.begin(), col.end(), std::ostream_iterator<T>{std::cout, " "});
        std::cout << "\n";
    }
}

int main()
{
    Matrix<3, 1> M1{ };

    print_matrix(M1);

    return 0;
}