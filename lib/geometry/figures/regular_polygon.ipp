#pragma once

#include "regular_polygon.hpp"
#include <ostream>

namespace lib::geometry {

template <std::floating_point T, std::size_t N>
RegularPolygon<T, N>::RegularPolygon() noexcept {
    side_ = 0.0;
    for (std::size_t i = 0; i < 5; ++i) {
        points_[i] = {0.0, 0.0};
    }
}

template <std::floating_point T, std::size_t N>
RegularPolygon<T, N>::RegularPolygon(const Vec2<T>& radius_vector,
                                     const Vec2<T>& guiding_vector) noexcept {
    side_ = guiding_vector.Length();
    Vec2<T> cur_point = radius_vector;
    Vec2<T> guide = guiding_vector;
    for (std::size_t i = 0; i < N; ++i) {
        points_[i] = cur_point;
        cur_point += guide;
        guide = Vec2<T>::Rotate(guide, ROTATION_ANGLE);
    }
}

template <std::floating_point T, std::size_t N>
RegularPolygon<T, N>::operator double() const noexcept {
    double area = 0.25 * N * side_ * side_ * (1.0 / std::tan(M_PI / N));
    return area;
}

template <std::floating_point T, std::size_t N>
Vec2<T> RegularPolygon<T, N>::Centroid() const noexcept {
    return Vec2<T>::GetCentroid(N, points_);
}

template <std::floating_point T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const RegularPolygon<T, N>& pol) {
    os << "Regular polygon with " << N << " sides:\n";
    for (std::size_t i = 0; i < N; ++i) {
        os << pol.points_[i];
    }
    os << std::endl;
    return os;
}

template <std::floating_point T, std::size_t N>
std::istream& operator>>(std::istream& is, RegularPolygon<T, N>& pol) {
    Vec2<T> radius_vector, guiding_vector;
    is >> radius_vector >> guiding_vector;

    pol = RegularPolygon<T, N>(radius_vector, guiding_vector);

    return is;
}

} // namespace lib::geometry
