cmake_minimum_required(VERSION 3.11)
project(c++17-expected
  DESCRIPTION "C++17 std::experimental::expected with functional-style extensions"
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
option(EXPECTED_BUILD_TESTS "Enable tests" ON)

add_library(expected INTERFACE)
target_include_directories(expected
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(EXPECTED_BUILD_TESTS)
  include(CTest)
  include(FetchContent)
  FetchContent_Declare(catch2 URL
    https://github.com/catchorg/Catch2/archive/v2.11.1.zip)
  FetchContent_GetProperties(catch2)
  if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    message("catch2 ${catch2_SOURCE_DIR} ${catch2_BINARY_DIR}")
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
  endif()

  file(GLOB test-sources CONFIGURE_DEPENDS tests/*.cpp)
  list(FILTER test-sources EXCLUDE REGEX "tests/test.cpp")
  add_executable(${PROJECT_NAME}-tests "${test-sources}")
  target_compile_options(${PROJECT_NAME}-tests PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>)

  target_link_libraries(${PROJECT_NAME}-tests
    PRIVATE
      Catch2
      expected)
  add_test(NAME test COMMAND ${PROJECT_NAME}-tests)
endif()
