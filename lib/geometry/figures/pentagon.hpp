#pragma once

#include "regular_polygon.hpp"

namespace lib::geometry {

template <std::floating_point T>
using Pentagon = RegularPolygon<T, 5>;

} // namespace lib::geometry
