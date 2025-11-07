#pragma once

#include <concepts>
#include <istream>
#include <ostream>

namespace lib::geometry {

// T is only a floating type and not also integral
// because most of the functions associated with Vec2
// don't make any sense when used with integral types
// Such functions are: Length(), Normalize(), Rotate(), GetCentroid()
// Most of the resulting Vec2 of these functions won't have integral coordinates
// even if the given Vec2 had them
// Integral coordinates for some figures also don't make sense
// since there doesn't exist any right pentagon with integral coordinates
template <std::floating_point T>
struct Vec2 {
  private:
    static constexpr T EPS = static_cast<T>(1e-6);

  public:
    T x = 0.0;
    T y = 0.0;

    T Length() const noexcept;
    void Normalize() noexcept;

    static Vec2 Rotate(const Vec2& v, T angle_radians) noexcept;

    // Finds the centroid of the array of points
    // where n is the size of the array
    static Vec2 GetCentroid(std::size_t n, const Vec2* arr) noexcept;

    Vec2& operator+=(const Vec2& rhs) noexcept;
    Vec2 operator+(const Vec2& rhs) noexcept;
    Vec2& operator-=(const Vec2& rhs) noexcept;
    Vec2 operator-(const Vec2& rhs) noexcept;
    Vec2& operator*=(const T v) noexcept;
    Vec2 operator*(const T v) noexcept;
    Vec2& operator/=(const T v) noexcept;
    Vec2 operator/(const T v) noexcept;
    bool operator==(const Vec2& rhs) const noexcept;
    bool operator!=(const Vec2& rhs) const noexcept;

    template <std::floating_point T1>
    friend std::ostream& operator<<(std::ostream& os, const Vec2<T1>& vec);

    template <std::floating_point T1>
    friend std::istream& operator>>(std::istream& is, Vec2<T1>& vec);
};

} // namespace lib::geometry

#include "vec2.ipp"
