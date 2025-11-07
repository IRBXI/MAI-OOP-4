project(Tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(figures)

add_executable(vector_unit vector.cpp)
target_link_libraries(vector_unit vector gtest_main)

add_executable(unit test_main.cpp vector.cpp figures/regular_polygons.cpp)
target_link_libraries(unit geometry vector gtest_main)
