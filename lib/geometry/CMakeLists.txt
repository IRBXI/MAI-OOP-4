project(Geometry)

set(HEADERS
    vec2.hpp
    figures/regular_polygon.hpp
)

add_library(geometry ${HEADERS})

set_target_properties(geometry PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/figures)
