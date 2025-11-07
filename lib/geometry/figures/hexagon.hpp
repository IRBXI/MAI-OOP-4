#pragma once

#include "regular_polygon.hpp"

namespace lib::geometry {

template <std::floating_point T>
using Hexagon = RegularPolygon<T, 6>;

} // namespace lib::geometry
