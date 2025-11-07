#pragma once

#include "vec2.hpp"

#include <cmath>
#include <concepts>

namespace lib::geometry {

template <std::floating_point T>
T Vec2<T>::Length() const noexcept {
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

template <std::floating_point T>
void Vec2<T>::Normalize() noexcept {
    T len = Length();
    x /= len;
    y /= len;
}

template <std::floating_point T>
Vec2<T> Vec2<T>::Rotate(const Vec2& v, T angle_radians) noexcept {
    T new_x = v.x * std::cos(angle_radians) - v.y * std::sin(angle_radians);
    T new_y = v.x * std::sin(angle_radians) + v.y * std::cos(angle_radians);
    return {new_x, new_y};
}

template <std::floating_point T>
Vec2<T> Vec2<T>::GetCentroid(std::size_t n, const Vec2<T>* arr) noexcept {
    Vec2<T> sum;
    for (std::size_t i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum / static_cast<T>(n);
}

template <std::floating_point T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& rhs) noexcept {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

template <std::floating_point T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) noexcept {
    Vec2<T> res = *this;
    res += rhs;
    return res;
}

template <std::floating_point T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& rhs) noexcept {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

template <std::floating_point T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) noexcept {
    Vec2<T> res = *this;
    res -= rhs;
    return res;
}

template <std::floating_point T>
Vec2<T>& Vec2<T>::operator*=(const T v) noexcept {
    this->x *= v;
    this->y *= v;
    return *this;
}

template <std::floating_point T>
Vec2<T> Vec2<T>::operator*(const T v) noexcept {
    Vec2<T> res = *this;
    res *= v;
    return res;
}

template <std::floating_point T>
Vec2<T>& Vec2<T>::operator/=(const T v) noexcept {
    this->x /= v;
    this->y /= v;
    return *this;
}

template <std::floating_point T>
Vec2<T> Vec2<T>::operator/(const T v) noexcept {
    Vec2<T> res = *this;
    res /= v;
    return res;
}

template <std::floating_point T>
bool Vec2<T>::operator==(const Vec2<T>& rhs) const noexcept {
    return std::abs(x - rhs.x) < EPS and std::abs(y - rhs.y) < EPS;
}

template <std::floating_point T>
bool Vec2<T>::operator!=(const Vec2<T>& rhs) const noexcept {
    return !(*this == rhs);
}

template <std::floating_point T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec) {
    os << '(' << vec.x << ", " << vec.y << ')' << std::endl;
    return os;
}

template <std::floating_point T>
std::istream& operator>>(std::istream& is, Vec2<T>& vec) {
    is >> vec.x >> vec.y;
    return is;
}

} // namespace lib::geometry
