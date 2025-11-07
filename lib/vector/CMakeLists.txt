project(Vector)


set(HEADERS vector.hpp)
add_library(vector ${HEADERS})

set_target_properties(vector PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(vector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
