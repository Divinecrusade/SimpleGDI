#include <Matrix.hpp>

#include <iostream>


using namespace Matrices;

template<size_t nRows, size_t nCols, typename T>
void print_matrix(Matrix<nRows, nCols, T> const& matrix)
{
    for (auto const& row : matrix)
    {
        std::copy(row.begin(), row.end(), std::ostream_iterator<T>{std::cout, " "});
        std::cout << "\n";
    }
}

int main()
{
    Matrix<1, 3> M1{ };
    Matrix<1, 3> M2{ {1.0, 2.0, 3.0} };
    Matrix<1, 3> M3{ M2 };

    print_matrix(M1);
    std::cout << "\n";
    print_matrix(M2);
    std::cout << "\n";
    print_matrix(M3);
    std::cout << "\n";
    M3 += M2;
    print_matrix(M3);
    std::cout << "\n";
    print_matrix(M2 + M3);
    std::cout << "\n";
    print_matrix(M3 - M2);
    std::cout << "\n";
    print_matrix(M3.get_transpered());
    std::cout << "\n";
    auto M4{ M3.get_transpered() * M3 };
    print_matrix(M4);
    std::cout << "\n";
    std::cout << M4.get_determinant();
    std::cout << "\n" << "\n";
    M4 = Matrix<3U, 3U>{ { 3., 1., 2., -4., 4., 1., 0., 2., 3. } };
    print_matrix(M4);
    std::cout << "\n" << M4.get_determinant() << "\n" << "\n";
    auto M5{ !M4 };
    print_matrix(M5);
    std::cout << "\n";
    print_matrix(M4 *= M5);
    std::cout << "\n";
    print_matrix(M3 *= 2.);
    std::cout << "\n";
    print_matrix(M3 * (1./2));
    std::cout << "\n";
    print_matrix((1./2) * M3);

    return 0;
}