project(task_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -O2")

add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(ut ut.cpp)
target_link_libraries(ut gtest gtest_main)
add_test(ut ut)

add_executable(${PROJECT_NAME} main.cpp)
