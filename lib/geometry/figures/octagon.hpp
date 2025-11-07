#pragma once

#include "regular_polygon.hpp"

namespace lib::geometry {

template <std::floating_point T>
using Octagon = RegularPolygon<T, 8>;

} // namespace lib::geometry
