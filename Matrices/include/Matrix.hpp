#ifndef MATRIX
#define MATRIX

#include <array>
#include <algorithm>
#include <cassert>
#include <cmath>


namespace Matrices
{
    static constexpr double PRECISION{ 1e-7 };
    __forceinline constexpr bool is_equal_with_precision(double a, double b)
    {
        return std::fabs(a - b) <= PRECISION;
    }


    template<size_t nCols, size_t nRows, typename T = double>
    class Matrix
    {
    public:

        constexpr Matrix() noexcept = default;
        constexpr Matrix(std::array<std::array<T, nRows>, nCols> const& data) noexcept
        :
        data{ data }
        { }
        Matrix(std::array<std::array<T, nRows>, nCols>&& tmp_data) noexcept
        :
        data{ data }
        { }
        constexpr Matrix(Matrix const& matrix) noexcept
        :
        matrix{ matrix }
        { }
        Matrix(Matrix&& tmp_matrix) noexcept
        {
            std::swap(data, tmp_matrix.data);
        }

        constexpr Matrix& operator=(Matrix const& other_matrix) noexcept
        {
            if (&other_matrix != this)
            {
                data = other_matrix.data;
            }

            return *this;
        }
        Matrix& operator=(Matrix&& tmp_matrix) noexcept
        {
            std::swap(data, tmp_matrix.data);

            return *this;
        }

        ~Matrix() = default;

        std::array<T, nRows> const& operator[](size_t i) const
        {
            assert(i <= nCols);

            return data[i];
        }
        std::array<T, nRows>& operator[](size_t i)
        {
            assert(i <= nCols);

            return data[i];
        }

        Matrix<nRows, nCols, T> operator!() const noexcept
        {
            Matrix tmp{ };

            double const D{ get_determinant() };
            assert(!is_equal_with_precision(D, 0));

            for (size_t i{ 0U }; i != nCols; ++i)
            for (size_t j{ 0U }; j != nRows; ++j)
            {
                tmp[i][j] = get_co_factor(i, j) / D;
            }

            return tmp.get_transpered();
        }

        std::array<std::array<T, nCols>, nRows> get_transpered() const noexcept
        {
            std::array<std::array<T, nCols>, nRows> tmp{ };

            for (size_t i{ 0U }; i != nRows; ++i)
            for (size_t j{ 0U }; j != nCols; ++j)
            {
                tmp[i][j] = data[j][i];
            }

            return tmp;
        }
        double get_determinant() const noexcept
        {
            double D{ 1. };
            Matrix B{ *this };

            for (size_t I{ 0U }; I != nCols - 1U; ++I)
            {
                if (!(is_equal_with_precision(B[I][I], 0.)))
                {
                    size_t i{ };
                    for (i = I + 1U; i != nCols; ++i)
                      if (is_equal_with_precision(B[i][I])) break;


                    for (size_t j{ 0U }; j != nRows; ++j)
                    {
                        std::swap(B[I][j], B[i][j]);
                    }

                    D *= -1.;
                }

                for (size_t i{ I + 1U }; i != nCols; ++i)
                {
                    T const k{ -(B[i][I] / B[I][I]) };

                    for (size_t R{ I }; R != nRows; ++R)
                    {
                        B[i][R] += B[I][R] * k;
                    }
                }
            }

            for (size_t i{ 0U }; i != nCols; ++i)
            {
                D *= B[i][i];
            }

            return D;
        }

    private:

        T get_co_factor(size_t I, size_t J) const noexcept
        {
            Matrix<(nCols - 1U != 0U ? nCols - 1U : nCols), (nRows - 1U != 0U ? nRows - 1U : nRows), T> minor_matrix{ };

            for (size_t i1{ 0U }, i2{ 0U }; i2 < nCols - 1U; ++i1, ++i2)
            {
                if (i1 == I) ++i;

                for (size_t j1{ 0U }, j2{ 0U }; j2 < nRows - 1U; ++j1, ++j2)
                {
                    if (j1 == J) ++j1;
                    minor_matrix[i2][j2] = *this[i1][j1];
                }
            }

            return pow(-1., I + J) * minor_matrix.get_determinant();
        }

    private:

        std::array<std::array<T, nRows>, nCols> data;
    };
}

#endif // !MATRIX
