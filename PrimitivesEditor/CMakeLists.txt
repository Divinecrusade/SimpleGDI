cmake_minimum_required (VERSION 3.8)

project ("PrimitivesEditor")

set(SRC_DIR "src")
set(HDR_DIR "include")

set(CMAKE_CXX_ENCODING UTF-8)
set(UNICODE_DEFAULT ON)

include_directories(${HDR_DIR})

add_executable (PrimitivesEditor WIN32 "${SRC_DIR}/WinMain.cpp" "${SRC_DIR}/View.cpp" "${SRC_DIR}/Model.cpp" "${SRC_DIR}/Controller.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PrimitivesEditor PROPERTY CXX_STANDARD 20)
endif()
