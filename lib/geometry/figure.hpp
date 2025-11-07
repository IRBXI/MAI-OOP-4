#pragma once

#include "vec2.hpp"

namespace lib::geometry {

template <std::floating_point T>
class Figure {
  public:
    virtual explicit operator double() const noexcept = 0;
    virtual Vec2<T> Centroid() const noexcept = 0;
};

} // namespace lib::geometry
