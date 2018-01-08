//
// Created by Piotr Sykulski on 05/01/2018.
//

#ifndef POBR_PROJEKT_KERNELS_HPP
#define POBR_PROJEKT_KERNELS_HPP

#include <array>

template<class T, size_t Rows, size_t Cols> using matrix = std::array<std::array<T, Cols>, Rows>;

const matrix<int, 3, 3> sobelx {{ {{-1, 0, 1}}, {{-2, 0, 2}}, {{-1, 0, 1}} }};
const matrix<int, 3, 3> sobely {{ {{1, 2, 1}}, {{0, 0, 0}}, {{-1, -2, -1}} }};

#endif //POBR_PROJEKT_KERNELS_HPP
