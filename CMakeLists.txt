cmake_minimum_required(VERSION 3.2)
project(zend)

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

add_library(zend ${LIB_TYPE} ${LIB_SOURCES})

target_include_directories(zend
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(zend PUBLIC ${Boost_INCLUDE_DIRS})

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

    add_executable(zend_tests ${LIB_SOURCES} ${TEST_SOURCES})
    target_link_libraries(zend_tests PRIVATE zend GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(zend_tests)
endif ()

add_executable(zend_service ${CMAKE_CURRENT_SOURCE_DIR}/binaries/service.cpp)
target_link_libraries(zend_service PRIVATE zend)
target_include_directories(zend_service PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(zend_instance ${CMAKE_CURRENT_SOURCE_DIR}/binaries/instance.cpp)
target_link_libraries(zend_instance PRIVATE zend)
target_include_directories(zend_instance PRIVATE ${Boost_INCLUDE_DIRS})