cmake_minimum_required(VERSION 3.17)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
   set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

project(essence-emulator-v2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros/")

include(GroupSources)
include(AutoCollect)
include(FindMySQL)

include_directories(${MYSQL_INCLUDE_DIRS})

IF (WIN32)
	add_definitions(-D_WIN32_WINNT=0x0601)
	set(CMAKE_GENERATOR_PLATFORM "Visual Studio 16 2019 -A Win32")
ENDIF()

set(Boost_USE_MULTITHREADED ON)

#vcpkg

find_package(Boost COMPONENTS system regex filesystem thread REQUIRED)
find_package(spdlog CONFIG REQUIRED)

#tnl config

add_definitions(-DTNL_DEBUG)
add_definitions(-DTNL_ENABLE_LOGGING)

file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX})

add_subdirectory(dependencies)

include_directories(${CMAKE_SOURCE_DIR}/dependencies)

add_subdirectory(src)