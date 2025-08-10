cmake_minimum_required(VERSION 3.2)
project(state)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED "Build as shared library" ON)
set(LIB_TYPE STATIC)
if(BUILD_SHARED)
    set(LIB_TYPE SHARED)
endif()

option(BUILD_TESTS "Build unit tests" ON)

find_package(Boost 1.87 REQUIRED)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(state ${LIB_TYPE} ${LIB_SOURCES})

target_include_directories(state
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(state PUBLIC ${Boost_INCLUDE_DIRS})

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc)

    add_executable(state_tests ${LIB_SOURCES} ${TEST_SOURCES})
    target_link_libraries(state_tests PRIVATE state GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(state_tests)
endif ()

add_executable(state_app main.cpp)
target_link_libraries(state_app PRIVATE state)
target_include_directories(state_app PRIVATE ${Boost_INCLUDE_DIRS})