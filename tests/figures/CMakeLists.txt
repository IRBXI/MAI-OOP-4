project(FiguresTests)

add_executable(regular_polygons ../test_main.cpp regular_polygons.cpp)
target_link_libraries(regular_polygons geometry vector gtest_main)
