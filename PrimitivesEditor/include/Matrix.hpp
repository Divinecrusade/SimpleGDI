#ifndef MATRIX
#define MATRIX

#include <array>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>


namespace Matrices
{
    static constexpr double PRECISION{ 1e-7 };
    __forceinline bool is_equal_with_precision(double a, double b)
    {
        return std::fabs(a - b) <= PRECISION;
    }

    template<size_t nRows1, size_t nCols1, size_t nRows2, size_t nCols2>
    concept MultableSizes = nCols1 == nRows2;

    template<size_t nCols, size_t nRows>
    concept SquareSizes = nCols == nRows;

    template<size_t nRows, size_t nCols, typename T = double>
    class Matrix
    {
    private:

        std::array<std::array<T, nCols>, nRows> data;

    public:

        constexpr Matrix() noexcept = default;
        constexpr Matrix(std::array<std::array<T, nCols>, nRows> const& data) noexcept
        :
        data{ data }
        { }
        Matrix(std::array<std::array<T, nCols>, nRows>&& tmp_data) noexcept
        :
        data{ tmp_data }
        { }
        constexpr Matrix(Matrix const& matrix) noexcept
        :
        data{ matrix.data }
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

        auto begin() noexcept -> decltype(data.begin())
        {
            return data.begin();
        }
        auto begin() const noexcept -> decltype(data.cbegin())
        {
            return data.cbegin();
        }

        auto end() noexcept -> decltype(data.end())
        {
            return data.end();
        }
        auto end() const noexcept -> decltype(data.cend())
        {
            return data.cend();
        }
        
        std::array<T, nCols> const& operator[](size_t i) const
        {
            assert(i <= nRows);

            return data[i];
        }
        std::array<T, nCols>& operator[](size_t i)
        {
            assert(i <= nRows);

            return data[i];
        }

        Matrix operator!() const noexcept
        {
            Matrix tmp{ };

            double const D{ get_determinant() };
            assert(!is_equal_with_precision(D, 0));

            for (size_t i{ 0U }; i != nRows; ++i)
            for (size_t j{ 0U }; j != nCols; ++j)
            {
                tmp[i][j] = get_co_factor(i, j) / D;
            }

            return tmp.get_transpered();
        }

        Matrix& operator+=(Matrix const& rhs) noexcept
        {
            std::transform((*this).begin(), (*this).end(), rhs.begin(), data.begin(),
            [](auto const& col1, auto const& col2)
            {
                std::array<T, nCols> new_row{ };

                std::transform(col1.begin(), col1.end(), col2.begin(), new_row.begin(), std::plus<T>{});

                return new_row;
            });

            return *this;
        }
        Matrix& operator-=(Matrix const& rhs) noexcept
        {
            std::transform(data.begin(), data.end(), rhs.begin(), data.begin(),
            [](auto const& col1, auto const& col2)
            {
                std::array<T, nCols> new_row{ };

                std::transform(col1.begin(), col1.end(), col2.begin(), new_row.begin(), std::minus<T>{});

                return new_row;
            });

            return *this;
        }
        template<size_t nRows2, size_t nCols2>
        requires MultableSizes<nRows, nCols, nRows2, nCols2>
        Matrix& operator*=(Matrix<nRows2, nCols2, T> const& rhs) noexcept
        {
            return (*this) = Matrix{ *this } * rhs;
        }
        Matrix& operator*=(T const& k) noexcept
        {
            for (auto& row : (*this))
            for (auto& col : row)
            {
                col *= k;
            }

            return *this;
        }

        Matrix<nCols, nRows, T> get_transpered() const noexcept
        {
            Matrix<nCols, nRows, T> transpered{ };

            for (size_t i{ 0U }; i != nCols; ++i)
            for (size_t j{ 0U }; j != nRows; ++j)
            {
                transpered[i][j] = data[j][i];
            }

            return transpered;
        }
        double get_determinant() const noexcept
        {
            double D{ 1. };
            Matrix triangled{ *this };

            for (size_t I{ 0U }; I != nRows - 1U; ++I)
            {
                if (is_equal_with_precision(triangled[I][I], 0.))
                {
                    for (size_t i{ I + 1U }; i != nRows; ++i)
                    {
                        if (!is_equal_with_precision(triangled[i][I], 0.))
                        {
                            std::swap(triangled[I], triangled[i]);

                            break;
                        }
                    }

                    D *= -1.;
                }

                for (size_t i{ I + 1U }; i != nRows; ++i)
                {
                    if (is_equal_with_precision(triangled[I][I], 0.)) return 0.;

                    T const k{ -(triangled[i][I] / triangled[I][I]) };

                    for (size_t R{ I }; R != nCols; ++R)
                        triangled[i][R] += triangled[I][R] * k;
                }
            }

            for (size_t i{ 0U }; i != nRows; ++i)
                D *= triangled[i][i];

            return D;
        }

    private:

        T get_co_factor(size_t I, size_t J) const noexcept
        {
            Matrix<(nRows - 1U != 0U ? nRows - 1U : nRows), (nCols - 1U != 0U ? nCols - 1U : nCols), T> minor_matrix{ };

            for (size_t i1{ 0U }, i2{ 0U }; i2 < nRows - 1U; ++i1, ++i2)
            {
                if (i1 == I) ++i1;

                for (size_t j1{ 0U }, j2{ 0U }; j2 < nCols - 1U; ++j1, ++j2)
                {
                    if (j1 == J) ++j1;
                    minor_matrix[i2][j2] = (*this)[i1][j1];
                }
            }

            return pow(-1., I + J) * minor_matrix.get_determinant();
        }
    };


    template<size_t nRows, size_t nCols, typename T>
    Matrix<nRows, nCols, T> operator+(Matrix<nRows, nCols, T> const& lhs, Matrix<nRows, nCols, T> const& rhs) noexcept
    {
        return Matrix{ lhs } += rhs;
    }

    template<size_t nRows, size_t nCols, typename T>
    Matrix<nRows, nCols, T> operator-(Matrix<nRows, nCols, T> const& lhs, Matrix<nRows, nCols, T> const& rhs) noexcept
    {
        return Matrix{ lhs } -= rhs;
    }

    template<size_t nRows1, size_t nCols1, size_t nCols2, typename T>
    Matrix<nRows1, nCols2, T> operator*(Matrix<nRows1, nCols1, T> const& lhs, Matrix<nCols1, nCols2, T> const& rhs) noexcept
    {
        Matrix<nRows1, nCols2, T> result{ };

        for (size_t i{ 0U }; i != nRows1; ++i)
        for (size_t j{ 0U }; j != nCols2; ++j)
        {
            result[i][j] = static_cast<T>(0);

            for (size_t k{ 0U }; k != nCols1; ++k)
                result[i][j] += lhs[i][k] * rhs[k][j];
        }

        return result;
    }
    
    template<size_t nRows, size_t nCols, typename T>
    Matrix<nRows, nCols, T> operator*(Matrix<nRows, nCols, T> const& lhs, T const& rhs)
    {
        return Matrix{ lhs } *= rhs;
    }

    template<size_t nRows, size_t nCols, typename T>
    Matrix<nRows, nCols, T> operator*(T const& lhs, Matrix<nRows, nCols, T> const& rhs)
    {
        return Matrix{ rhs } *= lhs;
    }
}

#endif // !MATRIX
