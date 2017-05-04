#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <vector>
#include <stdexcept>

template<class T>
class SquareMatrix {
public:
    static SquareMatrix eye(int odd_size);

    static SquareMatrix zeros(int odd_size);

    static SquareMatrix ones(int odd_size);

    SquareMatrix(int odd_size, std::initializer_list<T> il);

    SquareMatrix(const SquareMatrix &) = default;

    SquareMatrix(SquareMatrix &&) = default;

    SquareMatrix & operator=(const SquareMatrix &) = default;

    SquareMatrix & operator=(SquareMatrix &&) = default;

    T & operator()(int x, int y) { return values[y * size_ + x]; }

    const T & operator()(int x, int y) const { return values[y * size_ + x]; }

    int size() const { return size_; }

private:
    SquareMatrix(int odd_size, const T & fill_value);

    std::vector<T> values;
    int size_;
};

template<class T>
SquareMatrix<T> SquareMatrix<T>::eye(int odd_size) {
    SquareMatrix matrix(odd_size, static_cast<T>(0));
    for (int i = 0; i < odd_size; ++i) {
        matrix(i, i) = static_cast<T>(1);
    }
    return matrix;
}

template<class T>
SquareMatrix<T> SquareMatrix<T>::zeros(int odd_size) {
    return SquareMatrix(odd_size, static_cast<T>(0));
}

template<class T>
SquareMatrix<T> SquareMatrix<T>::ones(int odd_size) {
    return SquareMatrix(odd_size, static_cast<T>(1));
}

template<class T>
SquareMatrix<T>::SquareMatrix(int odd_size, const T & fill_value)
        : values(odd_size * odd_size, fill_value), size_(odd_size) {
    if (!odd_size % 2) {
        throw std::invalid_argument("Size must be an odd number.");
    }
}

template<class T>
SquareMatrix<T>::SquareMatrix(int odd_size, std::initializer_list<T> il)
        : values(il), size_(odd_size) {
    if (il.size() != odd_size * odd_size) {
        throw std::invalid_argument("Initializer list size_ doesn't match matrix size.");
    }
    if (!odd_size % 2) {
        throw std::invalid_argument("Size must be an odd number.");
    }
}

#endif //SQUARE_MATRIX_H
