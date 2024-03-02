cmake_minimum_required (VERSION 3.8)

project ("Matrices")

set(SRC_DIR "src")
set(HDR_DIR "include")

include_directories(${HDR_DIR})

add_executable (Matrices "${SRC_DIR}/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Matrices PROPERTY CXX_STANDARD 20)
endif()
